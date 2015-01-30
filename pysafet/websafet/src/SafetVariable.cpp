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
#include "SafetVariable.h"



SafetVariable::SafetVariable():SafetXmlObject() {

        _scope = "task";
        _type  = "sql";
        _config = "1";
}


void SafetVariable::addChild(SafetXmlObject* o) {
	Q_CHECK_PTR(o);
	SafetXmlObject::addChild(o);
	switch(SafetYAWL::getClassReference(SafetYAWL::translateNS(o->metaObject()->className()))) {
		case 8:
	//		documentlist.push_back(qobject_cast<SafetDocument*>(o));
			break;
		default:;
	}
}

QString SafetVariable::getXMLDocument(const QString &value, long fieldno, QString& documentid ) const {
	
	QString str, sql;	
	SafetYAWL::streamlog << SafetLog::Action << tr("Iniciando creacion del documento XML a firmar");
//	QSqlQuery query( "SELECT proveedor_cedula FROM proveedor" );
//	qDebug("...documentsource(): %s", qPrintable( documentsource() ) );
	SafetXmlObject *o = SafetXmlObject::parent();
	Q_CHECK_PTR( o );
	o = o->SafetXmlObject::parent();
	Q_CHECK_PTR( o );
	Q_ASSERT( QString(o->metaObject()->className()) == "SafetWorkflow" );
	SafetWorkflow *wf = qobject_cast<SafetWorkflow*>(o);
	Q_CHECK_PTR( wf );
	SafetSQLParser localparser;
        localparser.setWorkflow( wf );
        qDebug("...****...***...getXMLDocument...localparser.setWorkflow( wf )....");
	localparser.setStr(documentsource().toUtf8() );
	localparser.parse();
	Q_ASSERT_X(localparser.error() ==  SafetParser::CORRECT, "createXMLDocument", 
                                qPrintable(tr("NO es correcta la sentencia SQL: <%1> ").arg(documentsource())));
	QString ts = localparser.getTablesource();
	Q_CHECK_PTR( wf->getToken() );
	localparser.addField(wf->getToken()->key());	
	wf->setTablesource( ts );
	sql = localparser.dropWhereClauses();
        SafetYAWL::streamlog
                << SafetLog::Debug
                << tr("Safet Variable tokenlink: |%1|")
                .arg(tokenlink());
	QString link = wf->processTokenlink(tokenlink());
        SafetYAWL::streamlog
                << SafetLog::Debug
                << tr("Safet Variable tokenlink (procesado): |%1|")
                .arg(link);

	QString wherestring ;	
	QSet<QString> whereset = localparser.getWhereclauses();	
	if ( whereset.count() > 0 ) wherestring = " WHERE ";
	for(int i = 0; i < whereset.count(); i++) {
		wherestring = wherestring + whereset.toList().at(i);
		if (i < whereset.count()-1) wherestring += " AND ";
	}
	sql = sql + " " + link + " " + wherestring;
	SafetYAWL::streamlog << SafetLog::Debug << tr("Documento SQL a ser firmado: %1").arg(sql);
	QSqlQuery query( sql, SafetYAWL::currentDb ); // <-- puntero a db actual
	while (query.next() ) {
//		qDebug("....query.record().fieldName(fieldno): %s", qPrintable(query.record().fieldName(fieldno)));
		if ( query.value(fieldno).toString().trimmed() == value ) {
			str        = createXMLFileFromQuery(query, "::text");
            SYD << tr("......:SafetVariable::getXMLDocument....DOCUMENTID_value:|%1|")
                   .arg(value);
            SYD << tr("\n\n........GETXMLDOCUMENT...SafetWorkflow::getXMLDocument.....");
             for(int i = 0; i < query.record().count(); i++) {
                 SYD << tr("...........SafetWorkflow::getXMLDocument....fieldname:|%1|")
                        .arg(query.record().fieldName(i));
                 SYD << tr("...........SafetWorkflow::getXMLDocument....value:|%1|")
                        .arg(query.record().value(i).toString());

             }
             SYD << tr("...........SafetWorkflow::getXMLDocument.....\n\n");
                        documentid = SafetYAWL::getDocumentID(query,true);

             SYD << tr("......:SafetVariable::getXMLDocument....DOCUMENTID_documentid:|%1|")
                               .arg(documentid);

			break;
		}	
	}
	return str;
}


QString SafetVariable::createXMLFileFromQuery(const QSqlQuery& query, const QString& outputFileName ) const {
	
	QString result; 

	Q_ASSERT_X(!outputFileName.isEmpty(), 
			"SafetDocument::createXMLFileFromSQL()", 
			qPrintable(QObject::tr("outputFileName vacio")));
	
	if (outputFileName.isEmpty()){
		qDebug("cadena outputFileName vacia");
		return result;
    	}
	

	QFile file;
	bool open;
	QTextStream out;
	bool istext = outputFileName.compare("::text", Qt::CaseInsensitive) == 0;
	if ( !istext ) {	
		file.setFileName(outputFileName);
		open = file.open(QIODevice::WriteOnly | QIODevice::Text); 
	Q_ASSERT_X(open, 
			"SafetDocument::createXMLFileFromSQL()",
			qPrintable(QObject::tr("No se pudo crear el archivo XML correspondiente a la sentencia SQL.")));
		out.setDevice(&file);
	}
	else {
		out.setString(&result);		
	}
	
	
	
	QSqlRecord rec;
	rec = query.record();
	
	out << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << "\n";
	out << "<libsafetdb>" << "\n";
	
	QString cadena("");
	    	//qDebug("<: %s", qPrintable(x.setNum(j+1)), ">");
//		    qDebug("\t<Document>");
	   		
		out << "<Document>" << "\n" ;
	   	for (int i= 0; i< rec.count(); i++){
	   		//qDebug(qPrintable(rec.fieldName(i)));
	   		//qDebug(qPrintable(query.value(i).toString()));	
	   		cadena.append("<");
	   		cadena.append(rec.fieldName(i));
	   		cadena.append(">");
	   		cadena.append(query.value(i).toString());
	   		cadena.append("</");
	   		cadena.append(rec.fieldName(i));
	   		cadena.append(">");
	   		  	
//	   		qDebug(qPrintable(cadena));
	   		  	
	   		out << cadena << "\n" ;  	
	   		cadena.clear();
	   	}
//	    qDebug("</Document>");
	    
	out << "</Document>" << "\n" << "</libsafetdb>" << "\n";
	return result;
}
