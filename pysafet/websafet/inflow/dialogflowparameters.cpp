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

#include "dialogflowparameters.h"
#include "ui_dialogflowparameters.h"
#include "dommodel.h"
#include "getfilewidget.h"
#include "datetimewidget.h"
#include "combowidget.h"
#include "listwidget.h"
#include "numberwidget.h"
#include "texteditwidget.h"
#include "suggestwidget.h"


#include <QLabel>
#include <QLineEdit>

DialogFlowParameters::DialogFlowParameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFlowParameters)
{
    ui->setupUi(this);
    boxlayout = NULL;
}

DialogFlowParameters::~DialogFlowParameters()
{
    delete ui;
}


void DialogFlowParameters::layoutParameters() {

    qDebug("...DialogFlowParameters...(1)...");
    qDebug("....parameters.count():%d", parameters().count());
    qDebug("...DialogFlowParameters...(2)...");
    boxlayout = new QGridLayout(ui->boxLista);


    for(int i =0; i < parameters().count();i++) {
        QLabel *mylabel = new QLabel(this);
        SafetParameter *par = parameters().at(i);
        Q_CHECK_PTR(par);
        mylabel->setText("<b>"+par->title()+"</b>");
        mylabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        boxlayout->addWidget(mylabel,i,0,1,1);
        CmdWidget* mywidget = getWidget(par);
        boxlayout->addWidget(mywidget,i,1,1,1);
        _widgets.append(mywidget);
    }
}

CmdWidget* DialogFlowParameters::getWidget(SafetParameter* p) {
    QMap<QString,QVariant> myconf;
    CmdWidget* mywidget = NULL;
    Q_CHECK_PTR( p );

    if ( !p->options().isEmpty()) {
        myconf.insert("options",p->options());
    }
    if ( !p->path().isEmpty()) {
        myconf.insert("path",p->path());
    }
    if ( !p->filter().isEmpty()) {
        myconf.insert("filter",p->filter());
    }
    if ( !p->literal().isEmpty()) {
        myconf.insert("literal",p->literal());
    }
    if ( !p->format().isEmpty()) {
        myconf.insert("format",p->format());
    }
    if ( !p->order().isEmpty()) {
        myconf.insert("order",p->order());
    }
    if ( !p->validation().isEmpty()) {
        myconf.insert("validation",p->validation());
    }
    if ( !p->input().isEmpty()) {
        myconf.insert("input",p->input());
    }
    if ( !p->changefor().isEmpty()) {
        myconf.insert("changefor",p->changefor());
    }


    QString mytype = p->type();
    qDebug("...mytype:|%s|",qPrintable(mytype));
    DomModel::TypeField enumtype = DomModel::typeFieldFromString(mytype);
    qDebug("...enumtype:|%d|",enumtype);
    QRect myrect;

    int index = -1;
    QString defaultvalue;
    defaultvalue = p->defaultvalue();
    ComboWidget* mycombowidget = NULL;
    switch (enumtype) {
    case DomModel::FileName:
        mywidget = new GetFileWidget(p->title(),this,false);
        Q_CHECK_PTR( mywidget );
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        // mywidget->setValidator( validator);
        if (!defaultvalue.isEmpty()) {
            mywidget->setText(defaultvalue);
        }
        return mywidget;
    case DomModel::DateTime:
        qDebug(".....(1).............cargando Widget DATETIME");
        mywidget = new DateTimeWidget(p->title(), this,false);
        Q_CHECK_PTR( mywidget );
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        //mywidget->setValidator(validator);
        if (!defaultvalue.isEmpty()) {
            mywidget->setText(defaultvalue);
        }

        return mywidget;
    case DomModel::String:
        mywidget = new CmdWidget(p->title(), this,false);
        mywidget->setConf(myconf);
        Q_CHECK_PTR( mywidget );
        mywidget->buildWidget();
        //  mywidget->setValidator( validator);
        if (!defaultvalue.isEmpty()) {
            mywidget->setText(defaultvalue);
        }
        return mywidget;
    case DomModel::Suggest:
        mywidget = new SuggestWidget(p->title(), this,false);
        mywidget->setConf(myconf);
        Q_CHECK_PTR( mywidget );
        mywidget->buildWidget();
        // mywidget->setValidator( validator);
        if (!defaultvalue.isEmpty()) {
            mywidget->setText(defaultvalue);
        }

        return mywidget;

    case DomModel::Number:
        mywidget = new NumberWidget(p->title(), this, false);
        mywidget->setConf(myconf);
        Q_CHECK_PTR( mywidget );
        mywidget->buildWidget();
        // mywidget->setValidator( validator);
        if (!defaultvalue.isEmpty()) {
            mywidget->setText(defaultvalue);
        }

        return mywidget;
    case DomModel::StringBox:
        mywidget = new TextEditWidget(p->title(), this,false);
        Q_CHECK_PTR( mywidget );
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        // mywidget->setValidator( validator);
        if (!defaultvalue.isEmpty()) {
            mywidget->setText(defaultvalue);
        }

        return mywidget;
//    case DomModel::ComboAutofilter:
//        pathflow = searchPathFlow();

//        mywidget = new ComboWidget(p->title(),ComboWidget::AutofilterSafet, this);
//        Q_CHECK_PTR( mywidget );
//        if ( pathflow.length() > 0 ) {

//            myconf.insert("path", pathflow);
//        }
//        mywidget->setConf(myconf);
//        mywidget->buildWidget();
//        return mywidget;
//        break;
//    case DomModel::ComboRecursivefilter:
//        pathflow = searchPathFlow();

//        mywidget = new ComboWidget(p->title(),ComboWidget::RecursivefilterSafet, this);
//        Q_CHECK_PTR( mywidget );
//        if ( pathflow.length() > 0 ) {
//            myconf.insert("path", pathflow);
//        }
//        mywidget->setConf(myconf);
//        mywidget->buildWidget();
//        return mywidget;
//        break;

     case DomModel::ComboListTable:
        qDebug("....combotable.....(1)...");
        mywidget = new ComboWidget(p->title(),ComboWidget::ListTable, this,false);
        qDebug("....combotable.....(2)...");
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        qDebug("....combotable.....(3)...");

        mycombowidget = qobject_cast<ComboWidget*>(mywidget);
        Q_CHECK_PTR(mycombowidget);
        index = mycombowidget->combo()->findText(defaultvalue);
        if ( index >= 0 ) {
            mycombowidget->combo()->setCurrentIndex(index);
        }
        return mywidget;
     case DomModel::ComboListLiteral:
        mywidget = new ComboWidget(p->title(),ComboWidget::ListLiteral, this,false);
        mywidget->setConf(myconf);
        mywidget->buildWidget();

        mycombowidget = qobject_cast<ComboWidget*>(mywidget);
        Q_CHECK_PTR(mycombowidget);
        index = mycombowidget->combo()->findText(defaultvalue);
        if ( index >= 0 ) {
            mycombowidget->combo()->setCurrentIndex(index);
        }
        return mywidget;
     case DomModel::ComboColor:
        mywidget = new ComboWidget(p->title(),ComboWidget::ColorSafet, this,false);
        mywidget->setConf(myconf);
        mywidget->buildWidget();

        mycombowidget = qobject_cast<ComboWidget*>(mywidget);
        Q_CHECK_PTR(mycombowidget);
        index = mycombowidget->combo()->findText(defaultvalue);
        if ( index >= 0 ) {
            mycombowidget->combo()->setCurrentIndex(index);
        }

        return mywidget;
        break;
     case DomModel::ComboConffile:
     case DomModel::ListConffile:
        if (   enumtype  == DomModel::ComboConffile ) {
            mywidget = new ComboWidget(p->title(),ComboWidget::ConffileSafet, this,false);
        }
        else {
            mywidget = new ListWidget(p->title(),ListWidget::ConffileSafet, this);
            QRect myrect = mywidget->geometry();
            myrect.setHeight(300);
            mywidget->setGeometry(myrect);

        }

        qDebug("ListConffile...mywidget->setConf(myconf)...(1)...");
        mywidget->setConf(myconf);
        mywidget->buildWidget();

        qDebug("ListConffile...mywidget->setConf(myconf)...(2)...");
        return mywidget;
     case DomModel::ListTable:
        mywidget = new ListWidget(p->title(),ListWidget::ListTable, this,false);
        myrect = mywidget->geometry();
        myrect.setHeight(300);
        mywidget->setGeometry(myrect);
        mywidget->setConf(myconf);
        mywidget->buildWidget();        
        qDebug("ListTable...mywidget->setConf(myconf)...(2)...");
        return mywidget;
     case DomModel::ListLiteral:
        mywidget = new ListWidget(p->title(),ListWidget::ListLiteral, this,false);
        myrect = mywidget->geometry();
        myrect.setHeight(300);
        mywidget->setGeometry(myrect);
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        qDebug("ListLiteral...mywidget->setConf(myconf)...(2)...");
        return mywidget;
//     case DomModel::ComboFlow:
//        keyvalue = searchKeyValueOnTextEdit();
//        SafetYAWL::streamlog
//                << SafetLog::Debug
//                << tr("DOMMODEL ComboFlow: keyvalue: |%1|").arg(keyvalue);
//        myconf.insert("keyvalue", keyvalue);
//        mywidget = new ComboWidget(p->title(),ComboWidget::Flow, this);
//        mywidget->setConf(myconf);
//        mywidget->buildWidget();

        return mywidget;

        //   case Unknown:
        //         mywidget = checkDynWidget( attribute.nodeValue().simplified(), p->title(), this );
        //         if ( mywidget != NULL ) {
        //              myconf = tthis->findConf(mywidget->metaObject()->className());
        //              if ( !attributeMap.namedItem("options").isNull() ) {
        //                  myconf.insert("options", attributeMap.namedItem("options").nodeValue().simplified());
        //              }
        //              mywidget->setConf( myconf );
        //              mywidget->buildWidget();
        //              mywidget->setValidator(validator);
        //              return mywidget;
        //         }
        //         break;
        default:
            mywidget = NULL;
    }



    //mywidget = new CmdWidget(p->title(),this,false);
//    Q_CHECK_PTR( mywidget );
//    mywidget->setConf(myconf);
//    mywidget->buildWidget();
    return mywidget;
}

QStringList DialogFlowParameters::getValues() {
    QStringList result;


    foreach(CmdWidget* w, _widgets) {

        if ( QString(w->metaObject()->className()) == "ComboWidget" ) {
               ComboWidget* mycombo = qobject_cast<ComboWidget*>(w) ;
               Q_CHECK_PTR(mycombo);
               QString realvalue = mycombo->getRealValue(w->text());
               //qDebug("...getValues...realvalue:|%s|",qPrintable(realvalue));
               result.append(realvalue);
        }
        else {
            result.append(w->text());
        }
    }
    return result;
}
