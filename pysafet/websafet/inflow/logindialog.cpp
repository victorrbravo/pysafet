#include "logindialog.h"
#include "ui_logindialog.h"
#include "SafetYAWL.h"
#include "mainwindow.h"
#include <QCryptographicHash>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    loadInfo();
    loadAuthorization();
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(doAccept()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(doCancel()));
    connect(ui->comboUsers,SIGNAL(currentIndexChanged (int)),ui->linePassword,SLOT(clear()));
    checkInputFiles();

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::checkInputFiles() {
    QString info;
    if ( MainWindow::mymainwindow != NULL ) {
        int i = 0;
        foreach(QString s, MainWindow::mymainwindow->getFileCompleters()) {
            if ( !SafetYAWL::checkDTD(qPrintable(s)) ) {
                SafetYAWL::streamlog
                        << SafetLog::Error
                        << trUtf8("Ocurrió un error de validación XML/DTD para el archivo \"%1\"")
                        .arg(s);
                qDebug("Error de validacion DTD: |%s|",qPrintable(s));
//                info += trUtf8("<b>Error de validación <b>XML/DTD</b> :\"%1\"+<br/>")
//                        .arg(s);
                i++;

            }
        }
        if ( i == 0 ) {
            info = "Archivos de entrada correctos...ok!";
            ui->validmessageLabel->setText(info);
            ui->validmessageLabel->setVisible(false);


        }
        else {
            info = trUtf8("Archivos de configuración <b>(~/.safet/input)</b> incorrectos.<br/>"
                   "Revise el registro (~/.safet/log/safet.log)<br/> "
                   "para detalles de el(los) error(es).<br/>"
                   "Uno o varios módulos no funcionarán correctamente.");
            QString mystylered = "background-color: rgb(255, 255, 255);"
            "color: rgb(220, 0, 0);"
            "font: 8pt \"DejaVu Sans\";";
            ui->validmessageLabel->setStyleSheet(mystylered);
            ui->validmessageLabel->setText(info);
        }
    }




}

void LoginDialog::loadInfo() {



    int countuser = 1;

    while ( true ) {
        QString account = SafetYAWL::getAuthConf()[QString("Auth/account.%1").arg(countuser)];
        //qDebug("...account...keys: %d", SafetYAWL::getAuthConf().keys().count());
        //qDebug("...account: |%s|",qPrintable(account));
        if ( account.trimmed().length() == 0 ) {
            break;
        }
        QString pass = SafetYAWL::getAuthConf()[QString("Auth/pass.%1").arg(countuser)];
        QString role = SafetYAWL::getAuthConf()[QString("Auth/role.%1").arg(countuser)];
        QString realname = SafetYAWL::getAuthConf()[QString("Auth/realname.%1").arg(countuser)];
        if ( pass.trimmed().length() > 0 ) {
            QStringList mylist;
            mylist.append(pass);
            mylist.append(role);
            mylist.append(realname);
            users[ account ] = mylist;
        }

        ui->comboUsers->addItem(QIcon(":/ico_person_blue_16.png"),account);
        countuser++;

    }

    ui->linePassword->setFocus();
}


void LoginDialog::loadAuthorization() {

      int countperm = 1;
    while ( true ) {
        QString operation =  SafetYAWL::getAuthConf()[QString("Permises/operation.%1").arg(countperm)];
        QString accounts = SafetYAWL::getAuthConf()[QString("Permises/accounts.%1").arg(countperm)];
        QString types =  SafetYAWL::getAuthConf()[QString("Permises/types.%1").arg(countperm)];
        QString roles =  SafetYAWL::getAuthConf()[QString("Permises/roles.%1").arg(countperm)];
        QString thesign;
        if (operation.trimmed().length() == 0 ) {
            break;
        }
        QStringList info;

        info.append(accounts);
        info.append(types);
        info.append(roles);
        info.append(thesign);

        MainWindow::permises[ operation ] = info;
        countperm++;

    }

}

void LoginDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void LoginDialog::doCancel() {
    qDebug("...doCancel....");
    reject();
}

void LoginDialog::doAccept() {
    qDebug("...doAccept....");
    QString curuser = ui->comboUsers->currentText();
    if ( !users.contains(curuser) ) {
        QMessageBox::information(this,
                                              trUtf8("SAFET - Entrada"),
                                              trUtf8("El archivo de configuración de usuarios es inválido"),
                                                  QMessageBox::Ok  );
        return;

    }
    Q_ASSERT( users[curuser].count() > 0 );
    QByteArray textvalue = QByteArray(qPrintable(ui->linePassword->text()));
    QByteArray hashvalue = QCryptographicHash::hash( textvalue, QCryptographicHash::Sha1 );
    if ( users[curuser].at(0) != QString(hashvalue.toHex().data()) ) {
        QMessageBox::warning(this,
                                              trUtf8("SAFET - Entrada"),
                                              trUtf8("La contraseña para el usuario \"%1\" es incorrecta")
                                              .arg(ui->comboUsers->currentText()),
                                                  QMessageBox::Ok  );
        ui->linePassword->clear();
        return;
    }

    MainWindow::currentaccount = curuser;
    SafetYAWL::setCurrentAuthUser(MainWindow::currentaccount);
    SafetYAWL::streamlog
            << SafetLog::Action
            << tr("El usuario (sistema de autorizacion a cambiado a \"%1\"")
            .arg(SafetYAWL::currentAuthUser());

    if ( users[curuser].count() > 1 ) {
        MainWindow::currentrole = users[curuser].at(1);
        if ( users[curuser].count() > 2 ) {
            MainWindow::currentrealname = users[curuser].at(2);
            qDebug("...realname:%s", qPrintable(MainWindow::currentrealname ));
        }
    }
    else {
        MainWindow::currentrole = "";
    }

     accept();
}
