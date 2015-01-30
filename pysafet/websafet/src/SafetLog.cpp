//
// C++ Implementation: SafetLog
//
// Description: 
//
//
// Author: Víctor R. Bravo <vbravo@cenditel.gob.ve>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "SafetLog.h"
#include "SafetYAWL.h"

SafetLog::SafetLog()
 : QTextStream() {

	_dateformat = Qt::TextDate;
	haslevel = false;
	_level = templevel = 0;
        stateerrorstack = None;
        statewarnstack = None;
}


SafetLog::~SafetLog() {


}

void SafetLog::initErrorStack() {
    SafetYAWL::errorStack().clear();
    stateerrorstack = Init;
}


void SafetLog::initAllStack() {
    SafetYAWL::errorStack().clear();
    SafetYAWL::warnStack().clear();
    stateerrorstack = Init;
    statewarnstack = Init;
}

void SafetLog::initWarnStack() {
    SafetYAWL::warnStack().clear();
    statewarnstack = Init;
}

void SafetLog::stopAllStack() {
    stateerrorstack = None;
    statewarnstack = None;

}

QStack< QPair<QDateTime,QString> >&  SafetLog::stopErrorStack() {
    stateerrorstack = None;
    return SafetYAWL::errorStack();
}
QStack< QPair<QDateTime,QString> >& SafetLog::stopWarnStack() {
    statewarnstack = None;
    return SafetYAWL::warnStack();
}

QTextStream& SafetLog::operator<<( const QString & string ) {


	QTextStream* me = static_cast<QTextStream*>(this);
	Q_CHECK_PTR( me );

	QString strlevel = "[DEBUG]";
	if ( !haslevel ) {
                (*me) <<currentDate.toString(dateFormat()) << " " << strlevel << " " << string << endl;

	}
	else {
		if ( level() & templevel ) {
                        (*me) << string << endl;
                        if ( templevel == SafetLog::Error && stateErrorStack() == Init ) {
                            SafetYAWL::addErrorToStack(currentDate,string);
                        }
                        else if ( templevel == SafetLog::Warning && stateWarnStack() == Init ) {
                            SafetYAWL::addWarnToStack(currentDate,string);
                        }

		}
	}
	
	haslevel = false;
	templevel = 0;
	return  (*this);

}


void SafetLog::turnOff(Level l) {  
//	qDebug("Level %d l %d or:%d",_level,l,_level | l); 
	_level  = ((_level | l) ^ l );  

}

void SafetLog::turnOnAll() {  
	_level = (Debug + Action + Error + Warning); 
}	


void SafetLog::turnOn(Level l) { 
	_level = (_level | l);  
}


SafetLog& SafetLog::operator<< ( SafetLog::Level l ) {

        currentDate = QDateTime::currentDateTime();
	QTextStream* me = static_cast<QTextStream*>(this);
	Q_CHECK_PTR( me );
	QString strlevel;
	switch ( l ) {
		case Debug:
			if ( level() & l ) 
				strlevel = QObject::tr("[  DEBUG  ]");
			break;
		case Action:
			if ( level() & l ) 
				strlevel = QObject::tr("[ ACTION  ]");
			break;
		case Error:
			if ( level() & l ) 
				strlevel = QObject::tr("[  ERROR  ]");
			break;
		case Warning:
			if ( level() & l ) 
				strlevel = QObject::tr("[ WARNING ]");
			break;

	}
	if ( level() & l ) {
                (*me) <<  currentDate.toString(dateFormat()) << " " << strlevel << " ";
	}
	haslevel = true;
	templevel = l;
	return  (*this);
}

SafetLog::Level SafetLog::eval(bool c, const QString &s, SafetLog::Level l) {
	bool result = !c;		
	if ( result ) {		
		SafetLog *me = static_cast<SafetLog*>(this);
		 (*me) << l << s;
	}	
	if ( result ) return  ExitApp;
	return ContinueApp;
}
