#ifndef DICOMVIEWER_H
#define DICOMVIEWER_H

#include <QtGui>
#include <QDir>
#include <QCollator>
#include <QLineEdit>

#include "aspectratiopixmaplabel.h"
#include "custom_widgets.h"

#include "ui_dicomViewer.h"

#include "dicomlib/Buffer.hpp"
#include "dicomlib/DataSet.hpp"
#include "dicomlib/File.hpp"
#include "dicomlib/Tag.hpp"

using namespace std;

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>

#define _USE_MATH_DEFINES

class dicomViewer : public QDialog, private Ui::dicomDialog
{
  Q_OBJECT

private:
  AspectRatioPixmapLabel * imageLabel;

  int rowtype;
  CustomCellTableWidgetItem * ct;
  int mode;
  QUuid uid;
  int sType;

  void closeEvent(QCloseEvent *bar);

public:
  dicomViewer(int type_index_var, CustomCellTableWidgetItem * ct_var, int mode_var, QUuid uid_var, int sType_var, QWidget *parent = 0);

  void * cthread;

  public slots:
  void spinBoxValueChanged(int value);
  void show3D();
  void acceptCommand();
};

#endif
