#ifndef SELECTTYPEDIALOG_H
#define SELECTTYPEDIALOG_H

#include "ui_selecttypedialog.h"

#include <QtGui>

class InvestigationTypeSelector : public QWidget
{
  Q_OBJECT

public:
  InvestigationTypeSelector(QWidget *parent = Q_NULLPTR);
  Ui::InvestigationTypeSelector ui;
  int changed;

signals:
  void updateSignal();

  private slots:
  void acceptEvent();
  void rejectEvent();

private:
  void InvestigationTypeSelector::closeEvent(QCloseEvent *event);
};

#endif
