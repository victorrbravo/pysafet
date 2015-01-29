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

#include <QtGui>
#include <QComboBox>
#include <QMessageBox>
#include "listwidget.h"
#include "mainwindow.h"
#include "inflowfirmadoc.h"
//#include "ui_inflowfirmadoc.h"

ListWidget::ListWidget(const QString& s, ListWidget::Type t, QWidget *parent,bool istextparent)
    : CmdWidget(s, parent, istextparent)
{
     varboxlist = NULL;
     signbutton = NULL;
     viewdocbutton = NULL;
     _type = t;
     //configurator = new SafetYAWL();
}

ListWidget::~ListWidget() {

     //if (configurator) delete configurator;
}

void ListWidget::setText(const QString &newText) {
    qDebug("...ListWidget...newText:|%s|", qPrintable(newText));
    QStringList mylist = newText.split(";",QString::SkipEmptyParts);

    qDebug("varboxlist = %p", varboxlist );
    foreach(QString l, mylist) {
        QList<QListWidgetItem *> myitems = varboxlist->findItems(l,Qt::MatchExactly);
        if (myitems.count() > 0 ) {
            myitems.at(0)->setCheckState(Qt::Checked);
        }
    }

}

void ListWidget::updateCombo() {

     switch (type() ) {

          case VariableSafet:
               updateComboVariableSafet();
               break;
         case AutofilterSafet:
              updateComboAutofilterSafet();
              break;
         case RecursivefilterSafet:
             updateComboRecursivefilterSafet();
           break;
         case ListTable:
               updateComboListTable();
               break;
          case ListLiteral:
               updateComboListLiteral();
               break;
          case Flow:
               updateComboFlow();
               break;
     case ConffileSafet:
               updateComboConffileSafet();
               break;

          default:;
     }

}

void ListWidget::updateComboConffileSafet(bool inwidget) {
    QStringList options = conf()["options"].toString().split(":");
         if (options.count() < 1 ) {
         SafetYAWL::streamlog
                 << SafetLog::Warning
                 << tr("IMPORTANTE, No se definió correctamente el campo \"options\" --> \"%1\"").arg(conf()["options"].toString());
         return;
    }

    QString field = options.at(0);
    if (!field.endsWith(".*")) {
        SafetYAWL::streamlog
                << SafetLog::Warning
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
        foreach(QString newitem, _itemvaluelist) {
            QListWidgetItem* listitem = new QListWidgetItem(varboxlist);
           listitem->setCheckState(Qt::Unchecked);
           listitem->setText(newitem);
           //varboxlist->addItem(listitem);
       }
   }

}

bool ListWidget::isValid(QString& value) {

    //if ( !CmdWidget::isValid(value)) return false;


    switch(type()) {
    case ListLiteral:
        if ( !conf().contains("options")) {
            SafetYAWL::streamlog
                    << SafetLog::Error
                    << tr("Falla la validacion del widget combo: \"%1\"")
                    .arg(caption());
            return false;
        }
        generateLiterals(conf()["options"].toString(),false);
                break;
    case ListTable:
        updateComboListTable(false);

        break;
    case VariableSafet:
        updateComboVariableSafet(false);

        break;
    case AutofilterSafet:
        updateComboAutofilterSafet(false);
        break;
   case RecursivefilterSafet:
        updateComboRecursivefilterSafet(false);
        break;
        break;
   case Flow:
        updateComboFlow(false);
        break;
     case ConffileSafet:
        updateComboConffileSafet(false);
        break;
    default:;
    }

    if ( type() == ListLiteral) {

        qDebug("...ListLiteral...itemsRealValueList().count():%d",
               itemsRealValueList().count());
        if ( !itemsRealValueList().contains(value)) {
            return false;
        }
    }
    else if(type() == ListTable) {
        qDebug("value: |%s|",qPrintable(value));
        qDebug("...ListTable...itemsValueList().count():%d",
               itemsValueList().count());

        foreach(QString s, itemsValueList()) {
            qDebug("...s: |%s|",qPrintable(s));
        }

        QStringList myvalues = value.split(";",QString::SkipEmptyParts);
        foreach(QString v, myvalues) {
            QString myrealv = getRealValue(v);
            qDebug("myrealv: |%s|",qPrintable(myrealv));
            if ( !itemsRealValueList().contains(myrealv)) {
                return false;
            }
        }

    }
    else {
        qDebug("...validating:...|%s|",qPrintable(value));
        QStringList myvalues = value.split(";");
        foreach(QString myvalue, myvalues) {
            if ( !itemsValueList().contains(myvalue)) {
                return false;
            }
        }
    }
    return true;
}
void ListWidget::updateVarGlobal(const QString& value) {
    if ( varvalues.keys().contains(value)) {
        qDebug("               *****(1)value: |%s|combovarglobal SafetYAWL::combovarglobal0: |%s|",
               qPrintable(value),qPrintable(SafetYAWL::combovarglobal0));
        SafetYAWL::combovarglobal0 = getVarValue(value);
        qDebug("               *****(2)...value: |%s| combovarglobal SafetYAWL::combovarglobal0: |%s|",
               qPrintable(value),qPrintable(SafetYAWL::combovarglobal0));
    }

}


void ListWidget::updateComboFlow(bool inwidget) {
     infos.clear();

     if ( conf().contains("path") ) {
          if (MainWindow::configurator != NULL ) delete  MainWindow::configurator;
          MainWindow::configurator = new SafetYAWL();
          Q_CHECK_PTR( MainWindow::configurator );

          SafetYAWL::streamlog
                  << SafetLog::Debug
                  << tr("DOMMODEL ComboFlow: path: |%1|").arg(conf()["path"].toString());

          MainWindow::configurator->openXML(conf()["path"].toString());
          MainWindow::configurator->convertXMLtoObjects();
          MainWindow::configurator->openDataSources();

          if ( MainWindow::configurator->getWorkflows().count() > 0 ) {             
               infos = MainWindow::configurator->getWorkflows().at(0)->listNextStates(conf()["keyvalue"].toString());
               if ( infos.count() > 0 ) {
                    if ( infos.at( infos.count()-1) == "<SIGN>" ) {
                         buildButtonWidget(infos);
                         return;
                    }

               }
          }

          _itemvaluelist = infos;
          if ( inwidget ) {
              foreach(QString newitem, infos) {
                  QListWidgetItem* listitem = new QListWidgetItem(newitem);
                  listitem->setFlags(Qt::ItemIsUserCheckable);
                 varboxlist->addItem(listitem);
             }

        }
     }
     foreach(QString s, _itemvaluelist) {
         SafetYAWL::streamlog
                 << SafetLog::Debug
                 << tr("DOMMODEL inwidget: %2 ComboFlow: _itemvaluelist: |%1|").arg(s).arg(inwidget);

     }



}

void ListWidget::buildButtonWidget(const QStringList& l) {
     qDebug("...buildButtonWidget...");
     Q_ASSERT( l.count() > 0 );
     if ( quitbutton ) {
//          close();
          disconnect(quitbutton, 0, 0, 0 );
          delete quitbutton;
          delete okbutton;
          delete varboxlist;
          delete mainLayout;
          if ( checkbutton ) delete checkbutton;
          if ( viewdocbutton ) delete viewdocbutton;
//          setLayout(NULL);
          quitbutton = NULL;
          checkbutton = NULL;
          viewdocbutton = NULL;
          varboxlist = NULL;
          mainLayout = NULL;

          signbutton = new QPushButton;
          mainLayout = new QHBoxLayout;
  //        viewdocbutton  = new QToolButton;
          if (isTextParent()) {
            okbutton = new QToolButton;
            okbutton->setGeometry(0,0,30,36);
              okbutton->setIcon(QIcon(":/yes.png"));
              quitbutton = new QToolButton;
              quitbutton->setText( "X");
          }
          signbutton->setIcon(QIcon(":/firmadoc.png"));
          signbutton->setText(tr("Firmar %1").arg(l.at(0).simplified()));
          mainLayout->addWidget(signbutton);
          // mainLayout->addWidget(viewdocbutton);
//          setLayout(mainLayout);

          if (isTextParent()) {
              mainLayout->addWidget(quitbutton);
              mainLayout->addWidget(okbutton);
              connect(quitbutton, SIGNAL(clicked()), _texteditparent, SLOT(cancelAndClose()) );
              connect(okbutton, SIGNAL(clicked()), _texteditparent, SLOT(insertAndClose()) );
          }

          connect(signbutton, SIGNAL(clicked()), this, SLOT(viewdoc()) );


     }

}

void ListWidget::insertAndClose() {
/*
    QString str;
    if ( _texteditparent ) {
        qDebug("...insertAndClose...");
        if ( varboxlist ) {

            for(int i=0; i < varboxlist->count();i++) {
                QListWidgetItem *myitem = varboxlist->item(i);
                if ( myitem ) {
                    if (myitem->checkState() == Qt::Checked ) {
                        str += myitem->text();
                        str += ";";
                    }
                }
            }
            if (!str.isEmpty()) {
                str.chop(1);
                _texteditparent->insertPlainText(str.toLatin1());
                _texteditparent->insertPlainText("\n");
            }

        }
    }
    close();*/
}


void ListWidget::viewdoc() {

}


void ListWidget::buildWidget() {

     qDebug("...ListWidget::buildWidget()...");
     mainLayout = new QHBoxLayout;
     varboxlist = new QListWidget;
     quitbutton = new QToolButton;
     quitbutton->setText( "X");
     okbutton = new QToolButton;
     //okbutton->setGeometry(0,0,30,36);

     okbutton->setIcon(QIcon(":/yes.png"));

     Q_CHECK_PTR( varboxlist );
     //varboxlist->setEditable(true);
//     varbox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
     mainLayout->addWidget(varboxlist);
     QString mytip = tr("Seleccione uno o varios elementos de la lista");
     if ( conf().contains("validation")) {
         QStringList mylist = conf()["validation"].toString().split("::");
         if (mylist.count() > 1 ) {
             QString usertip = mylist.at(1);
             mytip = usertip;
         }
     }
     _usertooltip = mytip;
     varboxlist->setToolTip(mytip);


     //varboxlist->setGeometry(0,0,100,300);
     mainLayout->addWidget(quitbutton);
     mainLayout->addWidget(okbutton);
     connect(quitbutton, SIGNAL(clicked()), _texteditparent, SLOT(cancelAndClose()) );
     connect(okbutton, SIGNAL(clicked()), _texteditparent, SLOT(insertAndClose()) );
     //mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
//     setLayout(mainLayout);
     updateCombo();

}

void ListWidget::updateComboListLiteral() {
    if ( !conf().contains("options")) {
          return;
      }
      generateLiterals(conf()["options"].toString());



}

void ListWidget::generateLiterals(const QString& s, bool inwidget) {
    qDebug("...ComboWidget...generateLiterals....:%s", qPrintable(s));
    QStringList vars = s.split(",",QString::SkipEmptyParts);
    if ( inwidget && varboxlist == NULL ) {
        return;
    }
    int i = 0;
    _itemvaluelist.clear();
    _itemrealvaluelist.clear();
    foreach(QString r, vars) {
        QString value = r.split("::").at(0);
        QString realvalue;
        QVariant info;


        if ( r.split("::").count() > 1 ) {
            info = r.split("::").at(1);

        }
        if ( r.split("::").count() > 2 ) {
            realvalue = r.split("::").at(2);
        }
        qDebug("...value: %s ... realvalue.:%s..",qPrintable(value),
               qPrintable(realvalue));
        realvalues[value] = realvalue;
        if ( inwidget ) {
            if (value.trimmed().isEmpty()) {
                continue;
            }

            qDebug("...varboxlist...:%p",varboxlist);
            QListWidgetItem* listitem = new QListWidgetItem(varboxlist);
            qDebug("...listitem...:%p",listitem);
            listitem->setToolTip(info.toString());
            listitem->setCheckState(Qt::Unchecked);
            listitem->setText(value);
        }
        _itemvaluelist.append(value);
        _itemrealvaluelist.append(realvalue);
        i++;
    }

}

QString ListWidget::getRealValue(const QString& s) {

    if ( !realvalues.keys().contains(s)) {
        return s;
    }
    return realvalues[ s];
}

QString ListWidget::getVarValue(const QString& s) {

    if ( !varvalues.keys().contains(s)) {
        return s;
    }
    return varvalues[ s];
}

void ListWidget::updateComboListTable(bool inwidget) {

     QSqlQuery query(SafetYAWL::currentDb ); // <-- puntero a db actual
     QString field, table, where, value;
     if ( !conf().contains("options")) return;

     QStringList options = conf()["options"].toString().split(":");
          if (options.count() < 2 ) {
          SafetYAWL::streamlog
                  << SafetLog::Warning
                  << tr("IMPORTANTE, No se definio correctamente el campo \"options\" --> \"%1\"").arg(conf()["options"].toString());
          return;
     }
     field = options.at(0);

     QString realvalue;
     _itemrealvaluelist.clear();
     _itemvaluelist.clear();
     table = options.at(1);
     QString command;
     if ( options.count() > 2 ) {
          where = options.at(2);
          if ( options.count() > 3 ) {
              QString titleField = options.at(3);
              if ( options.count() >  4 ) {
                  QString varglobal = options.at(4);
                  if ( !titleField.isEmpty()) {
                command = QString("SELECT %1,%4,%5 from %2 %3;")
                          .arg(field).arg(table)
                          .arg(where)
                          .arg(titleField)
                          .arg(varglobal);
                }
                  else {
                      command = QString("SELECT %1,'n/a' as safetnotapplicable,%4 from %2 %3;")
                                .arg(field).arg(table)
                                .arg(where)
                                .arg(varglobal);
                  }

              }
              else {
                  if ( !titleField.isEmpty()) {
                command = QString("SELECT %1,%4 from %2 %3;")
                          .arg(field).arg(table)
                          .arg(where)
                          .arg(titleField);
                }
                  else {
                      command = QString("SELECT %1 from %2 %3;")
                                .arg(field).arg(table)
                                .arg(where);
                  }
            }

          }
          else {
            command = QString("SELECT %1 from %2 %3;").arg(field).arg(table).arg(where);
          }
     }
     else {
          command = QString("SELECT %1 from %2;").arg(field).arg(table);
     }

     SafetYAWL::streamlog << SafetLog::Debug << tr("Actualizando lista del combo: \"%1\"").arg( command );
     query.prepare(  command );
     bool executed = query.exec();
     if (!executed ) {
          SafetYAWL::streamlog << SafetLog::Warning << tr("IMPORTANTE, NO se ejecutó correctamente la consulta de opciones del combo: \"%1\"").arg(command);
          return;
     }
     bool istitlefield  = query.record().count() > 1 && query.record().fieldName(1) != "safetnotapplicable";

     bool isvarglobal = query.record().count() > 2;

//     qDebug(".....istitlefield: %d", istitlefield);
//     qDebug(".....isvarglobal: %d", isvarglobal);
//     qDebug(".....command: %s", qPrintable(command));
//     qDebug(".....query.record().count(): %d", query.record().count());
//     qDebug();
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

         foreach(QString newitem, _itemvaluelist) {
             if (newitem.trimmed().isEmpty()) {
                 continue;
             }
             QListWidgetItem* listitem = new QListWidgetItem(varboxlist);
             listitem->setCheckState(Qt::Unchecked);

                listitem->setText(newitem);

             //varboxlist->addItem(listitem);
         }

     }
 }



void ListWidget::updateComboVariableSafet(bool inwidget) {
    if (MainWindow::configurator == NULL ) {
        return;
    }
     if ( conf().contains("path") ) {
          delete  MainWindow::configurator;
          MainWindow::configurator = new SafetYAWL();
          Q_CHECK_PTR( MainWindow::configurator );

          qDebug("...path: %s", qPrintable(conf()["path"].toString()));
          QString myautofilter;
          QString myrecursivefilter;
          MainWindow::configurator->openXML(conf()["path"].toString());
          if ( conf().contains("autofilter")) {
              myautofilter = conf()["autofilter"].toString().trimmed();
              qDebug("        ****ComboWidget::updateComboVariableSafet autofilter: |%s|",
                     qPrintable(myautofilter));
              qDebug("        ****ComboWidget::updateComboVariableSafet conf()[\"path\"].toString(): |%s|",
                     qPrintable(conf()["path"].toString()));
              MainWindow::configurator->setAutofilters(myautofilter);
          }

          if ( conf().contains("recursivefilter")) {
              myrecursivefilter = conf()["recursivefilter"].toString().trimmed();
              qDebug("        ****ComboWidget::updateComboVariableSafet autofilter: |%s|",
                     qPrintable(myrecursivefilter));
              MainWindow::configurator->setRecursivefilters(myrecursivefilter);
          }

          MainWindow::configurator->openDataSources();
          MainWindow::configurator->convertXMLtoObjects();


          if ( MainWindow::configurator->getWorkflows().count() > 0 ) {
               _itemvaluelist = MainWindow::configurator->getWorkflows().at(0)->variablesId().toList();
          }
          if ( inwidget ) {
              foreach(QString newitem, _itemvaluelist) {
                  QListWidgetItem* listitem = new QListWidgetItem(newitem);
                  listitem->setFlags(Qt::ItemIsUserCheckable);
                  varboxlist->addItem(listitem);
              }
          }
     }
     qDebug("...(4)....***...updateComboVariableSafet....updateComboVar()....");
}



void ListWidget::updateComboRecursivefilterSafet(bool inwidget) {
    if (MainWindow::configurator == NULL ) return;
    if ( conf().contains("path") ) {
         delete  MainWindow::configurator;
         MainWindow::configurator = new SafetYAWL();
         Q_CHECK_PTR( MainWindow::configurator );

         qDebug("...path: %s", qPrintable(conf()["path"].toString()));
         MainWindow::configurator->openXML(conf()["path"].toString());

         MainWindow::configurator->convertXMLtoObjects();

         MainWindow::configurator->openDataSources();

         if ( MainWindow::configurator->getWorkflows().count() > 0 ) {
             _itemvaluelist = MainWindow::configurator->getWorkflow()->recursivefiltersId().toList();
         }

         if ( inwidget ) {
             foreach(QString newitem, _itemvaluelist) {
                 QListWidgetItem* listitem = new QListWidgetItem(newitem);
                 listitem->setFlags(Qt::ItemIsUserCheckable);
                 varboxlist->addItem(listitem);
             }
         }
    }

}


void ListWidget::updateComboAutofilterSafet(bool inwidget) {
     if (MainWindow::configurator == NULL ) return;
     qDebug("...***....(1)....updateComboAutofilterSafet....updateCombo()....");
     if ( conf().contains("path") ) {
          delete  MainWindow::configurator;
          MainWindow::configurator = new SafetYAWL();
          Q_CHECK_PTR( MainWindow::configurator );

          qDebug("...path: %s", qPrintable(conf()["path"].toString()));
          MainWindow::configurator->openXML(conf()["path"].toString());

          MainWindow::configurator->convertXMLtoObjects();
          MainWindow::configurator->openDataSources();

          if ( MainWindow::configurator->getWorkflows().count() > 0 ) {
               _itemvaluelist = MainWindow::configurator->getWorkflow()->autofiltersId().toList();
          }


          if ( inwidget ) {
              foreach(QString newitem, _itemvaluelist) {
                  QListWidgetItem* listitem = new QListWidgetItem(newitem);
                  listitem->setFlags(Qt::ItemIsUserCheckable);
                  varboxlist->addItem(listitem);
              }
          }
     }
     qDebug("...(4)....***...updateComboVariableSafet....updateComboVar()....");
}


QString ListWidget::text() const {
     Q_CHECK_PTR( varboxlist );
     QString str;
     if (varboxlist ) {
         for(int i=0; i < varboxlist->count();i++) {
             QListWidgetItem *myitem = varboxlist->item(i);
             if ( myitem ) {
                 if (myitem->checkState() == Qt::Checked ) {
                     str += myitem->text();
                     str += ";";
                 }
             }
         }
         if (!str.isEmpty()) {
             str.chop(1);
             return str;
         }

     }

     return QString("");
}



void ListWidget::setFocus ( Qt::FocusReason reason ) {
/*     if ( varboxlist == NULL ) {
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
          varboxlist->setFocus( reason);
     }
*/
}

