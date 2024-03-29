include(../AstCtiShared/AstCtiShared.pri)
include(../QtSingleApplication/qtsingleapplication.pri)

TARGET = AstCtiClient
TEMPLATE = app
QT += webkitwidgets \
    xml \
    network
CONFIG += qt \
    warn_on

contains(QT_BUILD_PARTS, tools):CONFIG += uitools
else:DEFINES += QT_NO_UITOOLS

SOURCES += \
	main.cpp \
	aboutdialog.cpp \
	browserwindow.cpp \
	cticlientapplication.cpp \
	webview.cpp \
	loginwindow.cpp \
	compactwindow.cpp \
	passwordwindow.cpp \
	managerwindow.cpp \
	cticlientwindow.cpp \
	astctidefaultstyle.cpp \
	astctispeeddialwidget.cpp \
	astcticallwidget.cpp \
    astctitcpclient.cpp
HEADERS += \
	aboutdialog.h \
    coreconstants.h \
    browserwindow.h \
    cticlientapplication.h \
    webview.h \
	cticonfig.h \
    loginwindow.h \
    astcticommand.h \
    astctichannel.h \
    compactwindow.h \
    passwordwindow.h \
    managerwindow.h \
    cticlientwindow.h \
    astctidefaultstyle.h \
    astctispeeddialwidget.h \
    astcticallwidget.h \
    astctitcpclient.h
FORMS += \
    aboutdialog.ui \
    browserwindow.ui \
    loginwindow.ui \
    compactwindow.ui \
    passwordwindow.ui \
    managerwindow.ui \
    astctispeeddialwidget.ui \
    astcticallwidget.ui
RESOURCES += mainresources.qrc
TRANSLATIONS = AstCtiClient_en_US.ts \
	AstCtiClient_it_IT.ts

!equals($${PWD}, $${OUT_PWD}) {
    # Shadow building is enabled
    # Specify files for copying
	LICENSE_SOURCE = $${PWD}/LICENSE.txt
	build_pass:CONFIG(debug, debug|release) {
		LICENSE_DEST = $${OUT_PWD}/debug
	} else:build_pass {
		LICENSE_DEST = $${OUT_PWD}/release
	}

    # Replace '/' with '\' in Windows paths
    win32 {
        LICENSE_SOURCE = $${replace(LICENSE_SOURCE, /, \\)}
		LICENSE_DEST = $${replace(LICENSE_DEST, /, \\)}
	}

    # COPY_FILE is a variable that qmake automatically creates in Makefile
	COPY_LICENSE = $(COPY_FILE) $$quote($$LICENSE_SOURCE) $$quote($$LICENSE_DEST)

    QMAKE_PRE_LINK += $$COPY_LICENSE
}
