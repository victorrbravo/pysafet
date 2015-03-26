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
#include "numberwidget.h"
#include "SafetYAWL.h"
#include "mainwindow.h"

NumberWidget::NumberWidget(const QString& t, QObject *parent, bool istextparent )
    :CmdWidget(t,parent, istextparent) {



}

void NumberWidget::setText(const QString &newText) {


}

QString NumberWidget::htmlForKey(const QString& k, const QString& otherk) {

    html();

}



QString NumberWidget::html() {
    QString result;
//    QString mypreffix = caption().left(4);
    QString wprops;


    QString mydesc;

    if (conf().contains("desc")) {
        mydesc = conf()["desc"].toString();
    }


    wprops = conf()["options"].toString().trimmed();

    QStringList positions;
    QString myposition = "";
    if (conf().contains("position")) {
         myposition = conf()["position"].toString();
        positions = myposition.split(",",QString::SkipEmptyParts);
    }
    setPosition(myposition);



    if (wprops.isEmpty()) {
        wprops = "width: '250px', height: '25px', digits: 3, decimalDigits: 0, spinButtons: true,promptChar:' '";
    }
    else {
        wprops.replace("::",":");
    }

      result = "";

//    result =  QString(""
//                      "<script  type=\"text/javascript\">\n"
//                      "$(function() {"
//                      "$(\"#%1\").jqxNumberInput({ %2,promptChar: ' ' });\n"
//                      "%3\n"
//                      "  $('#%1').bind('valuechanged', function (event) {\n"
//                      "  var value = event.args.value;\n"
//                      "  $('#%1').jqxNumberInput('val',value);\n"
//                      "  });\n"
//                      "});\n"

//                      "</script>\n"
//                      "")
//            .arg(caption())
//            .arg(wprops);


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


    if (posaction == 1 || posaction == 3) {
        result += "\n<div class=\"row clearfix\">\n";
    }


    if (positions.count() == 0 ) {
            result += QString(""
                   "<br/>\n"
                  "<div class=\"form-group\">"
                      "%3"
                      "<div class=\"input-group\">"
                        "<div class=\"input-group-addon\">%1</div>"
                        "<input type=\"text\" class=\"form-control\" id=\"%1\" name=\"%1\" %2>"
                        "<div class=\"input-group-addon\">.00</div>"
                      "</div>"
                    "</div>"
                  )
                    .arg(caption())
                    .arg(mydesc.isEmpty()?"":QString("placeholder=\"%1\"").arg(mydesc))
                    .arg(removelabel?"":QString("<label class=\"sr-only\" for=\"%1\">%1</label>").arg(caption()));
    }
    else {
            result += QString(""
                              "<br/>\n"
                  "<div class=\"col-%4 column \">\n"
                  "%3"
                  "<div class=\"input-group\">"
                    "<div class=\"input-group-addon\">%1</div>"
                    "<input type=\"text\" class=\"form-control\" id=\"%1\" name=\"%1\"  %2 >"
                    "<div class=\"input-group-addon\">.00</div>"
                  "</div>"
                "</div>"
              )
                    .arg(caption())
                    .arg(mydesc.isEmpty()?"":QString("placeholder=\"%1\"").arg(mydesc))
                    .arg(removelabel?"":QString("<label class=\"sr-only\" for=\"%1\">%1</label>").arg(caption()))
                    .arg(poscol);


    }

    if (posaction == 2 || posaction ==  3) {
        result += "</div>\n";
    }


    SYD << tr("...........NumberWidget::html........changefor:|%1|")
           .arg(conf()["changefor"].toString());
    foreach(QString k, conf().keys()) {
        SYD << tr("...........NumberWidget::html........k:|%1|")
               .arg(conf()[k].toString());

    }



    return result;
}


void NumberWidget::buildWidget() {
   CmdWidget::buildWidget();

}

void NumberWidget::setFocus ( Qt::FocusReason reason ) {
/*     qDebug("...NumberWidget::setFocus....(text)");
     QWidget::setFocus ( reason );
     spinboxedit->setFocus( reason);*/

}
bool NumberWidget::isValid(QString& value) {
    bool okint, okfloat;
    QString newvalue = value;

    newvalue.replace(".","");
    newvalue.replace(",",".");

    SYD << tr("....NumberWidget...validating...NUMBERWIDGET...newvalue:|%1|")
           .arg(newvalue);

    int vi = newvalue.toInt(&okint);
    float vf = newvalue.toDouble(&okfloat);

    return okint || okfloat;

}


QString NumberWidget::text() const {
    QString result;
       return result;
 }
