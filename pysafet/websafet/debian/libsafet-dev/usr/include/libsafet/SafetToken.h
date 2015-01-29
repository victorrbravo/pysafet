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

#ifndef SAFETTOKEN_H_
#define SAFETTOKEN_H_

#include "SafetXmlObject.h"

class SafetToken : public SafetXmlObject {
	
	
	Q_OBJECT
	Q_PROPERTY(QString key READ key WRITE setKey)
	Q_PROPERTY(QString keysource READ keysource WRITE setKeysource)
    Q_PROPERTY(QString type READ type WRITE setType)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString config READ description WRITE setConfig)   
    QString _key,_keysource,_type,_description,_config;
	
public:
	SafetToken();
	QString key() const {return _key; };
	void setKey(const QString& i) { _key = i; }
	QString keysource() const {return _keysource; };
	void setKeysource(const QString& i) { _keysource = i; }
	QString type() const {return _type; };
	void setType(const QString& i) { _type = i; }
	QString description() const {return _config; };
	void setDescription(const QString& i) { _config = i; }
	QString config() const {return _config; };
	void setConfig(const QString& i) { _config = i; }
	
};

#endif /*SAFETTOKEN_H_*/
