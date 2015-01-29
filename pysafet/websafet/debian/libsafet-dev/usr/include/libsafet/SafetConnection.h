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

#ifndef SAFETCONNECTION_H_
#define SAFETCONNECTION_H_

#include "SafetXmlObject.h"

/**
* \brief Clase SafetConnection
* 
* Esta Clase corresponde al objeto  Conexión. El objeto SafetConnection (Conexión)
* especifica la vinculación con el siguiente o los siguientes nodos del grafo del  
* flujo de trabajo
*  
* Los objetos SafetConnection están contenidos dentro de los Objetos SafetPort (Puertos) o 
* en los objetos Condiciones 
* 
* Deriva de la Clase SafetXmlObject, lo que quiere decir que es sincronizado con un elemento
* XML definido en documento de definición de datos (DTD)
*  
*/

class SafetConnection : public SafetXmlObject  {
	
	Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(QString query READ query WRITE setQuery)
    Q_PROPERTY(QString options READ options WRITE setOptions)
    Q_PROPERTY(QString tokenlink READ tokenlink WRITE setTokenlink)
    Q_PROPERTY(QString back READ back WRITE setBack)
        QString  _query, _source,_options,_tokenlink, _back;
public:
	SafetConnection();
	void setSource(QString s) { _source = s; }
    QString source() const { return _source; }
        void setQuery(QString s) { _query = s; }
    QString query() const { return _query; }
        void setOptions(QString s) { _options = s; }
    QString options() const { return _options; }
    void setTokenlink(QString s) { _tokenlink = s; }
    QString tokenlink() const { return _tokenlink; }
    void setBack(QString b) { _back = b; }
    QString back() const { return _back; }


};

#endif /*SAFETCONNECTION_H_*/
