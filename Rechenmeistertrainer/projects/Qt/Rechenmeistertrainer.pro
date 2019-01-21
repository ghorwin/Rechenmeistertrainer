#-------------------------------------------------
#
# Project requires Qt 5.10
#
#-------------------------------------------------

QT += widgets core gui multimedia

TARGET = Rechenmeistertrainer
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

# check if 32 or 64 bit version and set prefix variable for using in output paths
greaterThan(QT_MAJOR_VERSION, 4) {
	contains(QT_ARCH, i386): {
		DIR_PREFIX =
	} else {
		DIR_PREFIX = _x64
	}
} else {
	DIR_PREFIX =
}

CONFIG(debug, debug|release) {
	OBJECTS_DIR = debug$${DIR_PREFIX}
	DESTDIR = ../../../bin/debug$${DIR_PREFIX}
}
else {
	OBJECTS_DIR = release$${DIR_PREFIX}
	DESTDIR = ../../../bin/release$${DIR_PREFIX}
}

MOC_DIR = moc
UI_DIR = ui

win32-msvc* {
	QMAKE_CXXFLAGS += /wd4996
	QMAKE_CFLAGS += /wd4996
}
else {
	QMAKE_CXXFLAGS += -std=c++11
}

QMAKE_LIBDIR += ../../../externals/lib$${DIR_PREFIX}
LIBS += -L../../../externals/lib$${DIR_PREFIX}


INCLUDEPATH += ../../../externals/qwt/src
LIBS += -L../../../externals/ -lqwt6

SOURCES += \
	../../src/main.cpp \
	../../src/MainDialog.cpp \
	../../src/EinMalEinsDialog.cpp

HEADERS += \
	../../src/MainDialog.h \
	../../src/EinMalEinsDialog.h

FORMS += \
../../src/MainDialog.ui \
../../src/EinMalEinsDialog.ui


RESOURCES += \
	../../resources/Rechenmeistertrainer.qrc
