/********************************************************************************
** Form generated from reading UI file 'Configura.ui'
**
** Created: Wed Sep 8 21:45:10 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGURA_H
#define UI_CONFIGURA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Configura
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Configura)
    {
        if (Configura->objectName().isEmpty())
            Configura->setObjectName(QString::fromUtf8("Configura"));
        Configura->resize(400, 300);
        buttonBox = new QDialogButtonBox(Configura);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(210, 250, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(Configura);
        QObject::connect(buttonBox, SIGNAL(accepted()), Configura, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Configura, SLOT(reject()));

        QMetaObject::connectSlotsByName(Configura);
    } // setupUi

    void retranslateUi(QDialog *Configura)
    {
        Configura->setWindowTitle(QApplication::translate("Configura", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Configura: public Ui_Configura {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGURA_H
