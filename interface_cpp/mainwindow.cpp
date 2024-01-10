#include "mainwindow.h"

bool fileExists(QString path) {
  QFileInfo check_file(path);

  return (check_file.exists() && check_file.isFile());
}

QString settingsPath()
{
  return QCoreApplication::applicationDirPath() + "/";
}

QString appBaseName()
{
  QString appBaseName = QCoreApplication::applicationFilePath();
  QStringList sl = appBaseName.split("/");
  appBaseName = sl[sl.count() - 1];

  appBaseName.replace(".exe", "", Qt::CaseInsensitive);

  return appBaseName;
}

void MainWindow::updateDataCell(CustomCellTableWidgetItem * ct)
{
  int rowid = ct->row();
  int type = this->types[rowid];

  //qWarning("rowid: %d, type: %d", rowid, type);

  QDir dir;
  if (ct->activated == 1)
  {
    QString var0;
    if (ct->path == QString())
      var0 = QApplication::translate("MainWindowClass", "<B>\320\237\321\203\321\202\321\214</B>: \320\275\320\265 \320\267\320\260\320\264\320\260\320\275", Q_NULLPTR);
    else
    {
      if (ct->path != QString() && dir.exists(ct->path))
        var0 = QApplication::translate("MainWindowClass", "<B>\320\237\321\203\321\202\321\214</B>: \320\267\320\260\320\264\320\260\320\275", Q_NULLPTR);
      else
        var0 = QApplication::translate("MainWindowClass", "<B>\320\237\321\203\321\202\321\214</B>: <FONT COLOR=\"RED\">\320\267\320\260\320\264\320\260\320\275</FONT>", Q_NULLPTR);
    }

    QString var1;
    switch (ct->snapshot_type)
    {
    case 0:
    {
      var1 = QApplication::translate("MainWindowClass", "SWI-\321\201\320\275\320\270\320\274\320\272\320\270", Q_NULLPTR);
      break;
    }
    case 1:
    {
      var1 = QApplication::translate("MainWindowClass", "T2-\321\201\320\275\320\270\320\274\320\272\320\270", Q_NULLPTR);

      break;
    }
    case 2:
    {
      var1 = QApplication::translate("MainWindowClass", "\320\230\320\232\320\224-\321\201\320\275\320\270\320\274\320\272\320\270", Q_NULLPTR);
      break;
    }
    default:
      var1 = "undefined";
      break;
    }

    QString var2 = ((ct->all_snapshots || (ct->path == QString()) || !dir.exists(ct->path) || (ct->first_snapshot_index == -1 && ct->last_snapshot_index == -1)) ? "-" : tr("%1 - %2").arg(QString::number(ct->first_snapshot_index).toStdString().c_str(), QString::number(ct->last_snapshot_index).toStdString().c_str()));

    switch (type)
    {
    case 0:
    {
      QString pattern = "<HTML>" \
        "<BODY style=\"margin-top:22px;margin-bottom:22px;\">" \
        "<TABLE width=\"100%\">" \
        "<TR><TD width = \"100%\" align=\"center\">%1</TR>" \
        "<TR><TD width = \"100%\" align=\"center\"><B>\320\242\320\270\320\277</B> \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: %2</TD></TR>" \
        "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275</B>: %3</TD></TR>" \
        "</TABLE>" \
        "</BODY>" \
        "</HTML>";

      ct->document()->setHtml(tr(QApplication::translate("MainWindowClass", pattern.toStdString().c_str(), Q_NULLPTR).toStdString().c_str()).arg(var0, var1, var2));

      break;
    }

    case 1:
    {
      if (ct->period == -1)
      {
        QString pattern = "<HTML>" \
          "<BODY style=\"margin-top:8px;margin-bottom:8px;\">" \
          "<TABLE width=\"100%\">" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\276</B> \320\277\320\276\321\200\320\260\320\266\320\265\320\275\320\270\321\217</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\">%1</TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\242\320\270\320\277</B> \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: %2</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275</B>: %3</TD></TR>" \
          "</TABLE>" \
          "</BODY>" \
          "</HTML>";

        ct->document()->setHtml(tr(QApplication::translate("MainWindowClass", pattern.toStdString().c_str(), Q_NULLPTR).toStdString().c_str()).arg(var0, var1, var2));
      }
      else
      {
        QString pattern = "<HTML>" \
          "<BODY>" \
          "<TABLE width=\"100%\">" \
          "<TR><TD width=\"100%\" align=\"center\">" \
          "<B>\320\224\320\275\320\265\320\271</B> \320\277\320\276\321\201\320\273\320\265" \
          "<br />"  \
          "\320\277\320\276\321\200\320\260\320\266\320\265\320\275\320\270\321\217: %1" \
          "</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\">%2</TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\242\320\270\320\277</B> \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: %3</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275</B>: %4</TD></TR>" \
          "</TABLE>" \
          "</BODY>" \
          "</HTML>";

        ct->document()->setHtml(tr(QApplication::translate("MainWindowClass", pattern.toStdString().c_str(), Q_NULLPTR).toStdString().c_str()).arg(QString::number(ct->period), var0, var1, var2));
      }

      break;
    }

    case 2:
    {
      qWarning("var0: %s", var0.toStdString().c_str());
      qWarning("var1: %s", var1.toStdString().c_str());
      qWarning("var1: %s", var2.toStdString().c_str());

      if (ct->period == -1)
      {
        QString pattern = "<HTML>" \
          "<BODY style=\"margin-top:8px;margin-bottom:8px;\">" \
          "<TABLE width=\"100%\">" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\276</B> \320\262\320\262\320\265\320\264\320\265\320\275\320\270\321\217</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\">%1</TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\242\320\270\320\277</B> \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: %2</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275</B>: %3</TD></TR>" \
          "</TABLE>" \
          "</BODY>" \
          "</HTML>";

        ct->document()->setHtml(tr(QApplication::translate("MainWindowClass", pattern.toStdString().c_str(), Q_NULLPTR).toStdString().c_str()).arg(var0, var1, var2));
      }
      else
      {
        QString pattern = "<HTML>" \
          "<BODY>" \
          "<TABLE width=\"100%\">" \
          "<TR><TD width=\"100%\" align=\"center\">" \
          "<B>\320\224\320\275\320\265\320\271</B> \320\277\320\276\321\201\320\273\320\265" \
          "<br />"  \
          "\320\262\320\262\320\265\320\264\320\265\320\275\320\270\321\217: %1" \
          "</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\">%2</TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\242\320\270\320\277</B> \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: %3</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275</B>: %4</TD></TR>" \
          "</TABLE>" \
          "</BODY>" \
          "</HTML>";

        ct->document()->setHtml(tr(QApplication::translate("MainWindowClass", pattern.toStdString().c_str(), Q_NULLPTR).toStdString().c_str()).arg(QString::number(ct->period), var0, var1, var2));
      }

      break;
    }

    case 3:
    {
      if (!ct->activated)
      {
        ct->period = 0;

        ct->period_combined = 0;
      }

      if (ct->column() == 1)
      {
        QString pattern = "<HTML>" \
          "<BODY style=\"margin-top:98px;margin-bottom:98px;\">" \
          "<TABLE width=\"100%\">" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\276</B> \321\215\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202\320\260</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\">%1</TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\242\320\270\320\277</B> \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: %2</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275</B>: %3</TD></TR>" \
          "</TABLE>" \
          "</BODY>" \
          "</HTML>";

        ct->document()->setHtml(tr(QApplication::translate("MainWindowClass", pattern.toStdString().c_str(), Q_NULLPTR).toStdString().c_str()).arg(var0, var1, var2));
      }
      else
      {
        QString var0_combined;
        if (ct->path_combined == QString())
          var0_combined = QApplication::translate("MainWindowClass", "<B>\320\237\321\203\321\202\321\214</B>: \320\275\320\265 \320\267\320\260\320\264\320\260\320\275", Q_NULLPTR);
        else
        {
          if (ct->path_combined != QString() && dir.exists(ct->path_combined))
            var0_combined = QApplication::translate("MainWindowClass", "<B>\320\237\321\203\321\202\321\214</B>: \320\267\320\260\320\264\320\260\320\275", Q_NULLPTR);
          else
            var0_combined = QApplication::translate("MainWindowClass", "<B>\320\237\321\203\321\202\321\214</B>: <FONT COLOR=\"RED\">\320\267\320\260\320\264\320\260\320\275</FONT>", Q_NULLPTR);
        }

        QString var1_combined;
        switch (ct->snapshot_type_combined)
        {
        case 0:
        {
          var1_combined = QApplication::translate("MainWindowClass", "SWI-\321\201\320\275\320\270\320\274\320\272\320\270", Q_NULLPTR);
          break;
        }
        case 1:
        {
          var1_combined = QApplication::translate("MainWindowClass", "T2-\321\201\320\275\320\270\320\274\320\272\320\270", Q_NULLPTR);

          break;
        }
        case 2:
        {
          var1_combined = QApplication::translate("MainWindowClass", "\320\230\320\232\320\224-\321\201\320\275\320\270\320\274\320\272\320\270", Q_NULLPTR);
          break;
        }
        default:
          var1_combined = "undefined";
          break;
        }

        QString var2_combined = ((ct->all_snapshots_combined || (ct->path_combined == QString()) || !dir.exists(ct->path_combined) || (ct->first_snapshot_index_combined == -1 && ct->last_snapshot_index_combined == -1)) ? "-" : tr("%1 - %2").arg(QString::number(ct->first_snapshot_index_combined).toStdString().c_str(), QString::number(ct->last_snapshot_index_combined).toStdString().c_str()));

        QString pattern = "<HTML>" \
          "<BODY>" \
          "<TABLE width=\"100%\">" \
          "<TR><TD width=\"100%\" align=\"center\"><I>\320\234\320\241\320\232:</I></TD></TR>" \
          "<TR><TD width=\"100%\" align=\"center\">" \
          "<B>\320\224\320\275\320\265\320\271</B> \320\276\321\202 \320\275\320\260\321\207\320\260\320\273\320\260" \
          "<br />"  \
          "\321\215\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202\320\260: %1" \
          "</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\">%2</TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\242\320\270\320\277</B> \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: %3</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275</B>: %4<br /></TD></TR>" \
          "<TR><TD width=\"100%\" align=\"center\"><I>\320\230\321\210\320\265\320\274\320\270\321\217:</I></TD></TR>" \
          "<TR><TD width=\"100%\" align=\"center\"><B>\320\224\320\275\320\265\320\271</B> \320\276\321\202 \320\275\320\260\321\207\320\260\320\273\320\260" \
          "<br />"  \
          "\321\215\320\272\321\201\320\277\320\265\321\200\320\270\320\274\320\265\320\275\321\202\320\260: %5" \
          "</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\">%6</TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\242\320\270\320\277</B> \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: %7</TD></TR>" \
          "<TR><TD width = \"100%\" align=\"center\"><B>\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275</B>: %8</TD></TR>" \
          "</TABLE>" \
          "</BODY>" \
          "</HTML>";

        ct->document()->setHtml(tr(QApplication::translate("MainWindowClass", pattern.toStdString().c_str(), Q_NULLPTR).toStdString().c_str()).arg(QString::number(ct->period), var0, var1, var2, QString::number(ct->period_combined), var0_combined, var1_combined, var2_combined));
      }

      break;
    }

    default:
    {
      break;
    }
    }
  }
  else
  {
    ct->period = -1;
    ct->period_combined = -1;

    ct->snapshot_type = 0;
    ct->snapshot_type_combined = 0;

    ct->path = QString();
    ct->path_combined = QString();

    ct->all_snapshots = true;
    ct->all_snapshots_combined = true;

    ct->first_snapshot_index = -1;
    ct->last_snapshot_index = -1;

    ct->first_snapshot_index_combined = -1;
    ct->last_snapshot_index_combined = -1;

    ct->result_index = -1;

    ct->document()->setHtml("");
  }

  ct->setTextAlignment(Qt::AlignCenter);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  taskMainThread = 0;

  qRegisterMetaTypeStreamOperators<QList<QStringList>>("app_settings");
  QSettings * app_settings = new QSettings(settingsPath() + appBaseName() + ".ini", QSettings::IniFormat);

  ui.setupUi(this);

  ui.mainToolBar->setStyleSheet("QToolButton:!hover {background-color:lightgray};");

  foreach(QToolButton* button, ui.mainToolBar->findChildren<QToolButton*>())
    button->setToolTip(QString(""));

  // empty
  {
    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty->setFixedWidth(24);

    ui.mainToolBar->insertWidget(ui.AddNewSeries, empty);

    QWidget* empty0 = new QWidget();
    empty0->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty0->setFixedWidth(11);

    QWidget* empty1 = new QWidget();
    empty1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty1->setFixedWidth(11);

    QWidget* empty2 = new QWidget();
    empty2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty2->setFixedWidth(11);

    QWidget* empty3 = new QWidget();
    empty3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty3->setFixedWidth(11);

    QWidget* empty4 = new QWidget();
    empty4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty4->setFixedWidth(11);

    QWidget* empty5 = new QWidget();
    empty5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    empty5->setFixedWidth(11);

    ui.mainToolBar->insertWidget(ui.SelectAll, empty0);
    ui.mainToolBar->insertWidget(ui.DeselectAll, empty1);
    ui.mainToolBar->insertWidget(ui.DeleteSelected, empty2);
    ui.mainToolBar->insertWidget(ui.RunTasks, empty3);
    ui.mainToolBar->insertWidget(ui.ChangeSettings, empty4);
    ui.mainToolBar->insertWidget(ui.AboutProgram, empty5);
  }

  datarows_start = app_settings->value("datarows", QVariant(0)).toInt();
  datacols_start = app_settings->value("datacols", QVariant(6)).toInt();

  if (QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS10)
  {
    ui.tableWidget->setStyleSheet(
      "QHeaderView::section{"
      "border-top:0px solid #D8D8D8;"
      "border-left:0px solid #D8D8D8;"
      "border-right:1px solid #D8D8D8;"
      "border-bottom: 1px solid #D8D8D8;"
      "background-color:white;"
      "padding:4px;"
      "}"
      "QTableCornerButton::section{"
      "border-top:0px solid #D8D8D8;"
      "border-left:0px solid #D8D8D8;"
      "border-right:1px solid #D8D8D8;"
      "border-bottom: 1px solid #D8D8D8;"
      "background-color:white;"
      "}");
  }

  ui.tableWidget->setFrameStyle(QFrame::Box | QFrame::Plain);
  ui.tableWidget->setLineWidth(1);

  ui.tableWidget->setStyleSheet("QTableView:item:selected {background-color: #FFFFFF; color: #FFFFFF}\nQTableView:item:selected:focus {background-color: #FFFFFF;}");

  ui.tableWidget->setColumnCount(datacols_start + 1);

  ui.tableWidget->resizeColumnsToContents();

  ui.tableWidget->setColumnWidth(0, 185);

  for (int i = 0; i < datacols_start; i++)
  {
    QString data = tr(QApplication::translate("MainWindowClass", "\320\241\320\265\321\200\320\270\321\217 \321\201\320\275\320\270\320\274\320\272\320\276\320\262 \342\204\226%1", Q_NULLPTR).toStdString().c_str()).arg(QString::number(i + 1));

    QTableWidgetItem * item = new QTableWidgetItem(data);
    item->setTextAlignment(Qt::AlignHCenter);

    ui.tableWidget->setHorizontalHeaderItem(i + 1, item);

    ui.tableWidget->setColumnWidth(i + 1, 185);
  }

  QHeaderView *verticalHeader = ui.tableWidget->verticalHeader();
  verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
  verticalHeader->setDefaultSectionSize(140);

  QHeaderView *horizontalHeader = ui.tableWidget->horizontalHeader();
  horizontalHeader->setSectionResizeMode(QHeaderView::Fixed);

  QList<QStringList> & data = (QList<QStringList> &)app_settings->value("data").value<QList<QStringList>>();

  for (int i = 0; i < datarows_start && i < data.size(); i++)
    AddNewSeries(&data, i);

  delete app_settings;

  if (std::min(datarows_start, data.size()) && datacols_start)
  {
    ui.tableWidget->setCurrentItem(ui.tableWidget->item(0, 1));
  }

  ui.tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
  ui.tableWidget->setMouseTracking(true);
  ui.tableWidget->viewport()->installEventFilter(this);

  ui.tableWidget->verticalHeader()->setSectionsClickable(false);
  ui.tableWidget->horizontalHeader()->setSectionsClickable(false);

  ui.tableWidget->setFocusPolicy(Qt::NoFocus);
}

MainWindow::~MainWindow()
{}

void MainWindow::AddNewSeries()
{
  AddNewSeries(0, 0);
}

void MainWindow::AddNewSeries(QList<QStringList> * data, int i_iterator)
{
  bool doAction = true;
  unsigned int typeOfInvestigation;

  if (data)
  {
    if (i_iterator >= data->size())
      return;

    this->uids.insert(0, QUuid(data->operator[](i_iterator)[0]));

    typeOfInvestigation = (unsigned int)atoi(data->operator[](i_iterator)[1].toStdString().c_str());
    this->types.insert(0, typeOfInvestigation);
  }
  else
  {
    InvestigationTypeSelector * its = new InvestigationTypeSelector();

    its->showNormal();

    QEventLoop loop;
    connect(its, SIGNAL(updateSignal()), &loop, SLOT(quit()));
    loop.exec();

    if (its->changed)
    {
      typeOfInvestigation = its->ui.buttonGroup->checkedId();
    }
    else
    {
      typeOfInvestigation = 0U;

      doAction = false;
    }
  }

  qWarning("typeOfInvestigation: %u", typeOfInvestigation);

  // работа со строкой
  if (doAction)
  {
    if (data == 0)
    {
      this->uids.insert(0, QUuid::createUuid());

      this->types.insert(0, typeOfInvestigation);
    }

    ui.tableWidget->insertRow(0);

    if (typeOfInvestigation == 3) ui.tableWidget->setRowHeight(0, 320);

    {
      QWidget * pWidget = new QWidget(ui.tableWidget);
      pWidget->setContentsMargins(5, 0, 0, 0);
      QHBoxLayout * pLayout = new QHBoxLayout(ui.tableWidget);
      CustomCheckTableWidgetItem * cb = new CustomCheckTableWidgetItem();

      // 0 Выделение области с мозгом
      // 1 Поиск ишемических поражений
      // 2 Поиск мезенхимальных стволовых клеток
      // 3 Смешанный режим (МСК + ишемия)
      switch (typeOfInvestigation)
      {
      case 0:
      {
        ((QCheckBox *)cb)->setText(QApplication::translate("MainWindowClass", " \320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\270\320\265 \320\276\320\261\320\273\320\260\321\201\321\202\320\270\n \321\201 \320\274\320\276\320\267\320\263\320\276\320\274", Q_NULLPTR));

        break;
      }
      case 1:
      {
        ((QCheckBox *)cb)->setText(QApplication::translate("MainWindowClass", " \320\237\320\276\320\270\321\201\320\272\n \320\270\321\210\320\265\320\274\320\270\321\207\320\265\321\201\320\272\320\270\321\205\n \320\277\320\276\321\200\320\260\320\266\320\265\320\275\320\270\320\271", Q_NULLPTR));

        break;
      }
      case 2:
      {
        ((QCheckBox *)cb)->setText(QApplication::translate("MainWindowClass", " \320\237\320\276\320\270\321\201\320\272\n \320\274\320\265\320\267\320\265\320\275\321\205\320\270\320\274\320\260\320\273\321\214\320\275\321\213\321\205\n \321\201\321\202\320\262\320\276\320\273\320\276\320\262\321\213\321\205 \320\272\320\273\320\265\321\202\320\276\320\272", Q_NULLPTR));

        break;
      }
      case 3:
      {
        ((QCheckBox *)cb)->setText(QApplication::translate("MainWindowClass", " \320\241\320\274\320\265\321\210\320\260\320\275\320\275\321\213\320\271 \321\200\320\265\320\266\320\270\320\274\n (\320\234\320\241\320\232 + \320\270\321\210\320\265\320\274\320\270\321\217)", Q_NULLPTR));

        break;
      }

      default:
      {
        break;
      }
      }
      cb->setTextAlignment(Qt::AlignLeft);

      pLayout->addWidget(cb);
      pLayout->setAlignment(Qt::AlignLeft);
      pLayout->setContentsMargins(0, 0, 0, 0);
      pWidget->setLayout(pLayout);

      ui.tableWidget->setItem(0, 0, cb);
      ui.tableWidget->setCellWidget(0, 0, pWidget);
    }

    unsigned int base_number;
    for (unsigned int i = 1; i < ui.tableWidget->columnCount(); i++)
    {
      QWidget * pWidget = new QWidget(ui.tableWidget);
      QHBoxLayout * pLayout = new QHBoxLayout(ui.tableWidget);
      CustomCellTableWidgetItem * ct = new CustomCellTableWidgetItem();

      pLayout->addWidget(ct);
      pLayout->setAlignment(Qt::AlignHCenter);
      pLayout->setContentsMargins(0, 0, 0, 0);
      pWidget->setLayout(pLayout);

      ct->viewport()->installEventFilter(this);
      ct->viewport()->setMouseTracking(true);

      ui.tableWidget->setItem(0, i, ct);
      ui.tableWidget->setCellWidget(0, i, pWidget);

      ct->clearFocus();

      if (data)
      {
        base_number = number_of_saving_parameters * (i - 1U) + 2U;

        ct->activated = data->operator[](i_iterator)[base_number++].toInt();

        ct->period = data->operator[](i_iterator)[base_number++].toInt();
        ct->period_combined = data->operator[](i_iterator)[base_number++].toInt();

        ct->snapshot_type = data->operator[](i_iterator)[base_number++].toInt();
        ct->snapshot_type_combined = data->operator[](i_iterator)[base_number++].toInt();

        ct->path = data->operator[](i_iterator)[base_number++];
        ct->path_combined = data->operator[](i_iterator)[base_number++];

        ct->all_snapshots = (data->operator[](i_iterator)[base_number++] == "true");
        ct->all_snapshots_combined = (data->operator[](i_iterator)[base_number++] == "true");

        ct->first_snapshot_index = data->operator[](i_iterator)[base_number++].toInt();
        ct->last_snapshot_index = data->operator[](i_iterator)[base_number++].toInt();

        ct->first_snapshot_index_combined = data->operator[](i_iterator)[base_number++].toInt();
        ct->last_snapshot_index_combined = data->operator[](i_iterator)[base_number++].toInt();

        ct->result_index = data->operator[](i_iterator)[base_number++].toInt();

        updateDataCell(ct);
      }
    }
  }
}

void MainWindow::SelectAll()
{
  for (int i = 0; i < ui.tableWidget->rowCount(); i++)
  {
    CustomCheckTableWidgetItem * cb = dynamic_cast<CustomCheckTableWidgetItem *>(ui.tableWidget->item(i, 0));

    cb->setChecked(true);
  }
}

void MainWindow::DeselectAll()
{

  for (int i = 0; i < ui.tableWidget->rowCount(); i++)
  {
    CustomCheckTableWidgetItem * cb = dynamic_cast<CustomCheckTableWidgetItem *>(ui.tableWidget->item(i, 0));

    cb->setChecked(false);
  }
}

void MainWindow::DeleteSelected()
{
  for (int i = 0; i < ui.tableWidget->rowCount(); i++)
  {
    CustomCheckTableWidgetItem * cb = dynamic_cast<CustomCheckTableWidgetItem *>(ui.tableWidget->item(i, 0));

    if (cb->isChecked())
    {
      // очистка директорий от временных файлов
      {
        QDir results_folder(tr("%1%2results%3%4").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[i].toString()));
        if (results_folder != QString() && results_folder.exists())
        {
          try
          {
            results_folder.removeRecursively();
          }
          catch (...) {}
        }
      }

      ui.tableWidget->removeRow(i);
      this->uids.erase(this->uids.begin() + i);
      this->types.erase(this->types.begin() + i);

      i--;
    }
  }
}

void MainWindow::RunTasks()
{
  current_thread_index = 0;
  for (int i = 0; i < ui.tableWidget->rowCount(); i++)
  {
    const int type_id = this->types[current_thread_index];

    CustomCheckTableWidgetItem * cb = dynamic_cast<CustomCheckTableWidgetItem *>(ui.tableWidget->item(i, 0));

    if (cb->isChecked())
    {
      QString batfile_path = tr("%1%2run_kernel_%3.bat").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QString::number(type_id));
      if (fileExists(batfile_path))
      {
        QStringList llist;
        llist.append(this->uids[i].toString());

        unsigned int subsections = 0;
        bool have_before_data = false;
        for (int k = 1; k < ui.tableWidget->columnCount(); k++)
        {
          CustomCellTableWidgetItem * ct = dynamic_cast<CustomCellTableWidgetItem *>(ui.tableWidget->item(i, k));

          QDir dir;

          if (type_id == 3 && k > 1)
          {
            if (ct->activated && dir.exists(ct->path) && (ct->path != QString()) && dir.exists(ct->path_combined) && (ct->path_combined != QString()))
            {
              llist.append(QString::number(ct->period) + ":" + QString::number(ct->period_combined));

              llist.append(QString::number(ct->snapshot_type) + ":" + QString::number(ct->snapshot_type_combined));

              llist.append(ct->path + ":" + ct->path_combined);

              llist.append(QString::number((int)ct->all_snapshots) + ":" + QString::number((int)ct->all_snapshots_combined));

              llist.append(QString::number(ct->first_snapshot_index) + ":" + QString::number(ct->first_snapshot_index_combined));
              llist.append(QString::number(ct->last_snapshot_index) + ":" + QString::number(ct->last_snapshot_index_combined));

              ct->result_index = (int)(subsections - 1U);

              subsections++;
            }
          }
          else
          {
            if (ct->activated && dir.exists(ct->path) && (ct->path != QString()))
            {
              llist.append(QString::number(ct->period));
              llist.append(QString::number(ct->snapshot_type));
              llist.append(ct->path);
              llist.append(QString::number((int)ct->all_snapshots));
              llist.append(QString::number(ct->first_snapshot_index));
              llist.append(QString::number(ct->last_snapshot_index));

              if (type_id == 2 || type_id == 3)
              {
                if (k > 1) ct->result_index = (int)(subsections - 1U);
                else
                {
                  ct->result_index = -1;

                  have_before_data = true;
                }
              }
              else
              {
                ct->result_index = (int)(subsections);
              }

              subsections++;
            }
          }
        }

        if ((type_id == 0 && subsections > 0U) || (type_id == 1 && subsections > 0U) || ((type_id == 2 || type_id == 3) && have_before_data && subsections > 1U))
        {
          // создание рабочей директории
          {
            QString qpath = tr("%1%2results%3%4").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[i].toString());

            QDir results_folder(qpath);
            if (results_folder != QString() && results_folder.exists())
            {
              try
              {
                results_folder.removeRecursively();
              }
              catch (...) {}
            }

            try
            {
              results_folder.mkpath(qpath);
            }
            catch (...) {}

            if (type_id == 3)
            {
              QString qpath_msk = tr("%1%2results%3%4%3msk").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[i].toString());

              QDir results_folder_msk(qpath_msk);
              if (results_folder_msk != QString() && results_folder_msk.exists())
              {
                try
                {
                  results_folder_msk.removeRecursively();
                }
                catch (...) {}
              }

              try
              {
                results_folder_msk.mkpath(qpath_msk);
              }
              catch (...) {}

              QString qpath_ischemia = tr("%1%2results%3%4%3ischemia").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[i].toString());

              QDir results_folder_ischemia(qpath_ischemia);
              if (results_folder_ischemia != QString() && results_folder_ischemia.exists())
              {
                try
                {
                  results_folder_ischemia.removeRecursively();
                }
                catch (...) {}
              }

              try
              {
                results_folder_ischemia.mkpath(qpath_ischemia);
              }
              catch (...) {}
            }
          }

          // заблокировать кнопки
          ui.AddNewSeries->setEnabled(false);
          ui.SelectAll->setEnabled(false);
          ui.DeselectAll->setEnabled(false);
          ui.DeleteSelected->setEnabled(false);
          ui.RunTasks->setEnabled(false);
          ui.ChangeSettings->setEnabled(false);

          // заблокировать панель
          ui.tableWidget->setEnabled(false);

          // сохранить данные
          closeEvent(0);

          // запустить счет
          QDateTime time = QDateTime::currentDateTime();
          current_thread_index++;

          taskMainThread = new Thread(this, batfile_path, llist, QString(), time, current_thread_index);
          taskMainThread->te->dataIsChanged(tr("uid: %1").arg(llist[0]));

          for (int q = 0; q < subsections; q++)
          {
            QStringList local_list;

            if (type_id == 3)
            {
              if (q > 0)
              {
                for (int p = 0; p < number_of_saving_parameters - 8U; p++)
                  local_list.append(llist.at(number_of_saving_parameters - 7U + (q - 1) * (number_of_saving_parameters - 8U) + p));
              }
              else
              {
                for (int p = 0; p < number_of_saving_parameters - 8U; p++)
                  local_list.append(llist.at(1U + p));
              }
            }
            else
            {
              for (int p = 0; p < number_of_saving_parameters - 8U; p++)
                local_list.append(llist.at(1U + q * (number_of_saving_parameters - 8U) + p));
            }

            taskMainThread->te->dataIsChanged(tr("arguments[%1]: %2").arg(QString::number(q), local_list.join(' ')));
          }

          taskMainThread->start();
          tlist.push_back(taskMainThread);

          break;
        }
      }
      else
      {
        qWarning("%s", tr("Can't find file '%1'").arg(batfile_path).toStdString().c_str());
      }
    }

    current_thread_index++;
  }
}

void MainWindow::isFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  // Сообщение
  QString title = QApplication::translate("MainWindowClass", "\320\241\320\276\320\276\320\261\321\211\320\265\320\275\320\270\320\265", 0);
  QString message;
  QMessageBox::Icon icon;

  if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit)
  {
    qWarning("Finished with exit code '%d' and NormalExit", exitCode);

    // Задача успешно выполнена!
    message = tr(QApplication::translate("MainWindowClass", "\320\227\320\260\320\264\320\260\321\207\320\260 '%1' \321\203\321\201\320\277\320\265\321\210\320\275\320\276 \320\262\321\213\320\277\320\276\320\273\320\275\320\265\320\275\320\260!", 0).toStdString().c_str()).arg(QString::number(taskMainThread->rowid));

    // Информационная иконка
    icon = QMessageBox::Icon::Information;
  }
  else
  {
    qWarning("Finished with exit code '%d' and CrashExit", exitCode);

    // Задача не выполнена!
    message = tr(QApplication::translate("MainWindowClass", "\320\227\320\260\320\264\320\260\321\207\320\260 '%1' \320\275\320\265 \320\262\321\213\320\277\320\276\320\273\320\275\320\265\320\275\320\260!", 0).toStdString().c_str()).arg(QString::number(taskMainThread->rowid));

    // Критическая иконка
    icon = QMessageBox::Icon::Critical;
  }

  QMessageBox* pmbx = new QMessageBox(title, message, icon, QMessageBox::Ok, NULL, NULL);
  pmbx->show();

  taskMainThread = 0;

  bool runned = false;

  for (int i = current_thread_index; i < ui.tableWidget->rowCount(); i++)
  {
    int type_id = this->types[current_thread_index];

    CustomCheckTableWidgetItem * cb = dynamic_cast<CustomCheckTableWidgetItem *>(ui.tableWidget->item(i, 0));

    if (cb->isChecked())
    {
      QString batfile_path = tr("%1%2run_kernel_%3.bat").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QString::number(type_id));
      if (fileExists(batfile_path))
      {
        QStringList llist;
        llist.append(this->uids[i].toString());

        unsigned int subsections = 0;
        bool have_before_data = false;
        for (int k = 1; k < ui.tableWidget->columnCount(); k++)
        {
          CustomCellTableWidgetItem * ct = dynamic_cast<CustomCellTableWidgetItem *>(ui.tableWidget->item(i, k));

          QDir dir;

          if (type_id == 3 && k > 1)
          {
            if (ct->activated && dir.exists(ct->path) && (ct->path != QString()) && dir.exists(ct->path_combined) && (ct->path_combined != QString()))
            {
              llist.append(QString::number(ct->period) + ":" + QString::number(ct->period_combined));

              llist.append(QString::number(ct->snapshot_type) + ":" + QString::number(ct->snapshot_type_combined));

              llist.append(ct->path + ":" + ct->path_combined);

              llist.append(QString::number((int)ct->all_snapshots) + ":" + QString::number((int)ct->all_snapshots_combined));

              llist.append(QString::number(ct->first_snapshot_index) + ":" + QString::number(ct->first_snapshot_index_combined));
              llist.append(QString::number(ct->last_snapshot_index) + ":" + QString::number(ct->last_snapshot_index_combined));

              ct->result_index = (int)(subsections - 1U);

              subsections++;
            }
          }
          else
          {
            if (ct->activated && dir.exists(ct->path) && (ct->path != QString()))
            {
              llist.append(QString::number(ct->period));
              llist.append(QString::number(ct->snapshot_type));
              llist.append(ct->path);
              llist.append(QString::number((int)ct->all_snapshots));
              llist.append(QString::number(ct->first_snapshot_index));
              llist.append(QString::number(ct->last_snapshot_index));

              if (type_id == 2 || type_id == 3)
              {
                if (k > 1) ct->result_index = (int)(subsections - 1U);
                else
                {
                  ct->result_index = -1;

                  have_before_data = true;
                }
              }
              else
              {
                ct->result_index = (int)(subsections);
              }

              subsections++;
            }
          }
        }

        if ((type_id == 0 && subsections > 0U) || (type_id == 1 && subsections > 0U) || ((type_id == 2 || type_id == 3) && have_before_data && subsections > 1U))
        {
          // создание рабочей директории
          {
            QString qpath = tr("%1%2results%3%4").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[i].toString());

            QDir results_folder(qpath);
            if (results_folder != QString() && results_folder.exists())
            {
              try
              {
                results_folder.removeRecursively();
              }
              catch (...) {}
            }

            try
            {
              results_folder.mkpath(qpath);
            }
            catch (...) {}

            if (type_id == 3)
            {
              QString qpath_msk = tr("%1%2results%3%4%3msk").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[i].toString());

              QDir results_folder_msk(qpath_msk);
              if (results_folder_msk != QString() && results_folder_msk.exists())
              {
                try
                {
                  results_folder_msk.removeRecursively();
                }
                catch (...) {}
              }

              try
              {
                results_folder_msk.mkpath(qpath_msk);
              }
              catch (...) {}

              QString qpath_ischemia = tr("%1%2results%3%4%3ischemia").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[i].toString());

              QDir results_folder_ischemia(qpath_ischemia);
              if (results_folder_ischemia != QString() && results_folder_ischemia.exists())
              {
                try
                {
                  results_folder_ischemia.removeRecursively();
                }
                catch (...) {}
              }

              try
              {
                results_folder_ischemia.mkpath(qpath_ischemia);
              }
              catch (...) {}
            }
          }

          // запустить счет
          runned = true;

          QDateTime time = QDateTime::currentDateTime();
          current_thread_index++;

          taskMainThread = new Thread(this, batfile_path, llist, QString(), time, current_thread_index);
          taskMainThread->te->dataIsChanged(tr("uid: %1").arg(llist[0]));

          for (int q = 0; q < subsections; q++)
          {
            QStringList local_list;

            if (type_id == 3)
            {
              if (q > 0)
              {
                for (int p = 0; p < number_of_saving_parameters - 8U; p++)
                  local_list.append(llist.at(number_of_saving_parameters - 7U + (q - 1) * (number_of_saving_parameters - 8U) + p));
              }
              else
              {
                for (int p = 0; p < number_of_saving_parameters - 8U; p++)
                  local_list.append(llist.at(1U + p));
              }
            }
            else
            {
              for (int p = 0; p < number_of_saving_parameters - 8U; p++)
                local_list.append(llist.at(1U + q * (number_of_saving_parameters - 8U) + p));
            }

            taskMainThread->te->dataIsChanged(tr("arguments[%1]: %2").arg(QString::number(q), local_list.join(' ')));
          }

          taskMainThread->start();
          tlist.push_back(taskMainThread);

          break;
        }
      }
      else
      {
        qWarning("%s", tr("Can't find file '%1'").arg(batfile_path).toStdString().c_str());
      }
    }

    current_thread_index++;
  }

  if (runned == false)
  {
    // разблокировать кнопки
    ui.AddNewSeries->setEnabled(true);
    ui.SelectAll->setEnabled(true);
    ui.DeselectAll->setEnabled(true);
    ui.DeleteSelected->setEnabled(true);
    ui.RunTasks->setEnabled(true);
    ui.ChangeSettings->setEnabled(true);

    // разблокировать панель
    ui.tableWidget->setEnabled(true);

    // сбросить фокусы на чекбоксах
    for (int i = 0; i < ui.tableWidget->rowCount(); i++)
    {
      CustomCheckTableWidgetItem * cb = dynamic_cast<CustomCheckTableWidgetItem *>(ui.tableWidget->item(i, 0));

      cb->clearFocus();
    }
  }
}

void MainWindow::ChangeSettings()
{}

void MainWindow::AboutProgram()
{
  AboutDialog * dialog = new AboutDialog();
  dialog->exec();
}

void MainWindow::visualizeData(QWidget * ct_obj)
{
  CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)ct_obj;

  dicomViewer * dv = new dicomViewer(this->types[ct->row()], ct, 0, this->uids[ct->row()], 0);
  dv->showNormal();
}

void MainWindow::visualizeData_msk(QWidget * ct_obj)
{
  CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)ct_obj;

  dicomViewer * dv = new dicomViewer(this->types[ct->row()], ct, 0, this->uids[ct->row()], 0);
  dv->showNormal();
}

void MainWindow::visualizeData_ischemia(QWidget * ct_obj)
{
  CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)ct_obj;

  dicomViewer * dv = new dicomViewer(this->types[ct->row()], ct, 0, this->uids[ct->row()], 1);
  dv->showNormal();
}

void MainWindow::visualizeResults(QWidget * ct_obj)
{
  CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)ct_obj;

  dicomViewer * dv = new dicomViewer(this->types[ct->row()], ct, 1, this->uids[ct->row()], 0);
  dv->showNormal();
}

void MainWindow::deleteSeriesInformation(QWidget * ct_obj)
{
  CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)ct_obj;
  ct->activated = 0;

  updateSeriesInformation(ct_obj);
}

void MainWindow::updateSeriesInformation(QWidget * ct_obj)
{
  CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)ct_obj;

  int rowid = ct->row();
  int type = this->types[rowid];

  if (type == 3 && ct->column() > 1)
  {
    QString qpath_msk = tr("%1%2results%3%4%3msk").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[ct->row()].toString());
    QString qpath_ischemia = tr("%1%2results%3%4%3ischemia").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[ct->row()].toString());

    QDir results_folder_msk(qpath_msk);
    if (results_folder_msk != QString() && results_folder_msk.exists())
    {
      try
      {
        results_folder_msk.remove(tr("%1%2result_%3.dcm").arg(qpath_msk, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
        results_folder_msk.remove(tr("%1%2result_brain_%3.dcm").arg(qpath_msk, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
        results_folder_msk.remove(tr("%1%2result_mask_%3.dcm").arg(qpath_msk, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));

        results_folder_msk.remove(tr("%1%2brain_width_%3.txt").arg(qpath_msk, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));

        results_folder_msk.remove(tr("%1%2MSC_percentage_%3.html").arg(qpath_msk, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
      }
      catch (...) {}
    }

    QDir results_folder_ischemia(qpath_ischemia);
    if (results_folder_ischemia != QString() && results_folder_ischemia.exists())
    {
      try
      {
        results_folder_ischemia.remove(tr("%1%2result_%3.dcm").arg(qpath_ischemia, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
        results_folder_ischemia.remove(tr("%1%2result_brain_%3.dcm").arg(qpath_ischemia, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
        results_folder_ischemia.remove(tr("%1%2result_mask_%3.dcm").arg(qpath_ischemia, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));

        results_folder_ischemia.remove(tr("%1%2brain_width_%3.txt").arg(qpath_ischemia, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));

        results_folder_ischemia.remove(tr("%1%2ischemia_percentage_%3.html").arg(qpath_ischemia, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
      }
      catch (...) {}
    }

    ct->result_index = -1;

    updateDataCell(ct);
  }
  else
  {
    if ((type == 2 || type == 3) && ct->column() == 1)
    {
      for (int i = 0; i < datacols_start; i++)
      {
        CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)ui.tableWidget->item(rowid, i + 1);

        ct->result_index = -1;
      }

      QString qpath = tr("%1%2results%3%4").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[ct->row()].toString());
      QDir results_folder(qpath);
      if (results_folder != QString() && results_folder.exists())
      {
        try
        {
          results_folder.removeRecursively();
        }
        catch (...) {}
      }
    }
    else
    {
      QString qpath = tr("%1%2results%3%4").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[ct->row()].toString());
      QDir results_folder(qpath);
      if (results_folder != QString() && results_folder.exists())
      {
        try
        {
          results_folder.remove(tr("%1%2result_%3.dcm").arg(qpath, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
          results_folder.remove(tr("%1%2result_brain_%3.dcm").arg(qpath, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
          results_folder.remove(tr("%1%2result_mask_%3.dcm").arg(qpath, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));

          results_folder.remove(tr("%1%2brain_width_%3.txt").arg(qpath, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));

          if (type == 1) results_folder.remove(tr("%1%2ischemia_percentage_%3.html").arg(qpath, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
          else if (type == 2) results_folder.remove(tr("%1%2MSC_percentage_%3.html").arg(qpath, QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index)));
        }
        catch (...) {}
      }

      ct->result_index = -1;
    }

    updateDataCell(ct);
  }
}

void MainWindow::visualizeIschemiaPercentage(QWidget * ct_obj)
{
  CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)ct_obj;

  IschemiaPercentage * dv = new IschemiaPercentage(ct, this->uids[ct->row()]);

  dv->setWindowTitle(tr(QApplication::translate("MainWindowClass", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\265 \320\264\320\260\320\275\320\275\321\213\320\265 (\321\201\320\265\321\200\320\270\321\217 \321\201\320\275\320\270\320\274\320\272\320\276\320\262 \342\204\226%1)", Q_NULLPTR).toStdString().c_str()).arg(QString::number(ct->column())));

  dv->showNormal();
}

bool IsCheckBox(const QWidget *widget)
{
  return dynamic_cast<const QCheckBox*>(widget) != 0;
}
bool IsComboBox(const QWidget *widget)
{
  return dynamic_cast<const QComboBox*>(widget) != 0;
}

bool IsTextEdit(const QWidget *widget)
{
  return dynamic_cast<const QTextEdit*>(widget) != 0;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::ContextMenu)
  {
    QContextMenuEvent* mevent = static_cast<QContextMenuEvent *>(event);

    QWidget * qobj = qobject_cast<QWidget *>(obj);

    if (qobj != NULL)
    {
      QWidget * qobj_main = qobject_cast<QWidget *>(qobj->parent());

      if (qobj_main != NULL && IsTextEdit(qobj_main))
      {
        CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)qobj_main;

        if (ct->isEnabled())
        {
          QDir data_folder(ct->path);
          if (data_folder != QString() && data_folder.exists())
          {
            data_folder.setNameFilters(QStringList() << "*.IMA" << "*.ima");
            QStringList dataFileList = data_folder.entryList();

            if (dataFileList.size())
            {
              QMenu *menu = new QMenu(this);
              menu->setStyleSheet("QMenu:item {padding: 5 20 5 20;}"
                "QMenu:item:selected {background-color: #64C8FF;}");

              int type_id = this->types[ct->row()];

              // визуализация обрабатываемых данных
              {
                //qWarning("ct->column(): %d", ct->column());

                if (type_id == 3 && ct->column() > 1)
                {
                  QAction * a0_msk = new QAction(QApplication::translate("MainWindowClass", "\320\222\320\270\320\267\321\203\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217 \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205 (\320\234\320\241\320\232)", Q_NULLPTR), ct);

                  QSignalMapper* signalMapper0_msk = new QSignalMapper(this);
                  connect(a0_msk, SIGNAL(triggered()), signalMapper0_msk, SLOT(map()));
                  signalMapper0_msk->setMapping(a0_msk, ct);
                  connect(signalMapper0_msk, SIGNAL(mapped(QWidget *)), this, SLOT(visualizeData_msk(QWidget *)));

                  menu->addAction(a0_msk);

                  QAction * a0_ischemia = new QAction(QApplication::translate("MainWindowClass", "\320\222\320\270\320\267\321\203\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217 \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205 (\320\270\321\210\320\265\320\274\320\270\321\217)", Q_NULLPTR), ct);

                  QSignalMapper* signalMapper0_ischemia = new QSignalMapper(this);
                  connect(a0_ischemia, SIGNAL(triggered()), signalMapper0_ischemia, SLOT(map()));
                  signalMapper0_ischemia->setMapping(a0_ischemia, ct);
                  connect(signalMapper0_ischemia, SIGNAL(mapped(QWidget *)), this, SLOT(visualizeData_ischemia(QWidget *)));

                  menu->addAction(a0_ischemia);
                }
                else
                {
                  QAction * a0 = new QAction(QApplication::translate("MainWindowClass", "\320\222\320\270\320\267\321\203\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217 \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205", Q_NULLPTR), ct);

                  QSignalMapper* signalMapper0 = new QSignalMapper(this);
                  connect(a0, SIGNAL(triggered()), signalMapper0, SLOT(map()));
                  signalMapper0->setMapping(a0, ct);
                  connect(signalMapper0, SIGNAL(mapped(QWidget *)), this, SLOT(visualizeData(QWidget *)));

                  menu->addAction(a0);
                }
              }

              // визуализация результатов обработки
              if ((type_id == 0 || type_id == 1 || ((type_id == 2 || type_id == 3) && ct->column() > 1)) && ct->result_index != -1)
              {
                if (type_id == 3)
                {
                  QDir results_folder_msk(tr("%1%2results%3%4%3msk").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[ct->row()].toString()));
                  QDir results_folder_ischemia(tr("%1%2results%3%4%3ischemia").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[ct->row()].toString()));
                  if (results_folder_msk != QString() && results_folder_msk.exists() && results_folder_ischemia != QString() && results_folder_ischemia.exists())
                  {
                    results_folder_msk.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
                    QStringList resultsFileList_msk = results_folder_msk.entryList();

                    results_folder_ischemia.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
                    QStringList resultsFileList_ischemia = results_folder_msk.entryList();

                    if (resultsFileList_msk.size() && resultsFileList_ischemia.size())
                    {
                      QAction * a1 = new QAction(QApplication::translate("MainWindowClass", "\320\222\320\270\320\267\321\203\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217 \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\320\276\320\262 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\320\270", Q_NULLPTR), ct);

                      QSignalMapper* signalMapper1 = new QSignalMapper(this);
                      connect(a1, SIGNAL(triggered()), signalMapper1, SLOT(map()));
                      signalMapper1->setMapping(a1, ct);
                      connect(signalMapper1, SIGNAL(mapped(QWidget *)), this, SLOT(visualizeResults(QWidget *)));

                      menu->addAction(a1);
                    }
                  }
                }
                else
                {
                  QDir results_folder(tr("%1%2results%3%4").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[ct->row()].toString()));
                  if (results_folder != QString() && results_folder.exists())
                  {
                    results_folder.setNameFilters(QStringList() << "*.DCM" << "*.dcm");
                    QStringList resultsFileList = results_folder.entryList();

                    if (resultsFileList.size())
                    {
                      QAction * a1 = new QAction(QApplication::translate("MainWindowClass", "\320\222\320\270\320\267\321\203\320\260\320\273\320\270\320\267\320\260\321\206\320\270\321\217 \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\320\276\320\262 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\320\270", Q_NULLPTR), ct);

                      QSignalMapper* signalMapper1 = new QSignalMapper(this);
                      connect(a1, SIGNAL(triggered()), signalMapper1, SLOT(map()));
                      signalMapper1->setMapping(a1, ct);
                      connect(signalMapper1, SIGNAL(mapped(QWidget *)), this, SLOT(visualizeResults(QWidget *)));

                      menu->addAction(a1);
                    }

                    if (type_id == 1)
                    {
                      QString percentagePath = tr("%1%2results%3%4%5ischemia_percentage_%6.html").arg(qApp->applicationDirPath(), QDir::fromNativeSeparators(QDir::separator()), QDir::fromNativeSeparators(QDir::separator()), this->uids[ct->row()].toString(), QDir::fromNativeSeparators(QDir::separator()), QString::number(ct->result_index));

                      if (fileExists(percentagePath))
                      {
                        QAction * a2 = new QAction(QApplication::translate("MainWindowClass", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\265 \320\264\320\260\320\275\320\275\321\213\320\265", Q_NULLPTR), ct);

                        QSignalMapper* signalMapper2 = new QSignalMapper(this);
                        connect(a2, SIGNAL(triggered()), signalMapper2, SLOT(map()));
                        signalMapper2->setMapping(a2, ct);
                        connect(signalMapper2, SIGNAL(mapped(QWidget *)), this, SLOT(visualizeIschemiaPercentage(QWidget *)));

                        menu->addAction(a2);
                      }
                    }
                  }
                }
              }

              // Удалить информацию о серии
              {
                QAction * a3 = new QAction(QApplication::translate("MainWindowClass", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\216 \320\276 \321\201\320\265\321\200\320\270\320\270", Q_NULLPTR), ct);

                QSignalMapper* signalMapper3 = new QSignalMapper(this);
                connect(a3, SIGNAL(triggered()), signalMapper3, SLOT(map()));
                signalMapper3->setMapping(a3, ct);
                connect(signalMapper3, SIGNAL(mapped(QWidget *)), this, SLOT(deleteSeriesInformation(QWidget *)));

                menu->addAction(a3);
              }

              if (menu->actions().size())
                menu->popup(ct->viewport()->mapToGlobal(mevent->pos()));
              else
                delete menu;
            }
            else
            {
              if (ct->isEnabled() && ct->activated)
              {
                QMenu *menu = new QMenu(this);
                menu->setStyleSheet("QMenu:item {padding: 5 20 5 20;}"
                  "QMenu:item:selected {background-color: #64C8FF;}");

                // Удалить информацию о серии
                {
                  QAction * a3 = new QAction(QApplication::translate("MainWindowClass", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\216 \320\276 \321\201\320\265\321\200\320\270\320\270", Q_NULLPTR), ct);

                  QSignalMapper* signalMapper3 = new QSignalMapper(this);
                  connect(a3, SIGNAL(triggered()), signalMapper3, SLOT(map()));
                  signalMapper3->setMapping(a3, ct);
                  connect(signalMapper3, SIGNAL(mapped(QWidget *)), this, SLOT(deleteSeriesInformation(QWidget *)));

                  menu->addAction(a3);
                }

                menu->popup(ct->viewport()->mapToGlobal(mevent->pos()));
              }
              else
              {
                event->ignore();

                return true;
              }
            }
          }
          else
          {
            if (ct->isEnabled() && ct->activated)
            {
              QMenu *menu = new QMenu(this);
              menu->setStyleSheet("QMenu:item {padding: 5 20 5 20;}"
                "QMenu:item:selected {background-color: #64C8FF;}");

              // Удалить информацию о серии
              {
                QAction * a3 = new QAction(QApplication::translate("MainWindowClass", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\216 \320\276 \321\201\320\265\321\200\320\270\320\270", Q_NULLPTR), ct);

                QSignalMapper* signalMapper3 = new QSignalMapper(this);
                connect(a3, SIGNAL(triggered()), signalMapper3, SLOT(map()));
                signalMapper3->setMapping(a3, ct);
                connect(signalMapper3, SIGNAL(mapped(QWidget *)), this, SLOT(deleteSeriesInformation(QWidget *)));

                menu->addAction(a3);
              }

              menu->popup(ct->viewport()->mapToGlobal(mevent->pos()));
            }
            else
            {
              event->ignore();

              return true;
            }
          }
        }
        else
        {
          event->ignore();

          return true;
        }
      }
      else
      {
        event->ignore();

        return true;
      }
    }
    else
    {
      event->ignore();

      return true;
    }
  }
  else if (event->type() == QMouseEvent::MouseButtonDblClick)
  {
    QMouseEvent * key = static_cast<QMouseEvent *>(event);

    if (key->buttons() & Qt::LeftButton || key->button() == Qt::LeftButton)
    {
      QWidget * qobj = qobject_cast<QWidget *>(obj);

      if (qobj != NULL)
      {
        QWidget * qobj_main = qobject_cast<QWidget *>(qobj->parent());

        if (qobj_main != NULL && IsTextEdit(qobj_main))
        {
          CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)qobj_main;

          if (ct->isEnabled())
          {
            int type = this->types[ct->row()];

            QString base_styleSheet = ct->styleSheet();
            ct->setStyleSheet(base_styleSheet + "QWidget{background-color: #C8C8C8}");

            if (type == 3 && ct->column() > 1)
            {
              NewSeriesFormCombined * nsf = new NewSeriesFormCombined();

              if (ct->activated)
              {
                nsf->ui.mskPeriodSpinBox->setValue(ct->period);
                nsf->ui.ischemiaPeriodSpinBox->setValue(ct->period_combined);
              }
              else
              {
                ct->snapshot_type = 0;

                ct->snapshot_type_combined = 1;

                nsf->ui.mskPeriodSpinBox->setValue(0);
                nsf->ui.ischemiaPeriodSpinBox->setValue(0);
              }

              nsf->ui.mskSnaphotTypeComboBox->setCurrentIndex(ct->snapshot_type);
              nsf->ui.mskPlainTextEdit->setPlainText(ct->path);
              nsf->ui.mskSnapshotSelectorCheckBox->setChecked(ct->all_snapshots);

              nsf->ui.ischemiaSnaphotTypeComboBox->setCurrentIndex(ct->snapshot_type_combined);
              nsf->ui.ischemiaPlainTextEdit->setPlainText(ct->path_combined);
              nsf->ui.ischemiaSnapshotSelectorCheckBox->setChecked(ct->all_snapshots_combined);

              if (!ct->all_snapshots)
              {
                // проверить наличие директории
                {
                  QDir dir;
                  if (!dir.exists(ct->path) || (ct->path == QString()))
                  {
                    nsf->ui.mskSnapshotSelectorMinSpinBox->setMinimum(ct->first_snapshot_index);
                    nsf->ui.mskSnapshotSelectorMinSpinBox->setMaximum(ct->last_snapshot_index);

                    nsf->ui.mskSnapshotSelectorMaxSpinBox->setMinimum(ct->first_snapshot_index);
                    nsf->ui.mskSnapshotSelectorMaxSpinBox->setMaximum(ct->last_snapshot_index);

                    nsf->ui.mskSnapshotSelectorCheckBox->setEnabled(false);
                    nsf->ui.mskSnapshotSelectorMinSpinBox->setEnabled(false);
                    nsf->ui.mskSnapshotSelectorMaxSpinBox->setEnabled(false);
                  }
                }

                if (ct->first_snapshot_index == -1 && ct->last_snapshot_index == -1)
                {
                  nsf->ui.mskSnapshotSelectorCheckBox->setChecked(true);
                }
                else
                {
                  nsf->ui.mskSnapshotSelectorMinSpinBox->setValue(ct->first_snapshot_index);
                  nsf->ui.mskSnapshotSelectorMaxSpinBox->setValue(ct->last_snapshot_index);
                }
              }

              if (!ct->all_snapshots_combined)
              {
                // проверить наличие директории
                {
                  QDir dir;
                  if (!dir.exists(ct->path_combined) || (ct->path_combined == QString()))
                  {
                    nsf->ui.ischemiaSnapshotSelectorMinSpinBox->setMinimum(ct->first_snapshot_index_combined);
                    nsf->ui.ischemiaSnapshotSelectorMinSpinBox->setMaximum(ct->last_snapshot_index_combined);

                    nsf->ui.ischemiaSnapshotSelectorMaxSpinBox->setMinimum(ct->first_snapshot_index_combined);
                    nsf->ui.ischemiaSnapshotSelectorMaxSpinBox->setMaximum(ct->last_snapshot_index_combined);

                    nsf->ui.ischemiaSnapshotSelectorCheckBox->setEnabled(false);
                    nsf->ui.ischemiaSnapshotSelectorMinSpinBox->setEnabled(false);
                    nsf->ui.ischemiaSnapshotSelectorMaxSpinBox->setEnabled(false);
                  }
                }

                if (ct->first_snapshot_index_combined == -1 && ct->last_snapshot_index_combined == -1)
                {
                  nsf->ui.ischemiaSnapshotSelectorCheckBox->setChecked(true);
                }
                else
                {
                  nsf->ui.ischemiaSnapshotSelectorMinSpinBox->setValue(ct->first_snapshot_index_combined);
                  nsf->ui.ischemiaSnapshotSelectorMaxSpinBox->setValue(ct->last_snapshot_index_combined);
                }
              }

              nsf->showNormal();

              QEventLoop loop;
              connect(nsf, SIGNAL(updateSignal()), &loop, SLOT(quit()));
              loop.exec();

              ct->setStyleSheet(base_styleSheet);

              qWarning("nsf->changed: %d", nsf->changed);

              if (nsf->changed == 1)
              {
                ct->activated = 1;
                ct->period = nsf->ui.mskPeriodSpinBox->value();
                ct->period_combined = nsf->ui.ischemiaPeriodSpinBox->value();

                ct->snapshot_type = nsf->ui.mskSnaphotTypeComboBox->currentIndex();
                ct->snapshot_type_combined = nsf->ui.ischemiaSnaphotTypeComboBox->currentIndex();

                ct->path = nsf->ui.mskPlainTextEdit->toPlainText();
                ct->path_combined = nsf->ui.ischemiaPlainTextEdit->toPlainText();

                ct->all_snapshots = nsf->ui.mskSnapshotSelectorCheckBox->isChecked();
                ct->all_snapshots_combined = nsf->ui.ischemiaSnapshotSelectorCheckBox->isChecked();

                ct->first_snapshot_index = nsf->ui.mskSnapshotSelectorMinSpinBox->value();
                ct->last_snapshot_index = nsf->ui.mskSnapshotSelectorMaxSpinBox->value();

                ct->first_snapshot_index_combined = nsf->ui.ischemiaSnapshotSelectorMinSpinBox->value();
                ct->last_snapshot_index_combined = nsf->ui.ischemiaSnapshotSelectorMaxSpinBox->value();

                updateSeriesInformation(ct);
              }
            }
            else
            {
              NewSeriesForm * nsf = new NewSeriesForm(type);

              switch (type)
              {
              case 0:
              {
                if (!ct->activated) ct->snapshot_type = 1;

                if (ct->activated)
                  nsf->ui.periodSpinBox->setValue(ct->period);
                else
                  nsf->ui.periodSpinBox->setValue(-1);

                nsf->ui.periodSpinBox->setEnabled(false);

                nsf->ui.periodSpinBox->setVisible(false);
                nsf->ui.daysLabel->setVisible(false);

                nsf->setMinimumHeight(nsf->minimumHeight() - 25);
                nsf->setMaximumHeight(nsf->maximumHeight() - 25);

                break;
              }
              case 1:
              {
                if (!ct->activated) ct->snapshot_type = 1;

                if (ct->activated)
                  nsf->ui.periodSpinBox->setValue(ct->period);
                else
                  nsf->ui.periodSpinBox->setValue(0);

                break;
              }
              case 2:
              case 3:
              {
                if (!ct->activated) ct->snapshot_type = 0;

                if (ct->activated)
                  nsf->ui.periodSpinBox->setValue(ct->period);
                else
                  nsf->ui.periodSpinBox->setValue(-1);

                if (ct->column() == 1) nsf->ui.periodSpinBox->setEnabled(false);
                else if (ct->column() > 1) nsf->ui.periodSpinBox->setMinimum(0);

                break;
              }

              default:
              {
                break;
              }
              }

              nsf->ui.snaphotTypeComboBox->setCurrentIndex(ct->snapshot_type);
              nsf->ui.plainTextEdit->setPlainText(ct->path);
              nsf->ui.snapshotSelectorCheckBox->setChecked(ct->all_snapshots);

              if (!ct->all_snapshots)
              {
                // проверить наличие директории
                {
                  QDir dir;
                  if (!dir.exists(ct->path) || (ct->path == QString()))
                  {
                    nsf->ui.snapshotSelectorMinSpinBox->setMinimum(ct->first_snapshot_index);
                    nsf->ui.snapshotSelectorMinSpinBox->setMaximum(ct->last_snapshot_index);

                    nsf->ui.snapshotSelectorMaxSpinBox->setMinimum(ct->first_snapshot_index);
                    nsf->ui.snapshotSelectorMaxSpinBox->setMaximum(ct->last_snapshot_index);

                    nsf->ui.snapshotSelectorCheckBox->setEnabled(false);
                    nsf->ui.snapshotSelectorMinSpinBox->setEnabled(false);
                    nsf->ui.snapshotSelectorMaxSpinBox->setEnabled(false);
                  }
                }

                if (ct->first_snapshot_index == -1 && ct->last_snapshot_index == -1)
                {
                  nsf->ui.snapshotSelectorCheckBox->setChecked(true);
                }
                else
                {
                  nsf->ui.snapshotSelectorMinSpinBox->setValue(ct->first_snapshot_index);
                  nsf->ui.snapshotSelectorMaxSpinBox->setValue(ct->last_snapshot_index);
                }
              }

              nsf->showNormal();

              QEventLoop loop;
              connect(nsf, SIGNAL(updateSignal()), &loop, SLOT(quit()));
              loop.exec();

              ct->setStyleSheet(base_styleSheet);

              if (nsf->changed == 1)
              {
                ct->activated = 1;
                ct->period = nsf->ui.periodSpinBox->value();
                ct->snapshot_type = nsf->ui.snaphotTypeComboBox->currentIndex();
                ct->path = nsf->ui.plainTextEdit->toPlainText();
                ct->all_snapshots = nsf->ui.snapshotSelectorCheckBox->isChecked();
                ct->first_snapshot_index = nsf->ui.snapshotSelectorMinSpinBox->value();
                ct->last_snapshot_index = nsf->ui.snapshotSelectorMaxSpinBox->value();

                updateSeriesInformation(ct);
              }
            }
          }
          else
          {
            event->ignore();

            return true;
          }
        }
        else
        {
          event->ignore();

          return true;
        }
      }
      else
      {
        event->ignore();

        return true;
      }
    }
    else
    {
      event->ignore();

      return true;
    }
  }
  else if (event->type() == QMouseEvent::MouseButtonPress)
  {
    event->ignore();

    return true;
  }

  return QWidget::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (taskMainThread)
  {
    if (event)
      event->ignore();
  }
  else
  {
    int datarows_end = ui.tableWidget->rowCount();
    int datacols_end = ui.tableWidget->columnCount() - 1;

    QSettings * app_settings = new QSettings(settingsPath() + appBaseName() + ".ini", QSettings::IniFormat);
    app_settings->setValue("datarows", datarows_end);
    app_settings->setValue("datacols", datacols_end);

    QList<QStringList> data;

    for (int i = datarows_end - 1; i > -1; i--)
    {
      QStringList llist;
      llist.append(uids[i].toString());
      llist.append(QString::number(types[i]));

      for (int j = 0; j < datacols_end; j++)
      {
        CustomCellTableWidgetItem * ct = (CustomCellTableWidgetItem *)ui.tableWidget->item(i, j + 1);

        llist.append(QString::number(ct->activated));

        llist.append(QString::number(ct->period));
        llist.append(QString::number(ct->period_combined));

        llist.append(QString::number(ct->snapshot_type));
        llist.append(QString::number(ct->snapshot_type_combined));

        llist.append(ct->path);
        llist.append(ct->path_combined);

        llist.append(ct->all_snapshots ? "true" : "false");
        llist.append(ct->all_snapshots_combined ? "true" : "false");

        llist.append(QString::number(ct->first_snapshot_index));
        llist.append(QString::number(ct->last_snapshot_index));

        llist.append(QString::number(ct->first_snapshot_index_combined));
        llist.append(QString::number(ct->last_snapshot_index_combined));

        llist.append(QString::number(ct->result_index));
      }

      data.append(llist);
    }

    app_settings->setValue("data", QVariant::fromValue<QList<QStringList>>(data));

    delete app_settings;

    foreach(Thread * t, tlist)
    {
      if (t)
        delete t;
    }
    tlist.clear();

    if (event)
      event->accept();
  }
}
