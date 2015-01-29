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
/*    if (!isWiki()){
        return;
    }
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QTextEdit *textEdit;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;

    gridLayout_2 = new QGridLayout(this);
    gridLayout_2->setContentsMargins(0, 0, 0, 0);
    gridLayout_2->setHorizontalSpacing(0);
    gridLayout_2->setVerticalSpacing(0);
    gridLayout = new QGridLayout();
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(0);
    boldbutton = new QToolButton;

    horizontalLayout->addWidget(boldbutton);

    italicbutton = new QToolButton;

    horizontalLayout->addWidget(italicbutton);

    underbutton = new QToolButton;

    horizontalLayout->addWidget(underbutton);


    gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

    textedit = new QTextEdit();

    connect(textedit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
                this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));

    gridLayout->addWidget(textedit, 1, 0, 1, 2);


    gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    if ( isTextParent()) {
        okbutton = new QToolButton(this);
        verticalLayout->addWidget(okbutton);
        quitbutton = new QToolButton;
        verticalLayout->addWidget(quitbutton);
    }

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    verticalLayout->addItem(verticalSpacer);


    gridLayout_2->addLayout(verticalLayout, 0, 1, 1, 1);



//    textedit = new QTextEdit();
    textedit->setAcceptRichText(true);
    textedit->setGeometry(0,0,170,250);
//    QGridLayout *buttonLayout = new QGridLayout(this);


    actionTextBold = new QAction(QIcon(":/textbold.png"), tr("&Negrita"), this);
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setCheckable(true);

    actionTextItalic = new QAction(QIcon(":/textitalic.png"), tr("&Itálica"), this);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    actionTextItalic->setCheckable(true);

    actionTextUnder = new QAction(QIcon(":/textunder.png"), tr("&Subrayado"), this);
    actionTextUnder->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnder->setCheckable(true);

    //boldbutton->setIcon(QIcon(":/textbold.png"));
    boldbutton->setDefaultAction(actionTextBold);
    boldbutton->setMaximumSize(20,20);

    italicbutton->setMaximumSize(20,20);
    italicbutton->setDefaultAction(actionTextItalic);


    underbutton->setMaximumSize(20,20);
    underbutton->setDefaultAction(actionTextUnder);
//    italicbutton  = new QToolButton;

//    okbutton = new QToolButton;
    if ( isTextParent()) {
        okbutton->setIcon(QIcon(":/yes.png"));
        okbutton->setMaximumSize(20,20);


        quitbutton->setText( "X");
        quitbutton->setMaximumSize(20,20);

        connect(okbutton, SIGNAL(clicked()), _texteditparent, SLOT(insertAndClose()) );
        connect(quitbutton, SIGNAL(clicked()), _texteditparent, SLOT(cancelAndClose()) );
    }
    //connect(boldbutton, SIGNAL(clicked()), this, SLOT(setBold()) );
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(setBold()));
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(setItalic()));
    connect(actionTextUnder, SIGNAL(triggered()), this, SLOT(setUnder()));
    setLayout(gridLayout_2);
//    setLayout(buttonLayout);
*/
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
    result = QString("<textarea name=\"%1\" id=\"%1\" COLS=45 ROWS=5></textarea>")
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
/*     //CmdWidget::buildWidget();
     qDebug("...TextEditWidget...buildWidget...");
     //mainLayout = new QHBoxLayout;
     QStringList options = conf()["options"].toString().split(",");
     if ( options.count() > 0 && options.at(0) == "wiki" ) {
         _iswiki = true;
         buildWikiWidget();
         return;
     }
     mainLayout = NULL;
     textedit = new QTextEdit();
     textedit->setGeometry(0,0,170,250);
     QString mytip = tr("Campo de Texto Largo. Escriba Ctrl+L para finalizar");
     if ( conf().contains("validation")) {
         QStringList mylist = conf()["validation"].toString().split("::");
         if (mylist.count() > 1 ) {
             QString usertip = mylist.at(1);
             mytip = usertip;
         }
     }
     _usertooltip = mytip;
     textedit->setToolTip(mytip);

     QGridLayout *buttonLayout = new QGridLayout(this);
     okbutton = new QToolButton;
     okbutton->setIcon(QIcon(":/yes.png"));
     okbutton->setMaximumSize(20,20);
     quitbutton = new QToolButton;
     quitbutton->setText( "X");
     quitbutton->setMaximumSize(20,20);
     buttonLayout->addWidget(textedit,0,0,2,1,Qt::AlignJustify);
     buttonLayout->addWidget(okbutton,0,1);
     buttonLayout->addWidget(quitbutton,1,1);
     connect(okbutton, SIGNAL(clicked()), _texteditparent, SLOT(insertAndClose()) );
     connect(quitbutton, SIGNAL(clicked()), _texteditparent, SLOT(cancelAndClose()) );

     setLayout(buttonLayout);*/
}

void TextEditWidget::setFocus ( Qt::FocusReason reason ) {
/*
     Q_CHECK_PTR( textedit );
     QWidget::setFocus ( reason );
     textedit->setFocus( reason);
*/

}
