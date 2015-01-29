/*
* SAFET Sistema Automatizado para la Firma Electrónica y Estampado de Tiempo
* Copyright (C) 2008 Víctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigación en Tecnologías Libres 
*
* Este programa es software libre; Usted puede usarlo bajo los términos de la licencia de 
* software GPL versión 2.0 de la Free Software Foundation. 
*
* Este programa se distribuye con la esperanza de que sea útil, pero SI NINGUNA GARANTÍA;
* tampoco las implícitas garantías de MERCANTILIDAD o ADECUACIÓN A UN PROPÓSITO PARTICULAR.
* Consulte la licencia GPL para más detalles. Usted debe recibir una copia de la GPL junto 
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5º Piso, Boston, MA 02110-1301, USA.
*
*/


#ifndef SAFETXMLREPOSITORY_H_
#define SAFETXMLREPOSITORY_H_


#include <QString>
#include <QStringList>
#include <QtGlobal>
#include <QStringList>
#include <QFile>
#include <QObject>
#include <QtXml>


#include "db_cxx.h"
#include "DbXml.hpp"

#include "SafetDocument.h"
#include "SafetXmlRepository.h"
using namespace DbXml;

/**
* \brief Clase SafetXmlRepository
* 
* Esta Clase corresponde al objeto repositorio de sobres (contenedores) generados con la librería digidoc.
*
* SafetXmlRepository implimenta un repositorio de sobres digidoc utilizando la libreria libdbxml. Se utiliza esta
* libreria por las capacidades de almacenamiento y gestión de documentos en formato XML como los archivos digidoc (.ddoc).
*
* 
*/


class SafetDbXmlRepository : public SafetXmlRepository {

	Q_OBJECT

protected:

	/**
	* \brief Puntero a estructura de la base de datos de ambiente de libdbxml.
	* 
	*/
	DbEnv * myEnv;


	/**
	* \brief Puntero a estructura de un objeto XmlManager que gestiona distintos objetos de Berkeley DB XML
	* 
	*/
	XmlManager * myManager;
	XmlContainer container;
	XmlUpdateContext theContext;
	XmlData content;
	XmlDocument document;


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

	//enum DocumentStatus {SAFET_DOCUMENT_NOT_FOUND, SAFET_DOCUMENT_SIGNED, SAFET_DOCUMENT_NOT_SIGNED, SAFET_DOCUMENT_SIGNATURE_VALID, SAFET_DOCUMENT_SIGNATURE_INVALID};


	/**
	* Constructor de la clase.
	* \param env puntero a una estructura de base de datos dbxml
	* \param manager puntero a un objeto manager
	* \param home cadena con la ruta absoluta donde se almacena el ambiente de la base de datos dbxml
	*/ 
	SafetDbXmlRepository(const QString& home);

	/**
	* Destructor de la clase.
	*/ 
	virtual ~SafetDbXmlRepository();

	/**
	* Retorna puntero al objeto XmlManager 
	* \return Retorna puntero al objeto XmlManager
	*/ 
	XmlManager * getXmlManager();


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
	bool createContainer(const QString& containerName, const QString& containerType);


	/**
	* Abre un contendor existente.
	* \param containerName ruta absoluta del nombre del contenedor de documentos XML
	*/
	bool openContainer(const QString& containerName);

	/**
	* Elimina un contendor existente.
	* \param containerName ruta absoluta del nombre del contenedor de documentos XML
	*/ 
	void deleteContainer(const QString& containerName);

	/**
	* Renombra un contendor existente.
	* \param containerName ruta absoluta del nombre del contenedor de documentos XML
	* \param newContainerName ruta absoluta del nuevo nombre del contenedor de documentos XML
	*/ 
	void renameContainer(const QString& containerName, const QString& newContainerName);



	/**
	* Crea un contexto de actualizacion para realizar operaciones sobre un contenedor
	* 
	*/
	XmlUpdateContext createContextUpdate();



	/**
	* Agregar un documento XML a contenedor
	*
	* \param physicalName ruta absoluta del nombre del documentos XML a agregar
	* \param docName identificador/nombre del documento XML
	*
	*/
	void addXmlDocumentToContainer(const QString& physicalName, const QString& docName);


	/**
	* Retorna un documento XML de contenedor
	*
	* \param docName identificador/nombre del documento XML
	* \param container objeto contenedor en el cual se encuentra almacenado el documento XML
	*/
	QString getXmlDocument(const QString& docName);
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
	QString writeXmlFileFromDb(const QString& docName);

	/**
	* Busca un documento dentro de un contenedor especifico
	*
	* \param docName identificador/nombre del documento XML a buscar
	* \param container objeto contenedor en el cual se encuentra almacenado el documento XML
	*
	* \return true si el documento se encontro en el contenedor
	*/
	bool searchDocumentInContainer(const QString& docName);



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
	*addXmlDocumentToContainer
	* \param docName identificador/nombre del documento XML a buscar
	* \param CN nombre comun del certificado firmante del documento
	* \param container contenedor sobre el cual se encuentra el documento
	*
	* \return true si el documento se encontro en el contenedor
	*/
	int getSignatureStatus(const QString& docName, const QString& CN);


	/**
	* Función para eliminar un documento XML de un contendor
	*
	* \param docName identificador/nombre del documento XML a borrar
	* \param container objeto contenedor en el cual se encuentra almacenado el documento XML
	*/
	bool deleteXmlDocument(const QString& docName);

};





















#endif /*SAFETXMLREPOSITORY_H_*/

