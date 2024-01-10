#pragma once

#include <QWidget>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>

#include "ui_NewSeriesFormCombined.h"

class NewSeriesFormCombined : public QWidget
{
  Q_OBJECT

public:
  NewSeriesFormCombined(QWidget *parent = Q_NULLPTR);
  ~NewSeriesFormCombined();
  Ui::NewSeriesFormCombined ui;
  int changed;

signals:
  void updateSignal();

  private slots:
  void saveChangesAndClose();

  void mskSnapshotSelectorCheckBoxStatusChanged();
  void ischemiaSnapshotSelectorCheckBoxStatusChanged();

  void mskSelectPath();
  void ischemiaSelectPath();

  void mskPathUpdated();
  void ischemiaPathUpdated();

private:
  void NewSeriesFormCombined::closeEvent(QCloseEvent *event);

  // "Прошло дней от начала экспериментов:"
  const QString typeParamHeader = QApplication::translate("NewSeriesFormCombined", "\320\237\321\200\320\276\321\210\320\273\320\276 \320\264\320\275\320\265\320\271 \320\276\321\202 \320\275\320\260\321\207\320\260\320\273\320\260 \321\215\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202\320\260:", Q_NULLPTR);
};
