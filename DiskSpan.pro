######################################################################
# Automatically generated by qmake (3.0) Sat Feb 20 14:38:50 2016
######################################################################

TEMPLATE = app
TARGET = DiskSpan
INCLUDEPATH += .

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

# Input
HEADERS += src/MainWindow.h src/mulknap.h
FORMS += forms/mainwindow.ui
SOURCES += src/main.cpp src/MainWindow.cpp src/mulknap.c
RESOURCES += resources/application.qrc

QT += widgets