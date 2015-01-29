




#include "sendsigndocumentdialog.h"
#include "ui_sendsigndocumentdialog.h"
#include "SafetYAWL.h"
#include "SslCertificate.h"
//#include "DigiDoc.h"
#include "SafetDocument.h"
#include <QDesktopServices>
#include <QUrl>
 extern "C" {
#include "libdotar.h"
 }



// =======================================================================================================
// =============FUNCIONES ZLIB (COMPRESION) ==========================================================================================

// =======================================================================================================

SendSignDocumentDialog::SendSignDocumentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendSignDocumentDialog)
{
    ui->setupUi(this);
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(sendFiles()) );
    connect(ui->selectButton, SIGNAL(clicked()), this, SLOT(selectFiles()) );
    connect(ui->delButton, SIGNAL(clicked()), this, SLOT(delFiles()) );
    connect(ui->getContainerButton, SIGNAL(clicked()), this, SLOT(getContainerFiles()) );
     connect(ui->listFiles, SIGNAL(itemSelectionChanged()), this, SLOT(showFileInfo()) );
}




void SendSignDocumentDialog::sendFiles() {

    QStringList options;
    options.append(trUtf8("Enviar por correo-e"));
    options.append(trUtf8("Crear archivo comprimido .tar.gz"));


    bool ok;
    qDebug("...option..............");
    QString option = QInputDialog::getItem(this, tr("Seleccionar"),
                                             tr("Contenedor:"), options, 0, false, &ok);

    qDebug("...option:%s", qPrintable(option));
    if ( option.isEmpty()) {
        return;

    }

    if ( option == trUtf8("Enviar por correo-e") ) {
        QUrl url;
        url.setScheme( "mailto" );
        url.addQueryItem( "subject", trUtf8("Safet/Inflow: envío de archivos firmados formato XADES") );


        for( int i=0; i<ui->listFiles->count();i++){
            QListWidgetItem* item = ui->listFiles->item(i);

            QString filename = item->text();
            if ( QFile::exists(filename)) {
                url.addQueryItem( "attachment", filename );
                int row = ui->listFiles->row(item);
                ui->listFiles->takeItem(row);
                i--;
            }
        }
        QDesktopServices::openUrl( url );
    }
    else /* Archivo comprimido */ {
        qDebug(".......archivo comprimido.........");
        QStringList files;
        for( int i=0; i<ui->listFiles->count();i++){
            QListWidgetItem* item = ui->listFiles->item(i);

            QString filename = item->text();
            if ( QFile::exists(filename)) {
                files.append(filename);
                int row = ui->listFiles->row(item);
                ui->listFiles->takeItem(row);
                i--;
            }
        }
        QString types = tr("Archivos Comprimidos TAR(*.tar);;Todos (*)");

        QString fileName = QFileDialog::getSaveFileName(this,
                                                tr("Seleccione un directorio y un nombre de archivo"),
                                                QDir::homePath()+"/datosfirmados.tar",
                                                types);


        SendSignDocumentDialog::compresstar(files,fileName);

    }

    if ( ui->listFiles->count() == 0 ) {
        ui->sendButton->setEnabled(false);
    }

}

void  SendSignDocumentDialog::delFiles() {
    QList<QListWidgetItem *> curitems = ui->listFiles->selectedItems();

    int result = QMessageBox::question(this, trUtf8("Eliminar archivos(s)"),
                                          trUtf8("¿Realmente desea eliminar el(los) elemento(s) seleccionado(s)?"),
                                           QMessageBox::Yes | QMessageBox::No);
     if ( result == QMessageBox::Yes ) {
         foreach(QListWidgetItem* item, curitems) {
             qDebug("removeitem...");
             int row = ui->listFiles->row(item);
              ui->listFiles->takeItem(row);
         }
     }
     if ( ui->listFiles->count() > 0 )  {
         ui->sendButton->setEnabled(true);
         ui->delButton->setEnabled(true);
     }
     else {
         ui->sendButton->setEnabled(false);
         ui->delButton->setEnabled(false);
     }
}

void SendSignDocumentDialog::selectFiles() {

    QStringList files;
   QString types = tr("Archivos Firmados XADES (*.ddoc);;Todos (*)");


    QString curdir = QDir::homePath()+ "/" + Safet::datadir;

    files = QFileDialog::getOpenFileNames(this,tr("Seleccione archivo(s) firmados XADES a incluir"),
                                          curdir,types);


    if (files.count() > 0 ) {
        foreach(QString f, files ) {
            if ( !searchInListNameFile(f)) {
                ui->listFiles->addItem(f);
            }
        }
        ui->sendButton->setEnabled(true);
        ui->delButton->setEnabled(true);
    }
    else {
        ui->sendButton->setEnabled(false);
        ui->delButton->setEnabled(false);
    }

}

bool SendSignDocumentDialog::searchInListNameFile(const QString& n) {
    for( int i=0; i<ui->listFiles->count();i++){
        QListWidgetItem* item = ui->listFiles->item(i);
        if (item->text() == n ) return true;
    }

    return false;
}

void SendSignDocumentDialog::getContainerFiles() {

    QStringList containers;
    QString targetdir = SafetYAWL::getConf()["XmlRepository/path"];
    qDebug("...Directorio destino: %s",qPrintable(targetdir));

    QDir dir(targetdir);
    dir.setFilter(QDir::Dirs |  QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    qDebug("     Contenedores ");
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString namedir = fileInfo.fileName();
        if ( !namedir.startsWith(".")) {
            qDebug("Directorio: %s",qPrintable(QString("%1").arg(namedir)));
            containers.append(namedir);
        }
    }

    bool ok;
    QString curcontainer = QInputDialog::getItem(this, tr("Seleccionar"),
                                             tr("Contenedor:"), containers, 0, false, &ok);
     if (!ok || curcontainer.isEmpty()) {
        return;
    }
    // agregar a la lista los archivos del contenedor
     QDir containerdir(targetdir+ "/" + curcontainer);
     containerdir.setFilter(QDir::Files |  QDir::NoSymLinks);
     containerdir.setSorting(QDir::Size | QDir::Reversed);

     QFileInfoList fileslist = containerdir.entryInfoList();
     qDebug("     archivos... ");
     for (int i = 0; i < fileslist.size(); ++i) {
         QFileInfo fileInfo = fileslist.at(i);
         QString namedir = fileInfo.absoluteFilePath();
         if ( !namedir.startsWith(".") && !searchInListNameFile(namedir)) {
                ui->listFiles->addItem(namedir);
         }
     }


}

/*
void SendSignDocumentDialog::showFileInfo() {

    QListWidgetItem* item;
    DigiDoc *myfile;

    if ( ui->listFiles->selectedItems().count() > 0 ) {
        item = ui->listFiles->selectedItems().at(0);
            QString filename = item->text();
            qDebug("...showFileInfo: %s", qPrintable(filename));
            myfile = new DigiDoc(this);

            if ( QFile::exists(filename)) {
                myfile->open( filename ) ;

                QString textinfo;

                for (int i = 0; i < myfile->signatures().count();i++) {
                    DigiDocSignature signature = myfile->signatures().at(i);
                    const SslCertificate c = signature.cert();
                    textinfo += trUtf8("Firmado por: <b>\"%1\"</b><br/>")
                                       .arg(c.subjectInfoUtf8( QSslCertificate::CommonName ));
                    textinfo += tr("Emitido por: <b>\"%1\"</b>").arg(c.issuerInfo( QSslCertificate::CommonName ));
                    if ( i+1 < myfile->signatures().count() ) {
                        textinfo += tr("<br/><br/>");
                    }
                }

               ui->labelDididocInfo->setText(textinfo);
            }
            else {
                    ui->labelDididocInfo->setText(trUtf8("No contiene firmas electrónicas: \"%1\"")
                                      .arg(filename));

                }
          delete myfile;
      }

}
*/

void SendSignDocumentDialog::showFileInfo() {

    QListWidgetItem* item;
    //DigiDoc *myfile;

    SafetDocument doc;
    doc.initializeLibdigidoc();
    QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfilepath").toString();
    libdigidocConfigFile = QDir::homePath() + "/" + Safet::datadir + "/" + "digidoc.conf";
    doc.initDigidocConfigStore(libdigidocConfigFile);

    if ( ui->listFiles->selectedItems().count() > 0 ) {
        item = ui->listFiles->selectedItems().at(0);
            QString filename = item->text();
            qDebug("...showFileInfo: %s", qPrintable(filename));
            //myfile = new DigiDoc(this);

            if ( QFile::exists(filename)) {
                //myfile->open( filename ) ;
                // abrir archivo digidoc
                doc.readDigiDocFile(filename);

                QString textinfo;

                //for (int i = 0; i < myfile->signatures().count();i++) {
                for (int i = 0; i < doc.numberOfSignaturesOnOpenXAdESContainer();i++) {
                    //DigiDocSignature signature = myfile->signatures().at(i);
                    //const SslCertificate c = signature.cert();
                    textinfo += trUtf8("Firmado por: <b>\"%1\"</b><br/>")
                    //                 .arg(c.subjectInfoUtf8( QSslCertificate::CommonName ));
                                       .arg(doc.getCN(i));
                    //textinfo += tr("Emitido por: <b>\"%1\"</b>").arg(c.issuerInfo( QSslCertificate::CommonName ));
                    textinfo += tr("Emitido por: <b>\"%1\"</b>").arg(doc.getSignerCertificateIssuerName(i));
                    //if ( i+1 < myfile->signatures().count() ) {
                    if ( i+1 < doc.numberOfSignaturesOnOpenXAdESContainer() ) {
                        textinfo += tr("<br/><br/>");
                    }
                }

               ui->labelDididocInfo->setText(textinfo);
            }
            else {
                    ui->labelDididocInfo->setText(trUtf8("No contiene firmas electrónicas: \"%1\"")
                                      .arg(filename));

                }
          //delete myfile;
      }

}


SendSignDocumentDialog::~SendSignDocumentDialog()
{
    delete ui;
}

void SendSignDocumentDialog::changeEvent(QEvent *e)
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
