/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *AddNewSeries;
    QAction *SelectAll;
    QAction *DeselectAll;
    QAction *DeleteSelected;
    QAction *ChangeSettings;
    QAction *RunTasks;
    QAction *AboutProgram;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(1385, 668);
        MainWindowClass->setMinimumSize(QSize(1385, 668));
        QFont font;
        font.setPointSize(9);
        MainWindowClass->setFont(font);
        MainWindowClass->setAutoFillBackground(false);
        AddNewSeries = new QAction(MainWindowClass);
        AddNewSeries->setObjectName(QString::fromUtf8("AddNewSeries"));
        SelectAll = new QAction(MainWindowClass);
        SelectAll->setObjectName(QString::fromUtf8("SelectAll"));
        DeselectAll = new QAction(MainWindowClass);
        DeselectAll->setObjectName(QString::fromUtf8("DeselectAll"));
        DeleteSelected = new QAction(MainWindowClass);
        DeleteSelected->setObjectName(QString::fromUtf8("DeleteSelected"));
        ChangeSettings = new QAction(MainWindowClass);
        ChangeSettings->setObjectName(QString::fromUtf8("ChangeSettings"));
        RunTasks = new QAction(MainWindowClass);
        RunTasks->setObjectName(QString::fromUtf8("RunTasks"));
        AboutProgram = new QAction(MainWindowClass);
        AboutProgram->setObjectName(QString::fromUtf8("AboutProgram"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        tableWidget = new QTableWidget(centralWidget);
        if (tableWidget->columnCount() < 1)
            tableWidget->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignCenter);
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setMinimumSize(QSize(100, 100));
        tableWidget->setMaximumSize(QSize(16777215, 16777215));
        tableWidget->setLayoutDirection(Qt::LeftToRight);
        tableWidget->setAutoFillBackground(false);
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        tableWidget->setAlternatingRowColors(false);
        tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidget->setTextElideMode(Qt::ElideRight);
        tableWidget->setCornerButtonEnabled(false);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(1);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setDefaultSectionSize(130);
        tableWidget->horizontalHeader()->setHighlightSections(false);
        tableWidget->horizontalHeader()->setStretchLastSection(false);
        tableWidget->verticalHeader()->setVisible(true);
        tableWidget->verticalHeader()->setMinimumSectionSize(23);
        tableWidget->verticalHeader()->setHighlightSections(false);
        tableWidget->verticalHeader()->setStretchLastSection(false);

        horizontalLayout->addWidget(tableWidget);


        horizontalLayout_2->addLayout(horizontalLayout);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1385, 36));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setMinimumSize(QSize(1370, 0));
        QFont font1;
        font1.setPointSize(12);
        mainToolBar->setFont(font1);
        mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
        mainToolBar->setAutoFillBackground(false);
        mainToolBar->setMovable(false);
        mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        mainToolBar->setFloatable(false);
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        mainToolBar->addAction(AddNewSeries);
        mainToolBar->addAction(SelectAll);
        mainToolBar->addAction(DeselectAll);
        mainToolBar->addAction(DeleteSelected);
        mainToolBar->addAction(RunTasks);
        mainToolBar->addAction(ChangeSettings);
        mainToolBar->addAction(AboutProgram);
        mainToolBar->addSeparator();

        retranslateUi(MainWindowClass);
        QObject::connect(AddNewSeries, SIGNAL(triggered()), MainWindowClass, SLOT(AddNewSeries()));
        QObject::connect(SelectAll, SIGNAL(triggered()), MainWindowClass, SLOT(SelectAll()));
        QObject::connect(DeselectAll, SIGNAL(triggered()), MainWindowClass, SLOT(DeselectAll()));
        QObject::connect(DeleteSelected, SIGNAL(triggered()), MainWindowClass, SLOT(DeleteSelected()));
        QObject::connect(RunTasks, SIGNAL(triggered()), MainWindowClass, SLOT(RunTasks()));
        QObject::connect(ChangeSettings, SIGNAL(triggered()), MainWindowClass, SLOT(ChangeSettings()));
        QObject::connect(AboutProgram, SIGNAL(triggered()), MainWindowClass, SLOT(AboutProgram()));

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "\320\223\321\200\320\260\321\204\320\270\321\207\320\265\321\201\320\272\320\270\320\271 \320\270\320\275\321\202\320\265\321\200\321\204\320\265\320\271\321\201 \320\262\321\200\320\260\321\207\320\260-\320\270\321\201\321\201\320\273\320\265\320\264\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
        AddNewSeries->setText(QApplication::translate("MainWindowClass", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\275\320\276\320\262\320\276\320\265 \320\270\321\201\321\201\320\273\320\265\320\264\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
#ifndef QT_NO_TOOLTIP
        AddNewSeries->setToolTip(QApplication::translate("MainWindowClass", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\275\320\276\320\262\320\276\320\265 \320\270\321\201\321\201\320\273\320\265\320\264\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
#endif // QT_NO_TOOLTIP
        SelectAll->setText(QApplication::translate("MainWindowClass", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \320\262\321\201\320\265", nullptr));
#ifndef QT_NO_TOOLTIP
        SelectAll->setToolTip(QApplication::translate("MainWindowClass", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \320\262\321\201\320\265", nullptr));
#endif // QT_NO_TOOLTIP
        DeselectAll->setText(QApplication::translate("MainWindowClass", "\320\243\320\261\321\200\320\260\321\202\321\214 \320\262\321\213\320\264\320\265\320\273\320\265\320\275\320\270\320\265", nullptr));
#ifndef QT_NO_TOOLTIP
        DeselectAll->setToolTip(QApplication::translate("MainWindowClass", "\320\243\320\261\321\200\320\260\321\202\321\214 \320\262\321\213\320\264\320\265\320\273\320\265\320\275\320\270\320\265", nullptr));
#endif // QT_NO_TOOLTIP
        DeleteSelected->setText(QApplication::translate("MainWindowClass", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\262\321\213\320\261\321\200\320\260\320\275\320\275\320\276\320\265", nullptr));
#ifndef QT_NO_TOOLTIP
        DeleteSelected->setToolTip(QApplication::translate("MainWindowClass", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\262\321\213\320\261\321\200\320\260\320\275\320\275\320\276\320\265", nullptr));
#endif // QT_NO_TOOLTIP
        ChangeSettings->setText(QApplication::translate("MainWindowClass", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
#ifndef QT_NO_TOOLTIP
        ChangeSettings->setToolTip(QApplication::translate("MainWindowClass", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214 \320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
#endif // QT_NO_TOOLTIP
        RunTasks->setText(QApplication::translate("MainWindowClass", "\320\227\320\260\320\277\321\203\321\201\321\202\320\270\321\202\321\214 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\321\203", nullptr));
#ifndef QT_NO_TOOLTIP
        RunTasks->setToolTip(QApplication::translate("MainWindowClass", "\320\227\320\260\320\277\321\203\321\201\321\202\320\270\321\202\321\214 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\321\203", nullptr));
#endif // QT_NO_TOOLTIP
        AboutProgram->setText(QApplication::translate("MainWindowClass", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
#ifndef QT_NO_TOOLTIP
        AboutProgram->setToolTip(QApplication::translate("MainWindowClass", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
#endif // QT_NO_TOOLTIP
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindowClass", "\320\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
