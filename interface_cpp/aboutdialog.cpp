#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
{
  setupUi(this);

  Qt::WindowFlags flags = windowFlags();
  Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
  flags = flags & (~helpFlag);
  setWindowFlags(flags);
}
