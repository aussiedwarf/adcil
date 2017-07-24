TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


SOURCES +=  test.c \
            source/adcil.c \
            source/adbmp.c
            


HEADERS +=  adcil.h \
            source/adcil_types.h \
            source/adbmp.h
            
