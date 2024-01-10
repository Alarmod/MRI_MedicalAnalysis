#include "mainwindow.h"

#include <stdlib.h>
#include <windows.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/qDesktopwidget>

#ifdef __GNUC__
#define _stdcall  __attribute__((stdcall))
#endif

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  int point_size = floor(12.0 * 96.0 / (qreal)a.desktop()->logicalDpiX());

  char buff[256];
  sprintf(buff, "QWidget {font-size: %dpt}", point_size);
  a.setStyleSheet(buff);

  QDir::setCurrent(QCoreApplication::applicationDirPath());

  MainWindow w;
  w.show();
  return a.exec();
}

int _stdcall WinMain(struct HINSTANCE__ *hInstance, struct HINSTANCE__ *hPrevInstance, char *lpszCmdLine,  int nCmdShow)
{
  return main(__argc, __argv);
}