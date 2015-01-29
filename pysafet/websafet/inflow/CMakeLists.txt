cmake_minimum_required( VERSION 2.6 )
project( inflow )

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )

include( InstallSettings )

if(PKCS11_MODULE)
    add_definitions(-DPKCS11_MODULE="${PKCS11_MODULE}")
endif(PKCS11_MODULE)

find_package( Ldap REQUIRED )
find_package( LibDigiDoc REQUIRED )
find_package( LibP11 REQUIRED )
find_package( OpenSSL REQUIRED )
find_package( OpenSSLCrypto REQUIRED )
find_package( Qt4 REQUIRED )
find_package( XercesC REQUIRED )
find_package( XSD REQUIRED )

set( QT_MIN_VERSION 4.4.0 )
set( QT_USE_QTNETWORK true )
set( QT_USE_QTWEBKIT true )
set( QT_USE_QTXML true )
set( QT_USE_QTSVG true )
set( QT_USE_QTSQL true )
set( QT_USE_QTCORE true )
set( QT_USE_QTGUI true )

include( ${QT_USE_FILE} )

SET( MOC_HEADERS
	mainwindow.h 
	textedit.h 
	dommodel.h 
	domitem.h 
	getfilewidget.h 
	qcmdcompleter.h 
	highlighter.h 
	cmdwidget.h 
	combowidget.h 
	optionsdialog.h 
	graphicsworkflow.h 
	sceneworkflow.h 
	texteditwidget.h 
	graphicssvgitemlive.h 
	threadconsole.h 
	threadinfoserver.h 
	inflowfirmadoc.h 
	configdialog.h 
	pages.h 
	infloweditqlistwidget.h 
	docksbmenu.h 
	principalframe.h 
	)
QT4_WRAP_UI( UI_HEADERS
	ui/inflowopciones.ui 
	inflowfirmadoc.ui
	principal.ui
	)

QT4_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS} )
SET( SOURCES
	${HEADERS}
	${MOC_SOURCES}
	main.cpp 
	mainwindow.cpp 
	textedit.cpp 
	dommodel.cpp 
	domitem.cpp 
	getfilewidget.cpp 
	qcmdcompleter.cpp 
	highlighter.cpp 
	cmdwidget.cpp 
	combowidget.cpp 
	optionsdialog.cpp 
	graphicsworkflow.cpp 
	sceneworkflow.cpp 
	texteditwidget.cpp 
	graphicssvgitemlive.cpp 
	threadconsole.cpp 
	threadinfoserver.cpp 
	inflowfirmadoc.cpp 
	configdialog.cpp 
	pages.cpp 
	infloweditqlistwidget.cpp 
	docksbmenu.cpp 
	principalframe.cpp 
	)

set(QM_DIR ${CMAKE_CURRENT_BINARY_DIR})

QT4_ADD_RESOURCES( RC_SOURCES
	${COMMON_RCS}
	inflow.qrc
	)

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${OPENSSL_INCLUDE_DIR}
	${XERCESC_INCLUDE_DIR}
	${XSD_INCLUDE_DIR}
	${LIBDIGIDOCPP_INCLUDE_DIR}
	${LIBP11_INCLUDE_DIR}
	)

include_directories(../src /usr/local/include/libdigidoc/)
link_directories(../src /usr/local/lib)
find_library(SAFET_LIBRARY safet)


if(NOT "${LIBDIGIDOCPP_CONF}" STREQUAL "LIBDIGIDOCPP_CONF-NOTFOUND")
	add_definitions( -DBDOCLIB_CONF_PATH="${LIBDIGIDOCPP_CONF}" )
endif(NOT "${LIBDIGIDOCPP_CONF}" STREQUAL "LIBDIGIDOCPP_CONF-NOTFOUND")

ADD_EXECUTABLE(
	inflow
	${SOURCES}
	${QM_FILES}
	${RC_SOURCES}
	${RESOURCE_FILES}
	)


#install(TARGETS inflow DESTINATION ${BIN_INSTALL_DIR})


