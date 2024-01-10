#include "dicomViewer.h"

#include "Thread3D.h"

typedef std::codecvt_base::result res;
typedef std::codecvt<wchar_t, char, mbstate_t> codecvt_type; // internal, external, state

std::mbstate_t state;

std::codecvt_base::result toWstring(const std::string & str, const std::locale & loc, std::wstring & out)
{
  const codecvt_type& cdcvt = std::use_facet<codecvt_type>(loc);
  std::codecvt_base::result r;

  wchar_t * wchars = new wchar_t[str.size() + 1];

  const char *in_next = 0;
  wchar_t *out_next = 0;

  r = cdcvt.in(state, str.c_str(), str.c_str() + str.size(), in_next,
    wchars, wchars + str.size() + 1, out_next);
  *out_next = '\0';
  out = wchars;

  delete[] wchars;

  return r;
}

std::codecvt_base::result fromWstring(const std::wstring & str, const std::locale & loc, std::string & out)
{
  const codecvt_type& cdcvt = std::use_facet<codecvt_type>(loc);
  std::codecvt_base::result r;

  const wchar_t *in_next = 0;
  char *out_next = 0;

  std::wstring::size_type len = str.size() << 2;

  char * chars = new char[len + 1];

  r = cdcvt.out(state, str.c_str(), str.c_str() + str.size(), in_next,
    chars, chars + len, out_next);
  *out_next = '\0';
  out = chars;

  delete[] chars;

  return r;
}

dicomViewer::dicomViewer(int rowtype_var, CustomCellTableWidgetItem * ct_var, int mode_var, QUuid uid_var, int sType_var, QWidget *parent)
{
  rowtype = rowtype_var;
  ct = ct_var;
  mode = mode_var;
  uid = uid_var;
  sType = sType_var;

  cthread = 0;

  setupUi(this);

  connect(this->spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)), Qt::QueuedConnection);

  if (mode == 0)
  {
    if (rowtype == 3 && ct->column() > 1)
    {
      if (sType == 0)
        setWindowTitle(tr(QApplication::translate("dicomViewerClass", "\320\236\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265 DICOM (\321\201\320\265\321\200\320\270\321\217 \321\201\320\275\320\270\320\274\320\272\320\276\320\262 \342\204\226%1, \320\234\320\241\320\232)", Q_NULLPTR).toStdString().c_str()).arg(QString::number(ct->column())));
      else
        setWindowTitle(tr(QApplication::translate("dicomViewerClass", "\320\236\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265 DICOM (\321\201\320\265\321\200\320\270\321\217 \321\201\320\275\320\270\320\274\320\272\320\276\320\262 \342\204\226%1, \320\270\321\210\320\265\320\274\320\270\321\217)", Q_NULLPTR).toStdString().c_str()).arg(QString::number(ct->column())));
    }
    else
      setWindowTitle(tr(QApplication::translate("dicomViewerClass", "\320\236\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265 DICOM (\321\201\320\265\321\200\320\270\321\217 \321\201\320\275\320\270\320\274\320\272\320\276\320\262 \342\204\226%1)", Q_NULLPTR).toStdString().c_str()).arg(QString::number(ct->column())));

    pushButton3D->setVisible(false);
  }
  else
  {
    setWindowTitle(tr(QApplication::translate("dicomViewerClass", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\320\270 \320\264\320\260\320\275\320\275\321\213\321\205 DICOM (\321\201\320\265\321\200\320\270\321\217 \321\201\320\275\320\270\320\274\320\272\320\276\320\262 \342\204\226%1)", Q_NULLPTR).toStdString().c_str()).arg(QString::number(ct->column())));
  }

  setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

  imageLabel = new AspectRatioPixmapLabel(this);

  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  imageLabel->setSizePolicy(sizePolicy);
  imageLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  QPalette palette = imageLabel->palette();
  palette.setColor(imageLabel->backgroundRole(), Qt::gray);
  imageLabel->setPalette(palette);
  imageLabel->setAutoFillBackground(true);

  imageVerticalLayout->insertWidget(0, imageLabel);
  imageVerticalLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  if (rowtype == 3 && ct->column() > 1)
  {
    QDir data_folder_msk(ct->path);
    QDir data_folder_ischemia(ct->path_combined);
    if (data_folder_msk != QString() && data_folder_msk.exists() && data_folder_ischemia != QString() && data_folder_ischemia.exists())
    {
      data_folder_msk.setNameFilters(QStringList() << "*.IMA" << "*.ima");
      data_folder_ischemia.setNameFilters(QStringList() << "*.IMA" << "*.ima");

      QStringList dataFileList_msk = data_folder_msk.entryList();
      QStringList dataFileList_ischemia = data_folder_ischemia.entryList();

      if (dataFileList_msk.size() && dataFileList_ischemia.size())
      {
        int start_index;
        int stop_index;

        if (mode == 0) // оригинальные файлы
        {
          if (ct->all_snapshots || ct->all_snapshots_combined)
          {
            start_index = 0;

            if (sType == 0) stop_index = dataFileList_msk.size() - 1;
            else stop_index = dataFileList_ischemia.size() - 1;
          }
          else
          {
            if (sType == 0)
            {
              start_index = ct->first_snapshot_index;

              stop_index = ct->last_snapshot_index;
            }
            else
            {
              start_index = ct->first_snapshot_index_combined;

              stop_index = ct->last_snapshot_index_combined;
            }
          }

          this->spinBox->setMinimum(start_index);
          this->spinBox->setMaximum(stop_index);
          this->spinBox->setValue(start_index);

          this->minMaxLabel->setText(tr(" (%1 - %2) ").arg(QString::number(start_index), QString::number(stop_index)));
        }
        else // обработанные файлы
        {
          if (ct->all_snapshots) // используем нумерацию МСК-снимка
          {
            start_index = 0;

            stop_index = dataFileList_msk.size() - 1;
          }
          else
          {
            start_index = ct->first_snapshot_index;

            stop_index = ct->last_snapshot_index;
          }

          QDir results_folder_msk(tr("%1%2results%3%4%3msk").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString()));
          QDir results_folder_ischemia(tr("%1%2results%3%4%3ischemia").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString()));

          if (results_folder_msk != QString() && results_folder_msk.exists() && results_folder_ischemia != QString() && results_folder_ischemia.exists())
          {
            results_folder_msk.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
            QStringList resultsFileList_msk = results_folder_msk.entryList();

            QString fileName_msk = tr("%1%2results%3%4%5msk%5result_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

            results_folder_ischemia.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
            QStringList resultsFileList_ischemia = results_folder_ischemia.entryList();

            QString fileName_ischemia = tr("%1%2results%3%4%5ischemia%5result_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

            if (resultsFileList_msk.size() && QFile::exists(fileName_msk) && resultsFileList_msk.size() && QFile::exists(fileName_msk)) // есть файлы с результатами обработки
            {
              this->spinBox->setMinimum(start_index);
              this->spinBox->setMaximum(stop_index);
              this->spinBox->setValue(start_index);

              this->minMaxLabel->setText(tr(" (%1 - %2) ").arg(QString::number(start_index), QString::number(stop_index)));
            }
            else
            {
              qWarning("Wanted for '%s' and %s", fileName_msk.toStdString().c_str(), fileName_ischemia.toStdString().c_str());
            }
          }
        }
      }
    }
  }
  else
  {
    QDir data_folder(ct->path);
    if (data_folder != QString() && data_folder.exists())
    {
      data_folder.setNameFilters(QStringList() << "*.IMA" << "*.ima");

      QStringList dataFileList = data_folder.entryList();

      if (dataFileList.size())
      {
        int start_index;
        int stop_index;

        if (ct->all_snapshots)
        {
          start_index = 0;

          stop_index = dataFileList.size() - 1;
        }
        else
        {
          start_index = ct->first_snapshot_index;

          stop_index = ct->last_snapshot_index;
        }

        if (mode == 0) // оригинальные файлы
        {
          this->spinBox->setMinimum(start_index);
          this->spinBox->setMaximum(stop_index);
          this->spinBox->setValue(start_index);

          this->minMaxLabel->setText(tr(" (%1 - %2) ").arg(QString::number(start_index), QString::number(stop_index)));
        }
        else // обработанные файлы
        {
          QDir results_folder(tr("%1%2results%3%4").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString()));
          if (results_folder != QString() && results_folder.exists())
          {
            results_folder.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
            QStringList resultsFileList = results_folder.entryList();

            QString fileName = tr("%1%2results%3%4%5result_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

            if (resultsFileList.size() && QFile::exists(fileName)) // есть файл с результатом обработки
            {
              this->spinBox->setMinimum(start_index);
              this->spinBox->setMaximum(stop_index);
              this->spinBox->setValue(start_index);

              this->minMaxLabel->setText(tr(" (%1 - %2) ").arg(QString::number(start_index), QString::number(stop_index)));
            }
            else
            {
              qWarning("Wanted for '%s'", fileName.toStdString().c_str());
            }
          }
        }
      }
    }
  }
}

void dicomViewer::spinBoxValueChanged(int value)
{
  QString fileName = QString();
  QString fileMask = QString();
  QString fileBrain = QString();

  QString fileName_combined = QString();
  QString fileMask_combined = QString();
  QString fileBrain_combined = QString();

  if (mode == 0) // оригинальные файлы
  {
    QDir data_folder;

    if (rowtype == 3 && ct->column() > 1)
    {
      if (sType == 0)
        data_folder.setPath(ct->path);
      else
        data_folder.setPath(ct->path_combined);
    }
    else
    {
      data_folder.setPath(ct->path);
    }

    data_folder.setFilter(QDir::Files | QDir::NoSymLinks);
    data_folder.setSorting(QDir::NoSort);

    if (data_folder != QString() && data_folder.exists())
    {
      data_folder.setNameFilters(QStringList() << "*.IMA" << "*.ima");

      QStringList dataFileList = data_folder.entryList();

      QCollator collator;
      collator.setNumericMode(true);

      std::sort(
        dataFileList.begin(),
        dataFileList.end(),
        [&collator](const QString &file1, const QString &file2)
      {
        return collator.compare(file1, file2) < 0;
      });

      if (dataFileList.size())
      {
        if (value > -1 && value < dataFileList.size())
        {
          if (rowtype == 3 && ct->column() > 1)
          {
            if (sType == 0)
              fileName = tr("%1%2").arg(ct->path, dataFileList[value]);
            else
              fileName = tr("%1%2").arg(ct->path_combined, dataFileList[value]);
          }
          else
          {
            fileName = tr("%1%2").arg(ct->path, dataFileList[value]);
          }
        }
      }
    }
  }
  else // обработанные файлы
  {
    if (rowtype == 3 && ct->column() > 1)
    {
      QDir results_folder_msk(tr("%1%2results%3%4%3msk").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString()));
      results_folder_msk.setFilter(QDir::Files | QDir::NoSymLinks);
      results_folder_msk.setSorting(QDir::NoSort);

      qWarning("results_folder_msk: %s", results_folder_msk.path().toStdString().c_str());

      if (results_folder_msk != QString() && results_folder_msk.exists())
      {
        results_folder_msk.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
        QStringList resultsFileList_msk = results_folder_msk.entryList();

        if (resultsFileList_msk.size())
        {
          fileName = tr("%1%2results%3%4%3msk%5result_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileName.toStdString().c_str());

          fileMask = tr("%1%2results%3%4%3msk%5result_mask_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileMask.toStdString().c_str());

          fileBrain = tr("%1%2results%3%4%3msk%5result_brain_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileBrain.toStdString().c_str());
        }
      }

      QDir results_folder_ischemia(tr("%1%2results%3%4%3ischemia").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString()));
      results_folder_ischemia.setFilter(QDir::Files | QDir::NoSymLinks);
      results_folder_ischemia.setSorting(QDir::NoSort);

      qWarning("results_folder_ischemia: %s", results_folder_ischemia.path().toStdString().c_str());

      if (results_folder_ischemia != QString() && results_folder_ischemia.exists())
      {
        results_folder_ischemia.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
        QStringList resultsFileList_ischemia = results_folder_ischemia.entryList();

        if (resultsFileList_ischemia.size())
        {
          fileName_combined = tr("%1%2results%3%4%3ischemia%5result_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileName_combined.toStdString().c_str());

          fileMask_combined = tr("%1%2results%3%4%3ischemia%5result_mask_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileMask_combined.toStdString().c_str());

          fileBrain_combined = tr("%1%2results%3%4%3ischemia%5result_brain_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileBrain_combined.toStdString().c_str());
        }
      }
    }
    else
    {
      QDir results_folder(tr("%1%2results%3%4").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString()));
      results_folder.setFilter(QDir::Files | QDir::NoSymLinks);
      results_folder.setSorting(QDir::NoSort);

      if (results_folder != QString() && results_folder.exists())
      {
        results_folder.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
        QStringList resultsFileList = results_folder.entryList();

        if (resultsFileList.size())
        {
          fileName = tr("%1%2results%3%4%5result_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileName.toStdString().c_str());

          fileMask = tr("%1%2results%3%4%5result_mask_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileMask.toStdString().c_str());

          fileBrain = tr("%1%2results%3%4%5result_brain_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileBrain.toStdString().c_str());
        }
      }
    }
  }

  qWarning("fileName: '%s'", fileName.toStdString().c_str());

  if (fileName != QString() && QFile::exists(fileName))
  {
    size_t frame_number;
    if (mode == 0) // оригинальные файлы
    {
      frame_number = 0;

      qWarning("Load image '%s'", fileName.toStdString().c_str());
    }
    else // обработанные файлы
    {
      frame_number = value;

      qWarning("Load image '%s', snapshot '%d' and result_index '%d'", fileName.toStdString().c_str(), value, ct->result_index);
    }

    unsigned short width_dcm = 0, height_dcm = 0;
    unsigned short width_dcm_combined = 0, height_dcm_combined = 0;

    dicom::DataSet ds;
    dicom::ReadW(fileName.toStdWString(), ds);
    if (ds.exists(dicom::TAG_COLUMNS)) ds(dicom::TAG_COLUMNS) >> width_dcm;
    if (ds.exists(dicom::TAG_ROWS)) ds(dicom::TAG_ROWS) >> height_dcm;

    QImage image(width_dcm, height_dcm, QImage::Format_RGB32);

    if (width_dcm && height_dcm)
    {
      unsigned short bits_allocated = 1;

      const std::vector<UINT16>& pixel_data = ds(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();
      size_t ss = pixel_data.size();
      size_t area = width_dcm * height_dcm;

      if (ds.exists(dicom::TAG_BITS_ALLOC)) ds(dicom::TAG_BITS_ALLOC) >> bits_allocated;

      if (8 != bits_allocated)
      {
        UINT16 t_max = 0, t_min = 65535;
        double t_delta;
        for (size_t i = 0; i < ss; i++)
        {
          if (pixel_data[i] > t_max) t_max = pixel_data[i];

          if (pixel_data[i] < t_min) t_min = pixel_data[i];
        }

        t_delta = t_max - t_min;
        if (t_delta > 0)
        {
          int rgb;
          for (int i = 0; i < area; i++)
          {
            rgb = (int)floor(255.0 * double(pixel_data[frame_number * area + i] - t_min) / t_delta + 0.5);

            if (mode == 0) // оригинальные файлы
            {
              image.setPixelColor(i % width_dcm, i / width_dcm, QColor::fromRgb(rgb, rgb, rgb));
            }
            else // обработанные файлы
            {
              image.setPixelColor(width_dcm - (i % width_dcm) - 1U, height_dcm - (i / width_dcm) - 1U, QColor::fromRgb(rgb, rgb, rgb));
            }
          }
        }

        // использовать информацию с маски
        if (mode && fileMask != QString() && QFile::exists(fileMask))
        {
          dicom::DataSet ds_mask;
          dicom::ReadW(fileMask.toStdWString(), ds_mask);
          const std::vector<UINT16>& pixel_data_mask1 = ds_mask(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

          // использовать информацию с маски
          if (mode && fileBrain != QString() && QFile::exists(fileBrain))
          {
            dicom::DataSet ds_mask;
            dicom::ReadW(fileBrain.toStdWString(), ds_mask);
            const std::vector<UINT16>& pixel_data_mask2 = ds_mask(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

            if (ct->snapshot_type == 1)
            {
              for (int i = 0; i < area; i++)
              {
                // DEBUG Ichemia
                if (pixel_data_mask2[frame_number * area + i] == 255U)
                  image.setPixelColor(width_dcm - (i % width_dcm) - 1U, height_dcm - (i / width_dcm) - 1U, QColor::fromRgb(0, 0, 255));
                else
                {
                  if (pixel_data_mask1[frame_number * area + i] == 0U)
                    image.setPixelColor(width_dcm - (i % width_dcm) - 1U, height_dcm - (i / width_dcm) - 1U, QColor::fromRgb(255, 0, 255));
                }
              }
            }
            else
            {
              for (int i = 0; i < area; i++)
              {
                if (pixel_data_mask2[frame_number * area + i] == 255U)
                  image.setPixelColor(width_dcm - (i % width_dcm) - 1U, height_dcm - (i / width_dcm) - 1U, QColor::fromRgb(0, 0, 255));
              }
            }
          }


          if (ct->snapshot_type != 1)
          {
            // ишемия
            if (mode && rowtype == 3 && ct->column() > 1)
            {
              size_t frame_number_combined;

              if (ct->all_snapshots && ct->all_snapshots_combined) frame_number_combined = value;
              else if (ct->all_snapshots && !ct->all_snapshots_combined) frame_number_combined = value + ct->first_snapshot_index_combined;
              else if (!ct->all_snapshots && ct->all_snapshots_combined) frame_number_combined = value - ct->first_snapshot_index;
              else if (!ct->all_snapshots && !ct->all_snapshots_combined) frame_number_combined = value + (ct->first_snapshot_index_combined - ct->first_snapshot_index);

              //qWarning("frame_number_combined: '%d'", (int)frame_number_combined);

              if (mode && fileMask_combined != QString() && QFile::exists(fileMask_combined))
              {
                dicom::DataSet ds_mask_combined;
                dicom::ReadW(fileMask_combined.toStdWString(), ds_mask_combined);

                if (ds_mask_combined.exists(dicom::TAG_COLUMNS)) ds_mask_combined(dicom::TAG_COLUMNS) >> width_dcm_combined;
                if (ds_mask_combined.exists(dicom::TAG_ROWS)) ds_mask_combined(dicom::TAG_ROWS) >> height_dcm_combined;

                unsigned short bits_allocated_combined = 1;

                if (ds_mask_combined.exists(dicom::TAG_BITS_ALLOC)) ds_mask_combined(dicom::TAG_BITS_ALLOC) >> bits_allocated_combined;

                if (8 != bits_allocated_combined)
                {
                  const std::vector<UINT16>& pixel_data_mask1_combined = ds_mask_combined(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

                  size_t area_combined = width_dcm_combined * height_dcm_combined;

                  dicom::DataSet ds_mask;
                  dicom::ReadW(fileBrain.toStdWString(), ds_mask);
                  const std::vector<UINT16>& pixel_data_mask2 = ds_mask(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

                  for (int i = 0; i < area; i++)
                  {
                    size_t x_orig = width_dcm - (i % width_dcm) - 1U;
                    size_t y_orig = height_dcm - (i / width_dcm) - 1U;

                    size_t x_cord = (size_t)floor(width_dcm_combined * (double)(i % width_dcm) / (double)width_dcm + 0.5);
                    size_t y_cord = (size_t)floor(height_dcm_combined * (double)(i / width_dcm) / (double)height_dcm + 0.5);

                    if (pixel_data_mask1_combined[frame_number_combined * area_combined + y_cord * width_dcm_combined + x_cord] == 255U && pixel_data_mask2[frame_number * area + i] == 0)
                      image.setPixelColor(x_orig, y_orig, QColor::fromRgb(255, 242, 0));
                  }
                }
              }
            }

            for (int i = 0; i < area; i++)
            {
              if (pixel_data_mask1[frame_number * area + i] == 255U)
                image.setPixelColor(width_dcm - (i % width_dcm) - 1U, height_dcm - (i / width_dcm) - 1U, QColor::fromRgb(255, 0, 255));
            }
          }
        }
        else
        {
          // использовать информацию с маски
          if (mode && fileBrain != QString() && QFile::exists(fileBrain))
          {
            dicom::DataSet ds_mask;
            dicom::ReadW(fileBrain.toStdWString(), ds_mask);
            const std::vector<UINT16>& pixel_data_mask = ds_mask(dicom::TAG_PIXEL_DATA).Get<std::vector<UINT16> >();

            for (int i = 0; i < area; i++)
            {
              if (pixel_data_mask[frame_number * area + i] == 255U)
                image.setPixelColor(width_dcm - (i % width_dcm) - 1U, height_dcm - (i / width_dcm) - 1U, QColor::fromRgb(255, 0, 255));
            }
          }
        }
      }
    }

    imageLabel->setPixmap(QPixmap::fromImage(image));
  }

  this->spinBox->findChild<QLineEdit*>()->deselect();
}

void dicomViewer::show3D()
{
  if (rowtype == 3) // combined
  {
    QString fileName_msk = QString();
    QString fileMask_msk = QString();
    QString fileMask_ischemia = QString();

    QDir results_folder_msk(tr("%1%2results%3%4%3msk").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString()));
    results_folder_msk.setFilter(QDir::Files | QDir::NoSymLinks);
    results_folder_msk.setSorting(QDir::NoSort);

    QDir results_folder_ischemia(tr("%1%2results%3%4%3ischemia").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString()));
    results_folder_ischemia.setFilter(QDir::Files | QDir::NoSymLinks);
    results_folder_ischemia.setSorting(QDir::NoSort);

    if (results_folder_msk != QString() && results_folder_msk.exists() && results_folder_ischemia != QString() && results_folder_ischemia.exists())
    {
      results_folder_msk.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
      QStringList resultsFileList_msk = results_folder_msk.entryList();

      results_folder_ischemia.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
      QStringList resultsFileList_ischemia = results_folder_ischemia.entryList();

      if (resultsFileList_msk.size() && resultsFileList_ischemia.size())
      {
        // файл с мозгом
        fileName_msk = tr("%1%2results%3%4%5msk%5result_brain_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

        qWarning("Wanted for '%s'", fileName_msk.toStdString().c_str());

        fileMask_msk = tr("%1%2results%3%4%5msk%5result_mask_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

        qWarning("Wanted for '%s'", fileMask_msk.toStdString().c_str());

        fileMask_ischemia = tr("%1%2results%3%4%5ischemia%5result_mask_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

        qWarning("Wanted for '%s'", fileMask_ischemia.toStdString().c_str());
      }
    }

    if (fileName_msk != QString() && QFile::exists(fileName_msk))
    {
      unsigned short width_dcm = 0, height_dcm = 0;

      dicom::DataSet * ds = new dicom::DataSet();
      dicom::ReadW(fileName_msk.toStdWString(), *ds);

      // использовать информацию с маски
      if (fileMask_msk != QString() && QFile::exists(fileMask_msk) && fileMask_ischemia != QString() && QFile::exists(fileMask_ischemia))
      {
        dicom::DataSet * ds_mask = new dicom::DataSet();
        dicom::ReadW(fileMask_msk.toStdWString(), *ds_mask);

        dicom::DataSet * ds_mask2 = new dicom::DataSet();
        dicom::ReadW(fileMask_ischemia.toStdWString(), *ds_mask2);

        int offset = 0;

        if (ct->all_snapshots && ct->all_snapshots_combined) offset = 0;
        else if (ct->all_snapshots && !ct->all_snapshots_combined) offset = ct->first_snapshot_index_combined;
        else if (!ct->all_snapshots && ct->all_snapshots_combined) offset = -ct->first_snapshot_index;
        else if (!ct->all_snapshots && !ct->all_snapshots_combined) offset = ct->first_snapshot_index_combined - ct->first_snapshot_index;

        // show
        cthread = new Thread3D(this, rowtype, mode, ds, ds_mask, ds_mask2, ct->first_snapshot_index, ct->last_snapshot_index, offset);
      }

      try
      {
        ((Thread3D *)cthread)->start();
      }
      catch (...) {}
    }
  }
  else
  {
    QString fileName = QString();
    QString fileMask = QString();

    QDir results_folder(tr("%1%2results%3%4").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString()));
    results_folder.setFilter(QDir::Files | QDir::NoSymLinks);
    results_folder.setSorting(QDir::NoSort);

    if (results_folder != QString() && results_folder.exists())
    {
      results_folder.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
      QStringList resultsFileList = results_folder.entryList();

      if (resultsFileList.size())
      {
        // файл с мозгом
        fileName = tr("%1%2results%3%4%5result_brain_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

        qWarning("Wanted for '%s'", fileName.toStdString().c_str());

        // файл со стволовыми клетками или с ишемией
        if (mode == 1)
        {
          fileMask = tr("%1%2results%3%4%5result_mask_%6.dcm").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

          qWarning("Wanted for '%s'", fileMask.toStdString().c_str());
        }
      }
    }

    if (fileName != QString() && QFile::exists(fileName))
    {
      unsigned short width_dcm = 0, height_dcm = 0;

      dicom::DataSet * ds = new dicom::DataSet();
      dicom::ReadW(fileName.toStdWString(), *ds);

      // использовать информацию с маски
      if (mode && fileMask != QString() && QFile::exists(fileMask))
      {
        dicom::DataSet * ds_mask = new dicom::DataSet();
        dicom::ReadW(fileMask.toStdWString(), *ds_mask);

        qWarning("rowtype: %d", rowtype);

        switch (rowtype)
        {
        case 1: // ишемия
        {
          cthread = new Thread3D(this, rowtype, mode, ds, 0, ds_mask, ct->first_snapshot_index, ct->last_snapshot_index, 0);

          break;
        }
        case 2: // стволовые клетки
        {
          cthread = new Thread3D(this, rowtype, mode, ds, ds_mask, 0, ct->first_snapshot_index, ct->last_snapshot_index, 0);

          break;
        }
        }
      }
      else
      {
        cthread = new Thread3D(this, rowtype, mode, ds, 0, 0, ct->first_snapshot_index, ct->last_snapshot_index, 0);
      }

      try
      {
        ((Thread3D *)cthread)->start();
      }
      catch (...) {}
    }
  }
}

void dicomViewer::acceptCommand()
{
  if (cthread)
  {
    try
    {
      if (!((Thread3D *)cthread)->isFinished())
        ((Thread3D *)cthread)->stop();

      while (!((Thread3D *)cthread)->isFinished())
        WaitForSingleObject(GetCurrentThread(), 50);
    }
    catch (...) {}
  }

  this->accept();
}

void dicomViewer::closeEvent(QCloseEvent *bar)
{
  if (cthread)
  {
    try
    {
      if (!((Thread3D *)cthread)->isFinished())
        ((Thread3D *)cthread)->stop();

      while (!((Thread3D *)cthread)->isFinished())
        WaitForSingleObject(GetCurrentThread(), 50);
    }
    catch (...) {}
  }
}
