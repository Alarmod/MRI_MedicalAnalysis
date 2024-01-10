#include "NewSeriesFormCombined.h"

NewSeriesFormCombined::NewSeriesFormCombined(QWidget *parent)
  : QWidget(parent)
{
  ui.setupUi(this);

  ui.ischemiaDaysLabel->setText(this->typeParamHeader);
  ui.mskDaysLabel->setText(this->typeParamHeader);

  changed = 0;

  Qt::WindowFlags flags = windowFlags();
  Qt::WindowFlags minFlag = Qt::WindowMinimizeButtonHint;

  flags = flags & (~minFlag);
  setWindowFlags(flags);

  char buff[256];
  sprintf(buff, "QPlainTextEdit {font-size: %dpt}", (int)floor(this->font().pointSizeF() * 0.975));

  ui.mskPlainTextEdit->setStyleSheet(buff);
  ui.ischemiaPlainTextEdit->setStyleSheet(buff);
}

NewSeriesFormCombined::~NewSeriesFormCombined()
{
}

void NewSeriesFormCombined::saveChangesAndClose()
{
  changed = 1;

  close();
}

void NewSeriesFormCombined::mskSelectPath()
{
  QFileDialog * fd = new QFileDialog(this);
  fd->setFileMode(QFileDialog::FileMode::DirectoryOnly);

  if (ui.mskPlainTextEdit->toPlainText() == QString())
    fd->setDirectory(QCoreApplication::applicationDirPath());
  else
    fd->setDirectory(ui.mskPlainTextEdit->toPlainText());

  if (fd->exec())
  {
    QStringList sl = fd->selectedFiles();

    if (sl.size() > 0)
    {
      ui.mskSnapshotSelectorCheckBox->setChecked(true);

      QDir dir;
      ui.mskPlainTextEdit->setPlainText(dir.relativeFilePath(sl[0]) + QDir::fromNativeSeparators(QDir::separator()));

      mskSnapshotSelectorCheckBoxStatusChanged();

      ui.mskSnapshotSelectorCheckBox->setEnabled(true);
      ui.mskSnapshotSelectorMinSpinBox->setEnabled(false);
      ui.mskSnapshotSelectorMaxSpinBox->setEnabled(false);
    }
  }
}

void NewSeriesFormCombined::mskSnapshotSelectorCheckBoxStatusChanged()
{
  QString export_folder_path = ui.mskPlainTextEdit->toPlainText();
  QDir export_folder(export_folder_path);

  if (ui.mskSnapshotSelectorCheckBox->isChecked() == false && (export_folder != QString() && export_folder.exists()) == false)
    ui.mskSnapshotSelectorCheckBox->setChecked(true);
  else
  {
    if (ui.mskSnapshotSelectorCheckBox->isChecked())
    {
      ui.mskSnapshotSelectorMinSpinBox->setEnabled(false);
      ui.mskSnapshotSelectorMaxSpinBox->setEnabled(false);

      ui.mskSnapshotSelectorMinSpinBox->setMinimum(-1);
      ui.mskSnapshotSelectorMinSpinBox->setMaximum(-1);
      ui.mskSnapshotSelectorMinSpinBox->setValue(-1);

      ui.mskSnapshotSelectorMaxSpinBox->setMinimum(-1);
      ui.mskSnapshotSelectorMaxSpinBox->setMaximum(-1);
      ui.mskSnapshotSelectorMaxSpinBox->setValue(-1);
    }
    else
    {
      ui.mskSnapshotSelectorMinSpinBox->setMinimum(-1);
      ui.mskSnapshotSelectorMinSpinBox->setMaximum(-1);
      ui.mskSnapshotSelectorMinSpinBox->setValue(-1);

      ui.mskSnapshotSelectorMaxSpinBox->setMinimum(-1);
      ui.mskSnapshotSelectorMaxSpinBox->setMaximum(-1);
      ui.mskSnapshotSelectorMaxSpinBox->setValue(-1);

      // проверить директорию
      if (export_folder != QString() && export_folder.exists())
      {
        export_folder.setNameFilters(QStringList() << "*.IMA" << "*.ima");
        QStringList fileList = export_folder.entryList();

        if (fileList.size())
        {
          ui.mskSnapshotSelectorMinSpinBox->setMinimum(0);
          ui.mskSnapshotSelectorMinSpinBox->setMaximum(fileList.size() - 1);
          ui.mskSnapshotSelectorMinSpinBox->setValue(0);

          ui.mskSnapshotSelectorMaxSpinBox->setMinimum(0);
          ui.mskSnapshotSelectorMaxSpinBox->setMaximum(fileList.size() - 1);
          ui.mskSnapshotSelectorMaxSpinBox->setValue(fileList.size() - 1);
        }
      }

      ui.mskSnapshotSelectorMinSpinBox->setEnabled(true);
      ui.mskSnapshotSelectorMaxSpinBox->setEnabled(true);
    }
  }
}

void NewSeriesFormCombined::mskPathUpdated()
{
  ui.mskPlainTextEdit->setToolTip(ui.mskPlainTextEdit->toPlainText());
}

void NewSeriesFormCombined::ischemiaSelectPath()
{
  QFileDialog * fd = new QFileDialog(this);
  fd->setFileMode(QFileDialog::FileMode::DirectoryOnly);

  if (ui.ischemiaPlainTextEdit->toPlainText() == QString())
    fd->setDirectory(QCoreApplication::applicationDirPath());
  else
    fd->setDirectory(ui.ischemiaPlainTextEdit->toPlainText());

  if (fd->exec())
  {
    QStringList sl = fd->selectedFiles();

    if (sl.size() > 0)
    {
      ui.ischemiaSnapshotSelectorCheckBox->setChecked(true);

      QDir dir;
      ui.ischemiaPlainTextEdit->setPlainText(dir.relativeFilePath(sl[0]) + QDir::fromNativeSeparators(QDir::separator()));

      ischemiaSnapshotSelectorCheckBoxStatusChanged();

      ui.ischemiaSnapshotSelectorCheckBox->setEnabled(true);
      ui.ischemiaSnapshotSelectorMinSpinBox->setEnabled(false);
      ui.ischemiaSnapshotSelectorMaxSpinBox->setEnabled(false);
    }
  }
}

void NewSeriesFormCombined::ischemiaSnapshotSelectorCheckBoxStatusChanged()
{
  QString export_folder_path = ui.ischemiaPlainTextEdit->toPlainText();
  QDir export_folder(export_folder_path);

  if (ui.ischemiaSnapshotSelectorCheckBox->isChecked() == false && (export_folder != QString() && export_folder.exists()) == false)
    ui.ischemiaSnapshotSelectorCheckBox->setChecked(true);
  else
  {
    if (ui.ischemiaSnapshotSelectorCheckBox->isChecked())
    {
      ui.ischemiaSnapshotSelectorMinSpinBox->setEnabled(false);
      ui.ischemiaSnapshotSelectorMaxSpinBox->setEnabled(false);

      ui.ischemiaSnapshotSelectorMinSpinBox->setMinimum(-1);
      ui.ischemiaSnapshotSelectorMinSpinBox->setMaximum(-1);
      ui.ischemiaSnapshotSelectorMinSpinBox->setValue(-1);

      ui.ischemiaSnapshotSelectorMaxSpinBox->setMinimum(-1);
      ui.ischemiaSnapshotSelectorMaxSpinBox->setMaximum(-1);
      ui.ischemiaSnapshotSelectorMaxSpinBox->setValue(-1);
    }
    else
    {
      ui.ischemiaSnapshotSelectorMinSpinBox->setMinimum(-1);
      ui.ischemiaSnapshotSelectorMinSpinBox->setMaximum(-1);
      ui.ischemiaSnapshotSelectorMinSpinBox->setValue(-1);

      ui.ischemiaSnapshotSelectorMaxSpinBox->setMinimum(-1);
      ui.ischemiaSnapshotSelectorMaxSpinBox->setMaximum(-1);
      ui.ischemiaSnapshotSelectorMaxSpinBox->setValue(-1);

      // проверить директорию
      if (export_folder != QString() && export_folder.exists())
      {
        export_folder.setNameFilters(QStringList() << "*.IMA" << "*.ima");
        QStringList fileList = export_folder.entryList();

        if (fileList.size())
        {
          ui.ischemiaSnapshotSelectorMinSpinBox->setMinimum(0);
          ui.ischemiaSnapshotSelectorMinSpinBox->setMaximum(fileList.size() - 1);
          ui.ischemiaSnapshotSelectorMinSpinBox->setValue(0);

          ui.ischemiaSnapshotSelectorMaxSpinBox->setMinimum(0);
          ui.ischemiaSnapshotSelectorMaxSpinBox->setMaximum(fileList.size() - 1);
          ui.ischemiaSnapshotSelectorMaxSpinBox->setValue(fileList.size() - 1);
        }
      }

      ui.ischemiaSnapshotSelectorMinSpinBox->setEnabled(true);
      ui.ischemiaSnapshotSelectorMaxSpinBox->setEnabled(true);
    }
  }
}

void NewSeriesFormCombined::ischemiaPathUpdated()
{
  ui.ischemiaPlainTextEdit->setToolTip(ui.ischemiaPlainTextEdit->toPlainText());
}

void NewSeriesFormCombined::closeEvent(QCloseEvent *event)
{
  emit updateSignal();
}
