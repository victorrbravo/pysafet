/*
* SAFET Sistema Automatizado para la Firma Electrónica y Estampado de Tiempo
* Copyright (C) 2008 Víctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigación en TecnologÃ­as Libres
*
* Este programa es software libre; Usted puede usarlo bajo los términos de la licencia de
* software GPL versión 2.0 de la Free Software Foundation.
*
* Este programa se distribuye con la esperanza de que sea útil, pero SI NINGUNA GARANTÍA;
* tampoco las implí­citas garantí­as de MERCANTILIDAD o ADECUACIÃ“N A UN PROPÓSITO PARTICULAR.
* Consulte la licencia GPL para más detalles. Usted debe recibir una copia de la GPL junto
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5º Piso, Boston, MA 02110-1301, USA.
*
*/
//
// C++ Interface: SafetXmlRepository
//
// Description: 
//
//
// Author: VÃ­ctor R. Bravo, Antonio Araujo Brett <vbravo@cenditel.gob.ve,aaraujo@cenditel.gob.ve>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SAFETXMLREPOSITORY_H
#define SAFETXMLREPOSITORY_H

#include <QObject>
#include "SafetLog.h"
/**
	@author VÃ­ctor R. Bravo, Antonio Araujo Brett <vbravo@cenditel.gob.ve,aaraujo@cenditel.gob.ve>
*/
class SafetXmlRepository : public QObject
{
Q_OBJECT

protected:
        bool _isopencontainer;
        bool _firstsearch;
public:

	/**
	* \brief Enumerado que define el estado de una firma de un documento.
	* 
	* Los estados definidos hasta el momento para una firma son los siguientes:
	*
	*   SAFET_DOCUMENT_NOT_FOUND -> el documento no se encuentra en el contenedor
	*   SAFET_DOCUMENT_SIGNED -> el documento se encuentra firmado por el firmante
	*   SAFET_DOCUMENT_NOT_SIGNED -> el documento NO se encuentra firmado por el firmante
	*   SAFET_DOCUMENT_SIGNATURE_VALID -> el documento posee una firma valida del firmante
	*   SAFET_DOCUMENT_SIGNATURE_INVALID -> el documento posee una firma invalida del firmante
	*/

	enum DocumentStatus {SAFET_DOCUMENT_NOT_FOUND, SAFET_DOCUMENT_SIGNED, SAFET_DOCUMENT_NOT_SIGNED, SAFET_DOCUMENT_SIGNATURE_VALID, SAFET_DOCUMENT_SIGNATURE_INVALID};


	/**
	* Constructor de la clase.
	*
	*/
    	SafetXmlRepository(QObject *parent = 0);

	    ~SafetXmlRepository();
	/**
	* Asigna la cadena de la ruta donde se encuentra el home del ambiente dbxml.
	* \param home cadena con la ruta absoluta donde se almacena el ambiente de la base de datos dbxml
	*/ 
	virtual void setEnvHome(const QString& home);
	
	/**
	* Retorna la cadena de la ruta donde se encuentra el home del ambiente dbxml.
	* \return cadena con la ruta absoluta donde se almacena el ambiente de la base de datos dbxml
	*/ 
	QString getEnvHome();
	
	

	/**
	* Crea un contenedor que permite almacenar documentos XML
	* 
	* Un contenedor es un archivo en disco que contiene los datos asociados con los documentos XML, incluyendo metadatos e indices
	*
	* Los tipos de contenedores son: 
	* XmlContainer::WholedocContainer-> en el caso en que el contenedor va a almacenar el documento XML completo
	* 
	* XmlContainer::NodeContainer -> en el caso en que el contenedor se almacenan los documentos XML como nodos individuales, aqui 
	* cada registro de la base de datos contiene un unico nodo hoja, sus atributos, valores de atributos y sus nodos texto en caso de tenerlo.
	* En este caso dbxml posee suficiente informacion para reensamblar el documento XML a partir de los nodos individuales
	*
	* \param containerName ruta absoluta del nombre del contenedor de documentos XML.
	* \param containerType cadena con el tipo de contenedor. Debe ser uno de los tipos mencionados.
	*
	* \return contenedor creado
	*/
	virtual bool createContainer(const QString& containerName, const QString& containerType) = 0;
	
	
	/**
	* Abre un contendor existente.
	* \param containerName ruta absoluta del nombre del contenedor de documentos XML
	*/
	virtual bool openContainer(const QString& containerName) = 0;
	
	
	/**
	* Elimina un contendor existente.
	* \param containerName ruta absoluta del nombre del contenedor de documentos XML
	*/ 
	virtual void deleteContainer(const QString& containerName) = 0;
	
	/**
	* Renombra un contendor existente.
	* \param containerName ruta absoluta del nombre del contenedor de documentos XML
	* \param newContainerName ruta absoluta del nuevo nombre del contenedor de documentos XML
	*/ 
	virtual void renameContainer(const QString& containerName, const QString& newContainerName) = 0;
	
		
	
	/**
	* Agregar un documento XML a contenedor
	*
	* \param physicalName ruta absoluta del nombre del documentos XML a agregar
	* \param docName identificador/nombre del documento XML
	*
	*/
	virtual void addXmlDocumentToContainer(const QString& physicalName, const QString& docName) = 0;
	
	
	/**
	* Retorna un documento XML de contenedor
	*
	* \param docName identificador/nombre del documento XML
	* \param container objeto contenedor en el cual se encuentra almacenado el documento XML
	*/
	virtual QString getXmlDocument(const QString& docName) = 0;
	
	
	/**
	* Escribe un archivo XML de un contenedor en el sistema de archivos.
	*
	* Un contenedor posee documentos. Estos documentos corresponden a archivos XML, esta
	* funcion se encarga de obtener el archivo XML asociado a un documento dentro de un contenedor
	* del repositorio.
	*
	* \param docName identificador/nombre del documento XML dentro del contenedor
	* \param content objeto XmlData asociado al contenido de un documento dentro del contenedor
	*/
	virtual QString writeXmlFileFromDb(const QString& docName) = 0;
	
	
	/**
	* Busca un documento dentro de un contenedor especifico
	*
	* \param docName identificador/nombre del documento XML a buscar
	* \param container objeto contenedor en el cual se encuentra almacenado el documento XML
	*
	* \return true si el documento se encontro en el contenedor
	*/
	virtual bool searchDocumentInContainer(const QString& docName) = 0;
	
	
	
	/**
	* Funcion que retorna el estado de una firma de un documento digidoc dentro de un contenedor
	*
	* Los estados definidos hasta el momento para una firma son los siguientes:
	*
	*   SAFET_DOCUMENT_NOT_FOUND -> el documento no se encuentra en el contenedor
	*   SAFET_DOCUMENT_SIGNED -> el documento se encuentra firmado por el firmante
	*   SAFET_DOCUMENT_NOT_SIGNED -> el documento NO se encuentra firmado por el firmante
	*   SAFET_DOCUMENT_SIGNATURE_VALID -> el documento posee una firma valida del firmante
	*   SAFET_DOCUMENT_SIGNATURE_INVALID -> el documento posee una firma invalida del firmante
	*
	* \param docName identificador/nombre del documento XML a buscar
	* \param CN nombre comun del certificado firmante del documento
	* \param container contenedor sobre el cual se encuentra el documento
	*
	* \return true si el documento se encontro en el contenedor
	*/
	virtual int getSignatureStatus(const QString& docName, const QString& CN) = 0;
	
	
	/**
	* FunciÃ³n para eliminar un documento XML de un contendor
	*
	* \param docName identificador/nombre del documento XML a borrar
	* \param container objeto contenedor en el cual se encuentra almacenado el documento XML
	*/
	virtual bool deleteXmlDocument(const QString& docName) = 0;

	/**
	* \brief Cadena que almacena la ruta (home) del ambiente dbxml
	* 
	*/
	QString envHome;

        bool isOpenContainer() const { return _isopencontainer; }

};

#endif
