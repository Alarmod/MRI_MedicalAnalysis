#ifndef THREAD3D_H
#define THREAD3D_H

//#define show_texture // показывать текстуру

#define show_borders   // показывать границы областей (синим цветом)
#define show_cells     // визуализировать скопления стволовых клеток

#include <QThread>

#define start_distance 120.0F

#include <irrlicht.h>

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

#include "seidel/geometry.h"
#include "seidel/parse.h"

struct IntPoint
{
  int x;
  int y;
};

struct MRTPoint
{
  float x;
  float y;

  float polar_angle;

  void setAngle(MRTPoint & center)
  {
    polar_angle = std::atan2(center.y - y, center.x - x);

    if (polar_angle < 0) polar_angle += 6.283185308F;

    polar_angle = polar_angle * 180.0F / 3.141592654F;
  }
};

using namespace dicom;

using namespace irr;           //
using namespace core;          //  Здесь указываем пространства имен движка, для того, чтобы
using namespace scene;         //     не указывать их каждый раз, когда обращаемся
using namespace video;         //        к объекту движка.
using namespace io;            //
using namespace gui;           //

struct S3DVertexesQuad
{
  S3DVertex v0;
  S3DVertex v1;
  S3DVertex v2;
  S3DVertex v3;
};

#include "concave.h"

class CEventReceiver : public IEventReceiver
{
public:
  // This is the one method that we have to implement
  virtual bool OnEvent(const SEvent& event)
  {
    if (event.EventType == EET_KEY_INPUT_EVENT)
    {
      // if user press PLUS arrow
      if (event.KeyInput.Key == KEY_PLUS)
      {
        if (!m_keys[KEY_PLUS])
        {
          m_keys[KEY_PLUS] = true;

          const vector3df & cur = m_camera->getPosition();
          if (fabs(cur.X) < 25.0F) return false;

          m_camera->setPosition(cur * vector3df(0.95F, 0.95F, 0.95F));
        }
        else if (event.KeyInput.PressedDown == false)
        {
          m_keys[KEY_PLUS] = false;
        }
      }

      // if user press MINUS arrow
      if (event.KeyInput.Key == KEY_MINUS)
      {
        if (!m_keys[KEY_MINUS])
        {
          m_keys[KEY_MINUS] = true;

          const vector3df & cur = m_camera->getPosition();
          if (fabs(cur.X) > 2.125F * start_distance) return false;

          m_camera->setPosition(cur * vector3df(1.0F / 0.95F, 1.0F / 0.95F, 1.0F / 0.95F));
        }
        else if (event.KeyInput.PressedDown == false)
        {
          m_keys[KEY_MINUS] = false;
        }
      }

      // if user press RIGHT arrow
      if (event.KeyInput.Key == KEY_RIGHT)
      {
        if (!m_keys[KEY_RIGHT])
        {
          m_keys[KEY_RIGHT] = true;

          m_root->setRotation(m_root->getRotation() + vector3df(0, -5.0F, 0));
        }
        else if (event.KeyInput.PressedDown == false)
        {
          m_keys[KEY_RIGHT] = false;
        }
      }

      // if user press LEFT arrow
      if (event.KeyInput.Key == KEY_LEFT)
      {
        if (!m_keys[KEY_LEFT])
        {
          m_keys[KEY_LEFT] = true;

          m_root->setRotation(m_root->getRotation() + vector3df(0, 5.0F, 0));
        }
        else if (event.KeyInput.PressedDown == false)
        {
          m_keys[KEY_LEFT] = false;
        }
      }

      // if user press UP arrow
      if (event.KeyInput.Key == KEY_UP)
      {
        if (!m_keys[KEY_UP])
        {
          m_keys[KEY_UP] = true;

          m_root->setRotation(m_root->getRotation() + vector3df(0, 0, 5.0F));
          //m_root->setRotation(m_root->getRotation() + vector3df(0, 0, 0.5F));
        }
        else if (event.KeyInput.PressedDown == false)
        {
          m_keys[KEY_UP] = false;
        }
      }

      // if user press DOWN arrow
      if (event.KeyInput.Key == KEY_DOWN)
      {
        if (!m_keys[KEY_DOWN])
        {
          m_keys[KEY_DOWN] = true;

          m_root->setRotation(m_root->getRotation() + vector3df(0, 0, -5.0F));
          //m_root->setRotation(m_root->getRotation() + vector3df(0, 0, -0.5F));
        }
        else if (event.KeyInput.PressedDown == false)
        {
          m_keys[KEY_DOWN] = false;
        }
      }

      // if user press INSERT arrow
      if (event.KeyInput.Key == KEY_INSERT)
      {
        if (!m_keys[KEY_INSERT])
        {
          m_keys[KEY_INSERT] = true;

          m_root->setRotation(m_root->getRotation() + vector3df(5.0F, 0, 0));
        }
        else if (event.KeyInput.PressedDown == false)
        {
          m_keys[KEY_INSERT] = false;
        }
      }

      // if user press DELETE arrow
      if (event.KeyInput.Key == KEY_DELETE)
      {
        if (!m_keys[KEY_DELETE])
        {
          m_keys[KEY_DELETE] = true;

          m_root->setRotation(m_root->getRotation() + vector3df(-5.0F, 0, 0));
        }
        else if (event.KeyInput.PressedDown == false)
        {
          m_keys[KEY_DELETE] = false;
        }
      }
    }
    return false;
  }
  // This is used to check whether a key is being held down
  virtual bool IsKeyDown(EKEY_CODE keyCode) const
  {
    return KeyIsDown[keyCode];
  }

  // the default class constructor
  CEventReceiver()
  {
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
      KeyIsDown[i] = false;
  }
  // the custom class constructor
  CEventReceiver(ICameraSceneNode* camera, ISceneNode* root)
  {
    m_camera = camera;
    m_root = root;

    for (s32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
    {
      m_keys[i] = false;
    }
  }

private:
  // We use this array to store the current state of each key
  bool KeyIsDown[KEY_KEY_CODES_COUNT];

  ICameraSceneNode* m_camera;
  ISceneNode* m_root;

  bool m_keys[KEY_KEY_CODES_COUNT];
};

void validate_points(Mat * cv_arr_local, Mat * mask, std::vector<std::pair<int, int>> * data_for_check, int & label)
{
  std::vector<std::pair<int, int>> data_for_check_new;

  std::vector<std::pair<int, int>> * main_ptr = data_for_check;
  std::vector<std::pair<int, int>> * slave_ptr = &data_for_check_new;
  std::vector<std::pair<int, int>> * t_ptr;

  while (true)
  {
    bool data_is_modified = false;

    for (size_t _t = 0; _t < main_ptr->size(); _t++)
    {
      int L = main_ptr->operator[](_t).first;
      int P = main_ptr->operator[](_t).second;

      if (cv_arr_local->at<uchar>(L, P) == 0)
      {
        mask->at<uchar>(L, P) = label;

        for (short y = -1; y <= 1; y++)
        {
          if ((L + y) > -1 && (L + y) < cv_arr_local->rows)
          {
            for (short x = -1; x <= 1; x++)
            {
              if ((P + x) > -1 && (P + x) < cv_arr_local->cols)
              {
                if ((x == -1 && y == 0) || (x == 1 && y == 0) || (x == 0 && y == -1) || (x == 0 && y == 1))
                {
                  if (cv_arr_local->at<uchar>(L + y, P + x) == 0 && mask->at<uchar>(L + y, P + x) == 0)
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

                    mask->at<uchar>(p.first, p.second) = label;

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

void findCaverns(Mat * cv_arr_local, std::vector<Mat> & caverns, std::vector<int> & xMinCaverns, std::vector<int> & xMaxCaverns, std::vector<int> & yMinCaverns, std::vector<int> & yMaxCaverns)
{
  Mat mask = Mat::zeros(cv_arr_local->rows, cv_arr_local->cols, CV_8U);
  int label = 1;

  std::vector<std::pair<int, int>> data_for_check;

  for (int L = 0; L < cv_arr_local->rows; L++)
  {
    for (int P = 0; P < cv_arr_local->cols; P++)
    {
      if (cv_arr_local->at<uchar>(L, P) == 0 && mask.at<uchar>(L, P) == 0)
      {
        std::vector<std::pair<int, int>> data_for_check;
        std::pair<int, int> p;
        p.first = L;
        p.second = P;
        data_for_check.push_back(p);

        validate_points(cv_arr_local, &mask, &data_for_check, label);
      }
    }
  }

  qWarning("labels: %d", label);

  int *S = new int[label];
  int *XMin = new int[label];
  int *XMax = new int[label];
  int *YMin = new int[label];
  int *YMax = new int[label];

  memset(S, 0, label*sizeof(int));
  memset(XMax, 0, label*sizeof(int));
  memset(YMax, 0, label*sizeof(int));

  for (int i = 2; i < label; i++)
  {
    XMin[i] = INT_MAX;

    YMin[i] = INT_MAX;
  }

  for (int i = 2; i < label; i++)
  {
    for (int L = 0; L < cv_arr_local->rows; L++)
    {
      for (int P = 0; P < cv_arr_local->cols; P++)
      {
        if (mask.at<uchar>(L, P) == i)
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
  for (int i = 2; i < label; i++)
  {
    if (XMin[i]) XMin[i]--;
    if (XMax[i] < cv_arr_local->cols) XMax[i]++;

    if (YMin[i]) YMin[i]--;
    if (YMax[i] < cv_arr_local->rows) YMax[i]++;
  }

  for (int i = 2; i < label; i++)
  {
    if (S[i] > 2U)
    {
      unsigned short width_dcm_local = XMax[i] - XMin[i] + 1, height_dcm_local = YMax[i] - YMin[i] + 1;

      Mat cavern = Mat::zeros(height_dcm_local, width_dcm_local, CV_8U);

      for (int L = YMin[i] + 1; L < YMax[i]; L++)
      {
        for (int P = XMin[i] + 1; P < XMax[i]; P++)
        {
          if (mask.at<uchar>(L, P) == i)
            cavern.at<uchar>(L - YMin[i], P - XMin[i]) = 255U;
        }
      }

      caverns.push_back(cavern);

      qWarning("XMin[%d]: %d", i, XMin[i]);
      qWarning("YMin[%d]: %d", i, YMin[i]);

      xMinCaverns.push_back(XMin[i]);
      xMaxCaverns.push_back(XMax[i]);
      yMinCaverns.push_back(YMin[i]);
      yMaxCaverns.push_back(YMax[i]);
    }
  }

  delete[]S;
  delete[]XMin;
  delete[]XMax;
  delete[]YMin;
  delete[]YMax;
}

class Thread3D : public QThread
{
public:
  Thread3D(QObject * parent_var, int rowtype_var, int mode_var, dicom::DataSet * ds_var, dicom::DataSet * ds_mask_1_var, dicom::DataSet * ds_mask_2_var, int ind1_var, int ind2_var, int offset_var) // маски: стволовые клетки, ишемия
  {
    setParent(parent_var);

    rowtype = rowtype_var;
    mode = mode_var;
    ds = ds_var;
    ds_mask_1 = ds_mask_1_var;
    ds_mask_2 = ds_mask_2_var;

    stop_flag = 0U;

    ind1 = ind1_var;
    ind2 = ind2_var;
    offset = offset_var;
  };

  unsigned int stop_flag;

  void stop()
  {
    stop_flag = 1U;
  }

private:
  int rowtype;
  int mode;
  dicom::DataSet * ds;

  dicom::DataSet * ds_mask_1;
  dicom::DataSet * ds_mask_2;

  int ind1, ind2;
  int offset;

protected:
  void run()
  {
    const std::vector<UINT16>& pixel_data = ds->operator()(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();
    size_t ss = pixel_data.size();

    unsigned short width_dcm = 0U, height_dcm = 0U;
    if (ds->exists(TAG_COLUMNS)) ds->operator()(TAG_COLUMNS) >> width_dcm;
    if (ds->exists(TAG_ROWS)) ds->operator()(TAG_ROWS) >> height_dcm;

    size_t area = width_dcm * height_dcm;

    unsigned short width_dcm_combined = 0U, height_dcm_combined = 0U;
    size_t area_combined = 0;
    qreal combined_coeff_height = 1.0, combined_coeff_width = 1.0;

    if (ds_mask_2)
    {
      if (ds_mask_2->exists(TAG_COLUMNS)) ds_mask_2->operator()(TAG_COLUMNS) >> width_dcm_combined;
      if (ds_mask_2->exists(TAG_ROWS)) ds_mask_2->operator()(TAG_ROWS) >> height_dcm_combined;

      area_combined = width_dcm_combined * height_dcm_combined;

      combined_coeff_height = qreal(height_dcm_combined) / qreal(height_dcm), combined_coeff_width = qreal(width_dcm_combined) / qreal(width_dcm);
    }

    const float braindeep = 1.95F * start_distance;
    const float half_of_braindeep = braindeep / 2.0F;

    std::string a = QCoreApplication::applicationDirPath().toStdString() + std::string("\\");

    CHAR buff[MAX_PATH];
    sprintf(buff, "%scell.jpg", a.c_str());

    size_t brain_sections = ss / area;

    size_t start_value;
    size_t end_value;
    if (ind1 == -1)
    {
      start_value = 0;

      end_value = brain_sections;
    }
    else
    {
      start_value = ind1;

      end_value = ind2 + 1;
    }

    MRTPoint * centers_super = new MRTPoint[brain_sections];
    for (size_t j = 0; j < brain_sections; j++)
    {
      MRTPoint & cur = centers_super[j];

      cur.x = 0;
      cur.y = 0;
    }

    std::vector<MRTPoint> * points = new std::vector<MRTPoint>[brain_sections];
    std::vector< std::vector<MRTPoint> > * points_ishemia = new std::vector< std::vector<MRTPoint> >[brain_sections]; // контур верхнего уровня
    std::vector< std::vector< std::vector<MRTPoint> > > * points_ishemia_subconturs = new std::vector< std::vector< std::vector<MRTPoint> > >[brain_sections]; // вложенные контуры

    double m_value = (double)braindeep / (double)std::max<size_t>(width_dcm, height_dcm);

    unsigned long int index_of_first_pixel_in_section, index_of_first_pixel_in_section_wo_offset;
    for (size_t j = 0; j < brain_sections; j++)
    {
      Mat cv_arr = Mat::zeros(height_dcm, width_dcm, CV_8U);
      Mat canny_output;

      ConcavePointVector concave_points;

      const std::vector<UINT16>& pixel_data = ds->operator()(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

      MRTPoint & cur = centers_super[j];
      index_of_first_pixel_in_section = j * area;

      for (short y = 0; y < height_dcm; y++)
      {
        for (short x = 0; x < width_dcm; x++)
        {
          if (pixel_data[index_of_first_pixel_in_section] == 0U)
            cv_arr.at<uchar>(y, x) = 255U;

          index_of_first_pixel_in_section++;
        }
      }

      Canny(cv_arr, canny_output, 55, 200, 3);

      for (unsigned short y = 0; y < height_dcm; y++)
      {
        for (unsigned short x = 0; x < width_dcm; x++)
        {
          if (canny_output.at<uchar>(y, x) == 255U)
          {
            ConcavePoint p;
            p.x = x;
            p.y = y;

            concave_points.push_back(p);
          }
        }
      }

      IdentifyConcavePoints(concave_points);
      ConcavePointVector hull = ConcaveHull(concave_points, 3, true);

      for (size_t _t = 0; _t < hull.size(); _t++)
      {
        MRTPoint xp;
        xp.x = hull[_t].x;
        xp.y = hull[_t].y;

        points[j].push_back(xp);
      }

      for (size_t _t = 0; _t < points[j].size(); _t++)
      {
        cur.x += points[j][_t].x;

        cur.y += points[j][_t].y;
      }
    }

    std::vector<int> valid_sections;

    std::vector<Pointbase> * triangles = new std::vector<Pointbase>[brain_sections];

    S3DVertex ** triangles_vert = new S3DVertex *[brain_sections];
    memset(triangles_vert, 0, brain_sections * sizeof(S3DVertex *));

    u16 ** triangles_ind = new u16 *[brain_sections];
    memset(triangles_ind, 0, brain_sections * sizeof(u16 *));

    S3DVertex ** triangles_vert_2 = new S3DVertex *[brain_sections];
    memset(triangles_vert_2, 0, brain_sections * sizeof(S3DVertex *));

    // ишемические поражения
    if (ds_mask_2)
    {
      char wname[256];
      for (size_t j = start_value; j < end_value; j++)
      {
        const unsigned short img_mul = 4U;

        Mat cv_arr = Mat::zeros(height_dcm + 2, width_dcm + 2, CV_8U);

        const std::vector<UINT16>& pixel_data = ds_mask_2->operator()(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

        index_of_first_pixel_in_section = (j + offset) * area_combined;
        index_of_first_pixel_in_section_wo_offset = j * area;

        if (rowtype == 3)
        {
          const std::vector<UINT16>& pixel_data_brain = ds->operator()(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

          for (unsigned short y = 0; y < height_dcm; y++)
          {
            size_t yoff1 = index_of_first_pixel_in_section + floor(y * combined_coeff_height + 0.5) * width_dcm_combined;
            size_t yoff2 = index_of_first_pixel_in_section_wo_offset + y * width_dcm;

            for (unsigned short x = 0; x < width_dcm; x++)
            {
              if (pixel_data[yoff1 + floor(x * combined_coeff_width + 0.5)] == 255U && pixel_data_brain[yoff2 + x] == 0U)
                cv_arr.at<uchar>(y + 1, x + 1) = 255U;
            }
          }
        }
        else
        {
          for (unsigned short y = 0; y < height_dcm; y++)
          {
            size_t yoff = index_of_first_pixel_in_section + floor(y * combined_coeff_height + 0.5) * width_dcm_combined;

            for (unsigned short x = 0; x < width_dcm; x++)
            {
              if (pixel_data[yoff + x * combined_coeff_width] == 255U)
                cv_arr.at<uchar>(y + 1, x + 1) = 255U;
            }
          }
        }

        // выделение независимых зон 1
        {
          Mat background_mask = Mat::zeros(cv_arr.rows, cv_arr.cols, CV_8U); // маска, на которой единицами будет выделен фон
          Mat objects_mask_labeled = Mat::zeros(cv_arr.rows, cv_arr.cols, CV_8U); // маска, на которой будут промаркированные объекты

          std::vector<std::pair<int, int>> data_for_check;
          std::pair<int, int> p;
          p.first = 0;
          p.second = 0;
          data_for_check.push_back(p);

          std::vector<std::pair<int, int>> data_for_check_new;

          std::vector<std::pair<int, int>> * main_ptr = &data_for_check;
          std::vector<std::pair<int, int>> * slave_ptr = &data_for_check_new;
          std::vector<std::pair<int, int>> * t_ptr;

          // процесс поиска фона
          while (true)
          {
            bool data_is_modified = false;

            for (size_t _t = 0; _t < main_ptr->size(); _t++)
            {
              int L = main_ptr->operator[](_t).first;
              int P = main_ptr->operator[](_t).second;

              if (cv_arr.at<uchar>(L, P) == 0)
              {
                background_mask.at<uchar>(L, P) = 1U;

                for (short y = -1; y <= 1; y++)
                {
                  if ((L + y) > -1 && (L + y) < cv_arr.rows)
                  {
                    for (short x = -1; x <= 1; x++)
                    {
                      if ((P + x) > -1 && (P + x) < cv_arr.cols)
                      {
                        if ((x == -1 && y == 0) || (x == 1 && y == 0) || (x == 0 && y == -1) || (x == 0 && y == 1))
                        {
                          if (cv_arr.at<uchar>(L + y, P + x) == 0 && background_mask.at<uchar>(L + y, P + x) == 0)
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

                            background_mask.at<uchar>(p.first, p.second) = 1U;

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
              if (background_mask.at<uchar>(L, P) == 0 && objects_mask_labeled.at<uchar>(L, P) == 0)
              {
                std::vector<std::pair<int, int>> data_for_check;
                std::pair<int, int> p;
                p.first = L;
                p.second = P;
                data_for_check.push_back(p);

                validate_points(&background_mask, &objects_mask_labeled, &data_for_check, objects_label);
              }
            }
          }

          int *S = new int[objects_label];
          int *XMin = new int[objects_label];
          int *XMax = new int[objects_label];
          int *YMin = new int[objects_label];
          int *YMax = new int[objects_label];

          memset(S, 0, objects_label*sizeof(int));
          memset(XMax, 0, objects_label*sizeof(int));
          memset(YMax, 0, objects_label*sizeof(int));

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
                if (objects_mask_labeled.at<uchar>(L, P) == i)
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
            if (XMin[i]) XMin[i]--;
            if (XMax[i] < objects_mask_labeled.cols) XMax[i]++;

            if (YMin[i]) YMin[i]--;
            if (YMax[i] < objects_mask_labeled.rows) YMax[i]++;
          }

          for (int i = 1; i < objects_label; i++)
          {
            if (S[i] > 2U)
            {
              ConcavePointVector concave_points;

              unsigned short width_dcm_local = XMax[i] - XMin[i] + 1, height_dcm_local = YMax[i] - YMin[i] + 1;

              Mat cv_arr_local = Mat::zeros(height_dcm_local, width_dcm_local, CV_8U);

              for (int L = YMin[i] + 1; L < YMax[i]; L++)
              {
                for (int P = XMin[i] + 1; P < XMax[i]; P++)
                {
                  if (objects_mask_labeled.at<uchar>(L, P) == i)
                    cv_arr_local.at<uchar>(L - YMin[i], P - XMin[i]) = cv_arr.at<uchar>(L, P);
                }
              }

              std::vector<Mat> caverns;
              std::vector<int> xMinCaverns;
              std::vector<int> xMaxCaverns;
              std::vector<int> yMinCaverns;
              std::vector<int> yMaxCaverns;
              findCaverns(&cv_arr_local, caverns, xMinCaverns, xMaxCaverns, yMinCaverns, yMaxCaverns);
              qWarning("%d :: on object %d finded caverns: %d", j, i, (int)caverns.size());

              std::vector< std::vector<MRTPoint> > mrt_caverns;
              for (size_t cavernid = 0; cavernid < caverns.size(); cavernid++)
              {
                Mat & cur_cavern = caverns[cavernid];

                // пробежаться по слою и закрыть дыры
                for (int L = 1; L < cur_cavern.rows - 1; L++)
                {
                  for (int P = 1; P < cur_cavern.cols - 1; P++)
                  {
                    if (cur_cavern.at<uchar>(L, P) == 255U)
                      cv_arr_local.at<uchar>(yMinCaverns[cavernid] + L, xMinCaverns[cavernid] + P) = 255U;
                  }
                }

                // выделение контура каверны, его покрытие оболочкой и ее триангуляция
                {
                  ConcavePointVector concave_points_caverns;
                  Mat canny_output;

                  if (img_mul == 1U)
                  {
                    Canny(cur_cavern, canny_output, 55, 200, 3);

                    for (unsigned short y = 0; y < cur_cavern.rows; y++)
                    {
                      for (unsigned short x = 0; x < cur_cavern.cols; x++)
                      {
                        if (canny_output.at<uchar>(y, x) == 255U)
                        {
                          ConcavePoint p;
                          p.x = x + XMin[i] + xMinCaverns[cavernid];
                          p.y = y + YMin[i] + yMinCaverns[cavernid];

                          concave_points_caverns.push_back(p);
                        }
                      }
                    }

                    //sprintf(wname, "[0] Window %d, object %d, cavern %d", j, i, (int)cavernid);
                    //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
                    //imshow(wname, cur_cavern);
                  }
                  else
                  {
                    Mat cv_arr_resized = Mat(img_mul * cur_cavern.rows, img_mul * cur_cavern.cols, CV_8U);
                    cv::resize(cur_cavern, cv_arr_resized, cv_arr_resized.size(), 0, 0, CV_INTER_NN);
                    Canny(cv_arr_resized, canny_output, 55, 200, 3);

                    for (unsigned short y = 0; y < cv_arr_resized.rows; y++)
                    {
                      for (unsigned short x = 0; x < cv_arr_resized.cols; x++)
                      {
                        if (canny_output.at<uchar>(y, x) == 255U)
                        {
                          ConcavePoint p;
                          p.x = (x + 1) / qreal(img_mul) + XMin[i] + xMinCaverns[cavernid] - 1;
                          p.y = (y + 1) / qreal(img_mul) + YMin[i] + yMinCaverns[cavernid] - 1;

                          concave_points_caverns.push_back(p);
                        }
                      }
                    }

                    //sprintf(wname, "[0] Window %d, object %d, cavern %d", j, i, (int)cavernid);
                    //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
                    //imshow(wname, cv_arr_resized);
                  }

                  //qWarning("concave_points_caverns.size(): %d", (int)concave_points_caverns.size());

                  //sprintf(wname, "[1] Window %d, object %d, cavern %d", j, i, (int)cavernid);
                  //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
                  //imshow(wname, canny_output);

                  IdentifyConcavePoints(concave_points_caverns);
                  ConcavePointVector hull = ConcaveHull(concave_points_caverns, 3, true);

                  //qWarning("caverns hull.size(): %d", (int)hull.size());

                  if (hull.size() > 2U)
                  {
                    std::vector<MRTPoint> local_objects;
                    for (size_t _t = 0; _t < hull.size(); _t++)
                    {
                      ConcavePoint & cur_concave_point = hull[_t];

                      MRTPoint xp;
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
                        std::vector<MRTPoint> cur_vector_swap;
                        for (size_t a = best_point_index; a < hull.size(); a++)
                          cur_vector_swap.push_back(local_objects[a]);

                        for (size_t a = 0; a < best_point_index - 1; a++)
                          cur_vector_swap.push_back(local_objects[a]);

                        local_objects.clear();
                        for (size_t a = 0; a < cur_vector_swap.size(); a++)
                          local_objects.push_back(cur_vector_swap[a]);
                      }
                    }

                    mrt_caverns.push_back(local_objects);
                  }
                }
              }

              points_ishemia_subconturs[j].push_back(mrt_caverns);

              // работа с основным контуром
              Mat canny_output;

              if (img_mul == 1U)
              {
                Canny(cv_arr_local, canny_output, 55, 200, 3);

                for (unsigned short y = 0; y < height_dcm_local; y++)
                {
                  for (unsigned short x = 0; x < width_dcm_local; x++)
                  {
                    if (canny_output.at<uchar>(y, x) == 255U)
                    {
                      ConcavePoint p;
                      p.x = x + XMin[i];
                      p.y = y + YMin[i];

                      concave_points.push_back(p);
                    }
                  }
                }

                //sprintf(wname, "[0] Window %d, object %d", j, i);
                //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
                //imshow(wname, cv_arr_local);
              }
              else
              {
                Mat cv_arr_resized = Mat(img_mul * height_dcm_local, img_mul * width_dcm_local, CV_8U);
                cv::resize(cv_arr_local, cv_arr_resized, cv_arr_resized.size(), 0, 0, CV_INTER_NN);
                Canny(cv_arr_resized, canny_output, 55, 200, 3);

                for (unsigned short y = 0; y < img_mul * height_dcm_local; y++)
                {
                  for (unsigned short x = 0; x < img_mul * width_dcm_local; x++)
                  {
                    if (canny_output.at<uchar>(y, x) == 255U)
                    {
                      ConcavePoint p;
                      p.x = (x + 1) / qreal(img_mul) + XMin[i] - 1;
                      p.y = (y + 1) / qreal(img_mul) + YMin[i] - 1;

                      concave_points.push_back(p);
                    }
                  }
                }

                //sprintf(wname, "[0] Window %d, object %d", j, i);
                //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
                //imshow(wname, cv_arr_resized);
              }

              //qWarning("concave_points.size(): %d", (int)concave_points.size());

              //sprintf(wname, "[1] Window %d, object %d", j, i);
              //namedWindow(wname, WINDOW_NORMAL);// Create a window for display.
              //imshow(wname, canny_output);

              IdentifyConcavePoints(concave_points);
              ConcavePointVector hull = ConcaveHull(concave_points, 3, true);

              //qWarning("hull.size(): %d", (int)hull.size());

              if (hull.size() > 2U)
              {
                std::vector<MRTPoint> local_objects;
                for (size_t _t = 0; _t < hull.size(); _t++)
                {
                  ConcavePoint & cur_concave_point = hull[_t];

                  MRTPoint xp;
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
                    std::vector<MRTPoint> cur_vector_swap;
                    for (size_t a = best_point_index; a < hull.size(); a++)
                      cur_vector_swap.push_back(local_objects[a]);

                    for (size_t a = 0; a < best_point_index - 1; a++)
                      cur_vector_swap.push_back(local_objects[a]);

                    local_objects.clear();
                    for (size_t a = 0; a < cur_vector_swap.size(); a++)
                      local_objects.push_back(cur_vector_swap[a]);
                  }
                }

                points_ishemia[j].push_back(local_objects);

                // триангуляция Зейделя
                {
                  long int l_id = 0, p_id = 0;

                  unsigned int real_v_size = 0U;
                  BDMFile BDM;

                  for (size_t _t = 0; _t < hull.size(); _t++)
                  {
                    ConcavePoint & cur_concave_point = hull[_t];

                    if (!BDM.is_exist(cur_concave_point.x, cur_concave_point.y))
                    {
                      real_v_size++;

                      Pointbase* point = new Pointbase(real_v_size, cur_concave_point.x, cur_concave_point.y, SINPUT);
                      if (cur_concave_point.x > BDM._xmax) BDM._xmax = cur_concave_point.x;
                      if (cur_concave_point.x < BDM._xmin) BDM._xmin = cur_concave_point.x;
                      if (cur_concave_point.y > BDM._ymax) BDM._ymax = cur_concave_point.y;
                      if (cur_concave_point.y < BDM._ymin) BDM._ymin = cur_concave_point.y;

                      BDM._points[real_v_size] = point;
                    }
                  }

                  BDM._nVertices.push_back(real_v_size);
                  BDM._ncontours++;

                  // каверны на текущем срезе
                  for (size_t _q = 0; _q < mrt_caverns.size(); _q++)
                  {
                    std::vector<MRTPoint> & subcontur = mrt_caverns[_q]; // точки контура каверны

                    unsigned int cavern_real_v_size = 0U;
                    for (int _t = (int)subcontur.size() - 1; _t >= 0; _t--)
                    {
                      MRTPoint & cur_mrt_point = subcontur[_t];

                      if (!BDM.is_exist(cur_mrt_point.x, cur_mrt_point.y))
                      {
                        cavern_real_v_size++;
                        real_v_size++;

                        Pointbase* point = new Pointbase(real_v_size, cur_mrt_point.x, cur_mrt_point.y, SINPUT);
                        if (cur_mrt_point.x > BDM._xmax) BDM._xmax = cur_mrt_point.x;
                        if (cur_mrt_point.x < BDM._xmin) BDM._xmin = cur_mrt_point.x;
                        if (cur_mrt_point.y > BDM._ymax) BDM._ymax = cur_mrt_point.y;
                        if (cur_mrt_point.y < BDM._ymin) BDM._ymin = cur_mrt_point.y;

                        BDM._points[real_v_size] = point;
                      }
                    }

                    BDM._nVertices.push_back(cavern_real_v_size);
                    BDM._ncontours++;

                    //qWarning("cavern_real_v_size: %u", cavern_real_v_size);
                  }

                  // инициализация допструктур
                  {
                    int sid, eid;
                    int num = 0;

                    for (unsigned int jj = 0; jj < BDM._ncontours; jj++)
                    {
                      for (unsigned int ii = 1; ii <= BDM._nVertices[jj]; ii++)
                      {
                        sid = num + ii;
                        eid = (ii == BDM._nVertices[jj]) ? num + 1 : num + ii + 1;

                        Linebase* line = new Linebase(BDM._points[sid], BDM._points[eid], SINPUT, l_id);
                        BDM._edges[l_id] = line;
                      }
                      num += BDM._nVertices[jj];
                    }

                    int sum = 0;
                    for (unsigned int ii = 0; ii < BDM._ncontours; ii++)
                    {
                      sum += BDM._nVertices[ii];

                      BDM._nVertices[ii] = sum;
                    }

                    p_id = num;
                  }

                  SPolygon poly(BDM);
                  poly.setDebugOption(false);
                  poly.triangulation(l_id);

                  Triangles & tr = poly._triangles;
                  qWarning("%d :: have '%d' triangles", j, (int)tr.size());

                  Pointbase *p0, *p1, *p2;
                  unsigned int counter = 0;
                  for (Triangles::iterator itt = tr.begin(); itt != tr.end(); itt++)
                  {
                    //qWarning("[0] %d :: %u - tr[0]: %u %u %u", j, counter, (*itt)[0], (*itt)[1], (*itt)[2]);             

                    p0 = BDM._points[itt->operator[](0)];
                    p1 = BDM._points[itt->operator[](1)];
                    p2 = BDM._points[itt->operator[](2)];

                    triangles[j].push_back(*p0);
                    triangles[j].push_back(*p1);
                    triangles[j].push_back(*p2);

                    //qWarning("[1] %d :: %u - tr[0]: [%.4f; %.4f], [%.4f; %.4f], [%.4f; %.4f]", j, counter, p0->x, p0->y, p1->x, p1->y, p2->x, p2->y);

                    //counter++;
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

        if (points_ishemia[j].size())
          valid_sections.push_back(j);
        else
          triangles[j].clear();
      }
    }

    size_t _t_size;
    size_t _s_size;

    // получаем глобальный центр тяжести
    float xcenter_super = 0.0F, ycenter_super = 0.0F;
    float total_centers = 0.0F;

    for (size_t j = 0; j < brain_sections; j++)
    {
      std::vector<MRTPoint> & cur_vector = points[j];

      _t_size = cur_vector.size();

      if (_t_size != 0U)
      {
        MRTPoint & cur_center = centers_super[j];
        cur_center.x = cur_center.x / (float)_t_size;
        cur_center.y = cur_center.y / (float)_t_size;

        xcenter_super += cur_center.x;
        ycenter_super += cur_center.y;

        total_centers++;
      }
    }

    if (total_centers)
    {
      xcenter_super /= total_centers;
      ycenter_super /= total_centers;
    }

    for (size_t j = 0; j < brain_sections; j++)
    {
      std::vector<MRTPoint> & cur_vector = points[j];

      _t_size = cur_vector.size();

      if (_t_size != 0U)
      {
        size_t best_point_index = 0;
        qreal best_dist = DBL_MAX;
        for (size_t a = 0; a < _t_size; a++)
        {
          qreal dist = sqrt(cur_vector[a].x * cur_vector[a].x + cur_vector[a].y * cur_vector[a].y);

          if (dist < best_dist)
          {
            best_dist = dist;

            best_point_index = a;
          }
        }

        if (best_point_index)
        {
          std::vector<MRTPoint> cur_vector_swap;
          for (size_t a = best_point_index; a < _t_size; a++)
            cur_vector_swap.push_back(cur_vector[a]);

          for (size_t a = 0; a < best_point_index - 1; a++)
            cur_vector_swap.push_back(cur_vector[a]);

          points[j].clear();
          for (size_t a = 0; a < cur_vector_swap.size(); a++)
            points[j].push_back(cur_vector_swap[a]);
        }

        for (size_t _t = 0; _t < _t_size; _t++)
        {
          MRTPoint & cpoint = cur_vector[_t];

          cpoint.x = ((double)(xcenter_super)-cpoint.x) * m_value;
          cpoint.y = (cpoint.y - (double)(ycenter_super)) * m_value;

          cpoint.polar_angle = (float)(360.0 * double(_t) / double(_t_size));
        }
      }
    }

    double width_of_band = braindeep / (brain_sections - 1);
    double band_offset = width_of_band * 0.0625;

    // нормирование точек ишемии и каверн
    std::vector< std::vector<S3DVertex *> > v_for_ishemia;
    std::vector< std::vector<S3DVertex *> > v_for_ishemia_2;
    std::vector< std::vector<S3DVertex *> > v_for_ishemia_band;
    std::vector< std::vector<u16 *> > ind_for_ishemia;
    std::vector< std::vector<u16 *> > ind_for_ishemia_band;

    std::vector< std::vector< std::vector<S3DVertex *> > > v_for_ishemia_subconturs;
    std::vector< std::vector< std::vector<S3DVertex *> > > v_for_ishemia_subconturs_2;
    std::vector< std::vector< std::vector<S3DVertex *> > > v_for_ishemia_subconturs_band;
    std::vector< std::vector< std::vector<u16 *> > > ind_for_ishemia_subconturs;
    std::vector< std::vector< std::vector<u16 *> > > ind_for_ishemia_subconturs_band;

    if (ds_mask_2)
    {
      int type_of_object = 0;
      for (size_t j = start_value; j < end_value; j++)
      {
        std::vector< std::vector<MRTPoint> > & layer_vector = points_ishemia[j]; // объекты отдельного среза

        std::vector<S3DVertex *> v_obj;
        std::vector<S3DVertex *> v_obj_2;
        std::vector<S3DVertex *> v_obj_band;
        std::vector<u16 *> ind_obj;
        std::vector<u16 *> ind_obj_band;

        std::vector< std::vector<S3DVertex *> > v_obj_caverns_top;
        std::vector< std::vector<S3DVertex *> > v_obj_caverns_top_2;
        std::vector< std::vector<S3DVertex *> > v_obj_caverns_top_band;
        std::vector< std::vector<u16 *> > ind_obj_top;
        std::vector< std::vector<u16 *> > ind_obj_top_band;

        std::vector< std::vector< std::vector<MRTPoint> > > & points_of_all_caverns_on_layer_grouped_by_objects = points_ishemia_subconturs[j]; // точки всех каверн на срезе, сгруппированные по отдельным объектам

        for (size_t q = 0; q < layer_vector.size(); q++)
        {
          std::vector< std::vector<MRTPoint> > & points_of_all_caverns_on_separate_object = points_of_all_caverns_on_layer_grouped_by_objects[q]; // точки каверн на отдельном объекте среза, сгруппированные по кавернам

          std::vector<MRTPoint> & cur_vector = layer_vector[q];

          _t_size = cur_vector.size();

          if (_t_size > 1U)
          {
            S3DVertex * v_for_ishemia_j_ptr = new S3DVertex[_t_size];
            S3DVertex * v_for_ishemia_j_ptr_2 = new S3DVertex[_t_size];
            S3DVertex * v_for_ishemia_j_ptr_band = new S3DVertex[_t_size * 2];
            u16 * ind_for_ishemia_j_ptr = new u16[_t_size];
            u16 * ind_for_ishemia_j_ptr_band = new u16[_t_size * 2 + 2];

            float xcord = 0.5F * (half_of_braindeep - width_of_band * j);

            for (size_t _t = 0; _t < _t_size * 2; _t++)
              ind_for_ishemia_j_ptr_band[_t] = _t;

            ind_for_ishemia_j_ptr_band[_t_size * 2] = 0;
            ind_for_ishemia_j_ptr_band[_t_size * 2 + 1] = 1;

            for (size_t _t = 0; _t < _t_size; _t++)
            {
              ind_for_ishemia_j_ptr[_t] = _t;

              MRTPoint & cpoint = cur_vector[_t];

              cpoint.x = ((double)(xcenter_super)-cpoint.x) * m_value;
              cpoint.y = (cpoint.y - (double)(ycenter_super)) * m_value;

              v_for_ishemia_j_ptr[_t].Pos.set(xcord - band_offset, cpoint.y, cpoint.x);
              v_for_ishemia_j_ptr[_t].Color.set(150, 0, 0, 255);

              v_for_ishemia_j_ptr_2[_t].Pos.set(xcord + band_offset, cpoint.y, cpoint.x);
              v_for_ishemia_j_ptr_2[_t].Color.set(150, 0, 0, 255);

              if (type_of_object)
              {
                v_for_ishemia_j_ptr_band[_t * 2].Pos.set(xcord - band_offset, cpoint.y, cpoint.x);
                v_for_ishemia_j_ptr_band[_t * 2].Color.set(150, 0, 255, 255);

                v_for_ishemia_j_ptr_band[_t * 2 + 1].Pos.set(xcord + band_offset, cpoint.y, cpoint.x);
                v_for_ishemia_j_ptr_band[_t * 2 + 1].Color.set(150, 0, 255, 255);
              }
              else
              {
                v_for_ishemia_j_ptr_band[_t * 2].Pos.set(xcord - band_offset, cpoint.y, cpoint.x);
                v_for_ishemia_j_ptr_band[_t * 2].Color.set(150, 255, 255, 0);

                v_for_ishemia_j_ptr_band[_t * 2 + 1].Pos.set(xcord + band_offset, cpoint.y, cpoint.x);
                v_for_ishemia_j_ptr_band[_t * 2 + 1].Color.set(150, 255, 255, 0);
              }
            }

            v_obj.push_back(v_for_ishemia_j_ptr);
            v_obj_2.push_back(v_for_ishemia_j_ptr_2);
            v_obj_band.push_back(v_for_ishemia_j_ptr_band);
            ind_obj.push_back(ind_for_ishemia_j_ptr);
            ind_obj_band.push_back(ind_for_ishemia_j_ptr_band);

            std::vector<S3DVertex *> v_obj_caverns_bottom;
            std::vector<S3DVertex *> v_obj_caverns_bottom_2;
            std::vector<S3DVertex *> v_obj_caverns_bottom_band;
            std::vector<u16 *> ind_obj_top_bottom;
            std::vector<u16 *> ind_obj_top_bottom_band;

            for (size_t _s = 0; _s < points_of_all_caverns_on_separate_object.size(); _s++)
            {
              std::vector<MRTPoint> & cavern_points = points_of_all_caverns_on_separate_object[_s];

              _t_size = cavern_points.size();

              if (_t_size > 1U)
              {
                S3DVertex * v_cavern_ptr = new S3DVertex[_t_size];
                S3DVertex * v_cavern_ptr_2 = new S3DVertex[_t_size];
                S3DVertex * v_cavern_ptr_band = new S3DVertex[_t_size * 2];
                u16 * ind_cavern_ptr = new u16[_t_size];
                u16 * ind_cavern_ptr_band = new u16[_t_size * 2 + 2];

                for (size_t _t = 0; _t < _t_size * 2; _t++)
                  ind_cavern_ptr_band[_t] = _t;

                ind_cavern_ptr_band[_t_size * 2] = 0;
                ind_cavern_ptr_band[_t_size * 2 + 1] = 1;

                for (size_t _t = 0; _t < _t_size; _t++)
                {
                  ind_cavern_ptr[_t] = _t;

                  MRTPoint & cpoint = cavern_points[_t];

                  cpoint.x = ((double)(xcenter_super)-cpoint.x) * m_value;
                  cpoint.y = (cpoint.y - (double)(ycenter_super)) * m_value;

                  v_cavern_ptr[_t].Pos.set(xcord - band_offset, cpoint.y, cpoint.x);
                  v_cavern_ptr[_t].Color.set(200, 0, 0, 255);

                  v_cavern_ptr_2[_t].Pos.set(xcord + band_offset, cpoint.y, cpoint.x);
                  v_cavern_ptr_2[_t].Color.set(200, 0, 0, 255);

                  if (type_of_object)
                  {
                    v_cavern_ptr_band[_t * 2].Pos.set(xcord - band_offset, cpoint.y, cpoint.x);
                    v_cavern_ptr_band[_t * 2].Color.set(200, 0, 255, 255);

                    v_cavern_ptr_band[_t * 2 + 1].Pos.set(xcord + band_offset, cpoint.y, cpoint.x);
                    v_cavern_ptr_band[_t * 2 + 1].Color.set(200, 0, 255, 255);
                  }
                  else
                  {
                    v_cavern_ptr_band[_t * 2].Pos.set(xcord - band_offset, cpoint.y, cpoint.x);
                    v_cavern_ptr_band[_t * 2].Color.set(200, 255, 255, 0);

                    v_cavern_ptr_band[_t * 2 + 1].Pos.set(xcord + band_offset, cpoint.y, cpoint.x);
                    v_cavern_ptr_band[_t * 2 + 1].Color.set(200, 255, 255, 0);
                  }
                }

                v_obj_caverns_bottom.push_back(v_cavern_ptr);
                v_obj_caverns_bottom_2.push_back(v_cavern_ptr_2);
                v_obj_caverns_bottom_band.push_back(v_cavern_ptr_band);
                ind_obj_top_bottom.push_back(ind_cavern_ptr);
                ind_obj_top_bottom_band.push_back(ind_cavern_ptr_band);
              }
            }

            if (v_obj_caverns_bottom.size()) v_obj_caverns_top.push_back(v_obj_caverns_bottom);
            if (v_obj_caverns_bottom_2.size()) v_obj_caverns_top_2.push_back(v_obj_caverns_bottom_2);
            if (v_obj_caverns_bottom_band.size()) v_obj_caverns_top_band.push_back(v_obj_caverns_bottom_band);
            if (ind_obj_top_bottom.size()) ind_obj_top.push_back(ind_obj_top_bottom);
            if (ind_obj_top_bottom_band.size()) ind_obj_top_band.push_back(ind_obj_top_bottom_band);
          }
        }

        if (v_obj.size())
        {
          v_for_ishemia.push_back(v_obj);
          v_for_ishemia_subconturs.push_back(v_obj_caverns_top);

          v_for_ishemia_2.push_back(v_obj_2);
          v_for_ishemia_subconturs_2.push_back(v_obj_caverns_top_2);

          v_for_ishemia_band.push_back(v_obj_band);
          v_for_ishemia_subconturs_band.push_back(v_obj_caverns_top_band);
        }

        if (ind_obj.size())
        {
          ind_for_ishemia.push_back(ind_obj);
          ind_for_ishemia_band.push_back(ind_obj_band);

          ind_for_ishemia_subconturs.push_back(ind_obj_top);
          ind_for_ishemia_subconturs_band.push_back(ind_obj_top_band);
        }

        std::vector<Pointbase> & triangles_on_layer = triangles[j];

        if (triangles_on_layer.size() > 2U)
        {
          S3DVertex * tr_ptr = triangles_vert[j] = new S3DVertex[triangles_on_layer.size()];
          S3DVertex * tr_ptr_2 = triangles_vert_2[j] = new S3DVertex[triangles_on_layer.size()];
          u16 * ind_ptr = triangles_ind[j] = new u16[triangles_on_layer.size()];

          float xcord = 0.5F * (half_of_braindeep - width_of_band * j);

          // срезы ишемии
          for (size_t _k = 0; _k < triangles_on_layer.size(); _k++)
          {
            Pointbase & point = triangles_on_layer[_k];

            S3DVertex & vert = tr_ptr[_k];
            vert.Pos.set(xcord - band_offset, (point.y - (double)(ycenter_super)) * m_value, ((double)(xcenter_super)-point.x) * m_value);
            if (type_of_object) vert.Color.set(100, 0, 255, 255);
            else vert.Color.set(100, 255, 255, 0);

            S3DVertex & vert_2 = tr_ptr_2[_k];
            vert_2.Pos.set(xcord + band_offset, (point.y - (double)(ycenter_super)) * m_value, ((double)(xcenter_super)-point.x) * m_value);
            if (type_of_object) vert_2.Color.set(100, 0, 255, 255);
            else vert_2.Color.set(100, 255, 255, 0);

            ind_ptr[_k] = _k;
          }

          type_of_object = (type_of_object + 1) % 2;
        }
      }
    }

    SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
    params.AntiAlias = 32;
    params.DriverType = EDT_OPENGL;
    params.WindowSize = core::dimension2d<u32>(7.7F * start_distance, 7.7F * start_distance);
    IrrlichtDevice *device = createDeviceEx(params);

    device->setWindowCaption(L"3D-interface");      // Указываем заголовок нашего окошка.

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    const IGeometryCreator *geomentryCreator = smgr->getGeometryCreator();

    ISceneNode* root = smgr->addEmptySceneNode();
    root->setPosition(vector3df(0, 0, 0));
    root->setRotation(vector3df(0, 0, 0));

    ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(start_distance, 0, 0), vector3df(0, 0, 0));  // Добавляем в менеджер сцены новую камеру.

    // make a CEventReceiver object
    CEventReceiver receiver(camera, root);

    // add it to the device
    device->setEventReceiver(&receiver);

    //ISceneNode* wall = smgr->addSphereSceneNode(5, 16, root);
    //wall->setMaterialFlag(EMF_LIGHTING, false);
    //wall->setMaterialTexture(0, driver->getTexture(buff));
    //wall->setPosition(vector3df(0.0F, 10.0F, 0.0F));

    //ISceneNode* wall2 = smgr->addSphereSceneNode(5, 16, root);
    //wall2->setMaterialFlag(EMF_LIGHTING, false);
    //wall2->setMaterialTexture(0, driver->getTexture(buff2));
    //wall2->setPosition(vector3df(0.0F, -10.0F, 0.0F));

    SMaterial mtl;
    mtl.Lighting = false;
    mtl.setFlag(EMF_ANISOTROPIC_FILTER, true);
    mtl.setFlag(EMF_BACK_FACE_CULLING, false);
    mtl.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

    const float angle_step = 10.0F;
    const u16 quantity_of_lines = 360.0F / angle_step;

    std::vector<std::vector<MRTPoint>> pointsData;
    std::vector<float> pointX;
    std::vector<int> pointX_indexes;

    std::vector<S3DVertex *> v_for_section;
    std::vector<u16 *> ind_for_section;

    for (int j = 0; j < brain_sections; j++)
    {
      std::vector<MRTPoint> & points_ptr = points[j];

      if (points_ptr.size() > 4U)
      {
        float xcord = 0.5F * (half_of_braindeep - width_of_band * j);

        S3DVertex * v_for_section_j_ptr = new S3DVertex[quantity_of_lines];
        u16 * ind_for_section_j_ptr = new u16[quantity_of_lines];

        for (u16 _t = 0; _t < quantity_of_lines; _t++)
        {
          float wanted_angle = angle_step * _t;
          size_t index = (size_t)floorf(wanted_angle / 360.0F * (points_ptr.size() - 2U) + 1.5F);
          float dist_angle = fabs(points_ptr[index].polar_angle - wanted_angle);

          bool direct_order = false;
          while (1)
          {
            if ((index + 2) > points_ptr.size()) break;

            float ldist = fabs(points_ptr[index + 1].polar_angle - wanted_angle);

            if (ldist < dist_angle)
            {
              dist_angle = ldist;

              direct_order = true;

              index++;
            }
            else
            {
              break;
            }
          }

          if (direct_order == false)
          {
            while (1)
            {
              if (index < 1U) break;

              float ldist = fabs(points_ptr[index - 1].polar_angle - wanted_angle);

              if (ldist < dist_angle)
              {
                dist_angle = ldist;

                index--;
              }
              else
              {
                break;
              }
            }
          }

          MRTPoint & cur_point = points_ptr[index];
          v_for_section_j_ptr[_t].Pos.set(xcord, cur_point.y, cur_point.x);
          v_for_section_j_ptr[_t].Color.set(128, 255, 240, 255);

          ind_for_section_j_ptr[_t] = _t;
        }

        v_for_section.push_back(v_for_section_j_ptr);
        ind_for_section.push_back(ind_for_section_j_ptr);

        std::vector<MRTPoint> points_for_section;

        // заполнить массив с информацией о стволовых клетках
        if (ds_mask_1 && j >= start_value && j < end_value)
        {
          unsigned long int * mask = new unsigned long[width_dcm * height_dcm];
          memset(mask, 0, sizeof(unsigned long int) * width_dcm * height_dcm);

          int label = 1;
          int MLabel = 0;

          int *Parent = new int[width_dcm * height_dcm];
          memset(Parent, 0, sizeof(int) * width_dcm * height_dcm);

          int RLength = 4;
          IntPoint R[4] = { { -1, 0 }, { 0, -1 }, { -1, -1 }, { 1, -1 } };

          size_t points_start = j * area;

          Value vpixel_points = ds_mask_1->operator()(dicom::TAG_PIXEL_DATA);
          const std::vector<UINT16>& pixel_data_points = vpixel_points.Get<std::vector<UINT16> >();

          size_t point_index_on_line;
          for (int L = 0; L < height_dcm; L++)
          {
            point_index_on_line = points_start + L * width_dcm;

            for (int P = 0; P < width_dcm; P++)
            {
              int PixelValue = pixel_data_points[point_index_on_line + P];

              if (PixelValue == 255)
              {
                bool IsEmpty = true;

                for (int i = 0; i < RLength; i++)
                  if ((P + R[i].x >= 0) && (L + R[i].y >= 0) && (P + R[i].x < width_dcm) && (L + R[i].y < height_dcm))
                  {
                    PixelValue = pixel_data_points[points_start + (L + R[i].y) * width_dcm + P + R[i].x];

                    IsEmpty = (PixelValue == 255) ? false : IsEmpty;
                  }

                if (IsEmpty == true)
                {
                  MLabel = label;
                  label = label + 1;
                }
                else
                {
                  MLabel = label + 1;
                  for (int i = 0; i < RLength; i++)
                  {
                    if ((P + R[i].x >= 0) && (L + R[i].y >= 0) && (P + R[i].x < width_dcm) && (L + R[i].y < height_dcm))
                    {
                      unsigned long int m_value = mask[(L + R[i].y) * width_dcm + P + R[i].x];

                      if ((m_value != 0) && (m_value < MLabel))
                        MLabel = m_value;
                    }
                  }
                }

                mask[L * width_dcm + P] = MLabel;

                for (int i = 0; i < RLength; i++)
                {
                  if ((P + R[i].x >= 0) && (L + R[i].y >= 0) && (P + R[i].x < width_dcm) && (L + R[i].y < height_dcm))
                  {
                    unsigned long int m_value = mask[(L + R[i].y) * width_dcm + P + R[i].x];

                    if ((m_value != MLabel) && (m_value != 0))
                    {
                      int jjj = MLabel;

                      while (Parent[jjj] != 0)
                      {
                        jjj = Parent[jjj];
                      }

                      while (Parent[m_value] != 0)
                      {
                        m_value = Parent[m_value];
                      }

                      if (jjj != m_value)
                        Parent[m_value] = jjj;
                    }
                  }
                }
              }
            }
          }

          int q = 0;
          int* Rename = new int[width_dcm * height_dcm];
          memset(Rename, 0, sizeof(int) * width_dcm * height_dcm);

          for (int i = 0; i < width_dcm * height_dcm; i++)
          {
            if (Parent[i] == 0)
            {
              Rename[i] = q;
              q++;
            }
          }

          for (int L = 0; L < height_dcm; L++)
          {
            for (int P = 0; P < width_dcm; P++)
            {
              int jjj = mask[L * width_dcm + P];

              while (Parent[jjj] != 0)
                jjj = Parent[jjj];

              mask[L * width_dcm + P] = Rename[jjj];
            }
          }

          delete[]Rename;
          delete[]Parent;

          // извлечение объектов
          {
            int max_j = 0;
            for (int L = 0; L < height_dcm; L++)
            {
              for (int P = 0; P < width_dcm; P++)
              {
                int jjj = mask[L * width_dcm + P];

                if (jjj > max_j) max_j = jjj;
              }
            }

            max_j++;

            int *S = new int[max_j];
            int *XMin = new int[max_j];
            int *XMax = new int[max_j];
            int *YMin = new int[max_j];
            int *YMax = new int[max_j];

            memset(S, 0, max_j*sizeof(int));
            memset(XMax, 0, max_j*sizeof(int));
            memset(YMax, 0, max_j*sizeof(int));

            for (int i = 0; i < max_j; i++)
            {
              XMin[i] = INT_MAX;

              YMin[i] = INT_MAX;
            }

            for (int L = 0; L < height_dcm; L++)
            {
              for (int P = 0; P < width_dcm; P++)
              {
                int jjj = mask[L * width_dcm + P];

                if (jjj > 0)
                {
                  S[jjj]++;

                  if (P < XMin[jjj]) XMin[jjj] = P;
                  if (P > XMax[jjj]) XMax[jjj] = P;
                  if (L < YMin[jjj]) YMin[jjj] = L;
                  if (L > YMax[jjj]) YMax[jjj] = L;
                }
              }
            }

            for (int i = 1; i < max_j; i++)
            {
              if (S[i] == 0) continue;

              MRTPoint xp;
              xp.x = ((double)(xcenter_super)-(XMin[i] + XMax[i]) / 2.0) * m_value;
              xp.y = ((YMin[i] + YMax[i]) / 2.0 - (double)(ycenter_super)) * m_value;

              xp.polar_angle = (S[i] + 1) * start_distance / 180.F;

              points_for_section.push_back(xp);
            }

            delete[]S;
            delete[]XMin;
            delete[]XMax;
            delete[]YMin;
            delete[]YMax;
          }

          delete[]mask;
        }

        pointsData.push_back(points_for_section);
        pointX.push_back(xcord);
        pointX_indexes.push_back(j);
      }
    }

    S3DVertex ** v_for_section_lines = 0;
    u16 ** ind_for_section_lines = 0;
    if (v_for_section.size() > 1U)
    {
      v_for_section_lines = new S3DVertex *[quantity_of_lines];
      ind_for_section_lines = new u16 *[quantity_of_lines];

      for (size_t _t = 0; _t < quantity_of_lines; _t++)
      {
        S3DVertex * vptr = v_for_section_lines[_t] = new S3DVertex[v_for_section.size()];

        u16 * iptr = ind_for_section_lines[_t] = new u16[v_for_section.size()];

        for (size_t _q = 0; _q < v_for_section.size(); _q++)
        {
          vptr[_q] = v_for_section[_q][_t];

          iptr[_q] = _q;
        }
      }
    }

    float pointMinRadius = width_of_band / 12.0F;
#ifdef show_cells
    for (size_t _t = 0; _t < pointsData.size(); _t++)
    {
      std::vector<MRTPoint> & points_for_section = pointsData[_t];

      for (size_t _q = 0; _q < points_for_section.size(); _q++)
      {
        MRTPoint & xp = points_for_section[_q];

        ISceneNode* wall_point = smgr->addSphereSceneNode(pointMinRadius * xp.polar_angle, 16, root);

        wall_point->setMaterialFlag(EMF_LIGHTING, false);
        wall_point->setMaterialTexture(0, driver->getTexture(buff));
        wall_point->setPosition(vector3df(pointX[_t], xp.y, xp.x));
      }
    }
#endif

#ifdef show_texture
    if (ds_mask_2)
    {
      const core::dimension2d<u32> cd_size(width_dcm, height_dcm);
      char buff[256];

      int type_of_object = 0;
      for (size_t _t = 0; _t < valid_sections.size(); _t++)
      {
        int j = valid_sections[_t];

        float xcord = 0.5F * (half_of_braindeep - width_of_band * j);

        sprintf(buff, "rt_%u\0", _t);
        ITexture * ltexture = driver->addTexture(cd_size, buff, ECF_A8R8G8B8);

        u32 *text_ptr = (u32*)ltexture->lock();
        u32 *text_wave_ptr = text_ptr;

        const std::vector<UINT16>& pixel_data = ds_mask_2->operator()(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

        index_of_first_pixel_in_section = (j + offset) * area_combined;
        index_of_first_pixel_in_section_wo_offset = j * area;

        if (rowtype == 3)
        {
          if (type_of_object) qWarning("Draw 1");
          else qWarning("Draw 0");

          const std::vector<UINT16>& pixel_data_brain = ds->operator()(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

          for (short y = 0; y < height_dcm; y++)
          {
            size_t yoff1 = index_of_first_pixel_in_section + floor(y * combined_coeff_height + 0.5) * width_dcm_combined;
            size_t yoff2 = index_of_first_pixel_in_section_wo_offset + y * width_dcm;

            for (short x = 0; x < width_dcm; x++)
            {
              u8 * lptr = (u8 *)text_wave_ptr + 4 * x;

              if (pixel_data[yoff1 + floor(x * combined_coeff_width + 0.5)] == 255U && pixel_data_brain[yoff2 + x] == 0U)
              {
                if (type_of_object)
                {
                  lptr[0] = 255;
                  lptr[1] = 255;
                  lptr[2] = 0;
                  lptr[3] = 100; // alpha
                }
                else
                {
                  lptr[0] = 0;
                  lptr[1] = 255;
                  lptr[2] = 255;
                  lptr[3] = 100; // alpha
                }
              }
              else
              {
                lptr[0] = 0;
                lptr[1] = 0;
                lptr[2] = 0;
                lptr[3] = 0;
              }
            }

            text_wave_ptr = (u32 *)((u8 *)text_wave_ptr + ltexture->getPitch());
          }
        }
        else
        {
          for (short y = 0; y < height_dcm; y++)
          {
            size_t yoff1 = index_of_first_pixel_in_section + floor(y * combined_coeff_height + 0.5) * width_dcm_combined;
            size_t yoff2 = index_of_first_pixel_in_section_wo_offset + y * width_dcm;

            for (short x = 0; x < width_dcm; x++)
            {
              u8 * lptr = (u8 *)text_wave_ptr + 4 * x;

              if (pixel_data[yoff1 + floor(x * combined_coeff_width + 0.5)] == 255U)
              {
                if (type_of_object)
                {
                  lptr[0] = 255;
                  lptr[1] = 255;
                  lptr[2] = 0;
                  lptr[3] = 200; // alpha
                }
                else
                {
                  lptr[0] = 0;
                  lptr[1] = 255;
                  lptr[2] = 255;
                  lptr[3] = 200; // alpha
                }
              }
              else
              {
                lptr[0] = 0;
                lptr[1] = 0;
                lptr[2] = 0;
                lptr[3] = 0;
              }
            }

            text_wave_ptr = (u32 *)((u8 *)text_wave_ptr + ltexture->getPitch());
          }
        }

        ltexture->unlock();

        {
          qreal xdiff = xcenter_super - width_dcm / 2.0;
          qreal ydiff = height_dcm / 2.0 - ycenter_super;

          S3DVertexesQuad v;
          v.v0 = S3DVertex(xcord, (-height_dcm / 2.0 + ydiff) * m_value, (-width_dcm / 2.0 + xdiff) * m_value, 1, 0, 0, SColor(255, 255, 255, 255), 1, 0);
          v.v1 = S3DVertex(xcord, (height_dcm / 2.0 + ydiff) * m_value, (-width_dcm / 2.0 + xdiff) * m_value, 1, 0, 0, SColor(255, 255, 255, 255), 1, 1);
          v.v2 = S3DVertex(xcord, (-height_dcm / 2.0 + ydiff) * m_value, (width_dcm / 2.0 + xdiff) * m_value, 1, 0, 0, SColor(255, 255, 255, 255), 0, 0);
          v.v3 = S3DVertex(xcord, (height_dcm / 2.0 + ydiff) * m_value, (width_dcm / 2.0 + xdiff) * m_value, 1, 0, 0, SColor(255, 255, 255, 255), 0, 1);

          CMeshBuffer<S3DVertex> * buffer = new CMeshBuffer<S3DVertex>;

          buffer->Vertices.push_back(v.v0);
          buffer->Vertices.push_back(v.v1);
          buffer->Vertices.push_back(v.v2);
          buffer->Vertices.push_back(v.v3);

          buffer->Indices.push_back(0);
          buffer->Indices.push_back(1);
          buffer->Indices.push_back(2);
          buffer->Indices.push_back(1);
          buffer->Indices.push_back(2);
          buffer->Indices.push_back(3);

          buffer->recalculateBoundingBox();

          SMesh *mesh = new SMesh;
          mesh->addMeshBuffer(buffer);
          buffer->drop();
          mesh->recalculateBoundingBox();

          ISceneNode *plane = smgr->addMeshSceneNode(mesh, root);
          mesh->drop();

          plane->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
          plane->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
          plane->setMaterialFlag(EMF_LIGHTING, false);
          //plane->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
          plane->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
          plane->setMaterialTexture(0, ltexture);
        }

        type_of_object = (type_of_object + 1) % 2;
      }
    }
#endif

    while (device->run())          // Это наш основной цикл. Он будет выполняться постоянно, пока работает движок.
    {
      if (stop_flag == 1U) break;

      driver->beginScene(true, true, SColor(0, 200, 200, 200));      // Драйвер начинает отрисовку сцены

      smgr->drawAll();                                               // Менеджер сцены рисует свои объекты.

      driver->setTransform(E_TRANSFORMATION_STATE::ETS_WORLD, root->getRelativeTransformation());

      driver->setMaterial(mtl);

      if (v_for_section.size() > 1U)
      {
        for (size_t _t = 0; _t < quantity_of_lines; _t++)
          driver->drawVertexPrimitiveList(v_for_section_lines[_t], v_for_section.size(), ind_for_section_lines[_t], v_for_section.size() - 1U, EVT_STANDARD, EPT_LINE_STRIP, EIT_16BIT);
      }

      for (size_t _t = 0; _t < v_for_section.size(); _t++)
        driver->drawVertexPrimitiveList(v_for_section[_t], quantity_of_lines, ind_for_section[_t], quantity_of_lines, EVT_STANDARD, EPT_LINE_LOOP, EIT_16BIT);

#ifndef show_texture
      for (size_t j = start_value; j < end_value; j++)
      {
        std::vector<Pointbase> & triangles_on_layer = triangles[j];

        if (triangles_on_layer.size() > 2U)
        {
          driver->drawVertexPrimitiveList(triangles_vert[j], triangles_on_layer.size(), triangles_ind[j], triangles_on_layer.size() / 3U, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);

          driver->drawVertexPrimitiveList(triangles_vert_2[j], triangles_on_layer.size(), triangles_ind[j], triangles_on_layer.size() / 3U, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
        }
        }
#endif

#ifdef show_borders
      for (size_t _t = 0; _t < valid_sections.size(); _t++)
      {
        int j = valid_sections[_t];

        // отрисовка целевых объектов
        std::vector<S3DVertex *> & v_obj = v_for_ishemia[_t];
        std::vector<S3DVertex *> & v_obj_2 = v_for_ishemia_2[_t];
        std::vector<S3DVertex *> & v_obj_band = v_for_ishemia_band[_t];
        std::vector<u16 *> & ind_obj = ind_for_ishemia[_t];
        std::vector<u16 *> & ind_obj_band = ind_for_ishemia_band[_t];
        std::vector< std::vector<MRTPoint> > & p_for_ishemia = points_ishemia[j];

        for (size_t _q = 0; _q < v_obj.size(); _q++)
        {
          driver->drawVertexPrimitiveList(v_obj_band[_q], 2 * p_for_ishemia[_q].size(), ind_obj_band[_q], 2 * p_for_ishemia[_q].size(), EVT_STANDARD, EPT_TRIANGLE_STRIP, EIT_16BIT);

          driver->drawVertexPrimitiveList(v_obj[_q], p_for_ishemia[_q].size(), ind_obj[_q], p_for_ishemia[_q].size() - 1, EVT_STANDARD, EPT_LINE_LOOP, EIT_16BIT);

          driver->drawVertexPrimitiveList(v_obj_2[_q], p_for_ishemia[_q].size(), ind_obj[_q], p_for_ishemia[_q].size() - 1, EVT_STANDARD, EPT_LINE_LOOP, EIT_16BIT);
        }

        std::vector< std::vector< std::vector<MRTPoint> > > & points_of_all_caverns_on_layer_grouped_by_objects = points_ishemia_subconturs[j];
        std::vector< std::vector<S3DVertex *> > & v_obj_caverns_top = v_for_ishemia_subconturs[_t];
        std::vector< std::vector<S3DVertex *> > & v_obj_caverns_top_2 = v_for_ishemia_subconturs_2[_t];
        std::vector< std::vector<S3DVertex *> > & v_obj_caverns_top_band = v_for_ishemia_subconturs_band[_t];
        std::vector< std::vector<u16 *> > & ind_obj_top = ind_for_ishemia_subconturs[_t];
        std::vector< std::vector<u16 *> > & ind_obj_top_band = ind_for_ishemia_subconturs_band[_t];

        // отрисовка границы каверн
        for (size_t _m = 0; _m < v_obj_caverns_top.size(); _m++)
        {
          std::vector<S3DVertex *> v_obj_caverns_bottom = v_obj_caverns_top[_m];
          std::vector<S3DVertex *> v_obj_caverns_bottom_2 = v_obj_caverns_top_2[_m];
          std::vector<S3DVertex *> v_obj_caverns_bottom_band = v_obj_caverns_top_band[_m];
          std::vector<u16 *> ind_obj_caverns_bottom = ind_obj_top[_m];
          std::vector<u16 *> ind_obj_caverns_bottom_band = ind_obj_top_band[_m];

          std::vector< std::vector<MRTPoint> > & points_of_all_caverns_on_separate_object = points_of_all_caverns_on_layer_grouped_by_objects[_m];

          for (size_t _s = 0; _s < v_obj_caverns_bottom.size(); _s++)
          {
            std::vector<MRTPoint> & cavern_points = points_of_all_caverns_on_separate_object[_s];

            driver->drawVertexPrimitiveList(v_obj_caverns_bottom_band[_s], 2 * cavern_points.size(), ind_obj_caverns_bottom_band[_s], 2 * cavern_points.size(), EVT_STANDARD, EPT_TRIANGLE_STRIP, EIT_16BIT);

            driver->drawVertexPrimitiveList(v_obj_caverns_bottom[_s], cavern_points.size(), ind_obj_caverns_bottom[_s], cavern_points.size() - 1, EVT_STANDARD, EPT_LINE_LOOP, EIT_16BIT);
            driver->drawVertexPrimitiveList(v_obj_caverns_bottom_2[_s], cavern_points.size(), ind_obj_caverns_bottom[_s], cavern_points.size() - 1, EVT_STANDARD, EPT_LINE_LOOP, EIT_16BIT);
          }
        }
      }
#endif

      driver->endScene();                                            // Драйвер заканчивает отрисовку сцены.
      }

    device->drop();                                                 // Движок перестает работать, можно его удалить.

    delete[]centers_super;

    if (v_for_section.size() > 1U)
    {
      for (size_t _t = 0; _t < quantity_of_lines; _t++)
      {
        delete[]v_for_section_lines[_t];

        delete[]ind_for_section_lines[_t];
      }

      delete[]v_for_section_lines;
      delete[]ind_for_section_lines;
    }

    for (size_t _t = 0; _t < v_for_section.size(); _t++)
    {
      delete[]v_for_section[_t];

      delete[]ind_for_section[_t];
    }

    for (size_t _t = 0; _t < v_for_ishemia.size(); _t++)
    {
      std::vector<S3DVertex *> & v_obj = v_for_ishemia[_t];
      std::vector<S3DVertex *> & v_obj_2 = v_for_ishemia_2[_t];
      std::vector<S3DVertex *> & v_obj_band = v_for_ishemia_band[_t];
      std::vector<u16 *> & ind_obj = ind_for_ishemia[_t];
      std::vector<u16 *> & ind_obj_band = ind_for_ishemia_band[_t];

      for (size_t _q = 0; _q < v_obj.size(); _q++)
      {
        delete[]v_obj[_q];
        delete[]v_obj_2[_q];
        delete[]v_obj_band[_q];

        delete[]ind_obj[_q];
        delete[]ind_obj_band[_q];
      }
    }

    for (size_t _t = 0; _t < v_for_ishemia_subconturs.size(); _t++)
    {
      std::vector< std::vector<S3DVertex *> > & v_obj_caverns_top = v_for_ishemia_subconturs[_t];
      std::vector< std::vector<S3DVertex *> > & v_obj_caverns_top_2 = v_for_ishemia_subconturs_2[_t];
      std::vector< std::vector<S3DVertex *> > & v_obj_caverns_top_band = v_for_ishemia_subconturs_band[_t];
      std::vector< std::vector<u16 *> > & ind_obj_top = ind_for_ishemia_subconturs[_t];
      std::vector< std::vector<u16 *> > & ind_obj_top_band = ind_for_ishemia_subconturs_band[_t];

      for (size_t _q = 0; _q < v_obj_caverns_top.size(); _q++)
      {
        std::vector<S3DVertex *> & v_obj_caverns_bottom = v_obj_caverns_top[_q];
        std::vector<S3DVertex *> & v_obj_caverns_bottom_2 = v_obj_caverns_top_2[_q];
        std::vector<S3DVertex *> & v_obj_caverns_bottom_band = v_obj_caverns_top_band[_q];
        std::vector<u16 *> ind_obj_top_bottom = ind_obj_top[_q];
        std::vector<u16 *> ind_obj_top_bottom_band = ind_obj_top_band[_q];

        for (size_t _m = 0; _m < v_obj_caverns_bottom.size(); _m++)
        {
          delete[]v_obj_caverns_bottom[_m];
          delete[]v_obj_caverns_bottom_2[_m];
          delete[]v_obj_caverns_bottom_band[_m];

          delete[]ind_obj_top_bottom[_m];
          delete[]ind_obj_top_bottom_band[_m];
        }
      }
    }

    delete[]points;
    delete[]points_ishemia;
    delete[]points_ishemia_subconturs;

    for (size_t j = start_value; j < end_value; j++)
    {
      if (triangles_vert[j]) delete[]triangles_vert[j];
      if (triangles_vert_2[j]) delete[]triangles_vert_2[j];

      if (triangles_ind[j]) delete[]triangles_ind[j];
    }

    delete[]triangles_vert;
    delete[]triangles_vert_2;
    delete[]triangles_ind;
    delete[]triangles;

    delete ds;

    if (ds_mask_1 && ds != ds_mask_1) delete ds_mask_1;

    if (ds_mask_2 && ds != ds_mask_2 && ds_mask_1 != ds_mask_2) delete ds_mask_2;
    }
  };

#endif