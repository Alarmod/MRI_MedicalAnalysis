#ifndef EXTENDEDTEXTEDIT_H
#define EXTENDEDTEXTEDIT_H
#include <QTextEdit>
#include <QTextBlock>
#include <QTextCursor>

class ExtendedTextEdit : public QTextEdit
{
  Q_OBJECT

public:
  ExtendedTextEdit(QWidget *parent = 0);

  public slots:
  void setAppData(const QString &value);

signals:
  void dataIsChanged(const QString &value);
};

#endif
