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



#ifndef SAFETCONFIGURATION_H_
#define SAFETCONFIGURATION_H_

#include "SafetXmlObject.h"
#include "SafetConfFile.h"
#include <QtSql>

class SafetConfiguration   : public SafetXmlObject {

	Q_OBJECT
	Q_PROPERTY(QString id READ id WRITE setId)
	Q_PROPERTY(QString connectstring READ connectstring WRITE setConnectstring)
	Q_PROPERTY(QString hostname READ hostname WRITE setHostname)
	Q_PROPERTY(QString databasename READ databasename WRITE setDatabasename)
	Q_PROPERTY(QString username READ username WRITE setUsername)
	Q_PROPERTY(QString driver READ driver WRITE setDriver)
	
	
	QString _id, _connectstring, _hostname, _databasename, _username, _driver;
	QSqlDatabase _db;    
public:
	SafetConfiguration();
	QString id() const {return _id; };
	void setId(const QString& i) { _id = i; }
	QString connectstring() const {return _connectstring; };
	void setConnectstring(const QString& i) { _connectstring = i; }
	
	QString hostname() const {return _hostname; };
	void setHostname(const QString& h) { _hostname = h; }
	
	QString databasename() const {return _databasename; };
	void setDatabasename(const QString& db) { _databasename = db; }
	
	QString username() const {return _username; };
	void setUsername(const QString& us) { _username = us; }
	
	QString driver() const {return _driver; };
	void setDriver(const QString& d) { _driver = d; }

	void open();
    virtual void addChild(SafetXmlObject* o);
    

public slots:

signals:

private:

};

#endif /*SAFETCONFIGURATION_H_*/
