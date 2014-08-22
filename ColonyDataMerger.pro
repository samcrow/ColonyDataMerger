#-------------------------------------------------
#
# Project created by QtCreator 2012-08-04T07:19:49
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = ColonyDataMerger
TEMPLATE = app


SOURCES += main.cpp\
		colonydatamerger.cpp \
	mobiledevice.cpp \
	consoledialog.cpp

HEADERS  += colonydatamerger.hpp \
	mobiledevice.hpp \
	consoledialog.hpp

FORMS    += colonydatamerger.ui \
	consoledialog.ui


INCLUDEPATH += /opt/local/include
DEPENDPATH += /opt/local/include
# Include libmtp
unix:!symbian|win32: LIBS += -L/opt/local/lib -lmtp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../colonyutilsDebug/release/ -lcolonyutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../colonyutilsDebug/debug/ -lcolonyutils
else:symbian: LIBS += -lcolonyutils
else:unix: LIBS += -L$$PWD/../colonyutilsDebug/ -lcolonyutils

INCLUDEPATH += $$PWD/../Colony-Utilties-cpp-Dynamic
DEPENDPATH += $$PWD/../Colony-Utilties-cpp-Dynamic

