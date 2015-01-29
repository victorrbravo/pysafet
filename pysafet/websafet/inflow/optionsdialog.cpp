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
#include "optionsdialog.h"
#include "SafetYAWL.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QItemSelectionModel>
#include <QGridLayout>
#include <QSpacerItem>
#include "mainwindow.h"
#include "textedit.h"
#include "infloweditqlistwidget.h"


OptionsDialog::OptionsDialog(QWidget *parent)
: QDialog(parent)
{
    setupUi(this);

    //qDebug("...OptionsDialog...connect..(%d)...(%d)",ui.loadButton, this );
    //OptionsDialog *mydlg = this;   
   // connect(checkButton, SIGNAL(clicked()), SLOT(doLoadWidget()) );
    connect(loadButton, SIGNAL(clicked()), SLOT(doLoadWidget()) );
    connect(addButton, SIGNAL(clicked()), SLOT(addWidget()) );
     connect(delButton, SIGNAL(clicked()), SLOT(delWidget()) );
      connect(buttonBox, SIGNAL(accepted()), this, SLOT(doAccept()));
     Q_CHECK_PTR( parent );
     MainWindow* mw = qobject_cast<MainWindow*>(parent);
     Q_CHECK_PTR( mw );
     Q_CHECK_PTR( listWidget );
     listWidget->addItems( mw->plugs() );
     buildTab();
     //setWindowOpacity(0.5);

}

OptionsDialog::~OptionsDialog() {
     //qDebug("...~OptionsDialog();...");
}


void OptionsDialog::doAccept() {
     qDebug("...doAccept...doSaveConf...ok!...(1)");
     doSaveConfs();
     qDebug("...doAccept...doSaveConf...ok!...(2)");
     close();
}

void OptionsDialog::buildTab() {
     MainWindow* mw = qobject_cast<MainWindow*>(parent());
     Q_CHECK_PTR( mw );
      TextEdit* te = mw->centraledit();
     Q_CHECK_PTR( te );
     int ntabs = te->listConfs().count();
     int k = 0;
     qDebug("....te->listConfs().count(): %d", te->listConfs().count());
     for(int i = 0; i < ntabs; i++) {
          QWidget *tab = new QWidget;
          QGridLayout* glayout = new QGridLayout(tab);
          QSpacerItem *verticalSpacer = new QSpacerItem(20, 311, QSizePolicy::Minimum, QSizePolicy::Expanding);
          QGridLayout* glayoutitems = new QGridLayout();


          for( int j = 0; j < te->listConfs().at(i).keys().count(); j++) {
               if ( te->listConfs().at(i).keys().at(j) != "namewidget" ) {
                    QString key = te->listConfs().at(i).keys().at(j);
                    QLabel *plabel = new QLabel("<b>"+ key + "</b>: ");
                    plabel->setGeometry(0,0,100,40);
                    plabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed );
                    glayoutitems->addWidget(plabel, k, 0, Qt::AlignLeft);
                    QWidget *pwidget = getVariantWidget(te->listConfs().at(i).value(key), key);
                    if ( pwidget == NULL ) {
                         SafetYAWL::streamlog << SafetLog::Error << tr("Error en el proceso de creación del widget con el siguiente complemento: \"%1\" (%2)")
                                   .arg(key).arg(k);
                         continue;
                    }
                    glayoutitems->addWidget(pwidget, k, 1, Qt::AlignLeft);
                    k++;
                    widgetmap[te->listConfs().at(i)["namewidget"].toString()][key] = pwidget;
               }
          }
          glayoutitems->addItem(verticalSpacer, k, 0, 1, 1);
          glayout->addLayout(glayoutitems, 0, 0, 1, 1);

          tabOptions->addTab(tab, te->listConfs().at(i)["namewidget"].toString());
     }

}

void OptionsDialog::addTab(const QString& n) {
//     QWidget *tab = new QWidget;
//     QGridLayout* glayout = new QGridLayout(tab);
//     QSpacerItem *verticalSpacer = new QSpacerItem(20, 311, QSizePolicy::Minimum, QSizePolicy::Expanding);
//     QGridLayout* glayoutitems = new QGridLayout();
//
//
//     QLabel *plabel = new QLabel("<b>"+ key + "</b>: ");
//     plabel->setGeometry(0,0,100,40);
//     plabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred );
//     glayoutitems->addWidget(plabel, k, 0, Qt::AlignLeft);
//     QWidget *pwidget = getVariantWidget(te->listConfs().at(i).value(key));
//     if ( pwidget == NULL ) {
//          SafetYAWL::streamlog << SafetLog::Error << tr("Error en el proceso de creación del widget con el siguiente complemento: \"%1\" (%2)")
//                    .arg(key).arg(k);
//          continue;
//     }
//     glayoutitems->addWidget(pwidget, k, 1, Qt::AlignLeft);
//     k++;
//     widgetmap[te->listConfs().at(i)["namewidget"].toString()][key] = pwidget;
//
//
//     glayoutitems->addItem(verticalSpacer, k, 0, 1, 1);
//     glayout->addLayout(glayoutitems, 0, 0, 1, 1);
//
//     tabOptions->addTab(tab, te->listConfs().at(i)["namewidget"].toString());

}

void OptionsDialog::doSaveConfs() {
     MainWindow* mw = qobject_cast<MainWindow*>(parent());
     Q_CHECK_PTR( mw );
     TextEdit* te = mw->centraledit();
     for( int i = 0; i < te->listConfs().count(); i++) {
          QString wkey = te->listConfs().at(i)["namewidget"].toString();
          QMap<QString,QWidget*> mymap =  widgetmap [ wkey ];
          QMap<QString,QVariant>& myconf = const_cast<QMap<QString,QVariant>&>(te->listConfs().at(i));
          for(int j=0; j < mymap.keys().count(); j++) {
               QWidget* pwidget = mymap.value( mymap.keys().at(j));
               qDebug("...doSaveConfs()...(3)...mymap.keys().at(j): %s", qPrintable(mymap.keys().at(j)));
               if (pwidget != NULL) {
//                    qDebug("...doSaveConfs()...(4).i=%d..mymap.keys().at(%d):%s", i, j, qPrintable(mymap.keys().at(j)));
//                    qDebug("...doSaveConfs()...(6)...te->listConfs().at(%d)[\"namewidget\"]=%s",i,
//                           qPrintable(te->listConfs().at(i)["namewidget"].toString()));
//                         qDebug("...doSaveConfs()...(10)...te->listConfs().at(i).count(): %d",
//                                te->listConfs().at(i).count());
                    if ( te->listConfs().at(i)[mymap.keys().at(j)].type() == QVariant::String ) {
 //                        qDebug("...doSaveConfs()...(9)...");
                         QLineEdit* pedit = qobject_cast<QLineEdit*>(pwidget);
                         QString text = "";
                         if ( pedit == NULL) {
                            QComboBox* pcombo = qobject_cast<QComboBox*>(pwidget);                            
                            if ( pcombo == NULL ) {
                                InflowEditQListWidget* editlistwidget = (qobject_cast<InflowEditQListWidget*>(pwidget));
                                Q_CHECK_PTR( editlistwidget );
                                QString text = editlistwidget->editList().join(";");
                                text +=";";
                                myconf[mymap.keys().at(j)] = text;
                                qDebug("...OptionsDialog::doSaveConfs: |%s|",
                                       qPrintable(text));
                                continue;
                            }

                             for(int k=0;k<pcombo->count();k++){
                                    text += pcombo->itemText(k);
                                    if (k == pcombo->currentIndex() ) text += "+";
                                    text += ",";
                             }
                             text.chop(1);                        

           //              qDebug("...doSaveConfs()...(10)...combobox...text: %s", qPrintable(text));
                         myconf[mymap.keys().at(j)] = text;

                         } else {
                             QString text = (pedit)->text();

             //                qDebug("...doSaveConfs()...(8)...lineedit...text: %s", qPrintable(text));

                             myconf[mymap.keys().at(j)] = text;
                         }

                    }
                    else if (te->listConfs().at(i)[mymap.keys().at(j)].type() == QVariant::Int ) {
                         int value = (qobject_cast<QSpinBox*>(pwidget))->value();
                         myconf[mymap.keys().at(j)] = value;
                    }
                    else if (te->listConfs().at(i)[mymap.keys().at(j)].type() == QVariant::Bool ) {
                         Qt::CheckState state  = (qobject_cast<QCheckBox*>(pwidget))->checkState();
                         myconf[mymap.keys().at(j)] = (state == Qt::Checked);
                    }

               }
          }
     } 

}


QWidget* OptionsDialog::getVariantWidget(const QVariant& v, const QString& message) {
     QWidget* pwidget = NULL;
     QLineEdit *plineedit = NULL;
     QSpinBox *pspinbox;
     QDoubleSpinBox *pdoublespinbox;
     QCheckBox *pcheckbox;
     QComboBox *pcombobox;
     bool ok;
     QRect rect;
     QString currentitem;

     QStringList combolist;
     QStringList editlist;
     InflowEditQListWidget *editlistwidget;
     int currenti = -1, i = 0;
     switch( v.type() ) {
          case QVariant::String:
                   combolist = v.toString().split(",", QString::SkipEmptyParts);
                   if (combolist.count() > 1 ) {
                       pwidget = new QComboBox(this);
                       pcombobox = qobject_cast<QComboBox*>(pwidget);
                       Q_CHECK_PTR(pcombobox);
                       for( i = 0; i < combolist.count(); i++) {
                             currentitem = combolist.at(i);
                             if (currentitem.endsWith("+") ) {
                                 currenti = i;
                                 currentitem.chop(1);

                             }
                            pcombobox->addItem(currentitem);
                       }
                       if ( currenti >= 0 && currenti < pcombobox->count() ) {
                           pcombobox->setCurrentIndex( currenti );
                       }
                       break;
                   }

                   if ( v.toString().endsWith(";") ) {
                       editlist = v.toString().split(";", QString::SkipEmptyParts);
                       pwidget = new InflowEditQListWidget(this);
                       editlistwidget = (qobject_cast<InflowEditQListWidget*>(pwidget));
                       Q_CHECK_PTR( editlistwidget );
                       editlistwidget->buildWidget();
                       editlistwidget->setEditList(editlist);
                       break;
                   }
                   pwidget = new QLineEdit(v.toString() );
                   pwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding );
                   //rect = pwidget->geometry();
                   //rect.setWidth(350);
                   //pwidget->setGeometry(rect);
                   if (message.contains(QRegExp(tr("contrase[nñ]a|password|clave|pin"),Qt::CaseInsensitive)) ) {
                        plineedit = qobject_cast<QLineEdit*>(pwidget);
                        Q_CHECK_PTR( plineedit );
                        plineedit->setEchoMode(QLineEdit::Password);
                   }
               break;
          case QVariant::Bool:
                   pwidget = new QCheckBox("",this);
                   pcheckbox = qobject_cast<QCheckBox*>(pwidget);
                   if ( v.toBool() ) {
                         pcheckbox->setCheckState(Qt::Checked);
                   }
                   else {
                        pcheckbox->setCheckState(Qt::Unchecked);
                   }
                   break;
          case QVariant::Int:
                   pwidget = new QSpinBox();
                   pspinbox = qobject_cast<QSpinBox*>(pwidget);
                   pspinbox->setValue( v.toInt() );
               break;
          case QVariant::Double:
                    pwidget = new QDoubleSpinBox();
                    pdoublespinbox = qobject_cast<QDoubleSpinBox*>(pwidget);
                    pdoublespinbox->setValue( v.toDouble(&ok) );
               break;
          case QVariant::Date:
                    break;
          case QVariant::DateTime:
                    break;

          default:
               break;

     }
     return pwidget;

}

void OptionsDialog::doLoadWidget() {

     qDebug("...OptionsDialog::doLoadWidget()...");
     //QMessageBox::information (this,"Cargando...", "Widget");
    QString plugdir = SafetYAWL::getConf()["Plugins/path"];
    qDebug("...plugdir: %s", qPrintable(plugdir));
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Seleccione un complemento"),
                                plugdir,
                                tr("Archivos de complemento (lib*.so);;Todos (*)"));

    if (!fileName.isEmpty()) {
        filenameLine->setText(fileName);
         filenameLine->setFocus( Qt::PopupFocusReason );

    }


}

void OptionsDialog::addWidget() {
     if (filenameLine->text().isEmpty() ) {
          QMessageBox::warning(this, tr("Error al cargar complemento"), tr("Seleccione un archivo de complemento a agregar en \"Agregar Widget\""));
          return;

     }
     if ( !QFile::exists(filenameLine->text() ) ) {
          QMessageBox::warning(this, tr("Error al cargar complemento"), tr("El archivo de complemento: \"%1\" no se puede leer de la ubicación señalada")
                               .arg(filenameLine->text()));
          return;
     }
     if (listWidget->findItems( filenameLine->text(), Qt::MatchExactly ).count() > 0 ) {
          QMessageBox::warning(this, tr("Error al cargar complemento"), tr("El elemento \"%1\" ya está cargado").arg(filenameLine->text()));
          return;

     }

     MainWindow* mw = qobject_cast<MainWindow*>(parent());
     Q_CHECK_PTR( mw );
     if (!mw->loadWidgetPlugins( filenameLine->text(), true ) ) {
          QMessageBox::warning(this, trUtf8("Error al cargar complemento"),
                               tr("El archivo de complemento: \"%1\"\n no tiene un formato de construcción válida")
                               .arg(filenameLine->text()));
          return;

     }

     listWidget->addItem( filenameLine->text() );
     qDebug("...OptionsDialog::addWidget...loadWidgetPlugins...filename: |%s|", qPrintable(filenameLine->text()));


}
void OptionsDialog::delWidget() {
     QList<QListWidgetItem*> items = listWidget->selectedItems();
     if (items.count() == 0 ) {
          QMessageBox::warning(this, tr("Error al eliminar complemento"), tr("No existe un complemento seleccionado en la lista"));
          return;
     }
     QMessageBox msgBox;
     msgBox.setText("Se va(n) a descargar un(os) el(los) complemento(s) seleccionado(s), y desde este momento no estará(n) disponible(s)");
     msgBox.setInformativeText("¿Está seguro de realizar la acción?");
     msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No| QMessageBox::Cancel);
     msgBox.setDefaultButton(QMessageBox::No);
     int ret = msgBox.exec();
     QList<int> numbers;
     if ( ret == QMessageBox::Yes ) {
          qDebug("...descargado complemento...");
          if (SafetYAWL::listDynWidget != NULL  ) {
//               qDebug("...SafetYAWL::listDynWidget...:%d...count:%d", SafetYAWL::listDynWidget, SafetYAWL::listDynWidget->count());
//               qDebug("...SafetYAWL::listPlugins...:%d...count:%d", SafetYAWL::listPlugins, SafetYAWL::listPlugins->count());
//               Q_ASSERT(SafetYAWL::listDynWidget->count() == SafetYAWL::listPlugins.count() );
               for(int i= 0; i < listWidget->selectedItems().count(); i++) {
                    qDebug("...i: %d", i);
                    for(int j = 0; j < SafetYAWL::listDynWidget->count(); j++) {
                         qDebug("..j: %d", j);
                         if (SafetYAWL::listDynWidget->at(j)->path()
                              .compare(listWidget->selectedItems().at(i)->text()) == 0 ) {
                              //SafetYAWL::listPlugins.at(j).unload();
                              numbers.append(j);
                         }
                    }
               }
               qDebug("numbers.count(): %d", numbers.count());
               for(int i=0; i < numbers.count(); i++) {
                    SafetYAWL::listDynWidget->removeAt(numbers.at(i));
                    //SafetYAWL::listPlugins.removeAt(numbers.at(i));
               }
          }
          qDeleteAll(listWidget->selectedItems());
     }
}

