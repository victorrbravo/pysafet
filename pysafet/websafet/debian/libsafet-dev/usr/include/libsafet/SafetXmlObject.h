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


#ifndef SAFETXMLOBJECT_H_
#define SAFETXMLOBJECT_H_


#include <QObject>
#include <QtXml>


/* 
 * Clase Raíz para la conversión XML a objeto dentro del sistema, las
 * clases derivadas de SafetXmlObject se sincronizan con elementos XML
 * definidos en un documento DTD
 */

class SafetXmlObject : public QObject {
	
	Q_OBJECT

	SafetXmlObject* _parent;
public:
	SafetXmlObject();
	SafetXmlObject(const SafetXmlObject&);
	bool syncAttributes(const QDomElement& e);
	virtual void addChild(SafetXmlObject* o);
	SafetXmlObject* parent() const;
	void setParent(SafetXmlObject* p);
};

#endif /*SAFETXMLOBJECT_H_*/
