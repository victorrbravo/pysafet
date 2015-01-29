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
#include "outputwidget.h"
#include "SafetYAWL.h"
#include "mainwindow.h"


OutputWidget::OutputWidget(const QString& t, QObject *parent,bool istextparent)
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

void OutputWidget::setText(const QString &newText) {


}



bool OutputWidget::checkConfs() {
    if (conf().contains("path")) {
        _path = conf()["path"].toString();
        if (!QFile::exists(_path)) {
            SYW << tr("La ruta \"%1\" que fue colocada no se encuentra en el sistema ")
                   .arg(_path);
            return false;
        }
    }
    SYD << tr(".........OutputWidget::updateInfo...mypath:|%1|")
           .arg(_path);


    if (conf().contains("options")) {
        _options = conf()["options"].toString().split(",",QString::SkipEmptyParts);
    }


              SYD << tr("........OutputWidget::checkConfs...._options.count():|%1|")
                     .arg(_options.count());

    if (_options.count() == 0) {
        SYW << tr("El campo \"options\" se encuentra vacío");
        return false;
    }

    QString CONSTVARIABLE = "variable::";
    QString CONSTPARAMETERS = "parameters::";
    foreach(QString o, _options) {
        if (o.startsWith(CONSTVARIABLE)) {
            _variable = o.mid(CONSTVARIABLE.length());
        }
        else if (o.startsWith(CONSTPARAMETERS)) {
            _parameters = o.replace("::",".");
            SYD << tr("........OutputWidget::checkConfs....parameters:|%1|")
                   .arg(_parameters);


        }
    }
    if (_variable.isEmpty()) {
        SYW << tr("No ha colocado el nombre de la variable a listar en el formato variable::xxx");
        return false;
    }


    SYD << tr(".........OutputWidget::updateInfo..._variable:|%1|")
           .arg(_variable);

        if (conf().contains("changefor")) {
            QString cf = conf()["changefor"].toString();
            _changefor = cf;
        }

            SYD << tr(".........OutputWidget::updateInfo..._changefor:|%1|")
                   .arg(_changefor);






    return true;


}

bool  OutputWidget::updateInfo() {

    if (MainWindow::mymainwindow == NULL ) {
        SYW << tr("No fue posible actualizar la información del \"OutputWidget\", el sistema principal no está "
                  "cargado en memoria");
        return false;
    }

    bool isempty = false;

    if (!_parameters.isEmpty() ) {
        if (_parameters.endsWith(":")) {
            isempty = true;
        }
        if (_parameters.indexOf(":") == -1) {


            _parameters = "";
            //_variable = "vNinguno";
        }
    }


    QString action;
    action = QString("operacion:Listar_datos  Cargar_archivo_flujo:%1  Variable:%2 ")
            .arg(_path)
            .arg(_variable);


    SYD << tr("........***OutputWidget::updateInfo............_PARAMETERS:|%1|")
           .arg(_parameters);
            action += _parameters;


    SYD << tr("........***OutputWidget::updateInfo............(before)...action:|%1|")
           .arg(action);
    if ( !MainWindow::mymainwindow->toInputConsole(action,false) ) {
        SYW << tr(" No se ejecutó correctamente la actualización de la información del widget \"OutputWidget\"");
          return false;
    }

        SYD << tr("........OutputWidget::updateInfo............(after)...");

       _currentjson =MainWindow::mymainwindow->currentJSON();


        _currentdata = MainWindow::mymainwindow->currentADATA();


        if (isempty) {
            _currentdata = "[]";
        }


        SYD << tr(".........OutputWidget::updateInfo...OUTPUTINFO (currentdata):\n\n%1\n\n")
           .arg(_currentdata);



    return true;

}


bool OutputWidget::isValid(QString& value) {
    QString strvalid;
    if ( conf().contains("validation"))  {
        strvalid = conf()["validation"].toString().split("::").at(0);
    }

    if (strvalid.isEmpty()) {
        return true;
    }



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


    if ( resultvalue == "1") {
        return true;
    }

    return false;
}

QString OutputWidget::html() {
    QString result;
    //result = CmdWidget::html();


    if (!checkConfs()) {
        SYW << tr("No se actualizó la información del widget \"OutputWidget\"");
        return false;
    }



        updateInfo();

    bool isreadonly = conf()["options"].toString().indexOf(QRegExp("(,\\s+)?readonly(,\\s+)?")) >= 0;






    if (!isreadonly)  {

        if ( _parameters.isEmpty()) {
            SYD << tr("........OutputWidget::html...(before)...CURRENTJSON:|%1|")
                   .arg(_currentjson);
//            _currentjson.replace(QRegExp("safetlist\\s*=(.+)\\]\\s*;"),"safetlist = [];\n");

            _currentjson.replace(QRegExp(" \"tasks\"\\s*:\\s*\\[(.+)\\](.*)\\};"),"\"tasks\" : []\n};");

            SYD << tr("........OutputWidget::html...(after)...CURRENTJSON:|%1|")
                   .arg(_currentjson);

        }
       result += QString(


                "\n\n"
                "<script  type=\"text/javascript\">\n"
                "%1\n"
                "</script>\n"
                "\n\n<div id=\"complex\"></div>\n\n"
                   "<table cellpadding=\"0\" cellspacing=\"0\" border=\"0\" class=\"display\" name=\"safettable0\" id=\"safettable0\"></table>\n"
                   "<style type=\"text/css\">\n"
                   ".svgdiv { border: 1px solid #3c8243; }\n"
                   "#svgintro { float: right; width: 150px; height: 150px; margin-right: 30px; background: #fff; border: 1px solid #3c8243; }\n"
                   ".svgsample { float: left; width: 46%; margin: 1%; overflow: scroll; border: 1px solid #3c8243; padding: 5px; }\n"
                   ".drawOpt { float: left; width: 25%; }\n"
                   ".row { clear: both; }\n"
                   "#showMods { padding: 0em 0.25em; background-color: #ddffe8; color: #000; border: 1px solid #3c8243; font-size: 80%; text-decoration: none; cursor: pointer; }\n"
                   "#domMods { display: none; padding: 0em 1em 0.5em; background-color: #ddffe8; border: 1px solid #3c8243; }\n"
                   "</style>\n"
                   "<script type=\"text/javascript\" src=\"http://gestion.cenditel.gob.ve/media/jquery-latest.js\"></script> \n"
                   "<script type=\"text/javascript\" language=\"javascript\" src=\"http://gestion.cenditel.gob.ve/media/js/DataTables/media/js/jquery.dataTables.js\"></script>\n"
                   "<script type=\"text/javascript\" src=\"http://gestion.cenditel.gob.ve/media/jquery.svg.js\"></script>\n"
                   "<script type=\"text/javascript\" src=\"http://gestion.cenditel.gob.ve/media/jquery.debug.js\"></script>\n"
                   "<script type=\"text/javascript\" src=\"http://gestion.cenditel.gob.ve/media/jquery.svggraph.js\"></script>\n"
                   "<script type=\"text/javascript\" src=\"http://gestion.cenditel.gob.ve/media/jquery.svgplot.js\"></script>\n"
                   "<script type=\"text/javascript\" src=\"http://gestion.cenditel.gob.ve/media/jquery.svganim.js\"></script>\n"
                   "<script type=\"text/javascript\" src=\"http://gestion.cenditel.gob.ve/media/jquery.svgdom.js\"></script>\n"
                   "<script type=\"text/javascript\" src=\"http://gestion.cenditel.gob.ve/media/jquery.svgfilter.js\"></script>\n"
                   "<link type=\"text/css\" href=\"http://gestion.cenditel.gob.ve/media/jquery.svg.css\" rel=\"Stylesheet\" />\n"
                "<script  type=\"text/javascript\">\n"
                   "var oTable = null;\n"
                "safetproccessData = function() {\n"
                   "   oTable =    $('#safettable0').dataTable( {\n"
                   "\"aaData\": safetjson.tasks,\n"
                   "\"bDestroy\" : true,\n"
                   "    \"aoColumns\": safetjson.columns,\n"
                   "\"bPaginate\": false,\n"
                   "\"bProcessing\": true,\n"
                   "\"fnFooterCallback\": function ( nRow, aaData, iStart, iEnd, aiDisplay ) {}\n"
                   "});      \n"

                "};\n"

                "safetproccessData();\n"

                "</script>\n"
                )
            .arg(_currentjson);
    }

    return result;

}

QString OutputWidget::htmlForKey(const QString& k, const QString& otherk) {

    QString result;

    checkConfs();


    if (!_parameters.isEmpty() )  {
        _parameters = _parameters + ":" +k;
    }



    SYD << tr(".........................OutputWidget::htmlForKey...._parameters:|%1|")
           .arg(_parameters);

    SYD << tr(".........................OutputWidget::htmlForKey..................key:|%1|")
           .arg(k);


    SYD << tr(".........................OutputWidget::htmlForKey..................otherk:|%1|")
           .arg(otherk);

    updateInfo();


//    result = QString("myDataTable = new YAHOO.widget.DataTable(\"complex\", safetcolumns, "
//             "  new YAHOO.util.DataSource(%1), myConfigs);")
//            .arg(_currentdata);

    result = _currentdata;


    return result;
}


QString OutputWidget::processInternalFunction(const QString& op) {

    QString result = op;
    QRegExp rx;

    rx.setPattern("bdays\\((\\d+);(\\d+)\\)");
    SYD << tr(".....OutputWidget::isInternalFunction.....(2)...pattern:|%1|").arg(rx.pattern());

    if (rx.indexIn(op) == -1) {
        return result;

    }
    SYD << tr(".....OutputWidget::isInternalFunction.....(3)...");
    bool ok;
    QString myzero = rx.cap(0);
    QString myfirst = rx.cap(1);
    QString mysecond = rx.cap(2);
    SYD << tr(".....OutputWidget::isInternalFunction.....myzero:|%1|").arg(myzero);
    SYD << tr(".....OutputWidget::isInternalFunction.....myfirst:|%1|").arg(myfirst);
    SYD << tr(".....OutputWidget::isInternalFunction.....mysecond:|%1|").arg(mysecond);
    uint myinit = myfirst.toInt(&ok);
    uint mydays = mysecond.toInt(&ok);
    QDate mydate = QDateTime::fromTime_t(myinit).date();
    QStringList mylist = SafetYAWL::getConf()["Functions/bdays.*"].split(SafetYAWL::LISTSEPARATORCHARACTER,QString::SkipEmptyParts);

    SYD << tr(".....OutputWidget::isInternalFunction.....(5)...mylist.count():|%1|").arg(mylist.count());
    foreach(QString f, mylist) {
        SYD << tr(".....OutputWidget::isInternalFunction.....date:|%1|").arg(f);
    }

    SYD << tr(".....OutputWidget::isInternalFunction.....myinit:|%1|").arg(myinit);
    while (mydays > 0) {
        mydate = mydate.addDays(1);

        QDateTime mytimet;
        mytimet.setDate(mydate);
        QString s = QString("%1").arg(mytimet.toTime_t());

        SYD << tr(".....OutputWidget::isInternalFunction.....current....time_t:|%1|").arg(s);
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


     result.replace(rx,QString("%1").arg(QDateTime(mydate).toTime_t()));
     SYD << tr(".....OutputWidget::isInternalFunction.....(6)...result:|%1|").arg(result);

    return result;
}

QString OutputWidget::execBeforeFunction(const QString& op) {

    SYD << tr(".....OutputWidget::execBeforeFunction.....(1)...op:|%1|").arg(op);
    QString result;
    QString myop = op.mid(QString("beforefunction:").length());


    myop = processInternalFunction(myop);

    QString mysql = "SELECT "+ myop;
    QSqlQuery query( SafetYAWL::currentDb );
    query.prepare(  mysql );

    SYD << tr(".........OutputWidget::execBeforeFunction:|%1|")
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
      SYD << tr(".........OutputWidget::execBeforeFunction...result:|%1|")
             .arg(result);

      return result;



}

void OutputWidget::buildWidget() {
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

void OutputWidget::setFocus ( Qt::FocusReason reason ) {
/*     qDebug("...TextEditWidget::setFocus....(text)");
     QWidget::setFocus ( reason );
     datetimeedit->setFocus( reason);*/

}



QString OutputWidget::text() const {
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
