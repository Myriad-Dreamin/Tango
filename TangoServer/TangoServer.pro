#-------------------------------------------------
#
# Project created by QtCreator 2019-05-13T15:30:28
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TangoServer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
    ../TangoCommon/automator/AbstractGameAutomation.cpp \
    ../TangoCommon/automator/GameAutomation.cpp \
    ../TangoCommon/automator/GameConfig.cpp \
    ../TangoCommon/client/AbstractClient.cpp \
    ../TangoCommon/client/LocalClient.cpp \
    ../TangoCommon/component/ConfigSet.cpp \
    ../TangoCommon/component/Logger.cpp \
    ../TangoCommon/component/MessageBox.cpp \
    ../TangoCommon/component/PairTableItem.cpp \
    ../TangoCommon/component/TimerWidget.cpp \
    ../TangoCommon/network/SocketX.cpp \
    ../TangoCommon/network/json_rpc.cpp \
    ../TangoCommon/players/Author.cpp \
    ../TangoCommon/players/Consumer.cpp \
    ../TangoCommon/players/player.cpp \
    ../TangoCommon/types/RetriveMode.cpp \
    ../TangoCommon/types/TangoPair.cpp \
    ../TangoCommon/types/UserBriefInfo.cpp \
    ../TangoCommon/types/UserFullInfo.cpp \
    ../TangoCommon/types/UserStatus.cpp \
        engine/TangoThread.cpp \
        engine/TcpServer.cpp \
        main.cpp \
        mainwindow.cpp \

HEADERS += \
    ../TangoCommon/automator/AbstractGameAutomation.h \
    ../TangoCommon/automator/GameAutomation.h \
    ../TangoCommon/automator/GameConfig.h \
    ../TangoCommon/client/AbstractClient.h \
    ../TangoCommon/client/LocalClient.h \
    ../TangoCommon/component/ConfigSet.h \
    ../TangoCommon/component/Logger.h \
    ../TangoCommon/component/MessageBox.h \
    ../TangoCommon/component/PairTableItem.h \
    ../TangoCommon/component/TimerWidget.h \
    ../TangoCommon/network/SocketX.h \
    ../TangoCommon/network/json_rpc.h \
    ../TangoCommon/players/Author.h \
    ../TangoCommon/players/Consumer.h \
    ../TangoCommon/players/Player.h \
    ../TangoCommon/types/RetriveMode.h \
    ../TangoCommon/types/TangoPair.h \
    ../TangoCommon/types/UserBriefInfo.h \
    ../TangoCommon/types/UserFullInfo.h \
    ../TangoCommon/types/UserStatus.h \
        engine/TangoThread.h \
        engine/TcpServer.h \
        mainwindow.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
