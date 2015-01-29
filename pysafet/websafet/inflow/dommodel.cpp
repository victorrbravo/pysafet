/****************************************************************************
**
** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

//#include <QtGui>
#include <QtXml>
#include "mainwindow.h"
#include "cmdwidget.h"
#include "getfilewidget.h"
#include "combowidget.h"
#include "domitem.h"
#include "dommodel.h"
//#include "textedit.h"
#include "texteditwidget.h"
#include "suggestwidget.h"
#include "datetimewidget.h"
#include "outputwidget.h"
#include "numberwidget.h"
//#include "listwidget.h"


DomModel::DomModel(const QString& fileName, QObject *parent)
{
       docok = false;
       domDocument = NULL;	
       domModelFromFile(fileName);
       _cpparent = parent;
}

DomModel::~DomModel()
{
  qDebug("...~DomModel....**");
//  if (domDocument) {
//      delete domDocument;
//  }
//  if (SafetYAWL::listDynWidget != NULL  ) {
//      for(int i =0; i < SafetYAWL::listDynWidget->count();i++) {
//          if ( SafetYAWL::listDynWidget->at(i) ){
//              delete SafetYAWL::listDynWidget->at(i);
//          }
//      }
//      delete SafetYAWL::listDynWidget;
// }

//SafetYAWL::listDynWidget = NULL;


}

void DomModel::reset() {

     QStringList mylist;
//     setStringList(mylist);

}


QStringList DomModel::getCommands(bool includedesc, bool ascending) {

    _currentactions.clear();
    Q_CHECK_PTR( domDocument );
    if ( docok ) {
        QDomNode node = domDocument->lastChild();
        _prefix = node.attributes().namedItem("prefix").nodeValue();
        _suffix = node.attributes().namedItem("suffix").nodeValue();
        _commandname = node.attributes().namedItem("commandname").nodeValue();

        QStringList list;
        int sizelist = node.childNodes().count();

        for ( int i = ascending?0:sizelist-1;
              ascending?(i < sizelist):(i >= 0);
              ascending?(i=i+1):(i=i-1) ) {
            //                        newstr = node.childNodes().at( i ).nodeName().simplified();
            QDomNamedNodeMap attributeMap = node.childNodes().at( i ).attributes();
            QString title;
            QDomNode attribute = attributeMap.namedItem("name");
            QDomNodeList fields;
            if (!attribute.isNull()) {

                fields = node.childNodes().at( i ).firstChild().firstChild().childNodes();
                if ( fields.count() > 0  ) {
                    title = "operacion:" + attribute.nodeValue().simplified();
                }
                else {
                    title = attribute.nodeValue().simplified();
                }
            }


            QDomNode attricon = attributeMap.namedItem("icon");



            QDomNode attrsub = attributeMap.namedItem("subtitle");


            if ( fields.count() == 0 ) {
                QString iconname;

                if (!attricon.isNull()   ) {
                    QString myicon = attricon.nodeValue().simplified();
                    if (!myicon.isEmpty()) {
                        iconname = "icon:" + myicon + "::";
                    }
                }

                if (!attrsub.isNull() && attrsub.nodeValue().simplified() == "yes") {
                    list << QString("operacion:subtitulo::%1").arg(iconname)+title;
                }
                else {
                    list << QString("operacion:titulo::%1").arg(iconname)+title;
                }
                continue;
            }


            if (includedesc ) {
                QDomNode attrdesc = attributeMap.namedItem("desc");
                title =  title + SafetYAWL::LISTSEPARATORCHARACTER;
                if (!attrdesc.isNull()) {

                    title = title  + attrdesc.nodeValue().simplified();
                }

                QDomNode attricon = attributeMap.namedItem("icon");
                title =  title + SafetYAWL::LISTSEPARATORCHARACTER;
                if (!attricon.isNull()) {
                    title =  title + attricon.nodeValue().simplified();
                }
                title =  title + SafetYAWL::LISTSEPARATORCHARACTER;
                if ( fields.count() == 0 ) {
                    title =  title + "#ISTITLE#";
                }

            }

            QDomNode attraction = attributeMap.namedItem("action");

            if (!attraction.isNull()) {
                QString myaction = attraction.nodeValue().simplified();
                if (!myaction.isEmpty()) {
                    _currentactions[title] = myaction;
                }

            }


            list << title;


        }
        commandslist = list;
        MainWindow::sbmenulist = list;
    }

    return commandslist;
}

DomModel::TypeField DomModel::typeField(const QString& command) {
	TypeField state = Unknown;

	if ( command == "string" ) state = String;
        else if ( command == "stringbox" ) state = StringBox;
	else if ( command == "filename" ) state = FileName;
        else if ( command == "datetime" ) state = DateTime;
    else if ( command == "output" ) state = Output;
	else if ( command == "number" ) state = Number;
        else if ( command == "suggest" ) state = Suggest;
	return state;
}

void DomModel::readFile() {
     Q_CHECK_PTR( domDocument );
     QStringList list;
     if ( docok ) {
          QDomNode node = domDocument->lastChild();
          _prefix = node.attributes().namedItem("prefix").nodeValue();
          _suffix = node.attributes().namedItem("suffix").nodeValue();
          _commandname = node.attributes().namedItem("commandname").nodeValue();
     }
}


QString DomModel::getUpdateString(const QString& operationname, const QString& keyname,
                                  const QString& keyvalue, const QStringList& prevfields) {
    QString result;
    QString sql = QString("SELECT %1 FROM %2 WHERE %3=%4");
    QString titlelist,fieldlist, table, keyfieldname; // Parametros para realizar la consulta

    QStringList mydatetimefields;

   Q_CHECK_PTR( domDocument );
        if ( docok ) {
                QDomNode node = domDocument->lastChild();
                //QString prefix = node.attributes().namedItem("prefix").nodeValue();
                QString suffix = node.attributes().namedItem("suffix").nodeValue();

                int sizelist = node.childNodes().count();

                for ( int i = 0; i < sizelist; i++ ) {
                    //                        newstr = node.childNodes().at( i ).nodeName().simplified();
                    QDomNamedNodeMap attributeMap = node.childNodes().at( i ).attributes();
                    QString operation = attributeMap.namedItem("name").nodeValue().trimmed();
                    //qDebug("operacion: |%s| == |%s|", qPrintable(operation), qPrintable(operationname));
                    if ( operationname != operation ) {
                                continue;
                    }



                    QDomNodeList fields = node.childNodes().at( i ).firstChild().firstChild().childNodes();
                    QDomNode commandNode = node.childNodes().at( i ).firstChild();
                    if ( !commandNode.isNull() ) {
                        QDomNamedNodeMap commandAttributeMap = commandNode.attributes();                    
                        table = commandAttributeMap.namedItem("table").nodeValue();
                    }
                    //qDebug("fields attr count: %d", fields.count());
                    int countitle = 0;
                    for( int j  = 0; j < fields.count(); j++) {
                         QDomNode fieldNode = fields.at(j).firstChild();

                         QDomNamedNodeMap fieldAttributeMap = fields.at(j).attributes();

                         QDomNode attrvisible = fieldAttributeMap.namedItem("visible");
                         if ( !attrvisible.isNull() ) {
                             QString visible = attrvisible.nodeValue().simplified();
                             if (visible == "no" ) {
                                 continue;
                             }
                             if ( visible != "yes" && !visible.isEmpty()) {
                                 QString myfieldlist = fieldlist;
                                 QString mykeyfieldname = keyfieldname;
                                 QString mytitlelist = titlelist;
                                 if (myfieldlist.endsWith(",")) {
                                     myfieldlist.chop(1);
                                 }
                                 if (mytitlelist.endsWith(",")) {
                                     mytitlelist.chop(1);
                                 }

                                 QString mysql = sql.arg(myfieldlist).arg(table)
                                                 .arg(mykeyfieldname).arg(SafetYAWL::addQuotes(keyvalue));

                                 QString myresult = doUpdateSqlString(mysql,mytitlelist,mydatetimefields);


                                 qDebug();
                                 qDebug();
                                 qDebug("....myresult:|%s|", qPrintable(myresult));
                                 qDebug("     ...VISIBLE fieldNode:|%s|",
                                        qPrintable(fieldAttributeMap.namedItem("title").nodeValue()));
                                 qDebug();
                                 qDebug();



                                 bool isvisible = searchVisibility(fields.at(j),
                                                                   myresult);
                                 qDebug("...isvisible:%d",isvisible);
                                 //isvisible = false;
                                 if ( !isvisible ) {

                                     qDebug("     NO ES VISIBLE...");
                                     qDebug();
                                     continue;
                                 }
                             }
                         }

                         QString title = fieldAttributeMap.namedItem("title").nodeValue();
                         QString field = fieldNode.nodeValue().trimmed();
//                         QString options = fieldAttributeMap.namedItem("options").nodeValue();
                         QString typefield = fieldAttributeMap.namedItem("type").nodeValue().trimmed();
//                         if ( typefield.compare(QLatin1String("combolisttable"),Qt::CaseSensitive) == 0 ) {
//                                    field = extractNameField(options,table);
//                         }

                         if ( typefield.compare(QLatin1String("datetime"),Qt::CaseSensitive) == 0 ) {
                               mydatetimefields.append(field);
                         }
                         if ( title.isEmpty() ) {
                             title = field;
                         }
                         if (  keyname == field || keyname == title) {
                            keyfieldname = field;
                            // Probar si es la clave principal, falta ver si es un campo indice
                            if ( fieldAttributeMap.namedItem("primarykey").nodeValue().trimmed().length() == 0 ) {
                                return QString("");
                            }
                         }

                         if ( prevfields.contains( title ) ) {
                             continue;
                         }
//                         qDebug("........getUpdateString.field: %s",
//                                 qPrintable(fieldNode.nodeValue().trimmed()));

                         if ( !fieldAttributeMap.contains("primarykey") && !fieldAttributeMap.contains("sequence")
                               && !fieldAttributeMap.contains("function")
                             && !fieldAttributeMap.contains("literal")
                             && typeFieldFromString(typefield) != DomModel::Unknown ) {

                            fieldlist = fieldlist+field;
                            fieldlist = fieldlist + ",";
                            title+=suffix;
                            title = QString("%1%%2").arg(title).arg(countitle+1);
                            countitle++;
                            titlelist = titlelist+(title);
                            titlelist = titlelist + "\n";                            

                        }




                    }

                }
            }
            fieldlist.chop(1);
            titlelist.chop(1);
            SYD << tr("...DomModel::getUpdateString.....fieldlist: %1")
                   .arg(fieldlist);
            sql = sql.arg(fieldlist).arg(table).arg(keyfieldname).arg(SafetYAWL::addQuotes(keyvalue));
            result = doUpdateSqlString(sql,titlelist,mydatetimefields);

        return result;
}

QString DomModel::extractNameField(const QString& options,const QString& table) {
    QString result;
    QStringList mylist = options.split("::",QString::SkipEmptyParts);
    QStringList mypars = mylist.at(0).split(":",QString::SkipEmptyParts);
    if (mylist.count() < 2 ) {
        return  mypars.at(0);
    }

    result = mylist.at(1);

    return result;
}

QString DomModel::doUpdateSqlString(const QString& sql, const QString& titles, const QStringList& dtw) {
    SafetYAWL::fieldsvalue = "";
        QString result = titles;
        SafetYAWL::streamlog
                << SafetLog::Debug
                   << tr("convertTimetToDMY: count %1")
                       .arg(dtw.count());
        if (dtw.count() > 0 ) {
            SafetYAWL::streamlog
                    << SafetLog::Debug
                       << tr("convertTimetToDMY: field: |%1|")
                           .arg(dtw.at(0));
        }
      QSqlQuery query( SafetYAWL::currentDb );

      QString command = sql;
      command.replace("id='coloured'","id>=0");
       query.prepare(  command );
         bool executed = query.exec();
         if (!executed ) {
              SafetYAWL::streamlog << SafetLog::Error << tr("no se ejecutÃ³ correctamente la sentencia SQL: \"%1\"").arg(command);
              return QString("");
          }

         bool isnext = query.next();
         if ( !isnext ) {
          SYE << tr("No hay registros para ejecutar la sentencia SQL: \"%1\"").arg(command);
          return QString("");
         }
         for (int i = 0; i < query.record().count(); i++) {
             QString myvalue;
             myvalue = query.record().value(i).toString();
             if (dtw.contains(query.record().fieldName(i).trimmed())) {
                 myvalue = DomModel::convertTimetToDMY(query.record().value(i).toString());
                 result = result.arg(myvalue+SafetYAWL::LISTSEPARATORCHARACTER);
                 SafetYAWL::fieldsvalue = SafetYAWL::fieldsvalue+ "'"+myvalue+"',";
                continue;
             }

             result = result.arg(myvalue+SafetYAWL::LISTSEPARATORCHARACTER);
             bool addquotes =  !(SafetYAWL::canTypeConvert(myvalue, QVariant::Bool)
                                 || SafetYAWL::canTypeConvert(myvalue, QVariant::Int));
             if (addquotes) {
                SafetYAWL::fieldsvalue = SafetYAWL::fieldsvalue+ "'"+
                                       query.record().value(i).toString()+"',";
            }
             else {
                 SafetYAWL::fieldsvalue = SafetYAWL::fieldsvalue+
                                        query.record().value(i).toString()+",";

             }

         }
         SafetYAWL::fieldsvalue.chop(1);
         qDebug("***...doUpdateSqlString...:%s", qPrintable(result));
        return result;
}
QString DomModel::convertTimetToDMY(const QString& d) {
    QString result;
    QDateTime mydate;
    bool ok;
    mydate.setTime_t(d.toUInt(&ok));
    result = mydate.toString("dd/MM/yyyy");
    return result;
}

QStringList  DomModel::fieldsWithAttr(const QString& command,QString namekey) {

    QStringList result;

    QStringList opps = command.split(":");
    QString cmd = command;
    if (opps.count() > 1 ) {
        cmd = opps.at(1);
    }

    if ( cmd.isEmpty()) {
        SafetYAWL::streamlog
                << SafetLog::Error
                   << tr("El comando pasado para calcular la Clave (Campo) Primaria \"%1\" es Inválido")
                      .arg(command);
        return result;
    }
    Q_CHECK_PTR( domDocument );

    if ( !docok ) {

     SYE << tr("El documento XML para calcular la Clave (Campo) Primaria \"%1\" es Inválido")
                      .arg(command);
        return result;
    }
    QDomNode node = domDocument->lastChild();
    _prefix = node.attributes().namedItem("prefix").nodeValue();
    _suffix = node.attributes().namedItem("suffix").nodeValue();
    _commandname = node.attributes().namedItem("commandname").nodeValue();

    int sizelist = node.childNodes().count();
    int i = 0;
    for ( i = 0; i < sizelist; i++ ) {
        Q_ASSERT(node.childNodes().at( i ).attributes().contains("name") );
        QString foundcommand = node.childNodes().at( i ).attributes().namedItem("name").nodeValue();
        if (cmd.compare(foundcommand, Qt::CaseSensitive) == 0 ) {
            break;
        }
    }
    if ( i == sizelist )  {
        return result;
    }

    Q_ASSERT( !node.childNodes().at( i ).isNull() );
    Q_ASSERT( !node.childNodes().at( i ).firstChild().isNull() );
    Q_ASSERT( !node.childNodes().at( i ).firstChild().firstChild().isNull() );

    QDomNodeList fields = node.childNodes().at( i ).firstChild().firstChild().childNodes();

    for ( int i = 0; i < fields.count(); i++ ) {
        QDomNamedNodeMap attributeMap = fields.at( i ).attributes();
        QDomNode attrprimarykey = attributeMap.namedItem(namekey /* por defecto changekey */);
        QDomNode attrenablethis = attributeMap.namedItem("enablethis");
        if (!attrenablethis.isNull()) {
            QString enablethis = attrenablethis.nodeValue().simplified();
            result.append(QLatin1String("::enablethis:")+enablethis);
        }

        if ( !attrprimarykey.isNull() ) {
            QString primary = attrprimarykey.nodeValue().simplified();
            if (primary.compare("no") == 0 ) {
                continue;
            }
            if ( primary.compare("yes") == 0 ) {
                QDomNode attribute = attributeMap.namedItem("title");
                QString title;
                QDomNode attrliteral = attributeMap.namedItem("literal");
                QString literal = attrliteral.nodeValue().simplified();
                if ( !literal.isEmpty() ) {
                    result.append(QLatin1String("::literal:")+literal);
                }
                else {
                    if ( attribute.isNull() ) {
                        SafetYAWL::streamlog
                                << SafetLog::Error
                                << tr("Al buscar la clave primaria, resulto que el título estaba vacío");
                    }
                    title = attribute.nodeValue().simplified();
                    result.append(title);
                }
            }

        }

    }

    return result;
}

QStringList DomModel::getFields(const QString& command, bool includedesc) {

     repetiblesFields.clear(); // Limpiar los campos repetibles
     Q_CHECK_PTR( domDocument );
     QStringList list;
     if ( docok ) {
          QDomNode node = domDocument->lastChild();
          _prefix = node.attributes().namedItem("prefix").nodeValue();
          _suffix = node.attributes().namedItem("suffix").nodeValue();
          _commandname = node.attributes().namedItem("commandname").nodeValue();

          int sizelist = node.childNodes().count();
          QString mandatory = "";
          int i = 0;
          for ( i = 0; i < sizelist; i++ ) {
               Q_ASSERT(node.childNodes().at( i ).attributes().contains("name") );
               QString foundcommand = node.childNodes().at( i ).attributes().namedItem("name").nodeValue();
               if (command.length() == 0 || command.compare(foundcommand, Qt::CaseInsensitive) == 0 ) break;
          }
          if ( i == sizelist ) return list;

          Q_ASSERT( !node.childNodes().at( i ).isNull() );
          Q_ASSERT( !node.childNodes().at( i ).firstChild().isNull() );
          Q_ASSERT( !node.childNodes().at( i ).firstChild().firstChild().isNull() );

          QDomNodeList fields = node.childNodes().at( i ).firstChild().firstChild().childNodes();

          for ( int j = 0; j < fields.count(); j++ ) {
               QDomNamedNodeMap attributeMap = fields.at( j ).attributes();
               if ( attributeMap.contains("sequence") ||  attributeMap.contains("literal")  ) continue;
               if ( !attributeMap.namedItem("input").isNull() ) {
                   QString myinput = attributeMap.namedItem("input").nodeValue().simplified();
                    if ( myinput == "no" ) {
                         continue;
                    }
                    if (myinput !=  "yes") {
                        QStringList mylist = myinput.split("::");
                        if (mylist.count() > 2 ) {
                            continue;
                        }
                    }
               }

               QDomNode attrvisible = attributeMap.namedItem("visible");
               if ( !attrvisible.isNull() ) {
                    QString visible = attrvisible.nodeValue().simplified();
                    if (visible == "no" ) {
                         continue;
                    }
                    if ( visible != "yes" ) {
                         bool isvisible = searchVisibility(fields.at( j ));
                         if ( !isvisible ) continue;
                    }
               }

               QDomNode attribute = attributeMap.namedItem("title");
               QString title;
               if ( !attribute.isNull() ) {
                    title = attribute.nodeValue().simplified();
               }


               QDomNode attrmandatory = attributeMap.namedItem("mandatory");
               if ( !attrmandatory.isNull()) {
                   if (attrmandatory.nodeValue().simplified().compare("yes") == 0 ){
                         mandatory = "*";
                     }
                   else {
                         mandatory = "";
                     }
               }

               QDomNode attricon = attributeMap.namedItem("icon");
               QString pathicon;
               if ( !attricon.isNull()) {
                   pathicon = attricon.nodeValue().simplified();
               }

               if (title.isEmpty() ) {
                   title = fields.at( j ).firstChild().nodeValue().simplified();
                   title = title + mandatory;
               }
               else {
                   title = title + mandatory;
                   if ( includedesc ) {
                       QDomNode attrdesc = attributeMap.namedItem("desc");
                       if ( !attrdesc.isNull() ) {
                           QString desc;
                           desc = attrdesc.nodeValue().simplified();
                           title =  title + SafetYAWL::LISTSEPARATORCHARACTER + desc;
                       }
                   }
               }
               if ( includedesc ) {
                   if (!pathicon.isEmpty()) {
                       title =  title + SafetYAWL::LISTSEPARATORCHARACTER + pathicon;
                   }
               }
               QDomNode attrrepetible = attributeMap.namedItem("repetible");
               if (!attrrepetible.isNull() && attrrepetible.nodeValue().simplified()=="yes" )  {

                   QString titleRep = title;
                   if (titleRep.endsWith("*")) {
                       titleRep.chop(1);
                   }

                   repetiblesFields += titleRep;
               }

               list <<  title;

           }
      }
     MainWindow::sbmenulist = list;
//     setStringList ( list );
     return list;
}

bool DomModel::isRepetibleField(const QString& f) {
    if ( repetiblesFields.contains( f ) ) return true;
    return false;
}

bool DomModel::searchVisibility(const QDomNode& node,
                                const QString& texttosearch) {

    qDebug("....searchVisibility..,,(1)..");

     QDomNamedNodeMap attributeMap = node.attributes();
     QString visible = attributeMap.namedItem("visible").nodeValue().simplified();
     QString path = attributeMap.namedItem("path").nodeValue().simplified();
    qDebug("...path:|%s|",qPrintable(path));
     qDebug("...visible:|%s|",qPrintable(visible));
     qDebug("...fieldsvalue:|%s|",qPrintable(SafetYAWL::fieldsvalue));
     qDebug("...texttosearch:|%s|",qPrintable(texttosearch));


     if (!QFile::exists(path)) {
         QString resultvalue;
         QString localfieldsvalue = SafetYAWL::fieldsvalue;
         if ( localfieldsvalue.endsWith(",")) {
             localfieldsvalue.chop(1);
         }
         QString title,namefield;
         if ( path.split(":").count()>1) {
             title = path.split(":").at(0);
             namefield = path.split(":").at(1);
         }
         else  {
             title = path;
             namefield = path;

         }

//         qDebug("                  ****...title:|%s|",qPrintable(title));

         QString keyvalue = searchKeyValueOnTextEdit(title,texttosearch);



//         qDebug("                  ****...keyvalue:|%s|",qPrintable(keyvalue));


         bool result =true;
         bool addquotes =  !(SafetYAWL::canTypeConvert(keyvalue, QVariant::Bool)
                             || SafetYAWL::canTypeConvert(keyvalue, QVariant::Int));

         if (addquotes) {
             keyvalue = QString("'%1'").arg(keyvalue);
         }
         result = SafetYAWL::createTableSignDb(namefield,
                                                    keyvalue,
                                                    visible,
                                                    resultvalue);
//         qDebug("*****...resultvalue:|%s|",qPrintable(resultvalue));

         if (resultvalue == "0") {

             return false;
         }
         return true;

     }

     QString keyvalue = searchKeyValueOnTextEdit();
     if ( keyvalue.isEmpty() ) {
         return false;
     }


     QStringList list = visible.split(",");
     if (MainWindow::configurator != NULL ) {
         delete  MainWindow::configurator;
     }
     MainWindow::configurator = new SafetYAWL();
     Q_CHECK_PTR( MainWindow::configurator );


     MainWindow::configurator->openXML(path);
     MainWindow::configurator->convertXMLtoObjects();
     MainWindow::configurator->openDataSources();
     QStringList vars;
     if ( MainWindow::configurator->getWorkflows().count() > 0 ) {

          vars = MainWindow::configurator->getWorkflows().at(0)->listNextStates(keyvalue);
     }
     if ( vars.toSet().unite(list.toSet()).count() == vars.count() ) {
         return true;
     }
     return false;
}


//QVariant DomModel::data(const QModelIndex &index, int role) const
//{
//	return QStringListModel::data(index, role);
//}



void DomModel::domModelFromFile(const QString& fileName) {

 QFile file(fileName);
 if ( domDocument == NULL ) 
	domDocument = new QDomDocument("document");
 Q_CHECK_PTR( domDocument );
 if (!file.open(QIODevice::ReadOnly)) {
     return;
}
 if (!domDocument->setContent(&file)) {
     file.close();
     return;
 }
 file.close();
 
 docok = true;
}



/*!
    \fn DomModel::typeField(const QString& command)
 */
DomModel::TypeField DomModel::typeFieldFromString(const QString& command) {
       if  ( command == "string" ) return DomModel::String;
       else if (command == "stringbox" ) return DomModel::StringBox;
       else if (command == "datetime" ) return DomModel::DateTime;
       else if (command == "output" ) return DomModel::Output;
       else if (command == "number" ) return DomModel::Number;
       else if (command == "filename" ) return DomModel::FileName;
       else if (command == "suggest" ) return DomModel::Suggest;
       else if (command.compare("combovar",Qt::CaseInsensitive) == 0 ) return DomModel::ComboVar;
       else if (command.compare("combotask",Qt::CaseInsensitive) == 0 ) return DomModel::ComboTask;
       else if (command.compare("combographs",Qt::CaseInsensitive) == 0 ) return DomModel::ComboGraphs;
       else if (command.compare("comboautofilter",Qt::CaseInsensitive) == 0 ) {
           return DomModel::ComboAutofilter;
       }
       else if (command.compare("comborecursivefilter",Qt::CaseInsensitive) == 0 ) {
           return DomModel::ComboRecursivefilter;
       }
       else if (command.compare("combolisttable",Qt::CaseInsensitive) == 0 ) return DomModel::ComboListTable;
       else if (command.compare("combolistbinaryrepo",Qt::CaseInsensitive) == 0 ) return DomModel::ComboListBinaryRepo;
       else if (command.compare("combolistliteral",Qt::CaseInsensitive) == 0 ) return DomModel::ComboListLiteral;
       else if (command.compare("comboflow",Qt::CaseInsensitive) == 0 ) return DomModel::ComboFlow;
       else if (command.compare("comboconffile",Qt::CaseInsensitive) == 0 ) return DomModel::ComboConffile;
       else if (command.compare("listconffile",Qt::CaseInsensitive) == 0 ) return DomModel::ListConffile;
       else if (command.compare("listtable",Qt::CaseInsensitive) == 0 ) return DomModel::ListTable;
       else if (command.compare("listliteral",Qt::CaseInsensitive) == 0 ) return DomModel::ListLiteral;
       else if (command.compare("combocolor",Qt::CaseInsensitive) == 0 ) return DomModel::ComboColor;
	
       return DomModel::Unknown;
}



CmdWidget* DomModel::selWidgetFromField(const QString& command, const QString& field,
                                        QString keyvalue, const QStringList& form) {


    SYD << tr("............DomModel::selWidgetFromField.....................kevalue:|%1|")
           .arg(keyvalue);
     if ( !docok ) {
         return NULL;
     }


     CmdWidget *mywidget = NULL;
     QDomNode node = domDocument->lastChild();
     _prefix = node.attributes().namedItem("prefix").nodeValue();
     _suffix = node.attributes().namedItem("suffix").nodeValue();
     _commandname = node.attributes().namedItem("commandname").nodeValue();
     int sizelist = node.childNodes().count();

     int i;
     for ( i = 0; i < sizelist; i++ ) {
          Q_ASSERT(node.childNodes().at( i ).attributes().contains("name") );
          QString foundcommand = node.childNodes().at( i ).attributes().namedItem("name").nodeValue();
          if ( (command.compare(foundcommand, Qt::CaseInsensitive) == 0)
              || command.length() == 0 ) {
              break;
          }
     }

     if ( i == sizelist ) {
         return mywidget;
     }
     Q_ASSERT( !node.childNodes().at( i ).isNull() );
     //Q_ASSERT( !node.childNodes().at( i ).firstChild().isNull() );
     // Q_ASSERT( !node.childNodes().at( i ).firstChild().firstChild().isNull() );
     QDomNodeList fields = node.childNodes().at( i ).firstChild().firstChild().childNodes();

     QObject* parent = NULL;

     SYD << tr("............****DomModel::selWidgetFromField....(3)....fields.count():|%1|").arg(fields.count());
     for ( int j = 0; j < fields.count(); j++ ) {
          QDomNamedNodeMap attributeMap = fields.at( j ).attributes();
          QDomNode attribute = attributeMap.namedItem("title");
          QString namefield = fields.at(j).firstChild().nodeValue().simplified();
          if ( !attribute.isNull() ) {
                namefield = attribute.nodeValue().simplified();
           }
           if ( namefield == field ) {

//               QRegExpValidator* validator = NULL;
//               validator = checkValidation(attributeMap);

                SYD << tr("............****DomModel::selWidgetFromField....(4)");
               QMap<QString,QVariant> myconf;
//               QString pathflow;
               QDomNode attribute = attributeMap.namedItem("options");
               if ( !attribute.isNull() ) {
                    myconf.insert("options", attribute.nodeValue().simplified());
                   SYD << tr("............****DomModel::selWidgetFromField....options:|%1|")
                          .arg(attribute.nodeValue().simplified());
               }
               QDomNode pathattr = attributeMap.namedItem("path");
               if ( !pathattr.isNull() ) {
                    myconf.insert("path", pathattr.nodeValue().simplified());
               }

               attribute = attributeMap.namedItem("filter");
               if ( !attribute.isNull() ) {
                    myconf.insert("filter", attribute.nodeValue().simplified());
               }
               attribute = attributeMap.namedItem("literal");
               if ( !attribute.isNull() ) {
                    myconf.insert("literal", attribute.nodeValue().simplified());
               }

               attribute = attributeMap.namedItem("format");
               if ( !attribute.isNull() ) {
                    myconf.insert("format", attribute.nodeValue().simplified());
               }
               attribute = attributeMap.namedItem("order");
               if ( !attribute.isNull() ) {
                    myconf.insert("order", attribute.nodeValue().simplified());
               }
               attribute = attributeMap.namedItem("desc");
               if ( !attribute.isNull() ) {
                    myconf.insert("desc", attribute.nodeValue().simplified());
               }


               attribute = attributeMap.namedItem("validation");
               if ( !attribute.isNull() ) {
                    myconf.insert("validation", attribute.nodeValue().simplified());
               }

               attribute = attributeMap.namedItem("input");
               if ( !attribute.isNull() ) {
                    myconf.insert("input", attribute.nodeValue().simplified());
               }

               attribute = attributeMap.namedItem("exclude");
               if ( !attribute.isNull() ) {
                    myconf.insert("exclude", attribute.nodeValue().simplified());
               }


               attribute = attributeMap.namedItem("changefor");
               if ( !attribute.isNull() ) {
                    myconf.insert("changefor", attribute.nodeValue().simplified());
                    SYD << tr("................DomModel::changefor...changefor:|%1|")
                           .arg(attribute.nodeValue().simplified());
               }

               attribute = attributeMap.namedItem("enablethis");
               if ( !attribute.isNull() ) {
                    myconf.insert("enablethis", attribute.nodeValue().simplified());
               }

               if ( !form.isEmpty() ) {
                myconf.insert("formstring", form);
               }
               SYD << tr("............****DomModel::selWidgetFromField....(5)");

               for (int i = 0; i < attributeMap.count(); ++i) {
                    QDomNode attribute = attributeMap.item(i);
                    if ( attribute.nodeName() != "type" ) {
                        continue;
                    }


                    DomModel::TypeField currtype = typeFieldFromString( attribute.nodeValue().simplified() );

                    SYD << tr("currtype:%1").arg((int)currtype);
                    switch ( currtype ) {
                    case FileName:
                         SYD << tr("FileName...(1)");
                         mywidget = new GetFileWidget(field, parent);
                         SYD << tr("FileName...(2)");
                         Q_CHECK_PTR( mywidget );
                         mywidget->setConf(myconf);
                         SYD << tr("FileName...(3)");
                         mywidget->buildWidget();
                         SYD << tr("FileName...(4)");
                        // mywidget->setValidator( validator);
                         SYD << tr("FileName...(5)");
                         return mywidget;
                   case DateTime:
                         mywidget = new DateTimeWidget(field, parent);
                         Q_CHECK_PTR( mywidget );
                         myconf.insert("keyvalue", keyvalue);
                         mywidget->setConf(myconf);
                         return mywidget;
                    case Output:
                          mywidget = new OutputWidget(field, parent);
                          Q_CHECK_PTR( mywidget );
                          myconf.insert("keyvalue", keyvalue);
                          mywidget->setConf(myconf);
                          return mywidget;

                    case Number:
                         SYD << tr("............****DomModel::selWidgetFromField....NUMBER....");
                         SYD << tr("............****DomModel::selWidgetFromField....myconf....changefor:|%1|")
                                .arg(myconf["changefor"].toString());
                         mywidget = new NumberWidget(field, parent);
                         Q_CHECK_PTR( mywidget );
                         myconf.insert("keyvalue", keyvalue);
                         mywidget->setConf(myconf);
//                         mywidget->buildWidget();
                        // mywidget->setValidator( validator);
                         return mywidget;

                    case String:
                        SYD << tr(".............DomModel::selWidgetFromField........CmdWidget()...");
                         mywidget = new CmdWidget(field, parent);
                         mywidget->setConf(myconf);
                         Q_CHECK_PTR( mywidget );
                         mywidget->buildWidget();
                     //    mywidget->setValidator( validator);
                         return mywidget;
                    case Suggest:
                          mywidget = new SuggestWidget(field, parent);
                          mywidget->setConf(myconf);
                          Q_CHECK_PTR( mywidget );
                          mywidget->buildWidget();
                     //     mywidget->setValidator( validator);
                          return mywidget;

                    case StringBox:
                         mywidget = new TextEditWidget(field, parent);
                         Q_CHECK_PTR( mywidget );
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                     //    mywidget->setValidator( validator);
                         return mywidget;
                    case ComboAutofilter:

                        SYD << tr("............****DomModel::selWidgetFromField.................**ComboAutofilter....keyvalue:|%1|")
                                .arg(keyvalue);

                         mywidget = new ComboWidget(field,ComboWidget::AutofilterSafet, parent);
                         Q_CHECK_PTR( mywidget );
                         myconf.insert("keyvalue", keyvalue);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                        break;
                    case ComboRecursivefilter:

//                         pathflow = searchPathFlow();
                         mywidget = new ComboWidget(field,ComboWidget::RecursivefilterSafet, parent);
                         Q_CHECK_PTR( mywidget );
                         myconf.insert("keyvalue", keyvalue);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                        break;

                    case ComboVar:
                        SYD << tr("............****DomModel::selWidgetFromField.................**ComboVar....kevalue:|%1|")
                                .arg(keyvalue);

                          mywidget = new ComboWidget(field,ComboWidget::VariableSafet, parent);
                         Q_CHECK_PTR( mywidget );
                         SYD << tr("............****DomModel::selWidgetFromField.................ComboVar....kevalue:|%1|")
                                 .arg(keyvalue);

                         myconf.insert("keyvalue", keyvalue);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                        break;
                    case ComboTask:
                         SYD << tr(".........Combotask...keyvalue:|%1|").arg(keyvalue);
                          mywidget = new ComboWidget(field,ComboWidget::TaskSafet, parent);
                         Q_CHECK_PTR( mywidget );
                         myconf.insert("keyvalue", keyvalue);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                        break;

                    case ComboGraphs:
                         mywidget = new ComboWidget(field,ComboWidget::GraphsSafet, parent);
                         Q_CHECK_PTR( mywidget );
                         myconf.insert("keyvalue", keyvalue);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                        break;
                    case ComboListTable:
                          mywidget = new ComboWidget(field,ComboWidget::ListTable, parent);
                          myconf.insert("keyvalue", keyvalue);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                    case ComboListBinaryRepo:
                          mywidget = new ComboWidget(field,ComboWidget::ListBinaryRepo, parent);
                          myconf.insert("keyvalue", keyvalue);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                    case ComboListLiteral:

                          mywidget = new ComboWidget(field,ComboWidget::ListLiteral, parent);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                     case ComboColor:

                         mywidget = new ComboWidget(field,ComboWidget::ColorSafet, parent);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                         break;
                     case ComboConffile:
                     case ListConffile:
                         if ( typeFieldFromString( attribute.nodeValue().simplified() )
                             == ComboConffile ) {
                            mywidget = new ComboWidget(field,ComboWidget::ConffileSafet, parent);
                         }
                         else {
                           //mywidget = new ListWidget(field,ListWidget::ConffileSafet, parent);
//                           QRect myrect = mywidget->geometry();
//                           myrect.setHeight(300);
//                           mywidget->setGeometry(myrect);

                         }

                         mywidget->setConf(myconf);
                          mywidget->buildWidget();
                         return mywidget;
                         break;
                     case ListTable:
//                         mywidget = new ListWidget(field,ListWidget::ListTable, parent);
//                         myrect = mywidget->geometry();
//                         myrect.setHeight(300);
//                         mywidget->setGeometry(myrect);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         return mywidget;
                     case ListLiteral:
//                         mywidget = new ListWidget(field,ListWidget::ListLiteral, parent);
//                         myrect = mywidget->geometry();
//                         myrect.setHeight(300);
//                         mywidget->setGeometry(myrect);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();
                         qDebug("ListLiteral...mywidget->setConf(myconf)...(2)...");
                         return mywidget;
                     case ComboFlow:


                          SYD << tr("DOMMODEL ComboFlow: keyvalue: |%1|").arg(keyvalue);
                          myconf.insert("keyvalue", keyvalue);
                          mywidget = new ComboWidget(field,ComboWidget::Flow, parent);
                         mywidget->setConf(myconf);
                         mywidget->buildWidget();

                         return mywidget;
                         break;
                   case Unknown:
			 return NULL;
                        break;
                    }
                    break;
               }
               if ( mywidget ) {
                    return mywidget;
               }
          }
     }

     return mywidget;
}

bool DomModel::isDynWidget(const QString& field) {
    qDebug("...**DomModel::isDynWidget....field:%s", qPrintable(field));
 if ( SafetYAWL::listDynWidget == NULL ) {
     qDebug("isDynWidget...return false");
     return false;
 }
 for(int i = 0; i< SafetYAWL::listDynWidget->count(); i++) {
  WidgetInterface* w = SafetYAWL::listDynWidget->at( i ) ;
  qDebug("...w->descriptor(): %s", qPrintable(w->descriptor()));
        if ( w->descriptor() == field ) {
            return true;
        }
 }

  return false;

}

CmdWidget* DomModel::checkDynWidget(const QString& s, const QString& field, QWidget *parent) {
    qDebug("...DomModel::checkDynWidget....title:%s", qPrintable(s));
 CmdWidget *mywidget = NULL;
 if ( SafetYAWL::listDynWidget == NULL ) {
     return mywidget;
 }
 for(int i = 0; i< SafetYAWL::listDynWidget->count(); i++) {
  WidgetInterface* w = SafetYAWL::listDynWidget->at( i ) ;
  qDebug("...w->descriptor(): %s", qPrintable(w->descriptor()));
        if ( w->descriptor() == s ) {
             qDebug("...aqui se llama el widget....field: %s", qPrintable(field));
             qDebug("...aqui se llama el widget....w: %d", w);
             mywidget = w->getWidget(field, parent);                             
             break;
        }
 }

  return mywidget;
}


QString DomModel::searchPathFlow() const {
    qDebug("...DomModel::searchPathFlow....");

    QString result;

    QRegExp rx(QString("%1\\:\\s+([\\-a-zA-Z_0-9\\(\\)\\./]+)").arg(Safet::LOADFLOWFILE_NAMEFIELD));
    QString mystr = stringToParse();

    qDebug("searchPathFlow...mypattern:|%s|", qPrintable(rx.pattern()));
    qDebug("*searchPathFlow...mytext:|%s|",qPrintable(mystr));
    int index = rx.indexIn(mystr);
    qDebug("searchPathFlow...index...:%d", index);

    if ( index == -1 ) {
        return result;
    }

    result = rx.cap(1);

     return result;
}


QString DomModel::searchAutofilterFlow() const {
    qDebug("...DomModel::searchAutofilterFlow....");
     QString result;
     return result;
}

QString DomModel::searchRecursivefilterFlow() const {
    qDebug("...DomModel::searchRecursivefilterFlow....");
     QString result;
     return result;
}


QString DomModel::searchKeyValueOnTextEdit(const QString& keyname,
                                           const QString& texttosearch) const {

     QString result;
     return result;
}




