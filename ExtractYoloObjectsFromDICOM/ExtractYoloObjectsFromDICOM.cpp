#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <experimental/filesystem>

#include <concave.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

#include "dicomlib/Buffer.hpp"
#include "dicomlib/DataSet.hpp"
#include "dicomlib/File.hpp"
#include "dicomlib/Tag.hpp"
using namespace dicom;

#include <string>
#include <sstream>

#include <opencv2/imgproc.hpp>

struct dataset
{
  String path;
  bool have_brain_mask;
  bool have_target_mask;

  dataset(String path_value, bool have_brain_mask_value, bool have_target_mask_value)
  {
    path = path_value;
    have_brain_mask = have_brain_mask_value;
    have_target_mask = have_target_mask_value;
  }
};

struct ExtractPoint
{
  double x;
  double y;

  double polar_angle;

  ExtractPoint()
  {
    x = 0;
    y = 0;
  }

  void setAngle(ExtractPoint& center)
  {
    polar_angle = std::atan2(center.y - y, center.x - x);

    if (polar_angle < 0) polar_angle += (2.0 * M_PI);

    polar_angle = polar_angle * 180.0 / M_PI;
  }
};

typedef double qreal;

void validate_points(Mat* cv_arr_local, Mat* mask, std::vector<std::pair<int, int>>* data_for_check, int& label, bool full_connection)
{
  std::vector<std::pair<int, int>> data_for_check_new;

  std::vector<std::pair<int, int>>* main_ptr = data_for_check;
  std::vector<std::pair<int, int>>* slave_ptr = &data_for_check_new;
  std::vector<std::pair<int, int>>* t_ptr;

  while (true)
  {
    bool data_is_modified = false;

    for (size_t _t = 0; _t < main_ptr->size(); _t++)
    {
      int L = main_ptr->operator[](_t).first;
      int P = main_ptr->operator[](_t).second;

      if (cv_arr_local->at<unsigned short int>(L, P) == 0)
      {
        mask->at<unsigned short int>(L, P) = label;

        for (short y = -1; y <= 1; y++)
        {
          if ((L + y) > -1 && (L + y) < cv_arr_local->rows)
          {
            for (short x = -1; x <= 1; x++)
            {
              if ((P + x) > -1 && (P + x) < cv_arr_local->cols)
              {
                bool res;
                if (full_connection)
                {
                  res = ((x == -1 && y == 0) || (x == 1 && y == 0) || (x == 0 && y == -1) || (x == 0 && y == 1) || (x == -1 && y == -1) || (x == 1 && y == 1) || (x == -1 && y == 1) || (x == 1 && y == -1));
                }
                else
                {
                  res = ((x == -1 && y == 0) || (x == 1 && y == 0) || (x == 0 && y == -1) || (x == 0 && y == 1));
                }

                if (res)
                {
                  if (cv_arr_local->at<unsigned short int>(L + y, P + x) == 0 && mask->at<unsigned short int>(L + y, P + x) == 0)
                  {
                    bool point_exist = false;

                    std::pair<int, int> p;
                    p.first = L + y;
                    p.second = P + x;

                    for (size_t _q = 0; _q < slave_ptr->size(); _q++)
                    {
                      if (slave_ptr->operator[](_q).first == p.first && slave_ptr->operator[](_q).second == p.second)
                      {
                        point_exist = true;

                        break;
                      }
                    }

                    if (point_exist) continue;

                    mask->at<unsigned short int>(p.first, p.second) = label;

                    slave_ptr->push_back(p);

                    data_is_modified = true;
                  }
                }
              }
            }
          }
        }
      }
    }

    if (data_is_modified && slave_ptr->size())
    {
      main_ptr->clear();

      t_ptr = main_ptr;

      main_ptr = slave_ptr;

      slave_ptr = t_ptr;
    }
    else
    {
      label++;

      break;
    }
  }
}

void update_image(Mat& img_out, unsigned char intense, double x_start, double y_start, int thickness, bool single_class, unsigned char v_index, unsigned char single_class_empty_color)
{
  for (double y = y_start - thickness; y <= y_start + thickness; y++)
  {
    int y_res = (int)floor(y + 0.5);
    if (y_res >= 0 && y_res < img_out.rows)
    {
      for (double x = x_start - thickness; x <= x_start + thickness; x++)
      {
        int x_res = (int)floor(x + 0.5);
        if (x_res >= 0 && x_res < img_out.cols)
        {
          Vec3b& color = img_out.at<Vec3b>(Point(x_res, y_res));

          if (single_class)
          {
            switch (single_class_empty_color)
            {
            case 0:
            {
              color[0] = 0;
              color[1] = intense;
              color[2] = intense;

              break;
            }
            case 1:
            {
              color[0] = intense;
              color[1] = 0;
              color[2] = intense;

              break;
            }
            case 2:
            {
              color[0] = intense;
              color[1] = intense;
              color[2] = 0;

              break;
            }
            }
          }
          else
          {
            switch (v_index)
            {
            case 0:
            {
              color[0] = intense;
              color[1] = 0;
              color[2] = intense;

              break;
            }
            case 1:
            {
              color[0] = intense;
              color[1] = intense;
              color[2] = 0;

              break;
            }
            default:
            {
              color[0] = 0;
              color[1] = intense;
              color[2] = intense;
            }
            break;
            }
          }
        }
      }
    }
  }
}

void process_func(Mat& img, Mat& img_out, bool single_class, const unsigned char possible_values_count, const unsigned char possible_values[], std::string file_without_extension, std::string& out_str, int thickness_value, double step_mod, double scale_factor, int img_mul_value, unsigned char single_class_empty_color, bool one_object, bool full_connection, bool remove_small_target_objects)
{
  //printf("NEXT\r\n");

  //char wname[256];

  srand(777U);

  //sprintf(wname, "Original -1");
  //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
  //imshow(wname, img);
  //waitKey(0);

  bool first_line_printed = false;

  unsigned char v_max = (single_class) ? 1U : possible_values_count;
  for (unsigned char v_index = 0; v_index < v_max; v_index++)
  {
    Mat cv_arr = Mat::zeros(img.rows + 2, img.cols + 2, CV_8U);

    if (single_class)
    {
      for (unsigned short y = 0; y < img.rows; y++)
      {
        for (unsigned short x = 0; x < img.cols; x++)
        {
          for (unsigned char v_index2 = 0; v_index2 < possible_values_count; v_index2++)
          {
            unsigned char m_value = possible_values[v_index2];
            unsigned char dist = (unsigned char)std::abs((int)img.at<unsigned char>(y, x) - (int)m_value);

            if (dist < 255.0 / ((double)(possible_values_count + 1)))      // пиксель объекта
              cv_arr.at<unsigned char>(y + 1, x + 1) = 255U;
          }
        }
      }
    }
    else
    {
      unsigned char m_value = possible_values[v_index];

      for (unsigned short y = 0; y < img.rows; y++)
      {
        for (unsigned short x = 0; x < img.cols; x++)
        {
          unsigned char dist = (unsigned char)std::abs((int)img.at<unsigned char>(y, x) - (int)m_value);

          if (dist < 255.0 / ((double)(possible_values_count + 1)))      // пиксель объекта
            cv_arr.at<unsigned char>(y + 1, x + 1) = 255U;
        }
      }
    }

    //sprintf(wname, "Original 0");
    //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
    //imshow(wname, cv_arr);
    //waitKey(0);

    // выделение зон
    {
      Mat background_mask = Mat::zeros(cv_arr.rows, cv_arr.cols, CV_16U);        // маска, на которой будет выделен фон (значения 1)
      Mat objects_mask_labeled = Mat::zeros(cv_arr.rows, cv_arr.cols, CV_16U);   // маска, на которой будут промаркированные объекты

      std::vector<std::pair<int, int>> data_for_check;
      std::pair<int, int> p;
      p.first = 0;
      p.second = 0;
      data_for_check.push_back(p);

      std::vector<std::pair<int, int>> data_for_check_new;

      std::vector<std::pair<int, int>>* main_ptr = &data_for_check;
      std::vector<std::pair<int, int>>* slave_ptr = &data_for_check_new;
      std::vector<std::pair<int, int>>* t_ptr;

      // процесс поиска фона
      while (true)
      {
        bool data_is_modified = false;

        for (size_t _t = 0; _t < main_ptr->size(); _t++)
        {
          int L = main_ptr->operator[](_t).first;
          int P = main_ptr->operator[](_t).second;

          if (cv_arr.at<unsigned char>(L, P) == 0)
          {
            background_mask.at<unsigned short int>(L, P) = 1U;

            for (short y = -1; y <= 1; y++)
            {
              if ((L + y) > -1 && (L + y) < cv_arr.rows)
              {
                for (short x = -1; x <= 1; x++)
                {
                  if ((P + x) > -1 && (P + x) < cv_arr.cols)
                  {
                    bool res;
                    if (full_connection)
                    {
                      res = ((x == -1 && y == 0) || (x == 1 && y == 0) || (x == 0 && y == -1) || (x == 0 && y == 1) || (x == -1 && y == -1) || (x == 1 && y == 1) || (x == -1 && y == 1) || (x == 1 && y == -1));
                    }
                    else
                    {
                      res = ((x == -1 && y == 0) || (x == 1 && y == 0) || (x == 0 && y == -1) || (x == 0 && y == 1));
                    }

                    if (res)
                    {
                      if (cv_arr.at<unsigned char>(L + y, P + x) == 0 && background_mask.at<unsigned short int>(L + y, P + x) == 0)
                      {
                        bool point_exist = false;

                        std::pair<int, int> p;
                        p.first = L + y;
                        p.second = P + x;

                        for (size_t _q = 0; _q < slave_ptr->size(); _q++)
                        {
                          if (slave_ptr->operator[](_q).first == p.first && slave_ptr->operator[](_q).second == p.second)
                          {
                            point_exist = true;

                            break;
                          }
                        }

                        if (point_exist) continue;

                        background_mask.at<unsigned short int>(p.first, p.second) = 1U;

                        slave_ptr->push_back(p);

                        data_is_modified = true;
                      }
                    }
                  }
                }
              }
            }
          }
        }

        if (data_is_modified && slave_ptr->size())
        {
          main_ptr->clear();

          t_ptr = main_ptr;

          main_ptr = slave_ptr;

          slave_ptr = t_ptr;
        }
        else
        {
          break;
        }
      }

      data_for_check.clear();
      data_for_check_new.clear();

      int objects_label = 1;
      for (int L = 0; L < background_mask.rows; L++)
      {
        for (int P = 0; P < background_mask.cols; P++)
        {
          if (background_mask.at<unsigned short int>(L, P) == 0 && objects_mask_labeled.at<unsigned short int>(L, P) == 0)
          {
            std::vector<std::pair<int, int>> data_for_check;
            std::pair<int, int> p;
            p.first = L;
            p.second = P;
            data_for_check.push_back(p);

            validate_points(&background_mask, &objects_mask_labeled, &data_for_check, objects_label, full_connection);
          }
        }
      }

      //sprintf(wname, "[0] Window");
      //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
      //imshow(wname, background_mask);
      //waitKey(0);

      int* S = new int[objects_label];
      int* XMin = new int[objects_label];
      int* XMax = new int[objects_label];
      int* YMin = new int[objects_label];
      int* YMax = new int[objects_label];

      memset(S, 0, objects_label * sizeof(int));
      memset(XMax, 0, objects_label * sizeof(int));
      memset(YMax, 0, objects_label * sizeof(int));

      for (int i = 1; i < objects_label; i++)
      {
        XMin[i] = INT_MAX;

        YMin[i] = INT_MAX;
      }

      for (int i = 1; i < objects_label; i++)
      {
        for (int L = 0; L < objects_mask_labeled.rows; L++)
        {
          for (int P = 0; P < objects_mask_labeled.cols; P++)
          {
            if (objects_mask_labeled.at<unsigned short int>(L, P) == i)
            {
              S[i]++;

              if (XMin[i] > P) XMin[i] = P;
              if (YMin[i] > L) YMin[i] = L;
              if (XMax[i] < P) XMax[i] = P;
              if (YMax[i] < L) YMax[i] = L;
            }
          }
        }
      }

      // расширим область для корректной работы Canny
      for (int i = 1; i < objects_label; i++)
      {
        XMin[i]--;
        XMax[i]++;

        YMin[i]--;
        YMax[i]++;
      }

      int i_start = 1;
      int i_end = objects_label;

      if (one_object)
      {
        int s_max_index = 1;
        int s_max = -1;

        for (int i = i_start; i < i_end; i++)
        {
          if (S[i] > s_max)
          {
            s_max_index = i;
            s_max = S[i];
          }
        }

        i_start = s_max_index;
        i_end = s_max_index + 1;
      }

      for (int i = i_start; i < i_end; i++)
      {
        if ((remove_small_target_objects == true && S[i] > 1U) || (remove_small_target_objects == false && S[i] > 0U))
        {
          //printf("S[%d]: %d\r\n", i, S[i]);

          ConcavePointVector concave_points;

          unsigned short width_dcm_local = XMax[i] - XMin[i] + 1, height_dcm_local = YMax[i] - YMin[i] + 1;

          Mat cv_arr_local = Mat::zeros(height_dcm_local, width_dcm_local, CV_8U);

          for (int L = YMin[i] + 1; L < YMax[i]; L++)
          {
            for (int P = XMin[i] + 1; P < XMax[i]; P++)
            {
              if (objects_mask_labeled.at<unsigned short int>(L, P) == i)
                cv_arr_local.at<unsigned char>(L - YMin[i], P - XMin[i]) = cv_arr.at<unsigned char>(L, P);
            }
          }

          // работа с основным контуром
          Mat canny_output;

          Mat cv_arr_resized = Mat(img_mul_value * height_dcm_local, img_mul_value * width_dcm_local, CV_8U);
          cv::resize(cv_arr_local, cv_arr_resized, cv_arr_resized.size(), 0, 0, CV_INTER_AREA);
          ::Canny(cv_arr_resized, canny_output, 55, 200, 3, true);

          for (unsigned short y = 0; y < img_mul_value * height_dcm_local; y++)
          {
            for (unsigned short x = 0; x < img_mul_value * width_dcm_local; x++)
            {
              if (canny_output.at<unsigned char>(y, x) == 255)
              {
                ConcavePoint p;
                p.x = (x + 1) / qreal(img_mul_value) + XMin[i] - 1;
                p.y = (y + 1) / qreal(img_mul_value) + YMin[i] - 1;

                concave_points.push_back(p);
              }
            }
          }

          //sprintf(wname, "[0] Window");
          //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
          //imshow(wname, cv_arr_resized);
          //waitKey(0);

          //std::cout << "concave_points.size(): " << (int)concave_points.size() << std::endl;

          //sprintf(wname, "[1] Window");
          //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
          //imshow(wname, canny_output);
          //waitKey(0);

          IdentifyConcavePoints(concave_points);
          ConcavePointVector hull = ConcaveHull(concave_points, 3, true);

          //std::cout << "hull.size(): " << (int)hull.size() << std::endl;

          size_t step = (size_t)floor(pow(hull.size(), 0.5) * step_mod + 0.5);
          if (step < 1) step = 1;
          //std::cout << "step: " << step << std::endl;

          if (hull.size() > (3U * step - 1))
          {
            std::vector<ExtractPoint> local_objects;
            for (size_t _t = 0; _t < hull.size(); _t++)
            {
              ConcavePoint& cur_concave_point = hull[_t];

              ExtractPoint xp;
              xp.x = cur_concave_point.x;
              xp.y = cur_concave_point.y;

              local_objects.push_back(xp);
            }

            // resort
            {
              size_t best_point_index = 0;
              qreal best_dist = DBL_MAX;
              for (size_t a = 0; a < hull.size(); a++)
              {
                qreal dist = sqrt(local_objects[a].x * local_objects[a].x + local_objects[a].y * local_objects[a].y);

                if (dist < best_dist)
                {
                  best_dist = dist;

                  best_point_index = a;
                }
              }

              if (best_point_index)
              {
                std::vector<ExtractPoint> cur_vector_swap;
                for (size_t a = best_point_index; a < hull.size(); a++)
                  cur_vector_swap.push_back(local_objects[a]);

                for (size_t a = 0; a < best_point_index; a++)
                  cur_vector_swap.push_back(local_objects[a]);

                local_objects.clear();
                for (size_t a = 0; a < cur_vector_swap.size(); a++)
                  local_objects.push_back(cur_vector_swap[a]);
              }
            }

            // отрисовка точек из local_objects
            {
              if (first_line_printed)
                out_str += "\n";
              else first_line_printed = true;

              out_str += std::to_string(single_class ? 0 : (int)v_index) + " ";

              for (size_t exp_t = 0; exp_t < local_objects.size(); exp_t += step)
              {
                //std::cout << local_objects[exp_t].x << " and " << local_objects[exp_t].y << std::endl;

                double x_data = local_objects[exp_t].x / (double)img.cols;
                double y_data = local_objects[exp_t].y / (double)img.rows;

                out_str += std::to_string(x_data) + " " + std::to_string(y_data);
                if ((exp_t + step) < local_objects.size()) out_str += " ";

                // update debug image
                {
                  unsigned char intense = 55 + (unsigned char)floor(201 * (double)exp_t / (double)local_objects.size());

                  update_image(img_out, intense, x_data * img_out.cols, y_data * img_out.rows, thickness_value, single_class, v_index, single_class_empty_color);
                }
              }
            }
          }
        }
      }

      delete[]S;
      delete[]XMin;
      delete[]XMax;
      delete[]YMin;
      delete[]YMax;
    }
  }

  cv::destroyAllWindows();
}

struct train_test_split_result {
  std::vector<String> train;
  std::vector<String> test;
};

train_test_split_result train_test_split(std::vector<String>& data, double test_size)
{
  srand(777U);
  train_test_split_result result;

  std::random_shuffle(data.begin(), data.end());

  for (size_t i = 0; i < data.size(); i++)
  {
    if (((100.0 * (i + 1.0)) / double(data.size())) <= test_size)
    {
      result.test.push_back(data[i]);
    }
    else
    {
      result.train.push_back(data[i]);
    }
  }

  return result;
}

void dataset_process(UINT16 normalize_value_start, UINT16 normalize_value_end, std::string tomograph_operating_mode, std::vector<String>& input_img_paths, dataset& data, bool single_class, const unsigned char possible_values_count, const unsigned char* possible_values, std::string mode, int thickness, int thickness_target, int img_mul, int img_mul_target, std::string target, bool only_dilate_and_save_masks, bool blue_mode = false)
{
  UINT16 val_min = 65535U;
  UINT16 val_max = 0U;

  for (int i = 0; i < (int)input_img_paths.size(); i++)
  {
    std::string::size_type idx = input_img_paths[i].rfind('.');

    if (idx != std::string::npos)
    {
      std::string extension = input_img_paths[i].substr(idx + 1);

      std::string base_filename = input_img_paths[i].substr(input_img_paths[i].find_last_of("/\\") + 1);
      std::string::size_type const p(base_filename.find_last_of('.'));
      std::string file_without_extension = base_filename.substr(0, p);

      dicom::DataSet* full_ds = new dicom::DataSet(); // исходный датасет
      std::wstring full_path = fs::path(input_img_paths[i].operator std::string()).wstring();
      dicom::ReadW(full_path, *full_ds);
      const std::vector<UINT16>& full_pixel_data = full_ds->operator()(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

      unsigned short width_dcm = 0U, height_dcm = 0U;
      if (full_ds->exists(TAG_COLUMNS)) full_ds->operator()(TAG_COLUMNS) >> width_dcm;
      if (full_ds->exists(TAG_ROWS)) full_ds->operator()(TAG_ROWS) >> height_dcm;

      std::string target_path = data.path + file_without_extension + ".png";
      Mat img_target = (data.have_target_mask && std::experimental::filesystem::exists(target_path)) ? imread(target_path, IMREAD_GRAYSCALE) : Mat::zeros(height_dcm, width_dcm, CV_8U);

      if (only_dilate_and_save_masks)
      {
        Mat img = Mat::zeros(img_target.rows, img_target.cols, CV_8U);
        cv::dilate(img_target, img, Mat(), Point(-1, -1), 1);

        imwrite(target_path, img);

        continue;
      }

      unsigned long int index_of_first_pixel_in_section_0;
      //unsigned long int index_of_first_pixel_in_section_1;

      size_t area = (size_t)width_dcm * (size_t)height_dcm;
      size_t num_of_frames = full_pixel_data.size() / area;

      index_of_first_pixel_in_section_0 = 0UL;
      //index_of_first_pixel_in_section_1 = 0UL;

      std::string brain_path = data.path + file_without_extension + "_brain.png";
      Mat img_brain = (data.have_brain_mask && std::experimental::filesystem::exists(brain_path)) ? imread(brain_path, IMREAD_GRAYSCALE) : Mat::zeros(height_dcm, width_dcm, CV_8U);

      if (num_of_frames == 1)
      {
        std::cout << "Processing '" << input_img_paths[i] << "' (file '" << (i + 1) << " / " << input_img_paths.size() << "')" << std::endl;

        Mat img_out_debug_brain = Mat::zeros(height_dcm, width_dcm, CV_8UC3);
        Mat img_out_debug_target = Mat::zeros(height_dcm, width_dcm, CV_8UC3);

        for (short y = 0; y < height_dcm; y++)
        {
          for (short x = 0; x < width_dcm; x++)
          {
            UINT16 val_UINT16 = full_pixel_data[index_of_first_pixel_in_section_0];

            if (val_UINT16 < val_min) val_min = val_UINT16;
            if (val_UINT16 > val_max) val_max = val_UINT16;

            unsigned char val_data;
            if (val_UINT16 > normalize_value_end) val_data = 255U;
            else if (val_UINT16 < normalize_value_start) val_data = 0U;
            else
              val_data = (unsigned char)floor((val_UINT16 - normalize_value_start) / (double)(normalize_value_end - normalize_value_start) * 255.0 + 0.5);

            if (data.have_brain_mask)
            {
              cv::Vec3b& pixel_brain = img_out_debug_brain.at<Vec3b>(y, x);

              if (blue_mode)
              {
                pixel_brain[0] = 227;
                pixel_brain[1] = val_data;
                pixel_brain[2] = 27;
              }
              else
              {
                pixel_brain[0] = pixel_brain[1] = pixel_brain[2] = val_data;
              }
            }

            if (data.have_target_mask)
            {
              cv::Vec3b& pixel_target = img_out_debug_target.at<Vec3b>(y, x);

              if (blue_mode)
              {
                pixel_target[0] = 227;
                pixel_target[1] = val_data;
                pixel_target[2] = 27;
              }
              else
              {
                pixel_target[0] = pixel_target[1] = pixel_target[2] = val_data;
              }
            }

            index_of_first_pixel_in_section_0++;
          }
        }

        double scale_factor = (tomograph_operating_mode == "t2_data" || tomograph_operating_mode == "swi_data") ? 2.5 : 8.0;
        double step_mod = (tomograph_operating_mode == "t2_data" || tomograph_operating_mode == "swi_data") ? 1.0 : 0.5;

        if (data.have_brain_mask)
        {
          bool full_connection = false;
          bool one_object = true;
          bool remove_small_target_objects = true;

          imwrite(".\\" + tomograph_operating_mode + "\\" + "brain\\" + mode + "\\images\\" + file_without_extension + ".png", img_out_debug_brain);

          Mat im_resized;
          cv::resize(img_out_debug_brain, im_resized, cv::Size(), scale_factor, scale_factor, CV_INTER_LINEAR);

          std::string out_str_brain = "";
          if (std::experimental::filesystem::exists(brain_path))
            process_func(img_brain, im_resized, single_class, possible_values_count, possible_values, file_without_extension, out_str_brain, thickness, step_mod, scale_factor, img_mul, 1U, one_object, full_connection, remove_small_target_objects);

          imwrite(".\\" + tomograph_operating_mode + "\\" + "debug\\" + file_without_extension + "_brain.png", im_resized);

          std::ofstream fw_brain(".\\" + tomograph_operating_mode + "\\" + "brain\\" + mode + "\\labels\\" + file_without_extension + ".txt", std::ofstream::out);
          if (fw_brain.is_open())
          {
            fw_brain << out_str_brain;
            fw_brain.close();
          }
          else std::cout << "Problem with opening file '" << ".\\" + tomograph_operating_mode + "\\" + "brain\\" + mode + "\\labels\\" + file_without_extension + ".txt'" << std::endl;
        }

        if (data.have_target_mask)
        {
          bool full_connection = (tomograph_operating_mode == "swi_data");
          bool one_object = false;
          bool remove_small_target_objects = (tomograph_operating_mode == "t2_data" || tomograph_operating_mode == "adc_data");

          imwrite(".\\" + tomograph_operating_mode + "\\" + target + "\\" + mode + "\\images\\" + file_without_extension + ".png", img_out_debug_target);

          Mat im_resized;
          cv::resize(img_out_debug_target, im_resized, cv::Size(), scale_factor, scale_factor, CV_INTER_LINEAR);

          std::string out_str_target = "";
          if (std::experimental::filesystem::exists(target_path))
            process_func(img_target, im_resized, single_class, possible_values_count, possible_values, file_without_extension, out_str_target, thickness_target, step_mod, scale_factor, img_mul_target, 0U, one_object, full_connection, remove_small_target_objects);

          imwrite(".\\" + tomograph_operating_mode + "\\" + "debug\\" + file_without_extension + "_" + target + ".png", im_resized);

          std::ofstream fw_target(".\\" + tomograph_operating_mode + "\\" + target + "\\" + mode + "\\labels\\" + file_without_extension + ".txt", std::ofstream::out);
          if (fw_target.is_open())
          {
            fw_target << out_str_target;
            fw_target.close();
          }
          else std::cout << "Problem with opening file '" << ".\\" + tomograph_operating_mode + "\\" + target + "\\" + mode + "\\labels\\" + file_without_extension + ".txt'" << std::endl;
        }
      }
      else
      {
        std::cout << "Expected DICOM only with one frame (" << input_img_paths[i] << " have " << num_of_frames << " frames)" << std::endl;
      }
    }
  }

  if (val_max >= val_min)
    std::cout << mode << " :: Pixel Data --- min: " + std::to_string(val_min) + ", max: " + std::to_string(val_max) << std::endl;
}

void dataset_parser(UINT16 normalize_value_start, UINT16 normalize_value_end, std::string tomograph_operating_mode, std::vector<dataset>& data, bool single_class, const unsigned char possible_values_count, const unsigned char* possible_values, double test_size, int thickness, int thickness_target, int img_mul, int img_mul_target, std::string target, bool only_dilate_and_save_masks)
{
  std::cout << "Total datasets: " << data.size() << std::endl << std::endl;

  for (size_t _t = 0; _t < data.size(); _t++)
  {
    std::cout << "Work on '" << data[_t].path << "', have_brain_mask_value: " << data[_t].have_brain_mask << ", have_target_mask: " << data[_t].have_target_mask << std::endl;

    if (boost::filesystem::exists(data[_t].path.c_str()))
    {
      std::vector<String> input_img_paths;
      glob(data[_t].path + "*.IMA", input_img_paths);

      train_test_split_result res = train_test_split(input_img_paths, test_size);
      std::cout << "train set length: " << res.train.size() << std::endl;
      std::cout << "test set length: " << res.test.size() << std::endl << std::endl;

      dataset_process(normalize_value_start, normalize_value_end, tomograph_operating_mode, res.train, data[_t], single_class, possible_values_count, possible_values, "train", thickness, thickness_target, img_mul, img_mul_target, target, only_dilate_and_save_masks);
      dataset_process(normalize_value_start, normalize_value_end, tomograph_operating_mode, res.test, data[_t], single_class, possible_values_count, possible_values, "test", thickness, thickness_target, img_mul, img_mul_target, target, only_dilate_and_save_masks);
    }
    else
    {
      std::cout << "Path '" << data[_t].path << "' don't exist" << std::endl;
    }

    std::cout << std::endl;
  }
}

int func(UINT16 normalize_value_start, UINT16 normalize_value_end, std::string tomograph_operating_mode, std::vector<dataset>& data, int argc, char* argv[], double test_size, int thickness, int thickness_target, int img_mul, int img_mul_target, std::string target)
{
  std::cout << "ExtractYoloObjectsFromDICOM.exe" << std::endl;

  std::cout << "   --- only_dilate_and_save_masks 0 or 1 (default 0); it for ischemia and msc only" << std::endl;
  bool only_dilate_and_save_masks = (argc < 2) ? false : (atoi(argv[1]) == 1);
  std::cout << "only_dilate_and_save_masks_param: '" << only_dilate_and_save_masks << "'" << std::endl;

  std::cout << "   --- single_class_param 0 or 1 (default 1)" << std::endl;
  bool single_class = (argc < 3) ? true : (atoi(argv[2]) == 1);
  std::cout << "single_class_param: '" << single_class << "'" << std::endl;

  std::cout << std::endl;

  const unsigned char possible_values[] = { 255 }; // белый объект на чёрном фоне
  const unsigned char possible_values_count = sizeof(possible_values) / sizeof(unsigned char);

  if (only_dilate_and_save_masks == false)
  {
    try
    {
      fs::remove_all(".\\" + tomograph_operating_mode + "\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\");

      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + "debug\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + "debug\\");

      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + "brain\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + "brain\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + "brain\\train\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + "brain\\train\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + "brain\\test\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + "brain\\test\\");

      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + "brain\\train\\images\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + "brain\\train\\images\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + "brain\\train\\labels\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + "brain\\train\\labels\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + "brain\\test\\images\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + "brain\\test\\images\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + "brain\\test\\labels\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + "brain\\test\\labels\\");

      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + target + "\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + target + "\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + target + "\\train\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + target + "\\train\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + target + "\\test\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + target + "\\test\\");

      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + target + "\\train\\images\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + target + "\\train\\images\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + target + "\\train\\labels\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + target + "\\train\\labels\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + target + "\\test\\images\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + target + "\\test\\images\\");
      fs::remove_all(".\\" + tomograph_operating_mode + "\\" + target + "\\test\\labels\\");
      fs::create_directory(".\\" + tomograph_operating_mode + "\\" + target + "\\test\\labels\\");
    }
    catch (fs::filesystem_error const& e)
    {
      std::cout << "Please, close all old opened files in " << e.path1() << std::endl;

      return -1;
    }
  }

  dataset_parser(normalize_value_start, normalize_value_end, tomograph_operating_mode, data, single_class, possible_values_count, &possible_values[0], test_size, thickness, thickness_target, img_mul, img_mul_target, target, only_dilate_and_save_masks);

  return 0;
}

int main(int argc, char* argv[])
{
  // Параметры датасета: [путь к папке] [задает область мозга через png] [задает область ишемии или стволовые клетки через png]
  std::vector<dataset> t2_data = { dataset(".\\t2_brain_ischemia\\", true, true),
                                   dataset(".\\t2_brain\\", true, false),
                                   dataset(".\\t2_no_ischemia\\", false, true)
  };

  std::vector<dataset> adc_data = { dataset(".\\adc_brain_ischemia\\", true, true),
                                    dataset(".\\adc_no_ischemia\\", false, true)
  };

  std::vector<dataset> swi_data = { dataset(".\\swi_brain_msc\\", true, true) };

  std::cout << "Processing T2-data" << std::endl;
  int ret_t2 = func(25, 855, "t2_data", t2_data, argc, argv, 15.0, 2, 2, 8, 8, "ischemia");

  std::cout << "Processing ADC-data" << std::endl;
  int ret_adc = func(0, 855, "adc_data", adc_data, argc, argv, 15.0, 2, 2, 8, 8, "ischemia");

  std::cout << "Processing SWI-data" << std::endl;
  int ret_swi = func(25, 383, "swi_data", swi_data, argc, argv, 15.0, 2, 1, 8, 8, "msc");

  return (ret_t2 == -1 || ret_adc == -1 || ret_swi == -1) ? -1 : 0;
}
