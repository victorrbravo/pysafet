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
#ifndef SAFETLOG_H
#define SAFETLOG_H

#include <QTextStream>
#include <QDateTime>
#include <QStack>
#include <QPair>


/**
	@author Víctor R. Bravo <vbravo@cenditel.gob.ve>, Antonio Araujo Brett <aaraujo@cenditel.gob.ve>
*/
class SafetLog : public QTextStream
{

	Qt::DateFormat _dateformat;
        QDateTime currentDate;
public:	// Tipos de Datos públicos

	enum Level { Debug =1, Action = 2, Error=4, Warning = 8, ExitApp = 16, ContinueApp = 32};
        enum StateStack { None, Init };

private:
	int  _level, templevel;
	bool haslevel;
        StateStack stateerrorstack,statewarnstack;
public:

	SafetLog();
	
	~SafetLog();
	QTextStream & operator<< ( const QString & string );
	SafetLog&  operator<< ( SafetLog::Level l );

        StateStack stateErrorStack() { return stateerrorstack;}
        StateStack stateWarnStack() { return statewarnstack;}
        void initErrorStack();
        void initWarnStack();
        void initAllStack();
        QStack< QPair<QDateTime,QString> >& stopErrorStack();
        QStack< QPair<QDateTime,QString> >& stopWarnStack();
        void stopAllStack();

        Qt::DateFormat dateFormat() const { return _dateformat;}
	void setFormat(Qt::DateFormat f) { _dateformat = f; } 	
	void turnOffAll() {  _level = 0; }
	void turnOnAll();
	void turnOn(Level l);
	void turnOff(Level l); 
	int level() const { return _level;}
	SafetLog::Level eval(bool c, const QString &s, SafetLog::Level l = SafetLog::Error);
	

};

#endif
