/********************************************************************************
** Form generated from reading UI file 'ischemia_percentage.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ISCHEMIA_PERCENTAGE_H
#define UI_ISCHEMIA_PERCENTAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_IschemiaPercentage
{
public:
    QVBoxLayout *vboxLayout;
    QTextEdit *textEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *IschemiaPercentage)
    {
        if (IschemiaPercentage->objectName().isEmpty())
            IschemiaPercentage->setObjectName(QString::fromUtf8("IschemiaPercentage"));
        IschemiaPercentage->setWindowModality(Qt::NonModal);
        IschemiaPercentage->resize(680, 390);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(IschemiaPercentage->sizePolicy().hasHeightForWidth());
        IschemiaPercentage->setSizePolicy(sizePolicy);
        IschemiaPercentage->setMinimumSize(QSize(680, 390));
        IschemiaPercentage->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(12);
        IschemiaPercentage->setFont(font);
        IschemiaPercentage->setSizeGripEnabled(true);
        vboxLayout = new QVBoxLayout(IschemiaPercentage);
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(9, 9, 9, 9);
        textEdit = new QTextEdit(IschemiaPercentage);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        vboxLayout->addWidget(textEdit);

        buttonBox = new QDialogButtonBox(IschemiaPercentage);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(IschemiaPercentage);
        QObject::connect(buttonBox, SIGNAL(accepted()), IschemiaPercentage, SLOT(close()));

        QMetaObject::connectSlotsByName(IschemiaPercentage);
    } // setupUi

    void retranslateUi(QDialog *IschemiaPercentage)
    {
        IschemiaPercentage->setWindowTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class IschemiaPercentage: public Ui_IschemiaPercentage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ISCHEMIA_PERCENTAGE_H
