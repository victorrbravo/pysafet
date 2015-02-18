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

//#include <QtGui>
//#include <QComboBox>
//#include <QMessageBox>
//#include <QColorDialog>
#include "combowidget.h"
#include "mainwindow.h"
#include "SafetYAWL.h"

//#include "inflowfirmadoc.h"
//#include "ui_inflowfirmadoc.h"
#include "SafetBinaryRepo.h"
ComboWidget::ComboWidget(const QString& s, ComboWidget::Type t, QObject *parent,
                         bool istextparent)
    : CmdWidget(s, parent, istextparent)
{
    _oneoption = false;
//     varbox = NULL;
//     signbutton = NULL;
//     viewdocbutton = NULL;
//     getcolorbutton= NULL;
     _type = t;
     //configurator = new SafetYAWL();
     _currentgraphfile = Safet::GRAPHSFILENAME;
}

ComboWidget::~ComboWidget() {

     //if (configurator) delete configurator;
}

void ComboWidget::setText(const QString &newText) {
/*    qDebug("...ComboWidget::setText...newText:|%s|",qPrintable(newText));
    if (varbox ) {

        int newindex = varbox->findText(newText,Qt::MatchStartsWith);
        if (newindex != -1 ) {
            varbox->setCurrentIndex(newindex );
        }
    }
*/
}

void ComboWidget::updateCombo() {

    _filtercompleted = false;

    QStringList myoptions = conf()["options"].toString().split(",",QString::SkipEmptyParts);

    foreach(QString op, myoptions) {
        QString myfield = op.section("::",0,0).trimmed();
        if (myfield.compare("filtercompleted",Qt::CaseSensitive) == 0) {
            QString myvalue = op.section("::",1,1).trimmed();
            if (myvalue.compare("on",Qt::CaseSensitive) == 0 ) {
                _filtercompleted = true;
                break;
            }
        }
    }


    if ( conf().contains("input") ) {
        QString myinput = conf()["input"].toString();
        if (myinput != "yes" && myinput != "no") {
            setInput(myinput);
        }
    }
    SYD << tr("....ComboWidget::updateCombo()....type:|%1|...|%2|....|%3|").arg((int)type())
           .arg(VariableSafet).arg(AutofilterSafet);
     switch (type() ) {

          case VariableSafet:
               updateComboVariableSafet();
               break;
          case TaskSafet:
             updateComboTaskSafet();
               break;
          case AutofilterSafet:

              SYD << tr("........AutofilterSafet.....1");
              updateComboAutofilterSafet();
              break;
         case RecursivefilterSafet:
             updateComboRecursivefilterSafet();
           break;
         case ListTable:
               updateComboListTable();
               break;
         case ListBinaryRepo:
              updateComboListBinaryRepo();
               break;
          case ListLiteral:
               updateComboListLiteral();
               break;
          case Flow:
                SYD << tr("...........ComboWidget::isValid........Flow....UPDATECOMBO....");
               updateComboFlow();
               break;
     case ConffileSafet:
               updateComboConffileSafet();
               break;
     case GraphsSafet:
               updateComboGraphsSafet();
               break;

     case ColorSafet:
               updateComboColorSafet();
          default:;
     }

}

void ComboWidget::updateComboConffileSafet(bool inwidget) {
    QStringList options = conf()["options"].toString().split(":");
         if (options.count() < 1 ) {
         SafetYAWL::streamlog
                 << SafetLog::Warning
                 << tr("IMPORTANTE, No se definió correctamente el campo \"options\" --> \"%1\"").arg(conf()["options"].toString());
         return;
    }

    QString field = options.at(0);
    if (!field.endsWith(".*")) {
      SYD
                << tr("IMPORTANTE, Debe finalizar en \".*\". No se definió "
                      "correctamente el campo \"options\" --> \"%1\"").arg(conf()["options"].toString());

    }
    int count = 1;
    QString pattern = field.mid(0,field.length()-2)+QString(".%1");
    while (true) {
        QString newfield = pattern.arg(count);
        if ( !SafetYAWL::getAuthConf().contains(newfield)) {
            break;
        }
        _itemvaluelist.append(SafetYAWL::getAuthConf()[newfield]);

        count++;
    }
    if ( inwidget ) {
       //varbox->addItems( _itemvaluelist );
   }
    _options = _itemvaluelist;

}

void ComboWidget::updateComboGraphsSafet(bool inwidget) {

    QStringList options = conf()["options"].toString().split(";");

    foreach (QString o, options) {
        QStringList newfield = o.split("::",QString::SkipEmptyParts);
        if (newfield.count() == 2 ) {
            if (newfield.at(0) == "file") {
                if (newfield.at(1) == "planned") {
                    _currentgraphfile = Safet::PLANNEDGRAPHSFILENAME;
                }
            }

        }
    }

    SYD << tr("...ComboWidget::updateComboGraphsSafet..._currentgraphfile:|%1|")
           .arg(_currentgraphfile);
    if (MainWindow::mymainwindow == NULL ) {
        SYW <<
               tr("No se puede leer el combo de grafos, porque no existe un enlace a la pantalla"
                  "principal");
        return;
    }

    _itemvaluelist.clear();
    MainWindow::mymainwindow->doRestoreGraph(_currentgraphfile);

    SYD << tr("ComboWidget::updateComboGraphsSafet....begin");
    for(int i = 0; i < MainWindow::mymainwindow->graphs().keys().count();i++) {
        QString namegraph = MainWindow::mymainwindow->graphs().keys().at(i);

        namegraph.replace(QRegExp("\\s+")," ");
        SYD << tr("....*ComboWidget::updateComboGraphsSafet...append: |%1|")
               .arg(namegraph);


        if (!namegraph.isNull()) {
            _itemvaluelist.append(namegraph);
        }
    }

        SYD << tr("....*ComboWidget::updateComboGraphsSafet....intermediate");
    _options = _itemvaluelist;

    if (inwidget) {

    }
    SYD << tr("ComboWidget::updateComboGraphsSafet....end");

}

bool ComboWidget::isValid(QString& value) {

    SYD << tr("...........ComboWidget::isValid.....COMBOVALIDATION...value:|%1|")
           .arg(value);
    if (value.isEmpty() ) {
        return true;
    }

    bool nocheckvalid = true;
    // ***** Verificar la firma
    if (value.startsWith(tr("*FIRMAR Documento (")) ) {
            SYD << tr("....CmdWidget::isValid...is true (SIGN CASE)");
            return true;
    }

    if ( !CmdWidget::isValid(value)) {
        SYD << tr("....CmdWidget::isValid...is false");
        return false;
    }



    switch(type()) {
    case ListLiteral:
        if ( !conf().contains("options")) {
            
                  SYE  << tr("Falla la validacion del widget combo: \"%1\"")
                    .arg(caption());
            return false;
        }
        generateLiterals(conf()["options"].toString(),false);
                break;
    case ListTable:
        updateComboListTable(false);

        break;
    case ListBinaryRepo:
        updateComboListBinaryRepo(false);
        break;

    case VariableSafet:
        updateComboVariableSafet(false);
        break;
    case TaskSafet:        
        updateComboTaskSafet(false);
        return true;
        break;

    case AutofilterSafet:
        updateComboAutofilterSafet(false);        
        break;
   case RecursivefilterSafet:
        updateComboRecursivefilterSafet(false);
        break;
   case Flow:
        SYD << tr("...........ComboWidget::isValid........Flow....");
        if (nocheckvalid) {
            SYD << tr("...........ComboWidget::isValid........true value....");
            return true;
        }
        updateComboFlow(false);
         
        break;
     case ConffileSafet:
        updateComboConffileSafet(false);
        break;
     case ColorSafet:
        updateComboColorSafet(false);
        break;
     case GraphsSafet:
         updateComboGraphsSafet(false);
          return true;

        break;

    default:;
    }

    if ( type() == AutofilterSafet || type()==VariableSafet) {
        SYD << tr("ComboUpdate  QUITAR...QUITAR*** HACER VALIDACION CON AUTOFILTER Y VARIABLE");
        return true;
    }
    if ( type() == ListLiteral) {

        SYD << tr("ComboWidget...isValid...value:|%1|").arg(value);
        if ( !itemsValueList().contains(value)) {
            SYD << tr("ComboWidget...isValid...NO CONTAIN");
            return false;
        }
    }
    else if ( type() == Flow && oneoption() ) {
        QString myoptions = conf()["options"].toString();
        SYD << tr(".........***ComboWidget::isValid...options:|%1|").arg(myoptions);
        if (myoptions.indexOf("rolinfo") != -1 ) {
            return true;
        }
        if (myoptions.indexOf("noteinfo") != -1 ) {
            return true;
        }

        if (myoptions.indexOf("textualinfo") != -1 ) {
            return true;
        }

        bool ok;
        SYD << tr("\n\n\n\n\n...(HEAD(HEAD)).....infooneoption....ComboWidget...isValid...value:|%1|").arg(value);
        if (options().count() == 1 ) {

            SYD << tr("........infooneoption....ComboWidget...isValid...nlimit:|%1|").arg(options().at(0));
            int nvalue = value.toInt(&ok);
            int nlimit = options().at(0).toInt(&ok);

            if ( nvalue > nlimit || nvalue <  1) {

                if (nlimit == 1 ) {
                    SYE << tr("El valor (%1) que introdujo debe ser igual a 1")
                           .arg(value);
                } else {
                    SYE << tr("El valor \"%1\" debe estar entre el rango \"1\" al \"%2\"")
                           .arg(value)
                           .arg(nlimit);
                }
                return false;
            }

        }
        foreach(QString op, options() ) {
            SYD << tr("opcion: |%1|").arg(op);
        }

    }
    else if ( type() == ColorSafet ) {
        if ( !itemsRealValueList().contains(value)) {
            QRegExp rx;
            rx.setPattern("\\#[0-9]{,6}");
            int pos = rx.indexIn(value);
            if (pos == -1 ) {
                return false;
            }

        }

    }
    else if(type() == ListTable) {

        updateVarGlobal(value);
        SYD << tr("...ComboWidget::isValid......value:|%1|")
               .arg(value);
        if ( !itemsRealValueList().contains(value)) {
            return false;
        }
        else {
            value = getRealValue(value);
        }
        SYD << tr("...ComboWidget::isValid......|true|");

    }
    else if (type() == AutofilterSafet)  {
        SYD << tr("....***ComboWidget::isValid...value:|%1|").arg(value);
        QString newvalue = MainWindow::replaceMarks(value);
        SYD << tr("....***ComboWidget::isValid...newvalue:|%1|")
               .arg(MainWindow::replaceMarks(newvalue));
        if ( !itemsValueList().contains(newvalue) ) {
            SYD << tr("....***ComboWidget::isValid...itemsValueList().count():%1")
                   .arg(itemsValueList().count());
            foreach(QString v, itemsValueList()) {
                SYD << tr("....***ComboWidget::isValid...value:|%1|")
                       .arg(v);
            }

            return false;
        }
        if ( SafetYAWL::combovarglobal0.length() > 0 ) {
            SafetYAWL::combovarglobal0 =  SafetYAWL::combovarglobal0 + SafetYAWL::LISTSEPARATORCHARACTER + value;

        }
    }
    else {
        SYD << tr("....***ComboWidget::isValid...value:|%1|").arg(value);
        QString newvalue = MainWindow::replaceMarks(value);
        SYD << tr("....***ComboWidget::isValid...newvalue:|%1|")
               .arg(MainWindow::replaceMarks(newvalue));
        if ( !itemsValueList().contains(newvalue) ) {
            SYD << tr("....***ComboWidget::isValid...itemsValueList().count():%1")
                   .arg(itemsValueList().count());
            foreach(QString v, itemsValueList()) {
                SYD << tr("....***ComboWidget::isValid...value:|%1|")
                       .arg(v);
            }

            return false;
        }
    }
    return true;
}



void ComboWidget::updateVarGlobal(const QString& value) {
    if ( varvalues.keys().contains(value)) {
        qDebug("               *****(1)value: |%s|combovarglobal SafetYAWL::combovarglobal0: |%s|",
               qPrintable(value),qPrintable(SafetYAWL::combovarglobal0));
        SafetYAWL::combovarglobal0 = getVarValue(value);
        qDebug("               *****(2)...value: |%s| combovarglobal SafetYAWL::combovarglobal0: |%s|",
               qPrintable(value),qPrintable(SafetYAWL::combovarglobal0));
    }

}



void ComboWidget::updateComboFlow(bool inwidget) {

    infos.clear();
     QString mykeyvalue;

     QStringList myformlist;

     QString beforef;
     SYD << tr ("....ComboWidget::updateComboFlow...conf()[\"options\"].toString():|%1|")
            .arg(conf()["options"].toString());

     SafetWorkflow::NextStates myns = SafetWorkflow::AllNotCompleted;
     QString myoption = "next";
     if (conf().contains("options")) {
         QString myoptions = conf()["options"].toString();
         QStringList mylist = myoptions.split(",");
         if (mylist.count()> 1 && mylist.at(1)=="oneoption") {
                      setOneoption(true);
         }
         myoption = mylist.at(0);
         if (myoptions.contains("all")) {
             myns = SafetWorkflow::AllNotCompleted;
         }
         else if (myoptions.contains("next")) {
             myns = SafetWorkflow::OnlyNext;
         }
         else {
             myns = SafetWorkflow::Neighbour;
         }

         foreach(QString myo, mylist) {
             if (myo.startsWith("beforefunction:")) {
                 beforef = myo;
                 break;
             }

         }

     }

     if (!conf().contains("keyvalue")) {
         SYW << tr("Error al actualizar un ComboBox de flujo(Comboflow), no existe clave para filtrado");
     }
     mykeyvalue = findkeyvalue(conf()["keyvalue"].toString());

       QStringList mylistglobal = SafetYAWL::combovarglobal0.split(SafetYAWL::LISTSEPARATORCHARACTER);
     if (mykeyvalue.isEmpty()) {
         if (mykeyvalue.isEmpty() ) {
             SYW << tr("ComboWidget::updateComboFlow........mykeyvalue (isEmpty)....");
             SYW << tr("ComboWidget::updateComboFlow........reemplazando con (Variable) SafetYAWL::combovarglobal0: |%1|")
                    .arg(mylistglobal.at(0));
             /**
               * FIXME: listglobal (reemplazando    KEYVALUE
               **/
             SYW << tr("ComboWidget::updateComboFlow........mykeyvalue (isEmpty)..MYLISTGLOBAL..mylistglobal.at(0):|%1|")
                    .arg(mylistglobal.at(0));
             //mykeyvalue = mylistglobal.at(0);
         }
         SYD << tr("ComboWidget::updateComboFlow........returning for KEYVALUE==0");
         return;
     }

     SYD << tr ("...........ComboWidget::updateComboFlow...mykeyvalue:|%1|")
            .arg(mykeyvalue);
     SYD << tr ("ComboWidget::updateComboFlow...beforef:|%1|")
            .arg(beforef);

     SYD << tr ("ComboWidget::updateComboFlow...conf()[\"changefor\"].toString():|%1|")
            .arg(conf()["changefor"].toString());


     if ( conf().contains("path") ) {

         QString mypath = conf()["path"].toString().trimmed();
         SYD << tr("..............................--->updateComboFlow...mykeyvalue: |%1|")
                .arg(mykeyvalue);

         SYD << tr("..............................--->updateComboFlow...PATH_COMBOFLOW....path: |%1|")
                .arg(mypath);







         QString myother;
         if (conf().contains("otherkey")) {
             myother = conf()["otherkey"].toString();
         }

         SYD << tr("..............................->updateComboFlow...myother: |%1|")
                .arg(myother);

         mypath.replace("{#keyvalue0}",mykeyvalue);
         mypath.replace("{#keyvalue1}",myother);
         SYD << tr ("ComboWidget::updateComboFlow...mypath:|%1|")
                .arg(mypath);
         if (mypath.isEmpty() && myoption=="noteinfo") {
             return;
         }


         if (mypath.isEmpty() && myoption=="rolinfo") {
             return;
         }

         if (mypath.isEmpty() && myoption=="textualinfo") {
             return;
         }

         if (!QFile::exists(mypath)) {

             SYW << tr("No es posible leer el archivo \"%1\", por favor consulte al administrador")

                    .arg(mypath.section("/",-1));

             if (!mypath.startsWith("SELECT ",Qt::CaseInsensitive)) {
                 return;
              }
             SYD << tr("..............................--->updateComboFlow...PATH_COMBOFLOW..*IN..path: |%1|")
                    .arg(mypath);


             QString newsql = mypath;

             QSqlQuery query( SafetYAWL::currentDb );
             query.prepare(  newsql );
             bool executed = query.exec();
             SYD << tr("..............................--->updateComboFlow...PATH_COMBOFLOW..*IN.(1).path: |%1|")
                    .arg(mypath);

             if (!executed ) {
                 SYW
                         <<
                            tr("NO se ejecutó correctamente la sentencia de busqueda de Path SQL: \"%1\"")
                            .arg(newsql);
                 return;
             }

             if (!query.next()) {
                 SYW
                         <<
                            tr("Advertencia:No existen registros para busqueda de Path de archivow workfloe: \"%1\"")
                            .arg(newsql);
                 return;
             }
             SYD << tr("..............................--->updateComboFlow...SETTING (1): |%1|")
                    .arg(mypath);
             mypath = query.value(0).toString();
             SYD << tr("..............................--->updateComboFlow...SETTING (2): |%1|")
                    .arg(mypath);


         }


          if (MainWindow::configurator != NULL ) {
              delete  MainWindow::configurator;
              MainWindow::configurator = NULL;
          }
          MainWindow::configurator = new SafetYAWL();
          Q_CHECK_PTR( MainWindow::configurator );                 

          MainWindow::configurator->openXML(mypath);
          MainWindow::configurator->convertXMLtoObjects();
          MainWindow::configurator->openDataSources();

          if ( myoption=="textualinfo" || myoption=="rolinfo" || myoption=="noteinfo") {
                if ( MainWindow::configurator->getWorkflows().count() > 0 ) {
                    SYD << tr("...ComboWidget::updateComboFlow...(textualinfo|rolinfo|noteinfo)..(3)");
                    SafetWorkflow* mywf = MainWindow::configurator->getWorkflows().at(0);
                    Q_CHECK_PTR(mywf);
                    int fieldtype = 0;
                    if (myoption=="rolinfo") {
                        fieldtype = 1;
                    }
                    if (myoption=="noteinfo") {
                        fieldtype = 2;
                    }

                    SYD << tr("...ComboWidget::updateComboFlow...fieldtype:%1")
                           .arg(fieldtype);
                    infos = mywf->textualInfos(mykeyvalue,fieldtype,myother);
                    SYD << tr("...ComboWidget::updateComboFlow...(textualinfo)..infos.count():%1")
                           .arg(infos.count());
                    if ( infos.count() > 0 ){
                        SYD << tr("\n\n...(infochuchu) ComboWidget::updateComboFlow...(textualinfo)...infos.at(0):%1")
                               .arg(infos.at(0));
                    }

                }
          }
          else if ( myoption=="numericinfo") {
                if ( MainWindow::configurator->getWorkflows().count() > 0 ) {
                    SafetWorkflow* mywf = MainWindow::configurator->getWorkflows().at(0);
                    Q_CHECK_PTR(mywf);
                    QList<double> myinfos;
                    myinfos  = mywf->numericInfos(mykeyvalue);

                }
          }
          else {
              if ( MainWindow::configurator->getWorkflows().count() > 0 ) {

                  SYD << tr("\n\n\n......ComboWidget::updateComboFlow..entering listNextStates....inwidget:|%2|..mykeyvalue:|%1|").arg(mykeyvalue)
                         .arg(inwidget);
                   infos = MainWindow::configurator->getWorkflows().at(0)
                           ->listNextStates(mykeyvalue,myns,true);

                   // Solo el nombre del flujo
                   QString namewf = conf()["path"].toString().section("/",-1);
                   namewf = namewf.mid(0,namewf.lastIndexOf("."));

                   QMap<QString,bool> myperms;
                   QList<int> deleted;
                   SYD << tr(".................ComboWidget::updateComboFlow.......COMBOINFO...infos.count():|%1|")
                          .arg(infos.count());

                   for (int i=0; i < infos.count(); i++ ) {
                       myperms = MainWindow::doPermiseExecOperationAction(namewf + "." + infos.at(i)  );
                        SYD << tr("..............ComboWidget::updateComboFlow.....(list)....operation;|%1|").arg(namewf + "." + infos.at(i));

                        SYD << tr("..............ComboWidget::updateComboFlow.....(list)....myperms.count();|%1|").arg(myperms.count());



                       if (myperms.count() > 0 ) {
                           if (myperms.contains("noread") ) {
                                deleted.append(i);
                                continue;
                            }
                       }
                       SYD << tr(".................ComboWidget::updateComboFlow.......COMBOINFO: |%1|")
                              .arg(infos.at(i));
                       if ( infos.at(i).contains(tr("*FIRMAR Documento")) ) {
                           QString mysignaction = infos.at(i);                             
                             QString realsignaction = tr("%1 en %2 usando /%3/")
                                     .arg(infos.at(i))
                                     .arg(conf()["path"].toString())
                                     .arg(mykeyvalue);
                             realvalues[ mysignaction ] = realsignaction;

                        }

                   }
                   // Borrar los eliminados
                   for(int i = deleted.count()-1; i >= 0;i--) {
                       infos.removeAt(i);
                   }
              }
          }

          if (!beforef.isEmpty()) {


              foreach(QString f, myformlist) {

                    QString mypattern = QString("{#%1}").arg(f.split(":").at(0));
                   if (f.split(":").count()>1) {
                       beforef.replace(mypattern,f.split(":").at(1));
                   }
                   else {
                       beforef.replace(mypattern,"");
                   }

              }


              SYD << tr("........ComboWidget::updateComboFlow....MYBEFORES....(1)");
              foreach(QString info, infos) {
                  SYD << tr("........ComboWidget::updateComboFlow....MYBEFORES...INFOS...info:|%1|")
                         .arg(info);
              }

              QPair<QStringList,bool> myreturnb = execBeforeFunction(infos, beforef,mykeyvalue,myother);

              QStringList mybefores =  myreturnb.first;

              SYD << tr("........ComboWidget::updateComboFlow....MYBEFORES....(2)...mybefores.count():|%1|")
                     .arg(mybefores.count());

              foreach(QString myb, mybefores) {
                  SYD << tr("........ComboWidget::updateComboFlow....MYBEFORES...myb:|%1|")
                         .arg(myb);
              }

              if (myreturnb.second ||  mybefores.count() > 0 ) {
                  _options  = mybefores;
              }
              else {
                    _options = infos;
              }

              _itemvaluelist = _options;
              return;
          }
          else {
            _itemvaluelist = infos;
          }

     }

     _options = _itemvaluelist;

}

QString ComboWidget::findkeyvalue(const QString& k) {
    QString result = k;
    if (!conf().contains("changefor")) {
        return result;
    }
    if (k.indexOf(";") == -1 ){
        return result;
    }
    QStringList mylist = k.split(",",QString::SkipEmptyParts);

    foreach(QString myfield, mylist) {
        QStringList fields = myfield.split(";",QString::SkipEmptyParts);
        if (fields.count() < 2 ) {
            continue;
        }
        if (conf()["changefor"].toString()
                .split(",",QString::SkipEmptyParts).contains(fields.at(0))){
            return fields.at(1);
        }
    }


    return result;
}

void ComboWidget::updateComboColorSafet(bool inwidget) {
    if ( !conf().contains("options")) {
          return;
      }
      generateLiterals(conf()["options"].toString());

}

void ComboWidget::selColor() {
    qDebug("...ComboWidget::selColor....");
//    if (varbox == NULL ) {
//        return;
//    }

//QColor mycolor;
#if (QT_VERSION > QT_VERSION_CHECK(4, 4, 3)) 
//     QColorDialog mydialog;
//    if ( mydialog.exec() == QDialog::Accepted ) {
//        mycolor = mydialog.selectedColor();
//	}
#else
        //mycolor = QColorDialog::getColor();
#endif
        //QString curcolor =  QString("#%1%2%3").arg(mycolor.red(),2,16)
          //       .arg(mycolor.green(),2,16).arg(mycolor.blue(),2,16);
       // curcolor.replace(" ","0");
//        varbox->addItem(curcolor);
//        varbox->setCurrentIndex(varbox->count()-1);
    

}

void ComboWidget::buildButtonWidget(const QStringList& l) {
     qDebug("...buildButtonWidget...");
     Q_ASSERT( l.count() > 0 );

}


void ComboWidget::insertAndClose() {

//    close();
}


void ComboWidget::viewdoc() {
}


void ComboWidget::buildWidget() {

    //FIXME: Chequear si es necesario

     updateCombo();

}

void ComboWidget::updateComboListLiteral() {
    if ( !conf().contains("options")) {
          return;
      }
    generateLiterals(conf()["options"].toString());


}

void ComboWidget::generateLiterals(const QString& s, bool inwidget) {
    qDebug("...**ComboWidget...generateLiterals....:%s", qPrintable(s));    
    QStringList vars = s.split(",",QString::SkipEmptyParts);
    int i = 0;
    _itemvaluelist.clear();
    _itemrealvaluelist.clear();
    foreach(QString r, vars) {
        QStringList values = r.split("::");
        QString value = values.at(0);



        QString realvalue = value;
        QVariant info;



        if ( values.count() > 1 ) {
            info = values.at(1);

        }

        if ( values.count() > 2 ) {
            realvalue = values.at(2);
        }



        SYD << tr("ComboWidget::generateLiterals...value: |%1| ... realvalue.:|%2|")
               .arg(value)
               .arg(realvalue);

        realvalues[realvalue] = value;
        if ( inwidget ) {
//               varbox->addItem(value);
//               varbox->setItemData(i, info, Qt::ToolTipRole);
        }
        _itemvaluelist.append(realvalue);
        _itemrealvaluelist.append(value);
        i++;        
    }
    _options = _itemvaluelist;

}

QString ComboWidget::getRealValue(const QString& s) {

    if ( !realvalues.keys().contains(s)) {
        return s;
    }
    return realvalues[ s];
}

QString ComboWidget::getVarValue(const QString& s) {

    if ( !varvalues.keys().contains(s)) {
        return s;
    }
    return varvalues[ s];
}

void ComboWidget::updateComboListTable(bool inwidget) {

    //varbox->setInsertPolicy(QComboBox::InsertAlphabetically);
     QSqlQuery query(SafetYAWL::currentDb ); // <-- puntero a db actual
     QString field, table, where, value;
     if ( !conf().contains("options")) {
         return;
     }


     QString myoptions = conf()["options"].toString();
     myoptions.replace("_USERNAME", SafetYAWL::currentAuthUser());

     QStringList mytwofields = myoptions.split("::",QString::SkipEmptyParts);
     QStringList options = mytwofields.at(0).split(":",QString::SkipEmptyParts);
          if (options.count() < 2 ) {
          SafetYAWL::streamlog
                  << SafetLog::Warning
                  << tr("IMPORTANTE, No se definio correctamente el campo \"options\" --> \"%1\"")
		.arg(myoptions);
          return;
     }
     field = options.at(0);

     QString order =  " ORDER BY %1 ASC;";
     qDebug("***....conf().contains(\"order\"):|%s|",
            qPrintable(conf()["order"].toString()));
     if ( conf().contains("order")) {
         QString command = conf()["order"].toString();
         if ( command == "none" ) {
             order = ";";
         }
         else if (command == "desc") {
             order = " ORDER BY %1 DESC;";
         }
     }
     QString realvalue;
     _itemrealvaluelist.clear();
     _itemvaluelist.clear();
     table = options.at(1);
     QString command;
     QString titleField;
     if ( mytwofields.count() > 1 ) {
         titleField = mytwofields.at(1);
     }
     if ( options.count() > 2 ) {
         where = options.at(2);
         where = proccessWhereOption(where);
         SYD << tr("....ComboWidget::updateComboListTable...where:|%1|")
                .arg(where);
     }

     if (conf().contains("keyvalue")) {
         /**
          * FIXME: Chequear porque el keyvalue se pasa con el nombre del campo
          */
         QString mykey = conf()["keyvalue"].toString();
         QStringList mylist = mykey.split(";",QString::SkipEmptyParts);
         if ( mylist.count() > 1 ) {
             mykey = mylist.at(1);
         }
         QMap<QString,QString>  l;
         l["1"]= mykey;
         if (!where.isEmpty()) {
             bool doit = false;
            where = SafetWorkflow::replaceArg(where,l);
            SYD << tr("....ComboWidget::updateComboListTable...reemplazando where:|%1|")
                   .arg(where);
         }
     }

     if ( !titleField.isEmpty() ) {
         if ( options.count() >  3 ) {
             QString varglobal = options.at(3);
             if ( !titleField.isEmpty()) {
                 command = QString("SELECT %1,%4,%5 from %2 %3 "+order)
                         .arg(field).arg(table)
                         .arg(where)
                         .arg(titleField)
                         .arg(varglobal);
             }
             else {
                 command = QString("SELECT %1,'n/a' as safetnotapplicable,%4 "
                                   "from %2 %3 "+order)
                         .arg(field).arg(table)
                         .arg(where)
                         .arg(varglobal);
             }

         }
         else {
                 command = QString("SELECT %1,%4 from %2 %3 "+order)
                         .arg(field).arg(table)
                         .arg(where)
                         .arg(titleField);
         }

     }
     else {
         command = QString("SELECT %1 from %2 %3 "+order).arg(field).arg(table).arg(where);
     }


     SYD << tr("Actualizando lista del combo: \"%1\"").arg( command );
     query.prepare(  command );
     bool executed = query.exec();
     if (!executed ) {
          SYW << tr("IMPORTANTE, NO se ejecutó correctamente la consulta de opciones del combo: \"%1\"").arg(command);
          return;
     }
     bool istitlefield  = query.record().count() > 1 && query.record().fieldName(1) != "safetnotapplicable";

     bool isvarglobal = query.record().count() > 2;

     while( query.next() ) {
           realvalue = query.value(0).toString().trimmed();
          if (!_itemrealvaluelist.contains(realvalue) ) {
              _itemrealvaluelist.append(realvalue);
              if ( istitlefield ) {
                  value = query.value(1).toString().trimmed();
              }
              else {
                  value = realvalue;
              }              
              _itemvaluelist.append(value);
              realvalues[ value ] = realvalue;
              if ( isvarglobal ) {
//                  qDebug("........isvarglobal:....:value [ %s ] --> %s",
//                         qPrintable(value), qPrintable(query.value(2).toString().trimmed()));
                  varvalues[ value ] = query.value(2).toString().trimmed();
              }
          }

     }
     if ( inwidget ) {
//        varbox->addItems( _itemvaluelist );
    }
     if (!isTextParent()) {
//         varbox->addItem(tr("<Cualquiera>"));
     }

     _options = _itemvaluelist;
     //** Para parámetros

     //_options.prepend(tr("Cualquier valor...")); // Para Parámetros
}


void ComboWidget::updateComboListBinaryRepo(bool inwidget) {

    //varbox->setInsertPolicy(QComboBox::InsertAlphabetically);

    SafetBinaryRepo myrepo;

    myrepo.open();
    //QSqlQuery query(SafetYAWL::currentDb ); // <-- puntero a db actual

    QSqlQuery query( myrepo.binaryDb() );

     QString field, table, where, value;
     if ( !conf().contains("options")) {
         return;
     }


     QStringList mytwofields = conf()["options"].toString().split("::",QString::SkipEmptyParts);
     QStringList options = mytwofields.at(0).split(":",QString::SkipEmptyParts);
          if (options.count() < 2 ) {
          SafetYAWL::streamlog
                  << SafetLog::Warning
                  << tr("IMPORTANTE, No se definio correctamente el campo \"options\" --> \"%1\"").arg(conf()["options"].toString());
          return;
     }
     field = options.at(0);

     QString order =  " ORDER BY %1 ASC;";
     qDebug("***....conf().contains(\"order\"):|%s|",
            qPrintable(conf()["order"].toString()));
     if ( conf().contains("order")) {
         QString command = conf()["order"].toString();
         if ( command == "none" ) {
             order = ";";
         }
         else if (command == "desc") {
             order = " ORDER BY %1 DESC;";
         }
     }
     QString realvalue;
     _itemrealvaluelist.clear();
     _itemvaluelist.clear();
     table = options.at(1);
     QString command;
     QString titleField;
     if ( mytwofields.count() > 1 ) {
         titleField = mytwofields.at(1);
     }
     if ( options.count() > 2 ) {
         where = options.at(2);
         where = proccessWhereOption(where);
         SYD << tr("....ComboWidget::updateComboListTable...where:|%1|")
                .arg(where);
     }

     SafetWorkflow* mywf = MainWindow::configurator->getWorkflows().at(0);
     if (mywf == NULL) {
         SYD << tr("COMBO mywf NULL...**(2)");
     }

     if (conf().contains("keyvalue")) {
         QMap<QString,QString> l;
         l["1"] = conf()["keyvalue"].toString();
         if (!where.isEmpty()) {
             bool doit = false;
            where = SafetWorkflow::replaceArg(where,l);
            SYD << tr("....ComboWidget::updateComboListTable...reemplazando where:|%1|")
                   .arg(where);
         }
     }

     if ( !titleField.isEmpty() ) {
         if ( options.count() >  3 ) {
             QString varglobal = options.at(3);
             if ( !titleField.isEmpty()) {
                 command = QString("SELECT %1,%4,%5 from %2 %3 "+order)
                         .arg(field).arg(table)
                         .arg(where)
                         .arg(titleField)
                         .arg(varglobal);
             }
             else {
                 command = QString("SELECT %1,'n/a' as safetnotapplicable,%4 "
                                   "from %2 %3 "+order)
                         .arg(field).arg(table)
                         .arg(where)
                         .arg(varglobal);
             }

         }
         else {
                 command = QString("SELECT %1,%4 from %2 %3 "+order)
                         .arg(field).arg(table)
                         .arg(where)
                         .arg(titleField);
         }

     }
     else {
         command = QString("SELECT %1 from %2 %3 "+order).arg(field).arg(table).arg(where);
     }


     SYD << tr("....ComboWidget::updateComboBinaryRepo...Actualizando lista del combo: \"%1\"").arg( command );
     query.prepare(  command );
     bool executed = query.exec();
     if (!executed ) {
          SYW << tr("IMPORTANTE, NO se ejecutó correctamente la consulta de opciones del combo: \"%1\"").arg(command);
          return;
     }
     bool istitlefield  = query.record().count() > 1 && query.record().fieldName(1) != "safetnotapplicable";

     bool isvarglobal = query.record().count() > 2;

     SYD << tr("....ComboWidget::updateComboBinaryRepo...query.record.count():|%1|...")
            .arg(query.record().count());
     int regcount = 0;
     while( query.next() ) {
           realvalue = query.value(0).toString().trimmed();
          if (!_itemrealvaluelist.contains(realvalue) ) {
              _itemrealvaluelist.append(realvalue);
              if ( istitlefield ) {
                  value = query.value(1).toString().trimmed();
              }
              else {
                  value = realvalue;
              }
              _itemvaluelist.append(value);
              realvalues[ value ] = realvalue;
              if ( isvarglobal ) {
//                  qDebug("........isvarglobal:....:value [ %s ] --> %s",
//                         qPrintable(value), qPrintable(query.value(2).toString().trimmed()));
                  varvalues[ value ] = query.value(2).toString().trimmed();
              }
          }
          regcount++;

     }
     SYD << tr("....ComboWidget::updateComboBinaryRepo...regcount:|%1|...")
            .arg(regcount);

     if ( inwidget ) {
//        varbox->addItems( _itemvaluelist );
    }
     if (!isTextParent()) {
//         varbox->addItem(tr("<Cualquiera>"));
     }

     _options = _itemvaluelist;

}

QString ComboWidget::html() {
    QString result;

    QStringList myenables;

    if (conf().contains("enablethis")) {
            myenables = conf()["enablethis"].toString().split(",");
    }

    bool ok;
    uint shlen = SafetYAWL::getConf()["Widgets/combowidget.shrinklen"].toUInt(&ok);
    if (!ok || shlen == 0) {
        shlen = 30;
    }
    if (conf().contains("options")) {
        QStringList mylist = conf()["options"].toString().split(",");
        if (mylist.count()> 1 && mylist.at(1)=="oneoption") {
            result = QString("<input name=\"%1\" id=\"%1\" size=\"25\"></input>")
                    .arg(caption());
                    setOneoption(true);
                    return result;
        }

    }

    QString mypreffix = caption().left(4);
    result += QString("<select name=\"%1\" id=\"%1\" class=\"text ui-widget-content ui-corner-all\"")
            .arg(caption());


    bool isenables = myenables.count() > 0 ;
    if (isenables ) {
        result += QString(" onChange=\"%1Change(this)\" ")
                .arg(mypreffix);
    }

    result += ">\n";
    result += QLatin1String("<option value=\"\"></option>");
    foreach(QString s, options()){
        if (!s.trimmed().isEmpty()) {

        QString newitem;
        if (s.length() >= shlen) {
        newitem = QString("<option value=\"%2\" title=\"%3\">%1</option>\n")
                .arg(SafetYAWL::shrinkSentence(s,shlen))
                .arg(getRealValue(s))
                .arg(s);
        }
        else {
            newitem = QString("<option value=\"%2\">%1</option>\n")
                    .arg(SafetYAWL::shrinkSentence(s,shlen))
                    .arg(getRealValue(s));
        }

            result += newitem;
        }
    }


    result += QLatin1String("</select>");


    if (isenables ) {

        result += QLatin1String("\n<script>\n");
        result += QString("function %1Change(myobj) {\n").arg(mypreffix);
        result += "var currvalue = myobj.value;\n";
        QString namef;
        QMap<QString,QString> ormap;
        foreach(QString myenable, myenables ) {
            namef = myenable.section(":",0,0);
            if (ormap[namef].isNull() ) {
                ormap[namef] = "";
            }
            QString valuef = myenable.section(":",1,1);
            ormap[namef] += QString(" currvalue == '%1' ||").arg(valuef);

        }
        for(int i=0; i < ormap.keys().count(); i++) {
            ormap[ormap.keys().at(i) ].chop(2);
            result += QString("if ( %1 ) {\n").arg(ormap[ormap.keys().at(i) ]);
            result += QString("   for(i=0;i<safetCurrentCombos.length;i++) {\n");
            result += QString("     if (safetCurrentCombos[i]=='%1')  $('#%1').show();\n").arg(ormap.keys().at(i));
            result += QString("     else {\n");
            result += QString("   if (safetCurrentCombos[i] != myobj.id  && safetCurrentCombos[i] != 'safetDiv'+myobj.id) $('#'+safetCurrentCombos[i]).hide();\n");
            result += QString("      }\n");
            result += QString("   }\n");
            result += QString("}\n\n");

        }

        result += "}\n";
        result += "\n";

        result += QLatin1String("\n</script>\n");
    }



    return result;
}

QString ComboWidget::htmlForKey(const QString& k, const QString& otherk) {

    if (k.isEmpty()) {
        return QLatin1String("<option value=\"\"></option>");
    }
    conf()["keyvalue"] = k;
    conf()["otherkey"] = otherk;
    bool ok;
    uint shlen = SafetYAWL::getConf()["Widgets/combowidget.shrinklen"].toUInt(&ok);
    if (!ok || shlen == 0) {
        shlen = 30;
    }

    updateCombo();

    if (oneoption()) {
        QString myvalue;
        if (options().count() > 0 ) {
            myvalue = options().at(0);
        }
        QString result = QString("%1")
                .arg(myvalue);

        return result;
    }

//    QString result = QString("<select name=\"%1\" id=\"%1\" class=\"text ui-widget-content ui-corner-all\">\n")
//            .arg(caption());

    QString result;
    result += QLatin1String("<option value=\"\"></option>");
    foreach(QString s, options()){
        if (!s.trimmed().isEmpty()) {
        QString newitem = QString("<option value=\"%2\">%1</option>\n")
                .arg(SafetYAWL::shrinkSentence(s,shlen))
                .arg(getRealValue(s));
            result += newitem;
        }
    }


    return result;

}

QString ComboWidget::proccessWhereOption(const QString& w) {
    qDebug("..proccessWhereOption...(1)");
    QString result = w;
    QString mysearchvalue;
    QRegExp  editpattern;
    editpattern.setPattern("\\{\\{([a-zA-Z0-9_]+)\\}\\}");
    int pos = w.indexOf(editpattern);
    if ( pos == -1 ) {
        return w;
    }
    mysearchvalue = editpattern.cap(1);

    qDebug("...mysearchvalue:|%s|", qPrintable(mysearchvalue));

//    if ( _texteditparent == NULL ) {
//        qDebug("..._texteditparent == NULL");
//        return result;
//    }
/*    TextEdit* mytextedit;
    DomModel *mydommodel;
    mytextedit = qobject_cast<TextEdit*>(_texteditparent);
    if (mytextedit == NULL ) {
        qDebug("...mytextedit NULL");
        return result;
    }
    mydommodel = mytextedit->dommodel();

    QString value  = mydommodel->searchKeyValueOnTextEdit(mysearchvalue);
    result.replace("{{"+mysearchvalue+"}}",value); */

    return result;
}

void ComboWidget::updateComboTaskSafet(bool inwidget) {

    QString exclude;

    if (MainWindow::configurator == NULL ) {
        return;
    }
    if (conf().contains("exclude")) {
        exclude = conf()["exclude"].toString();

        SYD << tr("....updateComboTaskSafet....exclude:|%1|")
               .arg(exclude);

    }

    if ( !conf().contains("keyvalue") ) {
        SYW << tr("Se necesita una clave para actualizar el widget de Variables de Flujo de Trabajos");
        return;
    }
    QStringList mylist = conf()["keyvalue"].toString().split(SafetYAWL::LISTSEPARATORCHARACTER);
    QString mykeyvalue = mylist.at(0);

    QString mykeydata;
    if (mylist.count() > 1) {
        mykeydata = mylist.at(1);
    }
    mykeyvalue = findkeyvalue(mykeyvalue);


    SYD << tr("......ComboWidget::updateComboTaskSafet..(0)..*mykeydata*:|%1|")
           .arg(mykeydata);


    if (!QFile::exists(mykeyvalue)) {
        SYW << tr("......**ComboWidget::updateComboTaskSafet....no existe:|%1|")
               .arg(mykeyvalue);
        MainWindow::doRestoreGraph(Safet::PLANNEDGRAPHSFILENAME);
        SYD << tr("......ComboWidget::updateComboTaskSafet....key:|%1|")
               .arg(mykeyvalue);
        if (!MainWindow::graphs().contains(mykeyvalue)) {
            SYW << tr("No se encuentra en el repositorio el archivo de grafos planificados");
            return;
        }
        QString pathflow = MainWindow::graphs()[mykeyvalue].second;
        SYD << tr("........................**ComboWidget::updateComboTaskSafet......->pathflow:|%1|")
               .arg(pathflow);

        mykeyvalue = pathflow.section(SafetYAWL::LISTSEPARATORCHARACTER,1,1);

        SYD << tr("......ComboWidget::updateComboTaskSafet....mykeyvalue:|%1|")
               .arg(mykeyvalue);

    }
    else {
        SYD << tr("......ComboWidget::updateComboTaskSafet....SI existe:|%1|")
               .arg(mykeyvalue);
    }


    delete  MainWindow::configurator;
    MainWindow::configurator = new SafetYAWL();
    Q_CHECK_PTR( MainWindow::configurator );

    SYD << tr("..................................->ComboWidget::updateComboTaskSafet...keyvalue: |%1|").arg(mykeyvalue);

    QString myautofilter;
    QString myrecursivefilter;
    MainWindow::configurator->openXML(mykeyvalue);
    if ( conf().contains("autofilter")) {
        myautofilter = conf()["autofilter"].toString().trimmed();
        MainWindow::configurator->setAutofilters(myautofilter);
    }

    if ( conf().contains("recursivefilter")) {
        myrecursivefilter = conf()["recursivefilter"].toString().trimmed();
        MainWindow::configurator->setRecursivefilters(myrecursivefilter);
    }

    MainWindow::configurator->openDataSources();
    MainWindow::configurator->convertXMLtoObjects();


    QString myjson;
    QString info = mykeydata;
    if ( !info.isEmpty())  {

        QString img = MainWindow::configurator->getWorkflows().at(0)->generateGraph("svg", myjson,info);
        SYD << tr("..................................->ComboWidget::updateComboTaskSafet...myjsontable (2): |%1|")
               .arg(myjson);
    }

    if ( MainWindow::configurator->getWorkflows().count() ==  0 ) {
        SYW << tr("No se puede visualizar la lista de tareas en el widget \"combo\""
                  "La lista de objetos de flujos de trabajo está vacía");
        return;
    }

    QList<SafetTask*> mytasks = MainWindow::configurator
            ->getWorkflows().at(0)->getTasks();


    foreach(SafetTask* t, mytasks) {
        if (t != NULL ) {
            if ( _filtercompleted == false || myjson.isEmpty()) {
                _itemvaluelist.append(t->id());
                _options.append(t->id());
            }
            else {
                if ( SafetYAWL::checkCompletedTask(t->id(),myjson)  ) {
                    _itemvaluelist.append(t->id());
                    _options.append(t->id());
                }
            }
        }
    }

    if (!exclude.isEmpty()) {

        QString myother;
        if (conf().contains("otherkey")) {
            myother = conf()["otherkey"].toString();
        }
        SYD << tr("");
        SYD << tr("..........updateComboTask...exclude...:|%1|").arg(exclude);
        SYD << tr("..........updateComboTask...mykeyvalue:|%1|").arg(mykeyvalue);
        SYD << tr("..........updateComboTask.......**info:|%1|").arg(myother);
        SYD << tr("");

        QStringList myexcludes = SafetWorkflow::calculateListSQL(exclude,mykeyvalue,myother);
        foreach(QString e,myexcludes) {
            if(_options.contains(e)) {
                _options.removeAll(e);
            }
        }
    }

    if ( inwidget ) {

    }


}


void ComboWidget::updateComboVariableSafet(bool inwidget) {
    SYD << tr("combovariable");
    if (MainWindow::configurator == NULL ) {
        SYW << tr(".....ComboWidget::updateComboVariableSafet....MainWindow::configurator es NULO");
        return;
    }
    if ( !conf().contains("keyvalue") ) {
        SYW << tr("Se necesita una clave para actualizar el widget de Variables de Flujo de Trabajos");
        return;
    }

    SYD << tr("............ComboWidget::updateComboVariableSafet...keyvalue (before): |%1|").arg(conf()["keyvalue"].toString());
    QString mykeyvalue = findkeyvalue(conf()["keyvalue"].toString());

    delete  MainWindow::configurator;
    SYD << tr("...ComboWidget::updateComboVariableSafet...MainWindow::mymainwindow is NULL: %1")
           .arg(MainWindow::mymainwindow == NULL);
    MainWindow::configurator = new SafetYAWL(MainWindow::mymainwindow->_currconfpath + "/" + Safet::datadir );
    Q_CHECK_PTR( MainWindow::configurator );

    SYD << tr("ComboWidget::updateComboVariableSafet.....->keyvalue: |%1|").arg(mykeyvalue);







    QStringList mylistglobal = SafetYAWL::combovarglobal0.split(SafetYAWL::LISTSEPARATORCHARACTER);
    if (mykeyvalue.isEmpty() ) {
        mykeyvalue = mylistglobal.at(0);
        SYD << tr("ComboWidget::updateComboVariableSafet  Empty:|%1|").arg(mykeyvalue);

    }

    SYD << tr("ComboWidget::updateComboVariableSafet........mykeyvalue:|%1|").arg(mykeyvalue);
    QString myautofilter;
    QString myrecursivefilter;
    
    SYD << tr("ComboWidget::updateComboVariableSafet...openXML()..");
    MainWindow::configurator->openXML(mykeyvalue);
    if ( conf().contains("autofilter")) {
        myautofilter = conf()["autofilter"].toString().trimmed();


    }
    if (myautofilter.isEmpty()) {
        if ( mylistglobal.count() > 1 ) {
            myautofilter = mylistglobal.at(1);
        }
    }
    if ( !myautofilter.isEmpty() ) {
       MainWindow::configurator->setAutofilters(myautofilter);
    }

    SYD << tr("ComboWidget::updateComboVariableSafet.....->myautofilter: |%1|").arg(myautofilter);
    if ( conf().contains("recursivefilter")) {
        myrecursivefilter = conf()["recursivefilter"].toString().trimmed();
        MainWindow::configurator->setRecursivefilters(myrecursivefilter);
    }

    MainWindow::configurator->openDataSources();
    MainWindow::configurator->convertXMLtoObjects();


    if ( MainWindow::configurator->getWorkflows().count() > 0 ) {
        _itemvaluelist = MainWindow::configurator
                ->getWorkflows().at(0)->variablesId().toList();
    }
    if ( inwidget ) {

    }

     _options = _itemvaluelist;
}



void ComboWidget::updateComboRecursivefilterSafet(bool inwidget) {
    if (MainWindow::configurator == NULL ) {
        SYW << tr("Configurator is NULL");
        return;
    }
    if ( !conf().contains("keyvalue") ) {
        SYW << tr("Se necesita una clave para actualizar el widget de Variables de Flujo de Trabajos");
        return;
    }

    QString mykeyvalue = findkeyvalue(conf()["keyvalue"].toString());
    SYD << tr("........updateComboRecursivefilterSafet....mykeyvalue...(1)...:|%1|").arg(mykeyvalue);
    if (mykeyvalue.isEmpty()) {
         mykeyvalue = SafetYAWL::combovarglobal0;
    }

    SYD << tr("........updateComboRecursivefilterSafet....mykeyvalue...(2)...:|%1|").arg(mykeyvalue);

    delete  MainWindow::configurator;
    MainWindow::configurator = new SafetYAWL();
    Q_CHECK_PTR( MainWindow::configurator );

    SYD << tr("ComboWidget::updateComboRecursivefilterSafet...mykeyvalue: %s").arg(mykeyvalue);
    MainWindow::configurator->openXML(mykeyvalue);

    MainWindow::configurator->convertXMLtoObjects();

    MainWindow::configurator->openDataSources();

    if ( MainWindow::configurator->getWorkflows().count() > 0 ) {
        _itemvaluelist = MainWindow::configurator->getWorkflow()->recursivefiltersId().toList();
    }

    if ( inwidget ) {
        //            varbox->addItems( _itemvaluelist );
    }
    _options = _itemvaluelist;


}


void ComboWidget::updateComboAutofilterSafet(bool inwidget) {
     if (MainWindow::configurator == NULL ) return;

     SYD << tr("...ComboWidget::updateComboAutofilterSafet...(1)...");
     if ( !conf().contains("keyvalue") ) {
         SYW << tr("Se necesita una clave para actualizar el widget de Variables de Flujo de Trabajos");
         return;
     }
          SYD << tr("...ComboWidget::updateComboAutofilterSafet...(2)...");
     QString mykeyvalue = findkeyvalue(conf()["keyvalue"].toString());

     SYD << tr("...ComboWidget::updateComboAutofilterSafet...(3)...mykeyvalue:|%1|").arg(mykeyvalue);

     SYD << tr("...ComboWidget::updateComboAutofilterSafet...(4)...SafetYAWL::combovarglobal0:|%1|").arg(SafetYAWL::combovarglobal0);
     if (mykeyvalue.isEmpty()) {
          mykeyvalue = SafetYAWL::combovarglobal0;
     }

     SYD << tr("...ComboWidget::updateComboAutofilterSafet...(5)...mykeyvalue:|%1|").arg(mykeyvalue);

     delete  MainWindow::configurator;
     MainWindow::configurator = new SafetYAWL();
     Q_CHECK_PTR( MainWindow::configurator );
     SYD << tr(".........ComboWidget::updateComboAutofilterSafet..........mykeyvalue:|%1|")
            .arg(mykeyvalue);


     // mykeyvalue = "/home/vbravo/.safet/flowfiles/tilesbyday.xml";
     MainWindow::configurator->openXML(mykeyvalue);
     SYD << tr("ComboWidget::updateComboAutofilterSafet...mykeyvalue: |%1|").arg(mykeyvalue);
     MainWindow::configurator->convertXMLtoObjects();
     MainWindow::configurator->openDataSources();

     if ( MainWindow::configurator->getWorkflows().count() > 0 ) {
         _itemvaluelist = MainWindow::configurator->getWorkflow()->autofiltersId().toList();
     }


     if ( inwidget ) {
         //            varbox->addItems( _itemvaluelist );
     }

     _options = _itemvaluelist;
     SYD << tr(".........ComboWidget::updateComboAutofilterSafet.........._itemvaluelist.count():|%1|")
            .arg(_itemvaluelist.count());

     //qDebug("...(4)....***...updateComboVariableSafet....updateComboVar()....");
}


QString ComboWidget::text() const {

    QString str;
/*     if ( varbox != NULL ) {

        str = varbox->currentText();
    }
     else {
         if ( !hashtext.isEmpty()) {
             str = hashtext;
         }

     }
     if (signbutton != NULL && type()==Flow && !_cursigners.isEmpty()) {
         str += "|"+ _cursigners;
     }*/

    return str;

}

QRect ComboWidget::getGeoParams() const {
     QRect result;
//     Q_CHECK_PTR( varbox );
     result.setHeight( 36 );
     result.setWidth( 220 );
     return result;
}


void ComboWidget::setFocus ( Qt::FocusReason reason ) {
/*     if ( varbox == NULL ) {
          if ( signbutton == NULL ) {
               TextEdit* te = qobject_cast<TextEdit*>(parent());
               Q_CHECK_PTR(  te );
               te->setFocus(reason);
               return;
          } else {
               QWidget::setFocus ( reason );
               signbutton->setFocus(reason);
          }
     } else {
          QWidget::setFocus ( reason );
          varbox->setFocus( reason);
     } */

}


QPair<QStringList,bool> ComboWidget::execBeforeFunction(const QStringList& keys,
                                            const QString& op, const QString& kv, const QString& otherkey) {
    QStringList result;

    if (!op.startsWith("beforefunction:")) {
        SYE << tr("No existe opción correcta para ejecutar \"beforefunction\" (Función "
                  " de ejecución previa");
        return QPair<QStringList,bool>(keys,false);
    }


    QString mysql = op.mid(QString("beforefunction:").length());
    SYD << tr("ComboWidget::execBeforeFunction: mysql:\"%1\"")
           .arg(mysql);

    
    SYD << tr("ComboWidget::execBeforeFunction: keys.count():\"%1\"")
           .arg(keys.count());



    bool haszero = false;

    QString osql = mysql;
    int i = 0;



    foreach(QString k, keys) {
        QSqlQuery query( SafetYAWL::currentDb );
         mysql = osql;
         bool issign = false;
         if (k.startsWith(tr("*FIRMAR Documento (")) )  {
             int pos0 = k.indexOf("/");
             QString newk = k.trimmed();
             newk = newk.mid(pos0+1);
             newk.chop(1);

             SYD << tr("ComboWidget::execBeforeFunction: NEWK:\"%1\"")
                    .arg(newk);

             mysql.replace("{#currvalue}",newk);
             issign = true;

             haszero = true;

         }
         else {
            mysql.replace("{#currvalue}",k);
         }
         mysql.replace("{#keyvalue1}",kv);
         mysql.replace("{#keyvalue0}",otherkey);

         SYD << tr("ComboWidget::execBeforeFunction: mysql (replace):\"%1\"")
                .arg(mysql);

         query.prepare(  mysql );

           bool executed = query.exec();
           if (!executed ) {
                SYW << tr(" (before function) no se ejecutó correctamente "
                      "la sentencia SQL: \"%1\"")
                                        .arg(mysql);
                return QPair<QStringList,bool>(QStringList(),false);
            }

           bool isnext = query.next();
           if ( !isnext ) {
            SYD << tr(" (before function) No hay registros para ejecutar "
                   "la sentencia SQL: \"%1\"").arg(mysql);
            return QPair<QStringList,bool>(keys,false);
           }

           QString myvalue = query.record().value(0).toString();


           SYD << tr("ComboWidget::execBeforeFunction:myvalue: \"%1\"")
                  .arg(myvalue);
           SYD << tr("ComboWidget::execBeforeFunction:issign: \"%1\"")
                  .arg(issign);


           SYD << tr("......ComboWidget::execBeforeFunction........BEFOREMYVALUE....issign:|%1|")
                  .arg(issign);
           if (issign ) {
               SYD << tr("......ComboWidget::execBeforeFunction........BEFOREMYVALUE:|%1|")
                      .arg(myvalue);
               SYD << tr("......ComboWidget::execBeforeFunction........BEFOREMYVALUE....keys.at(i):|%1|")
                      .arg(keys.at(i));

               if (myvalue == "true" ) {
                    result.append(keys.at(i));
               }
           }
           else {
            result.append(myvalue);
           }
           i++;

    }

    SYD << tr(".............ComboWidget::execBeforeFunction.....MYBEFORES....(INSIDE)...returning");
    return QPair<QStringList,bool>(result,haszero);

}

