/********************************************************************************
** Form generated from reading UI file 'selectlevel.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTLEVEL_H
#define UI_SELECTLEVEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_selectlevel
{
public:
    QWidget *centralwidget;
    QPushButton *DoneB;
    QPushButton *level2B;
    QPushButton *level3B;
    QPushButton *level1B;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *selectlevel)
    {
        if (selectlevel->objectName().isEmpty())
            selectlevel->setObjectName("selectlevel");
        selectlevel->resize(574, 307);
        centralwidget = new QWidget(selectlevel);
        centralwidget->setObjectName("centralwidget");
        DoneB = new QPushButton(centralwidget);
        DoneB->setObjectName("DoneB");
        DoneB->setGeometry(QRect(223, 200, 121, 51));
        DoneB->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/dialogbutton.png);"));
        level2B = new QPushButton(centralwidget);
        level2B->setObjectName("level2B");
        level2B->setGeometry(QRect(210, 110, 151, 61));
        level2B->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/dialogbutton.png);"));
        level3B = new QPushButton(centralwidget);
        level3B->setObjectName("level3B");
        level3B->setGeometry(QRect(370, 110, 141, 61));
        level3B->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/dialogbutton.png);"));
        level1B = new QPushButton(centralwidget);
        level1B->setObjectName("level1B");
        level1B->setGeometry(QRect(60, 110, 141, 61));
        level1B->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/dialogbutton.png);"));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(170, 20, 231, 51));
        label->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/dialogtitle.png);"));
        selectlevel->setCentralWidget(centralwidget);
        menubar = new QMenuBar(selectlevel);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 574, 23));
        selectlevel->setMenuBar(menubar);
        statusbar = new QStatusBar(selectlevel);
        statusbar->setObjectName("statusbar");
        selectlevel->setStatusBar(statusbar);

        retranslateUi(selectlevel);

        QMetaObject::connectSlotsByName(selectlevel);
    } // setupUi

    void retranslateUi(QMainWindow *selectlevel)
    {
        selectlevel->setWindowTitle(QCoreApplication::translate("selectlevel", "MainWindow", nullptr));
        DoneB->setText(QString());
        level2B->setText(QString());
        level3B->setText(QString());
        level1B->setText(QString());
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class selectlevel: public Ui_selectlevel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTLEVEL_H
