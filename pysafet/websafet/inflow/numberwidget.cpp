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
    QString mypreffix = caption().left(4);
    QString wprops;
    wprops = conf()["options"].toString().trimmed();

    if (wprops.isEmpty()) {
        wprops = "width: '250px', height: '25px', digits: 3, decimalDigits: 0, spinButtons: true,promptChar:' '";
    }
    else {
        wprops.replace("::",":");
    }

    result =  QString(""
                      "<script  type=\"text/javascript\">\n"
                      "$(function() {"
                      "$(\"#%1\").jqxNumberInput({ %2,promptChar: ' ' });\n"
                      "%3\n"
                      "  $('#%1').bind('valuechanged', function (event) {\n"
                      "  var value = event.args.value;\n"
                      "  $('#%1').jqxNumberInput('val',value);\n"
                      "  });\n"
                      "});\n"

                      "</script>\n"
                      "")
            .arg(caption())
            .arg(wprops);

//    result =         "<script  type=\"text/javascript\">\n";
//    result += QString("$(document).ready(function(){\n");
//    result += QString("$(\"#%1\").jqxNumberInput("
//            "{ width: '250px', height: '25px', symbol: 'Bs.',  spinButtons: true });")
//            .arg(caption());

//    result +=  "\n});";
//    result +=  "</script>\n";


//    result += QString("\n<input type=\"text\" name=\"%1\" alt=\"Esto es un campo numérico\" id=\"%1\""
//                     "size=\"55\" %2 ></input>\n")
//            .arg(caption())
//            .arg(QString("onChange=\"%1Change(this)\" ").arg(mypreffix));

        result += QString("\n<div style='margin-top: 3px;' id=\"%1\""
                         " %2 ></div>\n"
                          )
                .arg(caption())
                .arg("");
                //.arg(QString("onChange=\"%1Change(this)\" ").arg(mypreffix));


    SYD << tr("...........NumberWidget::html........changefor:|%1|")
           .arg(conf()["changefor"].toString());
    foreach(QString k, conf().keys()) {
        SYD << tr("...........NumberWidget::html........k:|%1|")
               .arg(conf()[k].toString());

    }

        result += QString(""
                    "\n"
                "<script>\n"                          
                "function %1Change(myobj) {\n"
    //              "alert(\"value:\"+myobj.value);\n"
                          "    $.post(\"%2/loaddata\",{ id:$(this).val(),\n"
                          "    op:document.getElementById(\"safetoperation\").value,\n"
                          "    modname:\"%3\",\n"
                          "    primary:myobj.id,\n"
                          "    formstring: genFormString(document.forms[0]),\n"
                          "   formkey: document.forms[0].elements[1].value},\n"
                          " function(data)"
                          " {\n"
                          "  mylist = data.split(\"<SAFETSEPARATOR/>\");\n"
                          "  htmlsep = false;\n"
                          "  if (mylist.length < 2) {\n"
                          "    mylist = data.split(\"<SAFETHTMLSEPARATOR/>\");\n"
                          "    htmlsep = true;\n "
                          "  }\n"
                          "  for(i=0; i < mylist.length;i++){\n"
                          "       myname  = \"\";\n"
                          "       myvalue = \"\"; \n"
                          "       myname = mylist[i].substr(0,mylist[i].indexOf(\":\"));\n"
                          "       if (myname.length == 0 ) {\n"
                          "               continue;\n"
                          "       }\n"
                          "       myvalue = mylist[i].substr(mylist[i].indexOf(\":\")+1);\n"
                          "       j= 0;\n"
                          "       for(j=0; j<myname.length;j++){\n"
                          "               if ( (myname.charCodeAt(j)!=32) && (myname.charCodeAt(j)!=13) && (myname.charCodeAt(j)!= 10) ) {\n"
                          "                       break;\n"
                          "               }\n"
                          "       }\n"
                          "       myname = myname.substr(j);\n"
                          "       j=myvalue.length-1;\n"
                          "       lastpos = myvalue.length;\n"
                          "       for(j=myvalue.length-1;j>0;j--){\n"
                          "               if (myvalue.charAt(j) != ' ') {\n"
                          "                       break;\n"
                          "               }\n"
                          "               lastpos = lastpos -1;\n"
                          "       }\n"
                          "       myvalue = myvalue.substr(0,lastpos);\n"
                          "       if (document.getElementById(myname)) {\n"
                          "          if (htmlsep == true ) {\n"
                          "               $(\"#\"+myname).html(myvalue);\n"
                          "               if ( $(\"#\"+myname).is(\"input\") ) {\n "
                          "                  document.getElementById(myname).value = myvalue; \n   "
                          "               }                                     \n "
                          "               else {                                 \n"
                          "                 $(\"#\"+myname).html(myvalue);\n     \n"
                          "               }                                      \n"
                          "          } else {"
                          "               document.getElementById(myname).value = myvalue;\n"
                          "          }\n"
                          "      }\n"
                          "  }\n"
                          " });\n"

                "}\n"
                "</script>\n"
                          "")
                .arg(mypreffix)
                .arg(MainWindow::mymainwindow->hostURL())
                .arg("deftrac");

    return result;
}


void NumberWidget::buildWidget() {
     qDebug("...NumberWidget...buildWidget...");

/*     spinboxedit = NULL;
     if ( conf().contains("options"))  {

         setOptionsProperties(conf()["options"].toString().split(","));
         if ( conf()["options"].toString().split(",").contains("decimal") ) {
             spinboxedit = new QDoubleSpinBox();
         }
         else {
            spinboxedit = new QSpinBox();
            (qobject_cast<QSpinBox*>(spinboxedit))->setRange(0,9999);
         }
     }
     else {
         spinboxedit = new QSpinBox();
         (qobject_cast<QSpinBox*>(spinboxedit))->setRange(0,9999);
     }

     principalWidget = spinboxedit;
     QString mytip = tr("Campo Numérico. Escriba un número entero o decimal");
     if ( conf().contains("validation")) {
         QStringList mylist = conf()["validation"].toString().split("::");
         if (mylist.count() > 1 ) {
             QString usertip = mylist.at(1);
             mytip = usertip;
         }
     }
     _usertooltip = mytip;
     spinboxedit->setToolTip(mytip);

     setOptionsProperties(conf()["options"].toString().split(",")); // Colocar las propiedades
     spinboxedit->setGeometry(0,0,350,30);



     if (isTextParent()) {
         okbutton = new QToolButton;
         okbutton->setGeometry(0,0,25,30);
         okbutton->setIcon(QIcon(":/yes.png"));
         quitbutton = new QToolButton;
         quitbutton->setGeometry(0,0,25,30);
         quitbutton->setText( "X");
     }
     mainLayout = new QHBoxLayout;
     mainLayout->addWidget(spinboxedit);
     //mainLayout->addWidget(lblvalidator);
     mainLayout->setSpacing( 1 );

     if ( isTextParent()) {
        mainLayout->addWidget(quitbutton);
        mainLayout->addWidget(okbutton);
        connect(okbutton, SIGNAL(clicked()), _texteditparent, SLOT(insertAndClose()) );
        connect(quitbutton, SIGNAL(clicked()), _texteditparent, SLOT(cancelAndClose()) );
    }
//     setLayout(mainLayout);
*/
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
