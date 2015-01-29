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

#include "SafetYAWL.h"
#include "SafetConfiguration.h"


SafetConfiguration::SafetConfiguration()  {

}

void  SafetConfiguration::open() {

	 Q_ASSERT_X(_connectstring.length() > 0,"open", qPrintable(tr("La cadena de conexion está vacia")) );
	 QStringList mylist = _connectstring.split(":");
	 QString stringdriver = "Q" + mylist[0].toUpper();
//	qDebug("Agregando una base de datos");
     	_db = QSqlDatabase::addDatabase(stringdriver);
	 _db.setHostName(mylist[1]);
	 _db.setDatabaseName(mylist[2]);
	 _db.setUserName(mylist[3]);
	 _db.setPassword(mylist[4]);
	 SafetYAWL::databaseOpen = _db.open();	
	 Q_ASSERT_X( SafetYAWL::databaseOpen, "open",
			 qPrintable(tr("No se pudo establecer la conexion  a la base de datos con los datos suministrados: ") +
			  _db.lastError().driverText()));	
	
	
}

void SafetConfiguration::addChild(SafetXmlObject* o) {
	if (o == NULL ) return;
	SafetXmlObject::addChild(o);
	Q_CHECK_PTR(o);

}
