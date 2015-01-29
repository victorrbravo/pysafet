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


#ifndef SAFETPORT_H_
#define SAFETPORT_H_

#include "SafetXmlObject.h"
#include "SafetConnection.h"

class SafetPort : public SafetXmlObject {

	Q_OBJECT
	Q_PROPERTY(QString config READ config WRITE setConfig)
	Q_PROPERTY(QString side READ side WRITE setSide)
	Q_PROPERTY(QString type READ type WRITE setType)
	Q_PROPERTY(QString pattern READ pattern WRITE setPattern)
	Q_PROPERTY(QString query READ query WRITE setQuery)
	Q_PROPERTY(QString options READ options WRITE setOptions)
	Q_PROPERTY(QString tokenlink READ tokenlink WRITE setTokenlink)
	Q_PROPERTY(QString drop READ drop WRITE setDrop)
	QString _side, _type, _pattern, _query, _options, _tokenlink,_drop,_config;	
	QString _filterkey;
	QList<SafetConnection*> connectionlist;
    QList<SafetConnection*> backlist;
	
public:
	SafetPort();
	virtual void addChild(SafetXmlObject* o);
	QString side() const {return _side; };
	void setSide(const QString& i) { _side = i; }
	QString type() const {return _type; };
	void setType(const QString& i) { _type = i; }
	QString pattern() const {return _pattern; };
	void setPattern(const QString& i) { _pattern = i; }
	void setQuery(QString s) { _query = s; }
        QString query(int npath = 0) const;
	void setOptions(QString s) { _options = s; }
        QString options(int npath = 0) const;
        QString source(int npath = 0) const;
	void setTokenlink(QString s) { _tokenlink = s; }
        QString tokenlink(int npath = 0) const;
	void setDrop(QString s) { _drop = s; }
        QString drop() const { return _drop; }
	void setFilterkey(QString s) { _filterkey = s; }
        QString filterkey() const { return _filterkey; }
    	void setConfig(QString s) { _config = s; }
        QString config() const { return _config; }

    QList<SafetConnection*>& getConnectionlist();
    QList<SafetConnection*> getConnBackList();
    QList<SafetConnection *>& getBackList();

    void addConnection(SafetConnection* c);
    void clearConn();

};

#endif /*SAFETPORT_H_*/
