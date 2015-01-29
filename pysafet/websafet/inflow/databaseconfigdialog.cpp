

#include <QMessageBox>
#include <QCheckBox>
#include <QRegExp>

#include "databaseconfigdialog.h"
#include "ui_dataBaseConfig.h"
#include "SafetYAWL.h"
#include "mainwindow.h"


class errorEx
{
        private:
                QString msg;
        public:
                errorEx(QString txt, QString className = "") {
                        msg = txt;
                        if (!className.isEmpty())
                                msg += " (" + className + ")";
                }
                errorEx(const errorEx &e) { msg = e.msg; }
                QString getString() const {return msg;}
                //const char *getCString() const {return msg.latin1();}
                bool isEmpty() const { return msg.isEmpty();}
};

DatabaseConfigDialog::DatabaseConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseConfigDialog)
{
    ui->setupUi(this);

    // booleano para especificar si se pudo conectar al abrir
    // la aplicacion
    initialStage = false;

    // establecer el nombre del controlador para la conexion a BD
    ui->comboBoxDriver->insertItem(0,"psql");

    // conexiones
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(doAccept()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(doCancel()));
    connect(ui->checkBoxDBLocal, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLocal(int)));

    cancelFlag = false;
}

DatabaseConfigDialog::~DatabaseConfigDialog()
{
    delete ui;
}

void DatabaseConfigDialog::setInitialStage(bool b){
    initialStage = b;
}

bool DatabaseConfigDialog::getInitialStage(){
    return initialStage;
}

void DatabaseConfigDialog::doCancel() {
    qDebug("...doCancel....");
    setCancelFlag(true);
    reject();
}

void DatabaseConfigDialog::doAccept() {
    qDebug("...doAccept....");

    // validar que los campos no esten vacios
    if (!validateFields())
        return;

    // verificar si se desea crear la base de datos local de pruebas
    if (ui->checkBoxDBLocal->checkState () == Qt::Checked ){

        if (createDB()){
            MainWindow::mymainwindow->setEnableCompletingButtons(true);
            SafetYAWL::streamlog <<  SafetLog::Debug <<
                    tr("Creada la base de datos local para propositos de prueba.");

        }
        else{
            QMessageBox::warning(this, "SAFET - InFLow", trUtf8("No se pudo crear la base de datos local. Verifique nombre de usuario y contraseña."));
            SafetYAWL::streamlog << SafetLog::Warning << tr("No se pudo crear la base de datos local.");
            return;
        }

    }

    bool connection = false;
    //connection = testDataSources();
    connection = testNewDataSources();
    if (!connection){
        qDebug("... La conexion fallo");
        //QMessageBox::warning(this, "SAFET - InFLow", trUtf8("La conexión falló"));

        return;
    }
    else // en este punto los datos colocados son correctos y se debe cifrar
    {
        qDebug("Se va a cifrar");

        QStringList results = configValues();

        QStringList keys;
        keys << "database.sourcename.1"
                << "database.driver.1"
                << "database.host.1"
                << "database.db.1"
                << "database.port.1"
                << "database.user.1"
                << "database.password.1";

        QString fileconf = SafetYAWL::pathconf+ "/" + "auth.conf";
        QString replacetext;
        if (QFile::exists(fileconf)) {
            QString firstkeyfield, secondkeyfield, searchtext;
            int i=0;
            foreach(QString s, results) {
                firstkeyfield = keys.at(i).toLocal8Bit().constData();
                //qDebug("...firstkeyfield: %s", qPrintable(firstkeyfield));
                secondkeyfield = s;
                //qDebug("...secondkeyfield: %s",qPrintable(secondkeyfield));
/*
                searchtext = tr("\\s*(%1)\\s*\\=\\s*([áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f]"
                        "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$\\s,;\\x3c\\x3e\\x2f]*"
                        "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f])").arg(firstkeyfield);
*/
                searchtext = tr("\\s*(%1)\\s*\\=((\\s*([áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f]"
                                "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$\\s,;\\x3c\\x3e\\x2f]*"
                                "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f]))||(\\s*))").arg(firstkeyfield);

                replacetext  = tr("%1 = %2").arg(firstkeyfield).arg(secondkeyfield);

                //qDebug("...DatabaseConfigDialog::doAccept()...replacetext:|%s|",qPrintable(replacetext));
                if (SafetYAWL::replaceTextInFile(fileconf,
                                     searchtext,
                                     replacetext))
                i++;
            }

            MainWindow::mymainwindow->doCipherFile(true);

            SafetYAWL::readAuthConfFile();

            //limpiar el archivo auth.conf
            cleanConfigurationFile();

            //QMessageBox::information(this, "SAFET - InFlow", "Configuración de fuente de datos almacenada exitosamente.");

        }
        else
        {
            SafetYAWL::streamlog.initAllStack();
            SafetYAWL::streamlog
                    << SafetLog::Error
                    << tr("No se pueden realizar los cambios. No se encuentra el archivo :\"%1\"")
                    .arg(fileconf);

            if ( MainWindow::mymainwindow->queryForErrors() ) {
                qDebug(".......****..queryforerror....(2)");
                     return;
            }
        }
    }

    // en este punto todo esta ok
    accept();
}


void DatabaseConfigDialog::cleanConfigurationFile(){
    qDebug("...DatabaseConfigDialog::cleanConfigurationFile ...");
    QStringList results = configValues();

    QStringList keys;
    keys << "database.sourcename.1"
            << "database.driver.1"
            << "database.host.1"
            << "database.db.1"
            << "database.port.1"
            << "database.user.1"
            << "database.password.1";


    QString fileconf = SafetYAWL::pathconf+ "/" + "auth.conf";
    QString replacetext;
    if (QFile::exists(fileconf)) {
        QString firstkeyfield, secondkeyfield, searchtext;
        int i=0;
        foreach(QString s, results) {
            firstkeyfield = keys.at(i).toLocal8Bit().constData();

            //secondkeyfield = s;
            if ( (firstkeyfield == "database.db.1") || (firstkeyfield == "database.user.1") ||
                 (firstkeyfield == "database.password.1") ){
                secondkeyfield = "";
            }
            else{
                secondkeyfield = s;
            }

/*
            searchtext = tr("\\s*(%1)\\s*\\=\\s*([áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f]"
                    "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$\\s,;\\x3c\\x3e\\x2f]*"
                    "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f])").arg(firstkeyfield);
*/
            searchtext = tr("\\s*(%1)\\s*\\=((\\s*([áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f]"
                            "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$\\s,;\\x3c\\x3e\\x2f]*"
                            "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f]))||(\\s*))").arg(firstkeyfield);

            replacetext  = tr("%1 = %2").arg(firstkeyfield).arg(secondkeyfield);

            //qDebug("...DatabaseConfigDialog::doAccept()...replacetext:|%s|",qPrintable(replacetext));
            if (SafetYAWL::replaceTextInFile(fileconf,
                                 searchtext,
                                 replacetext))
            i++;
        }
        //QMessageBox::information(this, "SAFET - InFlow", "Configuración de fuente de datos almacenada exitosamente.");

    }
    else
    {
        SafetYAWL::streamlog.initAllStack();
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("No se pueden realizar los cambios. No se encuentra el archivo :\"%1\"")
                .arg(fileconf);

        if ( MainWindow::mymainwindow->queryForErrors() ) {
            qDebug(".......****..queryforerror....(2)");
                 return;
        }
    }


}



void DatabaseConfigDialog::changeEvent(QEvent *e)
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

bool DatabaseConfigDialog::validateFields(){
    int empty = 0;
    QString message = trUtf8("Los siguientes campos no pueden estar vacíos:\n");
    if (ui->lineEditName->text().isEmpty()){
        message.append(trUtf8("\n\tNombre"));
        empty++;
    }
    if (ui->lineEditHost->text().isEmpty()){
        message.append(trUtf8("\n\tHost"));
        empty++;
    }
    if (ui->lineEditPort->text().isEmpty()){
        message.append(trUtf8("\n\tPuerto"));
        empty++;
    }
    if (ui->lineEditDatabase->text().isEmpty()){
        message.append(trUtf8("\n\tBase de datos"));
        empty++;
    }
    if (ui->lineEditUser->text().isEmpty()){
        message.append(trUtf8("\n\tUsuario"));
        empty++;
    }
    if (ui->lineEditPassword->text().isEmpty()){
        message.append(trUtf8("\n\tContraseña"));
        empty++;
    }


    if (empty > 0){
        QMessageBox::warning(this, trUtf8("SAFET - InFlow"), message);
        if (ui->lineEditName->text().isEmpty()){

        }


        return false;
    }

    return true;

}


bool DatabaseConfigDialog::testDataSources(){
        qDebug("DatabaseConfigDialog::testDataSources()");

        QString driver = "database.driver.";
        QString hostName = "database.host.";
        QString dataBaseName = "database.db.";
        QString userName = "database.user.";
        QString portName = "database.port.";
        QString password = "database.password.";

        // leer el numero de fuentes de datos del archivo de configuracion
        QString datasources = SafetYAWL::getAuthConf()["Database/datasources"];
        int numDataSources = datasources.toInt();
        qDebug("**************************>>NÚMERO de fuentes de DATOS: %d", numDataSources);
        if (numDataSources <= 0) {
            SafetYAWL::readAuthConfFile();
              datasources = SafetYAWL::getAuthConf()["Database/datasources"];
              numDataSources = datasources.toInt();
              if (numDataSources <= 0) {
                SafetYAWL::streamlog << SafetLog::Error <<
                        tr("No se han definido fuentes de datos en "
                           "el archivo de configuración \"auth.conf\""
                           "o \"auth.conf.bin\"");
                            QMessageBox::warning(this, "SAFET - InFLow",
                                                 trUtf8("No se han definido fuentes de datos en "
                                                        "el archivo de configuración \"auth.conf\""
                                                        "o \"auth.conf.bin\""));
                return false;
              }
        }


        if (numDataSources <= 0) {
            SafetYAWL::streamlog << SafetLog::Error <<
                    tr("No se han definido fuentes de datos en el archivo de configuracion safet.conf");

            return false;
        }

        for (int i=0; i<numDataSources; i++) {
                qDebug("i: %d", i);

                QString activated = "database.actived.";
                QString databaseActivated = SafetYAWL::getAuthConf().getValue("Database", activated+QString::number(i+1)).toString();


                if (databaseActivated.compare("off", Qt::CaseInsensitive) == 0)
                {
                        SafetYAWL::streamlog <<  SafetLog::Debug <<
                                tr("La fuente de datos con id: \"%1\" no esta activada en archivo safet.con").arg(QString::number(i+1));
                        QMessageBox::warning(this, "SAFET - InFLow", trUtf8("La fuente de datos con id: \"%1\" no esta activada en archivo safet.con").arg(QString::number(i+1)));
                        return false;
                }
                else
                {
                        SafetYAWL::streamlog <<  SafetLog::Debug <<
                                tr("Activada fuente de datos con id: \"%1\"").arg(QString::number(i+1));

                        QString driverConf = SafetYAWL::getAuthConf().getValue("Database", driver+QString::number(i+1)).toString();
                        //qDebug("driver: %s",qPrintable(driverConf));

                        QString hostNameConf = SafetYAWL::getAuthConf().getValue("Database", hostName+QString::number(i+1)).toString();
                        //qDebug("hostname: %s", qPrintable(hostNameConf));
                        QString dataBaseNameConf = SafetYAWL::getAuthConf().getValue("Database", dataBaseName+QString::number(i+1)).toString();
                        //qDebug("database: %s", qPrintable(dataBaseNameConf));
                        QString userNameConf = SafetYAWL::getAuthConf().getValue("Database", userName+QString::number(i+1)).toString();

                        //qDebug("user: %s", qPrintable(userNameConf));
                        QString portConf = SafetYAWL::getAuthConf().getValue("Database", portName+QString::number(i+1)).toString();
                        //qDebug("port: %s", qPrintable(portConf ));

                        QString passwordConf = SafetYAWL::getAuthConf().getValue("Database", password+QString::number(i+1)).toString();
                        //qDebug(">>>> password: %s", qPrintable(passwordConf));
                        QString stringdriver = "Q" + driverConf.toUpper();
                        //qDebug("Qt driver: %s", qPrintable(stringdriver));

                        // si al menos un campo esta vacio se asigna la bandera configFieldsEmpty en true
                        if ( (driverConf.isEmpty()) || (hostNameConf.isEmpty()) || (dataBaseNameConf.isEmpty()) ||
                             (userNameConf.isEmpty()) || (portConf.isEmpty()) || (passwordConf.isEmpty()) ||
                             (stringdriver.isEmpty()) ){
                            QMessageBox::information(this,
                                                     "Fuente de datos",
                                                     "El archivo auth.conf tiene campos vacios. Por favor establezca\nlos valores de acceso a la fuente de datos en el siguiente dialogo");
                            setConfigFieldEmpty(true);
                            return false;
                        }
                        //QSqlDatabase currentDb;

                        qDebug("........DatabaseConfigDialog..........(1)...");
                        SafetYAWL::currentDb = QSqlDatabase::addDatabase(stringdriver, dataBaseNameConf);
                        SafetYAWL::currentDb.setHostName(hostNameConf);
                        SafetYAWL::currentDb.setDatabaseName(dataBaseNameConf);
                        SafetYAWL::currentDb.setUserName(userNameConf);
                        SafetYAWL::currentDb.setPassword(passwordConf);
                        bool ok;
                        int port = portConf.toInt(&ok);
                        qDebug("........DatabaseConfigDialog..........(2)...");
                        if ( !ok || !(port > 0 ) ) {
                            SafetYAWL::streamlog << SafetLog::Warning << tr("Se configur� incorrectamente el puerto a la base de datos: \"%1\"").arg(dataBaseName);
                            QMessageBox::warning(this, "SAFET - InFLow",
                                                trUtf8("El puerto utilizado en la fuente de datos no es correcto"));
                            return false;
                        }
                        SafetYAWL::currentDb.setPort(port);

                        SafetYAWL::streamlog << SafetLog::Action << tr("Configurando acceso de base de datos por el puerto: %1").arg(port);
                        //qDebug("*** antes de _db.open()");
                        qDebug("........DatabaseConfigDialog..........(3)...");
                        SafetYAWL::databaseOpen = SafetYAWL::currentDb.open();
                        qDebug("........DatabaseConfigDialog..........(4)...");
                        if (!SafetYAWL::currentDb.open()){
                            QMessageBox::warning(this, "SAFET - InFLow",
                                                 trUtf8("No se pudo establecer la conexion con la fuente de datos con la configuración actual. No estarán activas todas las funcionalidades. \n\nPor favor verifique los datos en el menu: Herramientas->Fuente de datos."));
                            return false;
                        }

                        else{
                            // asignar los valores en el archivos de configuracion y cifrarlo
                            // ...
                            qDebug("........DatabaseConfigDialog..........(4)...");
                            return true;
                        }



                }

        }



        //SafetYAWL::evalAndCheckExit((anyDataSourceActivated == true),
        //        tr("No existe ninguna fuente de datos activada en el archivo safet.conf. Por favor revise la configuracion"));
        QMessageBox::warning(this, "SAFET - InFLow",
                             trUtf8("No existe ninguna fuente de datos activada. Por favor revise la configuracion"));
        return false;
}



bool DatabaseConfigDialog::testNewDataSources(){
        qDebug("DatabaseConfigDialog::testNewDataSources()");

//        QString driver = "database.driver.";
//        QString hostName = "database.host.";
        QString dataBaseName = "database.db.";
//        QString userName = "database.user.";
//        QString portName = "database.port.";
//        QString password = "database.password.";
        //QString stringdriver;
//        bool anyDataSourceActivated = false;

        // leer el numero de fuentes de datos del archivo de configuracion
        QString datasources = SafetYAWL::getAuthConf()["Database/datasources"];
        int numDataSources = datasources.toInt();
        qDebug("**************************>>NÚMERO de fuentes de DATOS: %d", numDataSources);
        if (numDataSources <= 0) {
              SafetYAWL::readAuthConfFile();
              datasources = SafetYAWL::getAuthConf()["Database/datasources"];
              numDataSources = datasources.toInt();
              if (numDataSources <= 0) {
                SafetYAWL::streamlog << SafetLog::Error <<
                        tr("No se han definido fuentes de datos en "
                           "el archivo de configuración \"auth.conf\""
                           "o \"auth.conf.bin\"");
                            QMessageBox::warning(this, "SAFET - InFLow",
                                                 trUtf8("No se han definido fuentes de datos en "
                                                        "el archivo de configuración \"auth.conf\""
                                                        "o \"auth.conf.bin\" "));
                return false;
              }
        }

                       //SafetYAWL::streamlog <<  SafetLog::Debug <<
                       //         tr("Activada fuente de datos con id: \"%1\"").arg(QString::number(i+1));

                        //QString driverConf = SafetYAWL::getAuthConf().getValue("Database", driver+QString::number(i+1)).toString();
                        QString driverConf = ui->comboBoxDriver->currentText();
                        //qDebug("driver: %s",qPrintable(driverConf));
                        //QString hostNameConf = SafetYAWL::getAuthConf().getValue("Database", hostName+QString::number(i+1)).toString();

                        QString hostNameConf = ui->lineEditHost->text();
                        //qDebug("hostname: %s", qPrintable(hostNameConf));

                        //QString dataBaseNameConf = SafetYAWL::getAuthConf().getValue("Database", dataBaseName+QString::number(i+1)).toString();
                        QString dataBaseNameConf = ui->lineEditDatabase->text();
                        //qDebug("database: %s", qPrintable(dataBaseNameConf));

                        //QString userNameConf = SafetYAWL::getAuthConf().getValue("Database", userName+QString::number(i+1)).toString();
                        QString userNameConf = ui->lineEditUser->text();
                        QString _dbusername = userNameConf;
                        //qDebug("user: %s", qPrintable(userNameConf));

                        //QString portConf = SafetYAWL::getAuthConf().getValue("Database", portName+QString::number(i+1)).toString();
                        QString portConf = ui->lineEditPort->text();
                        //qDebug("port: %s", qPrintable(portConf ));

                        //QString passwordConf = SafetYAWL::getAuthConf().getValue("Database", password+QString::number(i+1)).toString();
                        QString passwordConf = ui->lineEditPassword->text();
                        //qDebug(">>>> password: %s", qPrintable(passwordConf));

                        QString stringdriver = "Q" + driverConf.toUpper();
                        qDebug("Qt driver: %s", qPrintable(stringdriver));

                        QSqlDatabase currentDb;

                        currentDb = QSqlDatabase::addDatabase(stringdriver, dataBaseNameConf);
                        currentDb.setHostName(hostNameConf);
                        currentDb.setDatabaseName(dataBaseNameConf);
                        currentDb.setUserName(userNameConf);
                        currentDb.setPassword(passwordConf);
                        bool ok;
                        int port = portConf.toInt(&ok);
                        if ( !ok || !(port > 0 ) ) {
                            SafetYAWL::streamlog << SafetLog::Warning << tr("Se configuró incorrectamente el puerto a la base de datos: \"%1\"").arg(dataBaseName);
                            QMessageBox::warning(this, "SAFET - InFLow",
                                                trUtf8("El puerto utilizado en la fuente de datos no es correcto"));
                            return false;
                        }
                        currentDb.setPort(port);
                        SafetYAWL::streamlog << SafetLog::Action << tr("Configurando acceso de base de datos por el puerto: %1").arg(port);
                        //qDebug("*** antes de _db.open()");
                        SafetYAWL::databaseOpen = currentDb.open();
                        if (!currentDb.open()){
                          //qDebug("LAST ERROR: %s", qPrintable(currentDb.lastError().databaseText()));
                        //qDebug("LAST ERROR2: %s", qPrintable(currentDb.lastError().driverText()));
                            //SafetYAWL::evalAndCheckExit( SafetYAWL::databaseOpen,tr("No se pudo establecer la conexion  a la base de datos con los datos suministrados: <%1>").arg(currentDb.lastError().driverText()));
                            QMessageBox::warning(this, "SAFET - InFLow",
                                                 trUtf8("No se pudo establecer la conexion a la base de datos con los datos suministrados. No estarán activas todas las funcionalidades. Por favor verifique los datos."));
                            return false;

/*
                            int ret = QMessageBox::information(this, "SAFET - InFLow",
                                    trUtf8("No se pudo establecer la conexion a la base de datos con los datos suministrados. ¿Desea crear una base de datos local con la información suministrada?."),QMessageBox::Ok | QMessageBox::Cancel);

                            switch (ret) {
                            case QMessageBox::Ok:{
                                   // Ok was clicked
                                    qDebug("se debe crear la base de datos local");
                                    if (createDB()){


                                        MainWindow::mymainwindow->setEnableCompletingButtons(true);

                                        return true;
                                    }
                                    else{
                                        return false;
                                    }
                                   break;
                               }
                            case QMessageBox::Discard:{
                                   // Don't Save was clicked
                                   break;
                               }
                            case QMessageBox::Cancel:{
                                   // Cancel was clicked
                                    return false;
                                   break;
                               }
                            default:{
                                   // should never be reached
                                   break;
                               }
                            }
                         return false;
*/
                        }
                        else{
                            // asignar los valores en el archivos de configuracion y cifrarlo
                            // ...
                            QMessageBox::information(this, "SAFET - InFLow",trUtf8("La conexión con los nuevos datos fue exitosa."));

                            // habilitar los botones de enviar dado que ya se establecio la conexion
                            MainWindow::mymainwindow->setEnableCompletingButtons(true);
                            //setInitialStage(false);
                            return true;
                        }




        //SafetYAWL::evalAndCheckExit((anyDataSourceActivated == true),
        //        tr("No existe ninguna fuente de datos activada en el archivo safet.conf. Por favor revise la configuracion"));
        QMessageBox::warning(this, "SAFET - InFLow",
                             trUtf8("No existe ninguna fuente de datos activada. Por favor revise la configuracion"));
        return false;
}


QStringList DatabaseConfigDialog::configValues(){
    QStringList values;
    values << ui->lineEditName->text()
            << ui->comboBoxDriver->currentText()
            << ui->lineEditHost->text()
            << ui->lineEditDatabase->text()
            << ui->lineEditPort->text()
            << ui->lineEditUser->text()
            << ui->lineEditPassword->text();
    return values;



}

void DatabaseConfigDialog::setConfigValues(const QString& host,
                                           const QString& database,
                                           const QString& port,
                                           const QString& user,
                                           const QString& pass,
                                           const QString& source) {

    ui->lineEditHost->setText(host);
    ui->lineEditDatabase->setText(database);
    ui->lineEditPort->setText(port);
    ui->lineEditUser->setText(user);
    ui->lineEditPassword->setText(pass);
    ui->lineEditName->setText(source);

}

void DatabaseConfigDialog::setConfigFieldEmpty(bool value){
    configFieldEmpty = value;
}


bool DatabaseConfigDialog::getConfigFieldEmpty(){
    return configFieldEmpty;
}


void DatabaseConfigDialog::setCancelFlag(bool value){
    cancelFlag = value;
}


bool DatabaseConfigDialog::getCancelFlag(){
    return cancelFlag;
}

void DatabaseConfigDialog::checkBoxLocal(int state){
    if (state == Qt::Unchecked){
        qDebug("checkBoxLocal 0");
        ui->lineEditName->setEnabled(true);
        ui->comboBoxDriver->setEnabled(true);
        ui->lineEditHost->setText("localhost");
        ui->lineEditHost->setReadOnly(false);
        ui->lineEditHost->setEnabled(true);
        ui->lineEditPort->setText("5432");
        ui->lineEditPort->setReadOnly(false);
        ui->lineEditPort->setEnabled(true);
        ui->lineEditDatabase->setEnabled(true);
    }
    if (state == Qt::Checked){
        qDebug("checkBoxLocal 2");
        ui->lineEditName->setEnabled(false);
        ui->comboBoxDriver->setEnabled(false);
        ui->lineEditHost->setText("localhost");
        ui->lineEditHost->setReadOnly(true);
        ui->lineEditHost->setEnabled(false);
        ui->lineEditPort->setText("5432");
        ui->lineEditPort->setReadOnly(true);
        ui->lineEditPort->setEnabled(false);
        ui->lineEditDatabase->setText("dbtracsafet");
        ui->lineEditDatabase->setReadOnly(true);
        ui->lineEditDatabase->setEnabled(false);


    }
}


QStringList DatabaseConfigDialog::parseSQLFile(const QString& s)
{
    qDebug("...QStringList DatabaseConfigDialog::parseSQLFile(const QString& s)");
        QStringList sqllist;
        QString temp, strfinal;
        int pos = 0;
        strfinal= s.section(";", -2 );
        strfinal = strfinal.section("*/", -1);
        strfinal = strfinal.trimmed();
        qDebug("STRFINAL:" );
        qDebug(qPrintable(strfinal));
        if ( (s.contains("--")) )
        {
            qDebug("s.contains(--)");
            throw errorEx( tr("SQL file not must contains commentary type ---"));
            //SafetYAWL::streamlog << SafetLog::Warning << tr("El archivo SQL no debe contener comentarios tipo ---");
            //return sqllist;
        }

        while(1)
        {
            //qDebug("while (1)");
                QRegExp rx(";(\\r|\\s)");
                temp =s.section(rx, pos, pos );
                temp = temp.section("*/", -1);

                temp = temp.trimmed();

                if ( strfinal.startsWith(temp))
                {
                        qDebug("parseSQLFile: break");
                        sqllist << temp;
                        break;
                }
                //qDebug("sqllist: ");
                //qDebug(qPrintable(temp));
                sqllist << temp;
                pos++;
        }
        qDebug("antes de salir");
        return sqllist;
}


bool DatabaseConfigDialog::createDB()
{
    qDebug("... DatabaseConfigDialog::createDB()");

        QString driverConf = ui->comboBoxDriver->currentText();

        QString hostNameConf = ui->lineEditHost->text();

        QString dataBaseNameConf = ui->lineEditDatabase->text();

        QString userNameConf = ui->lineEditUser->text();

        QString portConf = ui->lineEditPort->text();

        QString passwordConf = ui->lineEditPassword->text();

        QString stringdriver = "Q" + driverConf.toUpper();


        QSqlDatabase defaultDB = QSqlDatabase::addDatabase( "QPSQL7" );
        defaultDB.setDatabaseName( "template1" );
        defaultDB.setUserName( userNameConf );
        defaultDB.setPassword( passwordConf );
        defaultDB.setHostName( hostNameConf );
        defaultDB.setPort( portConf.toInt() );
        defaultDB.open();

        Q_ASSERT( defaultDB.isOpen() );



        QSqlQuery qry;
        QString str = "select datname from pg_database where datname='";
        str.append(ui->lineEditDatabase->text());
        str.append("'");
        qDebug("sql: ");
        qDebug(qPrintable(str));
        if ( !qry.exec( str ) )
        {
            qDebug("!qry.exec( str )");
            QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
            return false;
        }

        if ( qry.first() )
        {
            qDebug("qry.first()");
            QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
            return true;
        }

        QSqlQuery qry1;
        //str = "CREATE DATABASE ROOTVE ENCODING 'UNICODE'";
        str = "CREATE DATABASE ";
        str.append(ui->lineEditDatabase->text());
        str.append(" ENCODING 'UTF8'");

        if ( !qry1.exec( str ) )
        {
            QMessageBox::warning(this, "SAFET - InFLow", tr( "La base de datos no se pudo crear" ) );
            QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
            return false;
        }

        QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );

        QSqlDatabase currentDb;
        currentDb = QSqlDatabase::addDatabase(stringdriver/*, dataBaseNameConf*/);
        currentDb.setHostName(hostNameConf);
        currentDb.setDatabaseName(dataBaseNameConf);
        currentDb.setUserName(userNameConf);
        currentDb.setPassword(passwordConf);

        if ( ! currentDb.open() )
        {
                QMessageBox::warning(this, "SAFET - InFLow", tr( "Fallo la creacion de la base de datos <rootve>: " ) +
                currentDb.lastError().driverText());
                qWarning( qPrintable(currentDb.lastError().databaseText()));
                return false;
        }

        //QFile sqlfile( ":/basedatos.sql" );
        QFile sqlfile( ":/dbtracsafet281010.sql" );
        //QFile sqlfile( ":/intranet.sql" );
         if ( !sqlfile.open( QIODevice::ReadOnly ) )
         {
                QMessageBox::information(this, "SAFET - InFLow",
                        tr( "SQL File not found in the directory of application" ) );
                        currentDb.close();
                        QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
                        return true;
         }
        QString s;
        s = (QString) sqlfile.readAll();
        QStringList l;


        try
        {
            //qDebug("antes de parsear el archivo sql ");
            l = parseSQLFile( s );
            //qDebug("despues de parsear el archivo sql ");
        }
        catch(errorEx &err)
        {
                QMessageBox::information(this, "SAFET - InFLow",
                err.getString());
                currentDb.close();
                QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
                return false;
        }
        //qDebug("TAMANO DE L: " );
        //qDebug(qPrintable(QString::number(l.count())));
        if (l.count() == 0)
            return false;

        for( int i = 0; i < l.count(); i++)
        {
                QSqlQuery qry;
                QString strsql = l[i];
                //qDebug("SQL SENTENCE:");
                //qDebug(qPrintable(strsql));
                if ( !qry.exec(strsql) )
                {
                        QMessageBox::information(this, "SAFET - InFLow",
                        tr("This SQL SENTENCE failed:\n ") + l[i]);
                        return false;
                }
        }

        currentDb.close();
        QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
        //QMessageBox::information(this, "SAFET - InFLow", tr("Base de datos local creada exitosamente!.") );
        return true;
}
