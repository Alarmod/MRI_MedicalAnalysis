#include <iostream>
#include <algorithm>
#include <numeric>

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

#include <opencv2/opencv.hpp>

#include <onnxruntime_cxx_api.h>

#define half Ort::Float16_t

template <typename T>
size_t VectorProduct(const std::vector<T>& v)
{
  return static_cast<size_t>(std::accumulate(v.begin(), v.end(), 1u, std::multiplies<T>()));
};

#define USE_SIGMOID_FUNC true

struct OutputParams
{
  int id;
  float confidence;
  cv::Rect box;
  cv::Mat boxMask;
};

struct MaskParams
{
  cv::Size srcImgShape;
  cv::Vec4d params;
};

class ONNXEnvironment
{
private:
  Ort::Env* createEnvironment(unsigned int intra_op_threads)
  {
    Ort::ThreadingOptions tp_options;
    tp_options.SetGlobalIntraOpNumThreads(intra_op_threads);
    tp_options.SetGlobalInterOpNumThreads(1);
    return new Ort::Env(tp_options, ORT_LOGGING_LEVEL_WARNING, "Default");
  }
public:
  ONNXEnvironment(unsigned int threadPoolSize)
  {
    m_threadPoolSize = threadPoolSize;
    m_ortEnvPtr = createEnvironment(threadPoolSize);
    m_ref_count = 0;
  }

  ~ONNXEnvironment()
  {
    delete m_ortEnvPtr;
  }

  bool setGlobalThreadPoolSize(unsigned int threadPoolSize)
  {
    if (m_ref_count == 0)
    {
      if (m_threadPoolSize == threadPoolSize)
        return true;
      m_threadPoolSize = threadPoolSize;
      delete m_ortEnvPtr;
      m_ortEnvPtr = createEnvironment(m_threadPoolSize);
      return true;
    }
    return false;
  }

  void acquire()
  {
    m_ref_count++;
  }

  void release()
  {
    m_ref_count--;
  }

  const Ort::Env& getEnvironment()
  {
    return *m_ortEnvPtr;
  }
private:
  unsigned int m_threadPoolSize;
  int 	       m_ref_count;
  Ort::Env* m_ortEnvPtr = nullptr;
};

//use singleton
ONNXEnvironment& getONNXEnv()
{
  static ONNXEnvironment instance(std::thread::hardware_concurrency());
  return instance;
}

bool setONNXGlobalThreadPoolSize(int threadPoolSize)
{
  return getONNXEnv().setGlobalThreadPoolSize(threadPoolSize);
}

struct YOLO
{
private:
  bool isONNXProviderAvailable(const std::string& provider_name)
  {
    std::vector<std::string> available_providers = Ort::GetAvailableProviders();
    return (std::find(available_providers.begin(), available_providers.end(), provider_name) != available_providers.end());
  }
public:
  YOLO(const std::string& name, const std::string& net_path, const bool use_gpu, const bool use_fp16, const unsigned int net_width, const unsigned int net_height, const unsigned int cudaID = 0)
  {
    m_name = name;
    m_net_path = net_path;

    m_use_gpu = use_gpu;
    m_use_fp16 = use_fp16;
    m_net_width = net_width;
    m_net_height = net_height;

    ONNXEnvironment& env = getONNXEnv();

    Ort::SessionOptions session_options;
    session_options.DisablePerSessionThreads();
    session_options.SetExecutionMode(ExecutionMode::ORT_SEQUENTIAL);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

    if (m_use_gpu)
    {
      try
      {
        if (isONNXProviderAvailable("CUDAExecutionProvider"))
        {
          OrtCUDAProviderOptions provider_options;
          provider_options.device_id = cudaID;

          session_options.AppendExecutionProvider_CUDA(provider_options);
        }
        else
        {
          std::cout << "Warning: CUDAExecutionProvider not available" << std::endl;
        }
      }
      catch (std::exception& ex)
      {
        std::cout << "Warning: " << ex.what() << std::endl;
      }
    }

#ifdef _WIN32
    std::wstring model_path(net_path.begin(), net_path.end());
    m_session = new Ort::Session(env.getEnvironment(), model_path.c_str(), session_options);
#else
    m_session = new Ort::Session(env.getEnvironment(), net_path.c_str(), session_options);
#endif

    Ort::AllocatorWithDefaultOptions allocator;

    //init input
    size_t _inputNodesNum = m_session->GetInputCount();
    m_input_name = std::move(m_session->GetInputNameAllocated(0, allocator));

    m_inputs_names.push_back(m_input_name.get());

    Ort::TypeInfo inputTypeInfo = m_session->GetInputTypeInfo(0);
    auto input_tensor_info = inputTypeInfo.GetTensorTypeAndShapeInfo();
    ONNXTensorElementDataType _inputNodeDataType = input_tensor_info.GetElementType();
    m_input_tensor_shape = input_tensor_info.GetShape();

    //dynamic shape model init
    if (m_input_tensor_shape[0] == -1)
      m_input_tensor_shape[0] = 1;

    if (m_input_tensor_shape[2] == -1 || m_input_tensor_shape[3] == -1)
    {
      m_input_tensor_shape[2] = m_net_height;
      m_input_tensor_shape[3] = m_net_width;
    }

    //init output
    size_t _outputNodesNum = m_session->GetOutputCount();

    if (_outputNodesNum != 2)
    {
      const std::string message("Error: Loaded model has " + std::to_string(_outputNodesNum) + " output(s), it's not a YOLO segmentation model");
      throw std::exception(message.c_str());
    }

    m_output0_name = std::move(m_session->GetOutputNameAllocated(0, allocator));
    m_output1_name = std::move(m_session->GetOutputNameAllocated(1, allocator));

    Ort::TypeInfo type_info_output0(nullptr);
    Ort::TypeInfo type_info_output1(nullptr);
    if (strcmp(m_output0_name.get(), m_output1_name.get()) < 0)  //make sure "output0" is in front of  "output1"
    {
      type_info_output0 = m_session->GetOutputTypeInfo(0);  //output0
      type_info_output1 = m_session->GetOutputTypeInfo(1);  //output1
      m_outputs_names.push_back(m_output0_name.get());
      m_outputs_names.push_back(m_output1_name.get());
    }
    else
    {
      type_info_output0 = m_session->GetOutputTypeInfo(1);  //output0
      type_info_output1 = m_session->GetOutputTypeInfo(0);  //output1
      m_outputs_names.push_back(m_output1_name.get());
      m_outputs_names.push_back(m_output0_name.get());
    }

    auto tensor_info_output0 = type_info_output0.GetTensorTypeAndShapeInfo();
    ONNXTensorElementDataType _outputNodeDataType = tensor_info_output0.GetElementType();
    m_output0_tensor_shape = tensor_info_output0.GetShape();

    m_input_tensor_length = VectorProduct(m_input_tensor_shape);

    env.acquire();
  }

  ~YOLO()
  {
    delete m_session;

    getONNXEnv().release();
  }

  void warmup()
  {
    std::vector<Ort::Value> output_tensors;

    if (m_use_fp16)
    {
      half* temp = new half[m_input_tensor_length];
      Ort::Value input_tensor = Ort::Value::CreateTensor<half>(m_memory_info, temp, m_input_tensor_length, m_input_tensor_shape.data(), m_input_tensor_shape.size());
      output_tensors = m_session->Run(Ort::RunOptions{ nullptr }, m_inputs_names.data(), &input_tensor, m_inputs_names.size(), m_outputs_names.data(), m_outputs_names.size());
      delete[] temp;
    }
    else
    {
      float* temp = new float[m_input_tensor_length];
      Ort::Value input_tensor = Ort::Value::CreateTensor<float>(m_memory_info, temp, m_input_tensor_length, m_input_tensor_shape.data(), m_input_tensor_shape.size());
      output_tensors = m_session->Run(Ort::RunOptions{ nullptr }, m_inputs_names.data(), &input_tensor, m_inputs_names.size(), m_outputs_names.data(), m_outputs_names.size());
      delete[] temp;
    }
  }

  void GetMask(const cv::Mat& maskProposals, const cv::Mat& maskProtos, OutputParams& output, const MaskParams& maskParams)
  {
    int seg_channels = maskProtos.size[1];
    int seg_height = maskProtos.size[2];
    int seg_width = maskProtos.size[3];

    cv::Vec4f params = maskParams.params;
    cv::Size src_img_shape = maskParams.srcImgShape;
    cv::Rect temp_rect = output.box;

    // Crop from mask_protos
    int rang_x = floor((temp_rect.x * params[0] + params[2]) / m_net_width * seg_width);
    int rang_y = floor((temp_rect.y * params[1] + params[3]) / m_net_height * seg_height);
    int rang_w = ceil(((temp_rect.x + temp_rect.width) * params[0] + params[2]) / m_net_width * seg_width) - rang_x;
    int rang_h = ceil(((temp_rect.y + temp_rect.height) * params[1] + params[3]) / m_net_height * seg_height) - rang_y;

    rang_w = MAX(rang_w, 1);
    rang_h = MAX(rang_h, 1);
    if (rang_x + rang_w > seg_width)
    {
      if (seg_width - rang_x > 0)
        rang_w = seg_width - rang_x;
      else
        rang_x -= 1;
    }

    if (rang_y + rang_h > seg_height)
    {
      if (seg_height - rang_y > 0)
        rang_h = seg_height - rang_y;
      else
        rang_y -= 1;
    }

    std::vector<cv::Range> roi_rangs;
    roi_rangs.push_back(cv::Range(0, 1));
    roi_rangs.push_back(cv::Range::all());
    roi_rangs.push_back(cv::Range(rang_y, rang_h + rang_y));
    roi_rangs.push_back(cv::Range(rang_x, rang_w + rang_x));

    // Crop
    cv::Mat temp_mask_protos = maskProtos(roi_rangs).clone();
    cv::Mat protos = temp_mask_protos.reshape(0, { seg_channels, rang_w * rang_h });
    cv::Mat matmul_res = (maskProposals * protos).t();

#if USE_SIGMOID_FUNC
    cv::Mat masks_feature = matmul_res.reshape(1, { rang_h, rang_w });

    cv::Mat dest;
    cv::exp(-masks_feature, dest);
    dest = 1.0 / (1.0 + dest);
#else 
    cv::Mat dest = matmul_res.reshape(1, { rang_h, rang_w });
#endif

    float t1_val = float(m_net_width) / float(seg_width);
    float t2_val = float(m_net_height) / float(seg_height);

    int left = floor((rang_x * t1_val - params[2]) / params[0]);
    int top = floor((rang_y * t2_val - params[3]) / params[1]);
    int width = ceil(rang_w * t1_val / params[0]);
    int height = ceil(rang_h * t2_val / params[1]);

    cv::Mat mask;
    cv::resize(dest, mask, cv::Size(width, height), cv::INTER_NEAREST);

    cv::Rect mask_rect = temp_rect - cv::Point(left, top);
    mask_rect &= cv::Rect(0, 0, width, height);
    mask = mask(mask_rect) > _maskThreshold;

    if (mask.rows != temp_rect.height || mask.cols != temp_rect.width)
      cv::resize(mask, mask, temp_rect.size(), cv::INTER_NEAREST);

    output.boxMask = mask;
  }

  template<typename T>
  void BlobFromGrayImage(const cv::Mat& iImg, T* iBlob)
  {
    unsigned long long im_index;
    unsigned long long img_size = iImg.rows * iImg.cols;
    unsigned char* data_ptr = iImg.data;

    for (im_index = 0ULL; im_index < img_size; im_index++)
      iBlob[im_index] = static_cast<T>(data_ptr[im_index] / 255.0F);

    memcpy(&iBlob[img_size], iBlob, img_size * sizeof(T));
    memcpy(&iBlob[2 * img_size], iBlob, img_size * sizeof(T));
  }

  template<typename T>
  void BlobFromImage(cv::Mat iImg, T* iBlob)
  {
    int channels = iImg.channels();
    int imgHeight = iImg.rows;
    int imgWidth = iImg.cols;

    for (int c = 0; c < channels; c++)
    {
      for (int h = 0; h < imgHeight; h++)
      {
        for (int w = 0; w < imgWidth; w++)
        {
          iBlob[c * imgWidth * imgHeight + h * imgWidth + w] = typename std::remove_pointer<T>::type((iImg.at<cv::Vec3b>(h, w)[c]) / 255.0f);
        }
      }
    }
  }

  void LetterBox(const cv::Mat& image, cv::Mat& outImage, cv::Vec4d& params, const cv::Size& newShape, bool autoShape = false, bool scaleFill = false, bool scaleUp = true, int stride = 32, const cv::Scalar& color = cv::Scalar(114))
  {
    cv::Size shape = image.size();
    float r = std::min((float)newShape.height / (float)shape.height,
      (float)newShape.width / (float)shape.width);

    if (!scaleUp)
      r = std::min(r, 1.0f);

    float ratio[2]{ r, r };
    int new_un_pad[2] = { (int)std::round((float)shape.width * r),(int)std::round((float)shape.height * r) };

    auto dw = (float)(newShape.width - new_un_pad[0]);
    auto dh = (float)(newShape.height - new_un_pad[1]);

    if (autoShape)
    {
      dw = (float)((int)dw % stride);
      dh = (float)((int)dh % stride);
    }
    else if (scaleFill)
    {
      dw = 0.0f;
      dh = 0.0f;

      new_un_pad[0] = newShape.width;
      new_un_pad[1] = newShape.height;

      ratio[0] = (float)newShape.width / (float)shape.width;
      ratio[1] = (float)newShape.height / (float)shape.height;
    }

    dw /= 2.0f;
    dh /= 2.0f;

    if (shape.width != new_un_pad[0] && shape.height != new_un_pad[1])
      cv::resize(image, outImage, cv::Size(new_un_pad[0], new_un_pad[1]));
    else
      outImage = image.clone();

    int top = int(std::round(dh - 0.1f));
    int bottom = int(std::round(dh + 0.1f));
    int left = int(std::round(dw - 0.1f));
    int right = int(std::round(dw + 0.1f));

    params[0] = ratio[0];
    params[1] = ratio[1];
    params[2] = left;
    params[3] = top;

    cv::copyMakeBorder(outImage, outImage, top, bottom, left, right, cv::BORDER_CONSTANT, color);
  }

  template<typename T>
  void Detect(const cv::Mat& srcImg, const cv::Vec4d& params, std::vector<OutputParams>& output, const float rec_treshold, const unsigned int max_results, uchar& type)
  {
    output.clear();

    int col = srcImg.cols;
    int row = srcImg.rows;

    if (m_input_tensor_length != col * row * 3)
    {
      std::cout << "Warning: Incorrect model input, skipped" << std::endl;
      return;
    }

    T* blob = new T[m_input_tensor_length];

    if (srcImg.channels() == 1)
      BlobFromGrayImage<T>(srcImg, blob);
    else
      BlobFromImage<T>(srcImg, blob);

    Ort::Value input_tensor = Ort::Value::CreateTensor<T>(m_memory_info, blob, m_input_tensor_length, m_input_tensor_shape.data(), m_input_tensor_shape.size());

    std::vector<Ort::Value> output_tensors = m_session->Run(Ort::RunOptions{ nullptr }, m_inputs_names.data(), &input_tensor, m_inputs_names.size(), m_outputs_names.data(), m_outputs_names.size());

    delete[] blob;

    //post-process
    {
      auto& d_val_0 = output_tensors[0];
      auto& d_val_1 = output_tensors[1];

      auto* all_data = d_val_0.GetTensorMutableData<typename std::remove_pointer<T>::type>();
      m_output0_tensor_shape = d_val_0.GetTensorTypeAndShapeInfo().GetShape();
      m_output1_tensor_shape = d_val_1.GetTensorTypeAndShapeInfo().GetShape();

      std::vector<int> mask_protos_shape = { 1, (int)m_output1_tensor_shape[1], (int)m_output1_tensor_shape[2], (int)m_output1_tensor_shape[3] };

      int mask_protos_length = VectorProduct(mask_protos_shape);

      int64_t one_output_length = VectorProduct(m_output0_tensor_shape) / m_output0_tensor_shape[0];

      int net_data_width = (int)m_output0_tensor_shape[1];

      int score_array_length = net_data_width - 4 - m_output1_tensor_shape[1];

      cv::Mat output0 = cv::Mat(cv::Size((int)m_output0_tensor_shape[2], (int)m_output0_tensor_shape[1]), type, all_data).t();

      int rows = output0.rows;
      std::vector<int> class_ids;
      std::vector<float> confidences;
      std::vector<cv::Rect> boxes;
      std::vector<std::vector<float>> picked_proposals;

      cv::Mat output0_32;
      if (CV_16F == type)
      {
        output0_32 = cv::Mat(output0.size(), CV_32F);
        output0.convertTo(output0_32, CV_32F);
      }
      else
        output0_32 = output0;

      float* pdata = (float*)output0_32.data;
      cv::Point classIdPoint;
      double max_class_score;
      float x, y, w, h;
      int left, top;
      for (int r = 0; r < rows; ++r)
      {
        cv::Mat scores = cv::Mat(1, score_array_length, CV_32F, pdata + 4);
        minMaxLoc(scores, 0, &max_class_score, 0, &classIdPoint);
        max_class_score = (float)max_class_score;

        if (max_class_score >= rec_treshold)
        {
          std::vector<float> temp_proto(pdata + 4 + score_array_length, pdata + net_data_width);
          picked_proposals.push_back(temp_proto);

          //rect [x,y,w,h]
          x = (pdata[0] - params[2]) / params[0];
          y = (pdata[1] - params[3]) / params[1];
          w = pdata[2] / params[0];
          h = pdata[3] / params[1];
          left = MAX(int(x - 0.5 * w + 0.5), 0);
          top = MAX(int(y - 0.5 * h + 0.5), 0);
          class_ids.push_back(classIdPoint.x);
          confidences.push_back(max_class_score);
          boxes.push_back(cv::Rect(left, top, int(w + 0.5), int(h + 0.5)));
        }

        pdata += net_data_width;
      }

      all_data += one_output_length;

      std::vector<int> nms_result;
      cv::dnn::NMSBoxes(boxes, confidences, rec_treshold, _nmsThreshold, nms_result);

      std::vector<std::vector<float>> temp_mask_proposals;
      cv::Rect wholeImgRect(0, 0, col, row);

      int max_index = std::min((int)nms_result.size(), (int)max_results);
      OutputParams result;
      for (int i = 0; i < max_index; ++i)
      {
        int idx = nms_result[i];
        result.id = class_ids[idx];
        result.confidence = confidences[idx];
        result.box = boxes[idx] & wholeImgRect;
        temp_mask_proposals.push_back(picked_proposals[idx]);
        output.push_back(result);
      }

      MaskParams mask_params;
      mask_params.params = params;
      mask_params.srcImgShape = srcImg.size();

      cv::Mat mask_protos = cv::Mat(mask_protos_shape, type, d_val_1.GetTensorMutableData<T>());

      cv::Mat mask_protos_32;
      if (CV_16F == type)
      {
        mask_protos_32 = cv::Mat(mask_protos.size(), CV_32F);
        mask_protos.convertTo(mask_protos_32, CV_32F);
      }
      else
        mask_protos_32 = mask_protos;

      for (int i = 0; i < temp_mask_proposals.size(); ++i)
        GetMask(cv::Mat(temp_mask_proposals[i]).t(), mask_protos_32, output[i], mask_params);
    }
  }

  void mask_procesing(cv::Mat& res_mask_big, const bool get_brain, const unsigned int erode_level)
  {
    if (get_brain)
    {
      std::vector<std::vector<cv::Point>> contours;
      std::vector<cv::Vec4i> hierarchy;
      cv::findContours(res_mask_big, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

      if (contours.size() > 0)
      {
        size_t biggest_contour = 0;
        double biggest_area = fabs(cv::contourArea(cv::Mat(contours[biggest_contour])));
        for (size_t _t = 1; _t < contours.size(); _t++)
        {
          double cur_area = fabs(cv::contourArea(cv::Mat(contours[_t])));

          if (cur_area > biggest_area)
          {
            biggest_contour = _t;

            biggest_area = cur_area;
          }
        }

        res_mask_big.setTo(0);
        cv::drawContours(res_mask_big, contours, biggest_contour, cv::Scalar(255), cv::FILLED);
      }
    }

    if (erode_level)
    {
      cv::Mat res_mask_big_cloned = res_mask_big.clone();

      cv::erode(res_mask_big_cloned, res_mask_big, cv::getStructuringElement(cv::MORPH_ERODE, cv::Size(3, 3)), cv::Point(-1, -1), erode_level);
    }
  }

  void process_detection_result(OutputParams& result, cv::Mat& res_mask)
  {
    if (result.boxMask.rows && result.boxMask.cols)
    {
      int res_mask_padding_x = result.box.x + result.box.size().width - res_mask.cols;
      int res_mask_padding_y = result.box.y + result.box.size().height - res_mask.rows;
      if (res_mask_padding_x < 0) res_mask_padding_x = 0;
      if (res_mask_padding_y < 0) res_mask_padding_y = 0;

      if (res_mask_padding_x || res_mask_padding_y)
      {
        result.box.width -= res_mask_padding_x;
        result.box.height -= res_mask_padding_y;

        result.boxMask = result.boxMask(cv::Range(0, result.boxMask.rows - res_mask_padding_y), cv::Range(0, result.boxMask.cols - res_mask_padding_x));
      }

      res_mask(result.box).setTo(255, result.boxMask);
    }
  }

  nb::ndarray<nb::numpy, unsigned char> process(const nb::ndarray<nb::numpy, unsigned char> input, const float rec_treshold, const unsigned int max_results, const bool get_brain, const unsigned int erode_level)
  {
    cv::Mat res_mask = cv::Mat::zeros(input.shape(0), input.shape(1), CV_8UC1);

    int col = input.shape(1);
    int row = input.shape(0);

    bool color_data = (input.ndim() == 3);

    // LetterBox
    cv::Mat netInputImg;
    cv::Vec4d params;

    if (color_data)
    {
      // Convert ndarray to mat
      cv::Mat colored = cv::Mat(cv::Size(col, row), CV_8UC3, (void*)input.data());

      LetterBox(colored, netInputImg, params, cv::Size(m_net_width, m_net_height), true, false, true, 32, cv::Scalar(114, 114, 114));
    }
    else
    {
      // Convert ndarray to mat
      const cv::Mat gray = cv::Mat(input.shape(0), input.shape(1), CV_8UC1, (void*)input.data());

      LetterBox(gray, netInputImg, params, cv::Size(m_net_width, m_net_height), true);
    }

    // Run detection
    std::vector<OutputParams> result;

    if (m_use_fp16)
    {
      uchar tx = CV_16F;
      Detect<half>(netInputImg, params, result, rec_treshold, max_results, tx);
    }
    else
    {
      uchar tx = CV_32F;
      Detect<float>(netInputImg, params, result, rec_treshold, max_results, tx);
    }

    if (result.size() > 0)
    {
      if (get_brain)
        process_detection_result(result[0], res_mask);
      else
      {
        for (int i = 0; i < result.size(); i++)
          process_detection_result(result[i], res_mask);
      }

      if (get_brain || erode_level > 0)
      {
        const double scale_factor = m_net_width / (double)input.shape(1) / 2.0;

        if (fabs(scale_factor - 1.0) > 0.001)
        {
          cv::Mat res_mask_big;
          cv::resize(res_mask, res_mask_big, cv::Size(scale_factor * res_mask.cols, scale_factor * res_mask.rows), 0, 0, cv::INTER_NEAREST);

          mask_procesing(res_mask_big, get_brain, erode_level);

          cv::resize(res_mask_big, res_mask, cv::Size(res_mask.cols, res_mask.rows), 0, 0, cv::INTER_AREA);
          cv::threshold(res_mask, res_mask, 127, 255, 0);
        }
        else
        {
          mask_procesing(res_mask, get_brain, erode_level);
        }
      }
    }

    // Dynamically allocate 'data'
    unsigned char* data = new unsigned char[input.shape(0) * input.shape(1)];

    // Delete 'data' when the 'owner' capsule expires
    nb::capsule owner(data, [](void* p) noexcept {delete[](unsigned char*)p; });

    // Copy 'data_internal' to 'data'
    memcpy(data, res_mask.data, sizeof(unsigned char) * input.shape(0) * input.shape(1));

    // Return result
    return nb::ndarray<nb::numpy, unsigned char>(data, { (size_t)input.shape(0), (size_t)input.shape(1) }, owner);
  }

private:
  std::string m_name;
  std::string m_net_path;

  bool m_use_gpu;
  bool m_use_fp16;

  unsigned int m_net_width;
  unsigned int m_net_height;

  const float _nmsThreshold = 0.70F;

#if USE_SIGMOID_FUNC
  const float _maskThreshold = 0.50F;
#else 
  const float _maskThreshold = 0.00F;
#endif

  // ONNXRUNTIME
  Ort::Session* m_session = nullptr;
  Ort::MemoryInfo m_memory_info = Ort::MemoryInfo(Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtDeviceAllocator, OrtMemType::OrtMemTypeCPUOutput));

  std::shared_ptr<char> m_input_name, m_output0_name, m_output1_name; //holders
  std::vector<char*> m_inputs_names;
  std::vector<char*> m_outputs_names;

  std::vector<int64_t> m_input_tensor_shape;
  std::vector<int64_t> m_output0_tensor_shape;
  std::vector<int64_t> m_output1_tensor_shape;

  size_t m_input_tensor_length;
};

NB_MODULE(yolo_segment_ext, m) {
  m.def("setGlobalThreadPoolSize", &setONNXGlobalThreadPoolSize, nb::arg("threadPoolSize"), nb::call_guard<nb::gil_scoped_release>());

  nb::class_<YOLO>(m, "YOLO")
    .def(nb::init<const std::string&, const std::string&, const bool, const bool, const unsigned int, const unsigned int, const unsigned int>(), nb::arg("name"), nb::arg("net_path"), nb::arg("use_gpu"), nb::arg("use_fp16"), nb::arg("net_width"), nb::arg("net_height"), nb::arg("cudaID") = 0, nb::call_guard<nb::gil_scoped_release>())
    .def("warmup", &YOLO::warmup, nb::call_guard<nb::gil_scoped_release>())
    .def("process", &YOLO::process, nb::arg("input"), nb::arg("rec_treshold"), nb::arg("max_results"), nb::arg("get_brain"), nb::arg("erode_level"), nb::call_guard<nb::gil_scoped_release>());
}