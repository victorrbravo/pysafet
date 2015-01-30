TEMPLATE = lib
INCLUDEPATH += /usr/include/libdigidoc \
      ../../src \
      /usr/include/graphviz

DEFINES += QT_DEBUG SAFET_NO_DBXML
QT += core \
    sql \
    network \
    svg \
    xml
#QT -= gui
HEADERS += graphvizplugin.h
SOURCES += graphvizplugin.cpp
LIBS += -L../../lib \
#        -ldigidoc \
        -lsafet \
	-lgraph \
	-lgvc

TARGET = $$qtLibraryTarget(pnp_graphviz)
CONFIG += plugin
target.path = /usr/lib/libsafet
# target.files = plugviz.so*
INSTALLS += target
