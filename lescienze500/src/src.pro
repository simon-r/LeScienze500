# -------------------------------------------------
# Project created by QtCreator 2010-09-06T09:21:32
# -------------------------------------------------
TARGET = ../LeScienze500
TEMPLATE = app

SOURCES += main.cpp \
    lescienze500.cpp \
    querydb.cpp \
    queryresult.cpp \
    configls500.cpp \
    configura.cpp \
    querydb_mainq.cpp \
    previewarticolo.cpp \
    browsercopertine.cpp \
    lserrormessage.cpp \
    browsercopertinebase.cpp \
    querydata.cpp \
    bookmark.cpp \
    bookmarkgui.cpp \
    about.cpp \
    setname.cpp \
    startpdf.cpp
HEADERS += lescienze500.h \
    querydb.h \
    queryresult.h \
    configls500.h \
    configura.h \
    previewarticolo.h \
    browsercopertine.h \
    lserrormessage.h \
    browsercopertinebase.h \
    querydata.h \
    bookmark.h \
    bookmarkgui.h \
    about.h \
    setname.h \
    startpdf.h

FORMS += lescienze500.ui \
    configura.ui \
    previewarticolo.ui \
    browsercopertine.ui \
    lserrormessage.ui \
    bookmarkgui.ui \
    about.ui \
    setname.ui

win32{
    SOURCES += sqlite/sqlite3.c
    INCLUDEPATH += sqlite
 #   LIBS += winmm.lib
    RC_FILE += myapp_ico.rc
    #QTPLUGIN += qjpeg
}

unix{
    LIBS += -lsqlite3
}

RESOURCES += icons.qrc \
    html.qrc \
    crystal.qrc \
    sql.qrc
OBJECTS_DIR = tmp_files
MOC_DIR = tmp_files
target.path = /usr/bin
icons.path = /usr/share/icons/hicolor/
icons.files = icons/*
desktop.path = /usr/share/applications/
desktop.files = lescienze500.desktop
INSTALLS += target
INSTALLS += desktop
INSTALLS += icons
