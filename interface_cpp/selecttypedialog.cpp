#include "selecttypedialog.h"

InvestigationTypeSelector::InvestigationTypeSelector(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);

  Qt::WindowFlags flags = windowFlags();

  Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
  flags = flags & (~helpFlag);

  Qt::WindowFlags minFlag = Qt::WindowMinimizeButtonHint;
  flags = flags & (~minFlag);

  setWindowFlags(flags);

  changed = 0;

  ui.buttonGroup->setId(ui.type0RadioButton, 0);
  ui.buttonGroup->setId(ui.type1RadioButton, 1);
  ui.buttonGroup->setId(ui.type2RadioButton, 2);
  ui.buttonGroup->setId(ui.type3RadioButton, 3);
}

void InvestigationTypeSelector::acceptEvent()
{
  changed = 1;

  close();
}

void InvestigationTypeSelector::rejectEvent()
{
  changed = 0;

  close();
}

void InvestigationTypeSelector::closeEvent(QCloseEvent *event)
{
  emit updateSignal();
}
