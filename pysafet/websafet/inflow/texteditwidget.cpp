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
#include "texteditwidget.h"
#include "SafetYAWL.h"

TextEditWidget::TextEditWidget(const QString& t, QObject *parent, bool istextparent )
    :CmdWidget(t,parent, istextparent) {
//     textedit = NULL;
     _iswiki = false;
//     boldbutton = NULL;
//     italicbutton = NULL;
//     actionTextBold = NULL;
//     actionTextItalic = NULL;
//     actionTextUnder = NULL;
}




void TextEditWidget::setText(const QString &newText) {

}


void TextEditWidget::buildWikiWidget() {

}


void TextEditWidget::insertAndClose() {
/*    if ( _texteditparent ) {
        if ( textedit) {
            QString value = textedit->toHtml();
            _texteditparent->insertPlainText(value.toLatin1());
            _texteditparent->insertPlainText("\n");
        }
    }

    close();
*/
}
QString TextEditWidget::html() {
    QString result;
    QString mydesc;

    if (conf().contains("desc")) {
        mydesc = conf()["desc"].toString();
    }

    QString myposition = "";
    QStringList positions;

    if (conf().contains("position")) {
         myposition = conf()["position"].toString();
            positions = myposition.split(",",QString::SkipEmptyParts);
    }
    setPosition(myposition);

    bool removelabel = false;

    foreach(QString p, positions) {
        if (p == "removelabel") {
            removelabel = true;
        }
    }

    QString newcaption = caption();

    if (mandatory()) {
        newcaption += "<font color=\"red\">*</font>";
        SYD << tr("...NEWCAPTION:|%1|")
               .arg(newcaption);


    }



    result =
       QString(""

                 "<div class=\"form-group\" >\n"
                         "%3\n"
                        "<textarea name=\"%1\" id=\"%1\" rows=\"3\" class=\"form-control\" %2 ></textarea>\n"
               "<span id=\"count%1\"  name=\"count%1\"></span>"
              // "<span class=\"glyphicon glyphicon-picture form-control-feedback\" aria-hidden=\"true\"></span>"
               "</div>\n"
               )
            .arg(caption())
            .arg(mydesc.isEmpty()?"":QString("placeholder=\"%1\"").arg(mydesc))
            .arg((removelabel?"":QString("<label for=\"%1\" class=\"col-md-2 control-label\">%2</label>\n").arg(caption()).arg(newcaption)) );

      result += QString(""
                      "<script>"
                      "$(\"#%1\").keyup(function(){\n"
                         "myleft = 140-$(this).val().length;\n"
                         "mytext = " "+myleft;\n"
                        "$(\"#count%1\").html(mytext);\n"
                         "});"
                      "</script>"
                      )
            .arg(caption());


    return result;
}

QString TextEditWidget::toPlainText() const {

}
QString TextEditWidget::toHtml() const {



}



QString TextEditWidget::wikiToHtml(const QString& s) {
    QString result;


    QString leftbold = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.leftbold"];
    QString rightbold = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.rightbold"];

    QString leftitalic = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.leftitalic"];
    QString rightitalic = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.rightitalic"];

    QString leftunder = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.leftunderline"];
    QString rightunder = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.rightunderline"];


    QString para;
    para = s;

    int pos = 0;
    int from = 0;
    while (true ) {
        QRegExp rxbold;
        QString mypattern = leftbold+QString("([%1]+)")
                            .arg(SafetYAWL::REWIKI)+rightbold;
        rxbold.setPattern(mypattern);
        pos = para.indexOf(rxbold,from);
        if ( pos == -1 ) {
            result  += para.mid(from);
            break;
        }
        result  += para.mid(from,pos-from);
        result  += "<span style=\" font-weight:600;\">";
        result  += rxbold.cap(1);
        result  += "</span>";

        from = pos + rxbold.matchedLength();
    }

    pos = 0;
    from = 0;
    para = result;
    qDebug("...para: %s", qPrintable(para));
    result = "";
    while (true ) {
        QRegExp rx;
        QString mypattern = leftitalic+QString("([%1]+)")
                            .arg(SafetYAWL::REWIKI)+rightitalic;
        rx.setPattern(mypattern);
        pos = para.indexOf(rx,from);
        if ( pos == -1 ) {
            result  += para.mid(from);
            break;
        }
        result  += para.mid(from,pos-from);
        result  += "<span style=\" font-style:italic;\">";
        result  += rx.cap(1);
        result  += "</span>";

        from = pos + rx.matchedLength();
    }
    pos = 0;
    from = 0;
    para = result;
    result = "";
    qDebug("...para: %s", qPrintable(para));
    while (true ) {
        QRegExp rx;
        QString mypattern = leftunder+QString("([%1]+)").arg(SafetYAWL::REWIKI)+rightunder;
        rx.setPattern(mypattern);
        pos = para.indexOf(rx,from);
        if ( pos == -1 ) {
            result  += para.mid(from);
            break;
        }
        result  += para.mid(from,pos-from);
        result  += "<span style=\" text-decoration: underline;\">";
        result  += rx.cap(1);
        result  += "</span>";

        from = pos + rx.matchedLength();
    }

    qDebug("...wikiToHtml...result...:%s",qPrintable(result));
    return result;


}

QString TextEditWidget::htmlToWiki(const QString& s) {
    QString result;
    QRegExp rxparagraph;

    QString leftbold = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.leftbold"];
    QString rightbold = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.rightbold"];

    QString leftitalic = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.leftitalic"];
    QString rightitalic = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.rightitalic"];

    QString leftunder = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.leftunderline"];
    QString rightunder = SafetYAWL::getConf()["Widgets/texteditwidget.wiki.rightunderline"];

    qDebug(".......leftbold...:|%s|",qPrintable(leftbold));

    rxparagraph.setPattern("<p.*>(.*)</p>");
    int pos = rxparagraph.indexIn(s);
    if (pos == -1 ) {
        return result;
    }
    QString para = rxparagraph.cap(1);
    int from = 0;
    while (true ) {
        QRegExp rxbold;
        rxbold.setPattern(QString("<span([\"a-zA-Z0-9\\-\\(\\),;\\:#=\\.áéíóúñÑ\\s_¿\\?\\!]+)>"
                                  "([%1]+)</span>").arg(SafetYAWL::REWIKI));
        pos = para.indexOf(rxbold,from);
        if ( pos == -1 ) {
            result  += para.mid(from);
            break;
        }

        result  += para.mid(from,pos-from);
        QString mystyle = rxbold.cap(1);
        QString lefttag, righttag;

        if ( mystyle.indexOf("font-weight:600;") != -1 ) {
            lefttag  += leftbold;
            righttag  += rightbold;
        }

        if ( mystyle.indexOf("font-style:italic;") != -1 ) {
              lefttag  = leftitalic + lefttag;
              righttag =  righttag + rightitalic ;
        }
        if ( mystyle.indexOf("text-decoration: underline;") != -1 ) {
              lefttag  = leftunder + lefttag;
              righttag  = righttag + rightunder;
        }

        result  += lefttag;
        result  += rxbold.cap(2);
        result  += righttag;

        from = pos + rxbold.matchedLength();

    }

    return result;
}




void TextEditWidget::setBold() {

}

void TextEditWidget::setItalic() {


}

void TextEditWidget::setUnder() {

}




void TextEditWidget::buildWidget() {
    CmdWidget::buildWidget();


}

void TextEditWidget::setFocus ( Qt::FocusReason reason ) {
/*
     Q_CHECK_PTR( textedit );
     QWidget::setFocus ( reason );
     textedit->setFocus( reason);
*/

}
