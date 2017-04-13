HEADERS += \
    app.h \
    redisapi.h

SOURCES += \
    app.cpp \
    redisapi.cpp
LIBS += -ltufao1

QT += network
CONFIG += C++11
QT += sql

LIBS += -L/usr/lib/i386-linux-gun -lhiredis
