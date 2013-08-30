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

win32:CONFIG(release, debug|release): LIBS += -L/opt/local/lib/ -lqjson
else:win32:CONFIG(debug, debug|release): LIBS += -L/opt/local/lib/ -lqjson
else:symbian: LIBS += -lqjson
else:unix: LIBS += -L$$PWD/libraries/lib/ -lqjson

INCLUDEPATH += /opt/local/include
DEPENDPATH += /opt/local/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../colonyutils-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Debug/release/ -lcolonyutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../colonyutils-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Debug/debug/ -lcolonyutils
else:symbian: LIBS += -lcolonyutils
else:unix: LIBS += -L$$PWD/../colonyutils-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Debug/ -lcolonyutils

INCLUDEPATH += $$PWD/../Colony-Utilties-cpp-Dynamic
DEPENDPATH += $$PWD/../Colony-Utilties-cpp-Dynamic

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../colonyutils-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Debug/release/colonyutils.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../colonyutils-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Debug/debug/colonyutils.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../colonyutils-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Debug/libcolonyutils.dylib
