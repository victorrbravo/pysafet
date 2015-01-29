TEMPLATE = subdirs
SUBDIRS = src \    
    lib \
#    widgets \
    plugins/plugviz 
#    inflow 
#    utils \
#    tools
# DEFINES = QT_DEBUG
QT += core \
    sql \
    xml
QT -= gui
CONFIG += qt \
 ordered \
 thread \
 warn_off \
 debug_and_release \

# Mensaje
message( "Configuración y compilación de SAFET" )

# Libreria Libdigidoc
#!exists( /usr/include/libdigidoc/DigiDocLib.h ) {
#       error(" ... Ocurrió un error en la búsqueda de encabezados de Libdigidoc. Por favor instale la librería Libdigidoc")
#}
#message( "... Verificación de encabezados de Libdigidoc: OK" )

#!exists( /usr/lib/libdigidoc.so ) {
#       error(" ... Ocurrió un error en la búsqueda de la librería Libdigidoc. Por favor instale la librería Libdigidoc")
#}
message( "... Verificación de librería Libdigidoc: OK" )

# Libreria Libdigidocpp
#!exists( /usr/include/digidocpp/DDoc.h ) {
#       error(" ... Ocurrió un error en la búsqueda de encabezados de Libdigidocpp. Por favor instale la librería Libdigidocpp")
#}
#message( "... Verificación de encabezados de Libdigidocpp: OK" )

#!exists( /usr/lib/libdigidocpp.so.0 ) {
#       error(" ... Ocurrió un error en la búsqueda de la librería Libdigidocpp. Por favor instale la librería Libdigidocpp")
#}
#message( "... Verificación de librería Libdigidocpp: OK" )


# Libreria Libxml2
#!exists( /usr/include/libxml2/libxml/xmlreader.h ) {
#       error(" ... Ocurrió un error en la búsqueda de encabezados de Libxml2. Por favor instale la librería Libxml2")
#}
#message( "... Verificación de encabezados de Libxml2: OK" )

#!exists( /usr/lib/libxml2.so ) {
#       error(" ... Ocurrió un error en la búsqueda de la librería Libxml2. Por favor instale la librería Libxml")
#}
#message( "... Verificación de librería Libxml2: OK" )

# Libreria Libxslt1
#!exists( /usr/include/libexslt/exslt.h ) {
#       error(" ... Ocurrió un error en la búsqueda de encabezados de Libxslt1. Por favor instale la librería Libxslt1")
#}
#message( "... Verificación de encabezados de Libxslt1: OK" )

#!exists( /usr/lib/libexslt.so.0 ) {
#       error(" ... Ocurrió un error en la búsqueda de la librería Libxslt1.1. Por favor instale la librería Libxslt1.1")
#}
#message( "... Verificación de librería Libxslt1.1: OK" )

# Libreria Libgstreamer0.10
#!exists( /usr/include/gstreamer-0.10/gst/gst.h ) {
#       error(" ... Ocurrió un error en la búsqueda de encabezados de Libgstreamer0.10. Por favor instale la librería Libgstreamer0.10")
#}
message( "... Verificación de encabezados de Libgstreamer0.10: OK" )

#!exists( /usr/lib/libgstreamer-0.10.so ) {
#       error(" ... Ocurrió un error en la búsqueda de la librería Libgstreamer0.10. Por favor instale la librería Libgstreamer0.10")
#}
#message( "... Verificación de librería Libgstreamer0.10: OK" )

# Libreria Libglib2.0-dev
#!exists( /usr/lib/glib-2.0/include/glibconfig.h ) {
#       error(" ... Ocurrió un error en la búsqueda de encabezados de Libglib2.0-dev. Por favor instale la librería Libglib2.0-dev")
#}
#message( "... Verificación de encabezados de Libglib2.0: OK" )

#!exists( /usr/lib/libglib-2.0.so* ) {
#       error(" ... Ocurrió un error en la búsqueda de la librería Libglib2.0. Por favor instale la librería Libglib2.0")
#}
message( "... Verificación de librería Libglib2.0: OK" )


# Libreria Libgraphviz
!exists( /usr/include/graphviz/gvc.h ) {
       error(" ... Ocurrió un error en la búsqueda de encabezados de Libgraphviz. Por favor instale la librería Libgraphviz")
}
message( "... Verificación de encabezados de Libgraphviz: OK" )

!exists( /usr/lib/libgvc.so ) {
       error(" ... Ocurrió un error en la búsqueda de la librería Libgraphviz. Por favor instale la librería Libgraphviz")
}
message( "... Verificación de librería Libgraphviz: OK" )


# Libreria x11proto-xext-dev
# !exists( /usr/include/X11/extensions/shape.h ) {
#       error(" ... Ocurrió un error en la búsqueda de encabezados de x11proto-xext-dev. Por favor instale la librería x11proto-xext-dev")
#}
message( "... Verificación de encabezados de x11proto-xext-dev: OK" )

# Libreria libtar-dev
!exists( /usr/include/libtar.h ) {
       error(" ... Ocurrió un error en la búsqueda de encabezados de libtar-dev. Por favor instale la librería libtar-dev")
}
!exists( /usr/lib/libtar.a ) {
       error(" ... Ocurrió un error en la búsqueda de encabezados de libtar-dev. Por favor instale la librería libtar-dev")
}
message( "... Verificación de encabezados de libtar-dev: OK" )

# Compilador g++
!exists( /usr/bin/g++ ) {
       error(" ... Ocurrió un error en la búsqueda del compilador g++. Por favor instale el compilador g++")
}
message( "... Verificación de compilador g++ : OK" )



