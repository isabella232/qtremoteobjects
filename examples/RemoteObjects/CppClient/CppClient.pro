QT       += core

REPC_REPLICA += TimeModel.rep
QT += remoteobjects

QT       -= gui

TARGET = CppClient
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

OTHER_FILES += \
    TimeModel.rep

target.path = $$[QT_INSTALL_EXAMPLES]/RemoteObjects/CppClient
INSTALLS += target