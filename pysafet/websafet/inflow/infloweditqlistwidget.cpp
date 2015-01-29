#include <QPushButton>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QGroupBox>

#include "infloweditqlistwidget.h"

InflowEditQListWidget::InflowEditQListWidget(QWidget *parent):
        CmdWidget(QString(""),parent),
list(0),addButton(0), delButton(0), editButton(0) {

  _parent = qobject_cast<QWidget*>(parent);
  Q_CHECK_PTR( _parent );

}



void InflowEditQListWidget::buildWidget() {
    qDebug("...InflowEditQListWidget::buildWidget()...");
    mainLayout = new QGridLayout(this);
    gridLayout = new QGridLayout;

    horizontalLayout = new QHBoxLayout;
    horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    addButton = new QToolButton(this);
    addButton->setEnabled(true);
    addButton->setStyleSheet(QString::fromUtf8("QToolButton { \n"
                                               "     font: bold 10px;\n"
                                               "	 color: green;\n"
                                               "     border: 1px solid #8f8f91;\n"
                                               "     border-radius: 4px;\n"
                                               "     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                               "                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
                                               " }"
                                               "\n QToolButton:pressed {"
                                               "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                               "stop: 0 #dadbde, stop: 1 #f6f7fa);}"                                               
                                               ));

    //waddButton->setText("+");
    addButton->setToolTip(trUtf8("Agregar"));
    horizontalLayout->addWidget(addButton);

    delButton = new QToolButton(this);
    delButton->setToolTip(trUtf8("Eliminar"));
    delButton->setStyleSheet(trUtf8("QToolButton { \n"
                                               "     font: bold 10px;\n"
                                               "	 color: red;\n"
                                               "     border: 1px solid #8f8f91;\n"
                                               "     border-radius: 4px;\n"
                                               "     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                               "                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
                                               " }"
                                               "\n QToolButton:pressed {"
                                               "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                               "stop: 0 #dadbde, stop: 1 #f6f7fa);\n}"
                                               "\n QToolButton:disabled {"
                                               "color: white;"
                                               "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                               "stop: 0 #dadbde, stop: 1 #f6f7fa);}"

                                               ));

   //delButton->setText("-");
    horizontalLayout->addWidget(delButton);

    editButton = new QToolButton(this);
    editButton->setToolTip(trUtf8("Editar/Cambiar"));
    editButton->setStyleSheet(QString::fromUtf8("QToolButton { \n"
                                                "     font: bold 10px;\n"
                                                "	 color: green;\n"
                                                "     border: 1px solid #8f8f91;\n"
                                                "     border-radius: 4px;\n"
                                                "     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
                                                "                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
                                                " }"
                                               "\n QToolButton:pressed {"
                                               "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                               "stop: 0 #dadbde, stop: 1 #f6f7fa);}"
                                               "\n QToolButton:disabled {"
                                               "color: white;"
                                               "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                               "stop: 0 #dadbde, stop: 1 #f6f7fa);}"
                                               ));

    //editButton->setText(trUtf8("*"));
    horizontalLayout->addWidget(editButton);

    gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

     QSize sizeButton;
     sizeButton.setHeight(12);
     sizeButton.setWidth(12);
     addButton->setIconSize( sizeButton);
     delButton->setIconSize( sizeButton);
     editButton->setIconSize( sizeButton);

    list = new QListWidget();
    QString mytip = tr("Seleccione uno o varios elementos de la lista");
    if ( conf().contains("validation")) {
        QStringList mylist = conf()["validation"].toString().split("::");
        if (mylist.count() > 1 ) {
            QString usertip = mylist.at(1);
            mytip = usertip;
        }
    }
    _usertooltip = mytip;
    list->setToolTip(mytip);

    gridLayout->addWidget(list, 0, 0, 1, 1);
    mainLayout->addLayout(gridLayout, 0, 0, 1, 1);
    setLayout(mainLayout);

    editButton->setEnabled(false);

    delButton->setEnabled(false);    

    addButton->setIcon(QIcon(":/add.png"));
    editButton->setIcon(QIcon(":/edit.png"));
    delButton->setIcon(QIcon(":/del.png"));

    connect(addButton, SIGNAL(clicked()), this, SLOT(addItem()) );
    connect(delButton, SIGNAL(clicked()), this, SLOT(delItem()) );
    connect(editButton, SIGNAL(clicked()), this, SLOT(editItem()) );
    connect(list, SIGNAL(itemSelectionChanged ()), this, SLOT(areItemsEnabled()) );
}




InflowEditQListWidget::~InflowEditQListWidget(){

}


void InflowEditQListWidget::delItem() {    
    QList<QListWidgetItem *> curitems = list->selectedItems();
//    Q_CHECK_PTR (cur );
    Q_CHECK_PTR(list);
    int result = QMessageBox::question(this, trUtf8("Eliminar elemento(s)"),
                                          trUtf8("¿Realmente desea eliminar el(los) elemento(s) seleccionado(s)?"),
                                           QMessageBox::Yes | QMessageBox::No);
     if ( result == QMessageBox::Yes ) {
         qDebug("...list->removeItemWidget(cur);...curitems.count(): %d",curitems.count());
         foreach(QListWidgetItem* item, curitems) {
             qDebug("removeitem...");
             int row = list->row(item);
             Q_ASSERT( row >= 0 );
             list->takeItem(row);
         }
     }
}

QString  InflowEditQListWidget::genPasswordMask(const QString& s) const {
    QString result;
    for(int i = 0; i < s.count(); i++) {
         result.append("*");
    }
    return result;

}

void InflowEditQListWidget::setEditList(const QStringList& l) {
    if (list) {
        list->clear();
        QString pass, plain;
        foreach(QString s, l) {
            QListWidgetItem *pitem = new QListWidgetItem(s,list);
            Q_CHECK_PTR(pitem);
            plain = s.mid(0, s.length()/2);
            pass = s.mid(s.length()/2);
            //qDebug("...plain: |%s| ... pass: |%s|", qPrintable(plain), qPrintable(pass));
            if (pass  == genPasswordMask(plain)  ) {
                pitem->setText(pass);
                pitem->setData(Qt::UserRole,QVariant(plain));
            }
             list->addItem(pitem);
         }

        }

}

void InflowEditQListWidget::addItem() {
    Inflow::ListDialog dialog(this);
    QList<QPair<QString,QString> > values;
    values.append( QPair<QString,QString>("Clave (Registro)", "<RegisterKey>") );
    values.append( QPair<QString,QString>("Ruta de la imagen/video", "<MediaFilePath>") );
    dialog.setPredefinedValues(values);
    dialog.buildDialog();
    dialog.setInfo();
    int result = dialog.exec();

    if (result == QDialog::Accepted ) {
        QListWidgetItem *newitem = new QListWidgetItem(dialog.editinfo(),list);
        Q_CHECK_PTR( newitem);
        if ( !dialog.editinfo().isEmpty() ) {
            if (dialog.isPassword() ) {
                 newitem->setText(genPasswordMask(dialog.editinfo()));
                 newitem->setData(Qt::UserRole, QVariant(dialog.editinfo()) );
            }
            list->addItem( newitem );

        }
    }
}

void InflowEditQListWidget::editItem() {
    Inflow::ListDialog dialog(this);
    QList<QPair<QString,QString> > values;
    values.append( QPair<QString,QString>("Clave (Registro)", "<RegisterKey>") );
    values.append( QPair<QString,QString>("Ruta de la imagen/video", "<MediaFilePath>") );
    dialog.setPredefinedValues(values);
    dialog.buildDialog();
    dialog.setInfo(trUtf8("Editar"),trUtf8("Cambie el texto:"));
    QString s;
    QListWidgetItem *cur  =  list->currentItem();
    Q_ASSERT (cur != NULL );
    dialog.setEditinfo(cur);
    int result = dialog.exec();
    if ( result == QDialog::Accepted ) {
        if ( !dialog.editinfo().isEmpty() ) {
            if (dialog.isPassword() ) {
                 cur->setText(genPasswordMask(dialog.editinfo()));
                 cur->setData(Qt::UserRole, QVariant(dialog.editinfo()) );
            }
            else {
                cur->setText(dialog.editinfo());
                cur->setData(Qt::UserRole, QVariant(QString("")) );
            }
            list->editItem(cur);
        }
    }

}

void InflowEditQListWidget::areItemsEnabled() {
     QList<QListWidgetItem *> curitems = list->selectedItems();
     qDebug("...InflowEditQListWidget::areItemsEnabled...curitems.count(): %d",curitems.count());
    if ( curitems.count() == 1  ) {
          editButton->setEnabled(true);
          delButton->setEnabled(true);
    }    
    else if (curitems.count()  > 1) {
          editButton->setEnabled(false);
          delButton->setEnabled(true);

    }
    else if (curitems.count()  == 0) {
          editButton->setEnabled(false);
          delButton->setEnabled(false);
    }
}

QStringList InflowEditQListWidget::editList(bool withpass) const {
    QStringList result;
    for(int i = 0; i < list->count(); i++) {
        QString pass = list->item(i)->data(Qt::UserRole).toString();
        if ( !pass.isEmpty()) {
            if ( withpass ) {
                QString newpass = pass+genPasswordMask(pass);
                result.append(newpass);
            } else {
                result.append(pass);
            }
        } else {
            result.append(list->item(i)->text());
        }
    }
    return result;
}


Inflow::ListDialog::ListDialog(QWidget* parent, Qt::WindowFlags flags )
: QDialog(parent,Qt::Dialog) {
        groupButton = NULL;
        editInfo = NULL;
}

void Inflow::ListDialog::setEditinfo(QListWidgetItem* item) {
    Q_CHECK_PTR( item );
    if (editInfo) {
        QString pass = item->data(Qt::UserRole).toString();
        if ( !pass.isEmpty() ) {
            editInfo->setText(pass);
            checkInfo->setCheckState(Qt::Checked);
        } else {
            editInfo->setText(item->text());
        }

    }
 }

void Inflow::ListDialog::showEvent ( QShowEvent * event ) {
    QDialog::showEvent(event);
    QRect currentRect = geometry();
    //setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    qDebug("...Inflow::ListDialog::showEvent...");
    //setMaximumSize(currentRect.width(),currentRect.height());
    setMaximumHeight(currentRect.height());
    setMinimumSize(currentRect.width(),currentRect.height());

}

void Inflow::ListDialog::setInfo(const QString &title, const QString &info, const QString&helpmessage ) {
    QString mytitle, mymsg;
    mytitle = title;
    mymsg = helpmessage;
    if (mymsg.isEmpty() ) {
        mymsg = title;
    }
    setWindowTitle(mytitle);
    labelTypeInfo->setText(mymsg);
    labelInfo->setText(info);



}


void Inflow::ListDialog::buildDialog() {

    gridLayout = new QGridLayout(this);

    labelTypeInfo = new QLabel(this);
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    labelTypeInfo->setFont(font);
    labelTypeInfo->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    labelTypeInfo->setText("Agregar");
    gridLayout->addWidget(labelTypeInfo, 0, 0, 1, 1);

    verticalLayout = new QVBoxLayout();
    labelInfo = new QLabel(this);
    labelInfo->setText("Ingrese el texto a agregar");
    verticalLayout->addWidget(labelInfo);

    editInfo = new QLineEdit(this);

//    QRect editRect = editInfo->geometry();
//    editRect.setWidth(editRect.width()+150);
//    editInfo->setGeometry(editRect);
    verticalLayout->addWidget(editInfo);

    checkInfo = new QCheckBox(this);

    verticalLayout->addWidget(checkInfo);
    if (  predefinedValues.count() > 0 ) {
         QGroupBox *groupBox = new QGroupBox(trUtf8("Valores predefinidos"));
         groupButton = new QButtonGroup();
         Q_CHECK_PTR( groupButton );
         Q_CHECK_PTR( groupBox );
          QVBoxLayout *vbox = new QVBoxLayout;
          Q_CHECK_PTR( vbox );
        for(int i = 0; i < predefinedValues.count(); i++) {
            QCheckBox *newcheck = new QCheckBox(predefinedValues.at(i).first,this);
            vbox->addWidget(newcheck);
            groupButton->addButton(newcheck,i);
        }
        groupButton->setExclusive( false );
        connect(groupButton, SIGNAL(buttonClicked(int)),this,SLOT(predefineButtonClicked(int)));
        vbox->addStretch(1);
        groupBox->setLayout(vbox);
        verticalLayout->addWidget(groupBox);
    }
    checkInfo->setText(trUtf8("Modo Contraseña"));

    gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    gridLayout->addWidget(buttonBox, 2, 0, 1, 1);
    connect(checkInfo, SIGNAL(toggled(bool)), this, SLOT(setEditInfoType(bool)) );
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);

    connect(okButton, SIGNAL(clicked()),this,SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()),this,SLOT(reject()));

    editInfo->setFocus();

}
void Inflow::ListDialog::predefineButtonClicked(int id ) {
    Q_CHECK_PTR( editInfo );
    QCheckBox *curcheck = qobject_cast<QCheckBox *>(groupButton->button(id));
    Q_CHECK_PTR( curcheck );
    qDebug("....predefineButtonClicked:id: |%d|",id);
    if ( curcheck->checkState() == Qt::Checked ) {
        if (id < predefinedValues.count() ) {
            editInfo->setText(   predefinedValues.at( id ).second );
            editInfo->setEnabled( false);
            for( int i = 0; i < predefinedValues.count() ; i++ ) {
                if ( i != id ) {
                    QCheckBox *check = qobject_cast<QCheckBox *>(groupButton->button(i));
                    if (check) check->setCheckState( Qt::Unchecked );
                }
            }
        }
    } else {
       editInfo->setText("");
       editInfo->setEnabled( true);
    }

}
void Inflow::ListDialog::setEditInfoType(bool state) {
        qDebug("...Inflow::ListDialog::setEditInfoType...");
        editInfo->setEchoMode(state?QLineEdit::Password:QLineEdit::Normal);
}


