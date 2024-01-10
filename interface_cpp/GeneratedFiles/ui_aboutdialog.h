/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->setWindowModality(Qt::ApplicationModal);
        AboutDialog->resize(650, 200);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AboutDialog->sizePolicy().hasHeightForWidth());
        AboutDialog->setSizePolicy(sizePolicy);
        AboutDialog->setMinimumSize(QSize(650, 200));
        AboutDialog->setMaximumSize(QSize(650, 200));
        QFont font;
        font.setPointSize(12);
        AboutDialog->setFont(font);
        vboxLayout = new QVBoxLayout(AboutDialog);
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(9, 9, 9, 9);
        label = new QLabel(AboutDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 16777215));
        label->setLayoutDirection(Qt::LeftToRight);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);

        vboxLayout->addWidget(label);

        buttonBox = new QDialogButtonBox(AboutDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        vboxLayout->addWidget(buttonBox);


        retranslateUi(AboutDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AboutDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AboutDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
        label->setText(QApplication::translate("AboutDialog", "\320\237\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\260 \321\201\320\276\320\267\320\264\320\260\320\275\320\260 \321\201\320\273\320\265\320\264\321\203\321\216\321\211\320\270\320\274\320\270 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\260\320\274\320\270 \320\230\320\237\320\241 \320\270\320\274. \320\220.\320\232. \320\220\320\271\320\273\320\260\320\274\320\260\320\267\321\217\320\275\320\260 \320\240\320\220\320\235: \n"
" \320\262\320\265\320\264\321\203\321\211\320\270\320\274 \320\275\320\260\321\203\321\207\320\275\321\213\320\274 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\276\320\274, \320\272.\321\202.\320\275. \320\244\321\200\320\260\320\273\320\265\320\275\320\272\320\276 \320\222.\320\237., \n"
" \320\275\320\260\321\203\321\207\320\275\321\213\320\274 \321\201\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\276\320\274, \320\272.\321\202.\320\275. \320\250\321\203\321\201\321\202\320\276\320"
                        "\262\320\276\320\271 \320\234.\320\222.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
