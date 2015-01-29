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

#include "SafetBinaryRepo.h"
#include "SafetYAWL.h"
#include "SafetWorkflow.h"

SafetBinaryRepo::SafetBinaryRepo(const QString& name) {


    _templatecreate = "create table %1 (%2);";
    _templateinsert = "insert into %1 (%2) values (%3);";
    _templatedrop = "delete from %1 where %2";
    _templatedeleteone = "delete from %1 where %2;";
    _templatedeleteall = "delete from %1 where 1;";
    _templatesearch = "select * from %1 where %2";
    _templatesearchall = "select * from %1";



    _namedata = name;


    QString path = SafetYAWL::pathconf+"/stats";
    if ( !QFile::exists(path)) {
        SYD << tr("Repositorio Binario (SafetBinaryRepo): No existe la ruta \"%1\" para guardar el"
                  " archivo.")
               .arg(path);
        return;
    }
    path +=  "/"+_namedata;
    _currpathname = path;
    _fileexists = QFile::exists(_currpathname);
    SYD << tr("....*****SafetBinaryRepo::SafetBinaryRepo...existe \"%1\" el archivo ...\"%2\"")
           .arg(_fileexists)
           .arg(_currpathname);

}

bool SafetBinaryRepo::open() {
    QString path = SafetYAWL::pathconf+"/stats";
    if ( !QFile::exists(path)) {
        SYD << tr("Repositorio Binario (SafetBinaryRepo): No existe la ruta \"%1\" para guardar el"
                  " archivo.")
               .arg(path);
        return false;
    }
    path +=  "/"+_namedata;
    _currpathname = path;

    _binaryDb = QSqlDatabase::addDatabase( "QSQLITE" );
    _binaryDb.setDatabaseName(_currpathname);

    _binaryDb.open();
    if (!_binaryDb.isOpen()) {
        SYE << tr("Repositorio Binario (SafetBinaryRepo): No fue posible abrir la base de datos: \"%1\"-\"%2\"")
               .arg(_binaryDb.lastError().driverText())
               .arg(_binaryDb.lastError().databaseText());
        return false;
    }



    return true;

}


int SafetBinaryRepo::deleteRecords(const QString &nametable, const QString &whereclause) {
    if (!_binaryDb.isOpen()) {
        SYW << tr("Repositorio Binario (deleteRecords): la base de datos no est� abierta");
        return 0;
    }
    QString strsql;
    QSqlQuery myquery(_binaryDb);

    bool executed;
    strsql = _templatedeleteone.arg(nametable).arg(whereclause);
    executed = myquery.exec(strsql);
    if ( !executed ) {
        SYW << tr("...Repositorio Binario (deleteRecords): No se puede ejecutar la sentencia: \"%1\"")
               .arg(strsql);
        return 0;

    }
    SYD << tr(" Repositorio Binario(deleteRecords. Ejecutada la sentencia:|%1|")
           .arg(strsql);

    SYD << tr("....SafetBinaryRepo::deleteRecords...ok!");
    return 1;


}


bool  SafetBinaryRepo::createTable(const QString &nametable, const QString &fields) {
    if (!_binaryDb.isOpen()) {
        SYE << tr("Repositorio Binario (createTable): la base de datos no est� abierta");
        return false;
    }
    QString strsql;
    QSqlQuery myquery(_binaryDb);

    bool executed;
    strsql = _templatecreate.arg(nametable).arg(fields);
    executed = myquery.exec(strsql);
    if ( !executed ) {
        SYE << tr("...Repositorio Binario (createTable): No se puede ejecutar la sentencia: \"%1\"")
               .arg(strsql);
        //_binaryDb.close();
        return false;
   }

    SYD << tr("....SafetBinaryRepo::createTable...ok!");

}


void SafetBinaryRepo::append(const QString &nametable, const QString &fields, const QString& values)
{

    if (!_binaryDb.isOpen()) {
        SYW << tr("Repositorio Binario (append): la base de datos no est� abierta");
        return;
    }
    QString strsql;
    QSqlQuery myquery(_binaryDb);

    strsql = _templateinsert.arg(nametable).arg(fields).arg(values);
    bool executed = myquery.exec(strsql);
    if ( !executed ) {
        SYW << tr("...Repositorio Binario (append): No se puede ejecutar la sentencia: \"%1\"")
               .arg(strsql);
        //_binaryDb.close();
        return;
   }
   SYD << tr("...SafetBinaryRepo::append..ok!");

}


QPair<QString,QString> SafetBinaryRepo::dataJSON(const QString &nametable,
                                                 const QString& filter,
                                                 const QStringList& todatefields,
                                                 const QStringList& tospandatefields
                                                 ) {
    QPair<QString,QString> result;
    result.first = "safetlist =[\n";
    result.second = "safetcolumns = [\n";
    if (!_binaryDb.isOpen()) {
        SYE << tr("Repositorio Binario (search): la base de datos no est� abierta");
        return result;
    }
    QString strsql;

    QSqlQuery myquery(_binaryDb);

    if (filter.isEmpty()) {
        strsql = _templatesearchall.arg(nametable);
    }
    else {
        strsql = _templatesearch.arg(nametable).arg(filter);
    }


    bool executed = myquery.exec(strsql);

    if ( !executed ) {
        SYW << tr("...Repositorio Binario (JSON):"
                  "No se puede ejecutar la sentencia: \"%1\"")
               .arg(strsql);
        //_binaryDb.close();
        return result;
   }
   QSqlRecord rec = myquery.record();
   QDateTime mynow = QDateTime::currentDateTime();
   while(myquery.next() ) {
       result.first += "\n{";
       for (int i= 0; i< rec.count(); i++){
           QString fieldname = rec.fieldName(i).trimmed();
           QString value = myquery.value(i).toString().trimmed();
           if (todatefields.contains(fieldname)) {
               bool ok;
               uint seconds = value.toUInt(&ok);
               value = QDateTime::fromTime_t(seconds).toString("dd/MM/yyyy hh:mmap");
           }
           else if (tospandatefields.contains(fieldname) ) {
               bool ok;
               int days = 0;
               int seconds = value.toUInt(&ok);
               QString datestr = QDateTime::fromTime_t(mynow.toTime_t()-seconds).toString("dd/MM/yyyy hh:mmap");
               value = SafetWorkflow::humanizeDate(days,datestr,"dd/MM/yyyy hh:mmap",
                                                   mynow, SafetWorkflow::WaitTime);
           }

            result.first += QString("%1:\"%2\",")
                    .arg(fieldname)
                    .arg(value);
       }
       result.first.chop(1);
       result.first += "},\n";

   }
   result.first.chop(2);
   result.first += "\n];\n";



   // Nombre de campos
   for (int i= 0; i< rec.count(); i++){
        result.second += QString(
                "{ key:\"%1\",label:\"%1\","
                "width:%2,resizeable:true,sortable:true},\n")
                .arg(rec.fieldName(i).trimmed())
                .arg(SafetYAWL::getScriptLen(rec.field(i)));
   }

   result.second.chop(2);
   result.second += "\n];\n";


    return result;

}


QString SafetBinaryRepo::search(const QString &nametable, const QString &key) {

    QString result;
    if (!_binaryDb.isOpen()) {
        SYE << tr("Repositorio Binario (search): la base de datos no est� abierta");
        return result;
    }
    QString strsql;
    QSqlQuery myquery(_binaryDb);

    if (key.isEmpty()) {
        strsql = _templatesearchall.arg(nametable);
    }
    else {
        strsql = _templatesearch.arg(nametable).arg(key);
    }

    bool executed = myquery.exec(strsql);

    if ( !executed ) {
        SYW << tr("...Repositorio Binario (search):"
                  "No se puede ejecutar la sentencia: \"%1\"")
               .arg(strsql);
        //_binaryDb.close();
        return result;
   }
   QSqlRecord rec = myquery.record();
   while(myquery.next() ) {
       for (int i= 0; i< rec.count(); i++){
            result += myquery.value(i).toString().trimmed();
            result += ",";
       }
       result.chop(1);
       result += "\n";

   }
   result.chop(1);

   return result;

}


QString SafetBinaryRepo::calculateAverage(const QString& nametable, const QString& key,
                                          int fromdate,
                                          int todate) {

    QString result;
    SYD << tr("...SafetBinaryRepo::calculateAverage...fromdate:|%1|...todate:|%2|")
           .arg(fromdate)
           .arg(todate);
    if (!_binaryDb.isOpen()) {
        SYE << tr("Repositorio Binario (search): la base de datos no est� abierta");
        return result;
    }
    QString strsql;
    QSqlQuery myquery(_binaryDb);

    if (fromdate == -1 && todate == -1) {
        strsql = QString("select secondswait from %1 where wftask='%2';").arg(nametable).arg(key);
    }
    else if ( fromdate != -1 && todate == -1) {
        strsql = QString("select secondswait from %1 where wftask='%2' and timecurr>=%3;")
                .arg(nametable)
                .arg(key)
                .arg(fromdate);
    }
    else if ( fromdate == -1 && todate != -1) {
        strsql = QString("select secondswait from %1 where wftask='%2' and timecurr<=%3;")
                .arg(nametable)
                .arg(key)
                .arg(todate);
    }
    else  {
        strsql = QString("select secondswait from %1 where wftask='%2' and timecurr>=%3"
                         " and timecurr<=%4;")
                .arg(nametable)
                .arg(key)
                .arg(fromdate)
                .arg(todate);
    }


    SYD << tr(".......SafetBinaryRepo::calculateAverage....strsql:|%1|")
           .arg(strsql);
    bool executed = myquery.exec(strsql);

    if ( !executed ) {
        SYW << tr("...Repositorio Binario (search):"
                  "No se puede ejecutar la sentencia: \"%1\"")
               .arg(strsql);
        //_binaryDb.close();
        return result;
   }
   int avg = 0, avcount = 0;
   bool ok;
   while(myquery.next() ) {
        avg += myquery.value(0).toInt(&ok);
        avcount++;

   }

   if (avcount == 0 ) {
        return QString();
   }

   avg = avg / avcount;
   SYD << tr(".......SafetBinaryRepo::calculateAverage....avg (2):|%1|")
          .arg(avg);
   int tnow = QDateTime::currentDateTime().toTime_t();
   int tavg = tnow-avg;
   QDateTime tdavg = QDateTime::fromTime_t(tavg);
   QDateTime tdnow = QDateTime::fromTime_t(tnow);


   int days = 0;
   result = SafetWorkflow::humanizeDate(days,tdavg.toString("dd/MM/yyyy hh:mmap")
                                        ,"dd/MM/yyyy hh:mmap",tdnow);

   return result;


}
int deleteOne(const QString &nametable, const QString &whereclause) {


}

bool SafetBinaryRepo::modify(const QString &key) {

}

