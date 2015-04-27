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
#include "SafetAutofilter.h"
#include "SafetTask.h"
#include "SafetConnection.h"
#include "SafetSQLParser.h"
#include "SafetWorkflow.h"


SafetAutofilter::SafetAutofilter() {

  _countoptions = 0;
  filtertask = NULL;
 _filterType = None;

}
QString SafetAutofilter::period() const {

    if (_period.isEmpty() ) {
        return "week";
    }


   return _period;

}

void SafetAutofilter::setFiltertask(SafetTask* t) {
     Q_ASSERT( filtertask == NULL );
     Q_CHECK_PTR( t );
     filtertask = t;
     SafetWorkflow *wf = qobject_cast<SafetWorkflow*>( filtertask->parent() );
     localparser.setWorkflow(wf);
}

QList<SafetTask*> SafetAutofilter::createTasks(const QString& prefix) {
     SYD << tr("....SafetAutofilter::createTasks...(1)...");
     QList<SafetTask*> mylist;
     if ( filtertask == NULL || filtertask->parent() == NULL ) {
         return mylist;
     }

     SYD << tr("....SafetAutofilter::createTasks...(2)...");
     SafetVariable *v = NULL;
     if ( filtertask->getVariables().count() > 0 ) {
          v = qobject_cast<SafetVariable*>( filtertask->getVariables().at(0) );
     }

     SafetWorkflow *wf = qobject_cast<SafetWorkflow*>( filtertask->parent() );
     Q_CHECK_PTR ( wf );
     generateFilterOptions();
     QString logvar = period();

     SYD << tr("...........SafetAutofilter::createTasks...LOGVAR:|%1|").arg(logvar);

     bool ok;
     int hcount = 0;
     int hinc = 0;
     int nweek = 1;
     int nmonth = 1;
     int nquarter = 1;
     ForDatePeriod myperiod = SafetAutofilter::Hour;
     if (logvar == "week") {
         SYD << tr (".......Autofilter::createTask...WEEK");
         myperiod = SafetAutofilter::Week;
     }
     else if (logvar == "month" ) {

                  myperiod = SafetAutofilter::Month;

     }
     else if (logvar == "quarter" ) {
         myperiod = SafetAutofilter::Quarter;
     }
     else {
        hinc = logvar.toInt(&ok);
     }

     int i = 0;

     foreach(QString s, getoptions) {

            SYD << tr("....SafetAutofilter::createTasks...(3)...s:|%1|").arg(s);

          QString title;

          if ( filterType() == DateTime ) {
              if (myperiod == SafetAutofilter::Hour) {
                  s = s.split("|").at(0).trimmed().replace(QRegExp("[\\.\\-]"),"");
              }
              if ( s.isEmpty()) {
                  s = tr("vacio");
              }

              SYD  << tr("CreateTasks: Autofiltro es tipo Fecha/Hora con opcion....*: \"%1\"")
                      .arg(s);
              title = localparser.getFields().at(0);
              if ( myperiod == SafetAutofilter::Week) {
                  title = title.remove("_")+tr("Week_%1").arg(nweek);
                  nweek++;
              }
              else if ( myperiod == SafetAutofilter::Quarter) {
                  title = title.remove("_")+tr("Quarter_%1").arg(nquarter);

                      SYD << tr("..SafetAutofilter::createTask adding QUARTER:|%1|").arg(title);
                  nquarter++;
              }

              else if ( myperiod == SafetAutofilter::Month) {
                  title = title.remove("_")+tr("Month_%1").arg(nmonth);

                      SYD << tr("..SafetAutofilter::createTask adding MONTH:|%1|").arg(title);
                  nmonth++;
              }

              else {
                  title = title.remove("_")+tr("Plus%1").arg(hcount*hinc);
                  hcount++;
              }

          }
          else {
              s = s.split("|").at(0).trimmed().replace(QRegExp("[\\.\\-]"),"");
              if ( s.isEmpty()) {
                  s = tr("vacio");
              }

              title = s;
              title.replace("á","a");
              title.replace("é","e");
              title.replace("í","i");
              title.replace("ó","o");
              title.replace("ú","u");
              title.replace("ñ","n");
              title.replace("Ñ","N");

          }

          SYD   << tr("....SafetAutofilter::createTasks()....Opcion para el autofiltro: *\"%1\" Titulo:\"%2\"")
                   .arg(s)
                   .arg(title);
          SafetTask *mytask = new SafetTask();
          mytask->setParent( qobject_cast<SafetXmlObject*> (wf) );
          Q_CHECK_PTR( mytask );
          mytask->setId(title);
          mytask->setTitle(getvaluesoptions.at(i++));
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
          myvariable->setId( "v"+title);
          SYD  << tr(".......SafetAutofilter::createTasks().... la tarea de autofiltro: \"%1\"").arg(mytask->id());
          if ( v != NULL ) {
               myvariable->setDocumentsource( v->documentsource() );
               myvariable->setTokenlink( v->tokenlink() );
          }
          mytask->addVariable( myvariable );
          SYD << tr("......SafetAutofilter::createTasks(). la variable de autofiltro: \"%2\" a la tarea \"%1\"").arg(mytask->id()).arg(myvariable->id());
          // ** Para la  conexion

          SafetConnection *myconnection = new SafetConnection();
          Q_CHECK_PTR( myconnection );
          myconnection->setParent( qobject_cast<SafetXmlObject*>(myport));
          myconnection->setSource( source() );

          myport->addConnection( myconnection );


          QList<SafetTask*> subtasks;
          if ( getsecondoptions.contains(s) && getsecondoptions[s].count()  > 0 ) {
              SYD
                      << tr("Numero de sub-opciones: \"%1\"")
                      .arg(getsecondoptions[s].count());
              if ( filterType() == DateTime) {
                  QString subprefix;
                  QRegExp rx(".+(Plus\\d+)");
                  if (title.indexOf(rx,0) >= 0 ) {
                    subprefix = rx.cap(1);
                   }
                  else {
                      SYE
                              << tr("Falla en autofiltro fecha");
                      return mylist;
                  }
                subtasks = createSubTasks(mytask,getsecondoptions[s], subprefix);
              }
              else {
                  subtasks = createSubTasks(mytask,getsecondoptions[s], title.left(5));
              }
          }



            mylist.push_back( mytask);
     }
     SYD << tr("..SafetAutofilter::createTasks()...modifyForAutofilter...(1)..mylist.count():|%1|")
            .arg(mylist.count());

     modifyForAutofilter(mylist);
     SYD << tr("..SafetAutofilter::createTasks()...**modifyForAutofilter...(2)");
     // Agregar las tareas al padre
     foreach(SafetTask *t, mylist) {
          wf->addTask( t  );
     }
     if (_subsql.isEmpty()) {
        normalizeTasks(mylist);
    }
     SYD << tr("..SafetAutofilter::createTasks()...**modifyForAutofilter...(3))");
     return mylist;
}

QList<SafetTask*> SafetAutofilter::createSubTasks(SafetTask* task, QStringList subtasks, const QString& prefix) {
    QList<SafetTask*> result;
    if ( task == NULL ) {
        return result;
    }
    if ( task->outport() == NULL ) {
        return result;
    }
    if (_subsql.isEmpty()) {
        SYE
                << tr("No se pueden crear un sub-filtro, la expresion SQL esta vacia");
        return result;
    }

    SafetWorkflow *wf = qobject_cast<SafetWorkflow*>( filtertask->parent() );
    Q_CHECK_PTR ( wf );
    SafetConnection* taskConn = NULL;
    SafetPort* taskPort = NULL;

    SafetVariable *v = NULL;
    if ( filtertask->getVariables().count() > 0 ) {
         v = qobject_cast<SafetVariable*>( filtertask->getVariables().at(0) );
    }
    if ( task->outport()->getConnectionlist().count() ==  0 ) {
        return result;
    }
    taskConn = task->outport()->getConnectionlist().at(0);
    taskPort = qobject_cast<SafetPort*>(taskConn->parent());
    taskPort->clearConn();

    Q_CHECK_PTR( taskConn );
    Q_CHECK_PTR( taskPort );



    foreach(QString nametask, subtasks){
        QString curid = prefix+tr("_")+nametask;

        SafetTask *mytask = new SafetTask();
        mytask->setParent( qobject_cast<SafetXmlObject*> (wf) );
        Q_CHECK_PTR( mytask );

        mytask->setId(curid);
        mytask->setTitle(nametask);
        mytask->setReport( subreport() );
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
        myvariable->setId( prefix+"_"+curid+"_");
        SYA  << tr("Agregando la tarea de autofiltro: \"%1\"").arg(mytask->id());

        if ( v != NULL ) {
             myvariable->setDocumentsource( v->documentsource() );
             myvariable->setTokenlink( v->tokenlink() );
        }
        mytask->addVariable( myvariable );
        SYA << tr("Agregando a la variable de autofiltro: \"%2\" la tarea \"%1\"").arg(mytask->id()).arg(myvariable->id());
        // ** Para la  conexion

        // Colocar Conexion de tarea actual (mytask)
        SafetConnection *myconnection = new SafetConnection();
        Q_CHECK_PTR( myconnection );
        //myconnection->setParent( qobject_cast<SafetXmlObject*>(myport));

        qDebug("....... taskConn...curid:%s",qPrintable( curid));
        qDebug("....... taskConn->source():%s",qPrintable( taskConn->source()));
        qDebug("....... ............option:%s",qPrintable( nametask));
        qDebug();
        myconnection->setSource( taskConn->source() );
        myconnection->setQuery("true");
        myconnection->setOptions("");
        myport->addConnection(myconnection);


        // Colocar Conexion de tarea padre
        SafetConnection *taskconnection = new SafetConnection();
        Q_CHECK_PTR( taskconnection );
        //taskconnection->setParent( qobject_cast<SafetXmlObject*>(taskPort));
        taskconnection->setSource( curid );
        taskconnection->setQuery(_subsql);

        taskconnection->setOptions(nametask);
        taskPort->addConnection(taskconnection);
        if ( taskPort->getConnectionlist().count() == 2 ) {
            qDebug(".......colocando pattern or....");
            taskPort->setPattern("or");

        }


        result.append(mytask);
        wf->addTask( mytask  );


    }
    return result;

}

void SafetAutofilter::modifyForAutofilter(QList<SafetTask*>& lt) {
     Q_CHECK_PTR ( filtertask );
     SafetPort *myport = NULL;
     QString myfilterquery = "";
     SYD << tr(".....SafetAutofilter::modifyForAutofilter...AUTOFILTER...entering...(1)...");
     foreach(SafetPort* port, filtertask->getPorts()) {
          Q_CHECK_PTR( port );
          if ( port->type() == type() ) {
               myport = port;
               if (myport->getConnectionlist().count() > 0 ) {
                    SafetConnection * myfilterconn = myport->getConnectionlist().at(0);
                    myfilterquery = myfilterconn->query();
               }
           }
     }
     SYD << tr(".....SafetAutofilter::modifyForAutofilter...filtertask:|%1|")
            .arg(filtertask->id());
     if ( myport == NULL ) {
          myport = new SafetPort();
          Q_CHECK_PTR( myport );
          filtertask->addPort( myport );
     }
     Q_CHECK_PTR( myport);
     resetOptions();
     foreach(SafetTask* task, lt ) {
          Q_CHECK_PTR( task );
          SafetConnection *myconnection = new SafetConnection();
          Q_CHECK_PTR( myconnection);
          myconnection->setSource( task->id() ) ;
          myconnection->setQuery( query() ) ;
          QString nextoption = getNextOption();
          SYD << tr(".........modifyForAutofilter...CHECKING....nextOption:|%1|")
                 .arg(nextoption);
          if ( nextoption.indexOf(Safet::SEPARATORMARK) !=-1 ) {

              QStringList mylist = nextoption.split(Safet::SEPARATORMARK,QString::SkipEmptyParts);
              SYD  << tr("modifyForAutofilter....SEPARATORMARK mylist.count():|%1|").arg(mylist.count());
              nextoption = mylist.at(0);
              if ( mylist.count() > 1) {
                  SafetTask *intertask = createInterTask(mylist, task);
                  Q_CHECK_PTR(intertask);
                  SYD  << tr(".........modifyForAutofilter...creada InterTask:|%1|").arg(intertask->id());

                  myconnection->setSource( intertask->id() ) ;

                  lt.append(intertask);
              }

          }
          myconnection->setOptions( nextoption ) ;
          myconnection->setTokenlink( tokenlink() );
          if (myport->getConnectionlist().count() < 2 ) {
               myport->setPattern("or");
          }
          SYD << tr(".....SafetAutofilter::modifyForAutofilter...addConnection for:|%1|")
                 .arg(task->id());

          myport->addConnection( myconnection );
     }
}

SafetTask* SafetAutofilter::createInterTask(const QStringList& mylist, SafetTask* parenttask) {

    SafetTask* mytask = new SafetTask();

    if (mylist.count() < 2) {
        SYE << tr("No hay suficientes opciones para crear tareas intermedias");
        return mytask;

    }

    if (parenttask == NULL) {
        SYE << tr("Tarea padre de tarea intermedia es nula");
        return mytask;
    }
    if (filtertask == NULL) {
        SYE << tr("Tarea padre filtro de tarea intermedia es nula");
        return mytask;
    }

    SafetWorkflow *wf = qobject_cast<SafetWorkflow*>( filtertask->parent() );
    Q_CHECK_PTR ( wf );

    mytask->setParent( qobject_cast<SafetXmlObject*> (wf) );

    QString myid = parenttask->id() + "_inter";

    mytask->setId(myid);
//    mytask->setTitle("intertask");
    mytask->setTitle("::safethide::");
    mytask->setReport("no");
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
    SYD  << tr("getting variable...");
    SafetVariable* v = parenttask->getVariables().at(0);
    if (v == NULL) {
        SYE << tr("Variable Tarea padre de tarea intermedia es nula");
        return mytask;
    }

    SYD  << tr("getting connection...");

    QString variableid = "v" + myid;
    myvariable->setId( variableid );
    SYD  << tr(".......SafetAutofilter::createInterTask().... taskid: \"%1\"").arg(mytask->id());
    SYD  << tr("....parenttask...SafetAutofilter::createInterTask()..CHECKING.. variableid: \"%1\"").arg(variableid);

    myvariable->setDocumentsource( v->documentsource() );
    SYD  << tr("....parenttask...SafetAutofilter::**(2) CHECK1createInterTask()...documentsource: \"%1\"")
            .arg(v->documentsource());

    myvariable->setTokenlink( v->tokenlink() );
    SYD  << tr("....parenttask...SafetAutofilter::**(2) CHECK2 createInterTask().... tokenlink: \"%1\"")
            .arg(v->tokenlink());

    mytask->addVariable( myvariable );

    SYD  << tr("....parenttask...SafetAutofilter::**(2)createInterTask().... variableid: \"%1\"").arg(variableid);


    SafetConnection *myconnection = new SafetConnection();
    Q_CHECK_PTR( myconnection );
    myconnection->setParent( qobject_cast<SafetXmlObject*>(myport));

    QString myquery = query();
    if (mylist.count() > 2) {
        QString currfield = mylist.at(2);
        myquery.replace(currfield,currfield.toUpper());
    }
    if (mylist.count() > 3) {
        QString currfield = mylist.at(3);
        myquery.replace(currfield,currfield.toUpper());
    }


    myconnection->setQuery( myquery );
    SYD  << tr("....parenttask...SafetAutofilter::**(ycreateInterTask().... variableid: \"%1\"").arg(variableid);
    SYD  << tr("....parenttask...SafetAutofilter::**(3)createInterTask().... _query: \"%1\"").arg(myquery);

    myconnection->setSource( parenttask->id() );
    myconnection->setOptions( mylist.at(1) );
    SYD  << tr("....parenttask...SafetAutofilter::**(3)createInterTask().... _option: \"%1\"").arg(mylist.at(1));

    myport->addConnection( myconnection );

    return mytask;

}

void SafetAutofilter::normalizeTasks(const QList<SafetTask*>& tasks) {

     Q_CHECK_PTR ( filtertask );

     QList<SafetPort*> ports = filtertask->getPorts();
     Q_ASSERT( ports.count() > 0 );
     SafetPort *myport = ports.at(0);
     SafetConnection *myconn = NULL;
     Q_CHECK_PTR( myport );
     QList<SafetConnection*> conns = myport->getConnectionlist();
     QString srcsource;

     foreach( SafetTask* task, tasks) {
          Q_CHECK_PTR( task );
          Q_ASSERT( task->getPorts().count() > 0 ) ;
          myport = task->getPorts().at(0);
          Q_ASSERT( myport->getConnectionlist().count() > 0 );
          myconn = myport->getConnectionlist().at(0);
          Q_CHECK_PTR( myconn );
          srcsource = myconn->source();
          foreach ( SafetConnection* conn, conns) {
                    Q_CHECK_PTR( conn );
                    if ( conn->source().trimmed().compare(srcsource.trimmed(),Qt::CaseInsensitive) == 0 ) {
                         myconn->setOptions( conn->options() );
                         myconn->setQuery( conn->query() );
                         myconn->setTokenlink(conn->tokenlink() );
                         break;
                    }
          }
     }
}




QStringList SafetAutofilter::generateFilterOptions() {

     getoptions.clear();
     getvaluesoptions.clear();
    getsecondoptions.clear();
     localparser.setStr(query().toUtf8());
     localparser.parse();

     QString table = localparser.getTablesource();
     if ( localparser.getFields().count() <= 0 ) {
         SYE
                 << tr("EL numero de campos (%2)es incorrecta para la sentencia SQL: \"%1\"")
                 .arg(query())
                 .arg(localparser.getFields().count());
         return getoptions;
     }

     QString secondfield,firstfield =  localparser.getFields().at(0);
     if ( localparser.getFields().count() > 1 ) {
         secondfield = localparser.getFields().at(1);
     }
     QString newsql = "SELECT DISTINCT";
     newsql += " ";
     newsql += firstfield;
     newsql += " ";
     if (!secondfield.isEmpty()) {
         newsql += ", ";
         newsql += secondfield;
         newsql += " ";
         _subsql = "SELECT " + secondfield + " FROM " + table;
     }
     SafetWorkflow *wf = qobject_cast<SafetWorkflow*>( filtertask->parent() );
     if (wf == NULL ) {
         SYE
                 << tr("Ocurrió un error al crear Autofiltro (Falla en la creacion de"
                       " la lista de opciones");
         return QStringList();
     }
     QString link = wf->processTokenlink(tokenlink());

     newsql += "FROM";
     newsql += " ";
     newsql += table;
     newsql += " ";
     newsql += link;
     newsql += " ";     
     newsql += localparser.getWhereClause();
     newsql += " ";
     newsql += "ORDER BY";
     newsql += " ";
     newsql += firstfield;
     newsql += ";";

     SYD << tr(".........SafetAutofilter::generateFilterOptions AUTO_REPLACE...*NEWSQL:|%1|")
            .arg(newsql);


     QSqlQuery query(SafetYAWL::currentDb);
     query.prepare(  newsql );
     bool executed = query.exec();
     if ( !executed ) {
         SYE
                 << tr("La Sentencia SQL :\" %1 \" es incorrecta. "
                       "Ejecute la sentencia en un cliente del gestor de BD, "
                       "y compruebe resultados"
                       "<br/><br/><b>Detalle:</b> %2")
                    .arg(newsql)
                    .arg(query.lastError().databaseText());
         return getoptions;
   }

     int error;
     bool genfilter = generateDateFilters(query,table, error);
     if (error == 1 ) {
         return getoptions;
     }
      if ( genfilter ) {
          SYA
                  << tr("Generando filtro de fecha/hora con la expresion SQL  \"%1\"").arg(newsql);
           _filterType = DateTime;

           SYD
                   << tr("Numero de opciones del autofiltro (Fecha/Hora) \"%1\"").arg(getoptions.count());
           if ( query.record().count() > 1 ) {
                     //_subsql = "";
                    generateDateSubFilterOptions(firstfield,secondfield,table);

           }
           return getoptions;
      }


      QString lastsecondfield,currentsecondfield;

      while (query.next()) {
          QString currentfield;          
          QString oricurrentfield = query.value(0).toString();

          currentfield = oricurrentfield;
          currentfield.replace(" ","_");
          currentfield.replace(",",".");

          getoptions.push_back( currentfield );
          getvaluesoptions.push_back( oricurrentfield ); // Verificar para descripcion

          qDebug("...getoptions...: %s",qPrintable(currentfield));
          if ( query.record().count() > 1 ) {
              if (currentsecondfield.isEmpty()) {
                  currentsecondfield = query.value(1).toString();
                  getsecondoptions[currentfield].append(currentsecondfield);

                  qDebug("...getsecondoptions...empty: %s\n",qPrintable(currentsecondfield));

              }
              else {
                  currentsecondfield = query.value(1).toString();
                  if ( currentsecondfield != lastsecondfield ) {
                      getsecondoptions[currentfield].append(currentsecondfield);
                  }

                  qDebug("...getsecondoptions...not empty: %s\n",qPrintable(currentsecondfield));
              }

              lastsecondfield = currentsecondfield;
          }


      }


      _filterType = String;
     return getoptions;

}

void SafetAutofilter::generateDateSubFilterOptions(const QString& firstfield,const QString& secondfield,const QString& table) {

    foreach(QString getoption, getoptions) {
        QString newsql = "SELECT DISTINCT";
        newsql += " ";
        newsql += secondfield;
        newsql += " ";
        newsql += "FROM";
        newsql += " ";
        newsql += table;
        newsql += " ";
        newsql += "WHERE";
        newsql += " ";
        newsql += firstfield;
        newsql += getoption;

        QSqlQuery query(SafetYAWL::currentDb);
        query.prepare(  newsql );
        SafetYAWL::streamlog <<
               SafetLog::Debug
               << tr("Ejecutando la sentencia sub-campo: \"%1\"")
               .arg(newsql);
        bool executed = query.exec();
        if ( !executed ) {
            SafetYAWL::streamlog
                    << SafetLog::Error
                    << tr("La Sentencia para sub-campo SQL :\" %1 \" es incorrecta. Ejecute la sentencia en un cliente del gestor de BD, y compruebe resultados").arg(newsql);
            return;
        }

        while( query.next()) {
            QString value = query.value(0).toString();
            SafetYAWL::streamlog <<
                   SafetLog::Debug
                   << tr("Agregando sub-campo: \"%1\"")
                   .arg(value);

            getsecondoptions[ getoption ].append(value);
        }
    }

}

bool SafetAutofilter::generateDateFilters(const QSqlQuery& query, const QString& table, int &error) {
     error = 0;
     SYD   << tr("...SafetAutofilter::generateDateFilters....(1)...");

     Q_ASSERT( query.record().count() > 0  );
     bool ok;
     QVariant::Type t = query.record().field(0).type();
     SYD   << tr("...SafetAutofilter::generateDateFilters..Tipo del registro"
                   " a realizar autofiltro: |%1| (Datetime:|%2|,UInt:|%3,Int:|%4|)")
             .arg(t)
             .arg(QVariant::DateTime)
             .arg(QVariant::UInt)
             .arg(QVariant::Int);


     if ( t != QVariant::DateTime  ) {
         if (  t == QVariant::UInt || t == QVariant::Int ) {
             QString myname = query.record().field(0).name().toLower();
              SYD << tr("generateDateFilters....NODATE...name:|%1|")
                     .arg(myname);
              // FIX: colocar una lista en la configuracion de campos no autofiltros
              if (myname == "category"  || myname =="subcategory") {
                 SYD << tr("generateDateFilters....NODATE...field");
                 return false;
             }
         }
         error = 0;
            SYD << tr("generateDateFilters....false");
            return false;

     }

     SYD << tr("SafetAutofilter::generateDateFilters....**true");
     getoptions.clear();
     QString logvar = period();

     Q_ASSERT(query.lastQuery().length() > 1 );
     QString newsql = query.lastQuery();
     SYD
             << tr("Filtro de Fecha/Hora colocado a \"%1\" horas")
             .arg(logvar);

     if ( newsql.endsWith(";") ) {
          newsql.chop(1);
     }
     if ( !newsql.contains("ORDER BY")) {
        newsql += " ORDER BY " + query.record().fieldName(0);
    }

     QSqlQuery orderquery(SafetYAWL::currentDb);

     SYD
             << tr("Se va a ejecutar la sentencia SQL: \"%1\" (ordenada, tipo fecha)").arg(newsql);
     orderquery.prepare(  newsql );
     bool executed = orderquery.exec();
     if ( !executed ) {
         SYE
                 <<
      tr("No se puede generar el autofiltro porque la sentencia SQL \"%1\"  es incorrecta").arg(newsql);
         error = 1;
         return false;
    }
     executed =  orderquery.next();
     if ( !executed) {
         SYE
                 <<
         tr("No se puede generar el autofiltro porque la expresion \"%1\"  no contiene registros").arg(newsql);
         error = 1;
         return false;

     }
     QString fieldname = orderquery.record().fieldName(0);
     QDateTime iteDateFirst;
     if ( t == QVariant::DateTime  ) {
          iteDateFirst = orderquery.value(0).toDateTime();
     } else if ( t == QVariant::Int  ) {
          iteDateFirst = QDateTime::fromTime_t(orderquery.value(0).toUInt(&ok));
          if ( !ok ){
               SYE << tr("Error al convertir la fecha \"%1\"  del campo \"%2\"").arg(orderquery.value(0).toString(),fieldname);
          }
     }
     SYD << tr("Primera fecha para el autofiltro \"%1\": \"%2\"").arg(fieldname)
               .arg(iteDateFirst.toString() );
//     qDebug("..first: %s", qPrintable(orderquery.value(0).toDateTime().toString()));
     executed =  orderquery.last();
     if ( !executed ) {
        SYE
                 <<tr("No se puede generar el autofiltro porque fallo la busqueda del ultimo registro de la \"%1\"").arg(table);
         error = 1;
         return false;
     }
     QDateTime iteDateLast;
     if ( t == QVariant::DateTime  ) {
          iteDateLast = orderquery.value(0).toDateTime();
     } else if ( t == QVariant::Int  ) {
          iteDateLast = QDateTime::fromTime_t(orderquery.value(0).toUInt(&ok));
          if ( !ok ){
               SYE << tr("Error al convertir la fecha \"%1\"  del campo \"%2\"").arg(orderquery.value(0).toString(),fieldname);
          }

     }
     SYD << tr("Ultima fecha para el autofiltro  \"%1\": \"%2\"").arg(fieldname)
               .arg(iteDateLast.toString() );
     QDateTime iteDate, nextIteDate;
     iteDate = iteDateFirst;
     QString dateformat = "yyyy-MM-dd hh:mm:ss";
     QString valuedateformat = "ddd yy/MM/dd hh:mm";
     int i = 0;
     ForDatePeriod myperiod = Hour;

     if (logvar == "month") {
         myperiod = SafetAutofilter::Month;
     }
     else if (logvar == "quarter") {
         myperiod = SafetAutofilter::Quarter;
     }
     else if (logvar == "week") {
         myperiod = SafetAutofilter::Week;
     }


     int forday1 = 0;
     int forday7 = 0;
     int dayofweek = 1;


     bool initweek = false;


     int nweek = 1;
     int nmonth = 1;
     int nquarter =  1;
     int ncount = 0;
     QTime starttime(0,0,0);
     QTime endtime(23,59,59);
     while ( true ) {
           QString newfilter;
           bool ok;
            switch(myperiod) {
                case SafetAutofilter::Week:

                dayofweek = iteDate.date().dayOfWeek();



                SYD << tr("........SafetAutofilter::generateDateFilters...iteWeek dayofweek:|%1|")
                       .arg(dayofweek);

                if (initweek == false ) {
                   forday1 = dayofweek*(3600*24)*(-1);
                   forday7 = (7-dayofweek+1)*(3600*24);
                    SYD << tr("........SafetAutofilter::generateDateFilters...forday7:|%1|")
                           .arg(forday7);


                   iteDate =  iteDate.addSecs(forday1);

                   SYD << tr("........SafetAutofilter::generateDateFilters...*****forday1:|%1|")
                          .arg(forday1);
                   SYD << tr("\n\n*********\n....................SafetAutofilter::generateDateFilters...newdate1:|%1|")
                          .arg(iteDate.toString(Safet::DateFormat));
                   SYD << tr("........SafetAutofilter::generateDateFilters...forday1:|%1|")
                          .arg(forday1);

                    initweek = true;
                    nextIteDate = iteDateFirst.addSecs(forday7);
                    nextIteDate  = nextIteDate.addDays(-1);
                }
                else {
                    //int daysdiscount = 1*(24*3600);
                    //forday7 = 7*(24*3600)-daysdiscount;
                    //nextIteDate = iteDate.addSecs(forday7);
                    nextIteDate = iteDate.addDays(6);

                }
                iteDate.setTime(starttime);
                nextIteDate.setTime(endtime);

                SYD << tr("........SafetAutofilter::generateDateFilters...***date1:|%1|")
                       .arg(iteDate.toString(Safet::DateFormat));

                SYD << tr("........SafetAutofilter::generateDateFilters...***date2:|%1|")
                       .arg(nextIteDate.toString(Safet::DateFormat));


                break;
            case SafetAutofilter::Quarter:
            {
                QDate myinitdate;
                QDate myenddate;
                QTime myinittime;
                QTime myendtime;

                int currmonth = iteDate.date().month();
                SYD << tr("........SafetAutofilter::generateDateFilters...iteDate.date().month():|%1|")
                       .arg(currmonth);



                int startm = currmonth-1;
                SYD << tr("........SafetAutofilter::generateDateFilters...startm...(1):|%1|")
                       .arg(startm);

                startm = startm/3;
                SYD << tr("........SafetAutofilter::generateDateFilters...startm..(2):|%1|")
                       .arg(startm);


                startm = startm*3+1;
                SYD << tr("........SafetAutofilter::generateDateFilters...startm..(4):|%1|")
                       .arg(startm);

                int endm = startm + 2;
                SYD << tr("........SafetAutofilter::generateDateFilters...endm:|%1|")
                       .arg(endm);

                myinitdate.setDate(iteDate.date().year(),startm,1);
                QDate enddate;
                enddate.setDate(iteDate.date().year(),endm,1);

                myenddate.setDate(iteDate.date().year(),endm,enddate.daysInMonth());

                myinittime.setHMS(0,0,0);
                iteDate.setDate(myinitdate);
                iteDate.setTime(myinittime);

                myendtime.setHMS(23,59,59);
                nextIteDate.setDate(myenddate);
                nextIteDate.setTime(myendtime);

                SYD << tr("........SafetAutofilter::generateDateFilters...QUARTER....date1:|%1|")
                       .arg(iteDate.toString(Safet::DateFormat));

                SYD << tr("........SafetAutofilter::generateDateFilters...QUARTER...date2:|%1|")
                       .arg(nextIteDate.toString(Safet::DateFormat));
            }

                break;

            case SafetAutofilter::Month:
            {
                QDate myinitdate;
                QDate myenddate;
                QTime myinittime;
                QTime myendtime;

                myinitdate.setDate(iteDate.date().year(),iteDate.date().month(),1);
                myenddate.setDate(iteDate.date().year(),iteDate.date().month(),iteDate.date().daysInMonth());

                myinittime.setHMS(0,0,0);
                iteDate.setDate(myinitdate);
                iteDate.setTime(myinittime);

                myendtime.setHMS(23,59,59);
                nextIteDate.setDate(myenddate);
                nextIteDate.setTime(myendtime);

                SYD << tr("........SafetAutofilter::generateDateFilters...MONTH....date1:|%1|")
                       .arg(iteDate.toString(Safet::DateFormat));

                SYD << tr("........SafetAutofilter::generateDateFilters...MONTH.date2:|%1|")
                       .arg(nextIteDate.toString(Safet::DateFormat));
            }

                break;

            default:
                nextIteDate = iteDate.addSecs(logvar.toInt(&ok)*3600);
            }


            if ( t == QVariant::DateTime  ) {
                newfilter = QString(">='%1' AND %3<'%2'")
                            .arg(iteDate.toString(dateformat))
                            .arg(nextIteDate.toString(dateformat))
                            .arg(table+"."+fieldname);
           }
            else if ( t == QVariant::Int  ) {
                newfilter = QString(">%1%3<%2%3%4%3%5")
                            .arg(iteDate.toTime_t())
                            .arg(nextIteDate.toTime_t())
                            .arg(Safet::SEPARATORMARK)
                            .arg(table+"."+fieldname)
                            .arg(fieldname);

//                newfilter = QString(">%1")
//                            .arg(nextIteDate.toTime_t());
               }

           SYD << tr("......Proxima Fecha del Filtro: \"%1\"").arg(nextIteDate.toString());
           SYD << tr(".....Agregando CHECKING...filtro de fecha: \"%1\"").arg(newfilter);
           QString newvalue = "";
           if (myperiod ==  SafetAutofilter::Week) {
                //newvalue = tr("Week%1").arg(nweek);

               newvalue = QString("%1 a %2").arg(iteDate.toString(Safet::DateOnlyFormat))
                       .arg(nextIteDate.toString(Safet::DateOnlyFormat));

                nweek++;
                getvaluesoptions.append(newvalue);
                getoptions.append(newfilter);

           }
           else if (myperiod ==  SafetAutofilter::Month) {

               newvalue = QString("%1 a %2").arg(iteDate.toString(Safet::DateOnlyFormat))
                       .arg(nextIteDate.toString(Safet::DateOnlyFormat));

                nmonth++;
                getvaluesoptions.append(newvalue);
                getoptions.append(newfilter);
                nextIteDate = nextIteDate.addDays(1);
                SYD << tr(".....Agregando CHECKING...NEXTDAY: \"%1\"").arg(nextIteDate.toString(valuedateformat));


           }
           else if (myperiod ==  SafetAutofilter::Quarter) {

               newvalue = QString("%1 a %2").arg(iteDate.toString(Safet::DateOnlyFormat))
                       .arg(nextIteDate.toString(Safet::DateOnlyFormat));

                nquarter++;
                getvaluesoptions.append(newvalue);
                getoptions.append(newfilter);
                SYD << tr(".....Agregando CHECKING...QUARTER...newfilter: \"%1\"").arg(newfilter);
                nextIteDate = nextIteDate.addDays(1);
                SYD << tr(".....Agregando CHECKING...QUARTER...NEXTDAY: \"%1\"").arg(nextIteDate.toString(valuedateformat));


           }

           else {

              newvalue = QString("%1").arg(iteDate.toString(valuedateformat));
              getvaluesoptions.append(newfilter);
              getoptions.append(QString("Period%1").arg(ncount));
           }
           ncount++;



           if (nextIteDate >= iteDateLast ) {
               break;
           }
           iteDate = nextIteDate;
           SYD << tr(".....Agregando CHECKING...WEEK...NEW....ITEDATE (1): \"%1\"").arg(iteDate.toString(Safet::DateFormat));
           if (myperiod == SafetAutofilter::Week ) {
               SYD << tr(".....adding CHECKING...WEEK...**");
               iteDate = iteDate.addDays(1);

           }
           SYD << tr(".....Agregando CHECKING...WEEK...NEW....ITEDATE (2): \"%1\"\n").arg(iteDate.toString(Safet::DateFormat));
     }


     SYD << tr("..........generateDateFilter....RETURNING...(1)...");
     return true;
}

QString SafetAutofilter::getNextOption() {
    //QString logvar = SafetYAWL::getConf()["Autofilter/datetime.period"];
      if (filterType() == DateTime || filterType() == Date ) {
            if ( _countoptions < getoptions.count() ) {
                 return getoptions.at( _countoptions++ ) ;
            }
            _countoptions = 0;
            return getoptions.at( _countoptions++ ) ;
        }
        else {
          if ( _countoptions < getvaluesoptions.count() ) {
               return getvaluesoptions.at( _countoptions++ );
          }
          _countoptions = 0;
          return getvaluesoptions.at( _countoptions++ )  ;
        }
 }

