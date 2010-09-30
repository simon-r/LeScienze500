# -------------------------------------------------
# Project created by QtCreator 2010-09-06T09:21:32
# -------------------------------------------------
TARGET = LeScienze500
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
    querydata.cpp
HEADERS += lescienze500.h \
    querydb.h \
    queryresult.h \
    configls500.h \
    configura.h \
    previewarticolo.h \
    browsercopertine.h \
    lserrormessage.h \
    browsercopertinebase.h \
    querydata.h
FORMS += lescienze500.ui \
    configura.ui \
    previewarticolo.ui \
    browsercopertine.ui \
    lserrormessage.ui
LIBS += -lsqlite3
RESOURCES += icons.qrc \
    html.qrc \
    crystal.qrc \
    logo.qrc
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
