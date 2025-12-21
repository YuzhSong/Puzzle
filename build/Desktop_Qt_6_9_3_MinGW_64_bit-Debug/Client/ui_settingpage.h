/********************************************************************************
** Form generated from reading UI file 'settingpage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGPAGE_H
#define UI_SETTINGPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_settingpage
{
public:
    QWidget *centralwidget;
    QPushButton *Web;
    QPushButton *Help;
    QPushButton *Done;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_help;
    QLabel *label_web;
    QLabel *label_done;
    QLabel *label;
    QLabel *label_settings;
    QLabel *label_volume;
    QLabel *label_Bright;
    QPushButton *LangB;
    QLabel *LangL;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *settingpage)
    {
        if (settingpage->objectName().isEmpty())
            settingpage->setObjectName("settingpage");
        settingpage->resize(800, 555);
        centralwidget = new QWidget(settingpage);
        centralwidget->setObjectName("centralwidget");
        Web = new QPushButton(centralwidget);
        Web->setObjectName("Web");
        Web->setGeometry(QRect(460, 200, 131, 51));
        Web->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/dialogbutton.png);"));
        Help = new QPushButton(centralwidget);
        Help->setObjectName("Help");
        Help->setGeometry(QRect(140, 200, 131, 51));
        Help->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/dialogbutton.png);"));
        Done = new QPushButton(centralwidget);
        Done->setObjectName("Done");
        Done->setGeometry(QRect(300, 320, 131, 51));
        Done->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/dialogbutton.png);"));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(160, 140, 131, 31));
        label_3->setVisible(false);
        label_3->setStyleSheet(QString::fromUtf8("background-image: url(:/picture/Settingpage/slider-track-2.png);"));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(160, 200, 131, 31));
        label_4->setVisible(false);
        label_4->setStyleSheet(QString::fromUtf8("background-image: url(:/picture/Settingpage/slider-track-2.png);"));
        label_help = new QLabel(centralwidget);
        label_help->setObjectName("label_help");
        label_help->setGeometry(QRect(180, 210, 61, 31));
        label_help->setStyleSheet(QString::fromUtf8("font: 12pt \"Elephant\";\n"
"color: rgb(255, 255, 255);"));
        label_web = new QLabel(centralwidget);
        label_web->setObjectName("label_web");
        label_web->setGeometry(QRect(500, 210, 61, 31));
        label_web->setStyleSheet(QString::fromUtf8("font: 12pt \"Elephant\";\n"
"color: rgb(255, 255, 255);"));
        label_done = new QLabel(centralwidget);
        label_done->setObjectName("label_done");
        label_done->setGeometry(QRect(340, 330, 61, 31));
        label_done->setStyleSheet(QString::fromUtf8("font: 12pt \"Elephant\";\n"
"color: rgb(255, 255, 255);"));
        label_done->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(210, 20, 271, 81));
        label->setStyleSheet(QString::fromUtf8("background-image: url(:/picture/Settingpage/dialogtitle.png);"));
        label_settings = new QLabel(centralwidget);
        label_settings->setObjectName("label_settings");
        label_settings->setGeometry(QRect(300, 30, 121, 51));
        label_settings->setStyleSheet(QString::fromUtf8("font: 18pt \"Broadway\";\n"
"color: rgb(255, 255, 255);"));
        label_volume = new QLabel(centralwidget);
        label_volume->setObjectName("label_volume");
        label_volume->setGeometry(QRect(190, 139, 71, 31));
        label_volume->setVisible(false);
        label_volume->setStyleSheet(QString::fromUtf8("font: 12pt \"Elephant\";\n"
"color: rgb(255, 255, 255);"));
        label_Bright = new QLabel(centralwidget);
        label_Bright->setObjectName("label_Bright");
        label_Bright->setGeometry(QRect(180, 200, 101, 31));
        label_Bright->setVisible(false);
        label_Bright->setStyleSheet(QString::fromUtf8("font: 12pt \"Elephant\";\n"
"color: rgb(255, 255, 255);"));
        LangB = new QPushButton(centralwidget);
        LangB->setObjectName("LangB");
        LangB->setGeometry(QRect(280, 260, 171, 51));
        LangB->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/Toggle_1.png);"));
        LangL = new QLabel(centralwidget);
        LangL->setObjectName("LangL");
        LangL->setGeometry(QRect(340, 270, 91, 31));
        LangL->setStyleSheet(QString::fromUtf8("font: 12pt \"Elephant\";\n"
"color: rgb(255, 255, 255);"));
        settingpage->setCentralWidget(centralwidget);
        menubar = new QMenuBar(settingpage);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        settingpage->setMenuBar(menubar);
        statusbar = new QStatusBar(settingpage);
        statusbar->setObjectName("statusbar");
        settingpage->setStatusBar(statusbar);

        retranslateUi(settingpage);

        QMetaObject::connectSlotsByName(settingpage);
    } // setupUi

    void retranslateUi(QMainWindow *settingpage)
    {
        settingpage->setWindowTitle(QCoreApplication::translate("settingpage", "MainWindow", nullptr));
        Web->setText(QString());
        Help->setText(QString());
        Done->setText(QString());
        label_3->setText(QString());
        label_4->setText(QString());
        label_help->setText(QCoreApplication::translate("settingpage", "Help", nullptr));
        label_web->setText(QCoreApplication::translate("settingpage", "Web", nullptr));
        label_done->setText(QCoreApplication::translate("settingpage", "Done", nullptr));
        label->setText(QString());
        label_settings->setText(QCoreApplication::translate("settingpage", "Settings", nullptr));
        label_volume->setText(QCoreApplication::translate("settingpage", "Volume", nullptr));
        label_Bright->setText(QCoreApplication::translate("settingpage", "Brightness", nullptr));
        LangB->setText(QString());
        LangL->setText(QCoreApplication::translate("settingpage", "English", nullptr));
    } // retranslateUi

};

namespace Ui {
    class settingpage: public Ui_settingpage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGPAGE_H
