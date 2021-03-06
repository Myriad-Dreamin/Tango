#-------------------------------------------------
#
# Project created by QtCreator 2019-05-12T16:56:54
#
#-------------------------------------------------

QT       += core gui network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tango
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

CONFIG += c++11

SOURCES += \
        client/Client.cpp \
        client/GameAutomation.cpp \
        client/GameConfig.cpp \
        main.cpp \
        mainwindow.cpp \
        players/Author.cpp \
        players/Consumer.cpp \
        players/Player.cpp \
        scene/CreationScene.cpp \
        scene/CreationTableItem.cpp \
        scene/MainScene.cpp \
        scene/PlaySettleScene.cpp \
        scene/PlaySubScene.cpp \
        scene/PlayingScene.cpp \
        scene/QueryUsersScene.cpp \
        scene/RankingAuthorsScene.cpp \
        scene/RankingConsumersScene.cpp \
        scene/RegisterScene.cpp \
        scene/Scene.cpp \
        scene/SelectingScene.cpp \
        types/Logger.cpp \
        types/MessageBox.cpp \
        types/PairTableItem.cpp \
        types/RetriveMode.cpp \
        types/TangoPair.cpp \
        types/TimerWidget.cpp \
        types/UserBriefInfo.cpp \
        types/UserFullInfo.cpp \
        types/UserStatus.cpp

HEADERS += \
        client/Client.h \
        client/GameAutomation.h \
        client/GameConfig.h \
        mainwindow.h \
        players/Author.h \
        players/Consumer.h \
        players/Player.h \
        scene/CreationScene.h \
        scene/CreationTableItem.h \
        scene/MainScene.h \
        scene/PlaySettleScene.h \
        scene/PlaySubScene.h \
        scene/PlayingScene.h \
        scene/QueryUsersScene.h \
        scene/RankingAuthorsScene.h \
        scene/RankingConsumersScene.h \
        scene/RegisterScene.h \
        scene/Scene.h \
        scene/SelectingScene.h \
        types/Logger.h \
        types/MessageBox.h \
        types/PairTableItem.h \
        types/RetriveMode.h \
        types/TangoPair.h \
        types/TimerWidget.h \
        types/UserBriefInfo.h \
        types/UserFullInfo.h \
        types/UserStatus.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    main.qrc
