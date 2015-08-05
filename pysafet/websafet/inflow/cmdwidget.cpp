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

#include "cmdwidget.h"
#include "SafetYAWL.h"
#include "mainwindow.h"
#include <QCryptographicHash>


CmdWidget::CmdWidget(const QString& t, QObject *parent,bool istextparent)
 : QObject(parent),
 _conf()
{


    _caption = t;    

    _mandatory = false;
    _validator = NULL;
    _ispassword = false;
    _typeinput = " type=\"text\" ";
    setFocus(Qt::ActiveWindowFocusReason);
    _options.append(QLatin1String(""));
}


    QString CmdWidget::typeInput() const
{

return _typeinput;
}


QString CmdWidget::html() {
    QString result;

    QString mydesc;
    QString readonly;
    QString curraction;
    QString fieldsaction;
    QStringList options = conf()["options"].toString().split(",");


    QStringList positions;
    QString myposition = "";
    if (conf().contains("position")) {
         myposition = conf()["position"].toString();
        positions = myposition.split(",",QString::SkipEmptyParts);
    }
    setPosition(myposition);



    SYD << tr(".................CmdWidget::html.............CMDOPTIONS.........options:|%1|")
           .arg(conf()["options"].toString());
    if (conf().contains("desc")) {
        mydesc = conf()["desc"].toString();
        SYD << tr("..........CmdWidget::html...contiene desc:|%1|")
               .arg(mydesc);
    }

    foreach(QString option, options) {
        if (option.startsWith("type::")) {
            _typeinput = option.replace("::","=");
        }
        else if (option.startsWith("action::")) {
            curraction = option.mid(QString("action::").length());

        }
        else if (option.startsWith("fieldsaction::")) {
            fieldsaction = option.mid(QString("fieldsaction::").length());

            fieldsaction.replace("_USERNAME", SafetYAWL::currentAuthUser());

        }

        else if ( option == "readonly") {
            readonly = " readonly=\"readonly\" ";            
        }
    }

    QString newcaption = caption();

    if (mandatory()) {
        newcaption += "<font color=\"red\">*</font>";
        SYD << tr("...NEWCAPTION:|%1|")
               .arg(newcaption);


    }

    if (_typeinput == "type=file" && !readonly.isEmpty()) {




        SYD << tr("................CmdWidget::html().......CMDWIDGET_POSITION:|%1|:|%2|")
               .arg(caption())
               .arg(position());

        QString myid = _caption;

        myid.replace(" ","_");


        if ( positions.count()  > 0  ) {

            SYD << tr("................CmdWidget::html().......*CMDWIDGET_POSITION:|%1|:|%2|")
                   .arg(caption())
                   .arg(position());


            int posaction = 0;
            QString poscol = "md-4";
            foreach(QString p, positions) {
                if (p == "open") {
                    posaction = 1;
                }
                else if (p == "close") {
                    posaction = 2;
                }
                else if (p.startsWith("md")) {
                    poscol = p;
                }
            }

            SYD << tr("GET_POSITIONS...posaction:|%1|").arg(posaction);
            SYD << tr("GET_POSITIONS...poscol:|%1|").arg(poscol);

            if (posaction == 1) {
                result += "\n<div class=\"row clearfix\">\n";
            }

            result += QString(""
                       "\n<div class=\"col-%3 column \">\n"
                              "<div class=\"content-img\">"
                                              "<div class=\"load-img-profile\">"
                                                  "<img src=\"\" id=\"vistaPrevia%2\"/>"
                                                  "<div class=\"img-content-loader\" data-preview=\"vistaPrevia%2\" >"
                                                  "<button class=\"btn btn-shoppr\" ><i class=\"fa fa-plus\"></i></button>"
                                                  "<input id=\"%1\" name=\"%1\" type=\"file\" /></div>"
                                              "</div>"
                                               "<div class=\"progress\">\n"
                                              "<div class=\"progress-bar\" id=\"progress-bar\" role=\"progressbar\"></div>"
                                                 "</div>\n"
                                              "<div class=\"btn btn-primary btn-file-load full-width\" data-preview=\"vistaPrevia%2\" ><span>Cargar foto</span><input id=\"%1\" name=\"%1\" type=\"file\" /></div>"
                                   "</div>\n"

                       "</div>\n")
                    .arg(_caption.replace("_"," "))
                    .arg(myid)
                    .arg(poscol);

            if (posaction == 2) {
                result += "</div>\n";
            }




        } else {


                    result +=
                   QString(""
                           "<div class=\"form-group\" >\n"
                           "<div class=\"content-img\">"
                                           "<div class=\"load-img-profile\">"
                                               "<img src=\"\" id=\"vistaPrevia%2\"/>"
                                               "<div class=\"img-content-loader\" data-preview=\"vistaPrevia%2\" >"
                                               "<button class=\"btn btn-shoppr\" ><i class=\"fa fa-plus\"></i></button>"
                                               "<input id=\"%1\" name=\"%1\" type=\"file\" /></div>"
                                           "</div>"
                                            "<div class=\"progress\">\n"
                                           "<div class=\"progress-bar\" id=\"progress-bar\" role=\"progressbar\"></div>"
                                              "</div>\n"
                                           "<div class=\"btn btn-primary btn-file-load full-width\" data-preview=\"vistaPrevia%2\" ><span>Cargar foto</span><input id=\"%1\" name=\"%1\" type=\"file\" /></div>"
                                       "</div>\n"
                           "</div>\n"
                           )
                   .arg(_caption.replace("_"," "))
                   .arg(myid);
        }


    }
    else if (_typeinput == "type=radio" || _typeinput == "type=checkbox") {
        SYD << tr("........CmdWidget::html...TypeRADIO....");

        QList<QPair<QString,QString> > myradios;

        foreach(QString option, options) {
            if (option.indexOf("::") != -1  && (option != "type::radio" && option != "type::checkbox")) {
                QStringList mylist = option.split(";",QString::SkipEmptyParts);
                foreach(QString myvalue, mylist) {
                    QString value = myvalue.section("::",-1);
                    QString desc = myvalue.section("::",-2,-2);
                    if (desc.isEmpty()) {
                        desc = value;
                    }
                    SYD << tr("........CmdWidget::html...TypeRADIO..value:|%1|...desc:|%2|")
                           .arg(value)
                           .arg(desc);
                    QPair<QString,QString> myradio(value,desc);
                    myradios.append(myradio);

                }


            }

        }
        QString radiohtml = "";
        QString mytype = "radio";
        if (_typeinput == "type=checkbox") {
            mytype = "checkbox";
        }


        SYD << tr("........CmdWidget::html...TYPERADIO....myradios.count():|%1|")
               .arg(myradios.count());
        for(int i=0; i < myradios.count(); i++) {
            QPair<QString,QString> myradio = myradios.at(i);
            QString myinput = "";

            if (_typeinput == "type=checkbox") {
                myinput = QString("<input type=\"checkbox\" name=\"%1\" id=\"%1_%2\" value=\"%2\">\n")
                                          .arg(caption())
                                          .arg(i+1);
            }
            else {
                myinput = QString("<input type=\"radio\" name=\"%1\" id=\"%1_%2\" value=\"%2\">\n")
                                          .arg(caption())
                                          .arg(myradio.first);

            }

            QString mystr = QString( ""
              "<label>\n"
                   "%1\n"
                   "%2\n"
              "</label>"
            )
             .arg(myinput)
             .arg(myradio.second);

            radiohtml += mystr;


       }

        if (_typeinput == "type=checkbox") {
            QString myinput = QString("\n<input id=\"%1\"  type=\"hidden\" value=\"False\" name=\"%1\">\n")
                    .arg(caption());

            radiohtml += myinput;

            for(int i=0; i < myradios.count(); i++) {
                    //QPair<QString,QString> myradio = myradios.at(i);
                    radiohtml += QString(""

                                 "<script>\n"
                                 " jQuery(document).ready(function($) {        \n"

                                 "  $(\"#%1_%2\").change(\n"
                                 "  function() "
                                 "  {\n"
                                 "  console.log(\"changeando...\");\n"
                                 "     if ( document.getElementById(\"%1_%2\").checked ) {\n"
                                          "  console.log(\"changeando...checked\");\n"
                                 "         document.getElementById(\"%1\").value = \"True\";\n"
                                 "      }\n"
                                 "      else {\n"
                                          "  console.log(\"changeando...no cheecked\");\n"
                                 "         document.getElementById(\"%1\").value = \"False\";\n"
                                 "      }\n"
                                 "  });\n"
                                 " });\n"
                                 "</script>")
                    .arg(caption())
                    .arg(i+1);

            }

        }


        SYD << tr("..........CmdWidget::html........RADIOHTML:|%1|")
               .arg(radiohtml);



        if ( positions.count()  > 0  ) {

            SYD << tr("................CmdWidget::html().......*CMDWIDGET_POSITION:|%1|:|%2|")
                   .arg(caption())
                   .arg(position());


            int posaction = 0;
            QString poscol = "md-4";
            bool removelabel = false;

            foreach(QString p, positions) {
                if (p == "open") {
                    posaction = 1;
                }
                else if (p == "close") {
                    posaction = 2;
                }
                else if (p == "open_and_close") {
                    posaction = 3;

                }
                else if (p.startsWith("removelabel")) {
                        removelabel = true;
                }
                else if (p.startsWith("md")) {
                    poscol = p;
                }
            }

            SYD << tr("GET_POSITIONS...posaction:|%1|").arg(posaction);
            SYD << tr("GET_POSITIONS...poscol:|%1|").arg(poscol);



                if (posaction == 1 || posaction == 3) {
                    result += "\n<div class=\"row clearfix\">\n";
                }

                 result += QString(""
                               "<div class=\"col-%1 column \">\n"
                                "%2\n"
                                "%3\n"
                               "</div>\n")
                            .arg(poscol)
                            .arg((removelabel?"":QString("<label  class=\"col-md-2 control-label\">%2</label>\n").arg(_caption.replace("_"," ")).arg(newcaption)))
                            .arg(radiohtml);


                if (posaction == 2 || posaction ==  3) {
                    result += "</div>\n";
                }


        } else {

                QString mycaption = newcaption.replace(QRegExp("_+")," ");
                QString mynumber = "1";
                if (mycaption.length() > 6) {
                    mynumber = "3";
                }

                if (mycaption.length() > 6) {
                    mynumber = "4";
                }
                result +=
                QString(""
                          "<div class=\"form-group\">\n"
                          "<label  class=\"col-md-%3 control-label\">%1</label>\n"
                        "<br/>"
                        " <div class=\"controls\">\n"
                          "%2"
                        "</div>"
                        "</div>"
                        )
                .arg(mycaption)
                .arg(radiohtml)
                .arg(mynumber);

        }


    }
    else if (_typeinput == "type=button") {

        QString formaction = QString("/intranet/defconsole:operacion:%1")
                .arg(curraction);
        QString myform;
        if (MainWindow::mymainwindow != NULL ) {
            SYD << tr("..........CmdWidget::html()........MYFORM(0)....myform:|%1|")
                   .arg(formaction);

            myform = MainWindow::mymainwindow->menuForm(formaction, true,fieldsaction);
            SYD << tr("..........CmdWidget::html()....(compare.1)....MYFORM(1)....myform.result:|%1|")
                   .arg(myform);
        }
        result = QString(""
                        "<script  type=\"text/javascript\">\n"

                         "function load%3 () {\n"
                         "var url = 'https://gestion.cenditel.gob.ve/intranet/consola:Listar_datos_general';\n"
                         "var form = $('%4');\n"
                         "$('body').append(form);\n"
                         "$(form).submit();\n"
                         "}\n"
                        "</script>\n"
                         "<button %1   onclick=\"load%3()\">%2</button>")
                        .arg(_typeinput)
                        .arg(_caption)
                .arg(_caption.replace(" ","_"))
                .arg(myform);

        SYD << tr("..........CmdWidget::html().....(compare.2)...MYFORM(2)....myform.result:|%1|")
               .arg(result);


    }
    else  {

        if ( positions.count()  > 0  ) {

            SYD << tr("................CmdWidget::html().......*CMDWIDGET_POSITION:|%1|:|%2|")
                   .arg(caption())
                   .arg(position());


            int posaction = 0;
            QString poscol = "md-4";
            bool removelabel = false;

            foreach(QString p, positions) {
                if (p == "open") {
                    posaction = 1;
                }
                else if (p == "close") {
                    posaction = 2;
                }
                else if (p == "open_and_close") {
                    posaction = 3;

                }
                else if (p.startsWith("removelabel")) {
                        removelabel = true;
                }
                else if (p.startsWith("md")) {
                    poscol = p;
                }
            }

            SYD << tr("GET_POSITIONS...posaction:|%1|").arg(posaction);
            SYD << tr("GET_POSITIONS...poscol:|%1|").arg(poscol);



                if (posaction == 1 || posaction == 3) {
                    result += "\n<div class=\"row clearfix\">\n";
                }

                 result += QString(""
                               "<div class=\"col-%4 column \">\n"
                                "%5\n"
                                "<input %2 class=\"form-control\" id=\"%1\" name=\"%1\" %3  >\n"
                               "</div>\n")
                            .arg(_caption)
                            .arg(_typeinput)
                            .arg(mydesc.isEmpty()?"":QString("placeholder=\"%1\"").arg(mydesc))
                            .arg(poscol)
                            .arg((removelabel?"":QString("<label for=\"%1\" class=\"col-md-2 control-label\">%2</label>\n").arg(_caption).arg(newcaption)));


                if (posaction == 2 || posaction ==  3) {
                    result += "</div>\n";
                }


        } else {

                result +=
                QString(""
                          "<div class=\"form-group\" >"
                          "<label for=\"%1\" class=\"col-md-1 control-label\">%4</label>"
                          "<input %2 class=\"form-control\" id=\"%1\" name=\"%1\" %3  >"
                        "</div>"
                        )
                .arg(_caption)
                .arg(_typeinput)
                .arg(mydesc.isEmpty()?"":QString("placeholder=\"%1\"").arg(mydesc))
                .arg(newcaption.replace("_"," "));

        }



    }

    return result;
}

QString CmdWidget::htmlForKey(const QString& k, const QString& otherk) {

    SYD << tr(".............CmdWidget::htmlForKey........htmlForKey(1)...k:|%1|")
           .arg(k);
    SYD << tr(".............CmdWidget::htmlForKey........htmlForKey(1)...otherk:|%1|")
           .arg(otherk);

    if ( conf().contains(QLatin1String("keyvalue"))) {
        return html();
    }

}



void CmdWidget::setText(const QString &newText)  {

}

void CmdWidget::setChangefor(const QString& c) {
    if (c.isEmpty()){
        SYE << tr("Se introdujo un campo vacío: \"%1\" para el widget")
               .arg(c);
        return;
    }
    _changefor = c.split(",",QString::SkipEmptyParts);


}

void CmdWidget::buildWidget() {

    if (conf().contains("mandatory")) {
        QString myvalue = conf()["mandatory"].toString();
        setMandatory(myvalue.compare("yes") == 0);
        SYD << tr("CmdWidget::html caption:|%2|...UISMANDATORY:|%1|").arg(myvalue)
               .arg(caption());

    }


}


void CmdWidget::doReturn() {

}

CmdWidget::~CmdWidget()
{
}

bool CmdWidget::isValid(QString& value) {
    if (value.isEmpty()) {
        SYW << tr("No se lleno el valor para un widget");
        return false;
    }

    if ( !conf().contains("validation")) {
        return true;
    }

    QString mypattern = conf()["validation"].toString().trimmed();


    if (mypattern.isEmpty()) {
        return true;
    }

    QRegExp rx;
    rx.setPattern(mypattern);
    QString myvalue = value.trimmed();

    SYD << tr("...CmdWidget::isValid...mypattern:|%1|....myvalue=:|%2|")
           .arg(mypattern)
           .arg(myvalue);

    bool result = rx.exactMatch(myvalue);
    return result;
}

void CmdWidget::insertAndClose() {

}

void CmdWidget::setOptionsProperties(const QStringList ps) {
//    if (principalWidget == NULL ) {
//        qDebug("...principalWidget == NULL...");
//        return;
//    }
//    foreach(QString p, ps){
//        QStringList  fieldproperty = p.split("::");
//        if ( fieldproperty.count() > 1 ) {
//            QString localproperty = fieldproperty.at(0);
//            QVariant value = fieldproperty.at(1);
//            bool result = principalWidget->setProperty(qPrintable(localproperty),value);
//            qDebug("      CmdWidget::setOptionsProperties...localproperty: %s", qPrintable(localproperty));
//            qDebug("      CmdWidget::setOptionsProperties...value: %s", qPrintable(value.toString()));
//            if (!result) {
//                qDebug("La propiedad de nombre \"%s\" no existe",qPrintable(localproperty));
//            }
//            else {
//                if (QString("echoMode") == localproperty &&
//                    QString("Password") == value.toString() ) {
//                    qDebug("isPassword");
//                    _ispassword = true;
//                }
//            }

//        }
//    }

}

void CmdWidget::cancelAndClose() {

}

QString CmdWidget::text() const {
    return QString("");
}

void CmdWidget::setFocus ( Qt::FocusReason reason ) {


/*     QWidget::setFocus ( reason );
     if ( lineedit != NULL ) {
          lineedit->setFocus( reason);
     }
  */   
}



void CmdWidget::changeLblValidator(const QString& text) {

//    QString mytext = text;
//    if ( validator() == NULL ) {
//        if ( !text.isEmpty()) {
//            lblvalidator->setText( "<b><font color=\"Green\">+</font></b>" );
//        }
//        else {
//            lblvalidator->setText( "<b><font color=\"Red\">-</font></b>" );
//        }
//    }
//    else  {
//     int pos = 0;
//     if ( validator()->validate(mytext, pos ) == QValidator::Acceptable ) {
//	lblvalidator->setText( "<b><font color=\"Green\">+</font></b>" );
//     }
//     else {
//	 lblvalidator->setText( "<b><font color=\"Red\">-</font></b>" );
//       }
//    }

	
}
QStringList CmdWidget::options() const {

    return _options;
}


