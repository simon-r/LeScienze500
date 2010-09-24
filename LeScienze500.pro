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
    previewarticolo.cpp
HEADERS += lescienze500.h \
    querydb.h \
    queryresult.h \
    configls500.h \
    configura.h \
    previewarticolo.h
FORMS += lescienze500.ui \
    configura.ui \
    previewarticolo.ui
LIBS += -lsqlite3
RESOURCES += icons.qrc
target.path = /usr/bin
desktop.path = /usr/share/applications/
desktop.files = lescienze500.desktop
icons.path = /usr/share/icons/hicolor/72x72/apps/
icons.files = icons/lescienze.png
INSTALLS += target
INSTALLS += desktop
INSTALLS += icons
