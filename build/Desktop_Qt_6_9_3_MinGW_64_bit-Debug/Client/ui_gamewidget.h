/********************************************************************************
** Form generated from reading UI file 'gamewidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWIDGET_H
#define UI_GAMEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameWidget
{
public:
    QLabel *oriBkLbl;
    QLabel *borderLbl;
    QLabel *scoreLbl;
    QLabel *menuLbl;
    QLabel *hintLbl;
    QLabel *pauseLbl;
    QLabel *reSetBtn;

    void setupUi(QWidget *GameWidget)
    {
        if (GameWidget->objectName().isEmpty())
            GameWidget->setObjectName("GameWidget");
        GameWidget->resize(1920, 1080);
        GameWidget->setStyleSheet(QString::fromUtf8(""));
        oriBkLbl = new QLabel(GameWidget);
        oriBkLbl->setObjectName("oriBkLbl");
        oriBkLbl->setGeometry(QRect(243, 0, 1434, 1080));
        borderLbl = new QLabel(GameWidget);
        borderLbl->setObjectName("borderLbl");
        borderLbl->setGeometry(QRect(1665, 2, 1055, 1078));
        scoreLbl = new QLabel(GameWidget);
        scoreLbl->setObjectName("scoreLbl");
        scoreLbl->setGeometry(QRect(270, 60, 327, 178));
        menuLbl = new QLabel(GameWidget);
        menuLbl->setObjectName("menuLbl");
        menuLbl->setGeometry(QRect(354, 491, 158, 122));
        menuLbl->setStyleSheet(QString::fromUtf8(""));
        hintLbl = new QLabel(GameWidget);
        hintLbl->setObjectName("hintLbl");
        hintLbl->setGeometry(QRect(279, 613, 308, 272));
        hintLbl->setStyleSheet(QString::fromUtf8(""));
        pauseLbl = new QLabel(GameWidget);
        pauseLbl->setObjectName("pauseLbl");
        pauseLbl->setGeometry(QRect(354, 885, 158, 116));
        pauseLbl->setStyleSheet(QString::fromUtf8(""));
        reSetBtn = new QLabel(GameWidget);
        reSetBtn->setObjectName("reSetBtn");
        reSetBtn->setGeometry(QRect(374, 410, 121, 61));
        reSetBtn->setStyleSheet(QString::fromUtf8("border-image: url(:/picture/Settingpage/dialogbutton.png);"));

        retranslateUi(GameWidget);

        QMetaObject::connectSlotsByName(GameWidget);
    } // setupUi

    void retranslateUi(QWidget *GameWidget)
    {
        GameWidget->setWindowTitle(QCoreApplication::translate("GameWidget", "Form", nullptr));
        oriBkLbl->setText(QCoreApplication::translate("GameWidget", "BK", nullptr));
        borderLbl->setText(QCoreApplication::translate("GameWidget", "TextLabel", nullptr));
        scoreLbl->setText(QCoreApplication::translate("GameWidget", "TextLabel", nullptr));
        menuLbl->setText(QCoreApplication::translate("GameWidget", "menu", nullptr));
        hintLbl->setText(QCoreApplication::translate("GameWidget", "hint", nullptr));
        pauseLbl->setText(QCoreApplication::translate("GameWidget", "pause", nullptr));
        reSetBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GameWidget: public Ui_GameWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWIDGET_H
