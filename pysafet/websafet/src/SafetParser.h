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
#ifndef SAFETPARSER_H_
#define SAFETPARSER_H_

#include <QObject>
#include <QtXml>


class SafetParser : public QObject {
	
	Q_OBJECT
public:
        enum  Error { INCORRECT, CORRECT, BRACKETSNOBALANCE, OTHER, TRUESTATEMENT };
protected:
	QString _str;
	Error _error;
public:
        SafetParser(QObject* parent = 0);
	QString str() const { return _str; }
        void setStr(const QString& s) { _str = s; }
        virtual bool parse(const QString& s = "") = 0;
	Error error() { return _error; }
};

#endif /*SAFETPARSER_H_*/
