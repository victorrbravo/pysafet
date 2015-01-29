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

#ifndef SAFETDIRREPOSITORY_H
#define SAFETDIRREPOSITORY_H

#include "SafetXmlRepository.h"
#include "SafetYAWL.h"
#include "SafetDocument.h"

#include <QDir>
#include <QFile>

/**
	@author Víctor R. Bravo, Antonio Araujo Brett <vbravo@cenditel.gob.ve,aaraujo@cenditel.gob.ve>
*/

class SafetDirXmlRepository : public SafetXmlRepository
{

protected:

	/**
	* Cadena que mantiene el nombre del contenedor.
	* 
	*/
	QString internalContainerName;
        QMap<QString,bool> _documentsmap;

public:
	/**
	* Constructor de la clase.
	* 
	*/
	SafetDirXmlRepository();

	/**
	* Destructor de la clase.
	* 
	*/
	~SafetDirXmlRepository();

	/**
	* Crea un contendor
	* \param containerName ruta absoluta del nombre del contenedor de documentos XML
	* \param containerType tipo de contenedor
	* \return Verdadero si el contenedor se creo exitosamente
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
	* Agregar un documento XML a contenedor
	*
	* \param physicalName ruta absoluta del nombre del documento XML a agregar
	* \param docName identificador/nombre del documento XML
	*
	*/
	void addXmlDocumentToContainer(const QString& physicalName, const QString& docName);
	
	
	/**
	* Retorna un documento XML de contenedor
	*
	* \param docName identificador/nombre del documento XML
	* 
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
	* 
	*
	*/
	QString writeXmlFileFromDb(const QString& docName);
	
	/**
	* Busca un documento dentro de un contenedor especifico
	*
	* \param docName identificador/nombre del documento XML a buscar
	* 
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
	*
	* \param docName identificador/nombre del documento XML a buscar
	* \param CN nombre comun del certificado firmante del documento
	* 
	*
	* \return true si el documento se encontro en el contenedor
	*/
	int getSignatureStatus(const QString& docName, const QString& CN);
	
	
	/**
	* Función para eliminar un documento XML de un contendor
	*
	* \param docName identificador/nombre del documento XML a borrar
	* 
	*/
	bool deleteXmlDocument(const QString& docName);

};

#endif
