/********************************************************************************
** Form generated from reading UI file 'registerpage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERPAGE_H
#define UI_REGISTERPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_registerPage
{
public:

    void setupUi(QWidget *registerPage)
    {
        if (registerPage->objectName().isEmpty())
            registerPage->setObjectName("registerPage");
        registerPage->resize(400, 300);

        retranslateUi(registerPage);

        QMetaObject::connectSlotsByName(registerPage);
    } // setupUi

    void retranslateUi(QWidget *registerPage)
    {
        registerPage->setWindowTitle(QCoreApplication::translate("registerPage", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class registerPage: public Ui_registerPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERPAGE_H
