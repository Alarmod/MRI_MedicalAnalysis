#include "myprocessstarter.h"

MyProcessStarter::~MyProcessStarter()
{
  deletePointerInfo();
}

void MyProcessStarter::StartProcess(QMainWindow * parentWidget, QThread * parentThread, const QString &program, const QStringList &arguments, ExtendedTextEdit * teVar)
{
  te = teVar;

  myProcess = new QProcess(this);
  myProcess->setWorkingDirectory(QCoreApplication::applicationDirPath());

  connect(myProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
  connect(myProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
  connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus)), parentWidget, SLOT(isFinished(int, QProcess::ExitStatus)));

  connect((Thread *)parentThread, SIGNAL(killInternalProcessSignal()), (Thread *)parentThread, SLOT(killInternalProcess()));
  connect((Thread *)parentThread, SIGNAL(updateLogConsoleSignal()), (Thread *)parentThread, SLOT(updateLogConsole()));

  myProcess->start(program, arguments, QIODevice::ReadWrite);

  if (myProcess->waitForStarted(-1))
  {
    myProcess->waitForFinished(-1);

    delete myProcess;

    myProcess = 0;
  }
}

QProcess * MyProcessStarter::getProcess()
{
  return myProcess;
}

void MyProcessStarter::deletePointerInfo()
{
  if (myProcess)
  {
    delete myProcess;

    myProcess = 0;
  }
}

void MyProcessStarter::killProcess()
{
  if (myProcess)
  {
    myProcess->kill();
  }
}

void MyProcessStarter::readStandardOutput()
{
  m_mutex.lock();
  QByteArray processOutput;
  processOutput = myProcess->readAllStandardOutput();

#ifdef WIN32
  QTextCodec* defaultTextCodec = QTextCodec::codecForName("cp866");
  QTextCodec* htmlTextCodec = QTextCodec::codecForHtml(processOutput, defaultTextCodec);
  QString unicode = htmlTextCodec->toUnicode(processOutput);

  qWarning() << unicode.toStdString().c_str();

  emit te->dataIsChanged(unicode);
#else
  qWarning() << processOutput.constData();

  emit te->dataIsChanged(processOutput.constData());
#endif
  m_mutex.unlock();
}

void MyProcessStarter::readStandardError()
{
  m_mutex.lock();
  QByteArray processError;
  processError = myProcess->readAllStandardError();

#ifdef WIN32
  QTextCodec* defaultTextCodec = QTextCodec::codecForName("cp866");
  QTextCodec* htmlTextCodec = QTextCodec::codecForHtml(processError, defaultTextCodec);
  QString unicode = htmlTextCodec->toUnicode(processError);

  qWarning() << unicode.toStdString().c_str();

  emit te->dataIsChanged(unicode);
#else
  qWarning() << processError.constData();

  emit te->dataIsChanged(processError.constData());
#endif
  m_mutex.unlock();
}

Thread::Thread(QMainWindow * parentWidgetVar, const QString &programForStartVar, const QStringList &argumentsVar, const QString &programForStopVar, const QDateTime &time, int rowidVar)
{
  qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
  qRegisterMetaType<QTextBlock>("QTextBlock");
  qRegisterMetaType<QTextCursor>("QTextCursor");

  parentWidget = parentWidgetVar;
  program = programForStartVar;
  arguments = argumentsVar;
  stopProgram = programForStopVar;
  rowid = rowidVar;

  consoleOutput = new QWidget();
  consoleOutput->setWindowModality(Qt::WindowModality::NonModal);
  consoleOutput->setWindowTitle(tr("Console '%1' output for kernel start time '%2'").arg(QString::number(rowid), time.toString("yyyy.MM.dd hh:mm:ss.zzz")));
  consoleOutput->setMinimumSize(700, 450);
  consoleOutput->resize(700, 450);

  QVBoxLayout * pLayout = new QVBoxLayout(consoleOutput);
  te = new ExtendedTextEdit(consoleOutput);
  pLayout->setAlignment(Qt::AlignCenter);
  pLayout->setContentsMargins(0, 0, 0, 0);
  pLayout->addWidget(te);
  consoleOutput->setLayout(pLayout);

  consoleOutput->showNormal();
}

Thread::~Thread()
{
  if (starter)
  {
    delete starter;

    starter = 0;
  }

  if (te)
  {
    delete te;

    te = 0;
  }

  if (consoleOutput)
  {
    consoleOutput->close();

    delete consoleOutput;
    consoleOutput = 0;
  }
}

void Thread::run()
{
  starter = new MyProcessStarter();

  starter->StartProcess(parentWidget, this, program, arguments, te);
}

QProcess * Thread::getInternalProcess()
{
  return starter->getProcess();
}

void Thread::deleteInternalPointerInfo()
{
  if (starter)
    starter->deletePointerInfo();
}

MyProcessStarter * Thread::getStarter()
{
  return starter;
}

void Thread::killInternalProcess()
{
  MyProcessStarter * starter = getStarter();
  if (starter)
  {
    starter->killProcess();

    // убиваем запущенные вспомогательные процессы
    QProcess * myProcess = new QProcess(this);
    myProcess->start(stopProgram);

    emit updateLogConsoleSignal();
  }
}

void Thread::updateLogConsole()
{
  if (consoleOutput && te)
  {
    emit te->dataIsChanged("Process has been killed");
  }
}
