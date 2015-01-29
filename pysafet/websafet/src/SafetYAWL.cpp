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

#include <QCryptographicHash>
#include <QDir>
 #include <QVariant>
//#include <QDesktopServices>

#ifdef SAFET_XML2 // Definido uso de SAFET XML2

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>

#endif

#include "SafetYAWL.h"
#include "SafetVariable.h"
#include "SafetWorkflow.h"
#include "SafetAutofilter.h"
#include "SafetRecursiveFilter.h"
#include "SafetDocument.h"
//#include "threadconsole.h"

// **
// Algunas variables para el sistema
bool SafetYAWL::databaseOpen = false;
QString SafetYAWL::pathconf = "";
QString SafetYAWL::fileconf = "safet.conf";
QString SafetYAWL::fileauthconf = "auth.conf";
QString SafetYAWL::fileauthconfcipher = "auth.conf.bin";
int SafetYAWL::MAX_JSON = 1000;

// *** CADENAS CONSTANTES
const QString SafetYAWL::ENDOFFLOW = tr("<fin_del_flujo>");

const QString SafetYAWL::LISTSEPARATORCHARACTER = "<SAFETSEPARATOR/>";
const QString SafetYAWL::LISTHTMLSEPARATOR = "<SAFETHTMLSEPARATOR/>";
const QString SafetYAWL::PATTERNS = "or,and,xor";
// ** CADENAS CONSTANTES

// *** Variables de configuración
SafetConfFile SafetYAWL::conf;
SafetConfFile SafetYAWL::authconf;
QMap<QString,QString> SafetYAWL::mapConfFile;
QMap<QString,QString> SafetYAWL::mapAuthConfFile;

QTextStream SafetYAWL::streamout;
SafetLog SafetYAWL::streamlog;
QString SafetYAWL::_dbusername;
SafetYAWL::onEvalExitFunction SafetYAWL::_evalexit = NULL;
SafetYAWL::onEvalInputFunction SafetYAWL::_evalinput = NULL;
OutputInterface* SafetYAWL::curOutputInterface = NULL; // Interface de Salida seleccionada


QList<WidgetInterface*>* SafetYAWL::listDynWidget = NULL;
QList<QPluginLoader*>* SafetYAWL::listPlugins = NULL;
QSqlDatabase SafetYAWL::currentDb;
QSqlDatabase SafetYAWL::signDb;
QStack< QPair<QDateTime,QString> > SafetYAWL::lasterrors;
QStack< QPair<QDateTime,QString> > SafetYAWL::lastwarnings;
QString SafetYAWL::lastgraph;
QString SafetYAWL::lastinfodate;
//QFile SafetYAWL::fileout;
QFile SafetYAWL::filelog; // Archivos de salida por consola y de registro de eventos
QString SafetYAWL::combovarglobal0 = "";
QString SafetYAWL::_currentauthuser = "";
char SafetYAWL::FILENAMELOG[12] = "";
QVector<QString> SafetYAWL::argsflow(10);
bool SafetYAWL::_isstatstokenfound = false;
QMap<QString,QPair<QString, QString> > SafetYAWL::mapCurrentExtrainfo;

/**
  PATRONES PARA EXPRESIONES REGULARES
*/

/**
 \brief Utilizada para reconocer caracteres en WIKI
 */
QString SafetYAWL::REWIKI = "&@¿/¡\\*ç\\+\"\\$\\^a-zA-Z0-9\\-\\(\\),;\\:#=\\.áéíóúñÑ\\s_¿\\?\\!";

/**
  PATRONES PARA EXPRESIONES REGULARES
*/



// Valores de edicion temporales (campos y valores ) usados en SafetTextParser
QString SafetYAWL::fieldsname = "";
QString SafetYAWL::fieldsvalue = "";
QMap<QString,QString> SafetYAWL::fieldtypes;

QMap<QString,QString> SafetYAWL::mimestypes;


QString SafetYAWL::currentConfPath; // Ruta actual de la configuraci�n
QString SafetYAWL::_currentExtraInfoTask;
bool SafetYAWL::SafetYAWL::_hasextrainfo;
QMap<QString,SafetYAWL::ExtraInfoTask>  SafetYAWL::_extrainfomap;

// Métodos de la Clase SafetYAWL

void SafetMessageOutput(QtMsgType type, const char *msg)
 {
     switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s\n", msg);
         break;
     case QtWarningMsg: // nada ---
          SafetYAWL::streamlog << SafetLog::Debug << QObject::tr("Qt Warning: %1").arg(QString(msg));
 //        fprintf(stderr, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", msg);
         abort();
     }
 }
void SafetYAWL::mimeTypesToMap() {

    SafetYAWL::mimestypes.clear();

     QStringList mimes  = SafetYAWL::getConf()["Libdigidoc/mimestypes.*"]
                          .split(SafetYAWL::LISTSEPARATORCHARACTER,QString::SkipEmptyParts);
     foreach(QString s, mimes) {
         QStringList list = s.split(" ",QString::SkipEmptyParts);
         if ( list.count() < 2 ) {
             continue;
         }
         SafetYAWL::mimestypes[list.at(1)] = list.at(0);
     }
}

bool SafetYAWL::signDocumentsFromData(const ParsedSqlToData& data,
                                      QString& nametowrite,
                                      QStringList& list,
                                      SafetDocument &doc,
                                       bool withsmartcard) {
#ifdef SAFET_DIGIDOC // Componente para firma Digidoc

     QString namepath, nameexistspath;
     SafetYAWL::mimeTypesToMap();

    qDebug("Creacion de objeto SafetDocument ...");

    qDebug("Inicializada de libreria ...");

    QList<QPair<QString,QString> > mimestypeslocal;


    // ************* prueba para agregar un archivo a contenedor digidoc ******

    QString fileKey, fileKeyCert;
    SafetYAWL::streamlog.initAllStack();
    for(int i = 0; i < data.fields.count(); i++) {
        QString field = data.fields.at(i);
        QString value = data.values.at(i);
        //           qDebug("data.fields.at(%d): |%s|", i, qPrintable(field));
        //           qDebug("data.values.at(%d): |%s|", i, qPrintable(value));
        QString ext = data.values.at(i).section('.', -1);
        //           qDebug("ext: |%s|....mimestypes[ext].isEmpty():|%d|", qPrintable(ext),mimestypes[ext].isEmpty());
        //           qDebug("(2)....MainWindow::signDocumentsFromData.mimestypes[\"txt\"] = %s", qPrintable(mimestypes["txt"]));
        if ( field.indexOf(QRegExp("Cargar_Archivo_(\\d+)")) >= 0 && SafetYAWL::mimestypes[ext].isEmpty() ) {
            SafetYAWL::streamlog << SafetLog::Error
                    << tr("Tipo MIME no soportado \"%2\" para el archivo \"%1\". Agregue tipo de archivo en  \"safet.conf\"").arg(data.values.at(i)).arg(ext);
            break;
        }
        else if (field.indexOf(QRegExp("Cargar_Archivo_(\\d+)")) >= 0 ) {
            QPair<QString,QString> pairfile(value,SafetYAWL::mimestypes[ext]);

            mimestypeslocal.append( pairfile );
            //mimestypeslocal[field].append( mimestypes[ext] );
        }
        else if ( field.compare("Nombre_contenedor",Qt::CaseSensitive) == 0) {
            namepath = value;
        } else if (field.compare("Cargar_contenedor",Qt::CaseSensitive) == 0 ) {
            nameexistspath = value;
            qDebug("....signDocumentsFromData...:%s",
                   qPrintable(nameexistspath));

        } else if (field.compare("Archivo_de_clave",Qt::CaseSensitive) == 0 ) {
            fileKey = value;
        } else if (field.compare("Certificado_clave",Qt::CaseSensitive) == 0 ) {
            fileKeyCert = value;
        }


    }

    if ( namepath.isEmpty() && nameexistspath.isEmpty()) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("Campo <b>Nombre_contenedor</b>. "
                          "Debe llenar este campo para realizar la operación solicitada. "
                          "No se realizo la firma");
        return true;
    }


    doc.initializeLibdigidoc();
    if ( !nameexistspath.isEmpty() ) {
        if ( !QFile::exists(nameexistspath)) {
            SafetYAWL::streamlog << SafetLog::Error
                    << tr("No es posible leer el archivo contenedor (.ddoc): \"%1\"").arg(nameexistspath);
            return true;
        }
        doc.readDigiDocFile(nameexistspath);
        qDebug("...leyendo DOCUMENTO FIRMADO desde : %s",qPrintable(nameexistspath));

    }
    else {
        doc.createOpenXAdESContainer();
        qDebug("Creado contenendor OpenXAdES");

    }


    QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfilepath").toString();
    // ************* prueba para abrir el archivo de configuracion de digidoc ******
    libdigidocConfigFile = QDir::homePath() + "/" + Safet::datadir + "/" + "digidoc.conf";
    doc.initDigidocConfigStore(libdigidocConfigFile);
    //safetDocument.initDigidocConfigStore(libdigidocConfigFile);

    // Verificando Mimes Type Local
    for ( int i = 0; i < mimestypeslocal.count(); i++) {
       doc.addFileToOpenXAdESContainer(doc.signedDoc(), mimestypeslocal.at(i).first,
                                       SafetDocument::SAFET_CONTENT_EMBEDDED_BASE64, mimestypeslocal.at(i).second);
       doc.addAttributeToFile(doc.dataFile(i), mimestypeslocal.at(i).first, "Fecha",
                              QDateTime::currentDateTime().toString("dd/MM/yyyy"));
       doc.addAttributeToFile(doc.dataFile(i), mimestypeslocal.at(i).first, "Hora",
                              QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));

    }


        // ************* prueba para cargar driver de la tarjeta inteligente ********
    if ( withsmartcard ) {
        doc.testSmartCardDriver(0);

        SafetYAWL::streamlog << SafetLog::Action << tr("Carga del driver de la tarjeta probada ...OK!");
        // ************* prueba firma con tarjeta inteligente***********************

        SafetYAWL::streamlog << SafetLog::Action << tr("Esperando a ingresar el PIN....OK!");
        bool ok;
        if ( SafetYAWL::errorStack().count() > 0 ) {
            return false;
        }

        QString pin;
        if (currentPin().isEmpty()) {
            pin = (SafetYAWL::evalInput())(SafetYAWL::PIN, tr("Ingrese Pin de la Tarjeta:"), ok);
        }
        else {
            pin = currentPin();
        }
        if ( !ok ) {
            SafetYAWL::streamlog << SafetLog::Action << tr("PIN No se ingreso...No se aplica la firma...OK!");
            SafetYAWL::streamlog.stopErrorStack();
            SafetYAWL::streamlog.stopWarnStack();
            return false;
        }
        SafetYAWL::streamlog << SafetLog::Action << tr("PIN Ingresado...esperando chequeo...OK!");
        QString manifest, city, state, zip, country;
        manifest = SafetYAWL::getConf()["DefaultValues/digidoc.manifest"];
        city = SafetYAWL::getConf()["DefaultValues/digidoc.city"];
        state = SafetYAWL::getConf()["DefaultValues/digidoc.state"];
        zip = SafetYAWL::getConf()["DefaultValues/digidoc.zip"];
        country = SafetYAWL::getConf()["DefaultValues/digidoc.country"];


        int err = doc.signWithSmartCard(doc.signedDoc(), pin,manifest,city,state,
                                        zip,country);
        if ( err != ERR_OK ) {
            qDebug(".......**ddocCertGetSubjectCN........err: %d", err);
            QString stringerror = SafetDocument::digidocStringError(err);
            if (stringerror.isEmpty() ) {
                stringerror = getErrorString(err);
            }

            SafetYAWL::streamlog <<
                    SafetLog::Error
                    << tr("No se pudo realizar la Firma Electrónica "
                          "sobre el documento: error:\"%1\"").arg(stringerror);
            return false;

        }
        SafetYAWL::streamlog << SafetLog::Action << tr("PIN validado correctamente....OK!");

    }
    else {
          qDebug("...firmando con archivo...(1)");
          QString manifest, city, state, zip, country, notaryurl,
                  proxyhost, proxyport;
          manifest = SafetYAWL::getConf()["DefaultValues/digidoc.manifest"];
          city = SafetYAWL::getConf()["DefaultValues/digidoc.city"];
          state = SafetYAWL::getConf()["DefaultValues/digidoc.state"];
          zip = SafetYAWL::getConf()["DefaultValues/digidoc.zip"];
          country = SafetYAWL::getConf()["DefaultValues/digidoc.country"];
          notaryurl = (char*)ConfigItem_lookup("DIGIDOC_OCSP_URL");
          proxyhost = (char*)ConfigItem_lookup("DIGIDOC_PROXY_HOST");
          proxyport = (char*)ConfigItem_lookup("DIGIDOC_PROXY_PORT");

          bool ok;
          QString passwd;
          if (currentPin().isEmpty()) {
            passwd = (SafetYAWL::evalInput())(SafetYAWL::PIN, tr("Contraseña del archivo:"),
                                                    ok);
          }
          else {
              passwd = currentPin();
          }
          if (!ok  || passwd.isEmpty()) {
              return false;
          }
          doc.signWithPrivateKeyOnFile(fileKey,
                                     passwd,
                                     fileKeyCert,
                                     NULL,
                                     (namepath.length()>0?namepath:nameexistspath),
                                     manifest,
                                     city,
                                     state,
                                     zip,
                                     country,
                                     notaryurl,
                                     proxyhost,
                                     proxyport);
          QStack<QPair<QDateTime,QString> > mystack = SafetYAWL::streamlog.stopErrorStack();
          if ( mystack.count() > 0  ) {
                qDebug("   signWithCertFile..queryforerror....");
                   return false;
           }

    }

        SafetYAWL::streamlog << SafetLog::Debug << tr("Documento se firma de manera  correcta ...OK!");



    qDebug("...antes de remoteContainer...");
    QString remoteContainer = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.remote").toString();
    qDebug("...(1)...remoteContainer...%s", qPrintable(remoteContainer));
    remoteContainer  = SafetYAWL::getConf()["XmlRepository/remote"];
    qDebug("...(2)...remoteContainer...%s", qPrintable(remoteContainer));
    if ( remoteContainer.compare("on",Qt::CaseInsensitive) == 0 ) {
        // Implementar aqui escritura de contenedor remoto
        return true;
    }
    else {
        if (!namepath.isEmpty()) {
            nametowrite = namepath;
            doc.writeOpenXAdESContainerToFile("", nametowrite);

        }
        else {
            nametowrite = nameexistspath;
            doc.writeOpenXAdESContainerToFile(nameexistspath, nametowrite);

        }
        SafetYAWL::streamlog
                << SafetLog::Debug
                << tr("Se va a escribir el contenedor en la ruta \"%1\"").arg(nametowrite);


        doc.setPathOfSafetDocument(nametowrite);


        SafetYAWL::streamlog
                << SafetLog::Action
                << tr("Se va a escribir el contenedor en la ruta \"%1\"").arg(namepath);
        qDebug("Escrito contenedor digidoc en: %s", qPrintable(namepath));

    }
    int nsigns = doc.numberOfSignaturesOnOpenXAdESContainer();


    list << tr("Ver firmas del documento(%1)").arg(nsigns) << tr("Mostrar carpeta del contenedor") << tr("Enviar contenedor por correo-e");

    //successVerification(list,tr("Firma electrónica (digidoc) realizada "
    //                            "exitosamente sobre \"%1\"....ok!").arg(nametowrite));

    // prueba para eliminar dependencia de digidocpp

    doc.closeLibdigidoc();
#endif // Componente para firma Digidoc

    return true;
}


bool SafetYAWL::verifyDocumentsFromData(const ParsedSqlToData& data,
                                         SafetDocument& doc) {

#ifdef SAFET_DIGIDOC // Componente para firma Digidoc
    QString digidocfilename;
    int  i;

    SafetYAWL::streamlog.initAllStack();
    for(i = 0; i < data.fields.count(); i++) {
        QString field = data.fields.at(i);
        QString value = data.values.at(i);
        if ( field.compare("Cargar_Archivo",Qt::CaseSensitive) == 0 ) {
            if (value.endsWith(".ddoc") ) {
                digidocfilename = value;
                qDebug("...encontrado...");
            }
        }
        QString ext = data.values.at(i).section('.', -1);
        qDebug("***ext: %s", qPrintable(ext));
    }
    qDebug("cuantos:%d",i);
    qDebug("digidocfilename:%s",qPrintable(digidocfilename));
    qDebug("existe:%d",QFile::exists(digidocfilename));
    //if ( /*digiDocument &&*/ QFile::exists(digidocfilename) ) {
    if ( !QFile::exists(digidocfilename) ) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("No es posible leer el archivo para verificar su firma: \"%1\"")
                .arg(digidocfilename);
        return false;
    }

    qDebug("..........................doc->open( \"%s\" ) ;",qPrintable(digidocfilename));
    //digiDocument->open( digidocfilename ) ;
    //safetDocument.readDigiDocFile(digidocfilename);


    doc.initializeLibdigidoc();
    doc.readDigiDocFile(digidocfilename);

    doc.setPathOfSafetDocument(digidocfilename);
    qDebug("...+++++++++++++****VERIFICACION CORRECTA..........(0)");
    //int nsigns = digiDocument->signatures().count();
#endif // Componente para firma Digidoc
    return true;


}



void SafetYAWL::copyInitialFiles() {
    SafetYAWL::pathconf = QDir::homePath() + "/" + Safet::datadir;
    QDir homedir( QDir::homePath() );
    if ( !homedir.exists(Safet::datadir) ) {
            SafetYAWL::createUserSafetDirectory(homedir);
            // ** Configuración Básica
            SafetYAWL::copyFileToSafetDirectory(homedir, "safet.conf", "");
            SafetYAWL::copyFileToSafetDirectory(homedir, "auth.conf", "");
            SafetYAWL::copyFileToSafetDirectory(homedir, "digidoc.conf", "");
            // ** Archivos DTD
            SafetYAWL::copyFileToSafetDirectory(homedir, "yawlworkflow.dtd", "dtd");
            SafetYAWL::copyFileToSafetDirectory(homedir, "yawlinput.dtd", "dtd");
            // ** Archivos de Entrada
            SafetYAWL::copyFileToSafetDirectory(homedir, "deftrac.xml", "input");
            SafetYAWL::copyFileToSafetDirectory(homedir, "defconsole.xml", "input");
            SafetYAWL::copyFileToSafetDirectory(homedir, "defconfigure.xml", "input");
            SafetYAWL::copyFileToSafetDirectory(homedir, "defmanagementsignfile.xml", "input");
            SafetYAWL::copyFileToSafetDirectory(homedir, "defusers.xml", "input");
            SafetYAWL::copyFileToSafetDirectory(homedir, "ticketstracavanzado.xml", "flowfiles");
            SafetYAWL::copyFileToSafetDirectory(homedir, "flujofirmado.xml", "flowfiles");

            // ** Archivos de base de datos de ejemplos
            SafetYAWL::copyFileToSafetDirectory(homedir, "dbtracsafet28oct09.sql", "sql");
            SafetYAWL::copyFileToSafetDirectory(homedir, "dbtracsafet281010.sql", "sql");
            SafetYAWL::copyFileToSafetDirectory(homedir, "dbtracseguridad28oct09.sql", "sql");
            SafetYAWL::copyFileToSafetDirectory(homedir, "intranet.sql", "sql");

            // ** Archivos para reportes
            SafetYAWL::copyFileToSafetDirectory(homedir, "animation-min.js", "reports");
            SafetYAWL::copyBinFileToSafetDirectory( "bg_hd.gif", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "button.css", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "button-min.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "data.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "datasource-min.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "datatable.css", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "datatable-min.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "dom-min.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "dpSyntaxHighlighter.css", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "dragdrop-min.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "element-min.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "event-min.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "paginator.css", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "paginator-min.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "sf_plantillaLista01.html", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "sf_plantillaLista02.html", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "sf_plantillaFirma01.html", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "title_h_bg.gif", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "yui.css", "reports");
            SafetYAWL::copyBinFileToSafetDirectory( "yui.gif", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "yuiloader-min.js", "reports");
            SafetYAWL::copyFileToSafetDirectory(homedir, "accenditel9ago09.pem", "certs");
            SafetYAWL::copyFileToSafetDirectory(homedir, "accenditel.pem","certs"); 
            SafetYAWL::copyFileToSafetDirectory(homedir, "ocspd_cert.pem","certs"); 
            SafetYAWL::copyFileToSafetDirectory(homedir, "crl_accenditel.crl","certs");
           SafetYAWL::copyBinFileToSafetDirectory("default.png", "images");
           SafetYAWL::copyBinFileToSafetDirectory("desc.png", "images");
           SafetYAWL::copyBinFileToSafetDirectory("firmadoc.png", "images");
           SafetYAWL::copyBinFileToSafetDirectory("padlock.png", "images");
           SafetYAWL::copyBinFileToSafetDirectory("seal.png", "images");
           SafetYAWL::copyBinFileToSafetDirectory("plus.png", "images");
           SafetYAWL::copyBinFileToSafetDirectory("hito.png", "images");
           SafetYAWL::copyBinFileToSafetDirectory("yes.png", "images");
           SafetYAWL::copyBinFileToSafetDirectory("clear.png", "images");
          SafetYAWL::copyBinFileToSafetDirectory("resumen.png", "images");

    }

}

SafetYAWL::SafetYAWL(const QString& path)  {

        //copyInitialFiles();
        strncpy(FILENAMELOG,"/safet.log",10);
        FILENAMELOG[10] = '\0';

        if ( !path.isEmpty()) {
            SafetYAWL::pathconf = path;
        }

        QString logpath = SafetYAWL::getConf()["Log/filepath"] + /*"/safet.log"*/ FILENAMELOG;

        loadPlugins();

//        openout = fileout.open(stdout, QIODevice::WriteOnly);
//        SafetYAWL::streamout.setDevice(&fileout);



        filelog.setFileName(logpath );
        openlog = filelog.open(QIODevice::Append);
        //Q_ASSERT_X(openlog, "SafetConsoleApp", qPrintable(tr("No es posible leer el archivo de registro de eventos: \"%1\"").arg(myvar.toString())));
         SafetYAWL::streamlog.setDevice(&filelog);

         QString logvar;
	SafetYAWL::streamlog.turnOnAll();

        logvar = SafetYAWL::getConf()["Log/debug"];
    if ( logvar.compare("off",Qt::CaseInsensitive) == 0 )  {
//        SYA << tr("turning off DEBUG....(TURNINGOFF)");
        SafetYAWL::streamlog.turnOff(SafetLog::Debug);
    }
	logvar = SafetYAWL::getConfFile().getValue("Log", "log.action").toString();
	if ( logvar.compare("off",Qt::CaseInsensitive) == 0 )  SafetYAWL::streamlog.turnOff(SafetLog::Action);
	logvar = SafetYAWL::getConfFile().getValue("Log", "log.warning").toString();
	if ( logvar.compare("off",Qt::CaseInsensitive) == 0 )  SafetYAWL::streamlog.turnOff(SafetLog::Warning);
	logvar = SafetYAWL::getConfFile().getValue("Log", "log.error").toString();
	if ( logvar.compare("off",Qt::CaseInsensitive) == 0 )  SafetYAWL::streamlog.turnOff(SafetLog::Error);

        //SYA << tr("libreria SAFET iniciada correctamente...OK!");
        //SYD << tr("La funcion personalizada de SALIDA por ocurrencia de error esta colocada en el valor: \"%1\"").arg(SafetYAWL::getConfFile().getValue("System", "system.evalexit").toString());
        //qInstallMsgHandler(SafetMessageOutput);
        loadAllConfFiles();

}


SafetYAWL::SafetYAWL(const SafetYAWL& other) {

}

SafetYAWL::~SafetYAWL() {


//        fileout.close();
        filelog.close();
        foreach(SafetWorkflow *mywf, wflist) {
             if(mywf) delete mywf;
        }
        QSqlDatabase::removeDatabase(dataBaseNameConf);
        QSqlDatabase::removeDatabase(QString("SIGNDB"));
        SafetYAWL::filelog.close();
//        SafetYAWL::fileout.close();

}

void SafetYAWL::loadAllConfFiles(int option) {
    if ( option & 1) {
        SafetYAWL::readConfFile();
        //qDebug("                ----***SafetYAWL::readConfFile()              (1)");
    }
    if ( option & 2 ) {
        SafetYAWL::readAuthConfFile();
//        qDebug();
//        qDebug("                ----***SafetYAWL::readAuthConfFile()              (1)");
//        qDebug();
    }

}

void SafetYAWL::openXML(const QString& s) {
         bool result;
         QFile file(s);
         result = file.open(QIODevice::ReadOnly | QIODevice::Text);
         if ( !QFile::exists(s) ) {
             SYE
                     <<
                     tr("No existe el archivo del flujo de trabajo (XML): \"%1\"")
                     .arg(s);
         }
         if (!result ) {
             SYE
                     <<
                     tr("Ocurri� un error de lectura "
                        "al tratar de abrir el archivo XML: \"%1\""
                        "en la ruta indicada").arg(s);
             return;
         }

	 bool nameproccesing =true;
         if ( !checkDTD(qPrintable(s) ) ) {
             SYW << tr("...no CHECK DTD...");
             return;
         }
         QString errorMsg;
         int errorline, errorcol;
         result = doc.setContent(&file,nameproccesing, &errorMsg, &errorline, &errorcol);
        SafetYAWL::evalAndCheckExit(result, tr("En la linea %1 y columna %2 ocurrio el siguiente error: %3").arg(errorline).arg(errorcol).arg(errorMsg));

         Q_ASSERT_X(result, "openXML","No es posible leer adecuadamente el contenido del archivo XML");
         file.close();
	
}

QString SafetYAWL::transformOfXSL(const QString& filexml, const QString& filexsl) {
	QString str;
	return str;
}

bool SafetYAWL::checkDTD(const char *filename) {

#ifdef SAFET_XML2 // Definido uso de SAFET XML2
    xmlParserCtxtPtr ctxt; /* the parser context */
    xmlDocPtr doc; /* the resulting document tree */

    /* create a parser context */
    ctxt = xmlNewParserCtxt();
    if (ctxt == NULL) {
        //fprintf(stderr, "Failed to allocate parser context\n");
        SafetYAWL::streamlog << SafetLog::Error << tr("Problemas de Memoria: No fue posible chequear la sintaxis del documento utilizando DTD");
        return false;
    }
    /* parse the file, activating the DTD validation option */
    doc = xmlCtxtReadFile(ctxt, filename, NULL, XML_PARSE_DTDVALID | XML_PARSE_RECOVER |XML_PARSE_NOERROR |  XML_PARSE_NOWARNING); // Aqui se produce el mensaje de chequeo de error
//    qDebug("ctxt->valid: %d", ctxt->valid);
     if ( !ctxt->valid ) {
	QString message = QString(ctxt->lastError.message);
	message.chop(1);
 	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(false, tr("analisis sintactico (parser) fallo en %1:%3:%2").arg(QString(filename)).arg(message).arg(ctxt->lastError.line) ));
        SafetYAWL::streamlog
                << SafetLog::Error
                << trUtf8("Error de  sintáxis del archivo XML (<b>parser</b>) en %1:%3:%2")
                .arg(QString(filename)).arg(message).arg(ctxt->lastError.line);
        return false;

    }
    /* check if parsing suceeded */
    if (doc == NULL) {
 	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(false, tr("Ocurrio un error de analisis sintactico (parser) para el archivo:  %1").arg(QString(filename))) );
        SYE
                << trUtf8("Ocurrio un error de analisis sintactico "
                      "(parser) para el archivo:  %1")
                .arg(QString(filename));
        return false;

    } else {
	/* check if validation suceeded */
        if (ctxt->valid == 0) {
//	    fprintf(stderr, "Failed to validate %s\n", filename);
 		//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(true, tr("Ocurrio un error al validar (parser) para el archivo: %1").arg(QString(filename))) );
                //SafetYAWL::evalAndCheckExit(true, tr("Ocurrio un error al validar (parser) para el archivo: %1").arg(QString(filename)));

	}
	/* free up the resulting document */
	xmlFreeDoc(doc);
    }
    /* free up the parser context */
    xmlFreeParserCtxt(ctxt);
#else // NO Definido uso de SAFET XML2

#endif
    return true;
}


void SafetYAWL::convertXMLtoObjects() {
	 QDomElement docElem = doc.documentElement();
	 QDomNode n = docElem.firstChild();

         qDebug("...convertXMLtoObjects....(1)...");

	 SafetXmlObject *sf = NULL;
	 SafetXmlObject *parent = this;
	 QStack<QDomNode> control;
	 QStack<SafetXmlObject*> parents; 
	 while( true ) {
		 if ( n.isNull() ) {
 	    	 if (control.isEmpty() ) break;
 	    	 n = control.pop();
 	    	 parent = parents.pop();
		 }
		 if ( n.isElement() ) {
		     QDomElement e = n.toElement(); // try to convert the node to an element.
		     sf =  getInstance(qPrintable(e.tagName()) );
		     if (sf != NULL) {
		    	 sf->syncAttributes(e);
		    	 parent->addChild(sf);		    	 
		     }
		     if ( n.hasChildNodes() && n.firstChild().isElement() ) {
		    	 control.push(n.nextSibling());
		    	 parents.push(parent);
		    	 n = n.firstChild();
		    	 if ( sf != NULL ) parent = sf;
		    	 continue;
		     }
		 }
		 
	     n = n.nextSibling();	     	     
	 }
	 


}


void SafetYAWL::addChild(SafetXmlObject* o) {
	Q_CHECK_PTR(o);
	SafetXmlObject::addChild(o);
	switch(getClassReference(translateNS(o->metaObject()->className()))) {
	case 1:
		wflist.push_back(qobject_cast<SafetWorkflow*>(o));
		break;
	case 4:
		conflist.push_back(qobject_cast<SafetConfiguration*>(o));
		break;
        case 5:

	default:;
	}

}

SafetXmlObject* SafetYAWL::getInstance(const QString& s) {
	switch(getClassReference(s)){
	case 1:		
		return new SafetWorkflow();
	case 2:
		return new SafetTask();
	case 3:
		return new SafetPort();	
	case 4:
		return new SafetConfiguration();
	case 5:
		return new SafetCondition();	
	case 6:
		return new SafetConnection();
	case 7:
		return new SafetVariable();
	case 8:
		return new SafetDocument();
	case 9:
		return new SafetToken();
        case 10:
                return new SafetAutofilter();
        case 11:
         return new SafetRecursivefilter();
        case 12:
         return new SafetParameter();
            default:;
	}
	
	return NULL;
}

QString SafetYAWL::translateNS(const QString& s, TranslateDir dir,const QString& ns) {
	Q_ASSERT_X(s.length() > 0, "translateNS", qPrintable(tr("La Cadena debe ser de mayor longitud que 0")));
	QString result = s;
	if ( dir == SafetYAWL::NamespaceToWord ) {
			result = result.replace(ns, "");
			result = result.toLower();
	}
	else {
		result[0] = result[0].toUpper();
		result = ns+result;
		
	}
	return result;
}

int SafetYAWL::getClassReference(const QString& s) {
	QMap <QString,int> map;
	map["workflow"]      = 1;
	map["task"]          = 2;
	map["port"]          = 3;
	map["configuration"] = 4;	
	map["condition"]     = 5;
	map["connection"]    = 6;
	map["variable"]      = 7;
	map["document"]      = 8;
	map["token"]         = 9;
        map["autofilter"]    = 10;
        map["recursivefilter"]    = 11;
        map["parameter"]    = 12;
	return map[s];
}


QList<SafetWorkflow*>& SafetYAWL::getWorkflows()  {
	return wflist;
}

SafetWorkflow* SafetYAWL::getWorkflow() {
    if ( wflist.count() == 0 ) {
           return NULL;
   }
   SafetWorkflow* result = wflist.at(0);
   if ( result == NULL ) {
       return NULL;
   }
       return result;
}


QList<SafetConfiguration*>& SafetYAWL::getConfigurations() {
	return conflist;
}

QString SafetYAWL::getDocumentID(const QSqlQuery& query, bool withext) {
	QSqlRecord rec;
	rec = query.record();
	QString input, output;
	for ( int i=0; i < rec.count(); i++ ) {
		input += query.value( i ).toString().trimmed();
	}
	
	char tableid[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	QByteArray  data = QCryptographicHash::hash ( input.toUtf8(), QCryptographicHash::Sha1 );
	
	for (int i = 0; i < data.size(); i++) {
//			qDebug("data.at(%d): %d", i, data.at(i) % 62);
			int index = (data.at(i) % 62) > 0 ? (data.at(i) % 62) : (data.at(i) % 62)*-1;			
			output += tableid[ index ];
	}
	
        if (withext) {
            output += ".bdoc";
        }
	return output;

}

QString SafetYAWL::addQuotes(const QString& value) {
    QString result;

    bool addquotes =  !(SafetYAWL::canTypeConvert(value, QVariant::Bool)
                        && !SafetYAWL::canTypeConvert(value, QVariant::Int));
    if ( addquotes ) {
         result = "'"+ value + "'";
    }
    else {
          result = value;
    }
    return result;

}

bool SafetYAWL::canTypeConvert(const QString& s, QVariant::Type t) {
	bool result = false;
    QRegExp rx("^(true|false|null)$");
	rx.setCaseSensitivity( Qt::CaseInsensitive );
    QString news = s;

	switch ( t ) {
		case QVariant::Int:
            rx.setPattern("[\\d]+[\\d\\.]*");
            SYD << tr("...SafetYAWL::canTypeConvert...t:%1").arg(t);
            SYD << tr("...SafetYAWL::canTypeConvert...news:%1").arg(news);
            if (QDateTime::fromString(news,"yyyyMMdd").isValid()) {
                SYD << tr("...SafetYAWL::canTypeConvert....return false");
                return false;
            }
            result = rx.exactMatch(news.trimmed());
            SYD << tr("...SafetYAWL::canTypeConvert....return:%1").arg(result);
			break;
		case QVariant::String:
			rx.setPattern("^(\\s*(([1|2|3]\\d{3}-\\d{1,2}-\\d{1,2})|(\\d{1,2}-\\d{1,2}-[1|2|3]\\d{3})|(\\d{1,2}/\\d{1,2}/[1|2|3]\\d{3}))\\s*)$");
			result = !(rx.indexIn( s ) >= 0);
			if (!result ) break;                        
                        rx.setPattern("^(\\s*[a-zA-Z_/][/a-zA-Z0-9_\\.]*\\s*)$");
			result = rx.indexIn( s ) >= 0;
			break;
		case QVariant::Bool:
			result = rx.indexIn( s ) >= 0;
			break;
		case QVariant::Date:
			rx.setPattern("^(\\s*(([1|2|3]\\d{3}-\\d{1,2}-\\d{1,2})|(\\d{1,2}-\\d{1,2}-[1|2|3]\\d{3})|(\\d{1,2}/\\d{1,2}/[1|2|3]\\d{3}))\\s*)$");
			result = rx.indexIn( s ) >= 0;
			break;

		default:
			break;
	}

	return result;
}

QString SafetYAWL::processOperatorValue(const QString& s, QString& ope) {
	QString str = s;
	QRegExp rx;
        rx.setPattern("^\\s*(<|>|\\=|<\\=|>\\=|\\!\\=|LIKE|IS|NOT IN\\s|IN\\s).*");
	rx.setCaseSensitivity(Qt::CaseInsensitive);
	int pos = rx.indexIn(str);
	if (pos >= 0 & rx.cap(1).length() >  0 ) {
		str = str.mid(rx.cap(1).length());
		ope = rx.cap(1);
	}
	else {
		ope = "=";
	}	
	return str;
}


bool SafetYAWL::evalSQLValue(const QString& exp) {


    QSqlQuery query( SafetYAWL::currentDb );
    QString command = QString("SELECT %1").arg(exp);

   query.prepare(  command );
   bool executed = query.exec();
   if (!executed ) {
        SYE << tr("Problema con la secuencia \"%1\", no se ejecut� correctamente la sentencia SQL: \"%2\"")
               .arg(exp).arg(command);
        return false;
    }

   bool isnext = query.next();
   if (!isnext ) {
       SYE << tr("No hay registros para ejecutar la secuencia \"%1\"").arg(exp);
       return false;
   }

   QString value = query.value(0).toString();
   SYD << tr("...SafetYAWL::evalSQLValue..value:|%1|").arg(value);
   if (value == "true") {
       return true;
   }
   return false;


}

bool SafetYAWL::evalValues(const QString& s1, const QString& s2, const QString& ope ) {

      if ( (s1.compare("true") == 0)  )
              return true;

//      QString newexp = s1 + ope + s2;

//      QRegExp rxfunc("[a-zA-Z0-9\\-_]{3,}\\([a-zA-Z0-9\\-_\\s',]*\\)");

//      int pos = rxfunc.indexIn(s2);
//      SYD << tr("SafetYAWL::evalValues...pos:|%1|").arg(pos);

//      if (pos >= 0 ) {
//          bool result = SafetYAWL::evalSQLValue(newexp);
//          SYD << QString("EVALVALUES: result:|%1|")
//                 .arg(result);
//          return  result;
//      }
//      else {
          bool result = SafetYAWL::evalStaticValues(s1,s2,ope);
          SYD << QString("STATIC.....SafetYAWL::EVALVALUES: result:|%1|")
                 .arg(result);
          return result;
//      }
      return true;

}

bool SafetYAWL::evalStaticValues(const QString& s1, const QString& s2, const QString& ope ) {

      int result;
      QRegExp rx("[\\d\\.]+");
      QRegExp rxdatepsql(Safet::DATEFORMATPSQL);

      SYD << tr("\n\n....STATIC...SafetYAWL::evalStaticValues....s1...:|%2|. ope:|%3|..s2:|%1|")
             .arg(s2)
             .arg(s1)
             .arg(ope);

          if ( (s1.compare("true") == 0)  )
              return true;

      result = QString::compare(ope, "=", Qt::CaseInsensitive);
      if ( result == 0 )
        return s1 == s2;
      result = QString::compare(ope, "<", Qt::CaseInsensitive);
      if ( result == 0 ) {
        if (rx.exactMatch(s2)) {
            return s1 < s2;
        }
        else {
            QString news1 = s1;
            if (rxdatepsql.exactMatch(news1)) {
                news1.chop(7);
            }
            return evalSQLValue(addQuotes(news1)+ope+s2);
        }
      }
      result = QString::compare(ope, ">", Qt::CaseInsensitive);
      if ( result == 0 ) {
          if (rx.exactMatch(s2)) {
              return s1 > s2;
          }
          else {
              QString news1 = s1;
              if (rxdatepsql.exactMatch(news1)) {
                  news1.chop(7);
              }

              return evalSQLValue(addQuotes(news1)+ope+s2);
          }
      }

      result = QString::compare(ope, "<=", Qt::CaseInsensitive);
      if ( result == 0 ) {
          if (rx.exactMatch(s2)) {
              return s1 <= s2;
          }
          else {
              QString news1 = s1;
              if (rxdatepsql.exactMatch(news1)) {
                  news1.chop(7);
              }

              return evalSQLValue(addQuotes(news1)+ope+s2);
          }
      }
      result = QString::compare(ope, ">=", Qt::CaseInsensitive);
      if ( result == 0 ) {
          if (rx.exactMatch(s2)) {
              return s1 >= s2;
          }
          else {
              QString news1 = s1;
              if (rxdatepsql.exactMatch(news1)) {
                  news1.chop(7);
              }

              return evalSQLValue(addQuotes(news1)+ope+s2);
          }
      }
      result = QString::compare(ope, "!=", Qt::CaseInsensitive);
      if ( result == 0 )
        return s1 != s2;
       result = QString::compare(ope, "LIKE", Qt::CaseSensitive);
      if ( result == 0 ) {
        SYD << tr("evalStaticValues, ---> LIKE");
        QString news2 = s2;
        news2 = news2.remove(QChar('%')).remove(QChar('\'')).trimmed();
        if (s1.isEmpty() && news2.isEmpty()) {
            return true;
        }
        SYD << tr("evalStaticValues, ---> LIKE news2:|%1|")
               .arg(news2);
        SYD << tr("evalStaticValues, ---> LIKE s1:|%1|")
               .arg(s1);
        bool r = s1.indexOf(news2, 0,Qt::CaseInsensitive) >= 0;
        SYD << tr("evalStaticValues, ---> LIKE r:|%1|\n\n")
               .arg(r);
        return r;
      }
          result = QString::compare(ope, "NOT IN", Qt::CaseSensitive);
          if ( result == 0 ) {
                QString news2 = s2;
                news2 = news2.remove(QChar('\''));
                SafetYAWL::streamlog << SafetLog::Debug << tr("Parametros para el operador \"NOT IN\": \"%1\" \"%2\"").arg(s1).arg(s2);
                return true;
          }
          result = QString::compare(ope, "IN", Qt::CaseSensitive);
          if ( result == 0 ) {
                SafetYAWL::streamlog << SafetLog::Debug << tr("Parametros para el operador \"IN\": \"%1\" \"%2\"").arg(s1).arg(s2);
                return true;
          }

          result = QString::compare(ope, "IS", Qt::CaseSensitive);
      if ( result == 0 ) {
        bool value = false;
                if ( s2.left(4).compare("NULL", Qt::CaseInsensitive) == 0) {
              value =  s1.length() == 0 ;
                } else if ( s2.left(8).compare("NOT NULL", Qt::CaseInsensitive) == 0) {
              value =  s1.length() > 0 ;
        }
                SYD << tr("....***SafetYAWL::evalValues....value:|%1|")
                       .arg(value);
        return value;
       }
      if ( result == 0 )
        return s1 == s2;




}



void SafetYAWL::debugPaths(const QList<QQueue<QString> > &paths) {

	QString debugstring = tr("\nDebug de caminos\n");
	int cola = 0, nodo;
	foreach(QQueue<QString> myqueue, paths) {		
                debugstring += tr("\n** Debug de nodos: %1\n").arg(cola);
		debugstring += "; ";
		nodo = 0;
		cola++;
		while(!myqueue.isEmpty()) {
			debugstring += tr("\t%1 (Cola)  %2 (nodo).....%3").arg(cola).arg(++nodo).arg(myqueue.dequeue());
			debugstring += "\n";
		}
		debugstring += "**\n";	
	}
	debugstring += "*";	
	SafetYAWL::streamlog << SafetLog::Debug << debugstring;

}

void SafetYAWL::debugQueue(const QQueue<QString> &q,const QString& message) {
	QQueue<QString> myqueue = q;

	QString debugstring = tr("\nDebug de la COLA: ")+ message + "\n";	
	while (!myqueue.empty()) {
		debugstring += myqueue.dequeue();
		debugstring += "\n";	
	}		
	debugstring += "*";	
	SafetYAWL::streamlog << SafetLog::Debug << debugstring;
	
}

void SafetYAWL::debugStack(const QStack<QString> &e,const QString& message) {
	QStack<QString> mystack = e;

	QString debugstring = tr("\nDebug de la PILA: ")+ message + "\n";
	while (!mystack.empty()) {
		debugstring += mystack.pop();
		debugstring += "\n";	
	}		
	debugstring += "*";	
	SafetYAWL::streamlog << SafetLog::Debug << debugstring;
}


void SafetYAWL::setEvalExit(SafetYAWL::onEvalExitFunction f) { 	
	SafetYAWL::_evalexit = f; 
}


void SafetYAWL:: defaultEvalEventOnExit(SafetLog::Level l) {
        if ( l == SafetLog::ExitApp ) {
                SafetYAWL::streamout << tr("**  Funcion de salida por defecto (Consola):") << endl;
                SafetYAWL::streamout << tr("** (Error: Ocurrio un error.No se pudo completar la accion.");
                SafetYAWL::streamout << tr(" * Importante: Revise el registro de eventos (log)") << endl;
                ::exit(0);
        }
}


void SafetYAWL::setEvalInput(SafetYAWL::onEvalInputFunction f) { 	
	SafetYAWL::_evalinput = f; 
}


void SafetYAWL::evalAndCheckExit(bool a, const QString &m){

	QString configEvalExit = SafetYAWL::getConfFile().getValue("System", "system.evalexit").toString();
        if (SafetYAWL::evalExit() == NULL ) {
             setEvalExit( defaultEvalEventOnExit );

        }
        Q_CHECK_PTR( SafetYAWL::evalExit() );
	if (configEvalExit.compare("on", Qt::CaseInsensitive) == 0)
		(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(a, m) );
	else if(configEvalExit.compare("off", Qt::CaseInsensitive) == 0)
		SafetYAWL::streamlog.eval(a, m);

}

void SafetYAWL::closeDataSources() {
//    qDebug("..out..dataBaseNameConf: |%s|", qPrintable(dataBaseNameConf));
//     currentDb.close();




}

void SafetYAWL::readConfFile() {

    SafetYAWL::mapConfFile = SafetYAWL::getConfFile().readFile(SafetYAWL::pathconf + "/" + SafetYAWL::fileconf);
}

void SafetYAWL::readAuthConfFile() {


    if ( QFile::exists(SafetYAWL::pathconf + "/" + SafetYAWL::fileauthconfcipher)) {
        SafetYAWL::mapAuthConfFile =
                SafetYAWL::getAuthConfFile().
      //          readFile(SafetYAWL::pathconf + "/" + SafetYAWL::fileauthconf);
                readFile(SafetYAWL::pathconf + "/" + SafetYAWL::fileauthconfcipher,true);
//        SYD
//                << tr("readAuthConfFile():Cargado el archivo \"auth.conf.bin\"");

        return;
    }
    if ( QFile::exists(SafetYAWL::pathconf + "/" + SafetYAWL::fileauthconf)) {
        SafetYAWL::mapAuthConfFile =
                SafetYAWL::getAuthConfFile().
      //          readFile(SafetYAWL::pathconf + "/" + SafetYAWL::fileauthconf);
                readFile(SafetYAWL::pathconf + "/" + SafetYAWL::fileauthconf,false);
//        SYD
//                << tr("readAuthConfFile():Cargado el archivo \"auth.conf\"");
        return;
    }

            SYE << tr("No es posible leer alg�n archivo de configuraci�n en el"
                  " directorio especificado  (\"%1\")")
            .arg(SafetYAWL::pathconf);

}

bool SafetYAWL::openDataSources(){
	//qDebug("SafetYAWL::openDataSources()");
	
	QString driver = "database.driver.";
	QString hostName = "database.host.";
	QString dataBaseName = "database.db.";
	QString userName = "database.user.";
        QString portName = "database.port.";
	QString password = "database.password.";
        QString stringdriver;
	bool anyDataSourceActivated = false;

	// leer el numero de fuentes de datos del archivo de configuracion
        QString datasources = SafetYAWL::getAuthConf()["Database/datasources"];
	int numDataSources = datasources.toInt();
        qDebug("**************************>>NÚMERO de fuentes de DATOS: %d", numDataSources);
        if (numDataSources <= 0) {

              readAuthConfFile();
              datasources = SafetYAWL::getAuthConf()["Database/datasources"];
              numDataSources = datasources.toInt();

              if (numDataSources <= 0) {
                SafetYAWL::streamlog << SafetLog::Error <<
                        tr("No se han definido fuentes de datos en "
                           "el archivo de configuración \"auth.conf\""
                           "o \"auth.conf.bin\"");
                return false;
              }
        }
	
	for (int i=0; i<numDataSources; i++) {
		//qDebug("i: %d", i);
	
		QString activated = "database.actived.";
                QString databaseActivated = SafetYAWL::getAuthConf().getValue("Database", activated+QString::number(i+1)).toString();
		

		if (databaseActivated.compare("off", Qt::CaseInsensitive) == 0)
		{
			SafetYAWL::streamlog <<  SafetLog::Debug << 
                                tr("La fuente de datos con id: \"%1\" no esta activada en archivo safet.con").arg(QString::number(i+1));
		}
		else
		{
			SafetYAWL::streamlog <<  SafetLog::Debug << 
                                tr("Activada fuente de datos con id: \"%1\"").arg(QString::number(i+1));
			
                        QString driverConf = SafetYAWL::getAuthConf().getValue("Database", driver+QString::number(i+1)).toString();
			//qDebug("driver: %s",qPrintable(driverConf));
                        QString hostNameConf = SafetYAWL::getAuthConf().getValue("Database", hostName+QString::number(i+1)).toString();
			//qDebug("hostname: %s", qPrintable(hostNameConf));
                        dataBaseNameConf = SafetYAWL::getAuthConf().getValue("Database", dataBaseName+QString::number(i+1)).toString();
			//qDebug("database: %s", qPrintable(dataBaseNameConf));
                        QString userNameConf = SafetYAWL::getAuthConf().getValue("Database", userName+QString::number(i+1)).toString();
                        _dbusername = userNameConf;
			//qDebug("user: %s", qPrintable(userNameConf));
                        QString portConf = SafetYAWL::getAuthConf().getValue("Database", portName+QString::number(i+1)).toString();
                        //qDebug("port: %s", qPrintable(portConf ));

                        QString passwordConf = SafetYAWL::getAuthConf().getValue("Database", password+QString::number(i+1)).toString();
                        //qDebug(">>>> password: %s", qPrintable(passwordCóonf));
			stringdriver = "Q" + driverConf.toUpper();
			//qDebug("Qt driver: %s", qPrintable(stringdriver));

			currentDb = QSqlDatabase::addDatabase(stringdriver, dataBaseNameConf);
			currentDb.setHostName(hostNameConf);
			currentDb.setDatabaseName(dataBaseNameConf);
			currentDb.setUserName(userNameConf);
			currentDb.setPassword(passwordConf);
                        bool ok;
                        int port = portConf.toInt(&ok);
                        if ( !ok || !(port > 0 ) ) {
                            SafetYAWL::streamlog
                                    << SafetLog::Error
                                    << tr("Se configuró incorrectamente el puerto a la base de datos: \"%1\"").arg(dataBaseName);
                            return false;
                        }
                        currentDb.setPort(port);
//                        SYD  << tr("Configurando acceso de base de datos por el puerto: %1").arg(port);
                        //qDebug("*** antes de _db.open()");
			SafetYAWL::databaseOpen = currentDb.open();
//                          qDebug("LAST ERROR: %s", qPrintable(SafetYAWL::currentDb.lastError().databaseText()));
			//qDebug("LAST ERROR2: %s", qPrintable(SafetYAWL::currentDb.lastError().driverText()));
                        if ( !SafetYAWL::databaseOpen ) {
                            SafetYAWL::streamlog
                                    << SafetLog::Error
                                    <<
                            tr("No se pudo establecer la conexion  a la base de datos"
                               " con los datos suministrados: \"%1").arg(currentDb.lastError().driverText());
                            return false;
                        }
			
			anyDataSourceActivated = true;

                        // Abriendo base de datos para las Firmas Electr�nicas
                        //openSignDb();
		}

	}



        if (anyDataSourceActivated != true)  {
            SafetYAWL::streamlog
                    << SafetLog::Error
                <<
            tr("No existe ninguna fuente de datos activada en el archivo safet.conf. Por favor revise la configuracion");
            return false;

     }
        return false;
}



bool SafetYAWL::openSignDb() {
    signDb = QSqlDatabase::addDatabase("QSQLITE",QString("SIGNDB"));
    signDb.setDatabaseName("/home/vbravo/.safet/stats/stats.db");
    if (!signDb.open()) {
        SafetYAWL::streamlog << SafetLog::Error << tr("No se pudo crear la base de datos para manejar Firmas Electr�nicas (signDb)") ;
        return false;
    }

      return true;
}

bool SafetYAWL::createTableSignDb(const QString& fields, const QString& values,
                                  const QString& q, QString &resultvalue) {

    if (!signDb.isOpen()) {
        if (!openSignDb() ) {
            return false;
        }
    }
    resultvalue = "";
    QStringList namefields = fields.split(",",QString::SkipEmptyParts);
    QString strcreate = QString("create table editValues (_editValuesid int primary key,");
    QStringList myvalues = values.split(",");
    QStringList newvalues;

    int i = 0;
    foreach(QString s, namefields) {
        QString mytype = "varchar(60)";
        QString myvalue = myvalues.at(i);
        if (SafetYAWL::fieldtypes.contains(s)) {
            if (SafetYAWL::fieldtypes[s] == "datetime" ) {
                mytype = "int";
                myvalue.replace("'","");

            }
            else {
                myvalue.replace("'","");
                myvalue = QString("'%1'").arg(myvalue);

            }
        }
        strcreate += QString("%1 %2,")
                     .arg(s)
                     .arg(mytype);
        newvalues.append(myvalue);
        i++;
    }
    strcreate.chop(1);
    strcreate += ")";
    qDebug();
    qDebug("....strcreate: |%s|",qPrintable(strcreate));
    QSqlQuery myquery(SafetYAWL::signDb);
    myquery.exec("drop table editValues");
    bool executed = myquery.exec(strcreate);
    qDebug("(1)...lastError: |%s|", qPrintable(myquery.lastError().databaseText()));
    qDebug("(1)...executed: %d",executed);
    if ( !executed ) {
        closeSignDb();
        return false;
   }

    QString strinsert = QString("insert into editValues values(1,%1)")
                        .arg(newvalues.join(","));

    qDebug("strinsert:%s", qPrintable(strinsert));
    executed = myquery.exec(strinsert);
    qDebug("(2)...executed: %d",executed);

    QString strquery = QString("SELECT %1 FROM editValues where _editValuesid=1").arg(q);
    qDebug("strquery:%s", qPrintable(strquery));
    executed = myquery.exec(strquery);
      qDebug("(3)...executed: %d",executed);
    if ( !executed ) {
        closeSignDb();
        return false;
   }



    executed = myquery.next();
    qDebug("(*4)...executed: %d",executed);
    if ( !executed ) {
        closeSignDb();
        return false;
    }

    resultvalue = myquery.value(0).toString().trimmed();
    qDebug();
    closeSignDb();
    return true;


}


void SafetYAWL::closeSignDb() {
     signDb.close();
}

void SafetYAWL::getNewTemplate(QTextStream& out, const QStringList& params, SafetYAWL::TemplateType t) {
	Q_ASSERT( params.count() > 0 );
	switch (t) {
		case NewWorkflowDefault:
		
			SafetYAWL::streamlog << SafetLog::Action << tr("Generando plantilla de flujo de trabajo nuevo...OK!");
			out <<   "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << endl;
			out <<   "<!--" << endl;
    			out <<   "Documento  : " << params.at(0) << endl;
    			out <<   "Creado     : " << QDateTime::currentDateTime().toString(Qt::SystemLocaleDate)  << endl;
    			out <<   "Autor      : nombre_autor" << endl;
    			out <<   "Descripcion: Archivo generado por plantilla de la Libreria SAFET" << endl;
			out <<   "-->" << endl << endl;
			out << "<!DOCTYPE yawl SYSTEM \"http://repositorio.cenditel.gob.ve/yawlworkflow.dtd\">" << endl << endl;
			out <<   "<yawl  version=\"0.01\">" << endl;
			out <<   "<workflow id=\"NuevoFlujo\" desc=\"Nuevo Flujo\">" << endl;
    			out <<   "\t<condition id=\"inicial\" type=\"start\">" << endl;
	    		out <<   "\t\t<port side=\"forward\" type=\"split\" query=\"true\"  tokenlink=\"\">" << endl;
        		out <<   "\t\t\t<connection   source=\"Tarea1\">" << endl;
        		out <<   "\t\t\t</connection>" << endl;
        		out <<   "\t\t</port>" << endl;
			out <<   "\t</condition>" << endl; 
    			out <<   "\t<task id=\"Tarea1\"  title=\"Tarea1\">" << endl;
			out <<   "\t\t<port side=\"forward\" type=\"split\" query=\"true\">" << endl;
			out <<   "\t\t\t<connection source=\"final\">" << endl;
			out <<   "\t\t\t</connection>" << endl;
			out <<   "\t\t</port>" << endl;
			out <<   "\t\t<variable id=\"VariableTarea1\" scope=\"task\"  type=\"sql\" config=\"1\"  source=\"end\" tokenlink=\"\" documentsource=\"\" >" << endl;
			out <<   "\t\t</variable>" << endl;
			out <<   "\t</task>" << endl;
			out <<   "\t<condition id=\"final\">" << endl;
			out <<   "\t\t<port side=\"forward\" type=\"split\">" << endl;
			out <<   "\t\t\t<connection source=\"\">" << endl;
			out <<   "\t\t\t</connection>" << endl;
			out <<   "\t\t</port>" << endl;
			out <<   "\t</condition>" << endl; 
			out <<   "\t<token key=\"campo_clave\"  keysource=\"tabla\">" << endl;
			out <<   "\t</token>" << endl;
			out <<   "</workflow>" << endl;
			out <<   "\t<configuration id=\"1\" connectstring=\"psql:localhost:dbname:dbuser:dbpassword\">" << endl;
			out <<   "\t</configuration>" << endl;
			out <<   "</yawl>" << endl;			
		break;
		case NewWorkflowSign:
			break;
		default:;

	}

}

bool SafetYAWL::loadWidgetPlugins(const QString& f) {
      qDebug("...loadWidgetPlugins...*entering...f: %s", qPrintable(f));
      if (SafetYAWL::listPlugins == NULL ){
          qDebug("SafetYAWL::listPlugins == NULL ");
           SafetYAWL::listPlugins  = new QList<QPluginLoader*> ();
      }
      QPluginLoader* myplugloader = new QPluginLoader();
      qDebug("SafetYAWL::loadWidgetPlugins ...(1): |%s|", qPrintable(f));
      myplugloader->setFileName(f);
      QObject *myplug = myplugloader->instance();
      if ( myplug == NULL ) {
           SafetYAWL::streamlog << SafetLog::Error << tr("NO SE cargo el complemento \"%1\"\n\t\tError: \"%2\"").arg(f).arg(myplugloader->errorString());
           return false;
      }
      SafetYAWL::streamlog << SafetLog::Action << tr("Se cargo el complemento \"%1\"...ok!").arg(f);

      if ( SafetYAWL::listDynWidget == NULL) {
           SafetYAWL::listDynWidget = new QList<WidgetInterface*> ();

      }
      Q_CHECK_PTR( SafetYAWL::listDynWidget );
      WidgetInterface* mywid = qobject_cast<WidgetInterface*>(myplug);
      Q_CHECK_PTR( mywid );
      mywid->setPath( f );
      SafetYAWL::listDynWidget->append( mywid );
      SafetYAWL::listPlugins->append( myplugloader);
      qDebug("...cargando...widgetplugins....:Tc");

      return true;
 }

/*
void SafetYAWL::loadPlugins(const QString& plugname) {

      if ( plugsloaded.contains( plugname)) return;
      if ( plugname.isEmpty() ) return;
      QString plugdir = SafetYAWL::getConfFile().getValue("Plugins", "plugins.path").toString();
 //     qDebug(".....plugdir:%s",qPrintable(plugdir));

    // plugdir = ".";
     QDir dir(plugdir);
     if ( !dir.exists() ) {
         SafetYAWL::streamlog
                 << SafetLog::Error
                 << tr("No se encuentra el directorio de complementos (plugins): \"%1\"")
                 .arg(plugdir);
         return;
     }

     dir.setFilter( QDir::Files );
     dir.setSorting(QDir::Size | QDir::Reversed);
          //dir.cd(plugdir);
     QStringList filters;
     filters << "libpnp*.so";
     dir.setNameFilters( filters );
     QStringList pluglist = dir.entryList(QDir::Files);
     QPluginLoader loader;
     if ( pluglist.count() == 0 ) {
          SafetYAWL::streamlog << SafetLog::Warning << tr("No existe archivos de complementos (del tipo libpnp_<nombre>.so) en el directorio: \"%1\"").arg(plugdir) << endl;
     return;
     }
     bool found = false;
     foreach(QString fileName, pluglist) {
          found = fileName.indexOf(plugname) != -1;
          if ( found ) {

               if ( ThreadConsole::curOutputInterface != NULL ) {
                    delete ThreadConsole::curOutputInterface;
                    ThreadConsole::curOutputInterface = NULL;
               }
               loader.setFileName(dir.absoluteFilePath(fileName));
               QObject *myplug = loader.instance();
             if (myplug) {
                  ThreadConsole::curOutputInterface = qobject_cast<OutputInterface*>(myplug);
             }
             break;
          }
     }
     if ( !found ) {
          SafetYAWL::streamlog << SafetLog::Warning << tr("No es posible encontrar el archivo de complemento solicitado \"libpnp_%1.so\" en el directorio: \"%2\"").arg(plugname).arg(plugdir) << endl;
     }
     plugsloaded.append( plugname );

}
*/

void SafetYAWL::loadPlugins(const QString& plugname) {
    qDebug("...plugname:%s", qPrintable(plugname));
    SYD << tr("........loadPlugins...:|%1|")
           .arg(plugname);
    if ( plugname.isEmpty() ) {
        qDebug("...contains...empty");
        SYD << tr("........loadPlugins...EMPTY");
        return;
    }
    if ( plugsloaded.contains( plugname)) {
        qDebug("...contains...");
        return;
    }
    QString plugdir = SafetYAWL::getConfFile().getValue("Plugins", "plugins.path").toString();
    qDebug(".....plugdir:%s",qPrintable(plugdir));

    SYD << tr("........loadPlugins...PLUGDIR:|%1|")
    .arg(plugdir);
    // plugdir = ".";
    QDir dir(plugdir);
    if ( !dir.exists() ) {
        SafetYAWL::streamlog
            << SafetLog::Error
            << tr("No se encuentra el directorio de complementos (plugins): \"%1\"")
                .arg(plugdir);
        return;
    }

    dir.setFilter( QDir::Files );
    dir.setSorting(QDir::Size | QDir::Reversed);
    //dir.cd(plugdir);
    QStringList filters;
    filters << "libpnp*.so";
    dir.setNameFilters( filters );
    QStringList pluglist = dir.entryList(QDir::Files);
    QPluginLoader loader;
    if ( pluglist.count() == 0 ) {
        SafetYAWL::streamlog << SafetLog::Warning << tr("No existe archivos de complementos (del tipo libpnp_<nombre>.so) en el directorio: \"%1\"").arg(plugdir) << endl;
        return;
    }
    bool found = false;
    foreach(QString fileName, pluglist) {
        qDebug("...fileName: %s", qPrintable(fileName));
        found = fileName.indexOf(plugname) != -1;
        if ( found ) {

            if ( SafetYAWL::curOutputInterface != NULL ) {
                delete SafetYAWL::curOutputInterface;
                SafetYAWL::curOutputInterface = NULL;
            }
            loader.setFileName(dir.absoluteFilePath(fileName));
            qDebug("...cargando abs: |%s|", qPrintable(dir.absoluteFilePath(fileName)));
            QObject *myplug = loader.instance();
            if (myplug) {
                qDebug("...cargando: |%s|", qPrintable(fileName));
                SafetYAWL::curOutputInterface = qobject_cast<OutputInterface*>(myplug);
            }
            break;
        }
    }
    if ( !found ) {
        SafetYAWL::streamlog << SafetLog::Warning << tr("No es posible encontrar el archivo de complemento solicitado \"libpnp_%1.so\" en el directorio: \"%2\"").arg(plugname).arg(plugdir) << endl;
    }
    plugsloaded.append( plugname );

    SYD << tr("........loadPlugins...PLUGLOADED:|%1|")
    .arg(plugname);


}


void SafetYAWL::setAutofilters(const QString& list, bool drop  ) {
         //autofilters.clear();
         autofilters = list.split(",");
         qDebug("...setAutofilters...autofilters.count(): %d",autofilters.count());
}

void SafetYAWL::setRecursivefilters(const QString& list, bool drop  ) {
         //autofilters.clear();
         recursivefilters = list.split(",");
         qDebug("...setRecursivefilters...recursivefilters.count(): %d",recursivefilters.count());
}

bool SafetYAWL::isActiveFilter(const QString& s ) {
  //    qDebug("...isActiveFilter...autofilters.count(): %d",autofilters.count());
      return autofilters.contains(s, Qt::CaseInsensitive);
}

bool SafetYAWL::isActiveRecursiveFilter(const QString& s ) {
  //    qDebug("...isActiveFilter...autofilters.count(): %d",autofilters.count());
      return recursivefilters.contains(s, Qt::CaseInsensitive);
}


QString SafetYAWL::toUcode(const QString& s) {
     QByteArray encodedString = s.toAscii();
     QTextCodec *codec = QTextCodec::codecForName("UTF-8");
     Q_CHECK_PTR( codec );
     return codec->toUnicode(encodedString);
}


QMap<QString,QVariant>& SafetYAWL::getColFieldFromMap(QMap<QString,QVariant>& map, const QStringList& path) {

     QMap<QString,QVariant> &result = map;
     if ( path.count() == 0 ) {
          return result;
     }
     QMap<QString,QVariant> &nextresult = (QMap<QString,QVariant> &)(result[ path.at(0) ]);
     for( int i = 1; i < path.count(); i++) {
          nextresult = (QMap<QString,QVariant> &)(nextresult[ path.at(i) ]);
//          qDebug("...path.at(%d): %s...", i, qPrintable(path.at(i)));
     }
     return nextresult;

}



void SafetYAWL::xmlToMap(const QString& s, QMap<QString,QVariant>& map) {
     QDomDocument mydoc;
     QString errorMsg;
     int errorLine, errorColumn;
     qDebug("....(1)...xmlToMap...interno...s: |%s|", qPrintable(s));
     mydoc.setContent(s, &errorMsg, &errorLine,&errorColumn );
     SafetYAWL::evalAndCheckExit(errorMsg.length() == 0,
      tr("El documento XML generado de la consulta presenta errores:(%2,%3)\n\"%1\"\n ").arg(errorMsg,errorLine,errorColumn));
     qDebug("...errorMsg...:|%s|", qPrintable(errorMsg));
     QDomElement docElem = mydoc.documentElement();
     QDomNode n = docElem.firstChild();
     QStack<QDomNode> control;
     QStack<QStringList> vcontrol;
     int colField = 0;
     QVariant vmap(map);
     QStringList last;
     while( true ) {
          if ( n.isNull() ) {
               if (control.isEmpty() ) break;
               n = control.pop();
               last = vcontrol.pop();
          }
          if ( n.isElement() ) {
                Q_ASSERT ( !n.firstChild().isNull() );
                QDomNode t = n.firstChild();
                if ( t.isText() ) {
                     SafetYAWL::getColFieldFromMap(map,last).insertMulti(n.nodeName(), QVariant(t.toText().data()));
                     n = n.nextSibling();
                     continue;
                }
                QMap<QString,QVariant> currentmap;
                SafetYAWL::getColFieldFromMap(map,last).insertMulti(n.nodeName(),  QVariant(currentmap));
               QDomElement e = n.toElement(); // try to convert the node to an element.

               if ( n.hasChildNodes() && n.firstChild().isElement() ) {
                    control.push(n.nextSibling());
                    vcontrol.push( last );
                    last.append( n.nodeName() );
                    n = n.firstChild();
                    continue;
               }

          }

          n = n.nextSibling();
     }

}


QString SafetYAWL::searchLiteral(const QString& l) {
     QString result = "n/a";
     qDebug("...SafetYAWL::searchLiteral....");
     if ( l == "_USERNAME" ) {
         if (!SafetYAWL::currentAuthUser().isEmpty()) {
             result = SafetYAWL::currentAuthUser();
        }
         else {
            result = _dbusername;
         }


     }
     else if ( l == "_COMBOVARGLOBAL0") {

//         result = SafetYAWL::combovarglobal0;
//         qDebug("...SafetYAWL::searchLiteral....:result %s", qPrintable(result));
     }
     return result;

}

QStringList  SafetYAWL::getTempNameFiles(int n) {
    QStringList list;
    if (n == 0 ) return list;
    Q_ASSERT(n > 0 );

    QList<QTemporaryFile*> filelist;

    for (int i= 0; i < n; i++) {
        filelist.push_back(new QTemporaryFile());
        Q_CHECK_PTR( filelist.at(i) );
        bool result = filelist.at(i)->open();
        Q_ASSERT_X(result, "getTempNameFile", qPrintable(tr("No fue posible abrir el archivo temporal")));
        list.push_back(filelist.at(i)->fileName());
    }

    Q_ASSERT(n == filelist.count()); // Deben ser igual el numero de la lista de archivos
    for (int i= 0; i < n; i++) {
        filelist.at(i)->close();
        delete filelist.at(i);
    }
    return list;
}


bool  SafetYAWL::replaceTextInFile(const QString& filename, const QString& searchtext,
                                   const QString& replacetext,  Qt::CaseSensitivity cs,
                                   int replacewith) {

    bool found = false;
    QStringList list = SafetYAWL::getTempNameFiles(1);
    //QString tempdir = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
    //qDebug("tempdir: %s", qPrintable(tempdir));
    Q_ASSERT( list.count() == 1 );
    QString tempname = list.at(0);
    QFile file( filename );
    QFile tempfile ( tempname );

    bool isopen = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if ( !isopen ) {
        SafetYAWL::streamlog << SafetLog::Error << QObject::tr("Ocurrió un error al tratar de abrir el archivo \"%1\" para aplicar modificaci�n").arg(filename);
        return false;
    }
    bool isopentemp = tempfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if ( !isopentemp ) {
        SafetYAWL::streamlog << SafetLog::Error << QObject::tr("Ocurrió un error al tratar de abrir archivo temporal \"%1\" para aplicar modificaci�n").arg(tempname);
        return false;
    }

    QTextStream in(&file);
    in.setCodec("ISO 8859-1");
    QTextStream out(&tempfile);
    out.setCodec("ISO 8859-1");
    QRegExp rx(searchtext);
    rx.setCaseSensitivity(cs);
    QString newreplacetext = replacetext;
    while ( !in.atEnd()) {
        QString line = in.readLine();
        int pos = rx.indexIn(line);
        //qDebug("......line: %s", qPrintable(line));
        if  ( pos != -1 ) {
            found = true;
            if (replacewith > 0 ) {
                newreplacetext.replace("||cap||",rx.cap(replacewith));
            }
            line.replace(rx, newreplacetext);
        }
        out << line.toLatin1();
        out << "\n";
    }
    qDebug("...finalizando lectura....");
    file.close();
    tempfile.close();
    SafetYAWL::streamlog << SafetLog::Debug << QObject::tr("Se escribió el archivo con texto reemplazado: \"%1\"").arg(tempname);
    bool result = QFile::remove(filename);    
    qDebug("...borrando...: %s", qPrintable(filename));
    if (!result ) {
        SafetYAWL::streamlog
                << SafetLog::Error <<
        QObject::tr("No es posible eliminar el archivo en la ubicacion: \"%1\"").arg(filename);
    }
    result = QFile::rename(tempname, filename);
    qDebug("...moviendo...: \"%s\" a \"%s\"", qPrintable(tempname),qPrintable(filename));
    if ( !result ) {
        SafetYAWL::streamlog
                << SafetLog::Error <<
        QObject::tr("No es posible copiar el archivo en la ubicacion: \"%1\"").arg(tempname);
    }

    return found;
        //(SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(result, tr("No es posible escribir el archivo en la ubicacion: %1").arg(commands['g'])) );




}


bool  SafetYAWL::replaceSectionInFile(const QString& filename, const QString& sectiontitle, const QString& replacetext,  Qt::CaseSensitivity cs) {


    bool found = false;
    QStringList list = SafetYAWL::getTempNameFiles(1);
    //QString tempdir = QDesktopServices::storageLocation(QDesktopServices::TempLocation);
    //qDebug("     ....replaceSectionInFile....tempdir: |%s|", qPrintable(tempdir));
    SYD << tr("...........SafetYAWL::replaceSectionInFile.........(1)...");
    Q_ASSERT( list.count() == 1 );
    QString tempname = list.at(0);
    QFile file( filename );
    QFile tempfile ( tempname );

    bool isopen = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if ( !isopen ) {
        SYE << QObject::tr("Ocurri� un error al tratar de abrir el archivo \"%1\" para aplicar modificaci�n").arg(filename);
        return false;
    }
    bool isopentemp = tempfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if ( !isopentemp ) {
        SYE << QObject::tr("Ocurri� un error al tratar de abrir archivo temporal \"%1\" para aplicar modificaci�n").arg(tempname);
        return false;
    }

    bool isreplacing = false;
    QTextStream in(&file);
    in.setCodec("ISO 8859-1");
    QTextStream out(&tempfile);
    out.setCodec("ISO 8859-1");
    QRegExp rx("\\[\\s*"+sectiontitle+"\\s*\\]");
    rx.setCaseSensitivity(cs);
    while ( !in.atEnd()) {
        QString line = in.readLine();
        int pos = rx.indexIn(line);
       // qDebug("......line: %s", qPrintable(line));
      //  qDebug("......pos: %d", pos);

        if ( isreplacing ) {
            QRegExp rxnext("\\[[a-zA-Z0-9_\\.\\-�������]+\\]");
            int posnext = rxnext.indexIn(line);
            if ( posnext != -1 ) {
                isreplacing = false;
            }
        }
        if (!isreplacing) {
            out << line.toLatin1();
            out << "\n";
        }
        if  ( pos != -1 ) {
        //    qDebug("............isreplacing...........");
            isreplacing = true;
            found = true;
            if (!replacetext.isEmpty()) {
                out << QString("# Valores escritos por SAFET - INFLOW %1\n")
                        .arg(QDateTime::currentDateTime().toString());
                out << replacetext;
                out << "\n";
            }
        }

    }
    if ( found ) {
            out << "\n";
    }
    //qDebug("           ....replaceSectionInFile...finalizando lectura....");
    file.close();
    tempfile.close();
    SYD << QObject::tr("Se escribio el archivo con texto reemplazado: \"%1\"").arg(tempname);
    bool result = QFile::remove(filename);
    qDebug("...borrando...: %s", qPrintable(filename));
    if (!result ) {
        SYE << QObject::tr("No es posible eliminar el archivo en la ubicacion: \"%1\"").arg(filename);
    }
    result = QFile::rename(tempname, filename);
    SYD << tr("...moviendo...: \"%1\" a \"%2\"").arg(tempname).arg(filename);
    if ( !result ) {

        SYE << QObject::tr("No es posible copiar el archivo en la ubicacion: \"%1\"").arg(tempname);
    }

    return true;
        //(SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(result, tr("No es posible escribir el archivo en la ubicacion: %1").arg(commands['g'])) );

}


void SafetYAWL::createUserSafetDirectory(QDir homedir) {
        bool isdir = homedir.mkdir(Safet::datadir);
            if (!isdir ) {
                SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf);
            }
	    // crear el subdirectorio ~/.safet/input
            bool issubdir = homedir.mkdir(Safet::datadir + "/input");
	    if (!issubdir) {
		SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf+"/input");
	    }
	    issubdir = false;

	    // crear el subdirectorio ~/.safet/flowfiles
            issubdir = homedir.mkdir(Safet::datadir + "/flowfiles");
	    if (!issubdir) {
		SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf+"/flowfiles");
	    }
	    issubdir = false;

	    // crear el subdirectorio ~/.safet/dtd
            issubdir = homedir.mkdir(Safet::datadir + "/dtd");
	    if (!issubdir) {
		SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf+"/dtd");
	    }
	    issubdir = false;

	    // crear el subdirectorio ~/.safet/certs
            issubdir = homedir.mkdir(Safet::datadir + "/certs");
	    if (!issubdir) {
		SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf+"/certs");
	    }
            issubdir = false;

            // crear el subdirectorio ~/.safet/certs
            issubdir = homedir.mkdir(Safet::datadir + "/graphs");
            if (!issubdir) {
                SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"")
                        .arg(SafetYAWL::pathconf+"/graphs");
            }

	    issubdir = false;

	    // crear el subdirectorio ~/.safet/log
            issubdir = homedir.mkdir(Safet::datadir + "/log");
	    if (!issubdir) {
		SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf+"/log");
	    }
	    issubdir = false;

	    // crear el subdirectorio ~/.safet/reports
            issubdir = homedir.mkdir(Safet::datadir + "/reports");
	    if (!issubdir) {
		SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf+"/reports");
	    }
	    issubdir = false;

	// crear el subdirectorio ~/.safet/sql
            issubdir = homedir.mkdir(Safet::datadir + "/sql");
	    if (!issubdir) {
		SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf+"/sql");
	    }
	    issubdir = false;

	    // crear el subdirectorio ~/.safet/xmlrepository
            issubdir = homedir.mkdir(Safet::datadir + "/xmlrepository");
	    if (!issubdir) {
		SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf+"/xmlrepository");
	    }
	    issubdir = false;

	    // crear el subdirectorio ~/.safet/images
	    issubdir = homedir.mkdir(Safet::datadir + "/"+ Safet::imagesdir);
            if (!issubdir) {
                SafetYAWL::streamlog << SafetLog::Error << tr("Ocurrio un error al tratar "
                                                              "de crear el directorio  de datos de SAFET: \"%1\"").arg(SafetYAWL::pathconf+"/images");
            }

}
void SafetYAWL::copyBinFileToSafetDirectory(const QString& sourceFile, const QString& targetDirectory) {


    QString rcfile( ":/"+sourceFile);
    QString namefile = SafetYAWL::pathconf;
    if (targetDirectory.isEmpty()){
            namefile.append("/"+sourceFile);
    }
    else{
            namefile.append("/"+targetDirectory+"/"+sourceFile);
    }

    qDebug("...SafetYAWL::copyBinFileToSafetDirectory: |%s|->|%s|",
           qPrintable(rcfile),qPrintable(namefile));
    QFile::copy(rcfile,namefile);


}

void SafetYAWL::copyFileToSafetDirectory(const QDir& homedir,const  QString& sourceFile,
                                         const QString& targetDirectory) {

	qDebug("...SafetYAWL::copyFileToSafetDirectory");
	QFile rcfile( ":/"+sourceFile);
        bool isopenrc = rcfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if ( !isopenrc ) {
                SafetYAWL::streamlog
                        << SafetLog::Error
                        << tr("Ocurrió un error al tratar de leer archivo: \"%1\"").arg(sourceFile);
                return;
        }
	QString namefile = SafetYAWL::pathconf;
	if (targetDirectory.isEmpty()){
		namefile.append("/"+sourceFile);
	}
	else{
		namefile.append("/"+targetDirectory+"/"+sourceFile);
	}
        qDebug("...File Name to create:...: |%s|", qPrintable(namefile));
        QFile file( namefile );
        bool isopen = file.open(QIODevice::WriteOnly | QIODevice::Text);
        if ( !isopen ) {
        	SafetYAWL::streamlog << SafetLog::Error << 
                tr("Ocurrió un error al tratar de escribir archivo : \"%1\"").arg(namefile);
                return;
        }
        QTextStream in(&rcfile);
        in.setCodec("ISO 8859-1");
        QTextStream out(&file);
        out.setCodec("ISO 8859-1");
        while (!in.atEnd()) {
                QString line = in.readLine();
                line.replace("$USER", homedir.homePath());

                out << line;
                out << "\n";
        }
        rcfile.close();
        file.close();
}

void SafetYAWL::copyWithRe(const  QString& sourceFile,
                                         const QString& targetFile,
                                         const QString& re,
                                         const QString& replacetext) {

        QRegExp rx;
        rx.setPattern(re);
        QFile infile( sourceFile);
        bool isopenrc = infile.open(QIODevice::ReadOnly | QIODevice::Text);
        if ( !isopenrc ) {
                SafetYAWL::streamlog
                        << SafetLog::Error
                        << tr("Ocurrió un error al tratar de leer archivo: \"%1\"").arg(sourceFile);
                return;
        }
        QString namefile = targetFile;
        QFile outfile( namefile );
        bool isopen = outfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
        if ( !isopen ) {
                SafetYAWL::streamlog << SafetLog::Error <<
                tr("Ocurrió un error al tratar de escribir archivo : \"%1\"").arg(namefile);
                return;
        }
        QTextStream in(&infile);
        in.setCodec("ISO 8859-1");
        QTextStream out(&outfile);
        out.setCodec("ISO 8859-1");
        while (!in.atEnd()) {
                QString line = in.readLine();
                line = line.replace(rx, replacetext );
                out << line;
                out << "\n";
        }
        infile.close();
        outfile.close();
}



void SafetYAWL::copyDirectoryContentToSafetDirectory(QDir homedir, QString sourceDirectory, QString targetDirectory) {
	qDebug("...SafetYAWL::copyDirectoryContentToSafetDirectory");
	homedir.cd(sourceDirectory);
	QStringList filters;
	filters << "*.gif" << "*.css" << "*.html" << "*.js";
	homedir.setNameFilters(filters);
	QStringList list = homedir.entryList();
	QString sourceFile("");
	for (int i = 0; i < list.size(); ++i) {
		sourceFile = list.at(i).toLocal8Bit().constData();
		QFile rcfile( ":/"+sourceFile);
	
	        bool isopenrc = rcfile.open(QIODevice::ReadOnly | QIODevice::Text);
	        if ( !isopenrc ) {
        		SafetYAWL::streamlog << SafetLog::Error << tr("Ocurri� un error al tratar de leer archivo: \"%1\"").arg(sourceFile);
        	}
		QString namefile = SafetYAWL::pathconf;
		if (targetDirectory.isEmpty()){
			namefile.append("/"+sourceFile);
		}
		else{
			namefile.append("/"+targetDirectory+"/"+sourceFile);
		}
        	qDebug("...File Name to create:...: |%s|", qPrintable(namefile));
	        QFile file( namefile );
        	bool isopen = file.open(QIODevice::WriteOnly | QIODevice::Text);
        	if ( !isopen ) {
        		SafetYAWL::streamlog << SafetLog::Error << 
                        tr("Ocurrió un error al tratar de escribir archivo : \"%1\"").arg(namefile);
        	}
	        QTextStream in(&rcfile);
                in.setCodec("ISO 8859-1");
	        QTextStream out(&file);
                out.setCodec("ISO 8859-1");
	        while (!in.atEnd()) {
                        QString line = in.readLine();
			if (line.contains("$USER") > 0 ) {
				line.replace("$USER", homedir.homePath());
			}
                        out << line;
                        out << "\n";
        	}
        	rcfile.close();
        	file.close();
	}
}

void SafetYAWL::addErrorToStack(const QDateTime& d, const QString& m) {
     lasterrors.append(QPair<QDateTime,QString>(d,m));

}
void SafetYAWL::addWarnToStack(const QDateTime& d, const QString& m) {
    lastwarnings.append(QPair<QDateTime,QString>(d,m));
}


QByteArray SafetYAWL::readStringFromFile(const QString& namefile) {
QByteArray myxml;
    QFile file(namefile);

if (!file.open(QIODevice::ReadOnly)) {
    SafetYAWL::streamlog << SafetLog::Error <<
    tr("No es posible abrir el archivo: \"%1\"")
    .arg(namefile);
    return myxml;
}

myxml = file.readAll();

file.close();

return myxml;
}


QString SafetYAWL::getSubField(const QString& s, int nfield, int pos) {
    QString result;
    QStringList vars = s.split("," /*,QString::SkipEmptyParts */);
    int i = 0;
    if ( nfield >= vars.count()) {
        SafetYAWL::streamlog
                << SafetLog::Warning
                << tr ("El campo Número \"%1\" no existe en la cadena \"%2\"")
                .arg(nfield)
                .arg(s);
        return result;
    }
    QString field = vars.at(nfield);
    if ( pos >= vars.count()) {
        SafetYAWL::streamlog
                << SafetLog::Warning
                << tr ("El Subcampo Número \"%3\" del campo Numero \"%1\" no existe en la cadena \"%2\"")
                .arg(nfield)
                .arg(s)
                .arg(pos);

        return result;
    }
    result = field.split("::").at(pos);

    return result;

}


QString SafetYAWL::replaceArgsflow(const QString & s) {
    QString result = s;
    result.replace("_USERNAME", SafetYAWL::currentAuthUser());

    QString pattern = QString("{#%1}");


//    SYD << tr("..........SafetYAWL::replaceArgsflow count:|%1|").arg(SafetYAWL::argsflow.count());
    for(int i = 0; i < SafetYAWL::argsflow.count(); i++ ) {
        if (argsflow.at(i).isEmpty()) {
            continue;
        }
        QString number = QString("%1").arg(i+1).trimmed();
        pattern = pattern.arg(number);
       result.replace(pattern,argsflow.at(i));
    }

    return result;
}


QString SafetYAWL::getNumberVersion() {
    return QString(NUMBER_VERSION);

}

QString SafetYAWL::getRevisionVersion() {
    return QString(REVISION_VERSION);
}

QString SafetYAWL::getLastlogVersion() {
    return QString(LASTLOG_VERSION);
}

QString SafetYAWL::getInfoVersion() {
    return QString(INFO_VERSION);
}

bool SafetYAWL::checkCompletedTask(const QString &idtask, const QString &json)
{
    QStringList mylist;
    SYD << tr("...........SafetYAWL::checkCompletedTask...........***idtask:|%1|")
           .arg(idtask);

    mylist = json.split("\n",QString::SkipEmptyParts);
    QRegExp rx;
    rx.setPattern("\"(\\d+)\\%\"\\s*\\]");
//    SYD << tr("...........SafetYAWL::checkCompletedTask...........rx.pattern():|%1|")
    //       .arg(rx.pattern());

    foreach(QString s, mylist) {

        if (s.indexOf(idtask) > 0 ) {

            int pos = s.indexOf(rx);

            if (pos > 0 ) {

                SYD << tr("...........SafetYAWL::checkCompletedTask...........**porcentaje:|%1|")
                       .arg(rx.cap(1));
                bool ok;
                int value = rx.cap(1).toInt(&ok);
                if (value > 0 )  {
                    return true;
                }

            }
            else {
                SYW << tr("En SafeYAWL::checkCompletedTask mal formato de json:|%1|")
                       .arg(s);
            }

            break;

        }
    }

    return false;

}


QString SafetYAWL::shrinkSentence(const QString& s, uint maxchars ) {
    QString result = s;
    uint len = result.count();
    bool isremoving = false;
//    SYD << tr("SafetYAWL::shrinkSentence....len:%1 ...maxchars:%2")
//           .arg(len)
//           .arg(maxchars);

    while ( len > maxchars) {
        uint middle = len / 2;
        QRegExp  rx("([a-zA-Z�������0-9_\\-]+)");
        int pos = result.indexOf(rx,middle);
        if (pos == -1 ) {
            SYW << tr("Fallo al tratar de comprimir la frase (SafetYAWL::shrinkSentence):\"%1\""
                      " en la posici�n media: \"%2\"")
                   .arg(s)
                   .arg(middle);
            break;
        }
        if ((pos+rx.cap(1).count()+6)> len) {
            break;
        }
        result.remove(pos,rx.cap(1).count());
        isremoving = true;
        len = result.count();
    }
    if ( isremoving) {
        result.insert(result.count()/2,QLatin1String("..."));
    }
    return result;
}


QString SafetYAWL::getScriptLen(const QSqlField& f) {

    QString result = "40";
     SYD << tr(".............MainWindow::getScriptLen....name:|%1|")
           .arg(f.name());

     if( f.name().indexOf(tr("fecha"))>= 0 ) {
         result = "130";
         return result;
     }
    switch ( f.type()) {
    case QVariant::String:

        if (f.name().startsWith("ruta") || f.name().startsWith("archivo")) {
            result = "250";
        }
        else if (f.value().toString().length() > 50 || f.name().contains("desc")) {
            result = "220";
        }
        else {
            result = "90";
        }
        break;
    case QVariant::UInt:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::ULongLong:
    case QVariant::Double:
        result = "10";
        break;
    case QVariant::Date:
    case QVariant::DateTime:
                result = "260";
    default:;

    }

    return result;
}


