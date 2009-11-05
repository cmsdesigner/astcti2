# -------------------------------------------------
# Project created by QtCreator 2008-12-27T09:05:04
# -------------------------------------------------
QT += sql \
    xml \
    network
QT -= gui
TARGET = AstCTIServer
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    clientmanager.cpp \
    logger.cpp \
    amiclient.cpp \
    argumentlist.cpp \
    ctiserverapplication.cpp \
    qastctioperator.cpp \
    qastctiseat.cpp \
    qastctiservice.cpp \
    qastctivariable.cpp \
    qastctiaction.cpp \
    qastctiservices.cpp \
    qastctiservicesoperators.cpp \
    qastctiservicesvariables.cpp \
    configurationchecker.cpp \
    coretcpserver.cpp \
    qastctioperators.cpp \
    qastcticall.cpp \
    qastctioperatorservices.cpp \
    qastctiactions.cpp
HEADERS += clientmanager.h \
    cticonfig.h \
    logger.h \
    amiclient.h \
    coreconstants.h \
    argumentlist.h \
    ctiserverapplication.h \
    qastctioperator.h \
    qastctiseat.h \
    qastctiservice.h \
    qastctivariable.h \
    qastctiaction.h \
    qastctiservices.h \
    qastctiservicesoperators.h \
    qastctiservicesvariables.h \
    configurationchecker.h \
    coretcpserver.h \
    qastctioperators.h \
    qastcticall.h \
    qastctioperatorservices.h \
    qastctiactions.h
