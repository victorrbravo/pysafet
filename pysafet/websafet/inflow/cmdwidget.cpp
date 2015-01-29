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
    if (_typeinput == "type=file" && !readonly.isEmpty()) {


        result += QString("<input type=\"text\" name=\"%1\" id=\"%1\"    "
                         "onClick=\"window.open(this.value)\" size=\"80\" "

                         " readonly=\"readonly\" style=\"border: none;color: blue;text-decoration: underline;cursor: pointer;\"       "
                         "  value=\"sin seleccionar\" ></input>")
                .arg(_caption);
        result += QString("<input type=\"file\" name=\"%1\" id=\"%1\"    "
                         " size=\"55\" "                         
                          " %2 "
                         "    >\n"
                         "  </input>")
                .arg(_caption)
                .arg(mydesc.isEmpty()?"":QString("placeholder=\"%1\"").arg(mydesc));


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
        result += QString("<input %2 name=\"%1\" alt=\"Esto es un campo tipo texto\" id=\"%1\""
                          " %4 "
                         "   size=\"55\"  %3></input>")
                .arg(_caption)
                .arg(_typeinput)
                .arg(readonly)
                .arg(mydesc.isEmpty()?"":QString("placeholder=\"%1\"").arg(mydesc));

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
//     lineedit = new QLineEdit;

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


