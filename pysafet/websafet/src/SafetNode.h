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

#ifndef SAFETNODE_H_
#define SAFETNODE_H_

#include "SafetXmlObject.h"
#include "SafetPort.h"

/**
* \brief Clase SafetNode
* 
* Esta Clase corresponde al objeto  Nodo. El objeto Node (Nodo)
* especifica una abstracción de un nodo del grafo que modela el flujo de trabajo, 
* y que tiene relación con el modelo YAWL
*  
* Los objetos SafetNode pueden ser de dos tipos: SafetTask o SafetCondition 
* 
* Deriva de la Clase SafetXmlObject, lo que quiere decir que es sincronizado con un elemento
* XML definido en documento de definición de datos (DTD)
*  
*/
class SafetNode : public SafetXmlObject {
	
	Q_OBJECT
        int _option;
        Q_PROPERTY(QString title READ title WRITE setTitle)
        Q_PROPERTY(QString report READ report WRITE setReport) // Reporte para las estadísticas
        Q_PROPERTY(QString numericinfo READ numericinfo WRITE setNumericinfo)
        Q_PROPERTY(QString textualinfo READ textualinfo WRITE setTextualinfo)
        QString _title,_report;
        QString _numericinfo;
        QString _textualinfo;
        int _numtokens;
protected:
	QList<SafetPort*> portlist;
public:
	enum PortType { SPLIT, JOIN };
	SafetNode();
	virtual ~SafetNode();
	int option();
	void setOption(int n);
        virtual QString id() const = 0;
        QString title() const {return _title; };
        void setTitle(const QString& i) { _title = i; }
        QString report() const {return _report; };
        void setReport(const QString& i) { _report = i; }
        QString numericinfo() const {return _numericinfo; };
        void setNumericinfo(const QString& n) { _numericinfo = n; }
        QString textualinfo() const {return _textualinfo; };
        void setTextualinfo(const QString& t) { _textualinfo = t; }


        /*
	 * \brief Función virtual abstracta que regresa una pila con los nodos siguientes
	 */
	QStack<QString> next();
	QStack<QString> prev();
	QList<SafetPort*>& getPorts();
	virtual SafetPort* port();
	virtual SafetPort* outport();
	virtual SafetPort* inport();
        int numtokens() { return _numtokens;}
        void setNumtokens(int n) { _numtokens = n;}

	
};

#endif /*SAFETNODE_H_*/
