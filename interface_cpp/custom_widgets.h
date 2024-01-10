#ifndef CUSTOM_WIDGETS_H
#define CUSTOM_WIDGETS_H

#include <QCheckBox>
#include <QTextEdit>
#include <QTableWidgetItem>
#include <QUuid>

class CustomCheckTableWidgetItem : public QCheckBox, public QTableWidgetItem
{
  Q_OBJECT

public:
  CustomCheckTableWidgetItem(bool checked = false)
  {
    setChecked(checked);
  }

private:
  bool CustomCheckTableWidgetItem::operator<(const QTableWidgetItem& other) const
  {
    const QCheckBox *that = dynamic_cast<const QCheckBox *>(&other);
    if (that != 0)
    {
      if (this->isChecked() && that->isChecked() == false)
        return true;
    }
    return false;
  }
};

class CustomCellTableWidgetItem : public QTextEdit, public QTableWidgetItem
{
  Q_OBJECT

public:

  int activated;

  int period;
  int period_combined;

  int snapshot_type;
  int snapshot_type_combined;

  QString path;
  QString path_combined;

  bool all_snapshots;
  bool all_snapshots_combined;

  int first_snapshot_index;
  int last_snapshot_index;

  int first_snapshot_index_combined;
  int last_snapshot_index_combined;

  int result_index;

  CustomCellTableWidgetItem()
  {
    connect(this, SIGNAL(selectionChanged()), this, SLOT(newSelection()));

    activated = 0;  

    period = -1;
    period_combined = -1;

    snapshot_type = 0;
    snapshot_type_combined = 0;

    path = QString();
    path_combined = QString();

    all_snapshots = true;
    all_snapshots_combined = true;

    first_snapshot_index = -1;
    last_snapshot_index = -1;

    first_snapshot_index_combined = -1;
    last_snapshot_index_combined = -1;

    result_index = -1;

    QPalette p = this->palette();
    p.setColor(QPalette::Base, QColor(225, 225, 225));
    p.setColor(QPalette::WindowText, QColor(225, 225, 225));
    p.setColor(QPalette::Highlight, QColor(225, 225, 225));
    p.setColor(QPalette::Foreground, QColor(225, 225, 225));
    setPalette(p);

    setTextInteractionFlags(Qt::NoTextInteraction);

    setReadOnly(true);

    setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    viewport()->setCursor(Qt::PointingHandCursor);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }

  protected slots:
  void newSelection()
  {
    QTextCursor c = textCursor();
    c.setPosition(0);
    c.setPosition(0, QTextCursor::KeepAnchor);
    setTextCursor(c);
  };
};

#endif // CUSTOM_WIDGETS_H
