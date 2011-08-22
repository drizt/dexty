#-------------------------------------------------
#
# Project created by QtCreator 2010-06-03T16:00:28
#
#-------------------------------------------------

QT       += xml

TARGET = dexty
TEMPLATE = app

SOURCES += main.cpp         \
           mainwindow.cpp   \
           type.cpp         \
           action.cpp       \
           resultdlg.cpp

HEADERS  += mainwindow.h    \
            type.h          \
            action.h        \
            resultdlg.h

FORMS    += mainwindow.ui   \
            resultdlg.ui

RESOURCES += resources.qrc

INCLUDEPATH += $$_PRO_FILE_PWD_

unix{
    CONFIG += debug
    SYSTEM = unix
}

win32{
    CONFIG += release
    SYSTEM = win32
}

OBJECTS_DIR = build/$$SYSTEM
MOC_DIR = build/$$SYSTEM
RCC_DIR = build/$$SYSTEM
UI_DIR = build/$$SYSTEM
