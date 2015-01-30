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
#include "SafetStats.h"
#include "SafetYAWL.h"
#include "SafetBinaryRepo.h"

using namespace Safet;


SafetStats::SafetStats(SafetWorkflow *wf, QObject *parent) {

      stats = 0;
      mywf = wf;
      _totaltokens = -1;

}


SafetStats::~SafetStats() {


}


void SafetStats::setWorkflow(SafetWorkflow *wf) {
     Q_CHECK_PTR( wf );
     mywf = wf;


}


SafetStats::AttrType SafetStats::parseAttrType(const QString& a) {

     SafetStats::AttrType result = AttrNone;
     if ( a.compare("Size", Qt::CaseInsensitive ) == 0 ) {
               result =  SafetStats::TaskSize;
          }
     else if ( a.compare("Color", Qt::CaseInsensitive ) == 0 ) {
               result =  SafetStats::TaskColor;
     }
     else if ( a.compare("Size", Qt::CaseInsensitive ) == 0 ) {
               result =  SafetStats::TaskCustom;
     }

     return result;
}

QStringList SafetStats::processInfo(SafetNode* node, SafetStats::StatsType t,
                                    const QString& a,bool  norender,
                                    const QString& info) {
     QStringList result;
     QString newinfo;
     QString textualinfo;

     bool actived = SafetYAWL::getConf()["Stats/actived"] == QString("on");



     int fromdate = -1;
     int todate = -1;



     //Q_CHECK_PTR( wf );
     if ( node  == NULL) {
                 SYW << tr("Nodo con la informaci�n: \"%1\" es Nulo")
                 .arg(a);
         return result;

     }

     QString idtask =  node->id();
     SafetStats::AttrType attr = parseAttrType(a);
     int fichas = 0;
     if ( _totaltokens == -1 ) {
        if ( SafetYAWL::_isstatstokenfound ) {
           _totaltokens = 1;
        }
        else {
        _totaltokens =  numberAllOfTokens(norender,info);
        }
    }

     mywf->setNumberOfTokens( _totaltokens );

     if (node->numtokens() == -1 ) {
         if ( SafetYAWL::_isstatstokenfound ) {
              fichas = 0;
         }
         else {
            fichas = numberOfTokens(idtask,norender, info);
        }
        node->setNumtokens(fichas);
    }
     else {
         fichas = node->numtokens();
     }


     double porc = 0;
     bool isreported = false;
     QString reportvalue = SafetYAWL::getConf()["DefaultValues/report"].trimmed();
     
     QString currkey = mywf->id().trimmed()+idtask.trimmed();
     if (node->report().isEmpty() )  {
         if ( reportvalue == "yes") {
            isreported = true;
         }
         else  {
            isreported = false;
         }

     }
     else  if ( node->report().compare("no", Qt::CaseInsensitive)  != 0)  {

         if ( reportvalue.compare("yes",Qt::CaseInsensitive) == 0 ) {

             isreported = true;
         }
         else {

             isreported =  node->report().compare("yes", Qt::CaseInsensitive)  == 0;

             isreported = isreported || (info.length() > 0
                                         && info.compare("coloured",Qt::CaseInsensitive) != 0);

         }
     }
     else {
         isreported = false;
     }

     QString extrainfo;
     if (isreported ) {
         if ( _totaltokens != 0 ) {
             porc = double(fichas) / double(_totaltokens) ;
         }
         switch ( t ) {
         case Coloured:
             newinfo += tr(", ")+getInfoString(attr).arg(porc, 0, 'g', 2);
             newinfo += tr(", %1...%2").arg(fichas).arg(_totaltokens);

             if ( actived ) {
                 QString trydate = SafetYAWL::getConf()["Stats/fromdate"];
                 SYD << tr("....SafetStats::processInfo...trydate (init):|%1|")
                        .arg(trydate);
                 if (trydate != "*" ) {
                     fromdate = QDateTime::fromString(trydate,Safet::DateFormat).toTime_t();
                 }

                 trydate = SafetYAWL::getConf()["Stats/todate"];
                 SYD << tr("....afetStats::processInfo...trydate (end):|%1|")
                        .arg(trydate);

                 if (trydate != "*" ) {
                     todate = QDateTime::fromString(trydate,Safet::DateFormat).toTime_t();
                 }

                 SafetBinaryRepo myrepo;
                 if (myrepo.fileExists() ) {
                     myrepo.open();
                 }


                 if (myrepo.fileExists() ) {
                     extrainfo = myrepo.calculateAverage("graphs",currkey,
                                                         fromdate,
                                                         todate);

                     if (!extrainfo.isEmpty()) {
                         newinfo += QString("...%1...%2")
                                 .arg(tr("n/a"))
                                 .arg(extrainfo+ tr("(Promedio/Espera)"));
                     }
                 }
             }


             break;
         case ForTokenKey:

             if ( porc == 1 ) {
                 SafetYAWL::_isstatstokenfound = true; // Es el �nico token
                 textualinfo =  trUtf8("*El documento o solicitud se encuentra <b>aquí</b>");
             }

             newinfo +=  tr(", ")+getInfoString(attr).arg(porc, 0, 'g', 2);
             newinfo += tr(", %1...%2...").arg(fichas).arg(_totaltokens);
             break;
         default:;
         }
     }
     result.append(newinfo);
     if ( textualinfo.length() > 0 ) {
         result.append(textualinfo);
     }
     return result;
}

QString SafetStats::getInfoString(SafetStats::AttrType o ) {
      QString result;
      switch (o)
      {
           case TaskColor:
               result = "info.task.color: %1";
           break;
           case TaskSize:
               result = "info.task.size: %1";
           break;
           case TaskCustom:
               result = "info.task.custom: %1";
           break;
           case TaskLine:
               result = "info.task.line: %1";
           break;
          case ArrowColor:
               result = "info.arrow.line: %1";
           break;
           case ArrowSize:
               result = "info.arrow.size: %1";
           break;
           case ArrowCustom:
               result = "info.arrow.custom: %1";
           break;
           case ArrowLine:
               result = "info.arrow.line: %1";
           break;

      }

     return result;
}

int SafetStats::numberAllOfTokens(bool norender, const QString& info) {
     Q_CHECK_PTR( mywf );
     if (mywf->numberOfTokens() > 0 ) return mywf->numberOfTokens();
     QList<SafetTask*> mytasks =  mywf->getTasks();
     QList<SafetVariable*>::iterator j;
     SafetVariable *curvar;
     int result = 0;

     bool isreported = SafetYAWL::getConf()["DefaultValues/report"] == QLatin1String("yes");
	

     foreach(SafetTask* t, mytasks) {
         if ( QString(t->metaObject()->className()) != "SafetTask" )  {
             continue;
         }

          // ** Validar Si se reporta el nodo o NO
          if ( t->report() != QLatin1String("no") )  {
		if (!isreported) {
	                 isreported =  t->report() == QLatin1String("yes");
        	         isreported = isreported || (info.length() > 0 && info != QLatin1String("coloured") );
		}
          }
          else {
              isreported = false;
          }
          // ** Validar Si se reporta el nodo o NO

          if ( !isreported ) continue;
          if ( t->numtokens() >= 0 ) {
            return t->numtokens();
          }
          else {
              for (j = t->getVariables().begin(); j != t->getVariables().end(); ++j) {
                   curvar = *j;
//                   qDebug("Variable: |%s|",qPrintable(curvar->id()));

                   if (info.length() == 0 || info.compare("coloured",Qt::CaseInsensitive) == 0 ) {
                        result = result + mywf->numberOfTokens(*curvar);
//                        qDebug("...............................sumando..:%d",result);
                  }
                   else {
                        //qDebug("...tokensForKey...info: |%s|...result: %d", qPrintable(info), result);
                        result = result + tokensForKey(*curvar,info,norender);
                   }
              }
             // t->setNumtokens(result);
          }
     }
     return result;
}


int SafetStats::numberOfTokens(const QString& idtask, bool norender, const QString& info) {
     int result = 0;
     Q_CHECK_PTR( mywf );
//     qDebug("...idtask...(1)....");
     SafetTask *curtask = mywf->searchTask(idtask);
     if (curtask == NULL ) {
         return 0;
     }
//     qDebug("..(2)..curtask->metaObject()->className(): %s", curtask->metaObject()->className());
     if ( QString(curtask->metaObject()->className()) != "SafetTask" ) {
         return 0;
     }
     if (curtask->numtokens()>=0) {
         return curtask->numtokens();
     }

     Q_ASSERT_X(curtask != NULL,"numberOfTokens", qPrintable(tr("No se encuentra en el flujo de trabajo la tarea : %s").arg(qPrintable(idtask))));
     SafetVariable *curvar;

     QList<SafetVariable*>::iterator j;
     for (j = curtask->getVariables().begin(); j != curtask->getVariables().end(); ++j) {
          curvar = *j;
   //       qDebug("...(1)...numberOfTokens...result: %d", result);
          if (info.length() == 0 || info.compare("coloured",Qt::CaseInsensitive) == 0 ) {
               result = result + mywf->numberOfTokens(*curvar);
          }
          else {
               //qDebug("...tokensForKey...info: |%s|...result: %d", qPrintable(info), result);
               result = result + tokensForKey(*curvar,info,norender);
          }


     }
     return result;

}

bool SafetStats::checkExtraInfo(SafetWorkflow *wf, QStringList& fields, const QString& info) {

    if (wf->getExtrainfoRols().count() == 0 ) {

        for(int i = 0; wf->getTasklist().count(); i++) {

            SafetYAWL::ExtraInfoTask myextra;
            QString myidtask ;
            SafetTask *mytask = qobject_cast<SafetTask*>(wf->getTasklist().at(i));
            if (mytask == NULL ) {
                  qDebug("....generateExtraInfo mytask is NULL");
                break;
            }
            if (mytask->getVariables().count() == 0 ) {
//                qDebug("tokensForKey...TAREA no tiene variables....");
                continue;
            }
            myidtask = mytask->id().trimmed();
            SafetVariable* myvar = mytask->getVariables().at(0);
            QString myrolfield, mytsfield;
            myrolfield = myvar->rolfield();
            mytsfield = myvar->timestampfield();
            QRegExp rxCN("cn::(\\d+)");
            bool ok;
            int indexRolCN = -1;
            int indexTSCN = -1;
            QString derror;
            QString cnrol, cnts;
            //**********************************************************
            // Para ROL

            if (myrolfield.startsWith("cn::") || mytsfield.startsWith("cn::") &&
                info != "coloured") {
            QList<SafetWorkflow::InfoSigner> infosigners;
            infosigners = wf->getSignersDocument(myvar,info,derror);

            if (myrolfield.startsWith("cn::")) {
                int pos = myrolfield.indexOf(rxCN);
                if (pos >= 0) {
                    indexRolCN = rxCN.cap(1).toUInt(&ok);
                    if (indexRolCN < 0 ) {
                        SYE
                                << tr("Uno de los campos extra (Rol) no tiene una sintáxis"
                                      " adecuada cn::<numero>");
                        return false;
                    }
                }
                if ( info.compare("coloured",Qt::CaseInsensitive) != 0) {                    
                    if (indexRolCN < infosigners.count() ) {
                       cnrol = infosigners.at(indexRolCN).commonName;
                    }
                }

            }
            else if (!myrolfield.isEmpty() && !fields.contains(myrolfield)) {
                //myfields.append(myrolfield);
                SafetYAWL::_hasextrainfo = true;
                if (!wf->getExtrainfoRols().contains(myrolfield)) {
                    wf->getExtrainfoRols().append(myrolfield);

                }
            }
            //**********************************************************
            // Para Timestamp
            SYD << tr("...mytsfield...:%1")
                    .arg(mytsfield);
            if (mytsfield.startsWith("cn::")) {
                int pos = mytsfield.indexOf(rxCN);
                if (pos >= 0) {
                    indexTSCN = rxCN.cap(1).toUInt(&ok);
                    if (indexRolCN < 0 ) {
                            SYE
                                << tr("Uno de los campos extra (Rol) no tiene una sint�xis"
                                      " adecuada cn::<numero>");
                        qDebug("Rol no tiene sintaxis adecuada");
                        return false;
                    }
                }
                if ( info.compare("coloured",Qt::CaseInsensitive) != 0) {
                    if (indexTSCN < infosigners.count() ) {
                       cnts = infosigners.at(indexTSCN).date + " "
                              + infosigners.at(indexTSCN).hour;
                    }

                }

            }
            else if (!mytsfield.isEmpty() && !fields.contains(mytsfield)) {
                //myfields.append(mytsfield);
                if (!wf->getExtrainfoTSs().contains(mytsfield)) {
                    wf->getExtrainfoTSs().append(mytsfield);
                }
                SafetYAWL::_hasextrainfo = true;
            }
            if ( !cnrol.isEmpty() && !cnts.isEmpty()) {
                myextra.rolfied = myrolfield;
                myextra.timestampfield = mytsfield;
                myextra.rolvalue = cnrol;
                cnts.chop(1);
                QDateTime mydate = QDateTime::fromString(cnts,"dd-MM-yyyy hh:mm:ss");
                mydate = mydate.addSecs(-16200 ); // -4:30 horas //Venezuela TICKET PARA LOS DEMÁS ZONAS
                myextra.timestampvalue = mydate.toTime_t();

                SafetYAWL::_extrainfomap[ myidtask ] = myextra;

            }

        }
        }

    }

    fields += wf->getExtrainfoRols();
    fields += wf->getExtrainfoTSs();
    return true;

}

int SafetStats::tokensForKey(SafetVariable& v, const QString& info,bool norender) {

     int result = -1;

     if (mywf == NULL ) {
         qDebug("ERROR: mywf is NULL");
         SafetYAWL::streamlog
                 << SafetLog::Debug
                 << tr("Error de depuración en tokensForKey mywf es NULL");
         return 0;
     }

     Q_CHECK_PTR( mywf );
     QSqlQuery query = mywf->getSQLDocuments(v);
     SafetDocument mydocument;
     result  = mydocument.countForKey(query,info,mywf,norender);

     return result;
}
