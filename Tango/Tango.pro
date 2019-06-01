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
DEFINES += QT_NO_WARNING_OUTPUT
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        ../TangoCommon/automator/AbstractGameAutomation.cpp \
        ../TangoCommon/automator/GameAutomation.cpp \
        ../TangoCommon/automator/GameAutomationRelayer.cpp \
        ../TangoCommon/automator/GameConfig.cpp \
        ../TangoCommon/client/AbstractClient.cpp \
        ../TangoCommon/client/Client.cpp \
        ../TangoCommon/client/GameClient.cpp \
        ../TangoCommon/client/LocalClient.cpp \
        ../TangoCommon/client/RemoteClient.cpp \
        ../TangoCommon/component/ConfigSet.cpp \
        ../TangoCommon/network/json_rpc.cpp \
        ../TangoCommon/network/SocketX.cpp \
        ../TangoCommon/players/Author.cpp \
        ../TangoCommon/players/Consumer.cpp \
        ../TangoCommon/players/player.cpp \
        ../TangoCommon/component/Logger.cpp \
        ../TangoCommon/component/MessageBox.cpp \
        ../TangoCommon/component/PairTableItem.cpp \
        ../TangoCommon/component/TimerWidget.cpp \
        ../TangoCommon/types/RetriveMode.cpp \
        ../TangoCommon/types/TangoPair.cpp \
        ../TangoCommon/types/UserBriefInfo.cpp \
        ../TangoCommon/types/UserFullInfo.cpp \
        ../TangoCommon/types/UserStatus.cpp \
        main.cpp \
        mainwindow.cpp \
        scene/ConfigScene.cpp \
        scene/CreationScene.cpp \
        scene/LoginScene.cpp \
        scene/MainScene.cpp \
        scene/MultiPlayingScene.cpp \
        scene/PlaySettleScene.cpp \
        scene/PlaySubScene.cpp \
        scene/PlayingScene.cpp \
        scene/QueryUsersScene.cpp \
        scene/RankingAuthorsScene.cpp \
        scene/RankingConsumersScene.cpp \
        scene/RegisterScene.cpp \
        scene/Scene.cpp \
        scene/SelectingScene.cpp

HEADERS += \
        ../TangoCommon/automator/AbstractGameAutomation.h \
        ../TangoCommon/automator/GameAutomation.h \
        ../TangoCommon/automator/GameAutomationRelayer.h \
        ../TangoCommon/automator/GameConfig.h \
        ../TangoCommon/client/AbstractClient.h \
        ../TangoCommon/client/Client.h \
        ../TangoCommon/client/GameClient.h \
        ../TangoCommon/client/LocalClient.h \
        ../TangoCommon/client/RemoteClient.h \
        ../TangoCommon/component/ConfigSet.h \
        ../TangoCommon/network/json_rpc.h \
        ../TangoCommon/network/SocketX.h \
        ../TangoCommon/players/Author.h \
        ../TangoCommon/players/Consumer.h \
        ../TangoCommon/players/Player.h \
        ../TangoCommon/component/Logger.h \
        ../TangoCommon/component/MessageBox.h \
        ../TangoCommon/component/PairTableItem.h \
        ../TangoCommon/component/TimerWidget.h \
        ../TangoCommon/types/RetriveMode.h \
        ../TangoCommon/types/TangoPair.h \
        ../TangoCommon/types/UserBriefInfo.h \
        ../TangoCommon/types/UserFullInfo.h \
        ../TangoCommon/types/UserStatus.h \
        mainwindow.h \
        scene/ConfigScene.h \
        scene/LoginScene.h \
        scene/scene_common.h \
        scene/CreationScene.h \
        scene/MainScene.h \
        scene/MultiPlayingScene.h \
        scene/PlaySettleScene.h \
        scene/PlaySubScene.h \
        scene/PlayingScene.h \
        scene/QueryUsersScene.h \
        scene/RankingAuthorsScene.h \
        scene/RankingConsumersScene.h \
        scene/RegisterScene.h \
        scene/Scene.h \
        scene/SelectingScene.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    main.qrc
