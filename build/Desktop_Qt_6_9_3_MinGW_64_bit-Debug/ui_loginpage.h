/********************************************************************************
** Form generated from reading UI file 'loginpage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINPAGE_H
#define UI_LOGINPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loginPage
{
public:

    void setupUi(QWidget *loginPage)
    {
        if (loginPage->objectName().isEmpty())
            loginPage->setObjectName("loginPage");
        loginPage->resize(400, 300);

        retranslateUi(loginPage);

        QMetaObject::connectSlotsByName(loginPage);
    } // setupUi

    void retranslateUi(QWidget *loginPage)
    {
        loginPage->setWindowTitle(QCoreApplication::translate("loginPage", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loginPage: public Ui_loginPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINPAGE_H
