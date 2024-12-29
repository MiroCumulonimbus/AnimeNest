QT       += core gui charts network xml concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    animedatamanger.cpp \
    datafile/CustomAlgorithms.cpp \
    datafile/datadefine.cpp \
    extension/networkfetch.cpp \
    main.cpp \
    animenestwindow.cpp \
    myobject.cpp \
    src/Initialization.cpp \
    src/base_add.cpp \
    src/base_delete.cpp \
    src/base_event.cpp \
    src/base_find.cpp \
    src/base_rightclick.cpp \
    src/base_update.cpp \
    src/tableoperate.cpp \
    tools/detailData.cpp

HEADERS += \
    animedatamanger.h \
    animenestwindow.h \
    datafile/CustomAlgorithms.h \
    datafile/datadefine.h \
    extension/networkfetch.h \
    myobject.h

FORMS += \
    animenestwindow.ui

RC_ICONS += icon/AnimeNest_V1.0.ico

INCLUDEPATH += $$PWD/datafile
INCLUDEPATH += $$PWD/extension
INCLUDEPATH += $$PWD/JsonFiles
INCLUDEPATH += $$PWD/icon

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
