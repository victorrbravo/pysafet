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
#include "SafetNode.h"

SafetNode::SafetNode():SafetXmlObject() {
	_option = 0;
        _report = "";
        _numtokens = -1;
}



SafetNode::~SafetNode() {

}


int SafetNode::option() {
	return _option;	
}
void SafetNode::setOption(int n)  {
	_option = n;
}

SafetPort* SafetNode::port() {
	SafetPort *myport = NULL;
	for( int i=0; i < portlist.size(); i++) {
			if (portlist.at(i)->type().compare("split", Qt::CaseInsensitive) == 0 ) {
				return portlist.at(i);
			}
		}
	return myport;
}

QList<SafetPort*>& SafetNode::getPorts() {
	return portlist;
}

SafetPort* SafetNode::outport() {
	return port();
}

SafetPort* SafetNode::inport() {
	SafetPort *myport = NULL;
	for( int i=0; i < portlist.size(); i++) {
			if (portlist.at(i)->type().compare("join", Qt::CaseInsensitive) == 0 ) {
				return portlist.at(i);
			}
		}
	return myport;
}

QStack<QString> SafetNode::next() {
	QStack<QString> stack;
	for( int i=0; i < portlist.size(); i++) {
		if (portlist.at(i)->type() == "split") {
            for(int j=0; j < portlist.at(i)->getConnBackList().count(); j++) {
                stack.push( portlist.at(i)->getConnBackList().at(j)->source());
			}
		}
    }
	return stack;
}

QStack<QString> SafetNode::prev() {
	QStack<QString> stack;
	for( int i=0; i < portlist.size(); i++) {
		if (portlist.at(i)->type() == "join") {
			for(int j=0; j < portlist.at(i)->getConnectionlist().count(); j++) {
				stack.push( portlist.at(i)->getConnectionlist().at(j)->source());
			}
		}
	}
	return stack;
}
