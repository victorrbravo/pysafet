TARGET = safet
TEMPLATE = lib

DEFINES += QT_DEBUG \
    SAFET_NO_DBXML

#DEFINES += SAFET_XML2
#DEFINES += SAFET_SMTP
#DEFINES += SAFET_GSOAP
DEFINES += SAFET_TAR
#DEFINES += SAFET_OPENSSL
#DEFINES += SAFET_DIGIDOC
#DEFINES += SAFET_DIGIDOCPP
#DEFINES += SAFET_ESMTP
DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += QT_NO_DEBUG

INCLUDEPATH += /usr/include \
    ../src \
    ../inflow

contains( DEFINES, SAFET_XML2 ) {
   message( "Configuring for xml2..." )
INCLUDEPATH +=    /usr/include/libxml2

}
contains( DEFINES, SAFET_GSOAP ) {
message( "Configuring for gsoap..." )
INCLUDEPATH +=   ../gsoap

}

contains( DEFINES, SAFET_DIGIDOC ) {
message( "Configuring for digidoc..." )
INCLUDEPATH +=  /usr/include/libdigidoc
INCLUDEPATH +=  /usr/local/include/libdigidoc
}
contains( DEFINES, SAFET_DIGIDOCPP ) {
message( "Configuring for digidocpp..." )
INCLUDEPATH += /usr/local/include/digidocpp/ \
            /home/vbravo/firmaxml/libfirmaxml/trunk \
              /home/vbravo/estonia/libdigidocpp/src/  \
              /home/vbravo/estonia/libdigidoc/libdigidoc \
	     /home/vbravo/estonia/libdigidoc/trunk/libdigidoc 
}


QT += core \
    sql \
    network \
#    webkit \
#    gui \
    svg \
    script \
    xml

# QT -= gui
HEADERS += ../src/SafetSQLParser.h \
    ../src/SafetParser.h \
    ../src/SafetNode.h \
    ../src/SafetToken.h \
    ../src/SafetConnection.h \
    ../src/SafetPort.h \
    ../src/SafetCondition.h \
    ../src/SafetTask.h \
    ../src/SafetVariable.h \
    ../src/SafetDocument.h \
    ../src/SafetWorkflow.h \
    ../src/SafetConfiguration.h \
    ../src/SafetXmlObject.h \
    ../src/SafetYAWL.h \
    ../src/SafetXmlRepository.h \
    ../src/SafetConfFile.h \
    ../src/SafetLog.h \
    ../src/SafetDirXmlRepository.h \
    ../src/SafetInterfaces.h \
    ../src/SafetStats.h \
    ../src/SafetPlugin.h \
    ../src/SafetAutofilter.h \
    ../src/SafetRecursiveFilter.h \
    ../src/SafetParameter.h \
    ../inflow/mainwindow.h \
    ../inflow/SafetTextParser.h \
    ../inflow/dommodel.h \
    ../inflow/cmdwidget.h \
    ../inflow/combowidget.h \
    ../inflow/datetimewidget.h \
    ../inflow/texteditwidget.h \
    ../inflow/getfilewidget.h  \
#    ../inflow/suggestwidget.h \
#    ../inflow/googlesuggest.h \
    ../inflow/numberwidget.h \
#    ../inflow/listwidget.h \
#    ../inflow/inflowfirmadoc.h \
    ../src/SafetBinaryRepo.h


contains( DEFINES, SAFET_SMTP ) {

message(" Configuring SMTP library..." )
SOURCES += \
    ../smtp/emailaddress.cpp \
    ../smtp/mimeattachment.cpp \
    ../smtp/mimefile.cpp \
    ../smtp/mimehtml.cpp \
    ../smtp/mimeinlinefile.cpp \
    ../smtp/mimemessage.cpp \
    ../smtp/mimepart.cpp \
    ../smtp/mimetext.cpp \
    ../smtp/smtpclient.cpp \
    ../smtp/quotedprintable.cpp \
    ../smtp/mimemultipart.cpp \
    ../smtp/mimecontentformatter.cpp 


HEADERS  += \
    ../smtp/emailaddress.h \
    ../smtp/mimeattachment.h \
    ../smtp/mimefile.h \
    ../smtp/mimehtml.h \
    ../smtp/mimeinlinefile.h \
    ../smtp/mimemessage.h \
    ../smtp/mimepart.h \
    ../smtp/mimetext.h \
    ../smtp/smtpclient.h \
    ../smtp/SmtpMime \
    ../smtp/quotedprintable.h \
    ../smtp/mimemultipart.h \
    ../smtp/mimecontentformatter.h

}



contains( DEFINES, SAFET_TAR ) {
HEADERS +=     ../src/libdotar.h

}

contains( DEFINES, SAFET_OPENSSL ) {
HEADERS += ../src/safetpkcs12.h \
    ../src/SafetCipherFile.h
 HEADERS +=     ../src/SslCertificate.h
}

SOURCES += ../src/SafetSQLParser.cpp \
    ../src/SafetParser.cpp \
    ../src/SafetNode.cpp \
    ../src/SafetToken.cpp \
    ../src/SafetConnection.cpp \
    ../src/SafetPort.cpp \
    ../src/SafetCondition.cpp \
    ../src/SafetTask.cpp \
    ../src/SafetYAWL.cpp \
    ../src/SafetVariable.cpp \
    ../src/SafetDocument.cpp \
    ../src/SafetWorkflow.cpp \
    ../src/SafetConfiguration.cpp \
    ../src/SafetXmlObject.cpp \
    ../src/SafetXmlRepository.cpp \
    ../src/SafetConfFile.cpp \
    ../src/SafetLog.cpp \
    ../src/SafetDirXmlRepository.cpp \
    ../src/SafetStats.cpp \
    ../src/SafetPlugin.cpp \
    ../src/SafetAutofilter.cpp \
    ../src/SafetRecursiveFilter.cpp \
    ../src/SafetParameter.cpp \
     ../inflow/mainwindow.cpp \ 
    ../inflow/SafetTextParser.cpp \
    ../inflow/dommodel.cpp \
    ../inflow/cmdwidget.cpp \
    ../inflow/combowidget.cpp \
    ../inflow/datetimewidget.cpp \
    ../inflow/texteditwidget.cpp \
    ../inflow/getfilewidget.cpp \
#    ../inflow/suggestwidget.cpp \
#    ../inflow/googlesuggest.cpp \
    ../inflow/numberwidget.cpp \
#    ../inflow/listwidget.cpp \
#    ../inflow/inflowfirmadoc.cpp \
    ../src/SafetBinaryRepo.cpp

contains( DEFINES, SAFET_OPENSSL ) {
SOURCES +=  ../src/safetpkcs12.cpp \
       ../src/SslCertificate.cpp \
      ../src/SafetCipherFile.cpp
}

contains( DEFINES, SAFET_TAR ) {
SOURCES +=  ../src/libdotar.c
}

contains( DEFINES, SAFET_GSOAP ) {
SOURCES += ../gsoap/stdsoap2.cpp \
    ../gsoap/soapC.cpp \
    ../gsoap/soapClient.cpp
}


LIBS += -L/usr/local/lib \
    -L../inflow
contains( DEFINES, SAFET_GRAPHVIZ ) {
LIBS += -L/usr/lib/graphviz
}
contains( DEFINES, SAFET_DIGIDOC ) {
LIBS +=  -ldigidoc
}

contains( DEFINES, SAFET_DIGIDOCPP ) {
LIBS += -L/usr/local/lib \
        -L/home/vbravo/firmaxml/libfirmaxml/trunk \
        -lfirmaxml -ldigidocpp
}

contains( DEFINES, SAFET_TAR ) {
    LIBS +=  -ldl
    LIBS +=  -ltar
}

contains( DEFINES, SAFET_XML2 ) {
LIBS +=  -lxml2
}

CONFIG += qt \
    ordered \
    thread \
    warn_off \
    release \
    staticlib
VERSION = 0.1.0
contains( DEFINES, SAFET_DBXML ) { 
    # DEFINES contains 'SAFET_DBXML'
    message( "Configuring for dbxml..." )
    HEADERS += SafetDbXmlRepository.h
    SOURCES += SafetDbXmlRepository.cpp
    INCLUDEPATH += /usr/local/dbxml/install/include \
        /usr/local/dbxml/install/include/dbxml
    LIBS += -L/usr/local/dbxml/install/lib \
        -ldbxml \
        -ldb \
        -ldb_cxx \
        -lxqilla
}



target.path = /usr/lib/libsafet
target.files = libsafet.so*
INSTALLS += target
