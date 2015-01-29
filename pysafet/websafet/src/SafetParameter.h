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

#ifndef SAFETPARAMETER_H_
#define SAFETPARAMETER_H_

#include "SafetXmlObject.h"
#include "SafetPort.h"

/**
* \brief Clase SafetParameter
* 
* Esta Clase corresponde al objeto  Nodo. El objeto Node (Nodo)
* especifica una abstracción de un nodo del grafo que modela el flujo de trabajo, 
* y que tiene relación con el modelo YAWL
*  
* Los objetos SafetParameter pueden ser de dos tipos: SafetTask o SafetCondition
* 
* Deriva de la Clase SafetXmlObject, lo que quiere decir que es sincronizado con un elemento
* XML definido en documento de definición de datos (DTD)
*  
*/
class SafetParameter : public SafetXmlObject {
	
	Q_OBJECT

        Q_PROPERTY(QString type READ type WRITE setType)
        Q_PROPERTY(QString mandatory READ mandatory WRITE setMandatory)
        Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(QString  primarykey READ primarykey WRITE setPrimarykey)
        Q_PROPERTY(QString validation READ validation WRITE setValidation)
        Q_PROPERTY(QString options READ options WRITE setOptions)
        Q_PROPERTY(QString function READ function WRITE setFunction)
        Q_PROPERTY(QString format READ format WRITE setFormat)
        Q_PROPERTY(QString desc READ desc WRITE setDesc)
        Q_PROPERTY(QString literal READ literal WRITE setLiteral)
        Q_PROPERTY(QString sequence READ sequence WRITE setSequence)
        Q_PROPERTY(QString typesequence READ typesequence WRITE setTypesequence)
        Q_PROPERTY(QString input READ input WRITE setInput)
        Q_PROPERTY(QString path READ path WRITE setPath)
        Q_PROPERTY(QString visible READ visible WRITE setVisible)
        Q_PROPERTY(QString order READ order WRITE setOrder)
        Q_PROPERTY(QString icon READ icon WRITE setIcon)
        Q_PROPERTY(QString index READ index WRITE setIndex)
        Q_PROPERTY(QString filter READ filter WRITE setFilter)
        Q_PROPERTY(QString repetible READ repetible WRITE setRepetible)
        Q_PROPERTY(QString configurekey READ configurekey WRITE setConfigurekey)
        Q_PROPERTY(QString defaultvalue  READ defaultvalue WRITE setDefaultvalue)
        Q_PROPERTY(QString changefor  READ defaultvalue WRITE setChangefor)

        QString _type,_mandatory,_title,_primarykey,_validation,
        _options, _function, _format, _desc, _literal, _sequence,
        _typesequence, _input, _path, _visible, _order, _icon,
        _index,_filter, _repetible, _configurekey,
        _defaultvalue;
        QString _changefor;
        QStringList _changeforlist;

public:
        SafetParameter();
        virtual ~SafetParameter();

        QString type() const {return _type; };
        void setType(const QString& t) { _type = t; }
        QString mandatory() const {return _mandatory; };
        void setMandatory(const QString& m) { _mandatory = m; }
        QString title() const {return _title; };
        void setTitle(const QString& t) { _title = t; }
        QString primarykey() const {return _primarykey; };
        void setPrimarykey(const QString& p) { _primarykey = p; }
        QString validation() const {return _validation; };
        void setValidation(const QString& v) { _validation = v; }
        QString options() const {return _options; };
        void setOptions(const QString& o) { _options = o; }
        QString function() const {return _function; };
        void setFunction(const QString& f) { _function = f; }
        QString format() const {return _format; };
        void setFormat(const QString& f) { _format = f; }
        QString desc() const {return _desc; };
        void setDesc(const QString& d) { _desc = d; }
        QString literal() const {return _literal; };
        void setLiteral(const QString& l) { _literal = l; }
        QString sequence() const {return _sequence; };
        void setSequence(const QString& s) { _sequence = s; }
        QString typesequence() const {return _typesequence; };
        void setTypesequence(const QString& t) { _typesequence = t; }
        QString input() const {return _input; };
        void setInput(const QString& i) { _input = i; }
        QString path() const {return _path; };
        void setPath(const QString& p) { _path = p; }
        QString visible() const {return _visible; };
        void setVisible(const QString& v) { _visible = v; }
        QString order() const {return _order; };
        void setOrder(const QString& o) { _order = o; }
        QString icon() const {return _icon; };
        void setIcon(const QString& i) { _icon = i; }
        QString index() const {return _index; };
        void setIndex(const QString& i) { _index = i; }
        QString filter() const {return _filter; };
        void setFilter(const QString& f) { _filter = f; }
        QString repetible() const {return _repetible; };
        void setRepetible(const QString& r) { _repetible = r; }
        QString configurekey() const {return _configurekey; };
        void setConfigurekey(const QString& c) { _configurekey = c; }
        QString defaultvalue() const {return _defaultvalue; };
        void setDefaultvalue(const QString& d) { _defaultvalue= d; }
        QString changefor() const {return _changefor; };
        void setChangefor(const QString& c);
        QStringList changeforlist() { return _changeforlist;}



	
};

#endif /*SafetParameter_H_*/
