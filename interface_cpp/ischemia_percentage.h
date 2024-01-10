#ifndef ISCHEMIA_PERCENTAGE_H
#define ISCHEMIA_PERCENTAGE_H

#include <QtGui>

#include "ui_ischemia_percentage.h"

#include "custom_widgets.h"

class IschemiaPercentage : public QDialog
{
  Q_OBJECT

private:
  CustomCellTableWidgetItem * ct;
  QUuid uid;

public:
  Ui::IschemiaPercentage ui;
  IschemiaPercentage(CustomCellTableWidgetItem * ct_var, QUuid uid_var, QWidget *parent = 0);

  protected slots:
  void newSelection();
};

#endif
