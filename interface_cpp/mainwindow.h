#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QApplication>
#include <QCoreApplication>
#include <QtWidgets/qDesktopwidget>
#include <QtWidgets/QMainWindow>
#include <QCheckBox>
#include <QToolButton>
#include <QLabel>
#include <QEventLoop>
#include <QFont>
#include <QStringList>
#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include <QMessageBox>
#include <QFileInfo>
#include <QUuid>
#include <QSignalMapper>

#include "ui_mainwindow.h"

#include "NewSeriesForm.h"
#include "NewSeriesFormCombined.h"

#include "aboutdialog.h"
#include "myprocessstarter.h"
#include "dicomViewer.h"
#include "custom_widgets.h"
#include "selecttypedialog.h"
#include "ischemia_percentage.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void updateDataCell(CustomCellTableWidgetItem * ct);

  static const unsigned int number_of_saving_parameters = 14U;

private:
  Ui::MainWindowClass ui;
  int datarows_start, datacols_start;
  QList<Thread *> tlist;
  Thread * taskMainThread;

  QList<QUuid> uids;
  QList<unsigned int> types;

  int current_thread_index;

protected:
  bool eventFilter(QObject *obj, QEvent *event);
  void closeEvent(QCloseEvent *event);

  private slots:
  void AddNewSeries();
  void AddNewSeries(QList<QStringList> * data, int i_iterator);
  void SelectAll();
  void DeselectAll();
  void DeleteSelected();
  void RunTasks();
  void ChangeSettings();
  void AboutProgram();
  void isFinished(int exitCode, QProcess::ExitStatus exitStatus);

  void visualizeData(QWidget * ct_obj);
  void visualizeData_msk(QWidget * ct_obj);
  void visualizeData_ischemia(QWidget * ct_obj);

  void visualizeResults(QWidget * ct_obj);
  void deleteSeriesInformation(QWidget * ct_obj);
  void updateSeriesInformation(QWidget * ct_obj);
  void visualizeIschemiaPercentage(QWidget * ct_obj);
};

#endif // MAINWINDOW_H
