/********************************************************************************
** Form generated from reading UI file 'selecttypedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTTYPEDIALOG_H
#define UI_SELECTTYPEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InvestigationTypeSelector
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *vboxLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpacerItem *spacerItem;
    QRadioButton *type0RadioButton;
    QRadioButton *type1RadioButton;
    QRadioButton *type2RadioButton;
    QRadioButton *type3RadioButton;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *InvestigationTypeSelector)
    {
        if (InvestigationTypeSelector->objectName().isEmpty())
            InvestigationTypeSelector->setObjectName(QString::fromUtf8("InvestigationTypeSelector"));
        InvestigationTypeSelector->setWindowModality(Qt::ApplicationModal);
        InvestigationTypeSelector->resize(500, 140);
        InvestigationTypeSelector->setMinimumSize(QSize(500, 140));
        InvestigationTypeSelector->setMaximumSize(QSize(500, 140));
        QFont font;
        font.setPointSize(12);
        InvestigationTypeSelector->setFont(font);
        layoutWidget = new QWidget(InvestigationTypeSelector);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(349, 70, 141, 71));
        vboxLayout = new QVBoxLayout(layoutWidget);
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        vboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        vboxLayout->addWidget(cancelButton);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);

        type0RadioButton = new QRadioButton(InvestigationTypeSelector);
        buttonGroup = new QButtonGroup(InvestigationTypeSelector);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(type0RadioButton);
        type0RadioButton->setObjectName(QString::fromUtf8("type0RadioButton"));
        type0RadioButton->setGeometry(QRect(10, 10, 331, 17));
        type0RadioButton->setChecked(true);
        type1RadioButton = new QRadioButton(InvestigationTypeSelector);
        buttonGroup->addButton(type1RadioButton);
        type1RadioButton->setObjectName(QString::fromUtf8("type1RadioButton"));
        type1RadioButton->setGeometry(QRect(10, 30, 331, 17));
        type2RadioButton = new QRadioButton(InvestigationTypeSelector);
        buttonGroup->addButton(type2RadioButton);
        type2RadioButton->setObjectName(QString::fromUtf8("type2RadioButton"));
        type2RadioButton->setGeometry(QRect(10, 50, 341, 17));
        type3RadioButton = new QRadioButton(InvestigationTypeSelector);
        buttonGroup->addButton(type3RadioButton);
        type3RadioButton->setObjectName(QString::fromUtf8("type3RadioButton"));
        type3RadioButton->setGeometry(QRect(10, 70, 331, 17));

        retranslateUi(InvestigationTypeSelector);
        QObject::connect(okButton, SIGNAL(clicked()), InvestigationTypeSelector, SLOT(acceptEvent()));
        QObject::connect(cancelButton, SIGNAL(clicked()), InvestigationTypeSelector, SLOT(rejectEvent()));

        QMetaObject::connectSlotsByName(InvestigationTypeSelector);
    } // setupUi

    void retranslateUi(QWidget *InvestigationTypeSelector)
    {
        InvestigationTypeSelector->setWindowTitle(QApplication::translate("InvestigationTypeSelector", "\320\222\321\213\320\261\320\276\321\200 \321\202\320\270\320\277\320\260 \320\270\321\201\321\201\320\273\320\265\320\264\320\276\320\262\320\260\320\275\320\270\321\217", nullptr));
        okButton->setText(QApplication::translate("InvestigationTypeSelector", "OK", nullptr));
        cancelButton->setText(QApplication::translate("InvestigationTypeSelector", "Cancel", nullptr));
        type0RadioButton->setText(QApplication::translate("InvestigationTypeSelector", "\320\222\321\213\320\264\320\265\320\273\320\265\320\275\320\270\320\265 \320\276\320\261\320\273\320\260\321\201\321\202\320\270 \321\201 \320\274\320\276\320\267\320\263\320\276\320\274", nullptr));
        type1RadioButton->setText(QApplication::translate("InvestigationTypeSelector", "\320\237\320\276\320\270\321\201\320\272 \320\270\321\210\320\265\320\274\320\270\321\207\320\265\321\201\320\272\320\270\321\205 \320\277\320\276\321\200\320\260\320\266\320\265\320\275\320\270\320\271", nullptr));
        type2RadioButton->setText(QApplication::translate("InvestigationTypeSelector", "\320\237\320\276\320\270\321\201\320\272 \320\274\320\265\320\267\320\265\320\275\321\205\320\270\320\274\320\260\320\273\321\214\320\275\321\213\321\205 \321\201\321\202\320\262\320\276\320\273\320\276\320\262\321\213\321\205 \320\272\320\273\320\265\321\202\320\276\320\272", nullptr));
        type3RadioButton->setText(QApplication::translate("InvestigationTypeSelector", "\320\241\320\274\320\265\321\210\320\260\320\275\320\275\321\213\320\271 \321\200\320\265\320\266\320\270\320\274 (\320\270\321\210\320\265\320\274\320\270\321\217 + \320\234\320\241\320\232)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InvestigationTypeSelector: public Ui_InvestigationTypeSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTTYPEDIALOG_H
