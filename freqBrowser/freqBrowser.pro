if (!include(config.txt)) {
    error("Failed to open config.txt")
}
TARGET=Freqbrowser
INCLUDEPATH += \
                            audio\
                            ../include/Win32xx\
                            ../include/Portaudio\
                            ../include/Kissfft



SOURCES += main.cpp \
    DialogApp.cpp \
    MyDialog.cpp \
    StdAfx.cpp \
    audio/Frequency.cpp \
    utfwstring.cpp \
    aboutdlg.cpp

HEADERS += \
    DialogApp.h \
    MyDialog.h \
    resource.h \
    stdafx.h \
    audio/Frequency.h \
    utfwstring.h \
    aboutdlg.h

LIBS += -L../lib/Portaudio\
              -L../lib/Kissfft

LIBS += -lportaudio\
              -lkissfft-float


win32:RC_FILE += Resource.rc
