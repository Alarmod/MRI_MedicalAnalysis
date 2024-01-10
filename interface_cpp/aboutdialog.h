#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "ui_aboutdialog.h"

#include <QtGui/QtGui>

class AboutDialog : public QDialog, private Ui::AboutDialog
{
  Q_OBJECT

public:
  AboutDialog(QWidget *parent = 0);
};

#endif
