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
	mobiledevice.cpp \
	mtp-examples/pathutils.c

HEADERS  += colonydatamerger.hpp \
	mobiledevice.hpp \
	mtp-examples/pathutils.h \
	mtp-examples/common.h \
    mtp-examples/config.h

FORMS    += colonydatamerger.ui


INCLUDEPATH += /opt/local/include
DEPENDPATH += /opt/local/include
# Include libmtp
unix:!symbian|win32: LIBS += -L/opt/local/lib -lmtp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libcolonyutils-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Debug/release/ -llibcolonyutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libcolonyutils-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Debug/debug/ -llibcolonyutils
else:symbian: LIBS += -llibcolonyutils
else:unix: LIBS += -L$$PWD/../libcolonyutils-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Debug/ -llibcolonyutils

INCLUDEPATH += $$PWD/../libcolonyutils-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Debug
DEPENDPATH += $$PWD/../libcolonyutils-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Debug

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../libcolonyutils-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Debug/release/libcolonyutils.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../libcolonyutils-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Debug/debug/libcolonyutils.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../libcolonyutils-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Debug/liblibcolonyutils.a
