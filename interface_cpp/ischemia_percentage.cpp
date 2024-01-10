#include "ischemia_percentage.h"

IschemiaPercentage::IschemiaPercentage(CustomCellTableWidgetItem * ct_var,  QUuid uid_var, QWidget *parent)
{
  ct = ct_var;
  uid = uid_var; 

  ui.setupUi(this);

  Qt::WindowFlags flags = windowFlags();
  Qt::WindowFlags minFlag = Qt::WindowMinimizeButtonHint;
  Qt::WindowFlags maxFlag = Qt::WindowMaximizeButtonHint;
  Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;

  flags = flags & minFlag & maxFlag & (~helpFlag);
  setWindowFlags(flags);

  ui.textEdit->setReadOnly(true);

  ui.textEdit->setTextInteractionFlags(Qt::NoTextInteraction);

  connect(this, SIGNAL(ui.textEdit->selectionChanged()), this, SLOT(newSelection()));

  QString percentagePath = tr("%1%2results%3%4%5ischemia_percentage_%6.html").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), uid_var.toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

  QFile file(percentagePath);

  QString line;
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QTextStream stream(&file);
    while (!stream.atEnd()){
      line.append(stream.readLine() + "\n");
    }
    
    ui.textEdit->setText(line);
  }
  file.close();
}

void IschemiaPercentage::newSelection()
{
  QTextCursor c = ui.textEdit->textCursor();
  c.setPosition(0);
  c.setPosition(0, QTextCursor::KeepAnchor);
  ui.textEdit->setTextCursor(c);
}
