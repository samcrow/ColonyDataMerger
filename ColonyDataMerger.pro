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

win32:CONFIG(release, debug|release): LIBS += -L/opt/local/lib/release/ -lmtp
else:win32:CONFIG(debug, debug|release): LIBS += -L/opt/local/lib/debug/ -lmtp
else:symbian: LIBS += -lmtp
else:unix: LIBS += -L/opt/local/lib/ -lmtp

INCLUDEPATH += /opt/local/include
DEPENDPATH += /opt/local/include
