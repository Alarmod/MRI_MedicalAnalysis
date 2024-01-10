/********************************************************************************
** Form generated from reading UI file 'NewSeriesFormCombined.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWSERIESFORMCOMBINED_H
#define UI_NEWSERIESFORMCOMBINED_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewSeriesFormCombined
{
public:
    QGridLayout *gridLayout;
    QFormLayout *formLayout;
    QGroupBox *mskGroupBox;
    QWidget *formLayoutWidget_2;
    QFormLayout *mskFormLayout;
    QLabel *mskDaysLabel;
    QSpinBox *mskPeriodSpinBox;
    QLabel *mskSnaphotTypeLabel;
    QComboBox *mskSnaphotTypeComboBox;
    QHBoxLayout *mskHorizontalLayout;
    QLabel *mskPathLabel;
    QPlainTextEdit *mskPlainTextEdit;
    QPushButton *mskSelectPathButton;
    QHBoxLayout *mskHorizontalLayout_2;
    QLabel *mskSnapshotSelectorLabel;
    QCheckBox *mskSnapshotSelectorCheckBox;
    QSpinBox *mskSnapshotSelectorMinSpinBox;
    QLabel *mskDefLabel;
    QSpinBox *mskSnapshotSelectorMaxSpinBox;
    QGroupBox *ischemiaGroupBox;
    QWidget *formLayoutWidget;
    QFormLayout *ischemiaFormLayout;
    QLabel *ischemiaDaysLabel;
    QSpinBox *ischemiaPeriodSpinBox;
    QLabel *ischemiaSnaphotTypeLabel;
    QComboBox *ischemiaSnaphotTypeComboBox;
    QHBoxLayout *ischemiaHorizontalLayout;
    QLabel *ischemiaPathLabel;
    QPlainTextEdit *ischemiaPlainTextEdit;
    QPushButton *ischemiaSelectPathButton;
    QHBoxLayout *ischemiaHorizontalLayout_2;
    QLabel *ischemiaSnapshotSelectorLabel;
    QCheckBox *ischemiaSnapshotSelectorCheckBox;
    QSpinBox *ischemiaSnapshotSelectorMinSpinBox;
    QLabel *ischemiaDefLabel;
    QSpinBox *ischemiaSnapshotSelectorMaxSpinBox;
    QSpacerItem *verticalSpacer_2;
    QPushButton *saveChangesButton;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *NewSeriesFormCombined)
    {
        if (NewSeriesFormCombined->objectName().isEmpty())
            NewSeriesFormCombined->setObjectName(QString::fromUtf8("NewSeriesFormCombined"));
        NewSeriesFormCombined->setWindowModality(Qt::ApplicationModal);
        NewSeriesFormCombined->resize(750, 361);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NewSeriesFormCombined->sizePolicy().hasHeightForWidth());
        NewSeriesFormCombined->setSizePolicy(sizePolicy);
        NewSeriesFormCombined->setMinimumSize(QSize(750, 350));
        NewSeriesFormCombined->setMaximumSize(QSize(750, 361));
        gridLayout = new QGridLayout(NewSeriesFormCombined);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setSizeConstraint(QLayout::SetMaximumSize);
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        mskGroupBox = new QGroupBox(NewSeriesFormCombined);
        mskGroupBox->setObjectName(QString::fromUtf8("mskGroupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mskGroupBox->sizePolicy().hasHeightForWidth());
        mskGroupBox->setSizePolicy(sizePolicy1);
        mskGroupBox->setMinimumSize(QSize(0, 145));
        formLayoutWidget_2 = new QWidget(mskGroupBox);
        formLayoutWidget_2->setObjectName(QString::fromUtf8("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(9, 20, 711, 131));
        mskFormLayout = new QFormLayout(formLayoutWidget_2);
        mskFormLayout->setSpacing(6);
        mskFormLayout->setContentsMargins(11, 11, 11, 11);
        mskFormLayout->setObjectName(QString::fromUtf8("mskFormLayout"));
        mskFormLayout->setSizeConstraint(QLayout::SetMaximumSize);
        mskFormLayout->setContentsMargins(0, 0, 0, 0);
        mskDaysLabel = new QLabel(formLayoutWidget_2);
        mskDaysLabel->setObjectName(QString::fromUtf8("mskDaysLabel"));
        mskDaysLabel->setAlignment(Qt::AlignCenter);

        mskFormLayout->setWidget(0, QFormLayout::LabelRole, mskDaysLabel);

        mskPeriodSpinBox = new QSpinBox(formLayoutWidget_2);
        mskPeriodSpinBox->setObjectName(QString::fromUtf8("mskPeriodSpinBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mskPeriodSpinBox->sizePolicy().hasHeightForWidth());
        mskPeriodSpinBox->setSizePolicy(sizePolicy2);
        mskPeriodSpinBox->setMinimumSize(QSize(50, 0));
        mskPeriodSpinBox->setMaximumSize(QSize(50, 16777215));
        mskPeriodSpinBox->setAlignment(Qt::AlignCenter);
        mskPeriodSpinBox->setMinimum(0);
        mskPeriodSpinBox->setMaximum(100);
        mskPeriodSpinBox->setValue(0);

        mskFormLayout->setWidget(0, QFormLayout::FieldRole, mskPeriodSpinBox);

        mskSnaphotTypeLabel = new QLabel(formLayoutWidget_2);
        mskSnaphotTypeLabel->setObjectName(QString::fromUtf8("mskSnaphotTypeLabel"));

        mskFormLayout->setWidget(1, QFormLayout::LabelRole, mskSnaphotTypeLabel);

        mskSnaphotTypeComboBox = new QComboBox(formLayoutWidget_2);
        mskSnaphotTypeComboBox->addItem(QString());
        mskSnaphotTypeComboBox->addItem(QString());
        mskSnaphotTypeComboBox->addItem(QString());
        mskSnaphotTypeComboBox->setObjectName(QString::fromUtf8("mskSnaphotTypeComboBox"));

        mskFormLayout->setWidget(1, QFormLayout::FieldRole, mskSnaphotTypeComboBox);

        mskHorizontalLayout = new QHBoxLayout();
        mskHorizontalLayout->setSpacing(6);
        mskHorizontalLayout->setObjectName(QString::fromUtf8("mskHorizontalLayout"));
        mskPathLabel = new QLabel(formLayoutWidget_2);
        mskPathLabel->setObjectName(QString::fromUtf8("mskPathLabel"));

        mskHorizontalLayout->addWidget(mskPathLabel);

        mskPlainTextEdit = new QPlainTextEdit(formLayoutWidget_2);
        mskPlainTextEdit->setObjectName(QString::fromUtf8("mskPlainTextEdit"));
        mskPlainTextEdit->setEnabled(false);
        sizePolicy.setHeightForWidth(mskPlainTextEdit->sizePolicy().hasHeightForWidth());
        mskPlainTextEdit->setSizePolicy(sizePolicy);
        mskPlainTextEdit->setMinimumSize(QSize(220, 0));
        mskPlainTextEdit->setMaximumSize(QSize(16777215, 22));
        QFont font;
        font.setItalic(false);
        mskPlainTextEdit->setFont(font);
        mskPlainTextEdit->setInputMethodHints(Qt::ImhNone);
        mskPlainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mskPlainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mskPlainTextEdit->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        mskPlainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        mskPlainTextEdit->setReadOnly(true);
        mskPlainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        mskPlainTextEdit->setMaximumBlockCount(1);

        mskHorizontalLayout->addWidget(mskPlainTextEdit);


        mskFormLayout->setLayout(2, QFormLayout::LabelRole, mskHorizontalLayout);

        mskSelectPathButton = new QPushButton(formLayoutWidget_2);
        mskSelectPathButton->setObjectName(QString::fromUtf8("mskSelectPathButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(mskSelectPathButton->sizePolicy().hasHeightForWidth());
        mskSelectPathButton->setSizePolicy(sizePolicy3);
        mskSelectPathButton->setMinimumSize(QSize(50, 0));
        mskSelectPathButton->setMaximumSize(QSize(50, 16777215));

        mskFormLayout->setWidget(2, QFormLayout::FieldRole, mskSelectPathButton);

        mskHorizontalLayout_2 = new QHBoxLayout();
        mskHorizontalLayout_2->setSpacing(6);
        mskHorizontalLayout_2->setObjectName(QString::fromUtf8("mskHorizontalLayout_2"));
        mskSnapshotSelectorLabel = new QLabel(formLayoutWidget_2);
        mskSnapshotSelectorLabel->setObjectName(QString::fromUtf8("mskSnapshotSelectorLabel"));

        mskHorizontalLayout_2->addWidget(mskSnapshotSelectorLabel);

        mskSnapshotSelectorCheckBox = new QCheckBox(formLayoutWidget_2);
        mskSnapshotSelectorCheckBox->setObjectName(QString::fromUtf8("mskSnapshotSelectorCheckBox"));
        mskSnapshotSelectorCheckBox->setChecked(true);

        mskHorizontalLayout_2->addWidget(mskSnapshotSelectorCheckBox);

        mskSnapshotSelectorMinSpinBox = new QSpinBox(formLayoutWidget_2);
        mskSnapshotSelectorMinSpinBox->setObjectName(QString::fromUtf8("mskSnapshotSelectorMinSpinBox"));
        mskSnapshotSelectorMinSpinBox->setEnabled(false);
        sizePolicy3.setHeightForWidth(mskSnapshotSelectorMinSpinBox->sizePolicy().hasHeightForWidth());
        mskSnapshotSelectorMinSpinBox->setSizePolicy(sizePolicy3);
        mskSnapshotSelectorMinSpinBox->setMinimumSize(QSize(50, 0));
        mskSnapshotSelectorMinSpinBox->setMaximumSize(QSize(50, 16777215));
        mskSnapshotSelectorMinSpinBox->setAlignment(Qt::AlignCenter);
        mskSnapshotSelectorMinSpinBox->setMinimum(-1);
        mskSnapshotSelectorMinSpinBox->setMaximum(-1);
        mskSnapshotSelectorMinSpinBox->setValue(-1);

        mskHorizontalLayout_2->addWidget(mskSnapshotSelectorMinSpinBox);

        mskDefLabel = new QLabel(formLayoutWidget_2);
        mskDefLabel->setObjectName(QString::fromUtf8("mskDefLabel"));

        mskHorizontalLayout_2->addWidget(mskDefLabel);


        mskFormLayout->setLayout(3, QFormLayout::LabelRole, mskHorizontalLayout_2);

        mskSnapshotSelectorMaxSpinBox = new QSpinBox(formLayoutWidget_2);
        mskSnapshotSelectorMaxSpinBox->setObjectName(QString::fromUtf8("mskSnapshotSelectorMaxSpinBox"));
        mskSnapshotSelectorMaxSpinBox->setEnabled(false);
        sizePolicy3.setHeightForWidth(mskSnapshotSelectorMaxSpinBox->sizePolicy().hasHeightForWidth());
        mskSnapshotSelectorMaxSpinBox->setSizePolicy(sizePolicy3);
        mskSnapshotSelectorMaxSpinBox->setMinimumSize(QSize(50, 0));
        mskSnapshotSelectorMaxSpinBox->setMaximumSize(QSize(50, 16777215));
        mskSnapshotSelectorMaxSpinBox->setFont(font);
        mskSnapshotSelectorMaxSpinBox->setAlignment(Qt::AlignCenter);
        mskSnapshotSelectorMaxSpinBox->setMinimum(-1);
        mskSnapshotSelectorMaxSpinBox->setMaximum(-1);
        mskSnapshotSelectorMaxSpinBox->setValue(-1);

        mskFormLayout->setWidget(3, QFormLayout::FieldRole, mskSnapshotSelectorMaxSpinBox);


        formLayout->setWidget(0, QFormLayout::SpanningRole, mskGroupBox);

        ischemiaGroupBox = new QGroupBox(NewSeriesFormCombined);
        ischemiaGroupBox->setObjectName(QString::fromUtf8("ischemiaGroupBox"));
        sizePolicy1.setHeightForWidth(ischemiaGroupBox->sizePolicy().hasHeightForWidth());
        ischemiaGroupBox->setSizePolicy(sizePolicy1);
        ischemiaGroupBox->setMinimumSize(QSize(0, 145));
        QFont font1;
        font1.setItalic(true);
        ischemiaGroupBox->setFont(font1);
        formLayoutWidget = new QWidget(ischemiaGroupBox);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(9, 20, 711, 131));
        ischemiaFormLayout = new QFormLayout(formLayoutWidget);
        ischemiaFormLayout->setSpacing(6);
        ischemiaFormLayout->setContentsMargins(11, 11, 11, 11);
        ischemiaFormLayout->setObjectName(QString::fromUtf8("ischemiaFormLayout"));
        ischemiaFormLayout->setSizeConstraint(QLayout::SetMaximumSize);
        ischemiaFormLayout->setContentsMargins(0, 0, 0, 0);
        ischemiaDaysLabel = new QLabel(formLayoutWidget);
        ischemiaDaysLabel->setObjectName(QString::fromUtf8("ischemiaDaysLabel"));
        ischemiaDaysLabel->setFont(font);
        ischemiaDaysLabel->setAlignment(Qt::AlignCenter);

        ischemiaFormLayout->setWidget(0, QFormLayout::LabelRole, ischemiaDaysLabel);

        ischemiaPeriodSpinBox = new QSpinBox(formLayoutWidget);
        ischemiaPeriodSpinBox->setObjectName(QString::fromUtf8("ischemiaPeriodSpinBox"));
        sizePolicy2.setHeightForWidth(ischemiaPeriodSpinBox->sizePolicy().hasHeightForWidth());
        ischemiaPeriodSpinBox->setSizePolicy(sizePolicy2);
        ischemiaPeriodSpinBox->setMinimumSize(QSize(50, 0));
        ischemiaPeriodSpinBox->setMaximumSize(QSize(50, 16777215));
        ischemiaPeriodSpinBox->setFont(font);
        ischemiaPeriodSpinBox->setAlignment(Qt::AlignCenter);
        ischemiaPeriodSpinBox->setMinimum(0);
        ischemiaPeriodSpinBox->setMaximum(100);
        ischemiaPeriodSpinBox->setValue(0);

        ischemiaFormLayout->setWidget(0, QFormLayout::FieldRole, ischemiaPeriodSpinBox);

        ischemiaSnaphotTypeLabel = new QLabel(formLayoutWidget);
        ischemiaSnaphotTypeLabel->setObjectName(QString::fromUtf8("ischemiaSnaphotTypeLabel"));
        ischemiaSnaphotTypeLabel->setFont(font);

        ischemiaFormLayout->setWidget(1, QFormLayout::LabelRole, ischemiaSnaphotTypeLabel);

        ischemiaSnaphotTypeComboBox = new QComboBox(formLayoutWidget);
        ischemiaSnaphotTypeComboBox->addItem(QString());
        ischemiaSnaphotTypeComboBox->addItem(QString());
        ischemiaSnaphotTypeComboBox->addItem(QString());
        ischemiaSnaphotTypeComboBox->setObjectName(QString::fromUtf8("ischemiaSnaphotTypeComboBox"));
        ischemiaSnaphotTypeComboBox->setFont(font);

        ischemiaFormLayout->setWidget(1, QFormLayout::FieldRole, ischemiaSnaphotTypeComboBox);

        ischemiaHorizontalLayout = new QHBoxLayout();
        ischemiaHorizontalLayout->setSpacing(6);
        ischemiaHorizontalLayout->setObjectName(QString::fromUtf8("ischemiaHorizontalLayout"));
        ischemiaPathLabel = new QLabel(formLayoutWidget);
        ischemiaPathLabel->setObjectName(QString::fromUtf8("ischemiaPathLabel"));
        ischemiaPathLabel->setFont(font);

        ischemiaHorizontalLayout->addWidget(ischemiaPathLabel);

        ischemiaPlainTextEdit = new QPlainTextEdit(formLayoutWidget);
        ischemiaPlainTextEdit->setObjectName(QString::fromUtf8("ischemiaPlainTextEdit"));
        ischemiaPlainTextEdit->setEnabled(false);
        sizePolicy.setHeightForWidth(ischemiaPlainTextEdit->sizePolicy().hasHeightForWidth());
        ischemiaPlainTextEdit->setSizePolicy(sizePolicy);
        ischemiaPlainTextEdit->setMinimumSize(QSize(220, 0));
        ischemiaPlainTextEdit->setMaximumSize(QSize(16777215, 22));
        ischemiaPlainTextEdit->setFont(font);
        ischemiaPlainTextEdit->setInputMethodHints(Qt::ImhNone);
        ischemiaPlainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ischemiaPlainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ischemiaPlainTextEdit->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        ischemiaPlainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ischemiaPlainTextEdit->setReadOnly(true);
        ischemiaPlainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        ischemiaPlainTextEdit->setMaximumBlockCount(1);

        ischemiaHorizontalLayout->addWidget(ischemiaPlainTextEdit);


        ischemiaFormLayout->setLayout(2, QFormLayout::LabelRole, ischemiaHorizontalLayout);

        ischemiaSelectPathButton = new QPushButton(formLayoutWidget);
        ischemiaSelectPathButton->setObjectName(QString::fromUtf8("ischemiaSelectPathButton"));
        sizePolicy3.setHeightForWidth(ischemiaSelectPathButton->sizePolicy().hasHeightForWidth());
        ischemiaSelectPathButton->setSizePolicy(sizePolicy3);
        ischemiaSelectPathButton->setMinimumSize(QSize(50, 0));
        ischemiaSelectPathButton->setMaximumSize(QSize(50, 16777215));

        ischemiaFormLayout->setWidget(2, QFormLayout::FieldRole, ischemiaSelectPathButton);

        ischemiaHorizontalLayout_2 = new QHBoxLayout();
        ischemiaHorizontalLayout_2->setSpacing(6);
        ischemiaHorizontalLayout_2->setObjectName(QString::fromUtf8("ischemiaHorizontalLayout_2"));
        ischemiaSnapshotSelectorLabel = new QLabel(formLayoutWidget);
        ischemiaSnapshotSelectorLabel->setObjectName(QString::fromUtf8("ischemiaSnapshotSelectorLabel"));

        ischemiaHorizontalLayout_2->addWidget(ischemiaSnapshotSelectorLabel);

        ischemiaSnapshotSelectorCheckBox = new QCheckBox(formLayoutWidget);
        ischemiaSnapshotSelectorCheckBox->setObjectName(QString::fromUtf8("ischemiaSnapshotSelectorCheckBox"));
        ischemiaSnapshotSelectorCheckBox->setChecked(true);

        ischemiaHorizontalLayout_2->addWidget(ischemiaSnapshotSelectorCheckBox);

        ischemiaSnapshotSelectorMinSpinBox = new QSpinBox(formLayoutWidget);
        ischemiaSnapshotSelectorMinSpinBox->setObjectName(QString::fromUtf8("ischemiaSnapshotSelectorMinSpinBox"));
        ischemiaSnapshotSelectorMinSpinBox->setEnabled(false);
        sizePolicy3.setHeightForWidth(ischemiaSnapshotSelectorMinSpinBox->sizePolicy().hasHeightForWidth());
        ischemiaSnapshotSelectorMinSpinBox->setSizePolicy(sizePolicy3);
        ischemiaSnapshotSelectorMinSpinBox->setMinimumSize(QSize(50, 0));
        ischemiaSnapshotSelectorMinSpinBox->setMaximumSize(QSize(50, 16777215));
        ischemiaSnapshotSelectorMinSpinBox->setFont(font);
        ischemiaSnapshotSelectorMinSpinBox->setAlignment(Qt::AlignCenter);
        ischemiaSnapshotSelectorMinSpinBox->setMinimum(-1);
        ischemiaSnapshotSelectorMinSpinBox->setMaximum(-1);
        ischemiaSnapshotSelectorMinSpinBox->setValue(-1);

        ischemiaHorizontalLayout_2->addWidget(ischemiaSnapshotSelectorMinSpinBox);

        ischemiaDefLabel = new QLabel(formLayoutWidget);
        ischemiaDefLabel->setObjectName(QString::fromUtf8("ischemiaDefLabel"));

        ischemiaHorizontalLayout_2->addWidget(ischemiaDefLabel);


        ischemiaFormLayout->setLayout(3, QFormLayout::LabelRole, ischemiaHorizontalLayout_2);

        ischemiaSnapshotSelectorMaxSpinBox = new QSpinBox(formLayoutWidget);
        ischemiaSnapshotSelectorMaxSpinBox->setObjectName(QString::fromUtf8("ischemiaSnapshotSelectorMaxSpinBox"));
        ischemiaSnapshotSelectorMaxSpinBox->setEnabled(false);
        sizePolicy3.setHeightForWidth(ischemiaSnapshotSelectorMaxSpinBox->sizePolicy().hasHeightForWidth());
        ischemiaSnapshotSelectorMaxSpinBox->setSizePolicy(sizePolicy3);
        ischemiaSnapshotSelectorMaxSpinBox->setMinimumSize(QSize(50, 0));
        ischemiaSnapshotSelectorMaxSpinBox->setMaximumSize(QSize(50, 16777215));
        ischemiaSnapshotSelectorMaxSpinBox->setFont(font);
        ischemiaSnapshotSelectorMaxSpinBox->setAlignment(Qt::AlignCenter);
        ischemiaSnapshotSelectorMaxSpinBox->setMinimum(-1);
        ischemiaSnapshotSelectorMaxSpinBox->setMaximum(-1);
        ischemiaSnapshotSelectorMaxSpinBox->setValue(-1);

        ischemiaFormLayout->setWidget(3, QFormLayout::FieldRole, ischemiaSnapshotSelectorMaxSpinBox);


        formLayout->setWidget(1, QFormLayout::SpanningRole, ischemiaGroupBox);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(2, QFormLayout::FieldRole, verticalSpacer_2);

        saveChangesButton = new QPushButton(NewSeriesFormCombined);
        saveChangesButton->setObjectName(QString::fromUtf8("saveChangesButton"));
        saveChangesButton->setMaximumSize(QSize(16777215, 16777215));

        formLayout->setWidget(3, QFormLayout::FieldRole, saveChangesButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(4, QFormLayout::LabelRole, verticalSpacer);

        horizontalSpacer = new QSpacerItem(550, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(3, QFormLayout::LabelRole, horizontalSpacer);


        gridLayout->addLayout(formLayout, 0, 0, 1, 1);


        retranslateUi(NewSeriesFormCombined);
        QObject::connect(saveChangesButton, SIGNAL(clicked()), NewSeriesFormCombined, SLOT(saveChangesAndClose()));
        QObject::connect(mskSnapshotSelectorCheckBox, SIGNAL(toggled(bool)), NewSeriesFormCombined, SLOT(mskSnapshotSelectorCheckBoxStatusChanged()));
        QObject::connect(mskSelectPathButton, SIGNAL(clicked()), NewSeriesFormCombined, SLOT(mskSelectPath()));
        QObject::connect(mskPlainTextEdit, SIGNAL(textChanged()), NewSeriesFormCombined, SLOT(mskPathUpdated()));
        QObject::connect(ischemiaSnapshotSelectorCheckBox, SIGNAL(toggled(bool)), NewSeriesFormCombined, SLOT(ischemiaSnapshotSelectorCheckBoxStatusChanged()));
        QObject::connect(ischemiaSelectPathButton, SIGNAL(clicked()), NewSeriesFormCombined, SLOT(ischemiaSelectPath()));
        QObject::connect(ischemiaPlainTextEdit, SIGNAL(textChanged()), NewSeriesFormCombined, SLOT(ischemiaPathUpdated()));

        mskSnaphotTypeComboBox->setCurrentIndex(0);
        ischemiaSnaphotTypeComboBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(NewSeriesFormCombined);
    } // setupUi

    void retranslateUi(QWidget *NewSeriesFormCombined)
    {
        NewSeriesFormCombined->setWindowTitle(QApplication::translate("NewSeriesFormCombined", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \321\201\320\265\321\200\320\270\320\265\320\271 \321\201\320\275\320\270\320\274\320\272\320\276\320\262", nullptr));
        mskGroupBox->setTitle(QApplication::translate("NewSeriesFormCombined", "\320\224\320\260\320\275\320\275\321\213\320\265 \320\264\320\273\321\217 \321\200\320\260\320\261\320\276\321\202\321\213 \321\201 \320\274\320\265\320\267\320\265\320\275\321\205\320\270\320\274\320\260\320\273\321\214\320\275\321\213\320\274\320\270 \321\201\321\202\320\262\320\276\320\273\320\276\320\262\321\213\320\274\320\270 \320\272\320\273\320\265\321\202\320\272\320\260\320\274\320\270", nullptr));
        mskDaysLabel->setText(QString());
        mskSnaphotTypeLabel->setText(QApplication::translate("NewSeriesFormCombined", "\320\242\320\270\320\277 \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: ", nullptr));
        mskSnaphotTypeComboBox->setItemText(0, QApplication::translate("NewSeriesFormCombined", "SWI-\321\201\320\275\320\270\320\274\320\272\320\270", nullptr));
        mskSnaphotTypeComboBox->setItemText(1, QApplication::translate("NewSeriesFormCombined", "T2-\321\201\320\275\320\270\320\274\320\272\320\270", nullptr));
        mskSnaphotTypeComboBox->setItemText(2, QApplication::translate("NewSeriesFormCombined", "\320\230\320\232\320\224-\321\201\320\275\320\270\320\274\320\272\320\270", nullptr));

        mskPathLabel->setText(QApplication::translate("NewSeriesFormCombined", "\320\237\321\203\321\202\321\214 \320\264\320\276 \321\201\320\265\321\200\320\270\320\270 \321\201\320\275\320\270\320\274\320\272\320\276\320\262:                       ", nullptr));
        mskSelectPathButton->setText(QApplication::translate("NewSeriesFormCombined", "...", nullptr));
        mskSnapshotSelectorLabel->setText(QApplication::translate("NewSeriesFormCombined", "\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275 \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \321\201\321\200\320\265\320\267\320\276\320\262:                        ", nullptr));
        mskSnapshotSelectorCheckBox->setText(QApplication::translate("NewSeriesFormCombined", "\320\222\321\201\320\265 \321\201\321\200\320\265\320\267\321\213?", nullptr));
        mskDefLabel->setText(QApplication::translate("NewSeriesFormCombined", "-", nullptr));
        ischemiaGroupBox->setTitle(QApplication::translate("NewSeriesFormCombined", "\320\224\320\260\320\275\320\275\321\213\320\265 \320\270\321\201\321\201\320\273\320\265\320\264\320\276\320\262\320\260\320\275\320\270\321\217 \320\270\321\210\320\265\320\274\320\270\321\207\320\265\321\201\320\272\320\276\320\263\320\276 \320\277\320\276\321\200\320\260\320\266\320\265\320\275\320\270\321\217", nullptr));
        ischemiaDaysLabel->setText(QString());
        ischemiaSnaphotTypeLabel->setText(QApplication::translate("NewSeriesFormCombined", "\320\242\320\270\320\277 \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \320\264\320\260\320\275\320\275\321\213\321\205: ", nullptr));
        ischemiaSnaphotTypeComboBox->setItemText(0, QApplication::translate("NewSeriesFormCombined", "SWI-\321\201\320\275\320\270\320\274\320\272\320\270", nullptr));
        ischemiaSnaphotTypeComboBox->setItemText(1, QApplication::translate("NewSeriesFormCombined", "T2-\321\201\320\275\320\270\320\274\320\272\320\270", nullptr));
        ischemiaSnaphotTypeComboBox->setItemText(2, QApplication::translate("NewSeriesFormCombined", "\320\230\320\232\320\224-\321\201\320\275\320\270\320\274\320\272\320\270", nullptr));

        ischemiaPathLabel->setText(QApplication::translate("NewSeriesFormCombined", "\320\237\321\203\321\202\321\214 \320\264\320\276 \321\201\320\265\321\200\320\270\320\270 \321\201\320\275\320\270\320\274\320\272\320\276\320\262:                       ", nullptr));
        ischemiaSelectPathButton->setText(QApplication::translate("NewSeriesFormCombined", "...", nullptr));
        ischemiaSnapshotSelectorLabel->setText(QApplication::translate("NewSeriesFormCombined", "\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275 \320\276\320\261\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\265\320\274\321\213\321\205 \321\201\321\200\320\265\320\267\320\276\320\262:                        ", nullptr));
        ischemiaSnapshotSelectorCheckBox->setText(QApplication::translate("NewSeriesFormCombined", "\320\222\321\201\320\265 \321\201\321\200\320\265\320\267\321\213?", nullptr));
        ischemiaDefLabel->setText(QApplication::translate("NewSeriesFormCombined", "-", nullptr));
        saveChangesButton->setText(QApplication::translate("NewSeriesFormCombined", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewSeriesFormCombined: public Ui_NewSeriesFormCombined {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWSERIESFORMCOMBINED_H
