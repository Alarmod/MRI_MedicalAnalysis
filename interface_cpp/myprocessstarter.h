#ifndef MYPROCESSSTARTER_H
#define MYPROCESSSTARTER_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QThread>
#include <QTextCodec>
#include <QMainWindow>
#include <QWidget>
#include <QMutex>
#include <QVBoxLayout>
#include <QDateTime>
#include <QCoreApplication>

#ifdef WIN32
#include <windows.h>
#endif

#include "extendedtextedit.h"

class MyProcessStarter : public QObject
{
  Q_OBJECT

public:
  ~MyProcessStarter();
  void StartProcess(QMainWindow * parentWidget, QThread * parentThread, const QString &program, const QStringList &arguments, ExtendedTextEdit * teVar);
  QProcess * getProcess();
  void deletePointerInfo();
  void killProcess();

private:
  QProcess *myProcess;
  QMutex	m_mutex;
  ExtendedTextEdit * te;

  private slots:
  void readStandardOutput();
  void readStandardError();
};

class Thread : public QThread
{
  Q_OBJECT

public:
  Thread(QMainWindow * parentWidgetVar, const QString &programForStartVar, const QStringList &argumentsVar, const QString &programForStopVar, const QDateTime &time, int rowid);
  ~Thread();

  QWidget * consoleOutput;
  ExtendedTextEdit * te;

  int rowid;

  MyProcessStarter * getStarter();
  QProcess * getInternalProcess();
  void deleteInternalPointerInfo();

signals:
  void killInternalProcessSignal();
  void updateLogConsoleSignal();

  private slots:
  void killInternalProcess();
  void updateLogConsole();

protected:
  void run();

private:
  QMainWindow * parentWidget;
  QString program;
  QStringList arguments;
  QString stopProgram;
  MyProcessStarter * starter;
};

#endif
