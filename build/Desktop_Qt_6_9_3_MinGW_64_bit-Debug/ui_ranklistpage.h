/********************************************************************************
** Form generated from reading UI file 'ranklistpage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RANKLISTPAGE_H
#define UI_RANKLISTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_rankListPage
{
public:

    void setupUi(QWidget *rankListPage)
    {
        if (rankListPage->objectName().isEmpty())
            rankListPage->setObjectName("rankListPage");
        rankListPage->resize(400, 300);

        retranslateUi(rankListPage);

        QMetaObject::connectSlotsByName(rankListPage);
    } // setupUi

    void retranslateUi(QWidget *rankListPage)
    {
        rankListPage->setWindowTitle(QCoreApplication::translate("rankListPage", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class rankListPage: public Ui_rankListPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RANKLISTPAGE_H
