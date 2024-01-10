/********************************************************************************
** Form generated from reading UI file 'dicomViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DICOMVIEWER_H
#define UI_DICOMVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_dicomDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *imageVerticalLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QSpinBox *spinBox;
    QLabel *minMaxLabel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton3D;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *okButton;

    void setupUi(QDialog *dicomDialog)
    {
        if (dicomDialog->objectName().isEmpty())
            dicomDialog->setObjectName(QString::fromUtf8("dicomDialog"));
        dicomDialog->setWindowModality(Qt::NonModal);
        dicomDialog->resize(600, 400);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dicomDialog->sizePolicy().hasHeightForWidth());
        dicomDialog->setSizePolicy(sizePolicy);
        dicomDialog->setMinimumSize(QSize(600, 400));
        QFont font;
        font.setPointSize(12);
        dicomDialog->setFont(font);
        dicomDialog->setFocusPolicy(Qt::WheelFocus);
        dicomDialog->setSizeGripEnabled(true);
        verticalLayout_2 = new QVBoxLayout(dicomDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        imageVerticalLayout = new QVBoxLayout();
        imageVerticalLayout->setObjectName(QString::fromUtf8("imageVerticalLayout"));
        imageVerticalLayout->setSizeConstraint(QLayout::SetMaximumSize);

        verticalLayout_2->addLayout(imageVerticalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        spinBox = new QSpinBox(dicomDialog);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimumSize(QSize(74, 0));
        QPalette palette;
        QBrush brush(QColor(223, 223, 223, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush);
        palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush1);
        QBrush brush2(QColor(0, 120, 215, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush1);
        spinBox->setPalette(palette);
        spinBox->setFocusPolicy(Qt::StrongFocus);
        spinBox->setLayoutDirection(Qt::LeftToRight);
        spinBox->setAlignment(Qt::AlignCenter);
        spinBox->setMinimum(-1);
        spinBox->setMaximum(-1);

        horizontalLayout_2->addWidget(spinBox);

        minMaxLabel = new QLabel(dicomDialog);
        minMaxLabel->setObjectName(QString::fromUtf8("minMaxLabel"));
        minMaxLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(minMaxLabel);

        horizontalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton3D = new QPushButton(dicomDialog);
        pushButton3D->setObjectName(QString::fromUtf8("pushButton3D"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton3D->sizePolicy().hasHeightForWidth());
        pushButton3D->setSizePolicy(sizePolicy1);
        pushButton3D->setMinimumSize(QSize(31, 31));

        horizontalLayout_2->addWidget(pushButton3D);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalSpacer_3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        okButton = new QPushButton(dicomDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        horizontalLayout->addWidget(okButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(dicomDialog);
        QObject::connect(okButton, SIGNAL(clicked()), dicomDialog, SLOT(acceptCommand()));
        QObject::connect(pushButton3D, SIGNAL(clicked()), dicomDialog, SLOT(show3D()));

        QMetaObject::connectSlotsByName(dicomDialog);
    } // setupUi

    void retranslateUi(QDialog *dicomDialog)
    {
        dicomDialog->setWindowTitle(QString());
        minMaxLabel->setText(QString());
        pushButton3D->setText(QApplication::translate("dicomDialog", "3D", nullptr));
        okButton->setText(QApplication::translate("dicomDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dicomDialog: public Ui_dicomDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DICOMVIEWER_H
