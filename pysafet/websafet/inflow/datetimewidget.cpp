/*
* SAFET Sistema Automatizado para la Firma Electrónica y Estampado de Tiempo
* Copyright (C) 2008 Víctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
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
#include "datetimewidget.h"
#include "SafetYAWL.h"
#include <QScriptEngine>
#include <QCoreApplication>

DateTimeWidget::DateTimeWidget(const QString& t, QObject *parent,bool istextparent)
    :CmdWidget(t,parent,istextparent) {
     //datetimeedit = NULL;
     if ( !conf().contains("format")) {
//                SYW  << tr("Falta opción de formato (format) para el campo de entrada \"datetime\" llamado \"%1\"")
//                 .arg(caption());
         _format = "time_t";
     }
     else {
         _format = conf()["format"].toString();
     }


}

void DateTimeWidget::setText(const QString &newText) {
//    if ( datetimeedit ) {
//        bool ok;
//        uint myseconds = newText.toInt(&ok);
//        if (ok ) {
//            datetimeedit->setDateTime(QDateTime::fromTime_t(myseconds));
//        }
//    }

}




bool DateTimeWidget::isValid(QString& value) {
    QString strvalid;
    if ( conf().contains("validation"))  {
        strvalid = conf()["validation"].toString().split("::").at(0);
    }
    qDebug(".DateTimeWidget... strvalid:  |%s|", qPrintable(strvalid));
    qDebug(".DateTimeWidget... value:  |%s|", qPrintable(value));
    if (strvalid.isEmpty()) {
        return true;
    }

    qDebug();
    qDebug("**fieldspair.first: |%s|",qPrintable(SafetYAWL::fieldsname));
    qDebug("**fieldspair.second: |%s|",qPrintable(SafetYAWL::fieldsvalue));

    QString localfieldsvalue = SafetYAWL::fieldsvalue;
    if ( localfieldsvalue.endsWith(",")) {
        localfieldsvalue.chop(1);
    }

    QString resultvalue;
    bool result = SafetYAWL::createTableSignDb(SafetYAWL::fieldsname,
                                               localfieldsvalue,
                                               strvalid.arg(value),
                                               resultvalue);
    if (!result) {
        return false;
    }

    qDebug("...resultvalue:%s",qPrintable(resultvalue));
    if ( resultvalue == "1") {
        return true;
    }

    return false;
}

QString DateTimeWidget::html() {
    QString result;

    QString mydesc;
    QString readonly;
    QStringList options = conf()["options"].toString().split(",");

    if (conf().contains("desc")) {
        mydesc = conf()["desc"].toString();
        SYD << tr("..........CmdWidget::html...contiene desc:|%1|")
               .arg(mydesc);
    }

    foreach(QString option, options) {
        if (option.startsWith("type::")) {
            _typeinput = option.replace("::","=");
        }

        else if ( option == "readonly") {
            readonly = " readonly=\"readonly\" ";
        }
    }

    result = QString("<input %2 name=\"%1\" alt=\"Esto es un campo tipo texto\" id=\"%1\""
                      " %4 "
                     "   size=\"15\"  %3></input>")
            .arg(_caption)
            .arg(_typeinput)
            .arg(readonly)
            .arg(mydesc.isEmpty()?"":QString("placeholder=\"%1\"").arg(mydesc));



    bool isreadonly = conf()["options"].toString().indexOf(QRegExp("(,\\s+)?readonly(,\\s+)?")) >= 0;



    if (!isreadonly)  {
    result += QString(
                "\n\n"
                "<script  type=\"text/javascript\">\n"
                "$(function() {"
                "$( \"#%1\" ).datepicker({"
                "   showOtherMonths: true,"
                "   selectOtherMonths: true"
                "});"
                "$.datepicker.setDefaults($.datepicker.regional['es']);\n"
                "});"
                "</script>\n"
                )
            .arg(caption());
    }

    return result;

}

QString DateTimeWidget::htmlForKey(const QString& k, const QString& otherk) {

    QString result;

    QString beforef;
    if (!conf().contains("options")) {

        return result;

    }

    QString myoptions = conf()["options"].toString();
    QStringList mylist = myoptions.split(",");
    SYD << tr("..........................DateTimeWidget::htmlForKey....mylist.count():|%1|").arg(mylist.count());
    foreach(QString myo, mylist) {
        if (myo.startsWith("beforefunction:")) {
            beforef = myo;
            break;
        }
    }
    if (beforef.isEmpty()) {
        return result;
    }

    QStringList myformlist;
    if (conf().contains("formstring")) {
//        SYD << tr("..........................**DateTimeWidget::htmlForKey**.................(1)....");
        myformlist = conf()["formstring"].toStringList();
//        SYD << tr("..DateTimeWidget::htmlForKey........formstring:|%1|").arg(myformlist.join(";"));
        myformlist.append(QString("keyvalue0:%1").arg(k));
        myformlist.append(QString("keyvalue1:%1").arg(otherk));


        foreach(QString f, myformlist) {
            QString mypattern = QString("{#%1}").arg(f.split(":").at(0));

            if (f.split(":").count()>1) {
                QString myvalue = f.split(":").at(1);
                QDateTime mydate = QDateTime::fromString(myvalue,Safet::DateOnlyFormat);
                if (mydate.isValid() ) {
                    if ( _format == "time_t" ) {
                            myvalue = QString("%1").arg(mydate.toTime_t());
                    }
                }
                else {
                    mydate = QDateTime::fromString(myvalue,Safet::DateFormat);
                    if (mydate.isValid() ) {
                        if ( _format == "time_t" ) {
                                myvalue = QString("%1").arg(mydate.toTime_t());
                        }
                    }

                }

                beforef.replace(mypattern,myvalue);
            }
            else {
                beforef.replace(mypattern,"");
            }

        }

    }

       result = execBeforeFunction(beforef);




    SYD << tr("..........................DateTimeWidget::htmlForKey..................\n\n\n");



    return result;
}


QString DateTimeWidget::processInternalFunction(const QString& op) {

    QString result = op;
        QRegExp rx;

        rx.setPattern("bdays\\((\\d+);([\\d\\.\\+\\-\\*/]+)\\)");
        SYD << tr(".....DateTimeWidget::processInternalFunction.....(2)...pattern:|%1|").arg(rx.pattern());

        if (rx.indexIn(op) == -1) {
            return result;

        }
        SYD << tr(".....DateTimeWidget::isInternalFunction.....(3)...");
        bool ok;
        QString myzero = rx.cap(0);
        QString myfirst = rx.cap(1);
        QString mysecond = rx.cap(2);
        SYD << tr(".....DateTimeWidget::processInternalFunction....INTERNAL.myzero:|%1|").arg(myzero);
        SYD << tr(".....DateTimeWidget::processInternalFunction.....myfirst:|%1|").arg(myfirst);
        SYD << tr(".....DateTimeWidget::processInternalFunction.....mysecond:|%1|").arg(mysecond);
        uint myinit = myfirst.toInt(&ok);
        uint mydays = 0;
        int nargs;
        char** argv =NULL;

        QCoreApplication myapp(nargs,argv);
        QScriptEngine myEngine;

        try  {

           QScriptValue myvalue = myEngine.evaluate(mysecond);
                      QString currvalue = myvalue.toString();
           SYD << tr("....DateTimeWidget::processInternalFunction...currvalue:|%1|")
                  .arg(currvalue);
           if ( currvalue == "undefined") {
               SYW << tr("Sintaxis INCORRECTA en la funcion bdays: \"%1\"")
                      .arg(mysecond);
           }
           else {
            mydays = currvalue.toInt(&ok);
           }

        }
        catch(...) {
            SYE << tr("IMPORTANTE:Ocurrió un error (excepcion) al evaluar el script (two)");
        }

        SYD << tr("....DateTimeWidget::processInternalFunction...mydays:|%1|")
               .arg(mydays);

//        uint mydays = mysecond.toInt(&ok);
        QDate mydate = QDateTime::fromTime_t(myinit).date();
        QStringList mylist = SafetYAWL::getConf()["Functions/bdays.*"].split(SafetYAWL::LISTSEPARATORCHARACTER,QString::SkipEmptyParts);

        SYD << tr(".....DateTimeWidget::isInternalFunction.....(5)...mylist.count():|%1|").arg(mylist.count());
        int pos = 0;
        foreach(QString f, mylist) {
            SYD << tr(".....DateTimeWidget::isInternalFunction.....date:|%1|").arg(f);
            Q_ASSERT(pos < mylist.count());
            QDateTime myldate = QDateTime::fromString(f,Safet::DateOnlyFormat);
            if (myldate.isValid()) {
                myldate.setTime(QTime(0,0,0));
                QString newdate = QString("%1").arg(myldate.toTime_t());
                mylist.replace(pos, newdate);
                SYD << tr(".....DateTimeWidget::isInternalFunction.....replace newdate:|%1|").arg(newdate);
            }

            pos++;

        }

        SYD << tr(".....DateTimeWidget::isInternalFunction.....myinit:|%1|").arg(myinit);
        mydays--;
        while (mydays > 0) {
            mydate = mydate.addDays(1);

            QDateTime mytimet;
            mytimet.setDate(mydate);
            QString s = QString("%1").arg(mytimet.toTime_t());

            SYD << tr(".....DateTimeWidget::isInternalFunction.....current....time_t:|%1|").arg(s);
            if (mydate.dayOfWeek() == 6 || mydate.dayOfWeek() == 7 ) {
                if ( mylist.contains(s)) {
                    mydays--;
                }
            }
            else {
                if ( !mylist.contains(s)) {
                    mydays--;
                }

            }

        }


         result.replace(rx,QString("%1").arg(QDateTime(mydate).toTime_t()+3599));
         SYD << tr(".....DateTimeWidget::isInternalFunction.....(6)...result:|%1|").arg(result);

        return result;
}

QString DateTimeWidget::execBeforeFunction(const QString& op) {

    SYD << tr(".....DateTimeWidget::execBeforeFunction.....(1)...op:|%1|").arg(op);
    QString result;
    QString myop = op.mid(QString("beforefunction:").length());


    myop = processInternalFunction(myop);

    QString mysql = "SELECT "+ myop;
    QSqlQuery query( SafetYAWL::currentDb );
    query.prepare(  mysql );

    SYD << tr("..........................****DateTimeWidget::execBeforeFunction:|%1|")
           .arg(mysql);
      bool executed = query.exec();
      if (!executed ) {
           SYW << tr(" (before function) no se ejecutó correctamente "
                 "la sentencia SQL: \"%1\"")
                                   .arg(mysql);
           return result;
       }
      bool isnext = query.next();
      if ( !isnext ) {
       SYW << tr(" (before function) No hay registros para ejecutar "
              "la sentencia SQL: \"%1\"").arg(mysql);
       return result;
      }

      result = query.record().value(0).toString();

      if ( _format == "time_t") {
          bool ok;
          result = QDateTime::fromTime_t(result.toInt(&ok)).toString(Safet::DateOnlyFormat);
      }
      SYD << tr(".........DateTimeWidget::execBeforeFunction...result:|%1|")
             .arg(result);

      return result;



}

void DateTimeWidget::buildWidget() {
/*
     //CmdWidget::buildWidget();
     qDebug("...TextEditWidget...buildWidget...");
     //mainLayout = new QHBoxLayout;
     datetimeedit = new QDateTimeEdit();
     if ( conf().contains("options"))  {

         principalWidget = datetimeedit;
         setOptionsProperties(conf()["options"].toString().split(",")); // Colocar las propiedades

     }


     datetimeedit->setGeometry(0,0,230,30);

     datetimeedit->setDateTime(QDateTime::currentDateTime());

     if (isTextParent()) {
         okbutton = new QToolButton;
         okbutton->setGeometry(0,0,25,30);
         okbutton->setIcon(QIcon(":/yes.png"));
         quitbutton = new QToolButton;
         quitbutton->setGeometry(0,0,25,30);
         quitbutton->setText( "X");
     }
     mainLayout = new QHBoxLayout;
     mainLayout->addWidget(datetimeedit);
     QString mytip = tr("Seleccione la fecha del cuadro\nEscriba Ctrl+L para finalizar");
     if ( conf().contains("validation")) {
         QStringList mylist = conf()["validation"].toString().split("::");
         if (mylist.count() > 1 ) {
             QString usertip = mylist.at(1);
             mytip = usertip;
         }
     }
     _usertooltip = mytip;
     datetimeedit->setToolTip(mytip);
     mainLayout->setSpacing( 1 );
     if (isTextParent()) {
         mainLayout->addWidget(quitbutton);
         mainLayout->addWidget(okbutton);

         connect(okbutton, SIGNAL(clicked()), _texteditparent, SLOT(insertAndClose()) );
         connect(quitbutton, SIGNAL(clicked()), _texteditparent, SLOT(cancelAndClose()) );
     }

     setLayout(mainLayout);
*/
}

void DateTimeWidget::setFocus ( Qt::FocusReason reason ) {
/*     qDebug("...TextEditWidget::setFocus....(text)");
     QWidget::setFocus ( reason );
     datetimeedit->setFocus( reason);*/

}



QString DateTimeWidget::text() const {
    QString result;
//    if (datetimeedit != NULL )  {
//        QString pattern = "%1";
//        if ( _format.compare("time_t",Qt::CaseInsensitive) == 0 ) {
//            result = pattern.arg(datetimeedit->dateTime().toTime_t());
//        }
//        else {
//            result = datetimeedit->dateTime().toString(_format);
//        }
//    }
       return result;
 }
