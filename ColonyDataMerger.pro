#-------------------------------------------------
#
# Project created by QtCreator 2012-08-04T07:19:49
#
#-------------------------------------------------

QT       += core gui

TARGET = ColonyDataMerger
TEMPLATE = app


SOURCES += main.cpp\
		colonydatamerger.cpp \
	mobiledevice.cpp

HEADERS  += colonydatamerger.hpp \
	mobiledevice.hpp

FORMS    += colonydatamerger.ui


INCLUDEPATH += /opt/local/include
DEPENDPATH += /opt/local/include
# Include libmtp
unix:!symbian|win32: LIBS += -L/opt/local/lib -lmtp
