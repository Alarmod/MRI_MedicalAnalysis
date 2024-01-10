#include "NewSeriesForm.h"

NewSeriesForm::NewSeriesForm(unsigned int type_of_form, QWidget *parent)
  : QWidget(parent)
{
  ui.setupUi(this);

  switch (type_of_form)
  {
    case 0:
    {
      ui.daysLabel->setText(this->type0ParamHeader);
      break;
    }
    case 1:
    {
      ui.daysLabel->setText(this->type1ParamHeader);
      break;
    }
    case 2:
    {
      ui.daysLabel->setText(this->type2ParamHeader);
      break;
    }
    case 3:
    {
      ui.daysLabel->setText(this->type3ParamHeader);
      break;
    }

    default:
    {
      break;
    }
  }

  changed = 0;

  Qt::WindowFlags flags = windowFlags();
  Qt::WindowFlags minFlag = Qt::WindowMinimizeButtonHint;

  flags = flags & (~minFlag);
  setWindowFlags(flags);

  char buff[256];
  sprintf(buff, "QPlainTextEdit {font-size: %dpt}", (int)floor(this->font().pointSizeF() * 0.975));

  ui.plainTextEdit->setStyleSheet(buff);
}

NewSeriesForm::~NewSeriesForm()
{
}

void NewSeriesForm::saveChangesAndClose()
{
  changed = 1;

  close();
}

void NewSeriesForm::selectPath()
{
  QFileDialog * fd = new QFileDialog(this);
  fd->setFileMode(QFileDialog::FileMode::DirectoryOnly);

  if (ui.plainTextEdit->toPlainText() == QString())
    fd->setDirectory(QCoreApplication::applicationDirPath());
  else
    fd->setDirectory(ui.plainTextEdit->toPlainText());

  if (fd->exec())
  {
    QStringList sl = fd->selectedFiles();

    if (sl.size() > 0)
    {
      ui.snapshotSelectorCheckBox->setChecked(true);

      QDir dir;
      ui.plainTextEdit->setPlainText(dir.relativeFilePath(sl[0]) + QDir::fromNativeSeparators(QDir::separator()));

      snapshotSelectorCheckBoxStatusChanged();

      ui.snapshotSelectorCheckBox->setEnabled(true);
      ui.snapshotSelectorMinSpinBox->setEnabled(false);
      ui.snapshotSelectorMaxSpinBox->setEnabled(false);
    }
  }
}

void NewSeriesForm::snapshotSelectorCheckBoxStatusChanged()
{
  QString export_folder_path = ui.plainTextEdit->toPlainText();
  QDir export_folder(export_folder_path);

  if (ui.snapshotSelectorCheckBox->isChecked() == false && (export_folder != QString() && export_folder.exists()) == false)
    ui.snapshotSelectorCheckBox->setChecked(true);
  else
  {
    if (ui.snapshotSelectorCheckBox->isChecked())
    {
      ui.snapshotSelectorMinSpinBox->setEnabled(false);
      ui.snapshotSelectorMaxSpinBox->setEnabled(false);

      ui.snapshotSelectorMinSpinBox->setMinimum(-1);
      ui.snapshotSelectorMinSpinBox->setMaximum(-1);
      ui.snapshotSelectorMinSpinBox->setValue(-1);

      ui.snapshotSelectorMaxSpinBox->setMinimum(-1);
      ui.snapshotSelectorMaxSpinBox->setMaximum(-1);
      ui.snapshotSelectorMaxSpinBox->setValue(-1);
    }
    else
    {
      ui.snapshotSelectorMinSpinBox->setMinimum(-1);
      ui.snapshotSelectorMinSpinBox->setMaximum(-1);
      ui.snapshotSelectorMinSpinBox->setValue(-1);

      ui.snapshotSelectorMaxSpinBox->setMinimum(-1);
      ui.snapshotSelectorMaxSpinBox->setMaximum(-1);
      ui.snapshotSelectorMaxSpinBox->setValue(-1);

      // проверить директорию
      if (export_folder != QString() && export_folder.exists())
      {
        export_folder.setNameFilters(QStringList() << "*.IMA" << "*.ima");
        QStringList fileList = export_folder.entryList();

        if (fileList.size())
        {
          ui.snapshotSelectorMinSpinBox->setMinimum(0);
          ui.snapshotSelectorMinSpinBox->setMaximum(fileList.size() - 1);
          ui.snapshotSelectorMinSpinBox->setValue(0);

          ui.snapshotSelectorMaxSpinBox->setMinimum(0);
          ui.snapshotSelectorMaxSpinBox->setMaximum(fileList.size() - 1);
          ui.snapshotSelectorMaxSpinBox->setValue(fileList.size() - 1);
        }
      }

      ui.snapshotSelectorMinSpinBox->setEnabled(true);
      ui.snapshotSelectorMaxSpinBox->setEnabled(true);
    }
  }
}

void NewSeriesForm::pathUpdated()
{
  ui.plainTextEdit->setToolTip(ui.plainTextEdit->toPlainText());
}

void NewSeriesForm::closeEvent(QCloseEvent *event)
{
  emit updateSignal();
}
