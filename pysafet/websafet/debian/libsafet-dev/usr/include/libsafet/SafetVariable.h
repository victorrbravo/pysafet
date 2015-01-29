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
#ifndef SAFETVARIABLE_H_
#define SAFETVARIABLE_H_


#include <QtXml>
#include "SafetXmlObject.h"
#include "SafetDocument.h"



class SafetVariable: public SafetXmlObject {
	
	Q_OBJECT 
	Q_PROPERTY(QString id READ id WRITE setId)
	Q_PROPERTY(QString tokenlink READ tokenlink WRITE setTokenlink)
	Q_PROPERTY(QString scope READ scope WRITE setScope)
	Q_PROPERTY(QString type READ type WRITE setType)	
	Q_PROPERTY(QString config READ config WRITE setConfig)
	Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(QString groupby READ groupby WRITE setGroupby)
	Q_PROPERTY(QString documentsource READ documentsource WRITE setDocumentsource)
        Q_PROPERTY(QString description READ description WRITE setDescription)
        Q_PROPERTY(QString rolfield READ rolfield WRITE setRolfield)
        Q_PROPERTY(QString timestampfield READ timestampfield WRITE setTimestampfield)

        QDomDocument doc;
        QString _id, _tokenlink, _scope, _type, _config, _source, _documentsource, _description;
        QString _rolfield, _timestampfield;
        QString _groupby;



	QList<SafetDocument*> documentlist;
public:
	
	/**
	* Constructor de la clase.
	*/ 
	SafetVariable();
	
	virtual void addChild(SafetXmlObject* o);	
	
	QString id() const {return _id; };
	void setId(const QString& i) { _id = i; }
	QString tokenlink() const {return _tokenlink; };
	void setTokenlink(const QString& i) { _tokenlink = i; }
	QString scope() const {return _scope; };
	void setScope(const QString& i) { _scope = i; }
	QString type() const {return _type; };
	void setType(const QString& i) { _type = i; }
	QString config() const {return _config; };
	void setConfig(const QString& i) { _config = i; }
	QString source() const {return _source; };
	void setSource(const QString& i) { _source = i; }
    QString groupby() const {return _groupby; };
    void setGroupby(const QString& g) { _groupby = g; }

    QString documentsource() const {return _documentsource; };
	void setDocumentsource(const QString& i) { _documentsource = i; }
        QString description() const {return _description; };
        void setDescription(const QString& i) { _description = i; }
        QString rolfield() const {return _rolfield; };
        void setRolfield(const QString& r) { _rolfield = r; }
        QString timestampfield() const {return _timestampfield; };
        void setTimestampfield(const QString& t) { _timestampfield = t; }

	QString foo() { return "something foo"; }
	
        int getDocumentCount() { return documentlist.count(); }
        QList<SafetDocument*>& getDocuments() { return documentlist; }

/**
	* \brief Crea un archivo XML con el resultado del estado actual de objeto QSqlQuery. 
	* 
	* El contenido del archivo XML incluye el resultado de un objeto QSqlQuertL. Esto corresponde a un caso 
	* en el que se requiere aplicar firma electronica o estampillado de tiempo a un tipo de archivo que no
	* es de texto ni de datos. Los tipos de archivos que se categorizan son: archivo (file), SQL (resultado 
	* de una sentencia SQL) y pagina web. 
	* 
	* \param sentence  Sentencia SQL que se ejecutará la busqueda
	* \param value valor de la clave única que sirve para generar el documento XML
	* \param fieldno Número de campo de la clave única para la instrucción SQL
	* \param documentid (paramétro de salida ) id generado del documento
	* \return Documento XML correspondiente a los datos de la entrada, si ocurrió un error al generar el documento se devuelve una cadena vacía
	*/
	QString getXMLDocument(const QString &value, long fieldno, QString& documentid  ) const;

	/**
	* \brief Crea un archivo XML con el resultado del estado actual de objeto QSqlQuery. 
	* 
	* El contenido del archivo XML incluye el resultado de un objeto QSqlQuery. Esto corresponde a un caso 
	* en el que se requiere aplicar firma electronica o estampillado de tiempo a un tipo de archivo que no
	* es de texto ni de datos. Los tipos de archivos que se categorizan son: archivo (file), SQL (resultado 
	* de una sentencia SQL) y pagina web. 
	* 
	* \param query  estado del objeto de tipo QSqlQuery para posteriormente guardar en un 
	* contendor OpenXAdES
	* \param outputFileName cadena que representa el nombre del archivo XML de salida en el sistema de 
	* archivos
	* \return Retorna la cadena que contiene el documento  XML si se encontró el valor y no hubo problemas, y retorna una cadena VACÍA si no ocurrio un error
	*/
	QString createXMLFileFromQuery(const QSqlQuery& query, const QString& outputFileName) const;

				
};

#endif /*SAFETVARIABLE_H_*/
