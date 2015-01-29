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

#ifndef SAFETCONDITION_H_
#define SAFETCONDITION_H_

#include "SafetNode.h"
#include "SafetPort.h"
#include <QtXml>

/**
* \brief Clase SafetCondition
* 
* Esta Clase corresponde al objeto  Condición. El objeto Condition (condition)
* especifica una condición (lógica) a ser evaluado en función de obtener el siguiente 
* nodo que seguirá la ficha. Es un elemento del Modelo YAWL
*  
* Los objetos SafetTask son contenidos dentro de el objeto SafetWorkflow 
* 
* Deriva de la Clase SafetNode, lo que quiere decir que se considera un 
* nodo en el grafo del flujo de trabajo
*  
*/
class SafetCondition : public SafetNode  {
	
	Q_OBJECT
	Q_PROPERTY(QString id READ id WRITE setId)
	Q_PROPERTY(QString type READ type WRITE setType)
	QString _id, _type;
	
public:
	SafetCondition();
	void setId(QString i) { _id = i; }
       QString id() const { return _id; }
 	void setType(QString i) { _type = i; }
       QString type() const { return _type; }

	virtual void addChild(SafetXmlObject* o);
	
};

#endif /*SAFETCONDITION_H_*/
