#-------------------------------------------------
#
# Project created by QtCreator 2021-05-29T12:28:29
#
#-------------------------------------------------

QT += core gui multimedia widgets

TARGET = bejeweled_2021
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    client.cpp \
    loginpage.cpp \
    main.cpp \
    hoverbutton.cpp \
    ranklistpage.cpp \
    registerpage.cpp \
    selectlevel.cpp \
    settingpage.cpp \
    startpage.cpp \
    gamewidget.cpp \
    myprobar.cpp \
    gem.cpp

HEADERS += \
    client.h \
    hoverbutton.h \
    loginpage.h \
    ranklistpage.h \
    registerpage.h \
    selectlevel.h \
    settingpage.h \
    startpage.h \
    gamewidget.h \
    myprobar.h \
    gem.h

FORMS += \
    loginpage.ui \
    ranklistpage.ui \
    registerpage.ui \
    selectlevel.ui \
    settingpage.ui \
    startpage.ui \
    gamewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# 注释掉或删除 Windows 特有库
# win32: LIBS += -lole32