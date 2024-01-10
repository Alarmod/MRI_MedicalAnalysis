#include "extendedtextedit.h"

ExtendedTextEdit::ExtendedTextEdit(QWidget *parent)
{
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFrameStyle(QFrame::NoFrame);
  QPalette p = palette();
  p.setColor(QPalette::ColorRole::Base, Qt::black);
  setPalette(p);

  setStyleSheet("QTextEdit"
    "{"
    "color: white;"
    "}");

  setReadOnly(true);

  QObject::connect(this, SIGNAL(dataIsChanged(const QString &)), this, SLOT(setAppData(const QString &)));
}

void ExtendedTextEdit::setAppData(const QString &value)
{
  append(value);
}
