TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp
win32{
options = $$find(QMAKESPEC, "msvc2015_64")
MYLIB=-L$$PWD/../../lib/win64
DESTDIR = "../../bin/win64"
lessThan(options, 1) {
    message("Win32 Lib")
    MYLIB=-L$$PWD/../../lib/win32
    DESTDIR = "../../bin/win32"
}
}
linux{
message("linux Lib")
MYLIB=-L/usr/local/ffmpeg/ -lavcodec
}
LIBS += $$MYLIB
INCLUDEPATH += $$PWD/../../include

#message($$QMAKESPEC)
