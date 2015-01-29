#include "getsigndocumentdialog.h"
//#include "ui_getsigndocumentdialog.h"
#include "SafetYAWL.h"
//#include "DigiDoc.h"
#include "SafetDocument.h"
#include "SslCertificate.h"

extern "C" {
#include "libdotar.h"
}


GetSignDocumentDIalog::GetSignDocumentDIalog(QWidget *parent) :
    QDialog(parent)
//    ui(new Ui::GetSignDocumentDIalog)
{
//    ui->setupUi(this);
    connect(ui->selectButton, SIGNAL(clicked()), this, SLOT(selectFiles()) );
    connect(ui->delButton, SIGNAL(clicked()), this, SLOT(delFiles()) );
    connect(ui->toGetButton, SIGNAL(clicked()), this, SLOT(includeFiles()) );
    connect(ui->listFiles, SIGNAL(itemSelectionChanged()), this, SLOT(showFileInfo()) );


}


QStringList GetSignDocumentDIalog::uncompresstar(const QString& filename) {
    QStringList files;
    int argc = 5;
    char **argv = NULL;

    argv = new char*[argc];
    argv[0] = new char[30];
    strcpy(argv[0],"inflow");
    argv[1] = new char[30];
    strcpy(argv[1],"-C");

    argv[2] = new char[1024];
    //qDebug("......QDir::tempPath(): %s", qPrintable(QDir::tempPath()));
    QString rootdir;
    QStringList mytempfiles = SafetYAWL::getTempNameFiles(1);

   rootdir = mytempfiles.at(0);
    if (QFile::exists(rootdir)) {
        QFile::remove(rootdir);
    }
    QString xdir = rootdir.section("/",-1);
    //qDebug("......QDir::tempPath()tempdir.fileName() (2) xdir: |%s|", qPrintable(xdir));
    QDir mytempdir(QDir::tempPath());
    mytempdir.mkdir(xdir);
    rootdir  = QDir::tempPath() + "/" + xdir;

    bool mkresult = mytempdir.mkpath(rootdir);
    if (!mkresult ) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("Error descomprimiendo configuración."
                      "No es posible crear el directorio: \"%1\"")
                .arg(rootdir);
        return files;
    }
    qDebug("..ok!");
    qDebug("creado..:%s", qPrintable(rootdir));
    SafetYAWL::streamlog
            << SafetLog::Debug
            << tr("Descomprimiendo archivos: Creado el directorio temporal: \"%1\"")
            .arg(rootdir);

   //qDebug("....(arg2)..QDir::tempPath()tempdir.fileName() (2) rootdir: |%s|", qPrintable(rootdir));

    strcpy(argv[2],qPrintable(rootdir));

//    argv[3] = new char[30];
//    strcpy(argv[3],"-z");

    argv[3] = new char[30];
    strcpy(argv[3],"-x");

    argv[4] = new char[1024];
    strcpy(argv[4],qPrintable(filename));
//    qDebug("....uncompress.tar: |%s|", qPrintable(filename));
//    qDebug("....uncompress.tar: option extract |%s|", argv[3]);

    dotar(argc,argv);
    mytempdir.cd(rootdir);
    mytempdir.setFilter(QDir::Files|  QDir::NoSymLinks);
    mytempdir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = mytempdir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString namedir = fileInfo.fileName();
        if ( !namedir.startsWith(".")) {
            files.append(rootdir+"/"+ namedir);
//            qDebug(".....GetSignDocumentDIalog..files: %s",
//                   qPrintable(rootdir+"/"+namedir));
        }
      }
    for(int i=0; i< argc; i++) {
        delete argv[i];
    }
    delete[] argv;
    return files;


}


GetSignDocumentDIalog::~GetSignDocumentDIalog()
{
    delete ui;
}

bool GetSignDocumentDIalog::enableIncludeFiles() {
    if (ui->listFiles->count() > 0 ) {
        return true;
    }
    return false;
}

void GetSignDocumentDIalog::changeEvent(QEvent *e)
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

void  GetSignDocumentDIalog::delFiles() {
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
         ui->toGetButton->setEnabled(true);
         ui->delButton->setEnabled(true);
     }
     else {
         ui->toGetButton->setEnabled(false);
         ui->delButton->setEnabled(false);
     }
}
/*
void GetSignDocumentDIalog::showFileInfo() {

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

void GetSignDocumentDIalog::showFileInfo() {

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
                doc.readDigiDocFile(filename);

                QString textinfo;

                //for (int i = 0; i < myfile->signatures().count();i++) {
                for (int i = 0; i < doc.numberOfSignaturesOnOpenXAdESContainer();i++) {
                    //DigiDocSignature signature = myfile->signatures().at(i);
                    //const SslCertificate c = signature.cert();
                    textinfo += trUtf8("Firmado por: <b>\"%1\"</b><br/>")
                    //                   .arg(c.subjectInfoUtf8( QSslCertificate::CommonName ));
                                       .arg(doc.getCN(i));
                    //textinfo += tr("Emitido por: <b>\"%1\"</b>").arg(c.issuerInfo( QSslCertificate::CommonName ));
                    textinfo += tr("Emitido por: <b>\"%1\"</b>").arg(doc.getSignerCertificateIssuerName());
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


void  GetSignDocumentDIalog::includeFiles() {

    int result = QMessageBox::question(this, trUtf8("Incluir archivo(s) XADES"),
                                          trUtf8("¿Realmente desea incluir el(los) archivos(s) en la base de documentos"
                                                 "firmados (XADES) local?"),
                                           QMessageBox::Yes | QMessageBox::No);

    QStringList containers;
     if ( result == QMessageBox::Yes ) {
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


          SafetYAWL::streamlog
                  << SafetLog::Debug
                  << tr("Se van a copiar: \"%1\" archivos al repositorio de firmas")
                  .arg(ui->listFiles->count());


         for( int i=0; i<ui->listFiles->count();i++){
             QListWidgetItem* item = ui->listFiles->item(i);
             QString source = item->text();


            QString namefile = item->text().section('/', -1);
             QString target = targetdir + "/" + curcontainer+ "/" + namefile;


            if (QFile::copy(source, target)) {
                SafetYAWL::streamlog
                        << SafetLog::Action
                        << tr("Copiado el archivo \"%3\" origen \"%1\" a destino \"%2\"")
                        .arg(source)
                        .arg(target)
                        .arg(i+1);
                int row = ui->listFiles->row(item);
                ui->listFiles->takeItem(row);
                i--;
            }
         }


         if ( ui->listFiles->count() == 0 ) {
             ui->toGetButton->setEnabled(false);
         }
     }
}

bool GetSignDocumentDIalog::searchInListNameFile(const QString& n) {
    for( int i=0; i<ui->listFiles->count();i++){
        QListWidgetItem* item = ui->listFiles->item(i);
        if (item->text() == n ) return true;
    }

    return false;
}

void GetSignDocumentDIalog::selectFiles() {

    QStringList files;
   QString types = tr("Archivos Firmados XADES (*.ddoc);;Archivos Comprimidos TAR (*.tar);;Todos (*)");


    QString curdir = QDir::homePath()+ "/" + Safet::datadir;

    files = QFileDialog::getOpenFileNames(this,tr("Seleccione archivo(s) firmados XADES a incluir"),
                                          curdir,types);



    if (files.count() > 0 ) {

        QString filetar = files.at(0);
        if ( filetar.endsWith(".tar")) {
            QStringList tarfiles = GetSignDocumentDIalog::uncompresstar(filetar);
             foreach(QString f, tarfiles ) {
                 if ( !searchInListNameFile(f)) {
                     ui->listFiles->addItem(f);
                 }
             }
             if ( ui->listFiles->count() > 0 ) {
                 ui->toGetButton->setEnabled(true);
                 ui->delButton->setEnabled(true);
             }

        }
        else {
            foreach(QString f, files ) {
                if ( !searchInListNameFile(f)) {
                    ui->listFiles->addItem(f);
                }
            }
            ui->toGetButton->setEnabled(true);
            ui->delButton->setEnabled(true);
        }
    }

}
