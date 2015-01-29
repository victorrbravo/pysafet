/*
* SAFET Sistema Automatizado para la Firma Electrónica y Estampado de Tiempo
* Copyright (C) 2008 VÃ­ctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigación en TecnologÃ­as Libres
*
* Este programa es software libre; Usted puede usarlo bajo los términos de la licencia de
* software GPL versión 2.0 de la Free Software Foundation.
*
* Este programa se distribuye con la esperanza de que sea útil, pero SI NINGUNA GARANTÍA;
* tampoco las implí­citas garantí­as de MERCANTILIDAD o ADECUACIÃ“N A UN PROPÓSITO PARTICULAR.
* Consulte la licencia GPL para más detalles. Usted debe recibir una copia de la GPL junto
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5º Piso, Boston, MA 02110-1301, USA.
*
*/

#include "SafetYAWL.h"
#include "SafetRecursiveFilter.h"
#include "SafetTask.h"
#include "SafetConnection.h"
#include "SafetSQLParser.h"
#include "SafetWorkflow.h"


SafetRecursivefilter::SafetRecursivefilter() {

  _countoptions = 0;
  filtertask = NULL;
 _filterType = None;
 wasconnsdeleted = false;
 _onlyfathers = "no";

}


void SafetRecursivefilter::setFiltertask(SafetTask* t) {
     Q_ASSERT( filtertask == NULL );
     Q_CHECK_PTR( t );
     filtertask = t;
     SafetWorkflow *wf = qobject_cast<SafetWorkflow*>( filtertask->parent() );
     localparser.setWorkflow(wf);
}


QList<SafetTask*> SafetRecursivefilter::createTasks(const QString& prefix) {

     SafetWorkflow *wf = qobject_cast<SafetWorkflow*>( filtertask->parent() );
     Q_CHECK_PTR ( wf );


     qDebug("....onlyfathers: %s", qPrintable(onlyfathers()));
     generateRecursiveTask();
//     SafetYAWL::streamlog
//             << SafetLog::Debug
//             << tr("Enlace de tokenlink generado: \"%1\"")
//             .arg(link);
     foreach(SafetTask *t, listnewtasks) {
          wf->addTask(t);
     }     return listnewtasks;
}

void SafetRecursivefilter::linkTasks(SafetTask* tasksource, SafetTask* tasktarget,
                                     const QString& sql, const QString& opts) {

     Q_CHECK_PTR ( filtertask );

     SafetTask *parentTask = NULL;

     if ( tasksource == NULL) {
         parentTask = filtertask;
     }
     else {
         parentTask = tasksource;
     }
     QList<SafetPort*> ports = parentTask->getPorts();
     if ( ports.count() ==  0 ) {
         SafetYAWL::streamlog
                 << SafetLog::Error
                 << tr("El número de puertos de la tarea con id\"%1\" es igual a cero(0)")
                 .arg(parentTask->id());
            return;

     }
     SafetPort *myport = ports.at(0);
//     if ( tasksource == NULL ) {
//         myport->setPattern("o");
//     }

     // ** Para la  conexion
    SafetConnection *myconnection = new SafetConnection();
    Q_CHECK_PTR( myconnection );
    myconnection->setQuery( sql );
    myconnection->setOptions( opts );
    myconnection->setTokenlink(tokenlink());
    myconnection->setParent( qobject_cast<SafetXmlObject*>(myport));

    SafetYAWL::streamlog
            << SafetLog::Debug
            << tr("FiltroRecursivo(RecursiveFilter): Enlazando Tarea con id:|%2| a Tarea con id: |%3| con consulta: \"%1\""
                  " y opciones: \"%4\"")
            .arg(sql)
            .arg(tasksource==NULL?"NULL":tasksource->id())
            .arg(tasktarget==NULL?"NULL":tasktarget->id())
            .arg(opts);
    if ( tasktarget == NULL ) {
         myconnection->setSource( source() );
     }
     else {
         myconnection->setSource( tasktarget->id() );
     }


     if ( !wasconnsdeleted ) {
         myport->clearConn();
         wasconnsdeleted = true;
     }

     if ( myport->getConnectionlist().count() > 0 ) {
         myport->setPattern("or");
     }
    myport->addConnection( myconnection);
    SafetYAWL::streamlog
            << SafetLog::Debug
            << tr("FiltroRecursivo(Recursivefilter): Agregando conexion a la tarea\"%1\"" )
            .arg(parentTask->id());

}

SafetTask* SafetRecursivefilter::createTask(const QString& title, const QString& namefield,
                                            const QString& info) {
    Q_CHECK_PTR ( filtertask );
    Q_CHECK_PTR ( filtertask->parent() );

    SafetVariable *v = NULL;
    if ( filtertask->getVariables().count() > 0 ) {
        v = qobject_cast<SafetVariable*>( filtertask->getVariables().at(0) );
    }

    SafetWorkflow *wf = qobject_cast<SafetWorkflow*>( filtertask->parent() );
    Q_CHECK_PTR ( wf );
    SafetTask *mytask = new SafetTask();
    mytask->setParent( qobject_cast<SafetXmlObject*> (wf) );
    Q_CHECK_PTR( mytask );    
    mytask->setId(namefield+"_"+title);
    QString newinfo = info;
    newinfo.replace("á","a");
    newinfo.replace("é","e");
    newinfo.replace("í","i");
    newinfo.replace("ó","o");
    newinfo.replace("ú","u");
    newinfo.replace("ñ","n");
    mytask->setTitle(newinfo);
    mytask->setReport( report() );
    //** Para el puerto
    SafetPort* myport = new SafetPort();
    Q_CHECK_PTR( myport );
    myport->setSide("forward");
    myport->setType("split");
    myport->setQuery("");
    myport->setPattern("none");

    mytask->addPort( myport );
    //** Para la variable
    SafetVariable* myvariable = new SafetVariable();
    Q_CHECK_PTR( myvariable );
    myvariable->setId("_"+title+"_");
    SafetYAWL::streamlog
            << SafetLog::Action
            << tr("Agregando la tarea de filtro recursivo: \"%1\"").arg(mytask->id());
    if ( v != NULL ) {
        myvariable->setDocumentsource( v->documentsource() );
        myvariable->setTokenlink( v->tokenlink() );
    }
    mytask->addVariable( myvariable );
    SafetYAWL::streamlog << SafetLog::Debug
            << tr("Agregando a la variable de filtro recursivo: \"%2\" la tarea \"%1\"").arg(mytask->id()).arg(myvariable->id());

    listnewtasks.push_back( mytask);
    return mytask;

}

QPair<QString,QString> SafetRecursivefilter::getFieldsFromQuery(const QString& q, QString& basicsql) {
    QPair<QString,QString> result;
    localparser.setStr(q.toUtf8());
    if (!localparser.parse()) {
        return result;
    }

    Q_ASSERT( localparser.getFields().count() > 0 );

    SafetWorkflow *wf = qobject_cast<SafetWorkflow*>( filtertask->parent() );

    QString whereexp = localparser.getWhereClause();
//    qDebug("...***QPair<QString,QString> SafetRecursivefilter::getFieldsFromQuery...."
//           "wherec: %s", qPrintable(whereexp));
    QString basicexp = localparser.dropWhereClauses(false);

    QString link = wf->processTokenlink(tokenlink());
    result.first = localparser.getFields().at(0);
    result.second = q;
    whereexp = whereexp.replace("{{"+result.first+"}}","%1");
    result.second = basicexp+" "+link+" "+ whereexp;
    basicsql = "SELECT "+localparser.getFields().at(0)+ " FROM " + localparser.getTablesource()
               /*+ " " + link*/;

    return result;

}




void SafetRecursivefilter::generateRecursiveTask() {


     // Enlace al flujo de trabajo

     QString basicsql; // Para el campo query de los filtro de las tareas generadas
     QPair<QString,QString> fields = getFieldsFromQuery(filter(), basicsql);
     QString oldsql, newsql = fields.second.arg(initial());
     // qDebug("....query: |%s|", qPrintable(newsql ));
     SafetTask* newtask = NULL, *parenttask = NULL;
     QString namefield = fields.first;
     QStack< QPair<QString, QPair<SafetTask*,SafetTask*> > > mystack;
     parenttask  = newtask;
     QPair<QString, QPair<SafetTask*,SafetTask*> >  curvalue;


    while ( true ) {

        QSqlQuery localquery(SafetYAWL::currentDb);

        localquery.prepare(  newsql);
        bool executed = localquery.exec();

        if ( !executed ) {
            SafetYAWL::streamlog
                    << SafetLog::Error
                    << tr("La Sentencia SQL :\n\" %1 \" \nes incorrecta."
                          "Ejecute la sentencia en un cliente del gestor de BD,"
                          "y compruebe resultados"
                       "<br/><br/><b>Detalle:</b> %2")
                    .arg(newsql)
                    .arg(localquery.lastError().databaseText());

            return;

        }


        oldsql = newsql;

        if (localquery.size() <= 0 ) {
            if ( !mystack.isEmpty()) {
                linkTasks(parenttask,NULL,"true","");
                curvalue  = mystack.pop();
                QString d = curvalue.first.split(";").at(0);
                newsql = fields.second.arg(d);
                parenttask = curvalue.second.first;                
                QString info = tr("info");
                if (curvalue.first.split(";").count() > 1 ) {
                    info = curvalue.first.split(";").at(1);
                }
                newtask = createTask(d,namefield,info);
                //basicsql = query();
                //d = options();
//                qDebug("/***..............................Query: %s", qPrintable(query()));
//                qDebug("/***..............................Options: %s", qPrintable(options()));
                linkTasks(parenttask,newtask, basicsql,d);
                parenttask = newtask;
                continue;
            }
            else {
                linkTasks(parenttask,NULL,"true","");
                break;
            }


        }


        int howchildren = 0;
        while( localquery.next() ) {
            QString d = localquery.record().value(namefield).toString();
            QString info = tr("info1");
            if ( localquery.record().count() > 1) {
                info = localquery.record().value(1).toString();
                info.replace(" ","_");
            }
            if (onlyfathers().compare("yes") == 0 ) {
                QString mysql = fields.second.arg(d);
//                qDebug("**....SafetRecursivefilter...mysql: %s",
//                       qPrintable(fields.second.arg(d)));
                if (!hasChildren(mysql) ) {
                   continue;
                }
            }
            howchildren++;
            newtask = createTask(d,namefield,info);
//            basicsql = query();
//            d = options();
//            qDebug("/***..............................Query: %s", qPrintable(query()));
//            qDebug("/***..............................Options: %s", qPrintable(options()));

            linkTasks(parenttask,newtask, basicsql,d);
            curvalue.first = d +";"+info;
            curvalue.second.first = parenttask;
            curvalue.second.second = newtask;
            mystack.push(curvalue);
        }
        if ( howchildren == 0 ) {
            linkTasks(parenttask,NULL,"true","");
        }

        if ( mystack.isEmpty() ) {
            break;
        }
        curvalue = mystack.pop();
        newsql = fields.second.arg(curvalue.first.split(";").at(0));
        parenttask = curvalue.second.second;
        newtask= curvalue.second.first;

    }
     return;

}

bool SafetRecursivefilter::hasChildren(const QString& sql) {
    QSqlQuery localquery(SafetYAWL::currentDb);
    localquery.prepare(  sql);
    bool executed = localquery.exec();

    if ( !executed ) {
        return false;

    }

    if (localquery.size() <= 0 ) {
        return false;
    }
    return true;


}

