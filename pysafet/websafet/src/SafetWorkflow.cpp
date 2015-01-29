/*
* SAFET Sistema Automatizado para la Firma ElectrÃ³nica y Estampado de Tiempo
* Copyright (C) 2008 VÃ­ctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*     
* CENDITEL Fundacion Centro Nacional de Desarrollo e InvestigaciÃ³n en TecnologÃ­as Libres 
*  
* Este programa es software libre; Usted puede usarlo bajo los tÃ©rminos de la licencia de 
* software GPL versiÃ³n 2.0 de la Free Software Foundation. 
* 
* Este programa se distribuye con la esperanza de que sea Ãºtil, pero SI NINGUNA GARANTIA;
* tampoco las implÃ­citas garantÃ­as de MERCANTILIDAD o ADECUACION A UN PROPopITO PARTICULAR.
* Consulte la licencia GPL para mÃ¡s detalles. Usted debe recibir una copia de la GPL junto 
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5º Piso, Boston, MA 02110-1301, USA.
*
*/


#include <QSqlField>
#include "SafetYAWL.h"
#include "SafetWorkflow.h"
#include "SafetConfFile.h"
#include <locale.h>
//#include <QScriptEngine>

#define SAFET_MAINWINDOW

#ifdef SAFET_MAINWINDOW
#include "../inflow/mainwindow.h"
#endif



//#include "threadconsole.h"

//** En la compilacion incluir la c.onstantes -DSAFET_DBXML

#ifdef SAFET_DBXML   // Incluir la biblioteca dbxml para soporte de Base de Datos XML Nativa 
#include "SafetDbXmlRepository.h"
#endif
// ** Soporte DbXml Base de datos XML Nativa 
#include <QPointer>

// archivos incluidos para soportar llamadas a metodos remotos del servicio web safet
#ifdef SAFET_GSOAP // Definido conexion a servicios Web
#include "soapH.h"
#include "safet.nsmap"
#endif  // Definido conexion a servicios Web


SafetWorkflow::SafetWorkflow() {
     ptoken = NULL;	
     _tablesource = "";
     isStartingKeyFilter = true;
     shapecount = 0;
     RESOURCESFILES[ "node.split.xor"] = SafetYAWL::getConfFile().getValue("Operators", "split.xor").toString();
     RESOURCESFILES[ "node.split.or"] =  SafetYAWL::getConfFile().getValue("Operators", "split.or").toString();
     RESOURCESFILES[ "node.split.and"] =  SafetYAWL::getConfFile().getValue("Operators", "split.and").toString();
     RESOURCESFILES[ "node.join.or"] =  SafetYAWL::getConfFile().getValue("Operators", "join.xor").toString();
     RESOURCESFILES[ "node.join.and"] = SafetYAWL::getConfFile().getValue("Operators", "join.and").toString();
     RESOURCESFILES[ "node.join.xor"] = SafetYAWL::getConfFile().getValue("Operators", "join.xor").toString();
     RESOURCESFILES[ "node.none"] = SafetYAWL::getConfFile().getValue("Operators", "none").toString();
     RESOURCESFILES[ "node.split.none"] = "imgs/none.png";
     RESOURCESFILES[ "node.join.none"] = "imgs/none.png";
     RESOURCESFILES[ "node."] = "imgs/none.png";
     RESOURCESFILES[ "condition.start"] = "imgs/start.png";
     RESOURCESFILES[ "condition.end"] = "imgs/end.png";
     RESOURCESFILES[ "edge.and"] = "[style=bold";
     RESOURCESFILES[ "edge.or"] =  "[style=dotted";
     RESOURCESFILES[ "edge.xor"] = "[style=dashed";
     RESOURCESFILES[ "edge.none"] = "[style=solid";
     RESOURCESFILES[ "edge."] = "[style=solid";
// Valores inciales para la conversion de SVG a JSON
     SHAPEMAP[ "ellipse" ] = 0;
     SHAPEMAP[ "path" ] = 1;
     SHAPEMAP[ "text" ] = 2;
     SHAPEMAP[ "polygon" ] = 3;					
     oriminy = -500;
     orimaxy= 0;
     oriminxx = 0;
     orimaxx = 500;
     parser.setWorkflow(this);
     setNumberOfTokens( 0 );
     _xmlRepository = NULL;


}

SafetWorkflow::SafetWorkflow(const SafetWorkflow& other) {
    _xmlRepository = NULL;
}

SafetWorkflow::~SafetWorkflow() {
     if ( ptoken != NULL) delete ptoken;
 //    qDebug("saliendo...~SafetWorkflow...");
     foreach(SafetTask* mytask, tasklist){
          if ( mytask) delete mytask;
     }

     foreach(SafetCondition* mycondition, conditionlist){
          if ( mycondition) delete mycondition;
     }
     if ( _xmlRepository != NULL ) {
         delete _xmlRepository;
     }


}


void SafetWorkflow::addTask(SafetTask* t) {
     Q_CHECK_PTR( t );
     tasklist.push_back(t);
     t->setParent( qobject_cast<SafetXmlObject*>(this));
     nodemap.insert(t->id(),qobject_cast<SafetNode*>(t));
     visitnodemap.insert( t->id(),false);

}

QMap<QString,QString> SafetWorkflow::changeHumanizeParameters(const QMap<QString,QString>& l) {
    QMap<QString,QString> result;
    QDateTime mydate;
    SYD << tr("...SafetWorkflow::changeHumanizeParameters...l.count():%1").arg(l.count());
    QMap<QString, QString>::const_iterator i = l.begin();

     while (i != l.end() ) {
        QString p = i.value();
        QString strout = i.value();

        SYD << tr("...SafetWorkflow::changeHumanizeParameters...strout:|%1|").arg(strout);
        if (p == Safet::AnyOneObject ) {
            strout = Safet::SqlAllValues;
        }
        else {
            mydate = QDateTime::fromString(p, Safet::DateFormat);
            SYD << tr("...SafetWorkflow::changeHumanizeParameters...."
                      "...p....:|%2|"
                      "mydate.isValid():|%1|")
                   .arg(mydate.isValid())
                   .arg(p);
            if (mydate.isValid()) {
                strout = QString("%1").arg(mydate.toTime_t());
            }
        }
        result[i.key()]=strout;
        ++i;
    }

    return result;
}

void SafetWorkflow::evalAutofilters() {
    SafetYAWL   *myyawl = qobject_cast<SafetYAWL*>(parent());

    SYD << tr(".....SafetWorkflow::evalAutofilters()...(1)..");
    Q_CHECK_PTR(myyawl);

    SYD << tr(".....SafetWorkflow::evalAutofilters()...(2)..");
    foreach(SafetTask* task, getTasklist()) {
        QList<SafetAutofilter*> mylistaf = task->getAutofilters();
        foreach(SafetAutofilter* myfilter, mylistaf) {
          if ( myyawl->isActiveFilter( myfilter->id() ) ) {
                myfilter->setFiltertask( task );
                QList<SafetTask*> aflist = myfilter->createTasks(task->id().left(2).toLower());

            }
        }


    }
    SYD << tr(".....SafetWorkflow::evalAutofilters()...(3)..");

}

bool SafetWorkflow::putParameters(const QMap<QString,QString>& p) {
    bool result = false;

    QMap<QString,QString> list = changeHumanizeParameters(p);


     QString strin, strout;
    foreach(SafetTask* task, getTasklist()) {
        strin = task->title();
        SYD << tr("....SafetWorkflow::putParameters...strin:|%1|")
               .arg(strin);
        bool dodefaultvalue = false;
        strout = replaceArg(strin,p, dodefaultvalue);
        SYD << tr("....SafetWorkflow::putParameters...strout:|%1|")
               .arg(strout);

        SYD << tr("...***.SafetWorkflow::putParameters..dodefaultvalue.....DOIT:|%1|")
               .arg(dodefaultvalue);

        if ( strin != strout && dodefaultvalue ) {
            SYD << tr("...doit***.SafetWorkflow::putParameters..dodefaultvalue.....STRIN:|%1|")
                   .arg(strin);
            SYD << tr("...***.SafetWorkflow::putParameters..dodefaultvalue.....STROUT:|%1|")
                   .arg(strout);

            task->setTitle("::safethide::");
        }
        else if (strin != strout ) {
            SYD << tr("....SafetWorkflow::putParameters..(1)...");
            if ( strout.indexOf(QRegExp("\\s+NULL\\s*"),Qt::CaseInsensitive) == -1 ) {
                QString normtl = strout;

                QRegExp rx("\\d{10,10}");

                if (rx.exactMatch(normtl)) {
                    SYD << tr("....SafetWorkflow::putParameters..(4rx)...");
                    bool ok;
                    normtl = QDateTime::fromTime_t(normtl.toInt(&ok)).toString(Safet::DateOnlyFormat);
                }
                else {
                    normtl.replace("�","a");
                    normtl.replace("�","e");
                    normtl.replace("�","i");
                    normtl.replace("�","o");
                    normtl.replace("�","u");
                    normtl.replace("�","n");
                    normtl.replace("�","N");
                    normtl.replace(" ","_");
                }
                SYD << tr("....**SafetWorkflow::putParameters..*normtl:|%1|")
                       .arg(normtl);

                task->setTitle(normtl);
               // task->setTitle("tipo");
                SYD << tr("....SafetWorkflow::putParameters....(3)...title:|%1|").arg(task->title());
            }
            else {
                SYD << tr("....SafetWorkflow::putParameters..(2)...");
                //task->setTitle(Safet::AnyOneObject);

                task->setTitle("::safethide::");
            }
        }
        SYD << tr(".........SafetWorkflow::putParameters AUTOFILTERCOUNT:|%1|")
               .arg(task->getAutofilters().count());
        foreach(SafetAutofilter *filter, task->getAutofilters()) {
            strin = filter->query();
            bool doit = false;
            strout = replaceArg(strin,list,doit);
            strout.replace("_USERNAME", SafetYAWL::currentAuthUser());
            SYD << tr("......SafetWorkflow::putParameters...AUTO_REPLACE....AUTOFILTERCOUNT...strout:|%1|")
                   .arg(strout);
            if (strin != strout ) {
                filter->setQuery(strout);
            }

            doit = false;
            strin = filter->period();
            strout = replaceArg(strin,list,doit);
            strout.replace("_USERNAME", SafetYAWL::currentAuthUser());
            if (strin != strout ) {
                filter->setPeriod(strout);
            }


        }

        foreach(SafetPort *port, task->getPorts()) {
            if (port->type() == "split" ) {

                foreach (SafetConnection* conn, port->getConnectionlist()) {
                    strin = conn->options();
                    SYD << tr("......SafetWorkflow::putParameters....strin:|%1|")
                           .arg(strin);
                    SYD << tr("......SafetWorkflow::putParameters....list.count():|%1|")
                           .arg(list.count());
                    bool doit = false;
                    strout = replaceArg(strin,list, doit);
                    SYD << tr("......SafetWorkflow::putParameters....strout:|%1|")
                           .arg(strout);

                    if (strin != strout ) {
                        result = true;
                        conn->setOptions(strout);
                    }

                }

            }
        }

    }
    foreach(SafetCondition* cond, getConditionlist()) {
        strin = cond->title();
        bool doit = false;
        strout = replaceArg(strin,p,doit);
        if (strin != strout ) {
            cond->setTitle(strout);
        }
        foreach(SafetPort *port, cond->getPorts()) {
            if (port->type() == "split" ) {

                foreach (SafetConnection* conn, port->getConnectionlist()) {
                    strin = conn->options();
                    bool doit = false;
                    SYD << tr("...SafetWorkflow::putParameters....condition...strin:|%1|")
                           .arg(strin);
                    strout = replaceArg(strin,list,doit);
                    SYD << tr("...SafetWorkflow::putParameters....condition...strout:|%1|")
                           .arg(strout);
                    SYD << tr("...SafetWorkflow::putParameters....condition...list.count():|%1|")
                           .arg(list.count());

                    if (strin != strout ) {
                        result = true;
                        conn->setOptions(strout.replace("�","o"));
                    }
                }

            }
        }

    }


    return result;
}

QString SafetWorkflow::replaceArg(const QString& strin, const QMap<QString,QString>& l, bool &doit) {
    QString result = strin;
    doit = false; // es defaultvalue
   QString pattern = QString("(\\=|>|<|<\\=|>\\=|IS|IN|LIKE)?\\s*\\{\\#([a-zA-Z0-9_]+)\\}");
   QRegExp rx;
   rx.setPattern(pattern);
   int pos = 0;
   SYD << tr("--------------");
   foreach (QString k, l.keys()) {
       SYD << tr("key:|%1|").arg(k);
       SYD << tr("value:|%1|").arg(l[k]);
       SYD << tr("...");
   }
   SYD << tr("--------------");
   while(true) {
       pos  = strin.indexOf(rx,pos);
       if (pos == -1) break;

           bool ok;
           QString numpar = rx.cap(2);
           QString stringfinded;
           if (l.contains(numpar)) {
               QString mynumpar = l[numpar].trimmed();
               stringfinded = QString("{#%1}").arg(numpar);
               if (!mynumpar.isEmpty()) {
                    result.replace(stringfinded,mynumpar);
               }

           }
           pos += stringfinded.length()==0?5:stringfinded.length();

   }
   pos = 0;   
   QString newresult = result;
   QList<SafetParameter*> mypars = getParameterlist();


   while(true) {
          pos  = newresult.indexOf(rx,pos);
          if (pos == -1) break;

          SYD << tr(".......SafetWorkflow::replaceArg REPLACESQL....rx.cap(0):|%1|")
                 .arg(rx.cap(0));

          QString myparameter = rx.cap(0);
          QRegExp rxtitle("\\{\\#([a-zA-Z0-9_]+)\\}");

          int postitle = rxtitle.indexIn(myparameter);
          SYD << tr(".......SafetWorkflow::replaceArg REPLACESQL....postitle:|%1|")
                 .arg(postitle);

          if (postitle >= 0 ) {
              QString mytitle = rxtitle.cap(1);
              SYD << tr(".......SafetWorkflow::replaceArg REPLACESQL....mytitle:|%1|")
                     .arg(mytitle);

              foreach(SafetParameter* par, mypars) {
                  if (par->configurekey().isEmpty()) {
                        SYD << tr("..(parameter)...par title:|%1|").arg(par->title());
                        if (par->title().compare(mytitle,Qt::CaseSensitive) == 0 ) {
                            QString defaultvalue = par->defaultvalue();
                            SYD << tr("..(parameter)...par->defaultvalue():|%1|").arg(defaultvalue);
                            if (!defaultvalue.isEmpty()) {
                                result.replace(rx.cap(0),par->defaultvalue());
                                doit = true;
                            }
                            else {
                                result.replace(rx.cap(0)," "+Safet::SqlAllValues);
                            }
                            break;

                        }

                  }
              }




          }
          else {
            result.replace(rx.cap(0)," "+Safet::SqlAllValues);

          }
          pos += rx.cap(0).length()==0?5:rx.cap(0).length();

   }

    return result.trimmed();

}

QString SafetWorkflow::replaceArg(const QString& strin, const QMap<QString,QString>& l) {
    QString result = strin;
   QString pattern = QString("(\\=|>|<|<\\=|>\\=|IS|IN|LIKE)?\\s*\\{\\#([a-zA-Z0-9_]+)\\}");
   QRegExp rx;
   rx.setPattern(pattern);
   int pos = 0;
   while(true) {
       pos  = strin.indexOf(rx,pos);
       if (pos == -1) break;

           bool ok;
           QString numpar = rx.cap(2);
           QString stringfinded;
           if (l.contains(numpar)) {
               QString mynumpar = l[numpar].trimmed();
               stringfinded = QString("{#%1}").arg(numpar);
               if (!mynumpar.isEmpty()) {
                    result.replace(stringfinded,mynumpar);
               }

           }
           pos += stringfinded.length()==0?5:stringfinded.length();

   }
   pos = 0;
   QString newresult = result;
   while(true) {
          pos  = newresult.indexOf(rx,pos);
          if (pos == -1) break;
           result.replace(rx.cap(0)," "+Safet::SqlAllValues);
          pos += rx.cap(0).length()==0?5:rx.cap(0).length();
   }

    return result.trimmed();

}


void SafetWorkflow::addChild(SafetXmlObject* o) {
	Q_CHECK_PTR(o);
	SafetXmlObject::addChild(o);
	SafetTask *ptask;
	SafetCondition* pcond;
        SafetParameter* pparameter;
	switch(SafetYAWL::getClassReference(SafetYAWL::translateNS(o->metaObject()->className()))) {
	case 2: // SafetTask
		ptask = qobject_cast<SafetTask*>(o);
		Q_ASSERT(ptask);
        SYD << tr("....SafetWorkflow::addChild..ADDCHILD..(1)..ptask->title():|%1|")
               .arg(ptask->title());
         ptask->setTitle(SafetYAWL::replaceArgsflow(ptask->title()));
		tasklist.push_back(ptask);
        SYD << tr(".....***SafetWorkflow::addChild..ADDCHILD.(2)..ptask->title():|%1|")
               .arg(ptask->title());

		nodemap.insert(ptask->id(),qobject_cast<SafetNode*>(o));
		visitnodemap.insert( ptask->id(),false);              

		break;
	case 5:
		pcond = qobject_cast<SafetCondition*>(o);
                Q_ASSERT( pcond);
                pcond->setTitle(SafetYAWL::replaceArgsflow(pcond->title()));
		conditionlist.push_back(pcond);
		nodemap.insert(pcond->id(),qobject_cast<SafetNode*>(o));
		visitnodemap.insert( pcond->id(),false);
		break;
	case 9:
		Q_ASSERT(ptoken == NULL);
		ptoken = qobject_cast<SafetToken*>(o);
		break;
        case 12:
                pparameter = qobject_cast<SafetParameter*>(o);
                Q_CHECK_PTR( pparameter);
                parameterlist.append(pparameter);
                break;


	default:;
	}

}

QSet<QString> SafetWorkflow::getKeys(const QString& idtask) {
	QSet<QString> myset;
	SafetTask *curtask;
	QList<SafetTask*>::iterator i;
	QStack<QString> keys;
	
	//QSqlQuery query();
	QSqlQuery query(SafetYAWL::currentDb); // <-- SafetYAWL::currentDb puntero a db actual
	 for (i = tasklist.begin(); i != tasklist.end(); ++i) {
		curtask = *i;
		if (curtask->id() == idtask) {
			SafetVariable *curvar;
			QList<SafetVariable*>::iterator j;
			for (j = curtask->getVariables().begin(); j != curtask->getVariables().end(); ++j) {
				curvar = *j;
				query = getSQLDocuments(*curvar);
				break; // Documento para una sola variable, se necesita verificar el algoritmo para el caso que existen dos o mas variables
			}							
		}
	}
	while( query.next() ) {
		myset.insert( query.value(0).toString().trimmed() );
	}
	QString namefields;
	for( int i = 0; i < query.record().count(); i++){
		namefields += query.record().fieldName(i) + " ";
	}	
	namefields.chop(1);
	return myset;
}

int SafetWorkflow::getQuerySize(QSqlQuery& qry) {
    int n = -1;
    bool hassize = SafetYAWL::currentDb.driver()->hasFeature(QSqlDriver::QuerySize);
    if (hassize) {
        return qry.size();
    }
    else {
        n = 0;
        while (qry.next()) {
            n++;
        }
        //while (qry.previous());
        qry.first();
        qry.previous();
    }

    return n;
}



SafetDocument SafetWorkflow::getDocuments(const QString& idvariable, QList<QSqlField>& fields,
                                    int &howmanydocuments,
                                    OutputFormat of,const QString& info) {

    SYD << tr("....SafetWorkflow::getDocuments...entrando...(1)...");

    QString str;
    SafetVariable *curvar = searchVariable( idvariable );
    Q_CHECK_PTR( curvar );


    QSqlQuery query(SafetYAWL::currentDb); // <-- SafetYAWL::currentDb puntero a db actual

    SYD << tr("....SafetWorkflow::getDocuments...entrando...(1)...curvar->id():|%1|")
           .arg(curvar->id());

    query = getSQLDocuments(*curvar);
    SafetDocument mydocument;
    howmanydocuments = 0;

    getQuerySize(query);
    if ( getQuerySize(query) >  0 ) {
        switch (of) {
            case SafetWorkflow::XML:
                str = mydocument.getXmlQuery(query,howmanydocuments,info);
                break;
            case SafetWorkflow::JSON:
            SYD << tr("....SafetWorkflow::getDocuments...recorriendo..JSON");
                str = mydocument.getJsonQuery(query,fields,howmanydocuments,info);
                break;
        case SafetWorkflow::JSONARRAY:
                SYD << tr("....SafetWorkflow::getDocuments...recorriendo..JSONARRAY");
            str = mydocument.getJsonArrayQuery(query,fields,howmanydocuments,info);
            break;
            case SafetWorkflow::SVG:
                break;
            default:;
        }
    }
    SYD << tr("....SafetWorkflow::getDocuments...entrando...(before)...howmanydocuments:|%1|")
           .arg(howmanydocuments);

    SYD << tr("....SafetWorkflow::getDocuments...recorriendo..(1)");
    foreach (QString myfile, mydocument.availableFiles()) {
        SYD << tr("....SafetWorkflow::getDocuments...avalaible.file: |%1|")
               .arg(myfile);

    }
    SYD << tr("....SafetWorkflow::getDocuments...recorriendo..(2)");
    SYD << tr(".......................................->SafetWorkflow::getDocuments....str:|%1|")
           .arg(str);

    if ( SafetYAWL::curOutputInterface != NULL ) {
        QMap<QString,QString> mymap;
        mydocument.setDocuments(SafetYAWL::curOutputInterface->renderDocuments(str,mymap));
        SYD << tr(".............SafetWorkflow::**getDocuments..(after render)..str:|%1|")
               .arg(str);


    }


    return mydocument;
}

SafetDocument SafetWorkflow::getDocuments(const QString& idvariable,
                                    OutputFormat of,const QString& info) {
    QList<QSqlField> fields;
    int howmanydocuments;

    SYD << tr("................SafetWorkflow::getDocuments....of:|%1|")
           .arg(of);
    QString str;
    SafetVariable *curvar = searchVariable( idvariable );
    Q_CHECK_PTR( curvar );

    QSqlQuery query(SafetYAWL::currentDb); // <-- SafetYAWL::currentDb puntero a db actual
    query = getSQLDocuments(*curvar);
    SafetDocument mydocument;
    howmanydocuments =     SafetWorkflow::getQuerySize(query);
    SYD << tr("................SafetWorkflow::getDocuments....howmanydocuments:|%1|")
           .arg(howmanydocuments);

    if (howmanydocuments >  0 ) {
        switch (of) {
            case SafetWorkflow::XML:
                str = mydocument.getXmlQuery(query,howmanydocuments,info);
                break;
            case SafetWorkflow::JSON:
                str = mydocument.getJsonQuery(query,fields,howmanydocuments,info);
                break;
            case SafetWorkflow::JSONARRAY:
                SYD << tr("........:SafetWorkflow::getDocuments....getJsonArrayQuery....");
                str = mydocument.getJsonArrayQuery(query,fields,howmanydocuments,info);
                break;
            case SafetWorkflow::SVG:
                break;
            default:;
        }
    }

    SYD << tr(".............SafetWorkflow::getDocuments....str:|%1|")
           .arg(str);
    if ( SafetYAWL::curOutputInterface != NULL ) {
        QMap<QString,QString> mymap;
        mydocument.setDocuments(SafetYAWL::curOutputInterface->renderDocuments(str,mymap));
    }

    return mydocument;
}





QList<QString> SafetWorkflow::getFieldsOfDocument(const QString& idtask) {
	QList<QString> mylist;
	SafetTask *curtask = searchTask(idtask);
	Q_ASSERT_X(curtask != NULL,"getFieldsOfDocument", qPrintable(tr("No se encuentra en el flujo de trabajo la tarea : %s").arg(qPrintable(idtask))));
	SafetVariable *curvar;
	//QSqlQuery query;
	QSqlQuery query(SafetYAWL::currentDb);
	QList<SafetVariable*>::iterator j;
	for (j = curtask->getVariables().begin(); j != curtask->getVariables().end(); ++j) {
		curvar = *j;
		query = getSQLDocuments(*curvar);
		break; // Documento para una sola variable, se necesita verificar el algoritmo para el caso que existen dos o mas variables
	}
	
	Q_ASSERT( curtask->getVariables().size() > 0);	
	SafetDocument mydocument;
        if ( SafetWorkflow::getQuerySize(query) >  0 ) {
		mylist = mydocument.getFields(query);
	}						
	return mylist;
}


QList<QVariant::Type> SafetWorkflow::getInfosOfDocument(const QString& idtask) {
	QList<QVariant::Type> mylist;
	SafetTask *curtask = searchTask(idtask);
	Q_ASSERT_X(curtask != NULL,"getFieldsOfDocument", qPrintable(tr("No se encuentra en el flujo de trabajo la tarea : %s").arg(qPrintable(idtask))));
	SafetVariable *curvar;
	//QSqlQuery query;
	QSqlQuery query(SafetYAWL::currentDb);
	QList<SafetVariable*>::iterator j;
	for (j = curtask->getVariables().begin(); j != curtask->getVariables().end(); ++j) {
		curvar = *j;
		query = getSQLDocuments(*curvar);
		break; // Documento para una sola variable, se necesita verificar el algoritmo para el caso que existen dos o mas variables
	}
	
	Q_ASSERT( curtask->getVariables().size() > 0);	
	SafetDocument mydocument;
        if ( SafetWorkflow::getQuerySize(query) >  0 ) {
		mylist = mydocument.getInfos(query);
	}						
	return mylist;
}

QSqlQuery SafetWorkflow::getQueryFromTask(const QString& idtask) {
	SafetTask *curtask = searchTask(idtask);
	Q_ASSERT_X(curtask != NULL,"getFieldsOfDocument", qPrintable(tr("No se encuentra en el flujo de trabajo la tarea : %s").arg(qPrintable(idtask))));
	SafetVariable *curvar;
	//QSqlQuery query;
	QSqlQuery query(SafetYAWL::currentDb);
	QList<SafetVariable*>::iterator j;
	for (j = curtask->getVariables().begin(); j != curtask->getVariables().end(); ++j) {
		curvar = *j;
		query = getSQLDocuments(*curvar);
		break; 
	}
	return query;	
}


int SafetWorkflow::numberOfTokens(const QString& idtask) {
        int result = 0;
//	SafetTask *curtask = searchTask(idtask);
//	Q_ASSERT_X(curtask != NULL,"numberOfTokens", qPrintable(tr("No se encuentra en el flujo de trabajo la tarea : %s").arg(qPrintable(idtask))));
//	SafetVariable *curvar;
//	QList<SafetVariable*>::iterator j;
//	for (j = curtask->getVariables().begin(); j != curtask->getVariables().end(); ++j) {
//		curvar = *j;
//		result = result + numberOfTokens(*curvar);
//	}
	return result;
}

SafetTask* SafetWorkflow::searchTask(const QString& idtask) {
        foreach(SafetTask* curtask, tasklist) {
//                qDebug("....*curtask: %s", qPrintable(curtask->id()));
                Q_CHECK_PTR( curtask );
		if (curtask->id() == idtask) {
			return  curtask;
		}
	}
	return NULL;
}





SafetCondition* SafetWorkflow::searchCondition(const QString& idcon) {
        foreach(SafetCondition* cur, conditionlist) {
                Q_CHECK_PTR( cur);
                if (cur->id() == idcon) {
                        return  cur;
                }
        }
        return NULL;
}


int SafetWorkflow::numberOfTokens(const SafetVariable& v) {
    QSqlQuery myquery =  getSQLDocuments(v);
           int result = SafetWorkflow::getQuerySize( myquery );
           return result;
}

QString  SafetWorkflow::getStackExpression(const SafetVariable& v, QStack<QString> &splitresults) {

	QString sql = v.documentsource();
	Q_CHECK_PTR( SafetYAWL::evalExit() );

        if ( sql.length() == 0  ) {
            SafetYAWL::streamlog
                    << SafetLog::Error
                    <<
                    tr("La sentencia sql \"%1\" tiene longitud cero").arg(sql);
            return QString("");

     }

	Q_CHECK_PTR(ptoken);	
	Q_ASSERT_X(sql.length() > 0, "getStackExpression", qPrintable(tr("La sentencia sql tiene longitud cero")));
	SafetSQLParser localparser;
        localparser.setWorkflow(this);
	localparser.setStr(sql);
	localparser.parse();
        if (localparser.error() ==  SafetParser::INCORRECT) {
            SafetYAWL::streamlog
                    << SafetLog::Error
            << tr("NO es correcta la sentencia SQL: \"%1\" ").arg(sql);
            return QString("");

        }
	_tablesource = localparser.getTablesource();	
	// Sentencias Where para las VARIABLES (Documentos)
	for (int i=0; i< localparser.getWhereclauses().count(); i++ ) {
		whereset.push_back( localparser.getWhereclauses().toList().at(i));
	}


        joinWhereset += localparser.getJoinclauses();
	// Se debe relacionar con los joins necesarios para realizar la relaciÃ³n con el token 
//        qDebug("**...(1)...sql: %s", qPrintable(sql));
	sql = localparser.dropWhereClauses();
 //       qDebug("**...(2)...sql: %s", qPrintable(sql));
        QRegExp rx("^\\s*SELECT\\b"); // Colocar el campo clave en la lista de campos de la sentencia SQL
    	rx.setCaseSensitivity(Qt::CaseInsensitive);
	sql.replace(rx,"SELECT "+getToken()->key()+",");

	 SafetTask* mytask = qobject_cast<SafetTask*> (v.parent());
	 Q_CHECK_PTR( mytask );

     generateKeyFilterStack( mytask->id(), splitresults); // generar la pila de resultados dado
	 // identificador de la tarea padre de la variable
	//results =   generateKeyFilterStack( v.parent() ); // generar la pila de resultados dado el identificado dela tarea padre de la variable buscada

	_tablesource = localparser.getTablesource();	
        //SafetYAWL::debugQueue( tokenlinkqueue );
        //SafetYAWL::debugQueue( tokenlinkqueueInPath );


	return sql;
}



QString SafetWorkflow::removeOutParenthesis(const QString& s) {
    QString result = s;

    QRegExp rxInitial;
    QRegExp rxEnd;

    rxInitial.setPattern("(\\()+(\\s*SELECT)");
    rxEnd.setPattern("(\\)\\s*)(\\))+(\\s+UNION)");

    int pos = 0;
    SYD << tr("..........**SafetWorkflow::removeOutParenthesis....(1)...s:|%1|" )
    .arg(s);
    while (1) {
        pos = rxInitial.indexIn(result,pos);
//        pos = result.indexOf(rxInitial,pos);
        SYD << tr("..........**SafetWorkflow::removeOutParenthesis....pos:|%1|...")
               .arg(pos);
        if (pos == -1 ) {
            break;
        }

        SYD << tr("..........**SafetWorkflow::removeOutParenthesis....rxInitial.cap(0):|%1|...")
               .arg(rxInitial.cap(0));
        SYD << tr("..........**SafetWorkflow::removeOutParenthesis....rxInitial.cap(1):|%1|...")
               .arg(rxInitial.cap(1));
        SYD << tr("..........**SafetWorkflow::removeOutParenthesis....rxInitial.cap(2):|%1|...")
               .arg(rxInitial.cap(2));
        result.replace(rxInitial.cap(0), rxInitial.cap(2));
        pos += rxInitial.cap(0).length()+1;
    }

    pos = 0;
    while (1) {
        pos = rxEnd.indexIn(result,pos);
//        pos = result.indexOf(rxEnd,pos);
        if (pos == -1 ) {
            break;
        }
        result.replace(rxEnd.cap(0), rxEnd.cap(1)+rxEnd.cap(3));
        pos += rxEnd.cap(0).length()+1;
    }
    SYD << tr("........SafetWorkflow::removeOutParenthesis....(2)...s:|%1|" )
           .arg(result);



    return result;

}




QSqlQuery SafetWorkflow::getSQLDocuments(const SafetVariable& v) {
	resetContainers();	
   QString idvariable =   v.id();
    if (_cache.contains(idvariable) ) {
        return _cache[idvariable];
    }


	QString sql = v.documentsource();
    QStack<QString> splitresults;
    sql  =getStackExpression(v, splitresults);  // Obtener la pila de resultado desde la variable
        redefineSplitOperators(splitresults);

	QString splitoperation = evalSQLKeyFilterStack(sql, splitresults);



    QRegExp rx("\\s+AND\\s+\\(?\\s*TRUE\\s*\\)?");
	rx.setCaseSensitivity( Qt::CaseInsensitive );
	splitoperation.remove( rx );

        if ( SafetYAWL::currentDb.driverName().toUpper() == "QSQLITE"
             /* Chequear si es SQLITE */ )  {
            splitoperation = removeOutParenthesis(splitoperation);
        }


        SYD << tr("........(before)...SafetWorkflow::getSQLDocuments SPLITOPERATION:|%1|")
               .arg(splitoperation);

        SYD << tr("........(after)...GROUPBY...SafetWorkflow::getSQLDocuments SPLITOPERATION:|%1|")
               .arg(splitoperation+ " " + v.groupby());

        if (!v.groupby().isEmpty()) {
            splitoperation = splitoperation+ " " + v.groupby();
        }

	QSqlQuery query(splitoperation,SafetYAWL::currentDb); // <-- SafetYAWL::currentDb puntero a db actual


        if ( query.lastError().type() != QSqlError::NoError ) {
                
                      SYE   <<tr("Error SQL tipo: \"%2\". Mensaje: \"%3\". Errores en la  Sentencia SQL:\n\"%1\"")
                        .arg(splitoperation)
                        .arg(query.lastError().type())
                        .arg(query.lastError().driverText());

           return query;
        }

        if (SafetWorkflow::getQuerySize(query) == 0 ) {

                    //SYD <<tr("El resultado de la Sentencia SQL:\n\"%1\" no presenta registros").arg(splitoperation);
        }


      if (!idvariable.isEmpty()) {
          SYD << tr("...SafetWorkflow::getSQLDocuments...adding variable:|%1|").arg(idvariable);
        _cache[idvariable]  = query;
      }


//            SYA << tr(">>> Sentencia SQL:\n\"%1\"\n ejecutada correctamente...OK!").arg(splitoperation);

	return query;	
}

void SafetWorkflow::resetContainers() {
	whereset.clear(); 
	opeStack.clear();
	joinOpeStack.clear();
	joinWhereset.clear();
	tokenlinkqueue.clear();
	lastkeyset.clear();
	tokenlinkqueueInPath.clear();
	currentfilterkey = "";
	// Reiniciar las variables
}

QString SafetWorkflow::evalSQLKeyFilterStack(const QString& sql, QStack<QString> &e) {
	QStack<QString> mystack;
        //SafetYAWL::debugStack( e ); // Funcion agregada para listar las tareas de la pila
	if ( e.count() ==  1 ) {
		QString newsql = e.pop();
		QString path = newsql.section('|',1,1);
		newsql.remove("|" + path);
		SafetSQLParser localparser;
                localparser.setWorkflow(this);
		localparser.setStr(sql);
		localparser.parse();
                QString tokenlink = generateJoinString(path.toInt(), localparser.getTablesource() );
               SYD       << tr("Tokenlink: \"%1\"").arg(tokenlink);

		QString joinsql = sql + tokenlink;
		QString result  = addSQLPrefix(joinsql, newsql);	
        SYD << tr("...........SafetWorkflow::evalSQLKeyFilterStack.........Consulta 1: \"%1\"").arg(result);
  		return   result;
	}
	while (!e.isEmpty() ) {
		QString newsql, result = e.pop();
			
		if ( getSQLOperator(result).length() == 0 ) {
        	              mystack.push(result);

		} else  {
			newsql = processSQLOperation(sql, mystack, result);	
		}
 	}

// 	Q_ASSERT(mystack.count() > 0 );
        QString result = mystack.pop();

        return result;
}




QString SafetWorkflow::processTokenlink(const QString& s, bool final, int npath) {

	    QRegExp rx("(\\[[a-zA-Z_0-9\\.,:]*\\])");
	    int newpos, pos;
	    QString sql, str = s;
	    while (true) {
		pos = rx.indexIn(str);
		if ( pos < 0 ) break;
			QString internal = rx.cap(1);		    
			QRegExp rxInternal("([a-zA-Z_0-9\\.:]+),?([a-zA-Z_0-9\\.:]+)?");
			int posinternal = rxInternal.indexIn(internal);		    
			QStringList links = rxInternal.capturedTexts();
			if ( posinternal >  -1 )  {	
				QString sqltoken = generateSQLToken(links, final,npath);
				sql = sql + sqltoken;
			}
		newpos = pos+rx.matchedLength();
		str = str.mid(newpos);
	    }
	return sql;

}

QString SafetWorkflow::generateSQLToken(const QStringList &list, bool final, int npath) {


    SafetYAWL::streamlog
            << SafetLog::Debug
            << tr("Generando generateSQLToken");
    foreach(QString l, list) {
        SafetYAWL::streamlog
                << SafetLog::Debug
                << tr("   -->  l:%1").arg(l);

    }

    if ( ptoken == NULL ) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("No se ha cargado en el archivo de flujo de trabajo el elemento \"token\"."
                      "Se sugiere colocarlo al principio del archivo de flujo de trabajo, para que sea leído de primero");
            return QString("");
    }
	Q_ASSERT_X(_tablesource.length() > 0, "generatSQLToken", 
		qPrintable(tr("La tabla origen no tiene ningun nombre")));
	QString sql, linkleft, linkright, jointable; // algunas variables necesarias para generar el enlace a la ficha (tokenlink)
	if (list.at(2).length() == 0 ) { // Caso de una sola tabla en el tokenlink
//            SafetYAWL::streamlog
//                    << SafetLog::Debug
//                    << tr("(Sola) Analizando expresion: |%1|")
//                    .arg(list.at(1));
		sql =  " JOIN " + ptoken->keysource();
		bool found = false;
		foreach(QString s, tokenlinkqueue) {
			if ( s.indexOf( QRegExp("^" + sql+"\\b",Qt::CaseInsensitive) ) > -1   ) {
				found = true;
				break;
			}
		}

                QRegExp rx("([a-zA-Z_0-9\\.]+):?([a-zA-Z_0-9\\.\\(\\)]+)?");
		int pos = rx.indexIn(list.at(1));
                if (pos < 0 ) {
                    SafetYAWL::streamlog
                            << SafetLog::Error
                            << tr("El siguiente tokenlink  \"%1\" presenta errores").arg(list.at(1));
                       return QString();
                }

		QString left,right;		
		left = rx.cap(1);
		right = rx.cap(2);
                SafetYAWL::streamlog
                        << SafetLog::Debug
                        << tr("rx(1): |%1| rx(2): |%2|")
                        .arg(left)
                        .arg(right);

		if (right.length() > 0  ) { // Caso de una sola tabla en el tokenlink con tabla explicita
                    SafetYAWL::streamlog
                            << SafetLog::Debug
                            << tr("right.length() > 0");

                        QString mysql = " JOIN " +  right + " ON " + right + "." + left
			+ " = " + ptoken->keysource() + "." +  ptoken->key();					
			if ( npath > -1 )  {						
				mysql = mysql + "|" + QString("%1").arg(npath);
			}
			if ( !tokenlinkqueueInPath.contains(mysql) ) {

                            SafetYAWL::streamlog
                                    << SafetLog::Debug
                                    << tr("Agregando a cola tokenlinkqueueInPath: |%1|")
                                    .arg(mysql);
				tokenlinkqueueInPath.enqueue(mysql);
			}			
		}	
                SafetYAWL::streamlog
                        << SafetLog::Debug
                        << tr("sql");

                sql = sql + " ON " + left + "=" + ptoken->key();
                SafetYAWL::streamlog
                        << SafetLog::Debug
                        << tr("found: %2 ... final: %3 sql....|%1|")
                        .arg(sql)
                        .arg(found)
                        .arg(final);

                if ( !found  && final ) {
                    SafetYAWL::streamlog
                            << SafetLog::Debug
                            << tr("Agregando a cola tokenlinkqueue: |%1|")
                            .arg(sql);

			tokenlinkqueue.enqueue( sql );
		}		
		return sql;
	}
	
	for(int i=1; i < list.size(); i++) {
//            SafetYAWL::streamlog
//                    << SafetLog::Debug
//                    << tr("Analizando expresion: |%1|")
//                    .arg(list.at(i));

		QRegExp rx("([a-zA-Z_0-9\\.]+):?([a-zA-Z_0-9\\.]+)?");
		int pos = rx.indexIn(list.at(i));
		if ( pos > -1 ) {
			if (linkleft.length() == 0 ) {
				linkleft = rx.cap(1);
				if (rx.cap(2).length() > 0  ) {
					jointable = rx.cap(2);
				}	
			}
			else if (rx.cap(2).length() > 0  ) {
				linkright = rx.cap(1);
				if ( jointable.length() > 0  ) {
					QString mysql = " JOIN " +  jointable + " ON " + jointable + "." + linkleft
					+ " = " + rx.cap(2) + "." + linkright;					
					if ( npath > -1 )  {						
						mysql = mysql + "|" + QString("%1").arg(npath);
					}
					if ( !tokenlinkqueueInPath.contains(mysql)  && !final) {
                                            SafetYAWL::streamlog
                                                    << SafetLog::Debug
                                                    << tr("Agregando a cola (2)tokenlinkqueueInPath: |%1|")
                                                    .arg(mysql);
						tokenlinkqueueInPath.enqueue(mysql);
					}
					sql = " JOIN " +  rx.cap(2) + " ON " + _tablesource + "." + linkleft
                                        + "=" + rx.cap(2) + "." + linkright;
					jointable = "";
				}  else {				
					sql = " JOIN " +  rx.cap(2);
					bool found = false;
					foreach(QString s, tokenlinkqueue) {
						if ( s.indexOf( QRegExp("^" + sql+"\\b",Qt::CaseInsensitive) ) > -1   ) {
							found = true;
							break;
						}
					}
					sql = sql + " ON " + _tablesource + "." + linkleft
					+ " = " + rx.cap(2) + "." + linkright;
					if ( !found  && final ) {
                                            SafetYAWL::streamlog
                                                    << SafetLog::Debug
                                                    << tr("Agregando a cola (2) tokenlinkqueue:|%1|")
                                                    .arg(sql);
						tokenlinkqueue.enqueue( sql );
					}		
				}			
				_tablesource = rx.cap(2);
				return sql;
			}
		}
	}    
	return QString();
}



QString SafetWorkflow::printPaths(const QString& idtask, const QString& info) {
	QList<QQueue<QString> > paths = generatePaths(idtask);  // Generar los caminos a un nodo
        Q_CHECK_PTR( SafetYAWL::evalExit() );
        SafetYAWL::evalAndCheckExit(paths.count() > 0, tr("No existen caminos para la tarea indicada: \"%1\" (Revise el documento de flujo de trabajo)").arg(idtask));
	QString result;
	for( int i=paths.count()-1; i>=0 ; i--) {
		
		QQueue<QString> path = paths.at(i);
		while(!path.empty()) {
			QString nodename = path.dequeue();
			SafetNode *node = nodemap[ nodename ];			
			Q_CHECK_PTR( node );
			int type = SafetYAWL::getClassReference(SafetYAWL::translateNS(node->metaObject()->className()));
			if ( type == 2 ) { // Nodo es una Tarea
				QSet<QString> keys = getKeys(nodename);
				if ( keys.contains(info) ) {
					result += "[ " +nodename + " ]";		
				}	
				else {
					result += nodename;		
				}
				
			}
			else {
					result += "( *" +nodename + " )";		
			}
			result += "-->";	
		}
		result.chop(3);
		result += "\n";
	}
	return result;
}



void SafetWorkflow::redefineSplitOperators(QStack<QString>& splitresults){

    QList<SafetPort*> myports;
    foreach(SafetTask* task, tasklist){
        if (task->inport() != NULL) {
            myports.append(task->inport());
        }
    }

    int countport = 0;
    for(int i=0; i< splitresults.count();i++){
        if (SafetYAWL::PATTERNS.contains(splitresults.at(i)) ) {
            if ( countport < myports.count()) {
                QString newpattern = myports.at(countport)->pattern();
                splitresults[i] = newpattern;
            }
        }
    }

}



void SafetWorkflow::generateKeyFilterStack(const QString& idtask, QStack<QString>& splitresults){

    qDebug("............generating paths..........(1)....");
    QList<QQueue<QString> > paths = generatePaths(idtask);  // Generar los caminos a un nodo
    qDebug("............generating paths..........(2)....");
    //SafetYAWL::debugPaths(paths); // Depuracion de la lista de caminos
    if (paths.count() ==  0) {

        SYE << tr("No existen caminos para la tarea indicada: \"%1\" (Revise el documento de flujo de trabajo)").arg(idtask);
        return;
    }

    QSet<QString> myset;
    QString ope;

    lastkeyset.clear();

    //int pathssize = paths.size();
    int ipath = 0;
    SafetPort *splitPort = NULL, *joinPort = NULL;

    //QMap<QString,QSet<QString> > dropkey;
    for( int i=paths.count()-1; i>=0 ; i--,ipath++) {

        if ( !opeStack.isEmpty() ) {
            ope =  opeStack.pop(); // Chequear operadores
            if ( getSQLOperator(ope).length() > 0 ) {
                splitresults.push(ope);
                ope =  opeStack.pop(); // Visualizar el operador aqui
            }
        }

        QQueue<QString> path = paths.at(i);
        QStack<QString> reversepath, prevnodes;
        QString splitresult, joinresult;
        int countprevnodes = 0;

        // Recorrido (Caminata por cada Path)
        countprevnodes = walkPath(path,reversepath,prevnodes,splitresult,joinresult,splitPort,joinPort,idtask,ipath); // Recorrido


        myset = lastkeyset; // Se actualiza el conjunto de claves (lastkeyset)
        lastkeyset.clear();
        //QString keyresult = generateKeyFilterString();
        generateKeyFilterString();

        splitresult = generateWhereClauses(splitresult,ipath);        
        //SYD << tr("Consulta Nro %1 generada: \"%2\"").arg(ipath).arg(splitresult);
        splitresults.push(splitresult+ "|" + QString("%1").arg(ipath));//Agregar numero de camino
        // Operador SPLIT  JOIN
        if ( countprevnodes > 0 ) {
            joinresult  = joinresult + ((joinWhereset.count() > 0 ) ? " AND " : " ");
            for(int i = 0; i < joinWhereset.count(); i++) {
                joinresult = joinresult + joinWhereset.toList().at(i);
                if (i < joinWhereset.count()-1) joinresult += " AND ";
            }
            joinWhereset.clear();
        }
        isStartingKeyFilter = true;
        keysoptions.clear();
        dropkeys.clear();
        currentfilterkey = "";
    }
    lastkeyset = myset;
}


int SafetWorkflow::walkPath(QQueue<QString>& path, QStack<QString>& reversepath, QStack<QString>& prevnodes, QString& splitresult, QString& joinresult,SafetPort* splitPort, SafetPort* joinPort, const QString& idtask, int ipath) {
    QString nodename, prevnodename;
    SafetNode *node, *nextnode;
        SafetNode::PortType type = SafetNode::SPLIT;
    int countprevnodes = 0;
    int curroption = 0;
    while(!path.empty()) {
        QString query;
//          qDebug("...inside walkPath...(1)...");
        nodename = path.dequeue();
//          qDebug("......nodename: %s", qPrintable(nodename));
        node = nodemap[ nodename ];
//          qDebug("......nodename: %s", qPrintable(nodename));
        Q_CHECK_PTR( node );
        splitPort    =  getOperationPort(type, node); // Obtener el puerto del siguiente/anterior nodo
        nextnode = getOperationNode(type, path, reversepath);
        curroption = calculateCurroption(SafetNode::SPLIT,node,nextnode); // Calcular la  ruta actual
        if (idtask.compare(node->id(), Qt::CaseInsensitive) == 0) break;
        if ( splitPort != NULL ) {
            query  = generateSQLPortKeyFilterExpression(splitPort, curroption,ipath);
//            SafetYAWL::streamlog
//                    << SafetLog::Debug
//                    << tr("              ***generateSQLPortKeyFilterExpression...:%1")
//                    .arg(query);
            splitresult   = generateKeyFilterExpression(query, node, curroption,path,false,ipath);
//            SafetYAWL::streamlog
//                    << SafetLog::Debug
//                    << tr("              ***generateSQLPortKeyFilterExpression..(splitresult).:%1")
//                    .arg(splitresult);

        }

        joinPort = getOperationPort(SafetNode::JOIN, node);

        if (  joinPort != NULL ) {
            if ( prevnodes.isEmpty() ) {
                prevnodes = node->prev();
            }
            SafetNode* prevnode = getOperationNode(SafetNode::JOIN, path, reversepath);
            int prevoption = calculateCurroption(SafetNode::JOIN,node,prevnode);
            prevnodename = prevnode->id();
            if  ( prevnodes.contains( prevnodename ) ) {
                Q_CHECK_PTR( prevnode );
                splitPort    =  getOperationPort(SafetNode::SPLIT, prevnode);
                query  = generateSQLPortKeyFilterExpression(splitPort, prevoption,ipath);
                joinresult   = generateKeyFilterExpression(query,prevnode, curroption, path,true,ipath);
                countprevnodes++;
            }
        }
//          qDebug("...inside walkPath...(4)...");
        reversepath.push( nodename );
    }
    return countprevnodes;
}


int SafetWorkflow::calculateCurroption( SafetNode::PortType type, SafetNode *node, SafetNode *nextnode) {
    if ( nextnode == NULL ) return 0;
    Q_CHECK_PTR(node);
    int i;

    SafetPort* myport = NULL;
    if ( type == SafetNode::SPLIT )
        myport = node->outport();
    else if ( type == SafetNode::JOIN )
        myport = node->inport();
    Q_CHECK_PTR( myport );
    for(i = 0; i< myport->getConnectionlist().count(); i++) {
        SafetConnection* con = node->port()->getConnectionlist().at(i);
        Q_CHECK_PTR( con );
        if ( con->source().compare(nextnode->id(),Qt::CaseInsensitive) == 0 ){
            return i;
        }
    }
    SafetYAWL::evalAndCheckExit( i < node->port()->getConnectionlist().count(), tr("La tarea o condicion con el identificador: \"%1\" no esta conectada correctamente").arg(node->id()));
    return 0;
}

SafetNode* SafetWorkflow::getOperationNode(SafetNode::PortType type, const QQueue<QString>& path,
                                                                                const QStack<QString>& reversepath ) {
        SafetNode* mynode = NULL;
        switch (type) 	{
                case SafetNode::SPLIT:
                                if ( !path.empty() ) mynode = nodemap[ path.head() ];
                                break;
                case SafetNode::JOIN:
                                if ( !reversepath.isEmpty() ) mynode = nodemap[ reversepath.top() ];
                                break;
                default:;
        }
        return mynode;
}

SafetPort* SafetWorkflow::getOperationPort(SafetNode::PortType type, SafetNode* node) {
	SafetPort* myport = NULL;
	Q_CHECK_PTR( node );
	switch(type) {
		case SafetNode::SPLIT:
				myport = node->outport();
				break;
		case SafetNode::JOIN:
				myport = node->inport();
				break;				
		default:;
	}
	return myport;
}


QList<QQueue<QString> > SafetWorkflow::generatePaths(const QString& idtask, ItemSearched t) {
	SafetCondition* start = getStartCondition();
	SafetNode* node = start, *parent;
	QStack<SafetWorkflow::PathHistory> localstack;
	QList<QQueue<QString> > paths;
	QString nodename = start->id();	
        QQueue<QString> mypath;
        while ( true ) {
                 if (idtask == nodename) { // Aca encuentra el camino

                  if ( !mypath.contains(nodename))  {
                        mypath.enqueue(nodename);
                  }

			 	  paths.push_front(mypath);	  
			 	  if (localstack.isEmpty() )   break;
 		    	  SafetWorkflow::PathHistory h = localstack.pop();
                              nodename = h.nodename();
			      mypath = h.history();
			   	  node = nodemap[ nodename ];
			   	  Q_CHECK_PTR( node );	
			   	  if ( mypath.count() > 0 ) {			   		  
			   		  parent = nodemap[ mypath.last() ];
					  SafetPort* parentport = parent->port();
					  Q_CHECK_PTR( parentport );
			   		  if ( getSQLOperator(parentport->pattern()).length() > 0 ) {
				   		  Q_CHECK_PTR(parent);
			   		  }
			   	  }
		 }			
        if ( !mypath.contains(nodename))  {
            mypath.enqueue(nodename);
        }
		nodename = pushLeftChild(node, localstack, mypath);
		if ( nodename.length() == 0 ) 	{
			while (!localstack.isEmpty() && nodename.length() == 0) {
				SafetWorkflow::PathHistory h = localstack.pop();
				nodename = h.nodename();
				mypath = h.history();
			}			
			if ( nodename.length() != 0) {
				node = nodemap[ nodename ];
				Q_ASSERT(node);
			   	  if ( mypath.count() > 0 ) {			   		  
			   		  parent = nodemap[ mypath.last() ];
			   		  Q_CHECK_PTR(parent);
			   	  }
				continue;
			}
			if (localstack.isEmpty() ) {			
				if ( mypath.last().compare( idtask,Qt::CaseInsensitive)==0 ) paths.push_front( mypath );
				break;			
			}
		}
		node = nodemap[ nodename ];					
	}
	return paths;
}

QStringList SafetWorkflow::searchNextValues(const QString& keyvalue) {
     QStringList result;


     return result;
}


QStringList  SafetWorkflow::getTempNameFiles(int n) {
    QStringList list;
    if (n == 0 ) return list;
    Q_ASSERT(n > 0 );
    
    QList<QTemporaryFile*> filelist;
    
    for (int i= 0; i < n; i++) {
    	filelist.push_back(new QTemporaryFile());
    	Q_CHECK_PTR( filelist.at(i) );
    	bool result = filelist.at(i)->open();
    	Q_ASSERT_X(result, "getTempNameFile", qPrintable(tr("No fue posible abrir el archivo temporal")));
    	list.push_back(filelist.at(i)->fileName());
    }

    Q_ASSERT(n == filelist.count()); // Deben ser igual el numero de la lista de archivos
    for (int i= 0; i < n; i++) {
    	filelist.at(i)->close();
    	delete filelist.at(i); 
    }
    return list;
}

QString SafetWorkflow::pushAllChild(SafetNode *node,std::stack<QString>& localstack,
                                    QMap<QString,QString>& results,
                                    const QString& info,
                                    QMap<int,QString>& orderresults,
                                    uint& countresults,
                                    bool norender) {
     Q_CHECK_PTR(node);
     QString pop = "";
     QStack<QString> childstack = node->next();
     if ( childstack.isEmpty() ) {
         return pop;
     }
     QString newedge;

//     SYD  << tr("............SafetWorkflow::generateCodeGraph.....pushAllChild TIMEPRINT (1)");
     QRegExp rx("Siguiente:\\s*([a-zA-Z_0-9;\\.]+)\\s*(,[a-zA-Z_0-9]+)?");
     while( !childstack.isEmpty()) {
         QString childnamenode = childstack.pop();
         if (childnamenode.isEmpty() ) {
             continue;
         }
         SafetNode* childnode = nodemap[ childnamenode ]; // Colocar el camino, por si es
         // necesario
         Q_CHECK_PTR(childnode);


         newedge = printNodeInformation(node, childnamenode, "", info,norender);
         if ( /* !localstack.contains(childstack[i])  && */ !isBackNode(node,childnamenode) )  {
             localstack.push(childnamenode);
         }


         int pos = rx.indexIn( results[ node->id() ]);

         if ( pos != -1 ) {
             QString oldnodes = rx.cap(1);
             newedge = printNodeInformation(node, childnamenode, oldnodes,info,norender);
         }
         if ( !(node->id().isEmpty())) {
             results[ node->id() ] = newedge;

             if ( !visitnodemap[node->id()]) {
                 orderresults[ countresults ]  = node->id();
                 countresults++;
             }
         }


     }
  //   SYD  << tr("............SafetWorkflow::generateCodeGraph.....pushAllChild TIMEPRINT (2)");
     if (localstack.empty()) {
         return pop;
     }
     pop = localstack.top();
     localstack.pop();

     return pop;
}

bool SafetWorkflow::isBackNode(SafetNode* node, const QString& nodename) {


    if (node == NULL) {
        SYW << tr("Nodo de prueba (back) es nulo");
        return false;
    }

    int count = node->outport()->getBackList().count();

    for(int i = 0; i < count; i++) {
        if (nodename == node->outport()->getBackList().at(i)->source()) {
            return true;
        }

    }



    return false;
}

QString SafetWorkflow::printNodeInformation(SafetNode *node, const QString& nextnode,
                                             const QString& oldnextnode,
                                             const QString& info,
                                             bool norender) {
     Q_CHECK_PTR( node );
     QString newnode, result;
     SafetWorkflow* wf = qobject_cast<SafetWorkflow*>(this);
     Q_CHECK_PTR( wf );


     Safet::SafetStats *_mystats = new Safet::SafetStats();
     Q_CHECK_PTR( _mystats);
     _mystats->setWorkflow( wf );

     if ( numberOfTokens() == -1 ) {
           _mystats->setWorkflow( qobject_cast<SafetWorkflow*>(this) );

     }


     if ( oldnextnode.length() == 0 ) {
          newnode = nextnode;
     }
     else if ( oldnextnode.indexOf( nextnode ) == -1 ) {
          newnode = oldnextnode + ";" + nextnode;
     }
     else {
          newnode = oldnextnode;
     }
     Safet::SafetStats::StatsType t = Safet::SafetStats::None;
     QString myinfo;

     if (info.compare(tr("coloured"), Qt::CaseInsensitive) == 0 ) {
          t = Safet::SafetStats::Coloured;
     }
     else if ( info.length() > 0 ) {
          t = Safet::SafetStats::ForTokenKey;
     }
     if ( t != Safet::SafetStats::None ) {
          QString attr = SafetYAWL::getConf()["Plugins.Graphviz/stats.attribute"];
          QStringList infos = _mystats->processInfo(node, t, attr,norender,info);
          if (infos.count() > 0 ) {
            myinfo =  infos.at(0);
            if ( infos.count() > 1 ) {
                infonodemap[node->id()] = infos.at(1);
            }
          }

     }
     QString typeNode = "Task";
     int ntype = SafetYAWL::getClassReference
                 (SafetYAWL::translateNS(node->metaObject()->className()));
     if ( ntype == 5 ) typeNode = "Condition";

     QString nodetitle = node->title();

     SafetTask* mytask = qobject_cast<SafetTask*>(node);
     QString title = (nodetitle.length() == 0)?tr(""):tr(", ")
                     + tr("Titulo:")+nodetitle+",";
     QString noderole;
     if (mytask != NULL && !mytask->role().isEmpty()) {
         noderole = QString(",Rol:%1,").arg(calculateSQL(mytask->role(),info));
      }

     QString nodenote;
     if (mytask != NULL && !mytask->note().isEmpty()) {
         nodenote = QString(",Nota:%1,").arg(mytask->note());
      }

     QString nodetextinfo;
     QString currvariable = "n/a";
     if (mytask != NULL && !mytask->textualinfo().isEmpty()) {
         QString postfix = SafetYAWL::getConf()["GeneralOptions/textualinfo.postfix"];
         nodetextinfo = QString(",TextualInfo:%1%2,").arg(calculateSQL(mytask->textualinfo(),info)).arg(postfix);

      }

     if (mytask != NULL ) {
         SafetVariable *myvar = mytask->getVariables().at(0);
         if (myvar != NULL ) {
             currvariable = myvar->id();
         }
     }

     QString moreinfo = myinfo.length() == 0?tr(""):myinfo;
     saveStates(node->id(),newnode.split(";",QString::SkipEmptyParts));
     result  = tr("Nodo:") + node->id() +", " + tr("Tipo:")+typeNode+", "
               +tr("Siguiente:") + newnode +
               ", "
               +tr("Union:")+(node->inport()!=NULL?node->inport()->pattern():"none")
               +", "
	       +tr("Reporte:")+((node->report().isEmpty() || node->report() == "yes")?"yes":"no")
	       +", "	
           +tr("Patron:")+ node->port()->pattern()
           +", "
           +tr("Variable:")+ currvariable
           +title+noderole+nodenote+nodetextinfo+moreinfo;

     if (_mystats) {
         delete _mystats;
     }
     return result;
}

QString SafetWorkflow::calculateSQL(const QString& currsql, const QString& info, const QString& otherinfo) {

    QString result = "";
    QString df = "";
    QStringList mylist = currsql.split("::",QString::SkipEmptyParts);

    df = mylist.at(0);

    if (mylist.count() > 1 ) {
        result = mylist.at(1);
    }
    else if (mylist.count() == 1) {
        result = mylist.at(0);
    }
    else {
        return result;
    }


    if (!result.startsWith("SELECT ",Qt::CaseInsensitive)) {
        SYD << tr(".......SafetWorkflow::calculateSQL...returning df");
        return result;
     }

    SYD << tr(".......SafetWorkflow::calculateSQL...CALCULATESQL.....currsql (1): |%1|")
           .arg(result);

    SYD << tr(".......SafetWorkflow::calculateSQL...CALCULATESQL.....info: |%1|")
           .arg(info);

    QString newsql = result;

    newsql.replace("{#keyvalue0}",info);
    newsql.replace("{#keyvalue1}",otherinfo);
    QSqlQuery query( SafetYAWL::currentDb );
    query.prepare(  newsql );
    bool executed = query.exec();
    SYD << tr(".......SafetWorkflow::calculateSQL...CALCULATESQL.....newsql(2): |%1|")
           .arg(newsql);


    if (!executed ) {
        SYA
                <<
                   tr("NO se ejecut� correctamente la sentencia de calculo SQL: \"%1\"")
                   .arg(newsql);
        return df;
    }

    if (!query.next()) {
        SYA
                <<
                   tr("Advertencia: No existe el registro: \"%1\"")
                   .arg(result);
        return df;
    }
    result = query.value(0).toString();
    SYD << tr(".......SafetWorkflow::calculateSQL...CALCULATESQL.....result: |%1|").arg(result);

    return result;

}


QStringList SafetWorkflow::calculateListSQL(const QString& currsql, const QString& info, const QString& otherinfo) {

    QStringList resultlist ;
    QString result ;
    QString df = "";
    QStringList mylist = currsql.split("::",QString::SkipEmptyParts);

    df = mylist.at(0);

    if (mylist.count() > 1 ) {
        result = mylist.at(1);
    }
    else if (mylist.count() == 1) {
        result = mylist.at(0);
    }
    else {
        return resultlist;
    }


    if (!result.startsWith("SELECT ",Qt::CaseInsensitive)) {
        SYD << tr(".......SafetWorkflow::calculateSQL...returning df");
        resultlist.append(df);
        return resultlist;
     }

    SYD << tr(".......SafetWorkflow::calculateSQL...CALCULATESQL.....currsql (1): |%1|")
           .arg(result);

    SYD << tr(".......SafetWorkflow::calculateSQL...CALCULATESQL.....info: |%1|")
           .arg(info);

    QString newsql = result;

    newsql.replace("{#keyvalue0}",info);
    newsql.replace("{#keyvalue1}",otherinfo);
    QSqlQuery query( SafetYAWL::currentDb );
    query.prepare(  newsql );
    bool executed = query.exec();
    SYD << tr(".......SafetWorkflow::calculateSQL...CALCULATESQL.....newsql(2): |%1|")
           .arg(newsql);


    if (!executed ) {
        SYA
                <<
                   tr("NO se ejecut� correctamente la sentencia de calculo SQL: \"%1\"")
                   .arg(newsql);
        resultlist.append(df);
        return resultlist;
    }

    while (query.next()) {
        resultlist.append(query.value(0).toString());

    }
    SYD << tr(".......SafetWorkflow::calculateSQL...QStringList CALCULATESQL.....result count: |%1|")
           .arg(resultlist.count());

    return resultlist;

}


void SafetWorkflow::saveStates(const QString &currstate, const QStringList& nextstates) {
    QStringList myemptylist;
    if (!_nexts.contains(currstate)) {
        _nexts[currstate] = myemptylist;
    }
    foreach(QString p, nextstates) {
        if (!_previouses.contains(p)) {
            _previouses[p] = myemptylist;
        }
        if ( !_previouses[p].contains(currstate)) {
            _previouses[p].append(currstate);
        }
        if ( !_nexts[currstate].contains(p)) {
            _nexts[currstate].append(p);
        }
    }

}

QString SafetWorkflow::pushLeftChild(SafetNode *node, QStack<SafetWorkflow::PathHistory>& localstack,
		QQueue<QString>& qqueuepath) {
	Q_CHECK_PTR(node);
	QString pop = "";
	QStack<QString> childstack = node->next();
	if ( childstack.isEmpty() ) return pop;
	SafetPort* port = node->port();
	Q_CHECK_PTR(port);
	if (getSQLOperator(port->pattern()).length() > 0 ) opeStack.push_front(port->pattern());
	int j=0;
	for( int i=childstack.count()-1; i>=0;i--) {
        if (isBackNode(node,childstack[i])) {
            continue;
        }
		PathHistory pathhistory(childstack[i]); 
		if (childstack[i].length() > 0 ) {
			SafetNode* childnode = nodemap[ childstack[i] ]; // Colocar el camino, por si es necesario
			Q_CHECK_PTR(childnode);			
			childnode->setOption(i);
			SafetPort* childport = childnode->port();
			Q_CHECK_PTR( childport );
			pathhistory.history() += qqueuepath;
			localstack.push(pathhistory);
			if (getSQLOperator(port->pattern()).length() > 0 &&
                            getSQLOperator(childport->pattern()).length() == 0	) {
                            opeStack.push_front(childstack[j++]);
                        }
		}
	}
	if (localstack.isEmpty()) return pop;
	PathHistory h = localstack.pop(); 
	pop = h.nodename();
	qqueuepath = h.history();
	return pop; 
}




void SafetWorkflow::resetVisitNodeMap() {
	QList<QString> mylist = visitnodemap.keys();
		 for (int i = 0; i < mylist.count(); i++) {
			 visitnodemap[ mylist.at(i) ] = false;
	}
}

QString SafetWorkflow::drawDotGraph(const QString& dotfile,  char* filetype) {
    QStringList list  = getTempNameFiles(2);
//    Q_ASSERT( list.count() == 2 );
//    GVC_t *gvc; graph_t *g; FILE *fpin, *fpout;
//    gvc = gvContext();
//    Q_CHECK_PTR( gvc );
//    fpin = fopen (qPrintable(list.at(0)),"w+");
//    fpout = fopen (qPrintable(list.at(1)),"w+");
//     fwrite( qPrintable(dotfile), sizeof(char), dotfile.length()+1, fpin);
//     fflush(fpin);
//     fclose(fpin);
//    fpin = fopen (qPrintable(list.at(0)),"r");
//    Q_CHECK_PTR(fpin);
//    g = agread(fpin);
//    gvLayout(gvc, g, "dot");
//    gvRender(gvc, g,  filetype, fpout);
//    gvFreeLayout(gvc, g);
//    agclose(g);
//    gvFreeContext(gvc);
//    fclose(fpin);
//    fclose(fpout);
//
    return list.at(1);
}

QList<double> SafetWorkflow::numericInfos(const QString& id) {
    QList<double>    result;
    bool includeall = id.trimmed().isEmpty();
    QList<SafetTask*> mytasks = getTasks();
    bool ok;
    foreach(SafetTask* mytask, mytasks) {
        Q_CHECK_PTR(mytask);
        double value = mytask->numericinfo().toDouble(&ok);
        if (!ok) {
            SYE << tr("Error al tratar del convertir el atributo \"numericinfo\": "
                      "\"%1\"")
                   .arg(mytask->numericinfo());
            return result;
        }
        if (!includeall ) {
            if ( mytask->id()==id) {
                result.append(value);
                return result;
            }
        }
        else {
            result.append(value);
        }

    }

    return result;

}

QStringList SafetWorkflow::textualInfos(const QString& id, int fieldtype, const QString& otherid) {
    QStringList result;
    bool includeall = id.trimmed().isEmpty();
    QList<SafetTask*> mytasks = getTasks();
    bool ok;
    foreach(SafetTask* mytask, mytasks) {
       Q_CHECK_PTR(mytask);
        if (!includeall ) {
            if ( mytask->id()==id) {                
                QString myvalue;
                switch( fieldtype) {
                case 0:
                    SYD << tr("..............SafetWorkflow::textualInfos....TEXTUALVALUE ID(1):|%1|").arg(id);
                    SYD << tr("..............SafetWorkflow::textualInfos....TEXTUALVALUE OTHERID(1):|%1|").arg(otherid);
                    myvalue = calculateSQL(mytask->textualinfo(),id,otherid);
                    SYD << tr("..............SafetWorkflow::textualInfos....TEXTUALVALUE (2):|%1|").arg(myvalue);
                    break;
                case 1:
                    myvalue = calculateSQL(mytask->role(),id);
                    break;
                case 2:
                    myvalue = calculateSQL(mytask->note(),id);
                    break;
                }
                result.append(myvalue);
                return result;
            }
        }
        else {
            result.append(mytask->textualinfo());            
        }

    }

    return result;

}


QStringList SafetWorkflow::listNextStates(const QString& id, SafetWorkflow::NextStates st,  bool incback) {


     QStringList result;
     if ( id.isEmpty()) {
         SYW << tr("En la funcion de proximos etados el id est\E1 vac\EDo:|%1|")
                .arg(id);

         return result;
     }
     SafetYAWL::_isstatstokenfound = false; // Inicializando por si es un unico token
     QString code = generateCodeGraph("png",id,true,false);

     SYD << tr("....SafetWorkflow::listNextStates...id:|%1|")
            .arg(id);
     SYA << tr("...SafetWorkflow::listNextStates FIXING_2....GENERATECODEGRAPH:\n|%1|")
            .arg(code);

     SafetYAWL::_isstatstokenfound = false;
     QStringList mylist = code.split("\n",QString::SkipEmptyParts);
     bool ok;
     if ( st == SafetWorkflow::CurrentState ) {
         QString myCurrentState;
         for(int  i=mylist.count()-1; i >=0; i--) {
              QString sec = mylist.at(i);
              QRegExp rx("Nodo:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
              int pos = rx.indexIn( sec );
              Q_ASSERT ( pos >= 0 );
              QString newnode = rx.cap(1);

              rx.setPattern("Patron:\\s*([a-zA-Z0-9_\\.]+)\\s*,");
              pos = rx.indexIn( sec );
              Q_ASSERT ( pos >= 0 );
              // Para el color
              rx.setPattern("info\\.task\\.color:\\s*([a-zA-Z0-9_\\.]+)\\s*,?");
              pos = rx.indexIn( sec );
              double porc = 0;
              if  ( pos > 0 ) {
                   porc = rx.cap(1).toDouble(&ok);
                   if ( porc > 0.0 ) { // es
                        myCurrentState = newnode;
                        break;
                   }
              }
         }
         if (!myCurrentState.isEmpty()) {
             result.append(myCurrentState);
         }
         return result;
     }


     if ( st == SafetWorkflow::AllNotCompleted ) {
         QString myCurrentState;
         for(int  i=mylist.count()-1; i >=0; i--) {
              QString sec = mylist.at(i);
              QRegExp rx("Nodo:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
              int pos = rx.indexIn( sec );
              Q_ASSERT ( pos >= 0 );
              QString newnode = rx.cap(1);

              rx.setPattern("Patron:\\s*([a-zA-Z0-9_\\.]+)\\s*,");
              pos = rx.indexIn( sec );
              Q_ASSERT ( pos >= 0 );
              // Para el color
              rx.setPattern("info\\.task\\.color:\\s*([a-zA-Z0-9_\\.]+)\\s*,?");
              pos = rx.indexIn( sec );
              double porc = 0;
              if  ( pos > 0 ) {
                   porc = rx.cap(1).toDouble(&ok);
                   if ( porc > 0.0 ) { // es
                        myCurrentState = newnode;
                        break;
                   }
              }
         }

         QString  comp = SafetYAWL::getConf()["ExtraInfo/infotext.completed"];

         for(int  i=mylist.count()-1; i >=0; i--) {
             QString sec = mylist.at(i);
             QRegExp rx("Nodo:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
             int pos = rx.indexIn( sec );

             QString newnode = rx.cap(1);


         rx.setPattern("Reporte:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*");

         pos = rx.indexIn( sec );

         if ( pos >= 0  ) {
          if ( rx.cap(1) == "no" ) {
            continue;
         }

         }

             QString myinfos = sec.section(",",-1);

             QStringList mylist =  myinfos.split("...", QString::SkipEmptyParts);

             if (mylist.count() < 3
                     &&   newnode != tr("inicial")
                     &&   newnode != tr("final")) {
                 if (!result.contains(newnode) ) {
                    result.append(newnode);
                 }
             }
             else {

                 if ( mylist.count() > 2 ) {

                     QString myinfotext = mylist.at(2);

                     if (myinfotext.indexOf(comp) != -1) {
                         if (!result.contains(newnode) ) {
                         result.append(newnode);
                         }
                         continue;
                     }

                 }
             }

         }

     }
     else {

         SYD << tr("......**SafetWorkflow::listNextStates.........SIGN: ...mylist.count():|%1|")
                .arg(mylist.count());

         // -------------> Cuales nodos est\E1n activos
         QStringList marked;

         QStringList nextnodes;

         for(int i =0; i < mylist.count(); i++) {
             QString sec = mylist.at(i);
             QRegExp rx("Nodo:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
             int pos = rx.indexIn( sec );
             Q_ASSERT ( pos >= 0 );
             QString newnode = rx.cap(1);
             rx.setPattern("info\\.task\\.color:\\s*([a-zA-Z0-9_\\.]+)\\s*,?");
             pos = rx.indexIn( sec );
             double porc = 0;

             int ncount = sec.section(",",-1).split("...", QString::SkipEmptyParts).count();
             bool ispassed =  ncount > 2;

             SYD << tr("....SafetWorkflow::listNextStates...(2)...ISPASSED count:|%1|")
                    .arg(ncount);

             SYD << tr("....SafetWorkflow::listNextStates...(2)...ISPASSED section:|%1|")

                    .arg(sec.section(",",-1));

             SYD << tr("....SafetWorkflow::listNextStates...(2)...ISPASSED ispassed:|%1|")
                    .arg(ispassed);

             if  ( pos > 0 ) {
                 porc = rx.cap(1).toDouble(&ok);
                 if ( porc > 0.0 || ispassed ) { // es el nodo identificado con ID
                     SYD << tr("..SafetWorkflow::listNextStates..MARKED BY PORC:|%1|")
                            .arg(newnode);
                     marked.append(newnode);
                 }
             }

         }


         // -------------> Cuales nodos est\E1n activos

         for(int  i=mylist.count()-1; i >=0; i--) {
             QString sec = mylist.at(i);
             QRegExp rx("Nodo:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
             int pos = rx.indexIn( sec );
             Q_ASSERT ( pos >= 0 );
             QString newnode = rx.cap(1);

             SYD << tr("......**SafetWorkflow::listNextStates..............newnode:|%1|")
                    .arg(newnode);

             rx.setPattern("Patron:\\s*([a-zA-Z0-9_\\.]+)\\s*,");
             pos = rx.indexIn( sec );
             Q_ASSERT ( pos >= 0 );
             // Para el color
             rx.setPattern("info\\.task\\.color:\\s*([a-zA-Z0-9_\\.]+)\\s*,?");
             pos = rx.indexIn( sec );
             double porc = 0;
             if  ( pos > 0 ) {
                 porc = rx.cap(1).toDouble(&ok);

                 // **** Es un nodo marcado  ....
                 if ( porc > 0.0 ) { // es el nodo identificado con ID
                     SafetNode* node = nodemap[ newnode ];
                     Q_CHECK_PTR( node );
                     Q_ASSERT (_previouses.contains(newnode));
                     Q_ASSERT (_nexts.contains(newnode));

                     // ******* Previous Nodes
                     if ( st != SafetWorkflow::OnlyNext && st != SafetWorkflow::Neighbour) {
                        foreach(QString p, _previouses[newnode]) {
                           SYD << tr("....SafetWorkflow::listNextStates...previouses p:|%1|")
                                .arg(p);
                          foreach(QString nb, _nexts[p]) {
                             if ( nb != newnode) {
                                 if (!result.contains(nb)  ) {
                                    result.append(nb);
                                    SYD << tr("....SafetWorkflow::listNextStates...(1)...result ADDING:|%1|")
                                           .arg(nb);
                                    if (!nextnodes.contains(nb)) {
                                            nextnodes.append(nb);
                                    }
                                 }
                             }
                         }
                        }
                     }

                     SYD << tr("......**SafetWorkflow::listNextStates.........***processing....");
                     rx.setPattern("\\s*([a-zA-Z0-9_][a-zA-Z0-9_\\.\\-]*)(\\s+(SIGN|PASS))"
                                                               "\\s+([a-zA-Z0-9_][a-zA-Z0-9_\\.\\s]+"
                                                               "|#ANYBODY#\\s*)");
                     if ( st == SafetWorkflow::OnlyNext )  {
                         SafetNode* mycurr = nodemap[ newnode ];
                         if (mycurr == NULL ) {
                             SYE << tr("Ocurri\F3 un error al buscar el nodo actual \"%1\"").arg(newnode);
                             return result;
                         }
                         SafetPort* myport = mycurr->outport();
                         if (myport == NULL ) {
                             SYE << tr("Ocurri\F3 un error al buscar el puerto del nodo actual \"%1\"").arg(newnode);
                             return result;
                         }
                         QList<SafetConnection* > clist  =  incback ?myport->getConnBackList() :myport->getConnectionlist();
                         foreach(SafetConnection *c, clist) {
                             SYD << tr(".........SafetWorkflow::listNextStates... c is NULL:%1").arg(c == NULL);
                             SYD << tr("....SafetWorkflow::listNextStates........ADDING (4)...OnlyNext***_nexts p (connection source):|%1|")
                                    .arg(c->source());

                             if (rx.indexIn(c->query()) >= 0 )  {
                                 continue;
                             }
                             if (!result.contains(c->source()) ) {
                                result.append(c->source());
                             }

                         }

//                             return result;
                     }



                     SafetPort* port = node->outport();
                     Q_CHECK_PTR( port );
                     QList<SafetConnection* > clist  =  incback ?port->getConnBackList() :port->getConnectionlist();

                     for(int i = 0; i < clist.count(); i++) {
                             QString query = port->query(i);
                             rx.setPattern("\\s*([a-zA-Z0-9_][a-zA-Z0-9_\\.\\-]*)(\\s+(SIGN|PASS))"
                                                                       "\\s+([a-zA-Z0-9_][a-zA-Z0-9_\\.\\s]+"
                                                                       "|#ANYBODY#\\s*)");
                             pos = rx.indexIn( query );

                             SYD << tr("......**SafetWorkflow::listNextStates.........SIGN: ...*query:|%1|......**pos|%2|")
                                    .arg(query)
                                    .arg(pos);

                             if ( pos >= 0 ) {

                                        QString source = port->source(i);
                                        SYD << tr("......*******SafetWorkflow::listNextStates.........before...port->source():|%1|")
                                        .arg(source);
                                         if (marked.contains(source)) {
                                             SYD << tr("......******SafetWorkflow::listNextStates.........port->source():|%1|")
                                                    .arg(source);
                                             continue;
                                         }

                                         SYD << tr("......**SafetWorkflow::listNextStates.........SIGN: ...rx.cap(4):|%1|")
                                                .arg(rx.cap(4));
                                         QString namevar = rx.cap(1).simplified();
                                         SYD << tr("......**SafetWorkflow::listNextStates.........SIGN: ...rx.cap(4):|%1|")
                                               .arg(rx.cap(1));
                                         QString newsign = tr("*FIRMAR Documento (%2) por /%1/")
                                                 .arg(rx.cap(4))
                                                 .arg(namevar);
                                         if ( !result.contains(newsign)) {

                                                    result.append(newsign);
                                                    SYD << tr("....SafetWorkflow::listNextStates...(2)...**result ADDING:|%1|")
                                                           .arg(newsign);

                                                    if (!nextnodes.contains(port->source(i))) {
                                                        nextnodes.append(port->source(i) );
                                                    }
                                         }

                             }
                             else  {
                                 if ( port->options(i).simplified().length() > 0  ) {
                                     if ( !result.contains(port->source(i))) {
                                         result.append(port->source(i));
                                         SYD << tr("....SafetWorkflow::listNextStates...(3)...result ADDING:|%1|")
                                                .arg(port->source(i));

                                         if (!nextnodes.contains(port->source(i))) {
                                             nextnodes.append(port->source(i));
                                         }


                                     }

                                 }
                             }

                 }

                     // ****** Chequear que operadores JOIN

                     foreach(QString n, nextnodes) {
                         SYD << tr("...................................SafetWorkflow::listNextStates.....(nextnode)...node:|%1|")
                                .arg(n);

                         SafetPort *myport = NULL;
                         for(int j=0; j < nodemap[n]->getPorts().count(); j++) {
                             myport = nodemap[n]->getPorts().at(j);
                             Q_CHECK_PTR( myport);
                             if (myport->type() == "join" ) {

                              if (myport->pattern() == "and") {
                                 SYD << tr("...................................SafetWorkflow::listNextStates.....JOIN/AND:|%1|")
                                      .arg(n);

                                break;

                              }
                             }
                             myport = NULL;
                         }

                         if ( myport == NULL ) {
                             continue;
                         }

                         SYD << tr(".....SafetWorkflow::listNextStates.....There are JOIN/AND nodes");

                         int njoins = 0;
                         int nmarked = 0;


                         for(int j=0; j < nodemap.count(); j++) {

                             QString key = nodemap.keys().at(j);
                             SafetPort *port = nodemap[ key ]->outport();
                             QList<SafetConnection* > clist  =  incback ?port->getConnBackList() :port->getConnectionlist();
                             for( int k= 0; k < clist.count();k++) {
                                 SafetConnection* conn = clist.at(k);
                                 if ( conn->source() == n) {
                                     SYD << tr(".....SafetWorkflow::listNextStates.....njoin....node:|%1|").arg(key);
                                     njoins++;
                                     if ( marked.contains(nodemap[ key ]->id())) {
                                         SYD << tr(".....SafetWorkflow::listNextStates.....nmarked....node:|%1|").arg(key);
                                         nmarked++;
                                     }

                                 }
                             }

                         }

                         SYD << tr(".....SafetWorkflow::listNextStates.....nmarked:|%1|...njoins:|%2|")
                                .arg(njoins)
                                .arg(nmarked);


                         if (nmarked < njoins) {
                            result.removeAt(result.indexOf(n));
                         }


                     }

                     // ****** Chequear que operadores JOIN
                     foreach(QString m, marked) {
                         if (result.contains(m)) {
                             SYD << tr(".....SafetWorkflow::listNextStates.....(MARK) REMOVING:|%1|")
                                    .arg(m);
                             result.removeAt(result.indexOf(m));
                         }
                     }

             }
         }
     }

   }
     SYD << tr("\n*....SafetWorkflow::listNextStates...result.count():|%1|").arg(result.count());
     foreach(QString s, result) {
         SYD << tr("....SafetWorkflow::listNextStates...result:|%1|").arg(s);
     }

    return result;


}



QString SafetWorkflow::generateCodeGraph(char* filetype, const QString& info, bool norender, bool inccache) {
     SYD << tr(".................SafetWorkflow::generateCodeGraph......(entrando)");
     QMap<QString,QString> newresults;

     _nexts.clear();
     _previouses.clear();
     SYD << tr("...SafetWorkflow::generateCodeGraph....GENERATECODEGRAPH (1)...");



     SYD << tr("...SafetWorkflow::generateCodeGraph....GENERATECODEGRAPH (2)......(**INITTIME)");

     int newcountresults = 0;
     QMap<int,QString> neworderresults;

     if (inccache == false ) {
        _cache.clear();
     }
     for(int i = 0; i < getConditionlist().count();i++) {
         SafetCondition *mycond = getConditionlist().at(i);

         if (mycond != NULL  && mycond->id() == "inicial") {
             SYD << tr(".....SafetWorkflow::generateCodeGraph....GENERATECODEGRAPH....mycond->id():|%1|\n\n").arg(mycond->id());
             QString nodename = mycond->id();
             QString nextnodes;

             for( int j = 0 ; j < mycond->outport()->getConnectionlist().count(); j++) {
                  QString source = mycond->outport()->getConnectionlist().at(j)->source();
                  nextnodes += source + ";";
             }
             if (nextnodes.isEmpty()) {
                 SYE << tr("....ERROR Conditions (2) (GENERATECODEGRAPH) no existen nodos siguientes para el nodo:|%1|")
                        .arg(nodename);
                 break;
             }
             nextnodes.chop(1);

             QString newline = printNodeInformation(mycond, nextnodes, "", info,norender);
             SYD << tr("NL:|%1|").arg(newline);
             newresults[ nodename ] = newline;
             neworderresults[ newcountresults++ ] = nodename;
         }
     }

     QList<QPair<QString,QString> >  hides;

     if (inccache == false ) {
           _cache.clear();
     }
     for(int i = 0; i < getTasklist().count();i++) {
         SafetTask *mytask = getTasklist().at(i);
         if (mytask != NULL ) {
             SYD << tr(".....SafetWorkflow::generateCodeGraph....GENERATECODEGRAPH....mytask->id():|%1|").arg(mytask->id());
             QString nodename = mytask->id();
             QString nextnodes;
         if (!mytask->hideuntil().isEmpty() ) {
              QPair<QString,QString> mypair;
              mypair.first = mytask->id();
              mypair.second =  mytask->hideuntil();
              hides.append(mypair);
              }

             for( int j = 0 ; j < mytask->outport()->getConnectionlist().count(); j++) {
                  QString source = mytask->outport()->getConnectionlist().at(j)->source();
                  nextnodes += source + ";";
             }
             if (nextnodes.isEmpty()) {
                 SYE << tr("....ERROR Task (2) (GENERATECODEGRAPH) no existen nodos siguientes para el nodo:|%1|")
                        .arg(nodename);
                 break;
             }
             nextnodes.chop(1);

             QString newline = printNodeInformation(mytask, nextnodes, "", info,norender);
             SYD << tr("NEWLINE:|%1|").arg(newline);
             newresults[ nodename ] = newline;
             neworderresults[ newcountresults++ ] = nodename;

         }
     }


     if (inccache == false ) {
        _cache.clear();
     }

     for(int i = 0; i < getConditionlist().count();i++) {
         SafetCondition *mycond = getConditionlist().at(i);
         if (mycond != NULL  && mycond->id() == "final") {
             QString nodename = mycond->id();
             QString nextnodes;

             for( int j = 0 ; j < mycond->outport()->getConnectionlist().count(); j++) {
                  QString source = mycond->outport()->getConnectionlist().at(j)->source();
                  nextnodes += source + ";";
             }
             if (nextnodes.isEmpty()) {
                 SYE << tr("....ERROR Conditions 2 (GENERATECODEGRAPH) no existen nodos siguientes para el nodo:|%1|")
                        .arg(nodename);
                 break;
             }
             nextnodes.chop(1);

             QString newline = printNodeInformation(mycond, nextnodes, "", info,norender);
             SYD << tr("NEWLINE:|%1|").arg(newline);
             newresults[ nodename ] = newline;
             neworderresults[ newcountresults++ ] = nodename;
             SYD << tr(".....SafetWorkflow::generateCodeGraph....(2)....GENERATECODEPGRAPH....mycond->id():|%1|\n\n").arg(mycond->id());
         }
     }

     SYD << tr("****");
     foreach(QString n, newresults.keys()) {
         SYD << tr("newresult NODE:|%2|:->|%1|").arg(newresults[ n ]).arg(n);

     }

     for(int i=0; i < hides.count(); i++) {
         QPair<QString,QString> hidenode = hides.at(i);
         checkDisconnectedNodes(newresults,hidenode.first, hidenode.second,info,neworderresults);
      }



     proccessExtraInfo(newresults,info,neworderresults);


     shrinkNodes(newresults);

     QString newresult;

     foreach(QString nodename, newresults.keys() ) {
               newresult += newresults[nodename ] +  "\n";
     }

     SYD << tr("......NEW..CONNECTED..AFTER_SHRINK...************RESULT:\n%1\n*************")
            .arg(newresult);

     return newresult;
}
void  SafetWorkflow::shrinkNodes(QMap<QString,QString>&  nodes) {


    foreach(QString nodename, nodes.keys()) {
        QString deletenode = nodename;
        if (nodemap[nodename]->title() != "::safethide::") {
                 continue;
        }

        SYD << tr("...SHRINK node:|%1|").arg(nodename);
        QString line = nodes[nodename];
        SYD << tr("...SHRINK line:|%1|\n").arg(nodename);
        QStringList myfields = line.split(",",QString::SkipEmptyParts);
        if (myfields.count() < 3) {
            SYW << tr("En el proceso de reducci�n esta mal formado la l�nea \"%1\"")
                   .arg(line);
            continue;
        }
        SYD << tr("...SHRINK ...continuating!");


        QStringList unodes;
        QString oldfield = myfields.at(2).trimmed().mid(QString("Siguiente:").length());
        unodes = oldfield.split(";",QString::SkipEmptyParts);
        int result = nodes.remove(nodename);
        SYD << tr("...SHRINK ...remove result:|%1|").arg(result);
        foreach(QString othernode, nodes.keys()) {
            SYD << tr("...PASTE node:|%1|").arg(othernode);
            QString line = nodes[othernode];
            SYD << tr("..PASTE line:|%1|").arg(othernode);
            QString mynode = "";
            QStringList myfields = line.split(",",QString::SkipEmptyParts);
            for(int i=0; i < myfields.count(); i++) {
                if ( i==2 )  {
                    QString currnodes = myfields.at(i).trimmed().mid(QString("Siguiente:").length());
                    SYD << tr("..FINAL currnodes:|%1|").arg(currnodes);
                    QStringList cnodes = currnodes.split(";",QString::SkipEmptyParts);
                    if (cnodes.contains(deletenode) ) {
                        cnodes.removeAll(deletenode);
                        cnodes.append(unodes);
                        currnodes = "";
                        foreach(QString c, cnodes) {
                           currnodes += c;
                           currnodes += ";";
                        }
                        currnodes.chop(1);
                    }

                    mynode = mynode + QString("Siguiente:%1,").arg(currnodes);
                }
                else {

                    mynode = mynode + myfields.at(i) + ",";
                }


            }
            mynode.chop(1);
            SYD << tr("..FINAL MYNODE:|%1|\n").arg(mynode);
            nodes[othernode] = mynode;

        }

    }

}


void  SafetWorkflow::checkDisconnectedNodes(QMap<QString,QString>&  nodes,const QString& nodestart, const QString& nodeend,
                                            const QString& info,QMap<int,QString>& orders) {

    bool remainnodes;
    if (!nodes.contains(nodestart) ) {

        SYW << tr("El nodo \"%1\" no existe en el grafo, no es posible realizar una nueva conexi�n").arg(nodestart);
        return;
    }

    if (!nodes.contains(nodeend) ) {

        SYW << tr("El nodo \"%1\" no existe en el grafo, no es posible realizar una nueva conexi�n").arg(nodeend);
        return;
    }

    QString mynode = nodes[nodestart];
    QStringList myfields = mynode.split(",",QString::SkipEmptyParts);
    Q_ASSERT(myfields.count() > 2 );
    nodes.remove(nodestart);

    mynode = "";
    SafetTask* mytaskstart = searchTask(nodestart);
    if (mytaskstart == NULL ) {
        SYW << tr("No fue posible conseguir la tarea \"%1\"").arg(nodestart);
        return;
    }
    SYD << tr("...SafetWorkflow::checkDisconnectedNodes.. FOUNDED:|%1|").arg(nodestart);
    SafetVariable *myvarstart = mytaskstart->getVariables().at(0);
    SYD << tr("myvarstart == NULL");
    Q_ASSERT(myvarstart == NULL);



    for(int i=0; i < myfields.count(); i++) {
        if ( i==2 )  {
            QString oldfield = myfields.at(i).trimmed().mid(QString("Siguiente:").length());
            SYD << tr(".....SafetWorkflow::checkDisconnectedNodes...Siguiente..OLDFIELD:|%1|")
                   .arg(oldfield);
            //QStringList unodes = oldfield.split(";",QString::SkipEmptyParts);
            SYD << tr(".....SafetWorkflow::checkDisconnectedNodes...nodeend:|%1|")
                   .arg(nodeend);
            mynode = mynode + QString("Siguiente:%1,").arg(nodeend);
        }
        else {

            mynode = mynode + myfields.at(i) + ",";
        }


    }
    mynode.chop(1);


    SYD << tr("\n..... SafetWorkflow::checkDisconnectedNodes....mynode:\n%1")
           .arg(mynode);

    nodes[ nodestart ] = mynode;

    QRegExp rx; // Para saber si es un nodo marcado
    rx.setPattern("info\\.task\\.(color|size):\\s*([\\-a-zA-Z0-9_\\.]+)\\s*,?");

    QMap<QString,QString>  summnodes;
    while( true ) {

        remainnodes = false;
        QStringList connectnodes;
        SYD << tr("");
        foreach(QString nodename, nodes.keys()) {
            QString mynode = nodes[nodename];
            QStringList myfields = mynode.split(",",QString::SkipEmptyParts);
            Q_ASSERT(myfields.count() > 2 );
            SYD << tr("..........SafetWorkflow::checkDisconnectedNodes...mynode:|%1|").arg(mynode);
            QString fieldsig = myfields.at(2).trimmed();
            SYD << tr("..........SafetWorkflow::checkDisconnectedNodes...myfields.at(2):|%1|").arg(fieldsig);
            QStringList nextnodes = fieldsig.mid(QString("Siguiente:").length()).split(";",QString::SkipEmptyParts);
            nextnodes.removeAll("final");
            Q_ASSERT(nextnodes.count() > 0 );
            foreach(QString n, nextnodes) {
                if (!connectnodes.contains(n)) {
                    connectnodes.append(n);
                }
            }


        }

        SYD << tr("......SafetWorkflow::checkDisconnectedNodes.....connectnodes.count():|%1|")
               .arg(connectnodes.count());
        foreach(QString nodename, connectnodes) {
            SYD << tr("..........SafetWorkflow::checkDisconnectedNodes...nodename:|%1|").arg(nodename);
        }

        foreach(QString nodename, nodes.keys() ) {

            if (!connectnodes.contains(nodename) && nodename != "final"  && nodename != "inicial" ) {
                SafetTask* mytaskremoved = searchTask(nodename);
                if (mytaskremoved != NULL ) {

                    SYD << tr("..........SafetWorkflow::checkDisconnectedNodes...mytaskremoved:|%1|")
                           .arg(mytaskremoved->id());
                    SafetVariable* myvar = mytaskremoved->getVariables().at(0);
                    SYD << tr("..........myvar != NULL )(1)");
                    Q_ASSERT(myvar != NULL);
                    SYD << tr("..........myvar != NULL )(2)");
                    QString mytsvalue = myvar->timestampfield();

                    if ( rx.indexIn( nodes[nodename] ) > 0 ) {
                        bool ok;
                        double porc = rx.cap(2).toDouble(&ok);
//                        SYD << tr(".........SafetWorkflow::checkDisconnectedNodes..nodetocheck:|%1|....porc:|%2|")
//                               .arg(nodes[nodename])
//                               .arg(porc);
                        if (porc >  0 ) {
                            myvarstart->setTimestampfield(mytsvalue);
                        }
                    }



                }
                else {
                    SYW << tr("No se pudo eliminar la tarea \"%1\" en el proceso de ocultar (hide)")
                           .arg(nodename);

                }

                SYD << tr("..........SafetWorkflow::checkDisconnectedNodes...PLUS...summnodes:|%1|")
                       .arg(nodename);
                summnodes[nodename] =  nodes[nodename] + "\n";
                nodes.remove(nodename);
                remainnodes = true;
            }
        }

        if (!remainnodes ) {
            break;
        }

        proccessExtraInfo(summnodes,info,orders);
    }


    QString mysumm;
    foreach(QString nodename, summnodes.keys() ) {
        mysumm = mysumm +  summnodes[nodename] + "\n";
    }

    SYD << tr("...SafetWorkflow::checkDisconnectedNodes*****************mysumm:\n%1\n***************").arg(mysumm);
    QString formularesult = calculateGraphFormula(mysumm,Partial);
    QString textualresult = formularesult;
    textualresult.chop(1);
    //mytaskstart->setTextualinfo(textualresult);
    QString selectresult = QString("(select 'usuario %1') as rol").arg(formularesult);
    myvarstart->setRolfield(selectresult);
    QString currts = myvarstart->timestampfield();

    SYD << tr("Disconnected......CURRTS:|%1|").arg(currts);
    if ( currts.isEmpty() ) {

        QString newts = QString("(select 2323231231) as tiempo");
        myvarstart->setTimestampfield(newts);
        SYD << tr("Disconnected..empty....CURRTS:|%1|").arg(currts);
    }

    SYD << tr("..........SafetWorkflow::checkDisconnectedNodes...MYSUMM...task:|%2| selectresult:|%1|").arg(selectresult)
           .arg(mytaskstart->id());





 }

void SafetWorkflow::proccessExtraInfo( QMap<QString,QString>& codes,const QString& info,
                                        QMap<int,QString>& orders) {

    SYD << tr("....SafetWorkflow::proccessExtraInfo ....info:|%1|")
           .arg(info);


    QMap <QString,SafetTask*> timespans;



    for(int i=0; i < orders.count();i++) {

        if ( !orders.contains(i)) {
            continue;
        }
        QString mytask = orders[i];
        //        qDebug("**...mytask...:|%s|",
        //               qPrintable(mytask));

    if (!codes.keys().contains(mytask) ) {
        continue;
    }

        bool ok;
        QString derror;
        bool showonly = SafetYAWL::getConf()["GeneralOptions/extrainfo.showonly"]=="on"?true:false;


        bool showhuman = SafetYAWL::getConf()["GeneralOptions/extrainfo.showhumanize"]=="on"?true:false;
        QList<SafetWorkflow::InfoSigner> infosigners;

        for(int j=0; j < getTasklist().count(); j++) {

            SafetTask *searchtask = getTasklist().at(j);
            QString newsql;


            if (searchtask->id() == mytask) {
                QString rolvalue, tsvalue;
                SafetVariable* myvar = searchtask->getVariables().at(0);
                QString rolfield = myvar->rolfield();
                QString tsfield = myvar->timestampfield();


                if ( SafetYAWL::mapCurrentExtrainfo.contains(mytask)) {
                    rolvalue = SafetYAWL::mapCurrentExtrainfo[mytask].first;
                    tsvalue = SafetYAWL::mapCurrentExtrainfo[mytask].second;
                    bool ok;
                    uint numdate = tsvalue.toUInt(&ok);
                    tsvalue = QDateTime::fromTime_t(numdate).toString("dd/MM/yyyy hh:mmap");
                        if (tsvalue.startsWith("31/12/1969")) {
                            tsvalue = tr("n/a");
                        }

                    SYD << tr("....*geteando...tsvalue (format):|%1|").arg(tsvalue);
                    codes[mytask] = QString("%1%2...%3...")
                                    .arg(codes[mytask].trimmed())
                                    .arg(rolvalue.trimmed())
                                    .arg(tsvalue);
            SYD  << tr("...........proccessExtraInfo.......TOUCHCODE:\n%1")
            .arg(codes[mytask]);


                }

                else if (!rolfield.isEmpty() && !tsfield.isEmpty()) {
                    QString mysql = myvar->documentsource();
                    SafetSQLParser localparser;
                    localparser.setStr(mysql);
                    localparser.setWorkflow(this);
                    localparser.parse();



                    setTablesource(localparser.getTablesource());
                    QString link = processTokenlink(myvar->tokenlink());



                    if (!rolfield.startsWith("cn::")) {
                        localparser.addFieldToList(rolfield);
                    }
                    else {
                        int nrosign = 0;

                        infosigners =
                                getSignersDocument(myvar,info,derror);
                        QString rolmatch = rolfield.mid(QString("cn::").length());
                        if (infosigners.count() > 0 ) {
                            for(int j=0; j < infosigners.count(); j++ ) {
                              SafetWorkflow::InfoSigner signature =  infosigners.at(j);
                              rolvalue = signature.commonName;

                              SYD << tr("......SafetWorkflow::proccessExtraInfo....ROLMATCH...j:|%3|...rolmatch:|%1|...|%2|")
                                     .arg(rolmatch)
                                     .arg(rolvalue)
                                     .arg(j);
                              if (rolmatch == rolvalue) {
                                  break;
                              }
                          }

                            if (rolmatch != rolvalue) {
                                rolvalue = "";
                            }




                        }
                        else {
                            rolvalue = tr("firma::N/A");
                        }


                    }
                    if (!tsfield.startsWith("cn::")) {
                        localparser.addFieldToList(tsfield);
                    }
                    else {
                        int nrosign = 0;
                        if (infosigners.count() > 0 ) {
                            QString spanvalue = "<br/>N/A*";
                            QStack<QString> nexts;
                           QString rolmatch = rolfield.mid(QString("cn::").length());
                             for(int j=0; j < infosigners.count(); j++ ) {
                                SafetWorkflow::InfoSigner signature =  infosigners.at(j);
                                tsvalue = signature.date + " "+signature.hour;
                                tsvalue.chop(1);
                                SYD << tr(".....SafetWorkflow::proccessExtraInfo:...TSVALUE...(1) tsvalue:|%1|")
                                           .arg(tsvalue);
                                nexts = searchtask->next();
                                tsvalue = adjustTimeZone(tsvalue,Safet::BdocDateFormat,Safet::DateFormat);
                                SYD << tr(".....SafetWorkflow::proccessExtraInfo:..j:|%2|.TSVALUE...(2) tsvalue:|%1|")
                                           .arg(tsvalue)
                                       .arg(j);

                                rolvalue = signature.commonName;

                                SYD << tr("......SafetWorkflow::proccessExtraInfo....TSROLMATCH...rolmatch:|%1|...|%2|")
                                       .arg(rolmatch)
                                       .arg(rolvalue);
                                if (rolmatch == rolvalue) {
                                    break;
                                }

                             }

                            if (rolmatch != rolvalue) {
                                rolvalue = "";
                                tsvalue = "";

                            }

                            while ( !nexts.isEmpty()) {
                                QString nexttask = nexts.pop();
                                searchtask->setTsValue(tsvalue);
                                timespans[nexttask] = searchtask;
                            }
                            int days;
                            if ( timespans.contains(searchtask->id())) {

                                    spanvalue = tr("<br/>")+humanizeDate(days,timespans[searchtask->id()]->tsvalue(),
                                                              "dd/MM/yyyy hh:mmap",
                                                             QDateTime::fromString(tsvalue,
                                                             "dd/MM/yyyy hh:mmap"),
                                                                         SafetWorkflow::WaitTime,showhuman).trimmed();



                            }

                            if ( showhuman) {
                                if (!tsvalue.isEmpty()) {
                                    tsvalue += "<br/>"+humanizeDate(days,tsvalue,"dd/MM/yyyy hh:mmap",
                                                     QDateTime::currentDateTime(),
                                                         SafetWorkflow::SinceWhenTime,showhuman)
                                            + spanvalue;
                                }
                            }
                            SYD << tr(".....SafetWorkflow::proccessExtraInfo:...TSVALUE...(3) tsvalue:|%1|")
                                       .arg(tsvalue);



                        }
                        else {
                            tsvalue = tr("firma::N/A");
                        }
                    }
                    if (rolvalue.isEmpty() || tsvalue.isEmpty()) {
                        newsql = "SELECT ";
                        for(int k=0; k< localparser.getFields().count();k++){
                            newsql += localparser.getFields().at(k);
                            newsql += ",";
                        }
                        newsql.chop(1);
                        newsql += " FROM " + localparser.getTablesource();


                        newsql += " " + link + " ";
                        newsql += " WHERE ";
                        newsql += ptoken->key() + "=";
                        bool addquotes =  !(SafetYAWL::canTypeConvert(info, QVariant::Bool)
                                            || SafetYAWL::canTypeConvert(info, QVariant::Int));
                        newsql += (addquotes?"'":"")+info+(addquotes?"'":"");
                        QSqlQuery query( SafetYAWL::currentDb );
                        newsql= newsql.replace("WHERE id='coloured'","WHERE id>=0");
                        SYD << tr("newsql:|%1|").arg(newsql);

                        query.prepare(  newsql );
                        bool executed = query.exec();

                        if (!executed ) {
                            SYE
                                    <<
                                    tr("NO se ejecuto correctamente la sentencia SQL: \"%1\"")
                                    .arg(newsql);
                            break;
                        }

                        if (!query.next()) {
                         SYW
                                    <<
                                    tr("Advertencia:No existen registros para la clave: \"%1\"")
                                    .arg(newsql);
                            break;
                        }
                        if (rolvalue.isEmpty()) {
                            if (tsvalue.isEmpty()) {
                                rolvalue = query.value(query.record().count()-2).toString().trimmed();
                            }
                            else {
                                rolvalue = query.value(query.record().count()-1).toString().trimmed();
                            }
                        }
                        QDateTime mydate;

                        if (tsvalue.isEmpty()) {
                            QString mystr = query.value(query.record().count()-1)
                                            .toString().trimmed();
                            int intdate = mystr.toInt(&ok);

                            if (intdate == 0 ) {
                             rolvalue = tr("firma::N/A");
                            }
                            else {
                                QString spanvalue = QString("<br/>N/A");
                                mydate = mydate.fromTime_t(intdate);
                                tsvalue = mydate.toString("dd/MM/yyyy hh:mmap");
                                QStack<QString> nexts = searchtask->next();

                                while ( !nexts.isEmpty()) {
                                    QString nexttask = nexts.pop();
                                    searchtask->setTsValue(tsvalue);
                                    timespans[nexttask] = searchtask;
                                }

                                int days;
                                if ( timespans.contains(searchtask->id())) {
                                        spanvalue = tr("<br/>")+humanizeDate(days,
                                                         timespans[
                                                         searchtask->id()]->tsvalue(),
                                                         "dd/MM/yyyy hh:mmap",
                                                         QDateTime::fromString(tsvalue,
                                                               "dd/MM/yyyy hh:mmap"),
                                                    SafetWorkflow::WaitTime,showhuman).trimmed();                     ;

                                }

                                if ( showhuman) {
                                    tsvalue += "<br/>"+humanizeDate(days,tsvalue,

                                              "dd/MM/yyyy hh:mmap",
                                               QDateTime::currentDateTime(),
                                               SafetWorkflow::SinceWhenTime,showhuman
                                                                    ).trimmed()
                                     + spanvalue;
                                }


                            }
                        }


                    }


                    if (rolvalue != tr("firma::N/A")  ) {
            SYD << tr("TOUCH1");
                        codes[mytask] = QString("%1%2...%3...")
                                        .arg(codes[mytask].trimmed())
                                        .arg(rolvalue.trimmed())
                                        .arg(tsvalue);
                    }
                    else {
                        if (showonly) {
                            QRegExp rx;
                            rx.setPattern(",\\s*(\\d+)...\\d+...\\s*");

                SYD << tr("TOUCH2");
                            int pos = codes[mytask].indexOf(rx);
                            int value = 0;
                            bool ok;
                            if ( pos > 0 ) {
                                value = rx.cap(1).toInt(&ok);
                                if (value == 0) {
                                    codes[mytask].remove(rx);
                                }
                            }
                        }
                    }




                }
                else {
                    if (showonly) {
                        QRegExp rx;
                        rx.setPattern(",\\s*(\\d+)...\\d+...\\s*");

            SYD << tr("TOUCH4");
                        int pos = codes[mytask].indexOf(rx);
                        int value = 0;
                        bool ok;
                        if ( pos > 0 ) {
                            value = rx.cap(1).toInt(&ok);
                            if ( value == 0) {
                                codes[mytask].remove(rx);
                            }
                        }
                    }

                }
                _searchkey.key = info;
                if ( !rolvalue.isEmpty() && !tsvalue.isEmpty() ) {

            SYD << tr("TOUCH3...rolvalue:|%1|")
            .arg(rolvalue);
                    _searchkey.extrainfo[mytask]= rolvalue.trimmed()+";"
                                                  +tsvalue.trimmed();
                }

                break;
            }
        }

    }

    SYD << tr("....SafetWorkflow::proccessExtraInfo...saliendo...");
    return ;
}





QString SafetWorkflow::renderGraph(const QString& code, const char* ext) {
     QString fileName = "dfs.agl.tmp"; // Nombre del Archivo grafico (temporal)

      // ParÃ¡metros del grÃ¡fico

     return fileName;
}

QString SafetWorkflow::getJSONMiles(const QList<SafetWorkflow::Miles>& miles,
                                 QList<QSqlField>& myfields) {
    QString result;
    QTextStream out(&result);


    out << "{ \"columns\": [";

    myfields.clear();
    QSqlField f;
    f.setName(tr("\"Tarea\""));
    out <<  "{ \"sTitle\": \"Id\"},\n";

    out <<  "{ \"sTitle\": \"Tarea\"},\n";

    f.setType(QVariant::String);
    myfields.append(f);

    f.setName(tr("\"Rol\""));
    out <<  "{ \"sTitle\": \"Rol\"},\n";
    myfields.append(f);

    f.setName(tr("\"Fecha\""));

    out <<  "{ \"sTitle\": \"Fecha\"},\n";
    myfields.append(f);
    f.setName(tr("\"Tiempo\""));

    out <<  "{ \"sTitle\": \"Tiempo\"},\n";
    myfields.append(f);
    f.setName(tr("\"Espera\""));
    out <<  "{ \"sTitle\": \"Espera\"},\n";
    myfields.append(f);
    f.setName(tr("\"Porcentaje\""));
    out <<  "{ \"sTitle\": \"Porcentaje\"},\n";
    myfields.append(f);
    out <<  "{ \"sTitle\": \"Faltante\"},\n";
    myfields.append(f);
    out <<  "{ \"sTitle\": \"Nota\"},\n";


    out << "\n],\n";

    out << "\n\"tasks\": [\n";

    for( int i = 0; i < miles.count(); i++) {
        QString cadena("");
        out << "\t[ ";
            cadena.append(" ");
//            cadena.append(tr("\"Tarea\""));
            cadena.append("\"");
            cadena.append(QString("%1").arg(miles.at(i).id));
            cadena.append("\", ");

            cadena.append("\"");
            cadena.append(miles.at(i).nametask);
            cadena.append("\", ");
            cadena.append(" ");
//            cadena.append(tr("\"Rol\""));
            cadena.append("\"");
            cadena.append(miles.at(i).rol);
            cadena.append("\", ");
            cadena.append(" ");
//            cadena.append(tr("\"Fecha\""));
            cadena.append("\"");
            cadena.append(miles.at(i).ts.toString("dd/MM/yyyy hh:mmap"));
            cadena.append("\", ");
            cadena.append(" ");
//            cadena.append(tr("\"Tiempo\""));
            cadena.append("\"");
            cadena.append(miles.at(i).humandate);
            cadena.append("\"");
            cadena.append(", ");
//            cadena.append(tr("\"Espera\""));
            cadena.append("\"");
            cadena.append(miles.at(i).humanwait);
            cadena.append("\"");
            cadena.append(", ");
//            cadena.append(tr("\"Porc\""));
            cadena.append("\"");
            cadena.append(miles.at(i).porc);
            cadena.append("\"");
            cadena.append(", ");
//            cadena.append(tr("\"Faltante\""));
            cadena.append("\"");
            cadena.append(miles.at(i).porcremaining);
            cadena.append("\"");
            cadena.append(", ");
//            cadena.append(tr("\"Nota\""));
            cadena.append("\"");
            cadena.append(miles.at(i).note);
            cadena.append("\"");
            out << cadena ;
            out << "],\n";
            cadena.clear();
    }
    out << "\n  ]\n";

    out << "\n}\n";
    //result.chop(2);

    return result;
}
QString SafetWorkflow::currentGraphJSON(const QString& codegraph) {
    QString newresult = "";

    QString outformat = QString("{ \"nodes\":  [ %1 ],");


    QStringList mynodes = SafetYAWL::lastgraph.split("\n",QString::SkipEmptyParts);


    QString nodes = "";

    QList<uint> dates;

    if (mynodes.count() == 0 ) {
        SYD << tr("currentGraphJSON...**MYNODES:\n%1").arg(codegraph);
        return QString("{ \"nodes\": [ ]");
    }

    foreach(QString mynode, mynodes) {
        int pos = 0;
        QString nodejson = QString(" { %1 },");

       // QStringList myfields = mynode.split(",",QString::SkipEmptyParts);
        QString infonode;

        QRegExp rx;
        rx.setPattern("Nodo:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
        pos = mynode.indexOf(rx);
        if ( pos != -1 ) {

            infonode += QString(" \"name\": \"%1\",").arg(rx.cap(1));
        }

        rx.setPattern("Tipo:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
        pos = mynode.indexOf(rx);
        if ( pos != -1 ) {
            infonode += QString(" \"type\": \"%1\",").arg(rx.cap(1));
        }

        rx.setPattern("Reporte:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
        pos = mynode.indexOf(rx);
        if ( pos != -1 ) {
            infonode += QString(" \"report\": \"%1\",").arg(rx.cap(1));
        }

        rx.setPattern("Union:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
        pos = mynode.indexOf(rx);
        if ( pos != -1 ) {
            infonode += QString(" \"union\": \"%1\",").arg(rx.cap(1));
        }

        rx.setPattern("Patron:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
        pos = mynode.indexOf(rx);
        if ( pos != -1 ) {
            infonode += QString(" \"pattern\": \"%1\",").arg(rx.cap(1));
        }

        rx.setPattern("Titulo:\\s*([a-zA-Z0-9/\\+_\\.][a-zA-Z0-9/\\+_\\.\\s]+),");
        pos = mynode.indexOf(rx);
        if ( pos != -1 ) {
            infonode += QString(" \"title\": \"%1\",").arg(rx.cap(1));
        }

        rx.setPattern("Variable:\\s*([a-zA-Z0-9\\+_\\.][a-zA-Z0-9\\+_\\.\\s]+),");
        pos = mynode.indexOf(rx);
        if ( pos != -1 ) {
            infonode += QString(" \"variable\": \"%1\",").arg(rx.cap(1));
        }


        rx.setPattern("info.task.color:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
        pos = mynode.indexOf(rx);
        if ( pos != -1 ) {
            infonode += QString(" \"index\": \"%1\",").arg(rx.cap(1));
        }

        rx.setPattern("Siguiente:\\s*([a-zA-Z0-9\\+_\\.;]+)\\s*,");
        pos = mynode.indexOf(rx);
        if ( pos != -1 ) {
            QStringList mylist = rx.cap(1).split(";",QString::SkipEmptyParts);
            QString nexts = "[  ";
            foreach(QString n, mylist ) {
                nexts += "\""+ n + "\",";
            }
            nexts.chop(1);
            nexts += "]";

            infonode += QString(" \"following_nodes\": %1,").arg(nexts);
        }

        QString extrainfo = mynode.section(",",-1);
        SYD << tr("....SafetWorkflow::currentGraphJSON()...extrainfo:|%1|")
               .arg(extrainfo);
        QStringList  extralist = extrainfo.split(QRegExp("\\.\\.\\.|<br/>"),QString::SkipEmptyParts);
        int i = 1;

        foreach(QString e, extralist) {

            infonode += QString(" \"parameter%1\": \"%2\",").arg(i).arg(e);
            i++;
            QDateTime mydate = QDateTime::fromString(e, Safet::DateFormat);
            if (mydate.isValid()) {
                uint currdate = mydate.toTime_t();
                dates.append(currdate);
//                SYD << tr("\nDATESAPPEND format:|%1|").arg(e);
//                SYD << tr("DATESAPPEND:|%1|").arg(currdate);
                infonode += QString(" \"date\": \"%1\",").arg(QString("%1").arg(currdate));
            }
        }

        infonode.chop(1);

        nodejson = nodejson.arg(infonode);

        nodes +=  nodejson;

    }


     nodes.chop(1);
     outformat = outformat.arg(nodes);
    if (dates.count()  > 0 ) {

        qSort(dates);
        uint datebegin = dates.at(0);
        uint dateend = dates.at(dates.length()-1);


        outformat += QString(" \"first_date_number\": \"%1\",").arg(datebegin);
        outformat += QString(" \"last_date_number\": \"%1\",").arg(dateend);

        outformat += QString(" \"first_date_format\": \"%1\",")
                .arg(QDateTime::fromTime_t(datebegin).toString(Safet::DateFormat));
        outformat += QString(" \"last_date_format\": \"%1\",")
                .arg(QDateTime::fromTime_t(dateend).toString(Safet::DateFormat));
        outformat += QString(" \"lapse_date_nunmber\": \"%1\",")
                .arg(dateend-datebegin);
        outformat += QString(" \"lapse_date_text\": \"%1\",")
                .arg(SafetWorkflow::humanizeDate(dateend-datebegin).replace(tr("en espera").trimmed(),""));


    }

    outformat.chop(1);

    outformat += " }";
    newresult = outformat;

    return newresult;
}


QString SafetWorkflow:: generateGraph(char* filetype, QString& json, const QString& info) {

    if ( !checkSourcesTask())  {

        return QString(""); // Chequear que todos las tareas existan
    }
    QString codeGraph, parsedCodeGraph, result;
    SafetYAWL::_isstatstokenfound = false; // Inicializando por si es un unico token
    codeGraph = generateCodeGraph(filetype, info);

    SYA << tr("........SafetWorkflow::generateGraph....GENERATECODEGRAPH:\n|%1|\n")
           .arg(codeGraph);

    QString newCodeGraph;
    QList<SafetWorkflow::Miles> ordermiles;
    QStringList mylist = codeGraph.split("\n",QString::SkipEmptyParts);
    QSet<QString>  tasks;




    if (!codeGraph.isEmpty()) {
        QStringList codenodes;
        foreach(QString currnode, mylist) {
            QString nodename = currnode.split(",").at(0).mid(QString("Nodo:").length());
            SYD << tr("........CODENODE:|%1|").arg(nodename);
            codenodes.append(nodename);
        }

        SafetWorkflow::ExtraInfoSearchKey& sk = searchKey();
        SYD << tr("...sk.key():|%1|").arg(sk.key);
        SYD << tr("sk.extrainfo.keys().count(): |%1|").arg(sk.extrainfo.keys().count());

        QDateTime previewdate;


        for( int i = 0; i < sk.extrainfo.keys().count(); i++) {
            QString mykey = sk.extrainfo.keys().at(i);
            SYD << tr("...%1->%2")
                   .arg(mykey)
                   .arg(sk.extrainfo[ mykey]);

            QString mydata = sk.extrainfo[ mykey];

            if (codenodes.contains(mykey)) {
                continue;
            }

            QStringList mylist = mydata.split(QRegExp(";|<br/>"));
            if (mylist.count() < 3 ) {
                continue;
            }

            QDateTime mydate = QDateTime::fromString(mylist.at(1),"dd/MM/yyyy hh:mmap");
            if (!mydate.isValid()) {
                     continue;
            }

            SafetWorkflow::Miles mymile;
            mymile.nametask = mykey;
            tasks.insert(mykey);



            mymile.ts = mydate;

            mymile.secondswait = 0;


            mymile.humandate = mylist.at(2);
            QStringList myinfolist = mylist.at(0).split(QRegExp("\\s+"),QString::SkipEmptyParts);
            /**
              * FIXME: Chequear si es el rol del que registra o el rol del responsable"
              *  */
            mymile.rol = myinfolist.at(0);
            mymile.porc = "";
            if (myinfolist.count() > 1 ) {
                mymile.porc = myinfolist.at(1);
            }

            SafetTask* mytask = searchTask(mykey);

            if (mytask== NULL) {
                SYW << tr(" ALERT: In generating process Graph task \"%1\" not found")
                       .arg(mykey);
            }

            if (mytask != NULL) {
                bool ok;
                int tporc = mytask->textualinfo().toInt(&ok);
                int posc = mymile.porc.indexOf("%");
                int cporc  = 0;
                cporc =  mymile.porc.mid(0,posc).toInt(&ok);

                mymile.porcremaining = QString("%1%").arg(tporc-cporc);
             //   mymile.rol = mytask->role();
                mymile.note = mytask->note();

            }

            int pos = -1;
            for(int j=0; j < ordermiles.count(); j++) {
                if ( mymile.ts  < ordermiles.at(j).ts ) {
                    pos = j;
                    break;

                }
            }
            if (pos == -1 ) {
                pos = ordermiles.count();
            }
            SYD << tr ("....MainWindow::listDocuments insertando en la posicion: %1")
                   .arg(pos);
            ordermiles.insert(pos,mymile);
            previewdate  = mydate; // Colocar fecha anterior
        }



        SYD << tr("....SafetWorkflow::generateGraph...count():|%1|")
               .arg(ordermiles.count());
        QList<int> mymarknodes;



        for(int i = 0; i< ordermiles.count();i++) {
            int days;
            SafetWorkflow::Miles &m = ordermiles[i];

            m.id = (i+1);
            QString currnode;
            QString currtask = m.nametask;


            int marknode = 0;
            foreach(QString n,  mylist) {
                if ( n.split(",").at(0).trimmed() ==  QString("Nodo:%1").arg(currtask) ) {
                    currnode = n;
                    break;
                }
                marknode++;
            }
            if (marknode < mylist.count()) {
                mymarknodes.append(marknode);
            }
            QString newextra = currnode.section(",",-1);
            newextra = newextra.section("...",0,1)+"...";
            QDateTime mynow = QDateTime::currentDateTime();

            m.humanwait = tr("n/a");
            newextra += m.rol+" "+m.porc+"...";
            QString newextrainfo;
            if ( i > 0 ) {
                SafetWorkflow::Miles &prev = ordermiles[i-1];
                m.secondswait = m.ts.toTime_t()-prev.ts.toTime_t();
                m.humanwait = SafetWorkflow::humanizeDate(days,prev.ts.toString("dd/MM/yyyy hh:mm:ssap"),
                                                          "dd/MM/yyyy hh:mm:ssap",
                                                          m.ts,
                                                          SafetWorkflow::WaitTime);


                newextrainfo = QString("%1<br/>%2<br/>%3...")
                        .arg(m.ts.toString("dd/MM/yyyy hh:mmap"))
                        .arg(SafetWorkflow::humanizeDate(days,m.ts.toString("dd/MM/yyyy hh:mm:ssap"),
                                                         "dd/MM/yyyy hh:mm:ssap",
                                                         mynow,
                                                         SafetWorkflow::SinceWhenTime))
                        .arg(SafetWorkflow::humanizeDate(days,prev.ts.toString("dd/MM/yyyy hh:mm:ssap"),
                                                         "dd/MM/yyyy hh:mm:ssap",
                                                         m.ts,
                                                         SafetWorkflow::WaitTime));


            }
            else {
                newextrainfo = QString("%1<br/>%2<br/>%3...")
                        .arg(m.ts.toString("dd/MM/yyyy hh:mmap"))
                        .arg(SafetWorkflow::humanizeDate(days,m.ts.toString("dd/MM/yyyy hh:mm:ssap"),
                                                         "dd/MM/yyyy hh:mm:ssap",
                                                         mynow,
                                                         SafetWorkflow::SinceWhenTime))

                        .arg(tr("n/a"));

            }
            newextra += newextrainfo;

            currnode = currnode.section(",",0,-2)+","+newextra;
            newCodeGraph += currnode + "\n";

        }
        int marknode = 0;
        foreach(QString n,  mylist) {
            if (!mymarknodes.contains(marknode)) {
                newCodeGraph += n + "\n";
            }
            marknode++;
        }

        codeGraph = newCodeGraph;
    }

    QList<QSqlField> myfields;
    foreach(QString n,  mylist) {
        QString mytask = n.split(",").at(0).mid(QString("Nodo:").length());
        SafetTask* ptask = searchTask(mytask);
        if (mytask ==tr("inicial") || mytask == tr("final")) {
            continue;
        }
        if (ptask == NULL) {
            continue;
        }
        if (ptask->report() == "no") {
            continue;
        }

        if ( !tasks.contains(mytask) ) {
            QDateTime mydate = QDateTime::currentDateTime();
            SafetWorkflow::Miles mymile;
            mymile.nametask = mytask;
            mymile.id = 0;
            mymile.plannedts = mydate;
            mymile.porc = "0%";
            mymile.porcremaining = (ptask->textualinfo().isEmpty()?tr("n/reporte"):ptask->textualinfo()+"%");
            mymile.rol = ptask->role();
            mymile.note = ptask->note();
            mymile.prevtask = "n/a";
            mymile.ts = mydate;
            mymile.secondswait = 0;
            mymile.humandate ="n/a";
            mymile.humanwait ="n/a";
            ordermiles.append(mymile);

        }

    }

    json = SafetWorkflow::getJSONMiles(ordermiles,myfields);


    QString einfocoloured = SafetYAWL::getConf()["ExtraInfo/coloured"];

    if (einfocoloured == "yes") {

        codeGraph = calculateGraphFormula(newCodeGraph);

        codeGraph = SafetWorkflow::paintGraph(codeGraph);

    }
    SafetYAWL::_isstatstokenfound = false;
    QString type = QString(filetype);
    SafetYAWL::lastgraph = codeGraph;
    if ( SafetYAWL::curOutputInterface == NULL ) {
        SYD << "....SafetYAWL::curOutputInterface == NULL...";


        QStringList list= getTempNameFiles(1);
        Q_ASSERT( list.count() > 0 );


        QFile file(list.at(0));
        QString pathFileName;
        pathFileName.append(list.at(0));
        bool isopen = file.open(QIODevice::WriteOnly | QIODevice::Text);
        if ( !isopen ) {
           SYE << tr("No es posible escribir el archivo temporal: \"%1\"").arg(pathFileName);
            return result;
        }

        QTextStream out(&file);

        out << codeGraph;
        file.close();


        SYD <<
               tr("Nombre del archivo temporal flujo "
                  "de trabajo (codigo):%1").arg(pathFileName);
        result = pathFileName;

        return result;
    }

    Q_CHECK_PTR( SafetYAWL::curOutputInterface ) ; // Chequear que exista una Interfaz (Complemento )

    QMap<QString,QString> mymap = SafetYAWL::getConf().getMap();




    SYD << tr("...SafetWorkflow::generateGraph...parseCodeGraph....codeGraph (2):\n|%1|")
           .arg(codeGraph);

    parsedCodeGraph = SafetYAWL::curOutputInterface->parseCodeGraph(codeGraph, mymap);
    SafetYAWL::filelog.close();
    SafetYAWL::filelog.open(QIODevice::Append);
    SafetYAWL::streamlog.setDevice(&SafetYAWL::filelog);

    SYD << tr("...........SafetWorkflow::generateGraph:...........info (type):|%1|")
           .arg(type);

    result = SafetYAWL::curOutputInterface->renderGraph(parsedCodeGraph, type, mymap);
    if (result.startsWith("ERROR")) {
        SYE << tr("Ocurri� el siguiente error al generar el gr�fico de flujo \"%1\"")
               .arg(result);
        return QString("");
    }




    SafetYAWL::filelog.close();
    SafetYAWL::filelog.open(QIODevice::Append);
    SafetYAWL::streamlog.setDevice(&SafetYAWL::filelog);


    QLocale::setDefault(QLocale::Latin);
    setlocale(LC_ALL,"");

    return result;
}


QString SafetWorkflow::calculateGraphFormula(const QString& code, SafetWorkflow::ResultType rtype) {
    QString result;
    QStringList mynodes = code.split("\n",QString::SkipEmptyParts);

    QString einfosumm = SafetYAWL::getConf()["ExtraInfo/summarize"];

    QString etitle = SafetYAWL::getConf()["ExtraInfo/title"];

    if (etitle.isEmpty()) {
        etitle = tr("Valor:");
    }

    QMap<QString,QPair<QString,QString> >   nodesmap;

    bool iseinfosumm = (einfosumm == "yes");
    SYD << tr("....SafetWorkflow::paintGraph....einfosumm:|%1|").arg(einfosumm);

    if (!iseinfosumm) {
        return code;
    }

    QString epatttern0 = SafetYAWL::getConf()["ExtraInfo/pattern"]
            .replace("#PAR0","(")
            .replace("#PAR1",")")
            .replace("#CLA0","[")
            .replace("#CLA1","]")
            .replace("#SLA","\\");


    QString eformula = SafetYAWL::getConf()["ExtraInfo/formula"];


    QString finalnode;
    int totalporc = 0;
    int totalcount = 0;
    int totaldocs = 0;

    SYD << tr("....SafetWorkflow::calculateGraphFormula....epattern0:|%1|").arg(epatttern0);
    SYD << tr("....SafetWorkflow::calculateGraphFormula....eformula:|%1|").arg(eformula);


    foreach(QString mynode, mynodes) {

        QString mynewnode = mynode;
        QString s = mynode.section(",",-1).trimmed();
        QString namenode = mynode.section(",",0,0).section(":",1,1);


        Q_ASSERT(!namenode.isEmpty());


    SYD << tr("....SafetWorkflow::calculateGraphFormula...**s:|%1|").arg(s);

        QStringList myinfos =  s.split("...", QString::SkipEmptyParts);




        if (mynode.startsWith("Nodo:final,")) {

            mynewnode += QString(tr("...%2 %1...  ..."));
            finalnode = mynewnode;
            continue;
        }
        else {

            if (myinfos.count()  > 0 ) {

                bool ok;
                totaldocs += myinfos.at(0).toInt(&ok);


                if (myinfos.count() > 2 ) {
                    //QString myinfotext = myinfos.at(2);
                    QString myinfotext = s;
                    QRegExp myrx(epatttern0);
                    int pos = myinfotext.indexOf(myrx);
                    SYD << tr("....SafetWorkflow::calculateGraphFormula....myinfotext:|%1|").arg(myinfotext);
                    if (pos >= 0) {
                        bool ok;
                        totalporc += myrx.cap(1).toInt(&ok);
                        totalcount++;
                        nodesmap[namenode].first = myrx.cap(1);
                        nodesmap[namenode].second = "";

                    }
                    else {

                    }
                }
            }

            result += mynewnode;
            result += "\n";
        }
    }

    SYD << tr("....SafetWorkflow::calculateGraphFormula. (5)");
    SYD << tr("....SafetWorkflow::calculateGraphFormula. totalcount:|%1|").arg(totalcount);
    if ( eformula == Safet::SUMALL) {
  //      _porc = QString("%1%").arg(totalporc);
    if (rtype == Partial ) {
        SYD << tr("....SafetWorkflow::calculateGraphFormula. (6)");
        return QString("%1%").arg(totalporc);
    }
    else {
        SYD << tr("....SafetWorkflow::calculateGraphFormula. (7)");
                QString newfinal;
             if (epatttern0 == "Safet::Totaldocs")  {
                newfinal = finalnode.arg(QString("%1").arg(totaldocs)).arg(tr("Total:"));
             }
             else {
                newfinal = finalnode.arg(QString("%1").arg(totalporc)).arg(tr("Total:"));
             }

             SYD << tr("....SafetWorkflow::calculateGraphFormula. newfinal:|%1|").arg(newfinal);
             result += newfinal;
    }
    }
    else if ( eformula == Safet::AVGALL ) {
        if ( totalcount == 0) {
            SYE << tr("Error al calcular el promedio del grafo (cuenta igual a cero)");
            return result;
        }
        //_porc = QString("%1%").arg(double(totalporc)/double(totalcount));

    if (rtype == Partial ) {
        return QString("%1").arg(double(totalporc)/double(totalcount));
    }
    else {
            result += finalnode.arg(QString("%1%").arg(double(totalporc)/double(totalcount))).arg(tr("Promedio:"));
    }

    }
    else {
        // Buscar variables en eformula
        bool hasdatevar = false;

        for(int i = 0; i < nodesmap.keys().count(); i++)  {
            QString mykey = nodesmap.keys().at(i);
            QString myvar = nodesmap[mykey].first;
            if (myvar.isEmpty()) {
                continue;
            }
            if (myvar.endsWith("am") || myvar.endsWith("pm") ) {
                myvar = QString("%1").arg(QDateTime::fromString(myvar,Safet::DateFormat).toTime_t());
                hasdatevar  = true;
            }
            eformula.replace(QString("%1.0").arg(mykey),myvar);
        }

    }
    result += "\n";

    return result;
}




QString SafetWorkflow::paintGraph(const QString& code) {

    QString result;
    QStringList mynodes = code.split("\n",QString::SkipEmptyParts);

    QRegExp rxColor;

    rxColor.setPattern("info\\.task\\.color:\\s*([0-9\\.]+)");

    QString colorcompleted = QString("info.task.color: 0.4");
    QString colorcurrent = QString("info.task.color: 1");
    QString colorpartial = QString("info.task.color: 0.1");
    QString replacecolor = colorcompleted;

    QString spartial = SafetYAWL::getConf()["ExtraInfo/infotext.completed"];
    if (!result.isEmpty() ) {
        mynodes = result.split("\n",QString::SkipEmptyParts);
        result = "";
    }

    foreach(QString mynode, mynodes) {
        QString mynewnode = mynode;
        QString s = mynode.section(",",-1);

        QStringList myinfos =  s.split("...", QString::SkipEmptyParts);

        if (myinfos.count() > 2 ) {
            int pos = mynode.indexOf(rxColor);
            if (pos >= 0 ) {
                if ( rxColor.cap(1) == "1") {
                    replacecolor  = colorcurrent;
                }
                pos = s.indexOf(spartial);
                if ( pos>=0) {
                    replacecolor = colorpartial;
                }
            }
            else {
                result += mynewnode;
                result += "\n";
                continue;
            }

            mynewnode.replace(rxColor,replacecolor);
        }

        result += mynewnode;
        result += "\n";


    }

    return result;
}

QString SafetWorkflow::paintStats(const QString& code) {
    QString result;
    QStringList mynodes = code.split("\n",QString::SkipEmptyParts);


    QString finalnode;
    int totalporc = 0;
    foreach(QString mynode, mynodes) {
        QString mynewnode = mynode;
        QString s = mynode.section(",",-1);
        QStringList myinfos =  s.split("...", QString::SkipEmptyParts);
        if (mynode.startsWith("Nodo:final,")) {

            mynewnode += QString("Total: %1%...  ...");
            finalnode = mynewnode;
        }
        else {
            if (myinfos.count() > 2 ) {
                QString myinfotext = myinfos.at(2);
                QRegExp myrx("([0-9]+)\\%");
                int pos = myinfotext.indexOf(myrx);
                if (pos >= 0) {
                    bool ok;
                    totalporc += myrx.cap(1).toInt(&ok);
                }
            }
            result += mynewnode;
            result += "\n";
        }
    }

    result += finalnode.arg(totalporc);
    result += "\n";


    return result;
}


QString SafetWorkflow::convertSvgShapeToJSON(const QDomElement& n) {
	
	QString shape = "";
	if (  !SHAPEMAP.contains(n.nodeName())  ) {
		return shape;
	}
	int ishape = SHAPEMAP[ n.nodeName() ];
	shape = "\t\t\t{\n";	
	QMap<QString,QString> map;
   	QDomNamedNodeMap attrs = n.attributes ();
	
   	for(int i = 0; i < attrs.count(); i++) {  		
   		 map[attrs.item(i).nodeName()] = attrs.item(i).nodeValue();
   	 }
	QString currentName = n.nodeName(), str = "%1";
	bool ok;
	switch ( ishape ) {
		case 0: // Ellipse;

			map["cy"] = scaleToJSON(map["cy"],0,500,0,500);	
			currentName += str.arg(shapecount++); 
			shape += "\t\t\t\t\"name\": \""+ currentName +"\",\n";
			shape += "\t\t\t\t\"shape\": {\n\t\t\t\t \"type\": \"ellipse\",\n";
			shape += "\t\t\t\t\"cx\": " + map["cx"] + ",\n";
			shape += "\t\t\t\t\"cy\": " + map["cy"] + ",\n";
			shape += "\t\t\t\t\"rx\": " + map["rx"] + ",\n";
			shape += "\t\t\t\t\"ry\": " + map["ry"] + "";
			break;
		case 1:
 			map["d"] = scaleToJSON(map["d"],0,500,0,500);
			currentName += str.arg(shapecount++); 
			shape += "\t\t\t\t\"name\": \""+ currentName +"\",\n";
			shape += "\t\t\t\t\"shape\": {\n\t\t\t\t \"type\": \"path\",\n";
			shape += "\t\t\t\t\"path\": \"" +  map["d"] + "\"";
			break;
		case 2: // texto
			map["x"] = str.arg(map["x"].toInt(&ok) - (n.text().length()*3.5));
 			map["y"] = scaleToJSON(map["y"],0,500,0,500);
			currentName += str.arg(shapecount++); 
			shape += "\t\t\t\t\"name\": \""+ currentName +"\",\n";
			shape += "\t\t\t\t\"shape\": {\n\t\t\t\t \"type\": \"text\",\n";
			shape += "\t\t\t\t\"text\": \"" +  n.text() + "\",\n";
			shape += "\t\t\t\t\"x\": " +  map["x"] + ",\n";
			shape += "\t\t\t\t\"y\": " +  map["y"] + "";
			break;
		case 3:
 			map["points"] = scaleToJSON(map["points"],0,500,0,500);
			map["points"] = map["points"].replace(QRegExp("\\s+"), ",");
			currentName += str.arg(shapecount++); 
			shape += "\t\t\t\t\"name\": \""+ currentName +"\",\n";
			shape += "\t\t\t\t\"shape\": {\n\t\t\t\t \"type\": \"polyline\",\n";
			shape += "\t\t\t\t\"points\": [" +  map["points"] + "]";
			break;
			break;
	}			
	shape += "\n\t\t\t\t},";
	QString style ="";
	if ( ishape != 2 ) {
		style = addShapeStyleToJSON( map[ "style" ] );
	}
	else {
		style =  "\n\t\t\t\t\"fill\": \"black\",\n";
		style +=  "\t\t\t\t\"stroke\": \"black\"\n";	
	}
	shape += style;		
	if ( style.length() == 0  )  shape.chop(1);
	shape += "\n\t\t\t},\n";	
	return shape;
}

QString SafetWorkflow::scaleToJSON(const QString& s, int minx, int maxx, int miny, int maxy) {
	QString newnumber = "%1";
// 	int rangeorix = orimaxx - oriminx;
// 	int rangeoriy = orimaxy - oriminy;
// 	int rangenewx = maxx - minx;
// 	int rangenewy = maxy - miny;
	bool ok;
	QRegExp rx("(\\-\\d+)");
	QString str = s;
	int pos = 0;
	while ((pos = rx.indexIn(str, pos)) != -1) {
		double n =  rx.cap(1).toInt(&ok) - oriminy + miny;
		if ((pos-1) >= 0 && (pos-1) < str.length()  ) {
			if ( str.at( pos -1) != 'e' ) 
				str.replace(pos, rx.cap(1).length(), newnumber.arg(n));
		}
		else {
			str.replace(pos, rx.cap(1).length(), newnumber.arg(n));
		}
		pos += rx.matchedLength();

	}
	return str;
}

QString SafetWorkflow::addShapeStyleToJSON( const QString& s ) {
	QString str;
	str += "\t\t\t\t\n";
	QStringList list = s.split(";", QString::SkipEmptyParts);
	for (int i = 0 ; i < list.count(); i++) {
		QStringList fieldlist = list.at(i).split(":");
		Q_ASSERT_X(fieldlist.count() == 2, "addShapeStyleToJSON",
		qPrintable(tr("El campo estilo (style) del archivo svg/xml no contiene un numero de campos correctos: %1").arg(fieldlist.count())));
		str += "\t\t\t\t\"" + fieldlist.at(0) + "\":" + "\"" + fieldlist.at(1) + "\",\n";
			
	}
	str.chop(2);
	str += "\n";
	return str;
}

QString SafetWorkflow::svgToJSON(const QString& s) {
 	 QString jstr = "[\n";
	 QString currentName = "\"name\": \""+ QString("flujo") +"\",\n";
	 jstr += "{\n\t\t"+ currentName +"\t\t\"children\": [\n";
 	 QDomDocument doc;	
	 QFile file(s);
	 Q_ASSERT_X(QFile::exists(s), "svgToJSON",qPrintable(tr("Archivo SVG no existe: %1").arg(s)));
	 bool result = file.open(QIODevice::ReadOnly);
	 Q_ASSERT_X(result, "svgToJSON","No es posible abrir el archivo en modo solo lectura");
	 result = doc.setContent(&file);
         Q_ASSERT_X(result, "svgToJSON","No es posible leer adecuadamente el contenido del archivo XML");
	 file.close();
	 QDomElement docElem = doc.documentElement();
	 QDomNode n = docElem.firstChild();
	 QStack<QDomNode> control;	 
	 while( true ) {
		
		 if ( n.isNull() ) {
 	    	 if (control.isEmpty() ) break;
 	    	 n = control.pop();
 	 	 }		
		 if ( n.isElement() ) {
//		     qDebug("Nodo: %s", qPrintable(n.nodeName()));
		     jstr = jstr + convertSvgShapeToJSON( n.toElement() );   		
		     QDomElement e = n.toElement(); // try to convert the node to an element.
		     if ( n.hasChildNodes() && n.firstChild().isElement() ) {
		    	 control.push(n.nextSibling());
		    	 n = n.firstChild();
		    	 continue;
		     }
		 }
	    	 
	     n = n.nextSibling();	     	     
	 }	
	jstr.chop(2);
	jstr += "\n\t\t]\n\t}\n]";
	return jstr;
}

QString SafetWorkflow::addJScriptToMap( const QString& map ) {
	QString str = map;
	QRegExp rx("/>"); // Colocar el campo clave en la lista de campos de la sentencia SQL
	QRegExp rxRemove("title=\"([a-zA-Z0-9_ ]+)\""); // ExpresiÃ³n regular de remociÃ³n
    	rx.setCaseSensitivity(Qt::CaseInsensitive);
	int pos = 0;
	while ( true ) {
		pos = rxRemove.indexIn( str, pos);
		if ( pos == -1 ) break;
		QString label = rxRemove.cap( 1 );
//		qDebug("captura %d: %s", pos, qPrintable(label) );
		QStringList fields = label.split(QRegExp("\\s+"));
		if (fields.count() ==2 ) {
			QString replacestring = "onclick=\"alert('Datos')\" onmouseover=\"Tip('Existe(n)  <b><font color=black size=3>"+fields.at(0)+"</font></b>  documentos de tipo <br/> <b><font color=black size=3>"+ fields.at(1) +"</font></b> pendiente(s)',SHADOW, true,BGCOLOR, '#EEEE00')\" onmouseout=\"UnTip()\" ";
			str.replace( rxRemove.cap( 0 ), replacestring);
		}
		pos = pos + label.length()+1;
	}
	return str;
}

QString SafetWorkflow::addGraphvizNode(const QString& s, char* filetype,const QString& info) {
	SafetNode *node = nodemap[ s ];
	Q_CHECK_PTR( node );
	QString result, nameport, namepattern, namelink, urlstring, 
         type; // Tipo de archivo a generar por graphviz (png, cmap, imap, ...)
	SafetCondition *cond;
	SafetTask *task;
	int ntokens = 0;
	QString strcolor;
	int intcolor;
	QSet<QString> keys = getKeys(s);
	switch(SafetYAWL::getClassReference(SafetYAWL::translateNS(node->metaObject()->className()))) {
	case 2: // task
		task = qobject_cast<SafetTask*>(node);
		Q_CHECK_PTR(task);
		urlstring = "#";
		namelink ="node.";
		for(int i=0; i < task->getPorts().count(); i++) {
				nameport = task->getPorts().at(i)->type().toLower();
				namepattern = task->getPorts().at(i)->pattern().toLower();
				if ( nameport.compare( "split") == 0 || nameport.compare( "join") == 0 ) {
					namelink +=  nameport + "." +  namepattern; 
					break;
				}
			if ( (i-1)  ==  task->getPorts().count() ) namelink += "none";			
		}	
	
		if ( s.compare(tr("inicial"), Qt::CaseInsensitive) != 0 &&  
			s.compare(tr("final"), Qt::CaseInsensitive) != 0) {
			ntokens = numberOfTokens ( s ) ;
		}
		type =  filetype; // convertir de char* a QString
                intcolor = 255-(ntokens * 300)/6;
                strcolor = QString("#%1").arg(0, 2, 16,QLatin1Char('0'));
                strcolor = strcolor + QString("%1").arg(intcolor, 2, 16,QLatin1Char('0'));
                strcolor = strcolor + QString("%1").arg(0, 2, 16,QLatin1Char('0'));

                if ( type == "cmapx"  ||  type == "cmap"    ) {
                     QString alt = QString("%1 %2").arg(ntokens).arg(s);
                     urlstring = "\"#\"";
                     result = s + "[label=\"" + alt + "\",fontcolor=black,fontname=\"Bitstream Vera Sans\",color=\"#DEFFD7\",shape=box, URL=" + urlstring + "];\n";
                }
                else if ( type.compare("svg", Qt::CaseInsensitive ) == 0 ||
                          type.compare("png", Qt::CaseInsensitive ) == 0) {
                     QVariant myvar = SafetYAWL::getConfFile().getValue("Workflow", "defaultcolor");
                     QString statecolor = myvar.toString();
                     if ( info.length() == 0 ) {
                          QVariant myvar = SafetYAWL::getConfFile().getValue("Workflow", "activecolor");
                          statecolor = myvar.toString();
                     } else if (keys.contains(info)  ) {
                          QVariant myvar = SafetYAWL::getConfFile().getValue("Workflow", "traccolor");
                          statecolor = myvar.toString();
                     } else if ( info.compare( "stats:coloured", Qt::CaseInsensitive ) == 0  ) {
                          statecolor = strcolor;
                     }
                     result = s + "[label=\"" + s + "\",fontcolor=black,color=\""+ "black" + "\",style=filled,fillcolor=\""+statecolor+"\",shape=box];\n";
                     break;
                }
                else {
                     result = s + " [label=\"" + s + "\" ,fontsize=14,style=filled,bgcolor=yellow,fontcolor=white,fontname=\"Bitstream Vera Sans\",color=\""+strcolor+ "\",shape=box, URL=\"" + urlstring + "\"];\n";
                     //qDebug("StrColor result: %s", qPrintable(result));
                }

 		break;
	case 5: //condition
		cond = qobject_cast<SafetCondition*>(node);
		Q_CHECK_PTR(cond);
//		result = s + " [label=\"&#8226;\" fonsize=8,fontname=\"Bitstream Vera Sans\",color=white,shape=circle,shapefile=\""+
//		RESOURCESFILES[ "condition." + cond->type() ] + "\"];\n";
                result = s + " [label=\"&#8226;\",fontsize=16,fontname=\"Bitstream Vera Sans\",color=black,shape=circle" + "];\n";
		break;
	default:;
	}
	return result;
}


QString SafetWorkflow::generateSQLPortKeyFilterExpression(SafetPort* port, int curroption, int npath) {
    Q_CHECK_PTR( port );
   //   qDebug("....curroption: %d ...npath: %d", curroption, npath);
    QString sql = parser.checkBrackets(port->query(curroption)); // Chequear la consulta, quizÃ¡s se debe incluir tambien el patron
    Q_ASSERT_X(parser.error() ==	SafetParser::CORRECT, "generateSQLPortKeyFilterExpression",
               qPrintable(tr("Los parentesis no estÃ¡n balanceados en la expresion: <%1>").arg(port->query(curroption))) );
    parser.setStr(sql);
    parser.parse();

    _tablesource = parser.getTablesource();
    QString tokenlink = processTokenlink(port->tokenlink(),true,npath);

    bool result;
    if ( !SafetYAWL::canTypeConvert(sql, QVariant::Bool) ) {
               Q_ASSERT( parser.getFields().count() > 0 );
               result = delWhereClause(parser.getFields().at(0), true );

    }

    if ( parser.error() ==  SafetParser::CORRECT ) {

        Q_CHECK_PTR(getToken());
        sql = parser.addField(getToken()->key());
        sql = sql + tokenlink;

        if (lastkeyset.toList().count() > 0 ) {
            if (parser.getWhereclauses().count() == 0  ) {
                sql  += " WHERE " ;
            }
            else {
                sql  += " AND " ;
            }
           QString f = generateKeyFilterString();
            sql += ptoken->key() + " IN " + f;

        }
    }
    else {
//           qDebug("......generateSQLPortKeyFilterExpression.......(4).....");
        QString query = port->query(curroption);
        sql = "SELECT ";
        Q_CHECK_PTR(ptoken);
//           qDebug("......generateSQLPortKeyFilterExpression.......(7).....query:%s",qPrintable(query));
        int option  =  curroption;
        sql = sql + ptoken->key() + "," + query.trimmed() +" FROM " + ptoken->keysource();
        if (port->options().length() == 0 ) {
            port->setOptions(query);
        }

        QStringList optionlist = port->options().split("|");

        Q_ASSERT_X(option < optionlist.count(),"generateSQLPortKeyFilterExpression",
                   qPrintable(tr("La lista de opciones (options) no es correcta")));
        QString wexpression = 	generateWhereExpression(query,optionlist[option], true);

        sql = sql +  wexpression;
        if ( lastkeyset.toList().count() > 0 ) {
            QRegExp rx("\\s+WHERE\\s+(.)+");
            rx.setCaseSensitivity( Qt::CaseInsensitive );
            QString f = generateKeyFilterString();
            if ( sql.contains( rx )   ) {
                sql= sql + " AND " + ptoken->key() + " IN " + f;
            }
            else {
                sql= sql + " WHERE " + ptoken->key() + " IN " + f;
            }
        }
     }

    return sql;
}

QString SafetWorkflow::generateWhereExpression(const QString& f, const QString& o, bool haswhere) {
	QString str = " ";
	QVariant myvar;
	myvar = o;
	QString ope; // Cadena para el operador
        QRegExp rx("\\[([a-zA-Z_0-9\\.:\\-\\s;\\+]+)\\]");
	QString onew = SafetYAWL::processOperatorValue(o, ope).trimmed();

	QStringList oroptions = onew.split(",");
	// Colocar For
        if ( (f.compare("true") == 0) || (f.compare("false") == 0) ) {
		if (!haswhere ) {
			str = f;
		}
		else
			str = " WHERE " + f;
		
		return str;
	}	
	else 
	if ( haswhere ) 
		str = " WHERE ";
	foreach(QString oelement, oroptions ) {
		if ( SafetYAWL::canTypeConvert(oelement, QVariant::Bool) || SafetYAWL::canTypeConvert(oelement, QVariant::Int) ) {
			str +=  (f + " "+ ope + " "+ oelement).trimmed();
		}
		else if (SafetYAWL::canTypeConvert(oelement, QVariant::Date)) {
			QDate mydate = QDate::fromString( oelement, Qt::ISODate );
			QString strdate = QString("'%1-%2-%3'").arg(mydate.year()).arg(mydate.month(),2,10, QChar('0')).arg(mydate.day(),2,10, QChar('0'));
			str +=  (f + " "+ ope + " "+ strdate );
		} 
		else  {
                        rx.setPattern(".*\\s+from\\s+.*");

                        QRegExp rxfunc("[a-zA-Z0-9\\-_]{3,}\\([a-zA-Z0-9\\-_\\s',]*\\)");

                        SYD << tr("RXFUNC...olement:|%1|").arg(oelement);
                        SYD << tr("RXFUNC...pattern:|%1|").arg(rxfunc.pattern());
                        int pos = rxfunc.indexIn(oelement);
                        SYD << tr("RXFUNC...pos:|%1|").arg(pos);
                        if ( pos  >= 0 ) {
                            str +=  (f + " "+ ope + " "+ oelement).trimmed();

                            SYD << tr("RXFUNC...str:|%1|").arg(str);
                        }
                        else {

                            if (rx.indexIn( oelement) >= 0 ) {
                                   str +=  (f + " "+ ope + " "+ oelement );
                            }
                            else {
                               QRegExp rx("((AND|OR|NOT)\\s+|'(.*)')");
                               rx.setCaseSensitivity( Qt::CaseInsensitive );
                               int pos = rx.indexIn( oelement );
                               if ( pos == -1 ) {
                                  str +=  (f + " "+ ope + " '" + oelement +"'").trimmed();
                               }
                               else {
                                  str +=  (f + " "+ ope + " " + oelement + " ").trimmed();
                               }
                            }
                        }
		}
		str += " OR ";
	}	
	str.chop(4);

	return str;	
}

QString SafetWorkflow::generateJoinString(int numberPath, const QString& nametable) const {
	QString sql = "";
	QQueue<QString> tokenlinkqueueInPathCopy = tokenlinkqueueInPath;
	QQueue<QString>        tokenlinkqueueCopy = tokenlinkqueue;

	while ( !tokenlinkqueueCopy.isEmpty() ) {
		sql += " " + tokenlinkqueueCopy.dequeue();
	}
	QRegExp rx;
	rx.setCaseSensitivity(Qt::CaseInsensitive);
	while ( !tokenlinkqueueInPathCopy.isEmpty() ) {		
		QString allsql = tokenlinkqueueInPathCopy.dequeue();
		bool found = false;
		foreach(QString s, tokenlinkqueue) {
			if (allsql.startsWith(s.left(s.indexOf("ON", 0, Qt::CaseInsensitive)) ) ) {
					found = true;
					break;
				
			}
		}
		if (found) continue;
		QString path = allsql.section('|',1,1);
		allsql.remove("|" + path);
                rx.setPattern("JOIN\\s+([a-zA-Z_0-9\\.\\=']+)");
		int pos = allsql.indexOf(rx, Qt::CaseInsensitive);
		QString mytable;
		if ( pos > - 1 ) mytable = rx.cap(1);		
		
		if ( numberPath == path.toInt() ) {
			if ( nametable.length() == 0 || mytable.compare( nametable,Qt::CaseInsensitive) != 0 ) {
			sql += " " + allsql;
			}
		}
	}
	
	return sql;
}


QString SafetWorkflow::generateKeyFilterString(bool brackets) const {

	QString str;
    if ( lastkeyset.size() == 0 ) {
        return brackets?"(NULL)":"NULL";
    }
    if (brackets) {
        str+= "(";
    }

    QSet<QString>::const_iterator i;
     for (i = lastkeyset.begin(); i != lastkeyset.end(); ++i) {
         str  += "'" + *i + "'";
         str  += ",";
     }
     str.chop(1);
	if (brackets) str+=")";
	return str;
}





bool SafetWorkflow::doSubFlow(const QString& flowname, const QString& passname) {
    SYD << tr("...........SafetWorkflow::doSubFlow......(1)...");
#ifdef SAFET_MAINWINDOW
                QString myaction = QString("operacion:Listar_datos  "
                        "Cargar_archivo_flujo:/home/vbravo/.safet/flowfiles/%1.xml  "
                        " Variable:%2")
                        .arg(flowname)
                        .arg(passname);





//                QString myaction = QString("operacion:Generar_gr�fico_para_clave_Proyecto_Software  "
//                                           " Cargar_archivo_flujo:/home/vbravo/.safet/flowfiles/flujoAppPlanificacionSoftware.xml   "
//                                           " Clave:87");


                MainWindow myinflow("/home/vbravo");

                myinflow.setMediaPath("/var/www/media");
                myinflow.setHostURL("http://seguridad.cenditel.gob.ve/safet/intranet");
                bool result = myinflow.login("vbravo","d91408cd");
                SYD << tr("...........SafetWorkflow::doSubFlow....result(login): %1").arg(result);

                SYD << tr("...........SafetWorkflow::doSubFlow....myaction: %1").arg(myaction);

                if (!result ){
                    return false;
                }
                if ( !myinflow.toInputConsole(myaction,false) ) {
                    SYW << tr("....................SafetWorkflow::doSubFlow...TOINPUTCONSOLE FAIL" );
                    return false;
                }


                QString currjson = myinflow.currentJSON();

                if (currjson.length() > 0 ) {
                    SYD << tr(".............SafetWorkflow::doSubFlow............................*MARKJSON:|%1|")
                           .arg(currjson);

                    QRegExp rx;
                    rx.setPattern("safetlist\\s*\\=\\s*\\[(.+)\\]");

                    int pos = rx.indexIn(currjson);

                    SYD << tr(".............SafetWorkflow::doSubFlow....pos:|%1|")
                           .arg(pos);


                    if (pos >= 0) {
                        QString registers = rx.cap(1);
                        SYD << tr(".............SafetWorkflow::doSubFlow....RXJSON:|%1|")
                               .arg(registers);
                        int offset = 0;
                        // REGEXP revisar
                        rx.setPattern("\\{\\s*([a-zA-Z_0-9]+):\\s*'([a-zA-Z_0-9\\-#]+)\\s*");
                        int count = 0;


                        while (offset >= 0 ) {
                            offset = rx.indexIn(registers, offset);

                            if ( offset == -1 ) {
                                continue;
                            }
                            offset++;
                            count++;
                            SYD << tr(".............SafetWorkflow::doSubFlow....pos (registers):|%1|.....count:|%2")
                                   .arg(offset)
                                   .arg(count);
                            QString mykey = rx.cap(2);

                            SYD << tr(".............SafetWorkflow::doSubFlow.................found:|%1|")
                                   .arg(mykey);

                            lastkeyset &= mykey;


                        }
                        SYD << tr(".............SafetWorkflow::doSubFlow........addSet.........adding...........mykey");


                        foreach(QString k, lastkeyset) {

                            SYD << tr(".............SafetWorkflow::doSubFlow........addSet.........adding..... k:|%1|")
                                   .arg(k);

                        }





                    }

                }



#endif



    return true;
}

QString SafetWorkflow::generateKeyFilterExpression(QString& s,SafetNode* node, int curroption,
                                                   const QQueue<QString>& path , bool isjoin, int ipath) {
        Q_CHECK_PTR(node);
        QString result;
	SafetPort* port = node->port();
	Q_CHECK_PTR(port);
	QSet<QString> newkeyset;
	SafetSQLParser localparser;
        localparser.setWorkflow(this);
	localparser.setStr(s);	
        localparser.parse();
	QString strsign = localparser.signClause();




    if ( strsign.length() > 0 ) {

        if (strsign.startsWith("PASS ")) { // **** Generar consultas a subflujos
            QString flowname = localparser.currentFlowName();
            SYD << tr("...........SafetWorkflow::generateKeyFilterExpression................strsign flowname:|%1|")

                   .arg(flowname);
            QString passname = strsign.mid(QString("PASS ").length());
            SYD << tr("...........SafetWorkflow::generateKeyFilterExpression.................strsign  passname:|%1|")
                   .arg(passname);

            // **** Generar consultas a subflujos


            bool issub =  doSubFlow(localparser.currentFlowName(),passname);

            SYD << tr("...........SafetWorkflow::generateKeyFilterExpression.... ..........*****issub:|%1|")
                   .arg(issub);

            if (issub) {
                s.remove( strsign );

                result = generateKeyFilterString();

                SYD << tr("...........SafetWorkflow::generateKeyFilterExpression.... ..........result:|%1|")
                       .arg(result);
                return result;
            }

        }

    }

    if ( strsign.length() > 0 ) {
                   s.remove( strsign );
           }

	if ( strsign.length() == 0 ) {
                if ( localparser.getFields().count() <  2 ) {
                    if (s.trimmed().compare("true",Qt::CaseInsensitive) != 0 ) {                        
                        SYE << tr("La sentencia SQL \"%1\" debe tener al menos un campo").arg(s);
                    }
                    return result;
                }

		QString expression = localparser.getFields().at(1);
                bool istabletoken = true;
                QString str = generateWhereFilter(expression, node,curroption,ipath);
//                qDebug("...........whereset.str: %s", qPrintable(str));
//                foreach(QString s, whereset) qDebug(".....whereset...s: %s", qPrintable(s));
		whereset.push_back(str);	
		if ( isjoin ) 
			joinWhereset += str;
		istabletoken = false;		
                generateNewKeySet(s, newkeyset, true, node, curroption, path, false, istabletoken,"",ipath);

	} else if ( strsign.length() > 0 ) {
                generateNewKeySet(s, newkeyset, true, node, curroption, path, true  /* checkSign */, false /* tabletoken */,
                                  localparser.signers()/* lista de firmantes */, ipath);

   }
    if ( isStartingKeyFilter || lastkeyset.count() == 0) {
        lastkeyset          = newkeyset;
        isStartingKeyFilter = false;
    }
    else {
        lastkeyset &= newkeyset; // Interseccion de viejos con nuevo
    }
    if ( currentfilterkey.length() > 0 ) {
		lastkeyset += dropkeys[ currentfilterkey ];
		delWhereClause(currentfilterkey);
		currentfilterkey = "";
	}
        result = generateKeyFilterString();

	return result;
}

bool SafetWorkflow::delWhereClause(const QString& wc, bool onlynamefield) {
	QList<QString>::iterator i;
//	qDebug("...............wc: %s", qPrintable(wc));
 	for (i = whereset.begin(); i != whereset.end(); ++i) {
	    	QString k = *i;
                k.remove(QRegExp("[\\s']"));
                bool value  = onlynamefield? (k.startsWith(wc)) : (k == wc);
                if ( value ) {
                        whereset.erase(i);
                        if ( onlynamefield ) {
                             for (int j = 0; j < dropkeys.keys().count(); j++) {
                                    if ( dropkeys.keys().at(j).startsWith( wc ) ) {
                                          lastkeyset +=  dropkeys[ dropkeys.keys().at(j) ];
                                          break;
                                    }
                             }
                        }
			break;
		}
	} 
}


void SafetWorkflow::generateNewKeySet(const QString& s, QSet<QString> &set,
                                      bool found, SafetNode* node,
                int curroption, const QQueue<QString>& path, bool checkSign,
                bool checkTableToken, const QString& l, int ipath) {
    Q_CHECK_PTR(node);

    SafetPort* port = node->port();
    Q_CHECK_PTR(port);

    int option = 0;
    option = curroption;// Opcion del camino para el filtro
    QList< QStringList > optionlists; // Para cuando existe un operador (p.e. and, or, xor, etc)
    QStringList querys; // Para cuando existe un operador (p.e. and, or, xor, etc)
    int matchpath = 0;

    // *** Colocar la  opcion ACTUAL como primera
    if ( curroption < port->getConnectionlist().count()  ) {
        matchpath = curroption;
    }

    QStringList optionlist = port->options(matchpath).split("|");
    optionlists.push_back( optionlist );
    querys.push_back( s );
      SafetNode* myparent = qobject_cast<SafetNode*> (port->parent());
      Q_CHECK_PTR( myparent );
    QString pattern = port->pattern();

    if ( port->getConnectionlist().count() == 0) {
         SYE << tr("Numero de conexiones (connections) vac�as en el nodo: \"%1\"").arg(node->id());
         return;
    }
    if ( port->getConnectionlist().count() > 1 ) {

         if  (pattern.length() == 0 || pattern.compare(tr("none")) == 0 ) {

                     SYE <<
          tr("Un puerto que contiene varias conexiones necesita de un "
             "patron/operador diferente a vacio o \"none\","
             "el patron/operador de un puerto de la tarea \"%1\" esta vacio")
                  .arg(myparent->id());
            return;
        }
        for ( int i = 0; i < port->getConnectionlist().count(); i++ ) {
            if ( i != matchpath ) {
                querys.push_back(port->getConnectionlist().at(i)->query());
                QStringList myoptionlist = port->getConnectionlist().at(i)->options().split("|");
                optionlists.push_back(myoptionlist);
            }
        }
    }
    if ( checkTableToken || checkSign || optionlist.count() == 0) {
        if (curroption > port->getConnectionlist().count() -1 ) curroption = port->getConnectionlist().count() -1;
        checkForSignFilter(s,port->getConnectionlist().at(curroption),l,checkSign, set);
        return;
    }


   Q_ASSERT( querys.count() > 0 );
   proccessQueryFilter(pattern, querys, optionlists, set, found, node, 0 /* curroption */, path, ipath);


}


void SafetWorkflow::checkPatternWithOthers(QString pattern, QStringList& querys,
                                        QList< QStringList >& optionlists, QSet<QString> &set,
                                        bool found, SafetNode* node,
                                        int curroption, const QQueue<QString>& path,int ipath) {


    if (querys.count() == 1 ) {
        return;
    }

    // Hacer facil agregar mas patrones (PATRON)
    if (pattern.compare("or") == 0 ) {
        return;
    }

    QString debugope,debugnew = SafetYAWL::processOperatorValue(
            optionlists.at(0)[0], debugope).trimmed();
    SafetYAWL::streamlog
            << SafetLog::Debug
            << tr("..1..( checkPatternWithOthers) Sentencia SQL: \"%2\" Opcion (#ONEW): \"%1\"").arg(debugnew)
            .arg(querys.at(0));

    QSet<QString> otherspathsset;

    for( int i=1; i < querys.count();i++ ) {
        otherspathsset.clear();
        QString curquery = querys.at(i);
        parser.setSql(curquery);
        parser.parse();
        curquery = parser.addField(getToken()->key()) /*+ tokenlink*/;
        QSqlQuery query(curquery, SafetYAWL::currentDb); // <-- SafetYAWL::currentDb puntero a db actual

        /*QString news = */
        checkDropFilter(curquery,node,path,ipath); // Chequear si existe filtros drop
        QString ope; // Cadena para el operador a efectuar

        QString onew = SafetYAWL::processOperatorValue(
                optionlists.at(i)[0], ope).trimmed();
        SYD  << tr("( checkPatternWithOthers) Sentencia SQL: \"%2\" Opcion (#ONEW): \"%1\"").arg(onew)
                .arg(curquery);

        // ********* Colocar las fichas eliminadas antes de aplicar el filtro

        while (query.next()) {
            //qDebug("ornew: |%s|; ope: |%s| value: |%s|", qPrintable(ornew), qPrintable(ope), qPrintable(query.value(1).toString().trimmed()));
             QString valueforeval = query.value(1).toString().trimmed();

            if ( (found &&  SafetYAWL::evalValues(valueforeval,onew,ope)) || !found) {              

                otherspathsset.insert(query.value(0).toString());

            }

        }
        // *****************************************************************************************
        // *****************************************************************************************
        otherspathsset &= lastkeyset; // Intersectar con el ultimo grupo de claves
        // Aplicando el patron ...
        // ** Importante este paso .... (ACA INCLUIR OPERADOR XOR Y OTROS)

        if (pattern.compare("and") == 0 ) {            
            set &= otherspathsset;
        }
        // *****************************************************************************************
        // *****************************************************************************************
    }

}



void SafetWorkflow::proccessQueryFilter(QString pattern, QStringList& querys,
                                        QList< QStringList >& optionlists, QSet<QString> &set,
                                        bool found, SafetNode* node,
                                        int curroption, const QQueue<QString>& path,int ipath) {
    //  Q_ASSERT( querys.count() == optionlists.count() );
    int option = curroption;
    QSqlQuery query(querys.at(0 /* chequear para varias opciones */), SafetYAWL::currentDb); // <-- SafetYAWL::currentDb puntero a db actual

    /*QString news = */
    checkDropFilter(querys.at(0),node,path,ipath); // Chequear si existe filtros drop
    QString ope; // Cadena para el operador a efectuar
    QString lastk; // Ultima clave a evaluar
    QString keyfilter; // Filtro completo para la clave
    Q_ASSERT(option < optionlists.count() );
 //   foreach (QString s, optionlists.at(0)) qDebug("...........:%s", qPrintable(s));
    QString onew = SafetYAWL::processOperatorValue(
            optionlists.at(option)[0 /*chequear para varias opciones > 0 */], ope).trimmed();

    QStringList oroptions = onew.split(","); // chequear para casos del operador  or
    int poslist = checkInternalOrOptions(onew,optionsset,lastk);

    // ********* Colocar las fichas eliminadas antes de aplicar el filtro
//    SafetYAWL::streamlog
//            << SafetLog::Debug
//            << tr("(proccessQueryFilter) Sentencia SQL: \"%2\" Opcion (onew): \"%1\"").arg(onew)
//            .arg(querys.at(0));

    while (query.next()) {
        if ( poslist > -1 )  { // Esto es para admitir el operador "O" a traves ";"
            keysoptions[ query.value(0).toString().trimmed() ].insert (query.value(1).toString().trimmed());
            keyfilter = query.record().fieldName(1)+ope+SafetYAWL::addQuotes(lastk.trimmed());
        }
        else {
            foreach(QString ornew, oroptions) {
                 //qDebug("ornew: |%s|; ope: |%s| value: |%s|", qPrintable(ornew), qPrintable(ope), qPrintable(query.value(1).toString().trimmed()));
                if ( (found &&  SafetYAWL::evalValues(query.value(1).toString().trimmed(),ornew,ope)) || !found) {                    
                        set.insert(query.value(0).toString());
                }
                else {
                    keyfilter = query.record().fieldName(1)+ope+SafetYAWL::addQuotes(ornew);
                    SafetPort *outport = node->outport();
                    Q_CHECK_PTR( outport );
                    outport->setFilterkey( keyfilter );
                    dropkeys[ keyfilter ].insert(query.value(0).toString());
                }
            }
        }
    }
    checkPatternWithOthers(pattern,querys,
                           optionlists,set,
                           found,node,
                           curroption,path,ipath);
}


void SafetWorkflow::clearDropkeys(const QString& filters) {
QRegExp rx("^"+filters);

//qDebug("........filters......:%s", qPrintable("^"+filters));
for (int i = 0; i < dropkeys.keys().count(); i++) {
 //   qDebug("....clearDropkeys.........dropkeys.keys().at(i)...:%s", qPrintable(dropkeys.keys().at(i)));
     int index = dropkeys.keys().at(i).indexOf(rx);
//     qDebug("........index........:%d", index);
     if ( index > -1 ) {
         dropkeys[ dropkeys.keys().at(i) ].clear();
     }
 }
}

bool SafetWorkflow::checkPatternWithOthersPath(const QString& value, const QString& pattern, const QStringList& querys, const QList< QStringList >& optionlists) {
        Q_ASSERT( pattern.length() > 0 );
//          qDebug(".....checkPatternWithOthersPath.......value:%s", qPrintable(value));
//          qDebug(".....checkPatternWithOthersPath.....pattern:%s", qPrintable(pattern));
        if ( pattern.compare(tr("none")) == 0 || pattern.compare(tr("or")) == 0) return true;
         if ( (pattern.compare(tr("and")) == 0 && value.compare("8") == 0) ||
              (pattern.compare(tr("and")) == 0 && value.compare("4") == 0) ||
              (pattern.compare(tr("and")) == 0 && value.compare("2") == 0) ||
              (pattern.compare(tr("and")) == 0 && value.compare("9") == 0) ||
    //          (pattern.compare(tr("and")) == 0 && value.compare("6") == 0) ||
              (pattern.compare(tr("and")) == 0 && value.compare("3") == 0) ) {
//              qDebug("....return from 3.or...9.. true....value: %s", qPrintable(value));
             return true;
         }
        return false;
}


int SafetWorkflow::checkInternalOrOptions(const QString& onew, QSet<QString>& optionsset, QString& lastk) {
        QRegExp rx("\\[([a-zA-Z_0-9\\.;\\+]+)\\]");
        int poslist = onew.indexOf( rx );
        optionsset.clear();
        if (poslist > -1 ) {
                QStringList mylist = rx.cap(1).split(";");
                foreach(QString k, mylist) {
                        optionsset.insert( k );
                        lastk = k;
                }
        }
        return poslist;
}

void SafetWorkflow::checkForSignFilter(const QString& s, SafetConnection* conn,
                                       const QString& l, bool checkSign,
                                       QSet<QString> &set) {
    Q_CHECK_PTR( conn ) ;

    QString option = conn->options();
    QSqlQuery query(s, SafetYAWL::currentDb); // <-- SafetYAWL::currentDb puntero a db actual

     bool operatorOption = true;
    if ( conn != NULL ) {

         if ( option.compare(tr("no"),Qt::CaseInsensitive ) == 0 ) {
              operatorOption = false;
         }
    }
    if (checkSign) {
          checkDocuments(query,l,set,operatorOption);
    }
    else {
        while (query.next()) {
            set.insert(query.value(0).toString());
        }
    }

}

QString SafetWorkflow::checkDropFilter(const QString& s,SafetNode* node, const QQueue<QString> &path, int ipath) {
	Q_CHECK_PTR( node );
	QString result = s;
	// 2 Task, 5, Condition
	SafetTask *ptask = NULL;
	SafetCondition *pcond = NULL;
	SafetPort* pport = node->outport();
	Q_CHECK_PTR( pport );
	QString nodename;
	int classtype = SafetYAWL::getClassReference(SafetYAWL::translateNS(node->metaObject()->className()));
	switch( classtype ) {
		case 2: // SafetTask
			ptask = qobject_cast<SafetTask*>(node);
			Q_CHECK_PTR(ptask);
			nodename = ptask->id();
			break;
		case 5:
			pcond = qobject_cast<SafetCondition*>(node);
			Q_CHECK_PTR(pcond);
			nodename = pcond->id();	
		default:;
	}
	if ( pport->drop().length() > 0 ) {
		Q_ASSERT( nodename.length() > 0 );
		int hownodes = pport->drop().toInt();
        SYD << tr("..........SafetWorkflow::checkDropFilter......id:|%1|...drop:|%2|")
               .arg(node->id())
               .arg(hownodes);
		// Buscar el node respectivo
		int curr = path.indexOf ( nodename );
		curr = curr - hownodes;
		if ( curr < 0 ) curr = 0;
		QString newnodename = path.at( curr );

        //SafetNode* filternode = nodemap[ "Tarea1" ];
        SafetNode* filternode = nodemap[ newnodename ];
		Q_CHECK_PTR ( filternode );
		SafetPort *outport = filternode->outport();
		Q_CHECK_PTR( outport );
        SYD << tr("..........SafetWorkflow::checkDropFilter......newnodename:|%1|")
               .arg(newnodename);

		Q_ASSERT( outport->filterkey().length() > 0 );	
        Q_ASSERT( dropkeys.contains( outport->filterkey() )  );

        SYD << tr("..........SafetWorkflow::checkDropFilter......dropkeys.contains( outport->filterkey():|%1|")
               .arg(dropkeys.contains( outport->filterkey()));

        foreach(QString k, dropkeys[outport->filterkey()]) {
            SYD << tr("..........SafetWorkflow::checkDropFilter......k:|%1|")
                   .arg(k);

        }

		currentfilterkey = outport->filterkey() ;
        SYD << tr("..........SafetWorkflow::checkDropFilter......currentfilterkey:|%1|")
               .arg(currentfilterkey);
	}

	return result;
}

QString SafetWorkflow::generateWhereFilter(const QString& f, SafetNode* node, int curroption,int ipath) {
	Q_CHECK_PTR(node);
	SafetPort* port = node->port();
	Q_CHECK_PTR(port);
        int option = 0;
        option = curroption;
        QStringList optionlist = port->options(option).split("|");
        if ( option >= optionlist.count() ) option = optionlist.count() -1 ;

  //      qDebug("\t\t....optionlist[option]: %s", qPrintable(optionlist[option]));
	QString str = generateWhereExpression(f,optionlist[option]) ;
	return str;		
}


SafetCondition* SafetWorkflow::getStartCondition() {
	QList<SafetCondition*>::iterator i;
	SafetCondition *curcondition;
	 for (i = conditionlist.begin(); i != conditionlist.end(); ++i) {
		curcondition = *i;
		Q_CHECK_PTR(curcondition);
		if (curcondition->type() == QObject::tr("start") ) {
			return curcondition;
		}
	 }
	 return NULL;
}

QList<SafetTask*>& SafetWorkflow::getTasks() {
	return tasklist;
}

QString SafetWorkflow::listTasks(bool inc, const QString& c) {
     QString result;
     foreach(SafetTask* mytask, tasklist) {
                result += mytask->id();
                result += c;
        }
     if ( inc ) {
     QList<SafetCondition*> conditions = getConditions();
        foreach(SafetCondition* mycondition, conditions) {
                result += "*";
                result += mycondition->id();
                result += c;
        }
     }
     return result;

}


QStringList SafetWorkflow::getVariablesByRole(const QString& r) {
    QStringList result;
    foreach(SafetTask* mytask, tasklist) {
          if ( mytask->role() == r) {
               foreach(SafetVariable* v, mytask->getVariables()) {
                   result.append(v->id());
                   SYD << tr("SafetWorkflow::getVariableByRole:...|%1|").arg(v->id());
               }
            }

       }
        return result;
}



QList<SafetCondition*>& SafetWorkflow::getConditions() {
	return conditionlist;
}

SafetToken* SafetWorkflow::getToken() {
	return ptoken;
}

QMap<QString,SafetNode*>& SafetWorkflow::getNodes() {
	return nodemap;
}



QString SafetWorkflow::getSQLOperator(const QString& o) const {
	QString result;
	if ( o.compare("and", Qt::CaseInsensitive) == 0 ) result =" INTERSECT ";
	else if ( o.compare("or", Qt::CaseInsensitive) == 0 )result = " UNION ";
	else if ( o.compare("xor", Qt::CaseInsensitive) == 0 ) result = " EXCEPT ";
	return result;
}

QString SafetWorkflow::addSQLPrefix(const QString& sql, const QString& s) const {
	QString newsql;
        QRegExp rx("^\\s*\\(*\\s*SELECT\\s*(.)+\\s+FROM\\s+(.)+");
	rx.setCaseSensitivity( Qt::CaseInsensitive );       
	if ( s.contains( rx )   ) 	return s;

	if ( s.length() > 0 )  {
		newsql = sql + " WHERE " + ptoken->key() + " IN " + s;
	}
	else 
		newsql = sql + " WHERE FALSE";	

	return newsql;
}



QString SafetWorkflow::processSQLOperation(const QString& sql, QStack<QString> &stack, const QString& ope) const {
        QString result;
	SafetSQLParser localparser;
        localparser.setWorkflow((SafetWorkflow* const)this);
	localparser.setStr(sql);
	localparser.parse();
	if ( ope.compare("xor", Qt::CaseInsensitive) == 0 ) {
		QStack<QString> mystack;
		while(!stack.isEmpty() ) {
			QString newsql = stack.pop();
			QString mypath = newsql.section('|',1,1);
			newsql.remove("|" + mypath);
			QString tokenlink = generateJoinString( mypath.toInt(),localparser.getTablesource() );
			newsql = addSQLPrefix(sql+" "+tokenlink, newsql);

			mystack.push(newsql);
			result  = result + newsql;							
			if (!stack.isEmpty() ) 	result = result + " " + getSQLOperator(ope) + " ";			
		}
		result = "("+ result + ") UNION (";
		while(!mystack.isEmpty() ) {
			QString newsql = mystack.pop();
			result  = result + newsql;				
			if (!mystack.isEmpty() ) 	result = result + " " + getSQLOperator(ope) + " ";			
		}	
		result = result + ")";
	}
	else {
		while(!stack.isEmpty() ) {
			QString newsql = stack.pop();
			QString mypath = newsql.section('|',1,1);
			newsql.remove("|" + mypath);
			QString tokenlink = generateJoinString( mypath.toInt(), localparser.getTablesource() );
			newsql = addSQLPrefix(sql+" "+tokenlink, newsql);			

			result  = result + newsql;
			if (!stack.isEmpty() ) result = "(" + result + ") " + getSQLOperator(ope) + " ";
		}		
	}
	stack.push(result);
	return result;
}


QString SafetWorkflow::generateGraphKey(const QString& s, char* filetype) {
	
    QString myjson;
    QString result = generateGraph(filetype, myjson, s /* info de la clave */);
	return result;
}



bool SafetWorkflow::checkDocuments(QSqlQuery& query,
                                  const QString& signatories /* lista de firmantes */,
                                  QSet<QString> &set /* Conjunto Salida */,
                                  bool operatorOption /*para negacion de la firma */) {

    SYD << tr("................checkDocuments...NO DEFINE SAFET_DIGIDOC");
#ifdef SAFET_DIGIDOC
    SYD << tr("................checkDocuments...NO DEFINE SAFET_DIGIDOC**");
    // ************* prueba para abrir el archivo de configuracion de digidoc ******
    if ( _libdigidocConfigFile.isEmpty()) {
        _libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfilepath").toString();
        _libdigidocConfigFile = QDir::homePath() + "/" + Safet::datadir + "/" + "digidoc.conf";
    }
#endif

    QString remoteContainer = SafetYAWL::getConfFile()
                              .getValue("XmlRepository",
                                        "xmlrepository.remote")
                              .toString();

    QString type = SafetYAWL::getConfFile().getValue("XmlRepository",
                                                     "xmlrepository.type").toString();
    if ( type == "dir" && _xmlRepository == NULL )  {
        _xmlRepository = new SafetDirXmlRepository();
    }
#ifdef SAFET_DBXML   // Incluir la biblioteca dbxml para soporte de Base de Datos XML Nativa
    else if ( type == "dbxml" )  {
        xmlRepository = new SafetDbXmlRepository();

    }
#endif // Libreria DbXml
    QString url = SafetYAWL::getConfFile().getValue("XmlRepository",
                                                    "xmlrepository.remote.urlwebservice")
    .toString();

    QString home = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.path").toString();
    QString homefile = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.name").toString();
    QString mypath = home + "/" + homefile;
    SYD << tr("checkDocument...h:|%1|").arg(mypath);

    _xmlRepository->openContainer(mypath);

    bool isremotecontainer = remoteContainer.compare("on",Qt::CaseInsensitive) == 0;
    QStringList signers;
    int commapos = signatories.indexOf(",");
    int commapointpos = signatories.indexOf(";");
    if ( commapos != -1 && commapointpos != -1 ) {

                SYE << tr("No es posible incluir los caracteres \",\" y \";\""
                      " en una sentencia de una firma: \"%1\"")
                .arg(signatories);
        return false;
    }

    SYD << tr("checkDocument...(2)....");
    SafetDocument::SignatureOperator signatureOp = SafetDocument::AND;

    if ( commapos != -1 )    {
        signers = signatories.split(",");

    }
    else if ( commapointpos  != -1 ) {
        signers = signatories.split(";");
        signatureOp = SafetDocument::OR;
    }
    else {
        if (signatories == "#ANYBODY#") {
            signatureOp = SafetDocument::ALL;
        }
        signers.append(signatories);

    }

#ifdef SAFET_DIGIDOC
    SafetDocument doc;
    doc.initializeLibdigidoc();
    doc.initDigidocConfigStore(_libdigidocConfigFile);
#endif

#ifdef SAFET_DIGIDOCPP
    SYD << tr("checkDocument...(3)....");
     digidoc::initialize();
         SYD << tr("checkDocument...(3.1)....");
     digidoc::X509CertStore *store = new digidoc::DirectoryX509CertStore();
     SYD << tr("checkDocument...(3.2)....");
     digidoc::X509CertStore::init(store);
    SYD << tr("checkDocument...(4)....");
     BDocDocument doc;
     SYD << tr("checkDocument...(5)....");
#endif


     SYD << tr("\n\n........CHECKDOCUMENT...SafetWorkflow::getXMLDocument.....");
      for(int i = 0; i < query.record().count(); i++) {
          SYD << tr("...........SafetWorkflow::getXMLDocument....fieldname:|%1|")
                 .arg(query.record().fieldName(i));
          SYD << tr("...........SafetWorkflow::getXMLDocument....value:|%1|")
                 .arg(query.record().value(i).toString());

      }
      SYD << tr("...........SafetWorkflow::getXMLDocument.....\n\n");


    while (query.next()) {
        QString documentid = SafetYAWL::getDocumentID(query,true);

//        SYD << tr("...........SafetWorkflow::checkDocuments..DOCUMENTID..fieldname:|%1|")
//               .arg(query.record().fieldName(0));
//        SYD << tr("...........SafetWorkflow::checkDocuments....value:|%1|")
//               .arg(query.record().value(0).toString());

//        SYD << tr("...........SafetWorkflow::checkDocuments..DOCUMENTID..DOCUMENTID:|%1|")
//               .arg(documentid);
	// ********* prueba de llamada a metodo remoto de servicio web safet **************
	// obtener valores del archivo de configuracion	

      if ( isremotecontainer ) {
            SYA << tr("WebService (GSOAP) actualmente no est� soportado");
       }

        if (documentid.startsWith("Ci")) {
            //        SYD << tr("...........SafetWorkflow::checkDocuments..CiDocument:|%1|")
            //               .arg(documentid);

        }

       if (!_xmlRepository->searchDocumentInContainer(documentid)) {

           if (SafetWorkflow::verifyNegation(signers))  {
                set.insert(query.value(0).toString());
            }
            continue;
        }

        QString archivoEscrito = home + "/" + homefile + "/" +documentid;
	
	
	// ************* prueba para leer un contenedor desde un archivo en el sistema de archivos*************
#ifdef SAFET_DIGIDOC
        doc.readOpenXAdESContainerFile(archivoEscrito);
#endif
#ifdef SAFET_DIGIDOCPP
        SYD << tr("................checkDocuments...checking....file (bdoc):|%1|").arg(archivoEscrito);
        if (!QFile::exists(archivoEscrito)) {
            SYD << tr("................checkDocuments...doc.openBDocContainer...no existe:|%1|").arg(archivoEscrito);
            return true;
        }
        SYD << tr("................checkDocuments...doc.openBDocContainer...before...");
            doc.openBDocContainer(archivoEscrito);
       SYD << tr("................checkDocuments...doc.openBDocContainer........OK!");



#endif

	
        bool r = false;
#ifdef SAFET_DIGIDOC
        r = doc.verifySignMadeWithSmartCard(archivoEscrito, signers,isneg,signatureOp);
#endif
         int nsignatures = 0;
#ifdef SAFET_DIGIDOCPP

         if (signatureOp == SafetDocument::ALL ) {
            SYD << tr("................checkDocuments...signer..#ANYBODY# YES...");
             set.insert(query.value(0).toString());
             r = true;
             return true;
         }

         SYD << tr("................checkDocuments...signer..#ANYBODY# NO...");
         SYD << tr("\n Signers:");
        for( int i = 0; i< signers.count(); i++) {
            SYD << tr("................checkDocuments...signer...:|%1|").arg(signers.at(i));

        }

        SYD << tr("\n Common Names:");


        bool hasvalid = !(SafetYAWL::getConf()["GeneralOptions/signatures.validate"].compare("off") == 0 );

        bool signerpresent = false;
        bool startneg = false;
        for( int i = 0; i< doc.signatureCount(); i++) {
            QString common = doc.subjectCertificateCommonName(i);
            SYD << tr("...............NOT CONTAIN.checkDocuments...common:|%1|").arg(common);

            for( int j = 0; j < signers.count(); j++) {
                QString currsigner = signers.at(j);
                if ( currsigner.startsWith("!")) {
                    startneg = true;
                    currsigner = currsigner.mid(1);
                    if (common == currsigner) {
                        signerpresent = true;
                        break;
                    }
                }
            }

        }
        if ( startneg && !signerpresent) {
             set.insert(query.value(0).toString());
        }

        for( int i = 0; i< doc.signatureCount(); i++) {
            QString common = doc.subjectCertificateCommonName(i);
            SYD << tr("................checkDocuments...common:|%1|").arg(common);
            if (signers.contains(common) ) {
              SYD << tr("................checkDocuments...common.....Contain! ok!");
              /**
                *FIXME:
                * Quitar || true para ver validaciones reales, o colocar una variable en el archivo safet.conf que
                * verifique esto
                */
                if ( (hasvalid == false) || doc.validateSignature(i) == 0 ) {
                    SYD << tr("................checkDocuments...common.....validateSignature! ok!");
                    nsignatures++;
                    SYD << tr("................checkDocuments...r=true for:|%1|").arg(common);

                }
                SYD << tr("................checkDocuments...common.....after....validateSignature! ok!");

            }


        }
 #endif

        if (signatureOp == SafetDocument::AND ) {
            if (nsignatures  == signers.count()) {
                r = true;
            }
        }
        else {
            if ( nsignatures > 0 ) {
                r = true;
            }

        }

        if ( r && operatorOption ) {
            set.insert(query.value(0).toString());
        }

    }
#ifdef SAFET_DIGIDOC
    doc.closeLibdigidoc();
#endif


    SYD << tr("................checkDocuments...returning....ok!");
    return true;
}




bool SafetWorkflow::verifyNegation(const QStringList& list) {
    foreach(QString s, list) {
        if ( s.startsWith("!")) {
            return true;
        }
    }
    return false;

}

QList<SafetWorkflow::InfoSigner> SafetWorkflow::getSignersDocument(const SafetVariable* v,
                                                                   const QString& key,
                                                                   QString &derror) {
        QList<InfoSigner>  result;
#ifdef SAFET_DIGIDOC // Firma Electronica Digidoc

        QString documentid;
        QString dirid, pathid; // Variables para almacenar el documento

        SafetDocument doc;
        doc.initializeLibdigidoc();


        doc.createOpenXAdESContainer();

        // ************* prueba para abrir el archivo de configuracion de digidoc ******
        QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc",
                                                                        "libdigidoc.configfilepath")
                                        .toString();
        libdigidocConfigFile = QDir::homePath() + "/" + Safet::datadir + "/" + "digidoc.conf";
        doc.initDigidocConfigStore(libdigidocConfigFile);


        // Campos a incluir en el archivo de configuracion
        dirid       = "/tmp/"; //Directorio Temporal
        int fieldno = 1; // Numero de campo de la consulta


        qDebug("(1)....key:|%s|",qPrintable(key));
        QString searchkey = key;
        v->getXMLDocument(searchkey.trimmed(), fieldno, documentid );
        qDebug("(2)....key:|%s|",qPrintable(key));

        if (documentid.length() == 0 ) {
//            SafetYAWL::streamlog
//                    << SafetLog::Error
//                    << tr("El documento que tiene la clave: \"")
//                        << searchkey.trimmed()
//                        << tr("\" asociado a la variable \"")
//                        << v->id() << tr("\" no se encuentra en el repositorio de datos")
//                        << endl;
            qDebug("(*)Documento con clave: |%s| no se encuentra (getSignersDocument)",
                   qPrintable(searchkey.trimmed()));
                return result;
        }
        pathid  = dirid + documentid;


        QString home = SafetYAWL::getConfFile().getValue("XmlRepository",
                                                         "xmlrepository.path").toString();
        QString homefile = SafetYAWL::getConfFile().getValue("XmlRepository",
                                                             "xmlrepository.name").toString();

        SafetXmlRepository *xmlRepository = NULL;

        QString remoteContainer = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.remote").toString();
        bool founddoc = false;
        if (remoteContainer.compare("off",Qt::CaseInsensitive) == 0) {

                qDebug("...........remoteContainer.compare....off");
                QString type = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.type").toString();
                if ( type == "dir" )  {
                        xmlRepository = new SafetDirXmlRepository();
                }
                Q_CHECK_PTR( xmlRepository );
                if (QFile::exists( home + "/" + homefile) ) {
                        xmlRepository->openContainer(home + "/" + homefile);
                        founddoc =  xmlRepository->searchDocumentInContainer(documentid);
                        if ( founddoc ) {
                                doc.readOpenXAdESContainerFile(home + "/" + homefile + "/" + documentid);
                                qDebug("readOpen...: %s", qPrintable(home
                                                                     + "/" + homefile
                                                                     + "/" + documentid));
                                    QList<bool> ocspsigns = doc.verifyOCSPSign();
                                 for ( int i = 0; i < doc.numberOfSignaturesOnOpenXAdESContainer(); i++){
                                     InfoSigner mysigner;
                                     mysigner.commonName = doc.getCN(i);
                                     mysigner.issuer = doc.getSignerCertificateIssuerName(i).trimmed();
                                     mysigner.date = doc.getSingingTimeOnlyDate(i);
                                     mysigner.hour = doc.getSingingTimeOnlyHour(i);
                                     mysigner.location = doc.signatureLocations(i).join(",");
                                     mysigner.role = doc.getRole(i);
                                     SignatureInfo* mysignature = doc.SigInfo(i);
                                     if (mysignature != NULL ) {
                                         int len = 0;

                                         if (mysignature->pSigValue->mbufSignatureValue.nLen < 2048) {
                                             len = mysignature->pSigValue->mbufSignatureValue.nLen;
                                         }
                                         for(int j=0; j < len; j++) {
                                            mysigner.hexsign
                                                    .push_back(((char*)mysignature->pSigValue->mbufSignatureValue.pMem)[j]);
                                         }


                                         len = 0;
                                         DataFile *mydatafile = NULL;
                                         if (doc.numberOfDataFileOnOpenXAdESContainer() > 0 ) {
                                             doc.dataFile(0);
                                             if (mydatafile != NULL ) {
                                                 qDebug("...mydatafile->mbufDigest.nLen..:%d",
                                                        mydatafile->mbufDigest.nLen);
                                                 qDebug("...mysignature->pSigInfoRealDigest->mbufDigestValue.nLen:%d",
                                                        mysignature->pSigInfoRealDigest->mbufDigestValue.nLen);
                                                 int nLen = mysignature->pSigInfoRealDigest->mbufDigestValue.nLen;
                                                 if (nLen > 50) {
                                                     nLen = 50;
                                                 }
                                                 if (nLen >0 ) {
                                                     for(int j=0; j < nLen; j++){
                                                         qDebug("...getSignerDocument(1)...");
                                                         mysigner.digest.push_back(((char*) mydatafile->mbufDigest.pMem)[j]);
                                                         qDebug("...getSignerDocument(2)...");
                                                     }
                                                 }

                                             }
                                         }

                                     }


                                     if ( i < ocspsigns.count()) {
                                         mysigner.isvalid = ocspsigns.at(i);
                                     }
                                     else {
                                         mysigner.isvalid = false;

                                     }
                                     result.append(mysigner);
                                 }
                        }
                        else {
                            return result;
                        }
                }
                else {
                            return result;

                }
        }



        doc.closeLibdigidoc();
#endif

#ifdef SAFET_DIGIDOCPP
        QString documentid;
        QString dirid, pathid; // Variables para almacenar el documento
        digidoc::initialize();
        digidoc::X509CertStore *store = new digidoc::DirectoryX509CertStore();
        digidoc::X509CertStore::init(store);




        // ************* prueba para abrir el archivo de configuracion de digidoc ******

        // Campos a incluir en el archivo de configuracion
        dirid       = "/var/www/media/"; //Directorio Temporal
        int fieldno = 0; // Numero de campo de la consulta



        QString searchkey = key;
        v->getXMLDocument(searchkey.trimmed(), fieldno, documentid );

        if (documentid.length() == 0 ) {
            SYD << tr("(*)Documento con clave: |%1| no se encuentra (getSignersDocument)").arg(searchkey.trimmed());
                return result;
        }
        pathid  = dirid + documentid;

        SYD << tr("........SafetWorkflow::getSignersDocuments........*FIRMAR DOCUMENTO.......pathid:|%1|")
               .arg(pathid);

        BDocDocument bdoc;

        SYD << tr(".........SafetWorkflow::getSignersDocuments....pathid:|%1|").arg(pathid);

        if (!QFile::exists(pathid)) {
            SYD << tr(".........SafetWorkflow::getSignersDocuments...NO EXISTS pathid");

            return result;
        }


        bdoc.openBDocContainer(pathid);

        int ns = bdoc.signatureCount();

        SYD << tr(".........SafetWorkflow::getSignersDocuments....signatureCount:|%1|").arg(ns);

        for(int i = 0; i< ns; i++) {
            InfoSigner mysigner;
            mysigner.commonName = bdoc.subjectCertificateCommonName(i);
            mysigner.issuer = bdoc.issuerCertificateCommonName(i);
            mysigner.date = bdoc.signatureDateTime(i);
            mysigner.location = bdoc.signatureLocation(i).join(",");
            mysigner.role = bdoc.signatureRol(i);
            mysigner.isvalid = bdoc.validateSignature(i) == 0;
            int value = bdoc.validateSignature(i);
            mysigner.typevalid = value==0?tr("Valida"):(value==2?tr("N/A"):tr("Invalida"));
            mysigner.serial = bdoc.subjectCertificateSerialNumber(i);

           result.append(mysigner);
        }




#endif // DIGIDOCPP

        return result;

}

QString SafetWorkflow::signDocument(const SafetVariable* v, const QString& key, QString &derror) {

        QString documentid;
#ifdef SAFET_DIGIDOC // Firma Electronica Digidoc
	QString dirid, pathid; // Variables para almacenar el documento
//        DbXml::setLogLevel(DbXml::LEVEL_ALL, true);
//        DbXml::setLogCategory(DbXml::CATEGORY_ALL, true);

	SafetDocument doc;
        doc.initializeLibdigidoc();
	
	
	doc.createOpenXAdESContainer();
	
	// ************* prueba para abrir el archivo de configuracion de digidoc ******
	QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfilepath").toString();
        libdigidocConfigFile = QDir::homePath() + "/" + Safet::datadir + "/" + "digidoc.conf";
	doc.initDigidocConfigStore(libdigidocConfigFile);

	
	// Campos a incluir en el archivo de configuracion
	dirid       = "/tmp/"; //Directorio Temporal
	int fieldno = 1; // Numero de campo de la consulta	


	QString xml = v->getXMLDocument(key, fieldno, documentid );
 	//qDebug("xml:\n%s", qPrintable(xml));
	SafetYAWL::streamlog << SafetLog::Debug << tr("Archivo XML a Firmar:\n%1").arg(xml);
//	documentid  = v->id().trimmed() + "-" + key.trimmed();
        qDebug("documentid: |%s|", qPrintable(documentid));
	if (documentid.length() == 0 ) {
            SafetYAWL::streamlog
                    << SafetLog::Error
                    << tr("El documento con la clave: \"")
                        << key
                        << tr("\" asociado a la variable \"")
                        << v->id() << tr("\" no se encuentra en el repositorio de datos")
                        << endl;
                return QString("");
	}	
	pathid  = dirid + documentid;
        qDebug("pathid: |%s|", qPrintable(pathid.section(".",0,0) + ".xml"));

//	doc.createXMLFileFromQuery(query, pathid + ".xml");
        QFile file(pathid.section(".",0,0) + ".xml");
	
	bool open = file.open(QIODevice::WriteOnly | QIODevice::Text); 
        if (!open ) {

            SafetYAWL::streamlog
                    << SafetLog::Error
            << qPrintable(QObject::tr("No se pudo crear el archivo XML "
                                      "correspondiente a la sentencia SQL."));

            return QString("");
        }

	
	QTextStream out(&file);
	out << xml;
	out.flush();
	file.close();
        QString home = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.path").toString();
        QString homefile = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.name").toString();

	SafetXmlRepository *xmlRepository = NULL;

        QString remoteContainer = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.remote").toString();
	bool founddoc = false;
	if (remoteContainer.compare("off",Qt::CaseInsensitive) == 0) {

                QString type = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.type").toString();
		if ( type == "dir" )  {
			xmlRepository = new SafetDirXmlRepository();
		}
#ifdef SAFET_DBXML   // Incluir la biblioteca dbxml para soporte de Base de Datos XML Nativa 
		else if ( type == "dbxml" )  {
			xmlRepository = new SafetDbXmlRepository();
	
		}
#endif // Libreria DbXml
		Q_CHECK_PTR( xmlRepository );
		if (QFile::exists( home + "/" + homefile) ) {
//			qDebug("....xmlRepository->openContainer...documentid:%s", qPrintable(documentid));
			xmlRepository->openContainer(home + "/" + homefile);
			founddoc =  xmlRepository->searchDocumentInContainer(documentid);
//			qDebug("...............found : %d", found);
			if ( founddoc ) {
				doc.readOpenXAdESContainerFile(home + "/" + homefile + "/" + documentid);
//				qDebug("readOpen...: %s", qPrintable(home + "/" + homefile + "/" + documentid));
				SafetYAWL::streamlog << SafetLog::Action << tr("Contenedor \"%1\" encontrado").arg(homefile);
				SafetYAWL::streamlog << SafetLog::Action << tr("Documento \"%1\" encontrado y esta firmado previamente").arg(documentid);
			}
			else {
                                doc.addFileToOpenXAdESContainer(doc.signedDoc(), pathid.section(".",0,0) + ".xml",
					SafetDocument::SAFET_CONTENT_EMBEDDED_BASE64, "application/xml");
				SafetYAWL::streamlog << SafetLog::Action << tr("Contenedor \"%1\" encontrado").arg(homefile);
				SafetYAWL::streamlog << SafetLog::Action << tr("Documento \"%1\" creado").arg(documentid);
			}
		}
		else {
			SafetYAWL::streamlog << SafetLog::Action << tr("Creando contenedor \"%1\" ").arg(home + "/" + homefile);
			xmlRepository->createContainer(home + "/" + homefile, "XmlContainer::WholedocContainer");	
                        doc.addFileToOpenXAdESContainer(doc.signedDoc(), pathid.section(".",0,0) + ".xml",
					SafetDocument::SAFET_CONTENT_EMBEDDED_BASE64, "application/xml");
			SafetYAWL::streamlog << SafetLog::Action << tr("Contenedor \"%1\" creado").arg(homefile);
	
	
		}	
	}

	// ************* prueba para agregar un archivo a contenedor digidoc ******
	
	// ************* prueba para agregar atributos a un archivo existente en un contenedor digidoc ******
//	doc.addAttributeToFile(doc.dataFile(), pathid.section(".",0,0) + ".xml", "Fecha", "Martes 01/07/08");
//	doc.addAttributeToFile(doc.dataFile(), pathid.section(".",0,0) + ".xml", "Hora", "18:52pm");
	

	// ************* prueba para cargar driver de la tarjeta inteligente ********
	doc.testSmartCardDriver(0);
	
	SafetYAWL::streamlog << SafetLog::Action << tr("Carga del driver de la tarjeta probada ...OK!");
	// ************* prueba firma con tarjeta inteligente***********************

	SafetYAWL::streamlog << SafetLog::Action << tr("Esperando a ingresar el PIN....OK!");
        bool ok;
      QString pin = (SafetYAWL::evalInput())(SafetYAWL::PIN, tr("Ingrese Pin de la Tarjeta:"), ok);
        if ( !ok ) {
             SafetYAWL::streamlog << SafetLog::Action << tr("PIN No se ingreso...No se aplica la firma...OK!");
             return QString("");
        }
	SafetYAWL::streamlog << SafetLog::Action << tr("PIN Ingresado...esperando chequeo...OK!");

//qDebug("ANTES DE doc.signWithSmartCard");
//        doc.signWithSmartCard(doc.signedDoc(), "T6qB1jbe", "Desarrollador","Merida", "Merida", "5101", "Venezuela");
        int err = doc.signWithSmartCard(doc.signedDoc(), pin, "Desarrollador","Merida", "Merida", "5101", "Venezuela");
        if ( err != ERR_OK ) {
                QString stringerror = SafetDocument::digidocStringError(err);
                if (stringerror.isEmpty() ) {
                    stringerror = getErrorString(err);
                }

               SafetYAWL::streamlog
                       <<  SafetLog::Error
                       << tr("No se pudo realizar la Firma "
                       "Electrónica sobre el documento: error:\"%1\"")
                       .arg(stringerror);
               derror = stringerror;
               return QString("");

        }
        SafetYAWL::streamlog << SafetLog::Action << tr("PIN validado correctamente....OK!");


	SafetYAWL::streamlog << SafetLog::Debug << tr("Documento firmado correctamente ...OK!");

	// ************* prueba segunda firma con tarjeta inteligente***********************
	
//qDebug("DESPUES DE doc.signWithSmartCard");
	
	
	// ********* prueba de llamada a metodo remoto de servicio web safet **************
	
	// obtener valores del archivo de configuracion	
	if (remoteContainer.compare("on",Qt::CaseInsensitive) == 0) {

#ifdef SAFET_GSOAP // Definido conexion a servicios Web
		struct soap soap;
   		bool result;
                // QString stringToReturn = doc.returnFileToString(pathid + ".xml");
                doc.writeOpenXAdESContainerToFile((founddoc)?(pathid.section(".",0,0) + ".ddoc"):"", pathid.section(".",0,0) + ".ddoc");
                QString stringToReturn = doc.returnFileToString(pathid.section(".",0,0) + ".ddoc");

		//qDebug(qPrintable(stringToReturn));
                QString url = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.remote.urlwebservice").toString();
                //url = "http://192.168.37.148/cgi-bin/safet";
                //qDebug(qPrintable(url));

                SafetYAWL::streamlog << SafetLog::Action << tr("Llamando de llamar a ServicioWeb soap_call_ns__guardarXml a la direccion: \"%1\"").arg(url);
                SafetYAWL::streamlog << SafetLog::Debug << tr("Archivo a firmar remotamente: \"%1\"").arg(stringToReturn);
                SafetYAWL::streamlog << SafetLog::Debug << tr("Identificador (id) del documento a firmar remotamente: \"%1\"").arg(documentid);
		if (soap_call_ns__guardarXml(soap_new(), qPrintable(url), NULL, (char*)qPrintable(stringToReturn), (char*)qPrintable(documentid), result) == SOAP_OK)
   		{
			if (result)
			{
                                //printf("result = verdadero\n");
                                SafetYAWL::streamlog << SafetLog::Action << tr("Llamado a servicio:soap_call_ns__guardarXml exitoso...Ok!");
			}
			else
			{
                                //printf("result = false\n");
                                SafetYAWL::streamlog << SafetLog::Action << tr("Llamado a servicio:soap_call_ns__guardarXml...NO fue exitoso.. no Ok!");
			}
   		}
   		else
   		{
//			printf("\nERROR != SOAP_OK\n");
			soap_print_fault(&soap, stderr);
                        SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al llamar al servicio soap_call_ns__guardarXml ");
   		}
		soap_destroy(&soap);
  		soap_end(&soap);
	  	soap_done(&soap);
#else // SAFET_GSOAP
                SYA << tr("No se encuentra definido un gestor de servicios WEB (para firmar documentos)");

#endif  // SAFET_GSOAP
	}
	else {
		// ************* prueba para escribir contenedor en sistema de archivos ***************
                doc.writeOpenXAdESContainerToFile((founddoc)?(pathid.section(".",0,0) + ".ddoc"):"", pathid.section(".",0,0) + ".ddoc");
                SafetYAWL::streamlog << SafetLog::Debug
                        << tr("Documento \"%1\" escrito correctamente").arg(pathid.section(".",0,0) + ".ddoc");

                SafetYAWL::streamlog << SafetLog::Debug << tr("addXmlDocumentToContainer ruta del contenedor de archivos XML: %1").arg(pathid.section(".",0,0));
                xmlRepository->addXmlDocumentToContainer(pathid.section(".",0,0) + ".ddoc", documentid);
	}

//	qDebug("xmlRepository->addXmlDocumentToContainer(pathid.section(".",0,0) + \".ddoc\", documentid):%s,%s",
//	qPrintable(pathid.section(".",0,0) + ".ddoc"),qPrintable(documentid));

//	qDebug("...despues...documentid");
	int r = ERR_OK;
	doc.closeLibdigidoc();
#endif // DIGIDOC

	return documentid;

}



SafetVariable*  SafetWorkflow::searchVariable(const QString& n) {
	
	foreach(SafetTask* task, getTasks() ) {
		foreach(SafetVariable* variable, task->getVariables() ) {
                     Q_CHECK_PTR( variable );
            //         qDebug("var: %s", qPrintable(variable->id()) );
			if ( variable->id() == n ) {
				return variable;
			}	
		}
	}
	return NULL;
}






QStringList SafetWorkflow::UniteWhereset() {
        QRegExp rx("([a-zA-Z_0-9\\.][a-zA-Z_0-9\\.\\(\\)]*)\\s*(\\=|>|<|<\\=|>\\=|IS|LIKE)\\s*(['\"a-zA-Z_0-9:\\-\\.]+)");
        rx.setCaseSensitivity(Qt::CaseInsensitive);
        QStringList droplist;
	QList<QString> newwhereset;
	QSet<QString> fields;
	for( int i = whereset.size()-1; i >= 0; i--) {	
		QString w = whereset.at(i);
		int pos = w.indexOf(rx);	
                qDebug(".................w: %s", qPrintable(w));
		if ( SafetYAWL::canTypeConvert(w, QVariant::Bool) ) continue;
		Q_ASSERT( pos > -1 );
		QString field = rx.cap(1);
		if (!fields.contains(field) ) {
			newwhereset.push_back( w );
			fields.insert( field );
		}
		else {	
			w.remove(QRegExp("[\\s']"));
			droplist.push_back( w );			
		}
	}
        whereset = newwhereset;
        return droplist;
}


QString SafetWorkflow::generateWhereClauses(const QString& s, int ipath) {
//        QStringList droplist = UniteWhereset();
//	QString splitresult = addFilterToKeySet(s,droplist,ipath);
        QString splitresult;
//          qDebug("...before..splitresult:%s",qPrintable(splitresult));
          splitresult  = s + ((whereset.count() > 0 ) ? " AND " : " ");
//          qDebug("...after..splitresult:%s",qPrintable(splitresult));
        int i = 0;
	for(; i < whereset.count(); i++) {
		splitresult = splitresult + "(" + whereset.at(i) +" )";		
		splitresult += " AND ";
	}
	if ( i > 0) 
		splitresult.chop(5);

	whereset.clear();
	return splitresult;
}


bool SafetWorkflow::checkFilterNULL(const QString& f) {
    if ( f.trimmed().compare("('')", Qt::CaseInsensitive) == 0 ) 
			return false;

    return true;
}


bool  SafetWorkflow::checkSourcesTask() {
     foreach(SafetTask *curtask, tasklist) {
          Q_CHECK_PTR( curtask);
          for( int i = 0 ; i < curtask->outport()->getConnectionlist().count(); i++) {
               QString source = curtask->outport()->getConnectionlist().at(i)->source();
               if ( ! searchTask( source ) && !searchCondition( source)  ) {
                    SafetYAWL::streamlog << SafetLog::Error
                            << tr("<b>Falla  de la configuración del archivo de Flujo de Trabajo"
                                  "(xml)</b>: El nodo (tarea/condición) con id: \"%1\" no se encuentra en el enlace de la tarea \"%2\"").arg(source).arg(curtask->id());
                    return false;
               }

          }
     }
     return true;
}

QString SafetWorkflow::adjustTimeZone(const QString& d, const QString& formatin,
                                      const QString& formatout) {

    QMap<QString,int> zones;
    zones["America/Caracas"] = -5*3600+3600/2; // Una hora 3600 segundos
    QString result = d;
    QString timezone = SafetYAWL::getConf()["GeneralOptions/timezone"].trimmed();

    QDateTime mydate = QDateTime::fromString(d,formatin);
    if (zones.contains(timezone)) {
        mydate= mydate.addSecs(zones[timezone]);
    }
    result = mydate.toString(formatout);

    return result;

}


QString SafetWorkflow::humanizeDate(int seconds) {

    QString result;
    HumanizeDateType typetime = WaitTime;
    int months;
    int weeks;
    int days;
    int hours;
    int minutes;
    int timediff;
    if (seconds < 0) {
        seconds = -1*seconds; // cambiarlo a valor positivo
    }

    // Valor devuelto pdays *****
    int pdays = seconds/ ( 60 * 60 * 24 );
    // Valor devuelto pdays *****

    timediff = seconds;
    months = timediff / ( 60 * 60 * 24 * 7 * 4);
    timediff =  timediff - months*( 60 * 60 * 24 * 7 * 4);
    weeks = timediff / ( 60 * 60 * 24 * 7);
    timediff =  timediff - weeks*( 60 * 60 * 24 * 7);
    days = timediff / ( 60 * 60 * 24 );
    timediff =  timediff - days*( 60 * 60 * 24);
    hours = timediff / ( 60 * 60 );
    timediff =  timediff - hours*( 60 * 60 );
    minutes = timediff / ( 60 );
    timediff =  timediff - minutes*( 60 );
    seconds = timediff - minutes;


    if (months > 0 ) {
        if (months == 1) {
            result =  result + tr("un mes ");
        }
        else {
            result = result + tr("%1 meses").arg(months);
        }
        if (weeks > 1) {
            result = result + tr(" y %1 semana%2").arg(weeks).arg(weeks>1?"s ":"");
        }

    }
    else {
        if (weeks > 0) {
            if (weeks == 1) {
                result =  result + tr("una semana ");
            }
            else {
                result = result + tr("%1 semanas ").arg(weeks);
            }

            if (days > 1) {
                result = result + tr("y %1 dia%2").arg(days).arg(days>1?"s ":"");
            }
        }

        else {
            if (days > 0) {
                if (days == 1) {
                    result = tr("ayer");
                }
                else if ( days == 2 ) {
                    result =  tr("anteayer");
                }
                else {
                    result = result + tr("%1 dias").arg(days);
                }
                if (hours > 1) {
                    if (result == tr("ayer")) {
                        result = tr("un dia") + tr(" y %1 hora%2").arg(hours).arg(hours>1?"s ":"");
                    }
                    else if (result == tr("anteayer")) {
                        result = tr("dos  dias") + tr(" y %1 hora%2").arg(hours).arg(hours>1?"s ":"");
                    }
                    else {
                        result = result + tr(" y %1 hora%2").arg(hours).arg(hours>1?"s ":"");
                    }

                }
            }
            else {
                if (hours > 0) {
                    if (hours == 1) {
                        result = result + tr("una hora ");
                    }
                    else {
                        result = result + tr("%1 horas ").arg(hours);
                    }
                    if (minutes > 1) {
                        result = result + tr("y %1 minuto%2").arg(minutes).arg(minutes>1?"s ":"");
                    }
                }
                else {
                    if (minutes > 0 ) {
                        if (minutes == 1) {
                            result = result + tr("un minuto ");
                        }
                        else {
                            result = result + tr("%1 minutos ").arg(minutes);
                        }
                    }
                    else {
                        if (seconds > 1) {
                            result = result + tr("%1 segundos ").arg(seconds);
                        }
                        else {
                            result = result + tr("solo un momento");
                        }
                    }
                }
            }
        }
    }
    QString contexttime;
    switch(typetime) {
    case None:
        result.replace(tr("anteayer"),tr("dos dias"));
        result.replace(tr("ayer"),tr("un dia"));
         contexttime = result;
        break;
    case SinceWhenTime:

        if (result.endsWith("ayer")) {
            contexttime = tr("%1 llego aqui").arg(result);
        }
        else {
            contexttime = tr("hace %1 llego").arg(result);
        }
        break;
    case WaitTime:
        if (result == tr("ayer")) {
            contexttime = tr("%1 en espera").arg(tr("un dia"));
        }
        else if (result == tr("anteayer")) {
            contexttime = tr("%1 en espera").arg(tr("dos dias"));

        }
        else {
            contexttime = tr("%1 en espera").arg(result);
        }

        break;
    }

    return contexttime;

}



QString SafetWorkflow::humanizeDate(int &pdays, const QString& d, const QString& formatin,
                                    QDateTime now,
                                    HumanizeDateType typetime, bool humanize) {
    QString result = "";
    QDateTime mydate;
    QString newformatin = formatin;
    if (d.length() == 16) {
           newformatin.replace("yyyy","yy");
    }
    mydate = QDateTime::fromString(d,newformatin);

    int seconds = now.secsTo(mydate) ;

    if (humanize == false) {
        return QString("%1").arg(seconds);
    }

    int months;
    int weeks;
    int days;
    int hours;
    int minutes;
    int timediff;
    if (seconds < 0) {
        seconds = -1*seconds; // cambiarlo a valor positivo
    }

    // Valor devuelto pdays *****
    pdays = seconds/ ( 60 * 60 * 24 );
    // Valor devuelto pdays *****

    timediff = seconds;
    months = timediff / ( 60 * 60 * 24 * 7 * 4);
    timediff =  timediff - months*( 60 * 60 * 24 * 7 * 4);
    weeks = timediff / ( 60 * 60 * 24 * 7);
    timediff =  timediff - weeks*( 60 * 60 * 24 * 7);
    days = timediff / ( 60 * 60 * 24 );
    timediff =  timediff - days*( 60 * 60 * 24);
    hours = timediff / ( 60 * 60 );
    timediff =  timediff - hours*( 60 * 60 );
    minutes = timediff / ( 60 );
    timediff =  timediff - minutes*( 60 );
    seconds = timediff - minutes;


    if (months > 0 ) {
        if (months == 1) {
            result =  result + tr("un mes ");
        }
        else {
            result = result + tr("%1 meses").arg(months);
        }
        if (weeks > 1) {
            result = result + tr(" y %1 semana%2").arg(weeks).arg(weeks>1?"s ":"");
        }

    }
    else {
        if (weeks > 0) {
            if (weeks == 1) {
                result =  result + tr("una semana ");
            }
            else {
                result = result + tr("%1 semanas ").arg(weeks);
            }

            if (days > 1) {
                result = result + tr("y %1 dia%2").arg(days).arg(days>1?"s ":"");
            }
        }

        else {
            if (days > 0) {
                if (days == 1) {
                    result = tr("ayer");
                }
                else if ( days == 2 ) {
                    result =  tr("anteayer");
                }
                else {
                    result = result + tr("%1 dias").arg(days);
                }
                if (hours > 1) {
                    if (result == tr("ayer")) {
                        result = tr("un dia") + tr(" y %1 hora%2").arg(hours).arg(hours>1?"s ":"");
                    }
                    else if (result == tr("anteayer")) {
                        result = tr("dos  dias") + tr(" y %1 hora%2").arg(hours).arg(hours>1?"s ":"");
                    }
                    else {
                        result = result + tr(" y %1 hora%2").arg(hours).arg(hours>1?"s ":"");
                    }

                }
            }
            else {
                if (hours > 0) {
                    if (hours == 1) {
                        result = result + tr("una hora ");
                    }
                    else {
                        result = result + tr("%1 horas ").arg(hours);
                    }
                    if (minutes > 1) {
                        result = result + tr("y %1 minuto%2").arg(minutes).arg(minutes>1?"s ":"");
                    }
                }
                else {
                    if (minutes > 0 ) {
                        if (minutes == 1) {
                            result = result + tr("un minuto ");
                        }
                        else {
                            result = result + tr("%1 minutos ").arg(minutes);
                        }
                    }
                    else {
                        if (seconds > 1) {
                            result = result + tr("%1 segundos ").arg(seconds);
                        }
                        else {
                            result = result + tr("solo un momento");
                        }
                    }
                }
            }
        }
    }
    QString contexttime;
    switch(typetime) {
    case None:
        result.replace(tr("anteayer"),tr("dos dias"));
        result.replace(tr("ayer"),tr("un dia"));
         contexttime = result;
        break;
    case SinceWhenTime:

        if (result.endsWith("ayer")) {
            contexttime = tr("%1 llego aqui").arg(result);
        }
        else {
            contexttime = tr("hace %1 llego").arg(result);
        }
        break;
    case WaitTime:
        if (result == tr("ayer")) {
            contexttime = tr("%1 en espera").arg(tr("un dia"));
        }
        else if (result == tr("anteayer")) {
            contexttime = tr("%1 en espera").arg(tr("dos dias"));

        }
        else {
            contexttime = tr("%1 en espera").arg(result);
        }

        break;
    }

    return contexttime;

}
