TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH += include


SOURCES +=  test.c \
            source/adBits.c \
            source/adbmp.c \
            source/adcil.c \
            source/adbuffer.c \
            source/adpalette.c
            


HEADERS +=  include/adBits.h \
            include/adbmp.h \
            include/adbuffer.h \
            include/adcil.h \
            include/adcil_types.h \
            include/adpalette.h \
            include/adPlatform.h
            
