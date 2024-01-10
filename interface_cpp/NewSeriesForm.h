#pragma once

#include <QWidget>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>

#include "ui_NewSeriesForm.h"

class NewSeriesForm : public QWidget
{
  Q_OBJECT

public:
  NewSeriesForm(unsigned int type_of_form, QWidget *parent = Q_NULLPTR);
  ~NewSeriesForm();
  Ui::NewSeriesForm ui;
  int changed;

signals:
  void updateSignal();

  private slots:
  void saveChangesAndClose();
  void snapshotSelectorCheckBoxStatusChanged();
  void selectPath();
  void pathUpdated();

private:
  void NewSeriesForm::closeEvent(QCloseEvent *event);

  // ""
  const QString type0ParamHeader = QString();

  // "ƒней от дн€ начала ишемического поражени€ (-1 дл€ здорового мозга):"
  const QString type1ParamHeader = QApplication::translate("NewSeriesForm", "\320\224\320\275\320\265\320\271 \320\276\321\202 \320\264\320\275\321\217 \320\275\320\260\321\207\320\260\320\273\320\260 \320\270\321\210\320\265\320\274\320\270\321\207\320\265\321\201\320\272\320\276\320\263\320\276 \320\277\320\276\321\200\320\260\320\266\320\265\320\275\320\270\321\217 (-1 \320\264\320\273\321\217 \320\267\320\264\320\276\321\200\320\276\320\262\320\276\320\263\320\276 \320\274\320\276\320\267\320\263\320\260):", Q_NULLPTR);

  // "ƒней от дн€ введени€ клеток (-1 дл€ снимков до введени€):"
  const QString type2ParamHeader = QApplication::translate("NewSeriesForm", "\320\224\320\275\320\265\320\271 \320\276\321\202 \320\264\320\275\321\217 \320\262\320\262\320\265\320\264\320\265\320\275\320\270\321\217 \320\272\320\273\320\265\321\202\320\276\320\272 (-1 \320\264\320\273\321\217 \321\201\320\275\320\270\320\274\320\272\320\276\320\262 \320\264\320\276 \320\262\320\262\320\265\320\264\320\265\320\275\320\270\321\217):", Q_NULLPTR);

  // "ƒней от начала экспериментов (-1 дл€ снимков без вмешательства):"
  const QString type3ParamHeader = QApplication::translate("NewSeriesForm", "\320\224\320\275\320\265\320\271 \320\276\321\202 \320\275\320\260\321\207\320\260\320\273\320\260 \321\215\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202\320\276\320\262 (-1 \320\264\320\273\321\217 \321\201\320\275\320\270\320\274\320\272\320\276\320\262 \320\261\320\265\320\267 \320\262\320\274\320\265\321\210\320\260\321\202\320\265\320\273\321\214\321\201\321\202\320\262\320\260):", Q_NULLPTR);
};
