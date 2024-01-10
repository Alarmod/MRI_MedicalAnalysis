/********************************************************************************
** Form generated from reading UI file 'NewSeriesForm.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWSERIESFORM_H
#define UI_NEWSERIESFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewSeriesForm
{
public:
    QGridLayout *gridLayout;
    QFormLayout *formLayout;
    QLabel *daysLabel;
    QSpinBox *periodSpinBox;
    QHBoxLayout *horizontalLayout;
    QLabel *pathLabel;
    QPlainTextEdit *plainTextEdit;
    QPushButton *selectPathButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *snapshotSelectorLabel;
    QCheckBox *snapshotSelectorCheckBox;
    QSpinBox *snapshotSelectorMinSpinBox;
    QLabel *defLabel;
    QSpinBox *snapshotSelectorMaxSpinBox;
    QSpacerItem *verticalSpacer;
    QLabel *snaphotTypeLabel;
    QComboBox *snaphotTypeComboBox;
    QPushButton *saveChangesButton;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *NewSeriesForm)
    {
        if (NewSeriesForm->objectName().isEmpty())
            NewSeriesForm->setObjectName(QString::fromUtf8("NewSeriesForm"));
        NewSeriesForm->setWindowModality(Qt::ApplicationModal);
        NewSeriesForm->resize(760, 187);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NewSeriesForm->sizePolicy().hasHeightForWidth());
        NewSeriesForm->setSizePolicy(sizePolicy);
        NewSeriesForm->setMinimumSize(QSize(760, 187));
        NewSeriesForm->setMaximumSize(QSize(750, 187));
        gridLayout = new QGridLayout(NewSeriesForm);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setSizeConstraint(QLayout::SetMaximumSize);
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        daysLabel = new QLabel(NewSeriesForm);
        daysLabel->setObjectName(QString::fromUtf8("daysLabel"));
        daysLabel->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, daysLabel);

        periodSpinBox = new QSpinBox(NewSeriesForm);
        periodSpinBox->setObjectName(QString::fromUtf8("periodSpinBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(periodSpinBox->sizePolicy().hasHeightForWidth());
        periodSpinBox->setSizePolicy(sizePolicy1);
        periodSpinBox->setMinimumSize(QSize(50, 0));
        periodSpinBox->setMaximumSize(QSize(50, 16777215));
        periodSpinBox->setAlignment(Qt::AlignCenter);
        periodSpinBox->setMinimum(-1);
        periodSpinBox->setMaximum(100);
        periodSpinBox->setValue(-1);

        formLayout->setWidget(0, QFormLayout::FieldRole, periodSpinBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pathLabel = new QLabel(NewSeriesForm);
        pathLabel->setObjectName(QString::fromUtf8("pathLabel"));

        horizontalLayout->addWidget(pathLabel);

        plainTextEdit = new QPlainTextEdit(NewSeriesForm);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setEnabled(false);
        sizePolicy.setHeightForWidth(plainTextEdit->sizePolicy().hasHeightForWidth());
        plainTextEdit->setSizePolicy(sizePolicy);
        plainTextEdit->setMinimumSize(QSize(220, 0));
        plainTextEdit->setMaximumSize(QSize(16777215, 22));
        QFont font;
        font.setItalic(false);
        plainTextEdit->setFont(font);
        plainTextEdit->setInputMethodHints(Qt::ImhNone);
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        plainTextEdit->setReadOnly(true);
        plainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        plainTextEdit->setMaximumBlockCount(1);

        horizontalLayout->addWidget(plainTextEdit);


        formLayout->setLayout(2, QFormLayout::LabelRole, horizontalLayout);

        selectPathButton = new QPushButton(NewSeriesForm);
        selectPathButton->setObjectName(QString::fromUtf8("selectPathButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(selectPathButton->sizePolicy().hasHeightForWidth());
        selectPathButton->setSizePolicy(sizePolicy2);
        selectPathButton->setMinimumSize(QSize(50, 0));
        selectPathButton->setMaximumSize(QSize(50, 16777215));

        formLayout->setWidget(2, QFormLayout::FieldRole, selectPathButton);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        snapshotSelectorLabel = new QLabel(NewSeriesForm);
        snapshotSelectorLabel->setObjectName(QString::fromUtf8("snapshotSelectorLabel"));

        horizontalLayout_2->addWidget(snapshotSelectorLabel);

        snapshotSelectorCheckBox = new QCheckBox(NewSeriesForm);
        snapshotSelectorCheckBox->setObjectName(QString::fromUtf8("snapshotSelectorCheckBox"));
        snapshotSelectorCheckBox->setChecked(true);

        horizontalLayout_2->addWidget(snapshotSelectorCheckBox);

        snapshotSelectorMinSpinBox = new QSpinBox(NewSeriesForm);
        snapshotSelectorMinSpinBox->setObjectName(QString::fromUtf8("snapshotSelectorMinSpinBox"));
        snapshotSelectorMinSpinBox->setEnabled(false);
        sizePolicy2.setHeightForWidth(snapshotSelectorMinSpinBox->sizePolicy().hasHeightForWidth());
        snapshotSelectorMinSpinBox->setSizePolicy(sizePolicy2);
        snapshotSelectorMinSpinBox->setMinimumSize(QSize(50, 0));
        snapshotSelectorMinSpinBox->setMaximumSize(QSize(50, 16777215));
        snapshotSelectorMinSpinBox->setFont(font);
        snapshotSelectorMinSpinBox->setAlignment(Qt::AlignCenter);
        snapshotSelectorMinSpinBox->setMinimum(-1);
        snapshotSelectorMinSpinBox->setMaximum(-1);
        snapshotSelectorMinSpinBox->setValue(-1);

        horizontalLayout_2->addWidget(snapshotSelectorMinSpinBox);

        defLabel = new QLabel(NewSeriesForm);
        defLabel->setObjectName(QString::fromUtf8("defLabel"));

        horizontalLayout_2->addWidget(defLabel);


        formLayout->setLayout(3, QFormLayout::LabelRole, horizontalLayout_2);

        snapshotSelectorMaxSpinBox = new QSpinBox(NewSeriesForm);
        snapshotSelectorMaxSpinBox->setObjectName(QString::fromUtf8("snapshotSelectorMaxSpinBox"));
        snapshotSelectorMaxSpinBox->setEnabled(false);
        sizePolicy2.setHeightForWidth(snapshotSelectorMaxSpinBox->sizePolicy().hasHeightForWidth());
        snapshotSelectorMaxSpinBox->setSizePolicy(sizePolicy2);
        snapshotSelectorMaxSpinBox->setMinimumSize(QSize(50, 0));
        snapshotSelectorMaxSpinBox->setMaximumSize(QSize(50, 16777215));
        snapshotSelectorMaxSpinBox->setFont(font);
        snapshotSelectorMaxSpinBox->setAlignment(Qt::AlignCenter);
        snapshotSelectorMaxSpinBox->setMinimum(-1);
        snapshotSelectorMaxSpinBox->setMaximum(-1);
        snapshotSelectorMaxSpinBox->setValue(-1);

        formLayout->setWidget(3, QFormLayout::FieldRole, snapshotSelectorMaxSpinBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(6, QFormLayout::LabelRole, verticalSpacer);

        snaphotTypeLabel = new QLabel(NewSeriesForm);
        snaphotTypeLabel->setObjectName(QString::fromUtf8("snaphotTypeLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, snaphotTypeLabel);

        snaphotTypeComboBox = new QComboBox(NewSeriesForm);
        snaphotTypeComboBox->addItem(QString());
        snaphotTypeComboBox->addItem(QString());
        snaphotTypeComboBox->addItem(QString());
        snaphotTypeComboBox->setObjectName(QString::fromUtf8("snaphotTypeComboBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, snaphotTypeComboBox);

        saveChangesButton = new QPushButton(NewSeriesForm);
        saveChangesButton->setObjectName(QString::fromUtf8("saveChangesButton"));
        saveChangesButton->setFont(font);

        formLayout->setWidget(5, QFormLayout::FieldRole, saveChangesButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(4, QFormLayout::FieldRole, verticalSpacer_2);


        gridLayout->addLayout(formLayout, 0, 0, 1, 1);


        retranslateUi(NewSeriesForm);
        QObject::connect(saveChangesButton, SIGNAL(clicked()), NewSeriesForm, SLOT(saveChangesAndClose()));
        QObject::connect(snapshotSelectorCheckBox, SIGNAL(toggled(bool)), NewSeriesForm, SLOT(snapshotSelectorCheckBoxStatusChanged()));
        QObject::connect(selectPathButton, SIGNAL(clicked()), NewSeriesForm, SLOT(selectPath()));
        QObject::connect(plainTextEdit, SIGNAL(textChanged()), NewSeriesForm, SLOT(pathUpdated()));

        snaphotTypeComboBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(NewSeriesForm);
    } // setupUi

    void retranslateUi(QWidget *NewSeriesForm)
    {
        NewSeriesForm->setWindowTitle(QApplication::translate("NewSeriesForm", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \321\201\320\265\321\200\320\270\320\265\320\271 \321\201\320\275\320\270\320\274\320\272\320\276\320\262", nullptr));
        daysLabel->setText(QString());
        pathLabel->setText(QApplication::translate("NewSeriesForm", "\320\237\321\203\321\202\321\214 \320\264\320\276 \321\201\320\265\321\200\320\270\320\270 \321\201\320\275\320\270\320\274\320\272\320\276\320\262:                       ", nullptr));
        selectPathButton->setText(QApplication::translate("NewSeriesForm", "...", nullptr));
        snapshotSelectorLabel->setText(QApplication::translate("NewSeriesForm", "\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275 \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \321\201\321\200\320\265\320\267\320\276\320\262:                        ", nullptr));
        snapshotSelectorCheckBox->setText(QApplication::translate("NewSeriesForm", "\320\222\321\201\320\265 \321\201\321\200\320\265\320\267\321\213?", nullptr));
        defLabel->setText(QApplication::translate("NewSeriesForm", "-", nullptr));
        snaphotTypeLabel->setText(QApplication::translate("NewSeriesForm", "\320\242\320\270\320\277 \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: ", nullptr));
        snaphotTypeComboBox->setItemText(0, QApplication::translate("NewSeriesForm", "SWI-\321\201\320\275\320\270\320\274\320\272\320\270", nullptr));
        snaphotTypeComboBox->setItemText(1, QApplication::translate("NewSeriesForm", "T2-\321\201\320\275\320\270\320\274\320\272\320\270", nullptr));
        snaphotTypeComboBox->setItemText(2, QApplication::translate("NewSeriesForm", "\320\230\320\232\320\224-\321\201\320\275\320\270\320\274\320\272\320\270", nullptr));

        saveChangesButton->setText(QApplication::translate("NewSeriesForm", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewSeriesForm: public Ui_NewSeriesForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWSERIESFORM_H
