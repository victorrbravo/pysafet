TARGET = inflow
TEMPLATE = app
INCLUDEPATH += /usr/include \
    /usr/include/libdigidoc \ # /usr/include/digidocpp \
    /usr/include/libxml2 \
    ../widgets/iistaskpanel/include \
    ../gsoap \
    ../tools \
    ../src
DEFINES += SAFET_NO_DBXML
QT += core \
    sql \
    xml \
    gui \
    network \
    webkit

# network
QT += svg
HEADERS += mainwindow.h \
    textedit.h \
    dommodel.h \
    domitem.h \
    getfilewidget.h \
    qcmdcompleter.h \
    highlighter.h \
    cmdwidget.h \
    combowidget.h \
    listwidget.h \
    suggestwidget.h \
    googlesuggest.h \
    optionsdialog.h \
    graphicsworkflow.h \
    sceneworkflow.h \
    texteditwidget.h \
    datetimewidget.h \
    numberwidget.h \
    graphicssvgitemlive.h \
    threadconsole.h \
    threadinfoserver.h \
    inflowfirmadoc.h \
    configdialog.h \
    pages.h \
    infloweditqlistwidget.h \
    docksbmenu.h \
    CertificateWidget.h \
    principalframe.h \
    SafetTextParser.h \
    SignatureDialog.h \ # DigiDoc.h \
    TreeWidget.h \
    assistant.h \
    ui/getsigndocumentdialog.h \
    ui/sendsigndocumentdialog.h \
    logindialog.h \
    QProgressIndicator.h \
    iconwidget.h \
    databaseconfigdialog.h \
    dialogflowparameters.h
SOURCES += main.cpp \
    mainwindow.cpp \
    textedit.cpp \
    dommodel.cpp \
    domitem.cpp \
    getfilewidget.cpp \
    suggestwidget.cpp \
    googlesuggest.cpp \
    qcmdcompleter.cpp \
    highlighter.cpp \
    cmdwidget.cpp \
    datetimewidget.cpp \
    numberwidget.cpp \
    listwidget.cpp \
    combowidget.cpp \
    optionsdialog.cpp \
    graphicsworkflow.cpp \
    sceneworkflow.cpp \
    texteditwidget.cpp \
    graphicssvgitemlive.cpp \
    threadconsole.cpp \
    threadinfoserver.cpp \
    inflowfirmadoc.cpp \
    configdialog.cpp \
    pages.cpp \
    infloweditqlistwidget.cpp \
    docksbmenu.cpp \
    CertificateWidget.cpp \
    principalframe.cpp \
    SignatureDialog.cpp \
    SafetTextParser.cpp \ # DigiDoc.cpp \
    TreeWidget.cpp \
    assistant.cpp \
    ui/getsigndocumentdialog.cpp \
    ui/sendsigndocumentdialog.cpp \
    logindialog.cpp \
    QProgressIndicator.cpp \
    iconwidget.cpp \
    databaseconfigdialog.cpp \
    dialogflowparameters.cpp
LIBS += -L../src \
    -ldigidoc \ # -ldigidocpp \
    -ltar \
    -lesmtp \
    -lsafet
CONFIG += qt \
    ordered \
    warn_off \
    debug
FORMS += ui/inflowopciones.ui \
    inflowfirmadoc.ui \
    principal.ui \
    ui/SignatureDialog.ui \
    ui/CertificateWidget.ui \
    ui/sendsigndocumentdialog.ui \
    ui/getsigndocumentdialog.ui \
    ui/logindialog.ui \
    dataBaseConfig.ui \
    dialogflowparameters.ui
man.path = /usr/share/man/man1
man.files = ../debian/inflow.1.gz
RESOURCES = inflow.qrc
target.path =  /usr/bin
target.files = inflow
INSTALLS = target
