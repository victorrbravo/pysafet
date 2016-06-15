/****************************************************************************
**
** Copyright (C) 2006-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
/*
* SAFET Sistema Automatizado para la Firma Electr?nica y Estampado de Tiempo
* Copyright (C) 2008 V?ctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigaci?n en Tecnologías Libres
*
* Este programa es software libre; Usted puede usarlo bajo los t?rminos de la licencia de
* software GPL versi?n 2.0 de la Free Software Foundation.
*
* Este programa se distribuye con la esperanza de que sea ?til, pero SI NINGUNA GARANT?A;
* tampoco las impl??citas garant??as de MERCANTILIDAD o ADECUACIÓN A UN PROP?SITO PARTICULAR.
* Consulte la licencia GPL para m?s detalles. Usted debe recibir una copia de la GPL junto
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5? Piso, Boston, MA 02110-1301, USA.
*
*/




//#include <QCloseEvent>
#include <QGraphicsSvgItem>
//#include <QToolBar>
#include <QLatin1String>
//#include <QWebFrame>
//#include <QDesktopServices>
#include <QVariantMap>
#include <QVariantList>
#include <QCoreApplication>
#include <QScriptEngine>

#ifdef SAFET_TAR
//#include "ui/getsigndocumentdialog.h"
//#include "ui/sendsigndocumentdialog.h"
#endif


#include <getopt.h>
#include "mainwindow.h"
//#include "textedit.h"
//#include "qcmdcompleter.h"
#include "SafetTextParser.h"
#include "SafetYAWL.h"
#include "assistant.h"
#include "threadconsole.h"
#ifdef SAFET_OPENSSL
#include <openssl/rand.h>
#include "SafetCipherFile.h"
#endif
//#include "dialogflowparameters.h"
#include "combowidget.h"
#include "getfilewidget.h"
#include "listwidget.h"
#include "texteditwidget.h"
#include "datetimewidget.h"
#include "outputwidget.h"
#include "numberwidget.h"
#include "suggestwidget.h"
#include "SafetBinaryRepo.h"

// **** Bdoc files
#ifdef SAFET_DIGIDOCPP
#include "log.h"
#include "BDoc.h"
#include "Conf.h"
#include "Document.h"
//#include "crypto/crypt/RSACrypt.h"
//#include "crypto/Digest.h"

#include "crypto/cert/X509Cert.h"
#include "crypto/cert/X509CertStore.h"

#include "crypto/cert/DirectoryX509CertStore.h"

#include "bdocdocument.h"
#endif



//#include "inflowfirmadoc.h"
#ifdef SAFET_SMTP
#include "../smtp/SmtpMime"
#endif

#include "inflowfirmadoc.h"

// **** Bdoc files



// Variables Estáticas

SafetYAWL* MainWindow::configurator = NULL;
//QStatusBar* MainWindow::mystatusbar = NULL;
QStringList  MainWindow::sbmenulist;
MainWindow *MainWindow::mymainwindow = NULL;
QString MainWindow::currentaccount = "";
QString MainWindow::currentrole = "";
QString MainWindow::currentrealname = "";



QString MainWindow::showString = "";
bool MainWindow::_issigning = false;
QString MainWindow::_currentCommonName = "";
QMap<QString,QPair<QString,QString> > MainWindow::_graphs;

//ThreadConsole MainWindow::myThreadConsole(NULL);

QMap<QString,QStringList> MainWindow::permises; // Estructura para guardar los permisos




#ifdef SAFET_TAR
extern "C" {
#include "libdotar.h"
}
#endif


//ThreadConsole * MainWindow::myThreadConsole = NULL;

MainWindow::MainWindow(const QString& path)
     : streamText( &outputText )
{

//    QCoreApplication::addLibraryPath ( "/home/victorrbravo/PySafet/plugins/sqldrivers" );
//    QCoreApplication::addLibraryPath ( "/home/victorrbravo/PySafet/plugins" );
//    QCoreApplication::addLibraryPath ( "/usr/lib/libsafet" );


    _wfnumpars  = 0; // Contador temporal utilizado para calcular el número de parámetro en la función GetWidget
     //** Inicializacion de variables
     //** ...

     SafetYAWL::listDynWidget = NULL;

     parsed = false;
     MainWindow::mymainwindow = this; // Colocando direcci?n de MainWindow
     _listprincipalcount = 0; // Contador para la variable actual

    // ** Colocar los manejadores para Los servicioes del Escritorio (QDesktopServices)


     // ** Configurando el Menu de la pantalla principal

     _currentjson = QLatin1String("");

     // Colocar estilo principal


     _dirInput = SafetYAWL::getConf()["Input/path"];
     if ( !_dirInput.endsWith("/") ) _dirInput.append("/");

     _pathdoc = SafetYAWL::getConf()["Input/file"];

     filecompleters.append( _dirInput+_pathdoc);


     filecompleters.append( _dirInput + SAFETCONSOLEFILE );
     filecompleters.append( _dirInput + SAFETCONFIGFILE );
     filecompleters.append( _dirInput + SAFETMANAGEMENTSIGNFILE );
     filecompleters.append( _dirInput + SAFETUSERSFILE );


     loadSettings();

     // ********* Cargar Widgets Plugins
     int countplug = 0;
//     SafetYAWL::fileout.close();
     SafetYAWL::filelog.close();
     QString logpath = SafetYAWL::getConf()["Log/filepath"];
//     foreach( QString s, plugs() ) {
//          if ( loadWidgetPlugins( s  ) ) {
//              qDebug("...loadWidgetPlugins: %s",qPrintable(s));
//              countplug++;
//          }
//     }


//     SafetYAWL::fileout.open(stdout, QIODevice::WriteOnly);
//     SafetYAWL::streamout.setDevice(&SafetYAWL::fileout);



     SafetYAWL::filelog.setFileName(logpath + /*"/safet.log"*/ SafetYAWL::FILENAMELOG);
     qDebug("..asignado archivo log: %s",
            qPrintable(logpath + /*"/safet.log"*/ SafetYAWL::FILENAMELOG));
     SafetYAWL::filelog.open(QIODevice::Append);
     SafetYAWL::streamlog.setDevice(&SafetYAWL::filelog);

     SafetYAWL::streamlog.turnOnAll();




     jscriptload = false; // No cargar el script


     // Conectando el timer del panel de resultados



    initApp = false;

    // *** Conexiones

    // para mostrar el assistant
    // Colocar la Configuracion Local
    QLocale::setDefault(QLocale::C);
//    setlocale(LC_ALL, "C");

    // Configurar lista de mensajes de Digidoc
    SafetDocument::configureDigidocErrorString();


// ------------------------
    // verificar la configuracion de la fuente de datos
    // se coloca luego de new SafetYAWL() para que los archivos de
    // configuracion ya esten leidos


    if (path.isEmpty()) {
        _currconfpath = QDir::homePath();
    }
    else {
        _currconfpath = path;
        SafetYAWL::pathconf = path + "/" + Safet::datadir;
    }
    if ( _currconfpath.isEmpty()) {
        _currconfpath = QLatin1String("/var/www/");
    }

    QString authconfbin = _currconfpath+ "/.safet/auth.conf.bin";
    if (!QFile::exists(authconfbin)) // no existe el auth.conf.bin asi que se hace todo el proceso
    {
     

    }

    MainWindow::configurator = new SafetYAWL(_currconfpath + "/" + Safet::datadir);
    //if ( countplug > 0 ) {
        SafetYAWL::loadAllConfFiles(3 /* AuthConf */ );
    //}


    if( MainWindow::configurator ) {
        MainWindow::configurator->openDataSources();
    }

    SYD << tr("....MainWindow::MainWindow...SafetYAWL::_CURRCONFpath...:|%1|")
           .arg(_currconfpath + "/" + Safet::datadir);


    SYD << tr("....MainWindow::MainWindow...SafetYAWL::pathconf...:|%1|")
           .arg(SafetYAWL::pathconf);
    
    loadAuthorization();

    SYD << tr("....MainWindow::MainWindow...loadAuthorization()....(1)");


    loadInfo();

    SYD << tr("....MainWindow::MainWindow...loadInfo()....(1)");

    SYD << tr("check ADDING_PAR...1");

    checkDefaultValuesParameters();

    SYD << tr("check ADDING_PAR...2");


}



bool MainWindow::sendCheckEmail(const QString& user, const QString& plink) {
#ifdef SAFET_SMTP
    QString myhost = SafetYAWL::getConf()["Email/host"];
    if  (myhost.isEmpty()) {
        myhost = "localhost";
    }

    SmtpClient smtp(myhost, 25, SmtpClient::TcpConnection);


       QStringList infolist = getInfoOfUser(user);
       QString uinfo;
       QString email;
       if (infolist.count() > 2 )  {
           uinfo = infolist[2];
           QRegExp rx;


           rx.setPattern(Safet::EMAIL_PATTERN);


          SYD << tr("..........***MainWindow::sendCheckMail........entering...rx.pattern():|%1|").arg(rx.pattern());
          SYD << tr("..........***MainWindow::sendCheckMail........entering...UINFO:|%1|").arg(uinfo);
          int pos = uinfo.indexOf(rx);
          if (pos >= 0 ) {
              email = rx.cap(0);
              SYD << tr("..........**************MainWindow::sendCheckMail..POS0......entering...email:|%1|").arg(email);
          }

          SYD << tr("..........MainWindow::sendCheckMail..POS*1*......entering...email:|%1|").arg(email);

       }


       QString myuser = SafetYAWL::getConf()["Email/user"];
       if  (myuser.isEmpty()) {
           myuser = "vbravo";
       }
       QString mypass = SafetYAWL::getConf()["Email/password"];
       if  (mypass.isEmpty()) {
           mypass = "4n0d0";
       }

       SYD << tr("..........MainWindow::sendCheckMail....SENDEMAILLOCALHOST .user:|%1|")
              .arg(myuser);
       SYD << tr("..........MainWindow::sendCheckMail....SENDEMAILLOCALHOST .pass:|%1|")
              .arg(mypass);

       smtp.setUser(myuser);
       smtp.setPassword(mypass);

       // Create a MimeMessage

       MimeMessage message;
       QString hostsender = SafetYAWL::getConf()["Email/hostsender"];
       if  (hostsender.isEmpty()) {
           hostsender = "root@proxy1.cenditel.gob.ve";
       }

       QString namesender = SafetYAWL::getConf()["Email/namesender"];
       if  (namesender.isEmpty()) {           
           namesender = "Sistema - SAFET";
       }

       message.setSender(new EmailAddress(hostsender, namesender));
       if (email.isEmpty()) {
           email = "victorrbravo@gmail.com";

       }
       message.addRecipient(new EmailAddress(email, uinfo));



       SYD << tr("..........MainWindow::sendCheckMail........previous");
       // Add some text
       MimeText text;
       QString mytext;

       if (plink.indexOf("/activeaccount") != -1 ) {
           QString mymessage = SafetYAWL::getConf()["Messages/email.subject.active"];
           if (mymessage.isEmpty()) {
               mymessage = "Activación de cuenta - Sistema Safet";
           }



           message.setSubject(mymessage);
           mytext = QString("Para ACTIVAR  la cuenta del usuario \"%1\" debe seguir el enlace %2."
                                    "\n\nGracias por su atención.\n\n"

                                    "")
                   .arg(user)
                   .arg(plink);

        }
       else {
           QString mymessage = SafetYAWL::getConf()["Messages/email.subject.reset"];
           if (mymessage.isEmpty()) {
               mymessage = "Reinicio de contraseña - Sistema Safet";
           }

           message.setSubject(mymessage);
           mytext = QString("Para reiniciar la contraseña del su usuario \"%1\" debe seguir el enlace %2."
                                    "\n\nGracias por su atención.\n\n"
                                    "")
                   .arg(user)
                   .arg(plink);
       }

       text.setText(mytext);
       message.addPart(&text);


       int ishost = smtp.connectToHost();
       SYD << tr("..........MainWindow::sendCheckMail....SENDEMAILLOCALHOST .ishost:|%1|")
              .arg(ishost);
       int islogin = smtp.login();
       SYD << tr("..........MainWindow::sendCheckMail....SENDEMAILLOCALHOST .islogin:|%1|")
              .arg(islogin);

       if (ishost) {
          smtp.sendMail(message);
          smtp.quit();
          SYA << tr("..........MainWindow::sendCheckMail........SENDING_OK");
          return true;
       }
       SYA << tr("..........MainWindow::sendCheckMail........SENDING_NO");
  #endif
       return false;



}

QString MainWindow::checkUserRegister(const QString& fullname,
                                      const QString& account,
                                      const QString& email,
                                      const QString& passone,
                                      const QString& passtwo, const QString& ticket) {

//    QString result = tr("<b>Usuario registrado (%1)</b>. Espere el correo de completación de registro en la cuenta que suministró.")
//            .arg(account);
//    QString result = tr("Datos: nombres:|%1| cuenta:|%2| email: |%3| passone:|%4| passtwo:|%4|")
//            .arg(fullname)
//            .arg(account)
//            .arg(email)
//            .arg(passone)
//            .arg(passtwo);

#define FULLNAME  0x01
#define ACCOUNT   0x02
#define EMAIL     0x04
#define PASSONE   0x08
#define PASSTWO   16


    QString mymessage = SafetYAWL::getConf()["Messages/after.register"];

    if (mymessage.isEmpty()) {
        mymessage = tr("Se enviado un correo de activación de registro.");
    }

    SYD << tr(".....MainWindow::checkUserRegister............entering()");
    QString result = mymessage;
    int typecheck = 0;

    QString newfullname = fullname;
    if (newfullname.isEmpty()) {
        newfullname = Safet::NA;
    }

    typecheck =  (((!newfullname.isEmpty()) & FULLNAME))
     + (((!account.isEmpty()) << 1) & ACCOUNT)
    +  ((((!email.isEmpty()) << 2) & EMAIL))
    +  ((((!passone.isEmpty()) << 3) & PASSONE))
    +  (((!passtwo.isEmpty()) << 4) & PASSTWO);


    SYD << tr("........MainWindow::checkUserRegister....typecheck:|%1|")
           .arg(typecheck);


    // Campos Obligatorios
    SYD << tr("........MainWindow::checkUserRegister....(PASSTWO):|%1|")
           .arg(PASSTWO);

    if ( (typecheck & FULLNAME) == 0) {
        return tr("El campo \"Nombre Completo\" es obligatorio, debe escribir algún nombre");
    }

    if ( (typecheck & ACCOUNT) == 0) {
        return tr("El campo \"Cuenta\" es obligatorio");
    }

    if ( (typecheck & EMAIL) == 0) {
        return tr("El campo \"Correo-e\" es obligatorio");
    }

    SYD << tr("........MainWindow::checkUserRegister....(typecheck & FULLNAME):|%1|")
           .arg((typecheck & FULLNAME));

    if ( ((typecheck & PASSONE) == 0) || ((typecheck & PASSTWO) == 0)) {
        return tr("No puede dejar los campos de asignación de contraseñas vacíos");
    }

    // Expresiones regulares

    QRegExp rx(".{3,}");
    if (!rx.exactMatch(newfullname) ) {
        return tr("El nombre completo debe tener una longitud mayor o igual a 3 caracteres");
    }

    rx.setPattern("[a-zA-Z\\.0-9]{3,}");
    if (!rx.exactMatch(account) ) {
        return tr("El nombre de la cuenta debe contener caracteres alfanuméricos, y debe tener una longitud mayor o igual a 3 caracteres");
    }

    QString EMAIL_PATTERN =
            QString("^[_A-Za-z0-9-\\+]+(\\.[_A-Za-z0-9-]+)*@")
            + "[A-Za-z0-9-]+(\\.[A-Za-z0-9]+)*(\\.[A-Za-z]{2,})$";

    rx.setPattern(EMAIL_PATTERN);
    if (!rx.exactMatch(email) ) {
        return tr("El correo electrónico debe tener una dirección válida. Por ejemplo: foo@acme.org");
    }

    rx.setPattern(".{6,}");

    if (!rx.exactMatch(passone) ) {
        return tr("La contraseña debe tener más de 6 caracteres");
    }

//    if (passone != passtwo) {
//        return tr("La contraseñas deben ser iguales");
//    }

    if( users.contains(account)) {
        return tr("Ya existe un usuario con el nombre de cuenta \"%1\". Seleccione otro nombre de cuenta.").arg(account);
    }


        srand( (unsigned int)time( NULL ) );

        QString myrandom = QString("%1").arg(rand());
        QByteArray textsha1 = QCryptographicHash::hash(QByteArray(qPrintable(myrandom)),
                                                                       QCryptographicHash::Sha1);


        QByteArray passha1 = QCryptographicHash::hash(QByteArray(qPrintable(passone)),
                                                                       QCryptographicHash::Sha1);

        QString useEmail = SafetYAWL::getConf()["Email/used"];
        QString emailRole = SafetYAWL::getConf()["Email/newrole"];
        if (emailRole.isEmpty()) {
            emailRole = "Administrador";
        }

        QString newhash = "";
        if (useEmail == "on") {

                SYD << tr("........(enviando check).......MainWindow::checkUserRegister...useEmail...on");
                newhash = QString("_%1_%2_%3")
                .arg(account)
                .arg(textsha1.toHex().data())
                .arg(passha1.toHex().data());


        }
        else {
            SYD << tr("........(enviando check).......MainWindow::checkUserRegister...useEmail...off");
            newhash = QString("%1")
    //                .arg(account)
    //                .arg(textsha1.toHex().data())
                    .arg(passha1.toHex().data());

        }

        QString myaction = "";




            myaction = QString("operacion:Agregar_usuario Nombre_cuenta_usuario: %1 Contraseña_usuario: %2 "
               "Nombre_completo_usuario: %3 Rol_del_Usuario: %4 Ticket:%5")
                .arg(account)
                .arg(newhash)
                .arg(newfullname + " " + email)
                .arg(emailRole)
                .arg(ticket.isEmpty()?passtwo:ticket);



        SYD << tr(".................CHECKUSER......MainWindow::checkUserRegister....myaction:|%1|")
               .arg(myaction);


       toInputUsers(myaction);

       loadInfo();


        QString link = QString("%3/activeaccount/%1_%2")
                .arg(account)
                .arg(textsha1.toHex().data())
                .arg(hostURL());

        SYD << tr("........(enviando check).......MainWindow::checkUserRegister............LOCALLINK:|%1|").arg(link);

        SYD << tr("...............MainWindow::checkUserRegister...sending email (before)....");
        if (useEmail == "on") {
                SYD << tr("...............MainWindow::checkUserRegister...useEmail...SENDINGEMAIL....useEmail...on");
                sendCheckEmail(account,link);
        }
        SYD << tr("...............MainWindow::checkUserRegister...sending email (after)....");


    return result;
}






QString MainWindow::generateCaptcha() {


    QString result;


//    _captcha.getCaptcha();


    return result;
}




QString MainWindow::loadReportTemplate(const QString& json,const QString &filename,
                                       const QString& nameoperation) {

    QString html;




    QStringList mylist = nameoperation.split(":",QString::SkipEmptyParts);


    QStringList listjson = json.split("</SAFETHTMLHEADSEPARATOR>",QString::SkipEmptyParts);


    QString newjson = listjson.at(0);

    QString newhead = "";
    if (listjson.count() > 1 ) {
	    newhead = listjson.at(1);
    }

    QStringList mydatas = json.split(SafetYAWL::LISTSEPARATORCHARACTER,QString::SkipEmptyParts);



    QString shortnameop = mylist.at(mylist.count()-1);


        QString fullnamePage;
        if ( filename.isEmpty()) {
            fullnamePage = SafetYAWL::getConf()["Reports/general.template"];
        }
        else {
            fullnamePage = filename;
        }
//       QString namePage = fullnamePage.section("/",-1);


       QFile infile(fullnamePage);
       if ( !infile.open(QIODevice::ReadOnly) ) {

               SYE << tr("No es posible leer el archivo de plantilla: \"%1\" ")
                      .arg(fullnamePage);
           return html;
       }

       QTextStream in(&infile);
       in.setCodec("ISO 8859-1");
       bool isreplacing = false;
       while(!in.atEnd()) {
           QString line = in.readLine();
           if (line.indexOf("<SAFET/>") != -1 && !isreplacing) {
                line.replace("<SAFET/>",newjson);
           }
           else if (line.indexOf("<SAFETHEAD/>") != -1 && !isreplacing) {
                line.replace("<SAFETHEAD/>",newhead);
           }


           if (mydatas.count()>1 && line.indexOf("<SAFETFILENAME/>") != -1 && !isreplacing) {
                line.replace("<SAFETFILENAME/>",mydatas.at(1));
           }

           if (line.indexOf("<SAFETCURRENTACCOUNT/>") != -1 && !isreplacing) {
               QString myformat = SafetYAWL::getConf()["GeneralOptions/account.format"].trimmed();
               if ( myformat.isEmpty()) {
                   myformat = "%1 - %2/%3";
               }
               else {
                   myformat.replace("%realname","%1");
                   myformat.replace("%accountname","%2");
                   myformat.replace("%rolname","%3");
               }

               QString curr;

               if ( !MainWindow::currentaccount.isEmpty()) {
	       QStringList currentroles = MainWindow::currentrole.split(";",QString::QString::SkipEmptyParts);
	       QString mytitlerole;
	       for( int i=0; i < currentroles.count(); i++) {
			mytitlerole += currentroles.at(i);
			if ( (i+2) == currentroles.count() ) {
				mytitlerole += " y ";
			}
			else {
				if ( (i+1) < currentroles.count() ) {
				mytitlerole += ",";
				}
			}

		}

               curr = QString(myformat)
                                      .arg(MainWindow::currentrealname)
                       .arg(MainWindow::currentaccount)
                       .arg(mytitlerole);
               }


               line.replace("<SAFETCURRENTACCOUNT/>",curr);
           }

           if ( line.indexOf("<TEMPLATEPATH/>")!= -1 )  {
               line.replace("<TEMPLATEPATH/>",templatePath());
           }
           if ( line.indexOf("<MODULESMENU/>")!= -1 )  {
               line.replace("<MODULESMENU/>",generateModulesMenu());
           }

           if ( line.indexOf("<SAFETRETURNCONSOLE/>")!= -1 )  {
               line.replace("<SAFETRETURNCONSOLE/>",returnFormLink(shortnameop));
           }

           html += line;
           html += "\n";

       }

       infile.close();

       return html;


}

bool MainWindow::checkAndExecEvents() {

    QString eventsdir = SafetYAWL::pathconf + "/" + "events" + "/";
    SYD << tr("Ejecutando eventos en \"%1\"")
           .arg(eventsdir);


    int count = 0;
    while (true)  {
        QString eventfile = eventsdir + QString("event%1").arg(count);
        SYD << tr("...MainWindow::checkAndExecEvents...EVALUANDO:\"%1\"")
               .arg(eventfile);
        if (!QFile::exists(eventfile) ) {
            SYD << tr("...MainWindow::checkAndExecEvents...saliendo en:\"%1\"")
                   .arg(eventfile);
            break;

        }
        QFile myfile(eventfile);
        bool open = myfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!open) {
            SYW << tr("...MainWindow::checkAndExecEvents...No se pudo leer el archivo:\"%1\"")
                   .arg(eventfile);
            break;
        }
        SYD << tr("...MainWindow::checkAndExecEvents...ABIERTO:\"%1\"")
               .arg(eventfile);

        QString flowname = myfile.readLine();
        flowname.chop(1);
        QString flowvar = myfile.readLine();
        flowvar.chop(1);
        QString flowaction = myfile.readLine();
        flowaction.chop(1);


        QStringList currvalues;
        int ndocs = numberOfDocuments(QString("{#safetdocumentid:%1:%2}")
                                      .arg(flowname).arg(flowvar),QString(""), currvalues);



        SYD << tr("...MainWindow::checkAndExecEvents..................*ndocs: |%1|")
               .arg(ndocs);
        SYD << tr("...MainWindow::checkAndExecEvents...saliendo flowname:\"%1\"")
               .arg(flowname);
        SYD << tr("...MainWindow::checkAndExecEvents...saliendo flowvar:\"%1\"")
               .arg(flowvar);

        SYD << tr("...MainWindow::checkAndExecEvents...saliendo flowaction:|%1|")
               .arg(flowaction);

        if ( ndocs > 0 ) { // Se cumple la condición

            for(int i = 0; i < ndocs; i++) {
                QString mycurrvalue = currvalues.at(i);
                SYD << tr("...MainWindow::checkAndExecEvents...CS_currvalue:|%1|")
                       .arg(mycurrvalue);

                QString myflowaction = flowaction.arg(mycurrvalue);
                SYD << tr("...MainWindow::checkAndExecEvents..........CONDICION_NDOCS.......*ndocs: |%1|")
                       .arg(ndocs);
                SYD << tr("...MainWindow::checkAndExecEvents...(after)...EJECUTANDO...:|%1|")
                       .arg(myflowaction);
                toInputForm(myflowaction,false);
                SYD << tr("...MainWindow::checkAndExecEvents...**EJECUTADA...:|%1|")
                       .arg(myflowaction);
            }

        }
        else {
            SYD << tr("...MainWindow::checkAndExecEvents..........CONDICION_NO_NDOCS.......*ndocs: |%1|")
                   .arg(ndocs);

        }

        count++;
    }


    SYD << tr("...Finalizando acceso al directorio de eventos!");

    return true;


}



QString MainWindow::returnFormLink(const QString& nameoperation) {
    QString result;
    result =  tr("<br/> <a href=\"%1\">Regresar a la consulta</b></a>")
            .arg(hostURL()+"/"+QString("defconsole:operacion:%1")
                 .arg(nameoperation));

     return result;

}

QString MainWindow::generateModulesMenu() {
    QString result;
    QString hurl = hostURL();
    result = QString(
//                "<div id=\"nav\">"
            "<ul class=\"sf-menu dropdown\">"
                "<li><a href=\"%1\">Inicio</a></li>"
                "<li><a href=\"%2\">Gestión</a></li>"
                "<li><a href=\"%3\">Consultas</a></li>"
                "<li><a href=\"%4\">Créditos</a></li>"
                "<li><a href=\"%5\">Cerrar Sesión</a></li>"
            "</ul>"
//        "</div>"
                )
            .arg(hurl+"/login")
            .arg(hurl+"/goinputform")
            .arg(hurl+"/goinputconsole")
            .arg(hurl+"/goabout")
            .arg(hurl+"/logout");


    return result;
}



QString MainWindow::menuCommands_old() {
    QString result;
    QString TEMPL  = QString("<a href=\"%2\">%1</a><br/>");
    QString TEMPLONLY  = QString("%1<br/>");
    QString TEMPLTITLE  = QString("<br/><h2>%1</h2></a>");

    QString TEMPLTITLEWITHICON = QString( ""
                                          "<table border=\"0\">"
                                          "<tr >"
                                          "<td width=\"70\"><img src=\"%1\" height=\"64\" width=\"64\" /></td>"
                                          "<td align=\"left\"><h2>%2</h2></td>"
                                          "</tr>"
                                          "</table> "

                                          );
    QString TEMPLSUBTITLE  = QString("<br/><h3 >%1</h3></a>");
    QString TEMPLSUBTITLEWITHICON = QString( ""
                                          "<table border=\"0\" width=\"50%\">"
                                          "<tr >"
                                          "<td width=\"18\">   </td>"
                                          "<td width=\"50\"><img src=\"%1\" height=\"48\" width=\"48\" /></td>"
                                          "<td align=\"left\"><h3>%2</h3></td>"
                                          "</tr>"
                                          "</table> "

                                          );

    DomModel* mymodel = new DomModel(inputPath(),NULL);
    Q_CHECK_PTR( mymodel );

    QString modulename = inputPath().section("/",-1).split(".").at(0);

    SYD << tr("....MainWindow::menuCommands()....................modulename:|%1|")
           .arg(modulename);

    SYD << tr("....MainWindow::menuCommands()....................inputPath():|%1|")
           .arg(inputPath());

    QMap<QString,bool> myperms;

    QStringList commands = mymodel->getCommands(false,true);

    SYD << tr("....MainWindow::menuCommands()....................commands.count():|%1|")
           .arg(commands.count());

    QMap<QString,QString> myactions = mymodel->currentActions();

    SYD << tr("....MainWindow::menuCommands()....................myactions.count():|%1|")
           .arg(myactions.count());

    QString currtitle;
    QString curroperations;
    QString currstitle;

    QMap<QString,QString> mypmap = loadPermisesHierarchy();
    foreach(QString k, mypmap.keys()) {
        SYD << tr("....MainWindow::menuCommands()...mymap[%1]=%2")
               .arg(k)
               .arg(mypmap[k]);
    }


    QString mydirmedia = SafetYAWL::getConf()["GeneralOptions/url.media"].trimmed();

    SYD << tr("............MainWindow::menuCommands.....url.media:|%1|")
           .arg(mydirmedia);


    QRegExp rx("icon:([a-zA-Z0-9\\._áéíóúñÑ]+)::");

    bool foundsubtitle = false;

    foreach(QString c, commands) {


        QString title = c;
        SYD << tr(".........MainWindow::menuCommands..................................\nTITLE:|%1|")
               .arg(title);

        if (title.startsWith(QLatin1String("operacion:titulo::"))) {
            if (!curroperations.isEmpty()) {
                SYD << tr(".........MainWindow::menuCommands......................curroperations...not empty!");

                if ( foundsubtitle ) {
                    curroperations.remove(currstitle);
                    foundsubtitle = false;
                }

                result += currtitle;
                SYD << tr("\n\n.....CURTITLE:...(1)...|%1|")
                       .arg(currtitle);
                result += curroperations;
//                SYD << tr(".....CURROPERATIONS...(1)...|%1|\n\n")
//                       .arg(curroperations);

                if (curroperations.isEmpty()) {
                    result.remove(currtitle);
                }

                curroperations = "";
            }
            currtitle = "";
            title = c.mid(QString("operacion:titulo::").length());
            QString icon;

//            SYD << tr("............MainWindow::menuCommands.....command:|%1|")
//                   .arg(c);
            SYD << tr(".........MainWindow::menuCommands..................................\nTITLE(1):|%1|")
                   .arg(title);

            int pos = title.indexOf(rx);
            if (pos >= 0 ) {
                icon = rx.cap(1);
                title = title.remove(rx);
            }
            SYD << tr(".........MainWindow::menuCommands..................................\nTITLE(2):|%1|")
                   .arg(title);
            SYD << tr(".........MainWindow::menuCommands..................................\nICON:|%1|")
                   .arg(icon);

            if (!icon.isEmpty()) {
                currtitle =  TEMPLTITLEWITHICON
                        .arg(mydirmedia + "/" + icon)
                        .arg(MainWindow::convertOpToTitle(title));

            }
            else {
                currtitle =  TEMPLTITLE
                        .arg(MainWindow::convertOpToTitle(title));
            }
            continue;
        }

        if (title.startsWith(QLatin1String("operacion:subtitulo::"))) {
            title = c.mid(QString("operacion:subtitulo::").length());

            if ( foundsubtitle ) {
                curroperations.remove(currstitle);
                foundsubtitle = false;
            }
            QString icon;

            int pos = title.indexOf(rx);
            if (pos >= 0 ) {
                icon = rx.cap(1);
                title = title.remove(rx);
            }

            if (!icon.isEmpty()) {
                currstitle =  TEMPLSUBTITLEWITHICON
                        .arg(mydirmedia + "/" + icon)
                        .arg(MainWindow::convertOpToTitle(title));

            }
            else {
                currstitle =  TEMPLSUBTITLE
                        .arg(MainWindow::convertOpToTitle(title));
            }

            foundsubtitle = true;
            curroperations += currstitle;
            continue;

        }

        myperms= doPermiseExecOperationAction(c);

        if (myperms.isEmpty()) {
            continue;
        }




        if (myperms.contains("execute") && myperms["execute"]) {
            QString myaction;
            QString mycount;
            if (myactions.contains(title)) {
//                SYD << tr("......MainWindow::menuCommands....ACTIONCOUNT.title:|%1|\naction:|%2|")
//                       .arg(title)
//                       .arg(myactions[title]);
                myaction = myactions[title];
                myaction.replace("_USERNAME", SafetYAWL::currentAuthUser());
                toInputConsole(myaction,false);


                mycount=  QString(" (%1) ").arg(currentCOUNT());
//                SYD << tr("......MainWindow::menuCommands....ACTIONCOUNT......mycount:|%1|")
//                       .arg(mycount);

            }


           foundsubtitle = false;
            curroperations += TEMPL
                    .arg(MainWindow::convertOpToTitle(title)+mycount)
                    .arg(hostURL()+"/"+modulename+":"+title.replace(QRegExp("_SAFET\\d\\d"),""));

        }
        else {

            if (myperms.contains("read") && myperms["read"]) {
                foundsubtitle = false;
                curroperations += TEMPLONLY
                        .arg(MainWindow::convertOpToTitle(title.replace(QRegExp("_SAFET\\d\\d"),"")));
            }
        }

    }

    // Fin de los comandos

    if (!curroperations.isEmpty()) {
        result += currtitle;
        SYD << tr("\n.....CURTITLE:...(2)...|%1|\n")
               .arg(currtitle);
        result += curroperations;
        SYD << tr(".....CURROPEATIONS:...(2)...|%1|\n")
               .arg(curroperations);

        curroperations = "";
    }


    return result;

}







QString MainWindow::menuCommands() {
    QString result = "{ ";



    QString newbody = " \"actions\": [";
    int ilennewbody = newbody.length();


    QString TEMPL  = QString("{ \"type\":\"%2\", \"href\":\"%3\", \"action_name\":\"%1\", \"icon\":\"%4\", \"template\":\"%5\" },");

    DomModel* mymodel = new DomModel(inputPath(),NULL);
    Q_CHECK_PTR( mymodel );

    QString modulename = inputPath().section("/",-1).split(".").at(0);

    SYD << tr("....MainWindow::menuCommands()......NEWCOMMAND..............modulename:|%1|")
           .arg(modulename);

    SYD << tr("....MainWindow::menuCommands()......NEWCOMMAND..............inputPath():|%1|")
           .arg(inputPath());

    QMap<QString,bool> myperms;

    QStringList commands = mymodel->getCommands(false,true);

    SYD << tr("....MainWindow::menuCommands()....................*commands.count():|%1|")
           .arg(commands.count());

    QMap<QString,QString> myactions = mymodel->currentActions();

    SYD << tr("....MainWindow::menuCommands()....................*myactions.count():|%1|")
           .arg(myactions.count());

    QString currtitle;
    QString curroperations;
    QString currstitle;

    QMap<QString,QString> mypmap = loadPermisesHierarchy();
    foreach(QString k, mypmap.keys()) {
        SYD << tr("....MainWindow::menuCommands()...mymap[%1]=%2")
               .arg(k)
               .arg(mypmap[k]);
    }


    QString mydirmedia = SafetYAWL::getConf()["GeneralOptions/url.media"].trimmed();

    SYD << tr("............MainWindow::menuCommands.....url.media:|%1|")
           .arg(mydirmedia);


    QRegExp rx("icon:([a-zA-Z0-9\\._áéíóúñÑ]+)::");

    bool foundsubtitle = false;

    foreach(QString c, commands) {

        QString title = c;

        SYD << tr(".......currtype...GETCOMMAND..NEWTITLE:|%1|")
               .arg(title);

        QString currtype = "menu";
        QString currtemplate = "";
        // ** found type
        QRegExp rxtype("type::(.+)::");

        if (rxtype.indexIn(title) >= 0 ) {
           currtype = rxtype.cap(1);
           SYD << tr(".......currtype...GETCOMMAND...currtype:|%1|")
                  .arg(currtype);
           title.replace(rxtype.cap(0),"");
        }



        QRegExp rxtemp("template::(.+)::");

        if (rxtemp.indexIn(title) >= 0 ) {
           currtemplate = rxtemp.cap(1);
           SYD << tr(".......currtype...GETCOMMAND...*currtemplate:|%1|")
                  .arg(currtemplate);
           title.replace(rxtemp.cap(0),"");
           SYD << tr(".......currtype...GETCOMMAND..*currtemplatetitle:|%1|")
                  .arg(title);
        }

        // ** found type

        SYD << tr(".........MainWindow::menuCommands................NEWCOMMAND..................\nTITLE:|%1|")
               .arg(title);

        if (title.startsWith(QLatin1String("operacion:titulo::"))) {
            if (!curroperations.isEmpty()) {
                SYD << tr(".........MainWindow::menuCommands...........NEWCOMMAND...........curroperations...not empty!");

                if ( foundsubtitle ) {
                    curroperations.remove(currstitle);
                    foundsubtitle = false;
                }

                newbody += "{ \"title\":" + currtitle;


                SYD << tr("\n\n.....CURTITLE:...(1)...|%1|")
                       .arg(currtitle);
                curroperations.chop(1);
                newbody += " \"items\": [ " + curroperations + "] },";
//                SYD << tr(".....CURROPERATIONS...(1)...|%1|\n\n")
//                       .arg(curroperations);

                if (curroperations.isEmpty()) {
                   newbody.remove(currtitle);
                }

                curroperations = "";
            }
            currtitle = "";
            title = c.mid(QString("operacion:titulo::").length());
            QString icon;

            int pos = title.indexOf(rx);
            if (pos >= 0 ) {
                icon = rx.cap(1);
                title = title.remove(rx);
            }
            SYD << tr(".........MainWindow::menuCommands..................................\nTITLE(2):|%1|")
                   .arg(title);
            SYD << tr(".........MainWindow::menuCommands..................................\nICON:|%1|")
                   .arg(icon);

            if (!icon.isEmpty()) {
                currtitle =  TEMPL
                        .arg(MainWindow::convertOpToTitle(title))
                        .arg("title")
                        .arg("")
                        .arg(mydirmedia + "/" + icon)
                        .arg(currtemplate);


            } else {
                currtitle =  TEMPL
                        .arg(MainWindow::convertOpToTitle(title))
                        .arg("title")
                        .arg("")
                        .arg("")
                        .arg(currtemplate);
            }
            continue;
        }

        if (title.startsWith(QLatin1String("operacion:subtitulo::"))) {
            title = c.mid(QString("operacion:subtitulo::").length());

            if ( foundsubtitle ) {
                curroperations.remove(currstitle);
                foundsubtitle = false;
            }
            QString icon;

            int pos = title.indexOf(rx);
            if (pos >= 0 ) {
                icon = rx.cap(1);
                title = title.remove(rx);
            }

            if (!icon.isEmpty()) {
                currstitle =  TEMPL
                        .arg(MainWindow::convertOpToTitle(title))
                        .arg("subtitle")
                        .arg("")
                        .arg(mydirmedia + "/" + icon)
                        .arg(currtemplate);
                newbody += TEMPL
                        .arg(MainWindow::convertOpToTitle(title))
                        .arg("subtitle")
                        .arg("")
                        .arg(mydirmedia + "/" + icon)
                        .arg(currtemplate);


            }
            else {
                currstitle =  TEMPL
                        .arg(MainWindow::convertOpToTitle(title))
                        .arg("subtitle")
                        .arg("")
                        .arg("")
                        .arg(currtemplate);

                newbody += TEMPL
                        .arg(MainWindow::convertOpToTitle(title))
                        .arg("subtitle")
                        .arg("")
                        .arg("")
                        .arg(currtemplate);
            }
            foundsubtitle = true;
            curroperations += "{ \"title\":" + currstitle;
            continue;

        }

        //myperms= MainWindow::doPermiseExecOperationAction(c);
        myperms= doPermiseExecOperationAction(title);

        if (currtype == "ws") {
            continue;
        }

        if (myperms.isEmpty()) {
            continue;
        }



        if (myperms.contains("execute") && myperms["execute"]) {
            QString myaction;
            QString mycount;
            if (myactions.contains(title)) {
                myaction = myactions[title];
                myaction.replace("_USERNAME", SafetYAWL::currentAuthUser());
                toInputConsole(myaction,false);


                mycount=  QString(" (%1) ").arg(currentCOUNT());

            }


           foundsubtitle = false;

              curroperations += TEMPL.arg(MainWindow::convertOpToTitle(title)+mycount)
                    .arg("action")
                    .arg("/"+title.replace(QRegExp("_SAFET\\d\\d"),""))
                    .arg("")
                      .arg(currtemplate);


        }
        else {

            if (myperms.contains("read") && myperms["read"]) {
                foundsubtitle = false;
                curroperations += TEMPL
                        .arg(MainWindow::convertOpToTitle(title.replace(QRegExp("_SAFET\\d\\d"),"")))
                        .arg("action")
                        .arg("")
                        .arg("")
                        .arg(currtemplate);
            }
        }

     }


    // Fin de los comandos
    if (!curroperations.isEmpty()) {
        newbody += "{ \"title\":" + currtitle;
        curroperations.chop(1);
        newbody += ", \"items\": [ " + curroperations + "] },";
        curroperations = "";
    }

    if ( newbody.length() > ilennewbody ) {
            newbody.chop(1);
    }

    result += newbody;


    result += "] }";

    SYD << tr("......menuCommands()........MENU_JSON:|%1|")
           .arg(result);

    return result;
}




QString MainWindow::convertOpToTitle(const QString& op) {
    QString result;
    if ( op.isEmpty()) {
        return op;
    }

    result = op;
    result.replace("operacion:","");
    result.replace("_"," ");
    QStringList mylist = result.split(QRegExp("\\b"),QString::SkipEmptyParts);

    if (mylist.isEmpty()) {
        return op;
    }

    bool isupper = true;
    QString myword = mylist.at(0);
    for (int j=0; j < myword.length();j++) {
        if (!myword.at(j).isUpper()) {
            isupper = false;
            break;
        }
    }
    if ( !isupper ) {
        result = myword.mid(0,1).toUpper()+myword.mid(1).toLower();
    }
    else {
        result = myword;
    }
    for(int i=1; i < mylist.count();i++) {
        QString myword = mylist.at(i);
        bool isupper = false;
        for (int j=0; j < myword.length();j++) {
            if (myword.at(j).isUpper()) {
                isupper = true;
                break;
            }
        }
        if ( !isupper ) {
           myword =  myword.toLower();
        }
        result += " ";
        result += myword;
    }


    result.replace(QRegExp("\\s+")," ");

    return result;
}

void MainWindow::registerLogin(const QString& user) {
    qDebug("........MainWindow::registerLogin: |%s|", qPrintable(user));
    SYA << tr("LOGIN:El usuario \"%1\" ha ingresado al sistema SAFET")
           .arg(user);
}

void MainWindow::registerLogout(const QString& user) {

    MainWindow::currentaccount = "";
    MainWindow::currentrole = "";

    SYA << tr("LOGOUT:El usuario \"%1\" ha salido del sistema SAFET")
           .arg(user);

}

void MainWindow::log(const QString& message) {

    SYA << message;
}


QString MainWindow::generateFormHead(const QString& o) {

    QStringList actionslist = o.split(Safet::SEPARATOR,QString::SkipEmptyParts);
    Q_ASSERT(actionslist.count()  > 0);

    QString newo =  actionslist.at(0);
    QString purl = newo.section("/",-1);


    QStringList purls = purl.split(":");
    if (purls.count() < 3) {
        SYE
                   <<  tr("Al obtener el formulario la operación \"%1\" es Inválida")
                      .arg(purl);
        return QLatin1String("");
    }
    QString modname = purls.at(0);
    int npos = purl.indexOf(":");
    purl = purl.mid(npos+1);
    DomModel* mymodel = new DomModel(inputPath(),NULL);
    Q_CHECK_PTR( mymodel );

    //QString myflowname = "/home/vbravo/.safet/flowfiles/flujogeneralPorHitoYPropietario.xml";
    SYD << tr("generateFormHead: purl: %1")
           .arg(newo);
    QStringList  keymodifyfields = mymodel->fieldsWithAttr(purl);
    SYD << tr("generateFormHead: operacion: %1")
           .arg(newo);

    QString result = "";

    result = result + QString("\n");

/*      result += QString("\n"
        "<script src=\"%1/jquery-latest.js\"></script>\n"
            "<link type=\"text/css\" href=\"%1/css/ui-lightness/jquery-ui-1.8.8.custom.css\" rel=\"Stylesheet\" />\n"
            "<script src=\"%1/js/jquery.ui.custom.js\"></script>\n"
            "<script src=\"%1/jquery.jec-1.3.1.js\"></script>\n"
            "<script src=\"%1/development-bundle/ui/i18n/jquery.ui.datepicker-es.js\"></script>\n"
           "<script type=\"text/javascript\" language=\"javascript\" src=\"../media/js/DataTables/media/js/jquery.dataTables.js\"></script>\n"
                     //*** Con Jqwidgets
            "    <link rel=\"stylesheet\" href=\"../media/jqwidgets/jqwidgets/styles/jqx.base.css\" type=\"text/css\" />\n"
          "    <script type=\"text/javascript\" src=\"../media/jqwidgets/scripts/gettheme.js\"></script>\n"
            "    <script type=\"text/javascript\" src=\"../media/jqwidgets/jqwidgets/jqxcore.js\"></script>\n"
            "    <script type=\"text/javascript\" src=\"../media/jqwidgets/jqwidgets/jqxnumberinput.js\"></script>\n"
            "    <script type=\"text/javascript\" src=\"../media/jqwidgets/jqwidgets/jqxbuttons.js\"></script>\n"
)
            .arg(hostMediaPath());
            .arg("../media");

*/

    QString myfirsfield = "";
    if (keymodifyfields.count() > 0 ) {
    result += QString(
            ""		
	    "\n" 	
            "<script>\n"
            "\n\n function genFormString(f) {\n"
                     "var result= \"\";\n"
                     "   for(i=0; i<f.length;i++){\n"
                     "     result += f.elements[i].id+\":\"+ f.elements[i].value+\"\\n\";\n"
                     "   }\n"
                     "return result;\n"
           " }\n"
            );
            myfirsfield = keymodifyfields.at(0);
        }

   QString firstkeymodifyfield;
    if (keymodifyfields.count() > 0 ) {
	    result += QString("var safetCurrentCombos = new Array();\n");
        result += QString("var CKOBJ = null;\n");
        result += QString("var CKVALUE = \"\";\n");
	    result += QString("$(document).ready(function(){\n");
    }

    int keyscount = 0;
    foreach(QString keymodifyfield, keymodifyfields) {
        if ( firstkeymodifyfield.isEmpty()) {
            if ( keymodifyfield.startsWith(QLatin1String("::literal:"))) {
                  firstkeymodifyfield = QString("\"%1\"").arg(keymodifyfield
                                                              .mid(QString("::literal:").length()));
            }
            else if ( keymodifyfield.startsWith(QLatin1String("::enablethis:"))) {
                  QStringList mylist = keymodifyfield
                          .mid(QString("::enablethis:").length()).split(",",QString::SkipEmptyParts);

                  QStringList mypastlist;
                  foreach(QString s, mylist) {
                      QString namef = s.section(":",0,0);
                      if ( !mypastlist.contains(namef)) {
                        //QString myenable = QString("$('#%1').attr('disabled', true);\n").arg(namef);
                          QString myenable = QString("$('#%1').hide();\n").arg(namef);
                        result += myenable;
                        result += QString("safetCurrentCombos.push('%1');\n").arg(namef);
                        mypastlist.append(namef);
                      }
                  }

                  continue;
            }
            else {
                    firstkeymodifyfield = QString("$(\"#%1\").val()").arg(keymodifyfield);
            }
            SYD << tr("generateFormHead....firstkeymodifyfield:|%1|")
                   .arg(firstkeymodifyfield);

        }
        SYD << tr("generateFormHead: ....keymodifyfield: |%1|")
               .arg(keymodifyfield);

        QString mydirmedia = SafetYAWL::getConf()["GeneralOptions/dir.media"];

        QString myid = keymodifyfield;
        myid.replace(" ","_");
	 QString myFunctionAfter = "";
        if (keyscount == 0) {
         myFunctionAfter = QString("\nif (typeof safetProcessAfter == 'function') { safetProcessAfter(); }\n");
        }
        else if (keyscount > 0) {

            myFunctionAfter = QString("\nif (typeof safetProcessAfter%1 == 'function') { safetProcessAfter%1(); }\n")
                    .arg(keyscount);
        }

         QString newresult =  QString(""

                          "$(\"#%2\").change(\n"
                          "function() "
                          "{\n"
                          "   $(\"#divForLoading\").show();\n"
                          "    $.post(\"%1/loaddata\",{ id:$(this).val(),\n"
                          "    op:document.getElementById(\"safetoperation\").value,\n"
                          "    modname:\"%3\",\n"
                          "    primary:this.id,\n"
                          "    formstring: genFormString(document.forms[0]),\n"
                          "   formkey: document.forms[0].elements[1].value},\n"
                          " function(data)"
                          " {\n"
                          "  console.log(\"*data: \"+data);\n"
                          "  mylist = data.split(\"<SAFETSEPARATOR/>\");\n"
                          "  htmlsep = false;\n"
                          "  if (mylist.length < 2) {\n"
                          "    mylist = data.split(\"<SAFETHTMLSEPARATOR/>\");\n"
                          "    htmlsep = true;\n "
                          "  }\n"
                          "  for(i=0; i < mylist.length;i++){\n"
                          "       myname  = \"\";\n"
                          "       myvalue = \"\"; \n"
                          "       myname = mylist[i].substr(0,mylist[i].indexOf(\":\"));\n"
                          "       console.log(\"****************MYNAME:\" + myname);\n"

                          "       if (myname.length == 0 ) {\n"
                          "               continue;\n"
                          "       }\n"
                          "       myvalue = mylist[i].substr(mylist[i].indexOf(\":\")+1);\n"
                          "       console.log(\"myvalue(*):\" + myvalue);\n"

                          "       j= 0;\n"
                          "       for(j=0; j<myname.length;j++){\n"
                          "               if ( (myname.charCodeAt(j)!=32) && (myname.charCodeAt(j)!=13) && (myname.charCodeAt(j)!= 10) ) {\n"
                          "                       break;\n"
                          "               }\n"
                          "       }\n"
                          "       myname = myname.substr(j);\n"
                          "       console.log(\"myname(j):\" + myname);\n"

                          "       j=myvalue.length-1;\n"
                          "       lastpos = myvalue.length;\n"
                          "       for(j=myvalue.length-1;j>0;j--){\n"
                          "               if (myvalue.charAt(j) != ' ') {\n"
                          "                       break;\n"
                          "               }\n"
                          "               lastpos = lastpos -1;\n"
                          "       }\n"
                          "       myvalue = myvalue.substr(0,lastpos);\n"
                          "       console.log(\"*myvalue:\" + myvalue);\n"
                          "       if (myvalue.indexOf(\"%4\") != -1)  {\n"
                          "          mynewpreview = \"vistaPrevia\" + myname; \n"
                          "          console.log(\"preview:\" + mynewpreview);  \n    "
                          "          $(\"#\" + mynewpreview).attr(\"src\",myvalue);\n"
                          "         \n"
                          "       } else if (myname == \"Mostrar_tabla\") {\n"
                                      "safetjson.tasks = eval(myvalue);\n"
                                      "  oTable.fnDestroy();\n"
                                      "safetproccessData();\n"
                          "       } else if (document.getElementById(myname)) {\n"
                          "          if (htmlsep == true ) {\n"
                          "               $(\"#\"+myname).html(myvalue);\n"
                          "               if ( $(\"#\"+myname).is(\"input\") ) {\n "
                          "                 document.getElementById(myname).value = myvalue; \n   "

                          "               }                                     \n "
                          "               else {                                 \n"
                          "                  console.log(\"PUTTING myname:\"+myname); \n"
                          "                  console.log(\"PUTTING myvalue:\"+myvalue); \n"
                          "                 $(\"#\"+myname).html(myvalue);\n     \n"
                          "               }                                      \n"
                          "          } else {"
                          "                  document.getElementById(myname).value = myvalue;\n"
                          "          }\n"
                          "      } else {\n"
                                "}\n"
                          "   if (myvalue == \"true\" ) {\n"
                                "  console.log(\"MyName is true:\"+myname); \n"
                              "   if (document.getElementById(myname+\"_1\") != null ) {\n"
                              "             document.getElementById(myname+\"_1\").checked = true;"
                                  "}\n"
                                "}\n"
                          "   if (document.getElementById(myname) == null ) {\n"
                          "      myckname = myname +'_ckeditor';\n"
                          "       myobj =  document.getElementById(myckname);\n"
                          "         if ( myobj != null ) {\n"
                          "            console.log(\"ckeditor:\" + myckname );\n                "

                          "             mystr =  \"CKEDITOR.instances.\" + myckname; \n"
                          "             myckobj =  eval(mystr); \n"
                          "             console.log(\"myckobj:\" + myckobj);\n          "
                          //"             myvalue = myvalue.replace(/&#39;/g,\"'\");           "
                          "             CKOBJ = myckobj;\n                       "
                          "             CKVALUE = myvalue;\n                       "
                          "             myckobj.setData(myvalue);     \n"
                          "             console.log(\"ckeditor...setup...myvalue:\" + myvalue );\n                "
                          "          } else {"
                          "              console.log(\"MyName is null:\"+myname); \n"
                          "              mynewname = myname+\"_\"+myvalue; \n"
                          "              console.log(\"mynewname is null:\"+mynewname); \n"
                          "              if (document.getElementById(mynewname) != null ) {\n"
                          "                     document.getElementById(mynewname).checked = true; \n"
                                         "}\n"
                          "          }\n"
                          "   }\n"

                          "}\n"
                                      "%5"
                          "   $(\"#divForLoading\").hide();\n"
                          "     console.log(\"saliendo...\");\n"

                          " });\n"

                          "});\n")
                 .arg(hostURL())
                .arg(keymodifyfield)
                 .arg(modname)
                 .arg(mydirmedia)
		 .arg(myFunctionAfter);
        //         .arg(keyscount == 0?"\nif (typeof safetProcessAfter == 'function') { safetProcessAfter(); }\n":"");





         result += newresult;
         keyscount++;

    }


    if (!firstkeymodifyfield.isEmpty()) {
/*
        result +=  QString("$( \"#parsdialog\" ).dialog({\n"
                           "  autoOpen: false,\n"
                           "   height: 400,\n"
                           "  width: 510,\n"
                           "  modal: true,\n"
                           "  buttons: {\n"
                           "     \"Enviar\": function() {\n"
                           "       var newFields = document.getElementById('safetfieldset');\n"
                           "       var insertFormHere = document.getElementById('safetmarkup');\n"
                           "       insertFormHere.parentNode.insertBefore(newFields.cloneNode(true),insertFormHere);\n"
                           "       var copyFields = newFields;"
                           "       copyFields.style.display = 'none';\n"
                           "       var insertHere = document.getElementById('safetspan');\n"
                           "       insertHere.parentNode.insertBefore(copyFields,insertHere);\n"
                           "       $( this ).dialog( \"close\" );\n"
                           "       }\n"
                           "  },\n"
                           "  close: function() {\n"
                           "                 $( this ).dialog( \"close\" );\n"
                              "}\n"
                           "});\n"
                           "$( \"#parsbutton\" ).button().click(function() {\n"
                           "   $.post(\"%1/loadpars\",\n"
                           "          {op:document.getElementById(\"safetoperation\").value,\n"
                           "          modname:\"%3\",\n"
                           "         primary: %2 },\n"
                           "         function(data){\n"
                           "            if (data==\"SafetErrorFaltanDatos\") {\n"
                           "                  alert(\"Debe Seleccionar un opción de flujo de trabajo \\npara ver el Cuadro de Parámetros\");\n"
                           "                  return;\n"
                           "            }\n               "
                           "            document.getElementById(\"safetfieldset\").innerHTML = data;\n"
                           "            $( \"#parsdialog\" ).dialog( \"open\" );\n"
                           "         });\n"
                           "  });\n"
                           "$( \"#safetsubmit\" ).button().click( function() {\n"
                           "                 document.getElementById(\"safettable\").prepend(document.getElementById(\"safetfieldset\").innerHTML);\n"
                           "                 alert(document.getElementById(\"safetform\").innerHTML);"
                           "                 $( \"#safeform\" ).submit();\n"
                           "});\n"
                           "$( \"#safetcancel\" ).button().click(function() {\n"
                           "        $( \"#safeform\" ).reset();\n"s
                           "});\n"
                          )
                .arg(hostURL())
                .arg(firstkeymodifyfield)
                .arg(modname);
                */
    }



    if (keymodifyfields.count() > 0 ) {        
    	result += "});\n\n";	        
        result += "\n"

            "</script>\n"
//            "</head>\n"
//            "<body>\n"
            "";
	}
	else {
		result += "\n"
			"<script>\n"
        "$(document).ready(function(){\n"
		"$( \"#safetsubmit\" ).button().click(function() {});\n"
		"$( \"#safetcancel\" ).button().click(function() {});\n"

		"});\n"
		"</script>\n";
	}

    if (actionslist.count() > 1 ) {
        result += QString("<script type=\"text/javascript\">\n");
        result += QString("$(document).ready(function(){\n");

        QString myvalue = actionslist.at(1);
                SYD << tr("........generateFormHead...**firstvalue");

                result += QString("\n"
                        " myval = $('#%2 option%1').val();\n"
                        " console.log('myval:' + myval);\n"
                        " $('#id').val(myval).change();\n"
                        " console.log('myval:' + myval);\n"
                          " console.log('CKOBJ:' + CKOBJ);\n"
                          " console.log('CKVALUE:' + CKVALUE);\n"
                         "setTimeout(function(){ "
                         " if (CKOBJ != null) CKOBJ.setData(CKVALUE); }, 1000); \n"
                        )
                        .arg(myvalue)
                        .arg(myfirsfield);
                result += "});\n\n";
                result += QString("</script>\n");
    }

    return result;
}

QString MainWindow::generateModifyHTML(const QString& operation, const QString& fieldname,
                                       const QString& key, const QString& secondkey,const QStringList& form) {
    QString result;
    SYD << tr("MainWindow::generateModifyHTML....operation:|%1|..."
              "fieldname:|%2|....key:|%3|")
           .arg(operation)
           .arg(fieldname)
           .arg(key);


    DomModel* mymodel = new DomModel(inputPath(),NULL);
    Q_CHECK_PTR( mymodel );
    QStringList mylist;
    SYD
               << tr("MainWindow::generateModifyHTML...operation: |%1|..fieldname:|%2|...key:|%3|")
                  .arg(operation)
                  .arg(fieldname)
                  .arg(key);


    if (operation.startsWith(QLatin1String("Listar_"))
            || operation.endsWith(QLatin1String("Siguiente_estado"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Siguiente_estado"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Firmar_"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Registrar_completación_de_firmas_"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Cambiar_rol"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Cambiar_nota"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Cambiar_info"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Generar_solicitud"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Oficina_"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Asignar_"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Confirmar_"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Registrar_aprobación_"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Registrar_validación_"),Qt::CaseInsensitive)
            || operation.startsWith(QLatin1String("Agregar_planificación"),Qt::CaseInsensitive)
             ||   operation.endsWith(QLatin1String("Generar_gráfico_coloreado"),Qt::CaseInsensitive)
             ||   operation.endsWith(QLatin1String("Generar_grafico_coloreado"),Qt::CaseInsensitive)
             ||   operation.endsWith(QLatin1String("con_autofiltro"),Qt::CaseInsensitive)
             ||   operation.endsWith(QLatin1String("con_filtrorecursivo"),Qt::CaseInsensitive)
        ){

        SYD << tr("........MainWindow::generateModifyHTML.....(***)....enter list options->");
        if (fieldname == "Periodo_vacacional")  {
            result = mymodel->getUpdateString(operation,fieldname,key,mylist);
            SYD << tr("..........(Periodo_vacacional)...generateModifyHTML: |%1|")
               .arg(result);
        }
        else if (fieldname == "Correspondencia")  {

            result = mymodel->getUpdateString(operation,fieldname,key,mylist);
            SYD << tr("..........(Correspondencia)...generateModifyHTML: |%1|")
               .arg(result);
        }

        else {
            result = QString("%1")
                .arg(formFieldsForKey(operation,fieldname, key,mymodel,secondkey,form));
        }

                   SYD << tr("(formfieldkey)...MainWindow::generateModifyHTML...result:|%1|")
                      .arg(result);
    }
    else if (  (operation == "modificar_usuario" && fieldname == "Estado")
               || (operation == "modificar_perfil" && fieldname == "Estado")
               || (operation == "modificar_usuario" && (fieldname == "País" || fieldname == "Pais"))
               || (operation == "modificar_perfil" && (fieldname == "País" || fieldname == "Pais"))
               || operation == "agregar_publicación"
               || (operation == "modificar_publicación" && fieldname == "Categoría")
               || (operation == "modificar_publicación" && fieldname == "SubCategoria")
               || (operation == "modificar_publicación" && fieldname == "SubCategoría")) {

        SYD << tr("......MainWindow::generateModifyHTML.***EVALUATING...ADD_PUBLICATION");

        SYD << tr("......MainWindow::generateModifyHTML.(sub)...EVALUATING: ....KEY:|%1|")
               .arg(key);
        SYD << tr("......MainWindow::generateModifyHTML.EVALUATING: ....SECONDKEY:|%1|")
               .arg(secondkey);
        foreach(QString f, form) {
            SYD << tr("......MainWindow::generateModifyHTML.EVALUATING: ....f:|%1|")
                   .arg(f);
        }

        result = QString("%1")
            .arg(formFieldsForKey(operation,fieldname, key,mymodel,secondkey,form));


        SYD << tr("......MainWindow::generateModifyHTML.EVALUATING: result:|%1|")
               .arg(result);

         return result;
    }
    else if (operation.startsWith(tr("agregar_ticket") ) ) {
        SYD << tr("......MainWindow::generateModifyHTML....agregar_ticket....key:|%1|")
               .arg(key);
        QString pathflow = key;
        //pathflow = pathflow.section(SafetYAWL::LISTSEPARATORCHARACTER,1,1);
        SYD << tr("......**MainWindow::generateModifyHTML....pathflow:|%1|")
               .arg(pathflow);
        result = QString("%1")
                .arg(formFieldsForKey(operation,fieldname, pathflow,mymodel));
         SYD << tr("MainWindow::generateModifyHTML...result:|%1|")
                      .arg(result);
    }
    else if ( operation.startsWith(tr("Agregar_nodo"))
              || operation.startsWith(tr("Borrar_nodo"))
               || operation.startsWith(tr("Inicializar_"))
//              || operation.startsWith(tr("Cambiar_"))
              || operation.startsWith(tr("Cambiar_conexión")) ) {
        QString pathflow = key;
        SYD << tr("......MainWindow::generateModifyHTML....pathflow(key,agregar):|%1|")
               .arg(pathflow);
        result = QString("%1")
                .arg(formFieldsForKey(operation,fieldname, pathflow,mymodel));
      SYD << tr("MainWindow::generateModifyHTML...result(agregar):|%1|")
                      .arg(result);
    }
    else if ( operation.startsWith(tr("Cambiar_fecha"))) {

        doRestoreGraph(Safet::PLANNEDGRAPHSFILENAME);
        SYD << tr("......MainWindow::generateModifyHTML....***Cambiar_fecha.....key:|%1|")
               .arg(key);
        if (!graphs().contains(key)) {
            SYW << tr("No se encuentra en el repositorio el archivo de grafos planificados");
            return result;
        }
        SYD << tr("......MainWindow::generateModifyHTML....****graphs()[key].second......:|%1|")
               .arg(graphs()[key].second);

        QString pathflow = graphs()[key].second;
        SYD << tr("......MainWindow::generateModifyHTML....****pathflow....(1)......:|%1|")
               .arg(pathflow);
        pathflow = pathflow.section(SafetYAWL::LISTSEPARATORCHARACTER,1,1);

        SYD << tr("......MainWindow::generateModifyHTML....real...pathflow.....(2)........:|%1|")
               .arg(pathflow);


        result = QString("%1")
                .arg(formFieldsForKey(operation,fieldname, pathflow,mymodel));
       SYD
                   << tr("MainWindow::generateModifyHTML...result:|%1|")
                      .arg(result);

    }
    else if (operation.startsWith("agregar_adjunto_a_convocatoria")) {
        result = QString("%1")
            .arg(formFieldsForKey(operation,fieldname, key,mymodel,secondkey,form));

       SYD
                  << tr("MainWindow::generateModifyHTML...agregar_adjunto_a_convocatoria...result:|%1|")
                      .arg(result);




    }
    else  {
        result = mymodel->getUpdateString(operation,fieldname,key,mylist);

                   SYD << tr("........getUpdateString..MainWindow::generateModifyHTML: ->|%1|")
                      .arg(result);
    }

    return result;
}

QString MainWindow::generateFormFooter(const QString& o) {
    QString purl = o.section("/",-1);
    if (purl.split(":").count() < 2) {
        SYE                   <<  tr("Al obtener el formulario la operación \"%1\" es Inválida")
                      .arg(purl);
        return QLatin1String("");
    }

      QString result = "";
//    QString result = "</body>\n"
//    "</html>";
    return result;
}

QString MainWindow::formFieldsForKey(const QString& o, const QString& fieldname,
                                     const QString& keyvalue,
                                     DomModel* mymodel, const QString& otherkey,
                                     const QStringList& form) {
    QString result;
    QString purl = o.section("/",-1);
    QString cmd;
    if (purl.split(":").count() > 1 ) {
        cmd = purl.split(":").at(1).trimmed();
    }
    else {
        cmd = purl;
    }


    Q_CHECK_PTR( mymodel );



    QStringList fields = mymodel->getFields(cmd);
               SYD << tr("MainWindow::formFieldsForKey....fields.count():%1")
                  .arg(fields.count());

    foreach(QString f, fields) {
        QString newfield = f;
        QString trimfield = f;
        bool mandatory = false;

        if (newfield.endsWith("*")) {
            newfield.chop(1);
            trimfield = newfield.trimmed();
            mandatory = true;
        }
        if ( fieldname == trimfield) {
            continue;
        }
        SYD << tr("\n\n\n*******\n............MainWindow::formFieldsForKey....trimfield:%1")
                      .arg(trimfield);

        CmdWidget* mywidget = mymodel->selWidgetFromField(cmd,trimfield,"",form);

        QString newitem;
        SYD << tr("............**MainWindow::formFieldsForKey............(0)...");
        if (mywidget == NULL ) {
            if (trimfield.isEmpty()) {
                SYW  << tr("El nombre del campo está vacio (buscando información sobre claves)");
            }
            else {
                SYW  << tr("No hay widget para \"%1\"").arg(trimfield);
            }
            //newitem = tr("No hay widget para \"%1\"\n").arg(trimfield);
        }
        else {
             SYD << tr("...........***MainWindow::formFieldsForKey.......CHANGEFOR.....(1)...");
            if (!mywidget->conf().contains("changefor")) {
                continue;
            }
            SYD << tr("...........***MainWindow::formFieldsForKey.......CHANGEFOR.....(2)...keyvalue:|%1|")
                   .arg(keyvalue);
            SYD << tr("...........***MainWindow::formFieldsForKey.......CHANGEFOR.....(3)...otherkey:|%1|")
                   .arg(otherkey);
            SYD << tr("MainWindow::formFieldsForKey: |%1|")
                   .arg(fieldname);
            if ( !mywidget->conf()["changefor"].toString().split(",",QString::SkipEmptyParts).
                    contains(fieldname)) {
                continue;
            }
            newitem = QString("%3: %1%2\n")
                  .arg(mywidget->htmlForKey(keyvalue, otherkey))
                   .arg(SafetYAWL::LISTHTMLSEPARATOR)
                   .arg(trimfield);
                SYD << tr("MainWindow::formFieldsForKey....CHANGEFOR....newitem:|%1|")
                          .arg(newitem);


        }
          result += newitem;


    }
    if (result.count() > SafetYAWL::LISTSEPARATORCHARACTER.count()) {
        //result.chop(SafetYAWL::LISTSEPARATORCHARACTER.count());
    }
    return result;

}

QString MainWindow::menuForm(const QString& o, bool forwidget, const QString& fieldlist) {
    SYD << tr(".................MainWindow::menuForm......MENUFORM(1).......o:|%1|")
           .arg(o);
    SYD << tr(".................MainWindow::menuForm......MENUFORM(1).......fieldlist:|%1|")
           .arg(fieldlist);

    SYD << tr(".................MainWindow::menuForm......MENUFORM(1).......forwidget:|%1|")
           .arg(forwidget);
    QString result;
    QString purl = o;
    purl = purl.section("/",-1);
    QStringList purls = purl.split(":");
    if ( purls.count() < 3) {
       SYE        <<  tr("Al obtener el formulario la operación \"%1\" es Inválida")
                      .arg(purl);
        return QLatin1String("");
    }
    int npos = purl.indexOf(":");
    purl = purl.mid(npos+1);
    QString formurl = purl;

    if ( purls.at(0) == "deftrac" ) {
        formurl.replace("operacion:","forma_");
    }
    else if (purls.at(0) == "defconsole" ) {
        formurl.replace("operacion:","consola_");
    }

    formurl  = formurl + "{{SAFETCURRID}}";
    SYD << tr("MainWindow::menuForm...menuForm....formurl:|%1|")
                  .arg(formurl);


    SYD << tr("(1)...MainWindow::menuForm...menuForm....SafetYAWL::pathconf:|%1|")
           .arg(SafetYAWL::pathconf);
        result = QString("<form action=\"%1\" id=\"safetform\" name=\"safetform\" "
                   "class=\"signin\" method=\"post\"  enctype=\"multipart/form-data\">\n")
                .arg(hostURL() + "/" + formurl);

    SYD << tr("(1)...MainWindow::menuForm...menuForm....SafetYAWL::inputPath():|%1|")
           .arg(inputPath());

    DomModel* mymodel = NULL;

    QMap<QString,QString> defaultfields;
    if (forwidget ) {
        result = QString("<form action=\"%1\" id=\"safetwidgetform\" name=\"safetwidgetform\" "
                   "class=\"signin\" method=\"post\"  enctype=\"multipart/form-data\">")
                .arg(hostURL() + "/" + formurl);

        QString cmd = purl.split(":").at(1).trimmed();
        result += QString("<h3>%1</h3>")
                .arg(convertOpToTitle(cmd).replace(QRegExp("_SAFET\\d\\d"),""));

        result += QString("<br/><input name=\"safetoperation\""
                         " id=\"safetoperation\" "
                          "type=\"hidden\" value=\"%1\"/><br/>")
                .arg(cmd);

        foreach(QString field, fieldlist.split(";",QString::SkipEmptyParts)) {
            int pos = field.indexOf("::");
            if (pos > 0 ) {
                QString namefield = field.mid(0,pos);
                QString value = field.mid(namefield.length()+QString("::").length());
                defaultfields[namefield] = value;

                SYD << tr("..............MainWindow::menuForm...DEFAULTFIELDS..name:|%1|->|%2|")
                       .arg(namefield)
                       .arg(value);

                result += QString("<input type=\"text\" name=\"%1\" id=\"%1\" value=\"%2\" />")
                        .arg(namefield)
                        .arg(value);
            }


        }
        result += QString("</form>");

        return result;
    }

    SYD << tr("..............MainWindow::menuForm...defaultfields.count():|%1|")
           .arg(defaultfields.count());

    mymodel = new DomModel(inputPath(),NULL);

    Q_CHECK_PTR( mymodel );

    QString cmd = purl.split(":").at(1).trimmed();
    result += QString("<h3>%1</h3>")
            .arg(convertOpToTitle(cmd.replace(QRegExp("_SAFET\\d\\d"),"")));

    result += QString("<br/><input name=\"safetoperation\""
                     " id=\"safetoperation\" "
                      "type=\"hidden\" value=\"%1\"/>\n<br/>")
            .arg(cmd);

//    result += "<table id=\"safettable\" name=\"safettable\" style=\"\">\n";



    SYD << tr("(1)...MainWindow::menuForm...menuForm....cmd:|%1|")
           .arg(cmd);

    QStringList fields = mymodel->getFields(cmd);
    SYD << tr("(1)...MainWindow::menuForm...menuForm....fields.count():|%1|")
           .arg(fields.count());

    foreach(QString f, fields) {
        QString newfield = f;
        QString trimfield = f;
        bool mandatory = false;

        if (newfield.endsWith("*")) {
            newfield.chop(1);
            trimfield = newfield.trimmed();
            mandatory = true;
        }

        SYD << tr("...MainWindow:::menuForm...mywidget ....cmd:|%1|")
               .arg(cmd);
        SYD << tr("...MainWindow:::menuForm...mywidget ....(1)*TRIMFIELD...trimfield:|%1|")
               .arg(trimfield);

        CmdWidget* mywidget = mymodel->selWidgetFromField(cmd,trimfield);
          SYD << tr("...MainWindow:::menuForm...mywidget (2)");

        if (mywidget == NULL ) {            
                       SYW << tr("No hay widget para \"%1\"").arg(trimfield);
            continue;
        }


        QString newitem;


        SYD << tr("...MainWindow:::menuForm...mywidget ....(2)trimfield:|%1|")
               .arg(trimfield);

        QString classname = QString(mywidget->metaObject()->className());
        SYD << tr("........MainWindow::menuForm......classname:|%1|")
               .arg(classname);

        mywidget->html();
        QString mytype = mywidget->typeInput();
        SYD << tr("........MainWindow::menuForm......TYPEINPUT:|%1|")
               .arg(mytype);


        if (QString(mywidget->metaObject()->className()) != "OutputWidget" &&  mytype != "type=button") {
            SYD << tr("........MainWindow::menuForm..Yes");
//          newitem = QString("<tr id='%3'><td>%1</td><td>%2</td></tr>\n")
//                  .arg(convertOpToTitle(f))
//                  .arg(mywidget->html())
//                  .arg("safetDiv"+trimfield);

            newitem = QString("%1\n")
//                    .arg(convertOpToTitle(f))
                    .arg(mywidget->html());
        }
        else {
            SYD << tr("........MainWindow::menuForm..No");
//            newitem = QString("<tr id='%2' ><td colspan=\"2\">%1</td></tr>\n")
//                    .arg(mywidget->html())
//                    .arg("safetDiv"+trimfield);
            newitem = QString("%1\n")
                    .arg(mywidget->html());
        }


          result += newitem;


    }
  //  result += QString("<tr> <td></td><td></td>  </tr>\n</table>\n");

    result += QString("<span id=\"safetspan\" name=\"safetspan\"></span> \n");


    result += QString("<br/>\n");
    result += QString("<table><tr><td></td><td>"
                      "<div class=\"button\">"
                      "<button type=\"submit\" class=\"btn btn-primary\" id=\"safetsubmit\" name=\"safetsubmit\">"
                       " Enviar "
                      "</button>"
                      "</div>"
                      //"<button id=\"safetcancel\" name=\"safetcancel\" type=\"reset\">Limpiar</button>"
                      "</td></tr>\n"
                      );
    result += QString("\n</table>"
                "\n</form>\n");

        //result += addParametersDialog(formurl);


    SYD << tr(".................MainWindow::menuForm......MENUFORM(2).......result:|%1|")
           .arg(result);

    return result;
}

QString MainWindow::addParametersDialog(const QString& nameoperation) {
    QString result;
    if ( !nameoperation.startsWith("consola:")) {
        return QString();
    }

    result =
            "<button id=\"parsbutton\" >Par&aacute;metros</button>"
            "<div class=\"demo\">\n"
            "<div id=\"parsdialog\" title=\"Par&aacute;metros\"  >\n"
                    "<p class=\"validateTips\">Cargue de nuevo la p&aacute;gina para ver los par&aacute;metros</p>\n"
                    "<form >\n"
                    "<span id=\"safetmarkup\" name=\"safetmarkup\"></span>"
                    "<div id=\"safetfieldset\" name=\"safetfieldset\">\n"
                            "Nombre<br/>\n"
                            "<input type=\"text\" name=\"name\" id=\"name\" class=\"text ui-widget-content ui-corner-all\" />\n"
                    "</div>\n"
                    "</form>\n"
            "</div>\n"
            "</div>\n";


    return result;

}

QStringList MainWindow::autoComplete(const QString& path) {

    QSet<QString> result;
    setInputPath(path);
    DomModel* mymodel = new DomModel(inputPath(),NULL);
    Q_CHECK_PTR( mymodel );

    QStringList commands = mymodel->getCommands();


    foreach(QString c, commands) {
        if ( !c.startsWith("operacion:")) {
            continue;
        }
        QString newcmd = QString("%1|").arg(c);
        result.insert(newcmd);
        if (c.split(":").count() < 2) {
            continue;
        }
        QString cmd = c.split(":").at(1).trimmed();
        QStringList fields = mymodel->getFields(cmd);
        foreach(QString f, fields) {
            QString newfield = f;
            QString trimfield = f;
            bool mandatory = false;

            if (newfield.endsWith("*")) {
                newfield.chop(1);
                trimfield = newfield.trimmed();
                mandatory = true;
            }
            CmdWidget* mywidget = mymodel->selWidgetFromField(cmd,trimfield);
            if (mywidget == NULL ) {
                result.insert(QString("No hay widget para \"%1\"").arg(trimfield));
                continue;
            }
            QStringList values = mywidget->options();
            QString namefield = newfield;
            foreach(QString v, values) {
                newfield = namefield + QLatin1String(": ") + v.trimmed() +
                        QString("|%1")
                        .arg(mandatory?QLatin1String("*"):QLatin1String(""));
                result.insert(newfield);
            }

        }



    }

    return result.toList();

}

MainWindow::~MainWindow() {

    if (SafetYAWL::listPlugins != NULL ) {
        for(int i=0; i < SafetYAWL::listPlugins->count();i++) {
            QPluginLoader* myplug = SafetYAWL::listPlugins->at(i);
            if (myplug) {

                QObject* myobject = myplug->instance();
                if (myobject) {
                    delete myobject;
                }
                myplug->unload();
            }
        }
        SafetYAWL::listPlugins->clear();
        SafetYAWL::listDynWidget->clear();
        delete SafetYAWL::listPlugins;
        delete SafetYAWL::listDynWidget;
        SafetYAWL::listPlugins = NULL;
        SafetYAWL::listDynWidget = NULL;

    }

}


void MainWindow::setToInputManagementSignDocument() {

}

void MainWindow::setToInputReports() {

}



void MainWindow::setToInputFlowGraph() {

}


QString MainWindow::getPrincipalCSS() const {
    return QString::fromUtf8(
    "* {\n"
    "color: #355670;\n"
    "font: 12px \"Arial, Liberation Sans\";\n"
    "}\n"
    "\n"
    "QTabWidget {"
    "    background-image: url(:/background.png);} \n"
    "#background\n"
    "{ background-image: url(\":/background.png\"); }\n"
    "\n"
    "/* widgets */\n"
    "QPushButton\n"
    "{\n"
    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #406A9E, stop: 0.75 #00355F);\n"
    "border: 0px;\n"
    "border-radius: 2px;\n"
    "color: white;\n"
    "margin-left: 1px;\n"
    "margin-right: 1px;\n"
    "min-width: 75px;\n"
    "padding: 1px 7px;\n"
    "}\n"
    "\n"
    "QDialogButtonBox QPushButton, #home QPushButton\n"
    "{ padding: 4px 7px; }\n"
    "\n"
    "QPushButton:hover, QPushButton:focus\n"
    "{\n"
    "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #F1C157, stop:0.28 #FADF91, stop:1 #EBA927);\n"
    "color: #00355F;\n"
    "}\n"
    "\n"
    "QPushButton:disabled\n"
    "{ background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #A3AFC0, stop: 0.75 #80909D); }\n"
    "\n"
    "/* header */\n"
    "#headerLine, #headerLine2\n"
    "{ background-color: #00355F; }\n"
    "\n"
    "#cards, #languages\n"
    "{\n"
    "background-color: #00355F;"
            "\n"
    "border: 0px;\n"
    "margin-right: 0px;\n"
    "color: white;\n"
    "}\n"
    "\n"
    "#cards QListView, #languages QListView\n"
    "{\n"
    "background-color: white;\n"
    "border: 0px;\n"
    "color: #355670;\n"
    "}\n"
    "\n"
    "#cards::down-arrow, #languages::down-arrow\n"
    "{ image: url(\":/images/languages_button.png\"); }\n"
    "\n"
    "#cards::drop-down, #languages::drop-down\n"
    "{ border: 0px; }\n"
    "\n"
    "#settings, #help\n"
    "{\n"
    "background-color: #00355F;\n"
    "color: #afc6d1;\n"
    "font: bold;\n"
    "min-width: 0px;\n"
    "}\n"
    "\n"
    "#settings:hover, #settings:focus\n"
    "#help:hover, #help:focus\n"
    "{ text-decoration: underline; }\n"
    "\n"
    "#infoFrame\n"
    "{\n"
    "background-color: rgba( 255, 255, 255, 153 );\n"
    "border-radius: 3px;\n"
    "color: #668696;\n"
    "padding-left: 10px;\n"
    "padding-right: 10px;\n"
    "}\n"
    "\n"
    "#infoCard\n"
    "{ font: bold; }\n"
    "\n"
    "/* content */\n"
    "#homeButtonLabel,\n"
    "#viewContentLabel, #viewKeysLabel\n"
    "{\n"
    "color: #00355F;\n"
    "font: bold;\n"
    "}\n"
    "\n"
    "#home, #intro, #view,\n"
    "#viewContentView\n"
    "{ background-color: tra"
            "nsparent; }\n"
    "\n"
    "#viewContentFrame\n"
    "{\n"
    "background-color: rgba( 255, 255, 255, 200 );\n"
    "border: 1px solid gray;\n"
    "color: #355670;\n"
    "padding: 5px;\n"
    "}\n"
    "\n"
    "#viewKeysFrame\n"
    "{\n"
    "background-color: rgba( 255, 255, 255, 200 );\n"
                "border: 1px solid gray;\n"
    "padding: 5px;\n"
    "}\n"
    "\n"
    "#viewKeys, #viewKeysScroll\n"
    "{\n"
    "background-color: transparent;\n"
    "color: #71889A;\n"
    "}");
}



void MainWindow::goPrincipal() {

}



void MainWindow::linkClickedSbMenu(const QUrl& url) {
    Q_ASSERT( url.toString().split(":", QString::SkipEmptyParts).count() > 1 );
    QString action = url.toString().split(SafetYAWL::LISTSEPARATORCHARACTER, QString::SkipEmptyParts).at(1);
    Q_ASSERT(!action.isEmpty());
}

void MainWindow::linkClickedSbResult(const QUrl& url) {


}

void MainWindow::createDockWindow() {

}
void MainWindow::createDockShowResultWindow() {
 
}




void MainWindow::buildMenuOnPanelsbMenu(const QString& option) {

}






void MainWindow::loadSettings() {
     int count;
     bool ok;
     QMap<QString,QVariant> conf;
     QSettings settings("CENDITEL","Safet");
     settings.beginGroup("Inflow");
     QVariant vcount(count);
//     QVariant myvar(_plugs);
//     _plugs = settings.value("plugs", myvar).toStringList();
     count = settings.value("confscount", vcount).toInt(&ok);
     if ( ok == false ) {
         count = 0;
     }

     for ( int i = 0; i < count; i++) {
          QString valuename = QString("confscount%1").arg(i);
          conf = settings.value(valuename, conf).toMap();
     }

     settings.endGroup();

}

void MainWindow::writeSettings() {

     

 }


void MainWindow::setModelCompleter(int opt) {


}


void MainWindow::toDelAllWorkflow() {

}

void MainWindow::toDelOneWorkflow() {

}

void MainWindow::toClearTextEdit() {
   // limpiar la barra de estado

}

QMap<QString,QString> MainWindow::doPermiseOperation(const QString& op) {

    QMap<QString,QString> result;

    QString nameop = op;
    if (op.startsWith(QLatin1String("operacion:titulo::"))) {
        return result;
    }
    if (op.startsWith(QLatin1String("operacion:"))) {
        nameop = op.mid( QString("operacion:").length() );
    }

        SYD    <<  tr("...........MainWindow::doPermiseExecOperationAction...PERMISE...nameop:|%1|")
                   .arg(nameop);

    if (!MainWindow::permises.contains(nameop))  {

//        SYD    << tr("La operación \"\"%1\"\" no existe en el archivo de autorización<br/>."
//                      "Consulte con el administrador para que asigne el permiso solicitado").arg(op);
        return result;
    }

    if ( MainWindow::permises[ nameop ].count() < 3) {
                SYE << tr("La operación \"%1\" solo tiene \"%2\" elementos de información.<br/>Es "
                      "probable que el archivo de autorización no esté correctamente formado")
                .arg(nameop).arg(MainWindow::permises[ nameop ].count());

                return result;
    }

    QStringList mytypes = MainWindow::permises[ nameop ].at(1).split(";");

    QStringList myusers = MainWindow::permises[ nameop ].at(0).split(";");
    QStringList myroles = MainWindow::permises[ nameop ].at(2).split(";");

   QStringList currentroles = MainWindow::currentrole.split(";",QString::SkipEmptyParts);

    bool foundrole = false;
    foreach(QString r, currentroles)  {
        if ( myroles.contains(r) ) {
            foundrole = true;
            break;
        }

    }

    foreach(QString t, mytypes) {
        QStringList mylist = t.split(":",QString::SkipEmptyParts);
        if (mylist.count()>1) {
            if (currentaccount == mylist.at(0)) {
                result[mylist.at(1)] = "true";
             }
            else {
               if (myusers.contains(currentaccount) || foundrole) {
                     result[mylist.at(1)] = "true";
               }
               else {
                   result[mylist.at(1)] = "false";
               }
            }
        }
        else {
            if (myusers.contains(currentaccount) || foundrole) {
                result[mylist.at(0)] = "true";
            }
            else {
                result[mylist.at(0)] = "false";
            }
        }


    }

    return result;
}



QMap<QString,bool> MainWindow::doPermiseExecOperationAction(const QString& op) {

    QMap<QString,bool> result;

    QString nameop = op;
    if (op.startsWith(QLatin1String("operacion:titulo::"))) {
        return result;
    }
    if (op.startsWith(QLatin1String("operacion:"))) {
        nameop = op.mid( QString("operacion:").length() );
    }

        SYD    <<  tr("...........MainWindow::doPermiseExecOperationAction...PERMISE...nameop:|%1|")
                   .arg(nameop);

    if (!MainWindow::permises.contains(nameop))  {

//        SYD    << tr("La operación \"\"%1\"\" no existe en el archivo de autorización<br/>."
//                      "Consulte con el administrador para que asigne el permiso solicitado").arg(op);
        return result;
    }

    if ( MainWindow::permises[ nameop ].count() < 3) {
                SYE << tr("La operación \"%1\" solo tiene \"%2\" elementos de información.<br/>Es "
                      "probable que el archivo de autorización no esté correctamente formado")
                .arg(nameop).arg(MainWindow::permises[ nameop ].count());

                return result;
    }

    QStringList mytypes = MainWindow::permises[ nameop ].at(1).split(";");

    QStringList myusers = MainWindow::permises[ nameop ].at(0).split(";");
    QStringList myroles = MainWindow::permises[ nameop ].at(2).split(";");

   QStringList currentroles = MainWindow::currentrole.split(";",QString::SkipEmptyParts);

    bool foundrole = false;
    foreach(QString r, currentroles)  {
        if ( myroles.contains(r) ) {
            foundrole = true;
            break;
        }

    }

    foreach(QString t, mytypes) {
        QStringList mylist = t.split(":",QString::SkipEmptyParts);
        if (mylist.count()>1) {
            if (currentaccount == mylist.at(0)) {
                result[mylist.at(1)] = true;
             }
            else {
               if (myusers.contains(currentaccount) || foundrole) {
                     result[mylist.at(1)] = true;
               }
               else {
                   result[mylist.at(1)] = false;
               }
            }
        }
        else {
            if (myusers.contains(currentaccount) || foundrole) {
                result[mylist.at(0)] = true;
            }
            else {
                result[mylist.at(0)] = false;
            }
        }


    }

    return result;
}




QMap<QString,QString> MainWindow::loadPermisesHierarchy() {
    int i = 1;
    SYD << tr("MainWindow::loadPermisesHierarchy()...");
    QMap<QString,QString> result;
    while(true) {
        QString mykeyconf = QString("Hierarchy/path.%1").arg(i);
        QString mypath = SafetYAWL::getAuthConf()[mykeyconf].trimmed();
        if (mypath.isEmpty() || i>10) {
            break;
        }

        QStringList mylist = mypath.split(",",QString::SkipEmptyParts);
        if (mylist.size() > 0 ) {
            if ( !result.contains(mylist.at(mylist.size()-1)) ) {
                result[ mylist.at(mylist.size()-1)] = "";
            }
            else {
                result[ mylist.at(mylist.size()-1)] += ",";
            }
            for(int j=mylist.size()-2;j>=0;j--) {
                result[ mylist.at(mylist.size()-1)] += mylist.at(j);
                result[ mylist.at(mylist.size()-1)] += ",";
            }
             result[ mylist.at(mylist.size()-1)].chop(1);
        }

        i++;
    }

    return result;
}


bool MainWindow::doPermiseExecOperation(const QString& op, QMap<QString,QString>& phierarchy,
                                        const QString& permise) {

    SYD << tr("...MainWindow::doPermiseExecOperation...PERMISE....MainWindow::permises....:|%1|...permise:"
              "|%2|")
           .arg(MainWindow::permises.count())
           .arg(permise);

    QString nameop = op;
     SYD << tr("...MainWindow::doPermiseExecOperation...PERMISE...nameop:|%1|")
            .arg(nameop);

    if (op.startsWith(QLatin1String("operacion:titulo::"))) {
        return true;
    }
    if (op.startsWith(QLatin1String("operacion:"))) {
        nameop = op.mid( QString("operacion:").length() );
    }
    if (!MainWindow::permises.contains(nameop))  {

        SYD << tr("La operacion cuantos %1").arg(MainWindow::permises.count());
        foreach( QString k, MainWindow::permises.keys()) {
                 SYD << tr("La operacion %1").arg(k);
    }
//        SYD
//                << tr("La operación \"\"%1\"\" no existe en el archivo de autorización<br/>."
//                      "Consulte con el administrador para que asigne el permiso solicitado").arg(op);

         SYD << tr("...MainWindow::doPermiseExecOperation...---$PERMISE..(1)...");
        return false;
    }

    if ( MainWindow::permises[ nameop ].count() < 3) {
                SYE << tr("La operación \"%1\" solo tiene \"%2\" elementos de información.<br/>Es "
                      "probable que el archivo de autorización no esté correctamente formado")
                .arg(nameop).arg(MainWindow::permises[ nameop ].count());
        return false;
    }
    QStringList myusers = MainWindow::permises[ nameop ].at(0).split(";");
    QStringList myroles = MainWindow::permises[ nameop ].at(2).split(";");
   QStringList currentroles = MainWindow::currentrole.split(";",QString::SkipEmptyParts);

    bool foundrole = false;
    foreach(QString r, currentroles)  {
        if ( myroles.contains(r) ) {
            foundrole = true;
            break;
        }

    }

    if (!myusers.contains(currentaccount) && !foundrole) {
           SYD  << tr("El usuario <b>%1</b> no está autorizado para utilizar la operación <b>\"%2\"</b>.<br/>"
                      "Consulte con el administrador para que asigne el permiso solicitado")
                .arg(currentaccount).arg(nameop);
        return false;
    }
    QStringList mytypes = MainWindow::permises[ nameop ].at(1).split(";");

    QString realtypes;
    foreach(QString t, mytypes) {
        QStringList mylist = t.split(":",QString::SkipEmptyParts);
        if (mylist.count()>1) {
            if (currentaccount == mylist.at(0) || currentrole == mylist.at(0)) {
                realtypes += mylist.at(1);
                realtypes += ",";
            }
        }
        else {
            realtypes += mylist.at(0);
            realtypes += ",";
        }
    }
    realtypes.chop(1);

    if (!realtypes.contains(permise) && !MainWindow::checkPermiseInHierarchy(permise,phierarchy,realtypes)) {
                SYE << tr("El usuario \"%1\" no tiene autorizado  el permiso del tipo \"%3\" "
                      "que le permita utilizar la operación \"%2\"<br/>"
                      "Consulte con el administrador para que asigne el permiso solicitado")
                .arg(currentaccount).arg(nameop).arg(permise);
                SYD << tr("...MainWindow::doPermiseExecOperation...permise:|%2|...realtypes:|%1|")
                       .arg(realtypes)
                       .arg(permise);
        return false;
    }


    return true;
}

bool MainWindow::checkPermiseInHierarchy(const QString& permise,
                                         const QMap<QString,QString>& myphierarchy,
                                         const QString& realtypes) {
    SYD << tr("...MainWindow::checkPermiseInHierarchy...myphierarchy.size():|%1|")
           .arg(myphierarchy.size());
    if (!myphierarchy.contains(permise)) {
        return false;
    }
    QStringList  mylist = myphierarchy[permise].split(",",QString::SkipEmptyParts);

    foreach(QString s, mylist) {
        if (realtypes.contains(s)) {
            return true;
        }
    }
    return false;

}

void MainWindow::setupToolbar() {
}




void MainWindow::setEnabledToolBar(bool e) {

}


void MainWindow::checkSelInputTab(int opt) {
}


void MainWindow::selInputTab(int opt)  {

}


void MainWindow::toLoadWeb() {

    QString code;
    code = "SAFETlist = ["
     "{areacode: \"201\", state: \"Merida\"},"
     "{areacode: \"203\", state: \"Zulia\"},"
     "{areacode: \"204\", state: \"Trujillo\"},"
     "{areacode: \"205\", state: \"Bolivar\"}"
     "];"
     "myColumnDefs = ["
     "{key:\"areacode\",label:\"C?digo\",width:100,resizeable:true,sortable:true},"
     "{key:\"state\",label:\"Estado\",width:100,resizeable:true,sortable:true}"
     "];"
     " safetproccessData()";

    //code = "doListTable()";

    evalJS(code);

}



void MainWindow::executeJSCodeAfterLoad(/* bool ok */ ) {
    QString typeresult = SafetYAWL::getConf()["Result/output.type"];

    if (typeresult == "VariableData" ) {
        executeWithoutJSON();
    }
    else {
        executeWithJSON();
    }


}

void MainWindow::executeWithoutJSON() {


    // ** Reemplazar caracteres especiales, hacen falta pruebas aqui
     currentDocuments.replace("\n","");
     // ** Reemplazar caracteres especiales, hacen falta pruebas aqui
     QString data;
    data =  QString( "safetvariable = '%1';\n").arg(_listprincipalvariable);
    data += QString("safetkey = '%1';\n").arg(_listprincipalkey);
    QStringList mylist = _listprincipaltitle.split(SafetYAWL::LISTSEPARATORCHARACTER,QString::SkipEmptyParts);
    data += QString("safettitle = '%1';\n").arg(mylist.at(0));
    if ( mylist.count() > 1 ) {
            data += QString("safetreport = '%1';\n").arg(mylist.at(1));
    }
    else {
        data += QString("safetreport = '%1';\n").arg("Reporte");
    }

    data += QString("safetlistcount = %1;\n").arg(_listprincipalcount);
    SYD << tr("....MainWindow::listDocuments..._listprincipaltitle:|%1|")
           .arg(_listprincipaltitle);

    QString jsondata;


    _currentdata = "[" + currentDocuments + " ]";


    QString jquerycolumns = QString("\n safetjson = {\n \"columns\" : [");


    jscriptcolumns = "safetcolumns  = [";
    int i = 0;
    foreach(QSqlField f, currentFields ) {

        QString fname = f.name();
        QString scriptlen = SafetYAWL::getScriptLen(f);
         jscriptcolumns
                 += QString("{ key: \"%1\",label:\"%1\",width:%2,"
                            "resizeable:true,sortable:true},\n")
                    .arg(fname)
                    .arg(scriptlen);

         jquerycolumns += QString(" {\"sTitle\": \"%1\"},\n")
                 .arg(fname);

         i++;
    }
      jquerycolumns += " ],\n";

      jquerycolumns += "\n \"tasks\": [ " + currentADocuments + "]\n";

      jquerycolumns += "\n};\n\n";



    if (   i > 0 ) {
        jscriptcolumns.chop(2);
    }
    jscriptcolumns += "];\n";


    jsondata += jscriptcolumns;

    jsondata += jquerycolumns;

    jsondata += "safetlist = [";
    jsondata += currentDocuments;
    jsondata += "];\n";

    data += jsondata;


    QString code;
    code = data;
    // Colocando los datos JSON
    QString typeresult = SafetYAWL::getConf()["Result/output.type"];

    _currentjson = data;


    code = "safetproccessData();";



}

QStringList MainWindow::generateTips(const QString& cs, const QString& parcialscript) {

    QStringList result;
    int nargs;
        char** argv =NULL;
        QCoreApplication myapp(nargs,argv);
        QScriptEngine myEngine;

        QString newcs = cs;
        newcs.replace("\n","");

        QString mycs = QString("(function(fname) { mydata =  %1; try { \n"
                               "\n"
                               "%2"
                               "\n"


                               "} catch(err) { return \"::safeterror::\" }})")
                .arg(newcs)
                .arg(parcialscript);


        SYD << tr("mycs:\n%1\n")
               .arg(mycs);

        QScriptValue myfun;
        QScriptValue myvalue;
        QScriptValueList args;
        QString currvalue;
        args.append("datos");
        try  {

            myfun = myEngine.evaluate(mycs);
            myvalue = myfun.call(QScriptValue(),args);
            currvalue = myvalue.toString();
            if ( currvalue == "undefined") {
                SYD << tr(".....EVALTIPS....undefined");
            }

            if (myEngine.hasUncaughtException()) {
                int line = myEngine.uncaughtExceptionLineNumber();
                SYD  << tr("uncaught exception at line: %1 - value:|%2|").arg(line).arg(currvalue);
            }
        }
        catch(...) {
            SYE << tr("IMPORTANTE:Ocurrió un error (excepcion) al evaluar el script (two)");
        }
        SYD << tr("........EVALTIPS....cs tips...currvalue->:|%1|").arg(currvalue);

        result = currvalue.split(SafetYAWL::LISTSEPARATORCHARACTER, QString::SkipEmptyParts);
        return result;


}

void MainWindow::executeWithJSON( ) {



     currentDocuments.replace("\n","");
     QString data;
    data =  QString( " { \"safetvariable\" : \"%1\",\n").arg(_listprincipalvariable);
    data += QString(" \"safetkey\" : \"%1\",\n").arg(_listprincipalkey);
    QStringList mylist = _listprincipaltitle.split(SafetYAWL::LISTSEPARATORCHARACTER,QString::SkipEmptyParts);
    data += QString(" \"safettitle\" : \"%1\",\n").arg(mylist.at(0));
    if ( mylist.count() > 1 ) {
            data += QString("\"safetreport\" : \"%1\",\n").arg(mylist.at(1));
    }
    else {
        data += QString("\"safetreport\" : \"%1\",\n").arg("Reporte");
    }

    data += QString("\"safetlistcount\" : \"%1\",\n").arg(_listprincipalcount);
    SYD << tr("....MainWindow::listDocuments..._listprincipaltitle:|%1|")
           .arg(_listprincipaltitle);

    QString jsondata;
    jsondata += "\"safetlist\" : [";
    jsondata += currentDocuments;
    jsondata += "],\n";


    QString tipsdata;
    tipsdata += " { \"safetlist\": [";
    tipsdata += currentDocuments;
    tipsdata += "] }";
    QStringList mytips;

    _currentdata = "[" + currentDocuments + " ]";

    SYD << tr("....MainWindow::executeWithJSON....SCRIPTENGINE...myscript...searchScript:|%1|")
           .arg(_listprincipalvariable);
    QString myscript = searchScript(_listprincipalvariable);

    if (  !myscript.isEmpty() ) {
            SYD << tr("...EVALTIPS.......mydir....(1) exist");
            SYD << tr("...EVALTIPS.......mydir....(1) myscript:\n|%1|")
                   .arg(myscript);

            mytips = generateTips(tipsdata,myscript);
            SYD << tr("...EVALTIPS.......mydir....(1) mytips.count():\n|%1|")
                   .arg(mytips.count());
            if (mytips.count() > 0) {
                QString tipsresult;
                tipsresult += "  \"safettips\": [";
                foreach(QString t, mytips) {
                    tipsresult += "{ \"tip\": \"" + t +"\"},";
                }
                tipsresult.chop(1);
                tipsresult += "],\n";

                data += " ";
                data += tipsresult;
                data += "";


            }

    }


    jscriptcolumns = "\"safetcolumns\"  : [";
    int i = 0;
    foreach(QSqlField f, currentFields ) {

        QString scriptlen = SafetYAWL::getScriptLen(f);
         jscriptcolumns
                 += QString("{ \"key\": \"%1\",\"label\":\"%1\",\"width\":\"%2\","
                            "\"resizeable\":\"true\",\"sortable\":\"true\"},\n")
                    .arg(f.name())
                    .arg(scriptlen);

         i++;
    }
    if ( i > 0 ) {
        jscriptcolumns.chop(2);
    }
    jscriptcolumns += "  ]\n}\n";
    jsondata += jscriptcolumns;

    data += jsondata;

    QString code;
    code = data;

    _currentjson = data;


    // Colocando los datos JSON
    code += " ";
    code += jscriptcolumns;
    code += "";

    SYD << tr(".........EVALTIPS...MainWindow::executeJSCodeAfterLoad().......code:|%1|")
           .arg(code);




}

QString MainWindow::searchScript(const QString& namevar) {

    QString result = "";

    QString confpath = _currconfpath + "/" + Safet::datadir + "/" + "scripts";

    SYD << tr("MainWindow::searchScript.....confpath:|%1|").arg(confpath);
    SYD << tr("MainWindow::searchScript.....namevar:|%1|").arg(namevar);

    QDir dirconf(confpath);
    dirconf.setFilter(QDir::Files | QDir::NoSymLinks);
    if (!dirconf.exists()) {
        SYA                << tr("No existe el directorio de scripts: \"%1\"")
                .arg(confpath);
        return QString("");
    }


    SYD << tr("MainWindow::searchScript.....(1)");

    QFileInfoList mylist = dirconf.entryInfoList();

    QString currfile;

    for (int i=0; i<mylist.size(); i++) {
        QFileInfo fileInfo = mylist.at(i);
        QString myfile = fileInfo.filePath();
        SYD << tr("MainWindow::searchScript.....myfile:|%1|").arg(myfile);
        if (myfile.section("/",-1) == namevar) {
            SYD << tr("MainWindow::searchScript.....yes:|%1|").arg(myfile);
            currfile = myfile;
        }

    }

    SYD << tr("MainWindow::searchScript.....currfile:|%1|").arg(currfile);

    if (currfile.isEmpty()) {
        return QString("");
    }

    QFile myfile(currfile);
    bool open = myfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!open) {
        SYW << tr("No se pudo abrir el archivo :\"%1\"")
               .arg(currfile);
        return QString("");
    }

    QTextStream in(&myfile);


    result = in.readAll();


    myfile.close();

    return result;


}


void MainWindow::generateJScriptreports(const QString& documents,
                                        const QList<QSqlField>& fields) {

     currentDocuments = documents;
     currentFields = fields;
     jscriptload = true;

}


QString MainWindow::evalJS(const QString &js) {
//     Q_CHECK_PTR( weboutput );
//     qDebug(".....evalJS...QWebFrame *frame = weboutput->page()->mainFrame();");
//    QWebFrame *frame = weboutput->page()->mainFrame();
//    return frame->evaluateJavaScript(js).toString();
    return QString("");
}

void MainWindow::toSend(bool sign) {

    
}

bool MainWindow::toInputUsers(const QString& action) {

        SYD << tr("....**MainWindow::TOINPUTUSERS..(BEFORE_EXTRACT).....action:|%1|").arg(action);
    SafetTextParser parser;
    /**
      No especificar porque es un parametro en la funcion Generar_ticket
      **/
//    QString texto = extractParameters(action);
       /**
        * En lugar de
        */
    QString texto = action;


    SYD << tr("....MainWindow::TOINPUTUSERS...(AFTER_EXTRACT)....action (texto):|%1|").arg(texto);
//    texto = completingTextEdit->toPlainText().toLatin1();
    //qDebug(".. ...(1)..texto: \n|%s|\n", qPrintable(texto));

    parser.setStr( texto.toLatin1() );
    QString str = "agregar,eliminar,actualizar,mostrar";
    QString commandstr = "INSERT INTO,DELETE,UPDATE,SELECT";
    parser.setCommands(str.split(","),commandstr.split(","));

    QString xml = parser.toXml();

    SYD << tr("Cadena XML generada de la entrada: \n\"%1\"").arg(xml);

        SYD << tr(".......MainWindow::toInputUsers....processInput.....(1)...");
    parser.processInput( xml.toLatin1() );
    QString filepath = SafetYAWL::getConfFile().getValue("Input", "input.path").toString();
    QString filename = "defusers.xml";

            SYD << tr(".......MainWindow::toInputUsers....openXml.....(1)...");
    parser.openXml(filepath + "/" + filename);
            SYD << tr(".......MainWindow::toInputUsers....openXml.....(2)...");


    QStringList results = parser.processXml(false);

    SYD << tr(".......MainWindow::toInputUsers....queryForErrors.....(1)...");
    if ( queryForErrors() ) {

            SYE << tr("Ocurrió un error al procesar la acción en el archivo de usuarios");
           return false;
    }

    ParsedSqlToData data;

    SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....EVALUATING (1)...");

    if ( results.count() > 0) {

        SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....results.count():|%1|")
               .arg(results.count());
        data = SafetTextParser::parseSql(results.at(0),true);


        SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....****operationName:|%1|")
               .arg(parser.operationName());
        QString myoperation = parser.operationName();

        foreach(QString k, data.map.keys()) {

            SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS [%1] : |%2|")
                   .arg(k)
                   .arg(data.map[k]);
        }

        if ( myoperation.startsWith("Generar_ticket")) {

            SYD << tr("\n...........MainWindow::toInputUsers....INPUTUSERS....GENERATINGTICKETS...");

            SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....cuenta (key):|%1|")
                   .arg(data.key);
            SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....cuenta (keyvalue):|%1|")
                   .arg(data.keyvalue);
            SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....Destinarios:|%1|")
                   .arg(data.map["Destinatarios"]);
            SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....Nombre_operacion:|%1|")
                   .arg(data.map["Nombre_operacion"]);

            SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....Asunto:|%1|")
                   .arg(data.map["Asunto"]);
            SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....accion:|%1|\n\n")
                   .arg(data.map["accion"]);

            if (data.map.contains("Emailtemplate")) {
                SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....configurekey.Email/email.template.1:|%1|\n\n")
                       .arg(data.map["Emailtemplate"]);

            }
            else {
                SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....configurekey.Email/email.template.1:|%1|\n\n")
                       .arg("empty");

            }


            QString myaction = data.map["accion"];

            myaction.replace(" ","%20");
            QString emailtemplate = data.map["Emailtemplate"];
            emailtemplate.replace("_SAFETACTION",QString("=%1").arg(myaction));
            QString newaction = QString("operacion:%1 %2: %3"
                    "  Asunto: %4  Destinatarios: %5  "
                    "configurekey.Email/email.template.1:%6")
                    .arg(data.map["Nombre_operacion"])
                    .arg(data.key)
                    .arg(data.keyvalue)
                    .arg(data.map["Asunto"])
                    .arg(data.map["Destinatarios"])
                    .arg(emailtemplate);



            SYD << tr(".........MainWindow::toInputUsers....INPUTUSERS..NEWACTION (1)...newaction:|%1|")
                   .arg(newaction);
            toInputForm(newaction,false);
            SYD << tr(".........MainWindow::toInputUsers....INPUTUSERS..NEWACTION (2)...");


            return true;
        }

        SYD << tr("...........MainWindow::toInputUsers....INPUTUSERS....data.map.count():|%1|")
               .arg(data.map.count());




    }


        QString fileconf = SafetYAWL::pathconf+ "/" + "auth.conf";

        SYD << tr(".......MainWindow::toInputUsers....WITHOUTREPLACE...fileconf........:|%1|").arg(fileconf);
        if (QFile::exists(fileconf)) {
             foreach(QString s, results) {
                 SYD << tr("........MainWindow::toInputUsers....s:|%1|").arg(s);

                proccessConfFile(s,fileconf,true);
            }


            //doCipherFile(true);
             SafetYAWL::readAuthConfFile();

         }
        else {

                    SYE << tr("No se pueden realizar los cambios. No se encuentra el archivo :\"%1\"")
                    .arg(fileconf);

            if ( queryForErrors() ) {
                qDebug(".......****..queryforerror....(2)");
                     return false;
             }


        }

        SYD << tr(".......MainWindow::toInputUsers....fileconf........returning...ok!");

        return true;


    }

QString MainWindow::genTicket(const QString& user) {
    QString result;
    if (user.isEmpty()) {
        SYW << tr("En la funcion de generacion de ticket se paso un usuario vacio");
        return result;
    }

    if (!users.contains(user)) {
        SYW << tr("El usuario \"%1\"para la generación de ticket no se encuentra. No se generará el ticket")
               .arg(user);
        return result;
    }


    if (users[user].count() < 4 ) {
        SYW << tr("El usuario \"%1\" no contiene el campo para generación de tickets. No se generará el ticket")
               .arg(user);
        return result;
    }

    srand( (unsigned int)time( NULL ) );

    QString myrandom = QString("%1").arg(rand());
    QByteArray textsha1 = QCryptographicHash::hash(QByteArray(qPrintable(myrandom)),
                                                                   QCryptographicHash::Sha1);


    QString currhash = textsha1.toHex().data();
    QString newhash;

    result = currhash;

    newhash = QString(users[user][3].isEmpty()?"%2%1":"%2;%1")
            .arg(currhash)
            .arg(users[user][3]);
    QString newaction = QString("operacion:Modificar_usuario Nombre_cuenta_usuario:%1 Tickets_usuario:%2")
            .arg(user)
            .arg(newhash);

    toInputUsers(newaction);

    SYD << tr("..........MainWindow::genTicket...HASH.newticket:|%|").arg(newhash);




    return result;

}

void MainWindow::showSuccessfulMessage(const QString& m) {
    QString message = QString("<table><tr><td><font color=green>%1</font>"
                              "</td></tr></table>")
            .arg(m);

}

void MainWindow::configureStatusBar() {


}



void MainWindow::showSmartMenu() {


}


void MainWindow::timeHideResult() {

}




bool MainWindow::queryForErrors() {


    QStack<QPair<QDateTime,QString> > mystack = SafetYAWL::streamlog.stopErrorStack();
    SYD << trUtf8("Hay \"%1\" errores guardados en la lista. De inmediato se procede"
                                                      " a inicializar la lista de errores.")
            .arg(mystack.count());
    SafetYAWL::streamlog.stopAllStack();

    if (mystack.count() > 0 ) { // Verificacion NO exitosa
        QString message = renderMessageStack(mystack,SafetLog::Error);
        QString messagew = renderMessageStack(mystack,SafetLog::Warning);
	_currenterror = message;
	if ( !messagew.isEmpty()) {
	}
        return true;
    }
    return false;

}

bool MainWindow::createBdoc(const QString& content) {
       SYD << tr("..........MainWindow::createBdoc...(creando documento firmado)...content");
#ifdef SAFET_DIGIDOCPP
       digidoc::initialize();

           digidoc::X509CertStore* store = new digidoc::DirectoryX509CertStore();
           digidoc::X509CertStore::init(store);

          BDocDocument *mydoc = new BDocDocument();


          mydoc->init();
          mydoc->create("/var/www/media/firmaweb.bdoc");

          QString digest;
          mydoc->addDocument("/home/vbravo/tmp/salida.txt");
          mydoc->presignWeb("BES", "Merida", "Merida", "5101", "Venezuela",
                                "Desarrollador", "/home/vbravo/.firmaxml/digidocpp/certs/vb.pem", digest);


          SYD << tr("....MainWindow::createBdoc..........................digest:|%1|").arg(digest);
          mydoc->saveBDocContainer("/var/www/media/firmaelectronica.bdoc");


          if (mydoc) {
              delete mydoc;
          }
#endif
          return true;

}


QString  MainWindow::toInputForm(const QString& action,bool withpermises) {


     SafetTextParser parser;


     SYD << tr("MainWindow::toInputForm:...*LATIN1... (1) |%1|")
                       .arg(action);


     QString newaction = extractParameters(action);
     bool issignaction = false;
     SYD << tr("MainWindow::toInputForm:........|%1|")
                       .arg(newaction);




     QRegExp rx(tr("\\*FIRMAR\\s+Documento\\s+\\((.+)\\)\\s+por\\s+/(.+)/\\s+en\\s+(.+)\\s+usando\\s+/(.+)/"));


     int pos = rx.indexIn(newaction);


     /**
       *
       * pos>=0 indica que es una opción de firma electrónica
       *
       */
     /**
       * IMPLEMENTACIÓN DE FUNCIONES, TALES COMO "FIRMAR DOCUMENTO"
       *
       **/

     if (pos >= 0 ) {




         SYD << tr("MainWindow::toInputForm........ *SIGN (FIRMAR DOCUMENTO) evaluating rx....passed!");
         QString idvariable = rx.cap(1);
         QString iduser = rx.cap(2);
         QString idflow = rx.cap(3);
         QString idkey = rx.cap(4);
         SYD << QString("...MainWindow::toInputForm....*SIGN (FIRMAR DOCUMENTO) cap1:|%1|...cap2:|%2|...cap3:|%3|...cap4:|%4|")
                .arg(idvariable)
                .arg(iduser)
                .arg(idflow)
                .arg(idkey);


         int dcount = 0;
         if (MainWindow::configurator != NULL ) {
             delete  MainWindow::configurator;
             MainWindow::configurator = NULL;
         }

         SYD << QString("...MainWindow::toInputForm....*SIGN (FIRMAR DOCUMENTO) rx (1)....");
         MainWindow::configurator = new SafetYAWL();
         SYD << QString("...MainWindow::toInputForm....*SIGN (FIRMAR DOCUMENTO)rx (2)....idflow:|%1|").arg(idflow);
         MainWindow::configurator->openXML(idflow);
         MainWindow::configurator->convertXMLtoObjects();
         MainWindow::configurator->openDataSources();

         evalConffileValues();

         SYD << QString("...MainWindow::toInputForm....*SIGN (FIRMAR DOCUMENTO)....rx (4)....getDocuments...idkey:|%1|")
                .arg(idkey);
         SYD << QString("...MainWindow::toInputForm....*SIGN (FIRMAR DOCUMENTO)....rx (4)....getDocuments...idvariable:|%1|")
                .arg(idvariable);

          QList<QSqlField> fields;
          SafetDocument mydoc = MainWindow::configurator->getWorkflows().at(0)->getDocuments(idvariable,
                                                                                          fields,dcount,
                                                                                          SafetWorkflow::JSON, idkey);



          SYD << QString("...MainWindow::toInputForm....getDocuments...(After) fields.count():|%1|")
                 .arg(fields.count());

         QString documents = mydoc.documents();
         QString derror;



         SafetVariable* myvar = MainWindow::configurator->getWorkflows().at(0)->searchVariable(idvariable);

         QList<SafetWorkflow::InfoSigner> infosigners = MainWindow::configurator->getWorkflows()
                 .at(0)->getSignersDocument(myvar,idkey,derror);

         SYD << tr(".........MainWindow::toInputForm..........*SIGN (FIRMAR DOCUMENTO).....infosigners.count():|%1|")
                .arg(infosigners.count());


         SYD << tr(".........MainWindow::toInputForm..........*SIGN (FIRMAR DOCUMENTO).....derror:|%1|")
                .arg(derror);

         InflowFirmaDoc dlg(NULL);

         dlg.setAvailableFiles(mydoc.availableFiles());
         QString documentid;
         long fieldno = 0;



         SYD << tr("......MainWindow::toInputForm....................FIRMARDOCUMENTO....idkey:|%1|")
                .arg(idkey);

         if (myvar != NULL ) {

             myvar->getXMLDocument(idkey,fieldno, documentid);

             SYD << tr("......MainWindow::toInputForm....................FIRMARDOCUMENTO....documentid:|%1|")
                    .arg(documentid);

             SYD << tr("......MainWindow::toInputForm....................***documentid:|%1|").arg(documentid);

         }



            SYD << tr("......MainWindow::toInputForm....................**documents:|%1|")
                .arg(documents);


             QString _cursigners;
             dlg.loadSignersInfo(infosigners);
             dlg.generateJScriptreports(documents,fields,1,_cursigners, infosigners);
             bool ok;
             SYD << tr("......MainWindow::toInputForm............signers:\n|%1|").arg(dlg.currentSigners());

             SYD << tr("......MainWindow::toInputForm....................InflowFirmaDoc...(3)");
             dlg.executeJSCodeAfterLoad(ok);
             SYD << tr("......MainWindow::toInputForm....................InflowFirmaDoc...(5)");
             dlg.loadReportTemplate(documentid);
             dlg.savepdf();



         SYD << tr("......MainWindow::toInputForm....................pass.....InflowFirmaDoc...(1)");

         QString result;

         if (dlg.availableFiles().count() > 0 ) {


             SYD << tr("......MainWindow::toInputForm..................YES AVAILABLEFILE....");
                 result = documents+SafetYAWL::LISTSEPARATORCHARACTER+documentid+SafetYAWL::LISTSEPARATORCHARACTER+
                         dlg.availableFiles().at(0);
         }
         else {
             SYD << tr("......MainWindow::toInputForm..................NO AVAILABLEFILE....");

             result = documents+SafetYAWL::LISTSEPARATORCHARACTER+documentid;
         }

         SYD << tr("......MainWindow::toInputForm....*SIGN (FIRMAR DOCUMENTO) result:\n|%1|")
                .arg(result);

         return result;

     }


     if (newaction.contains(tr("*FIRMAR Documento ("))) {
         issignaction = true;
     }


     SYD << tr("MainWindow::toInputForm:...... |%1|")
            .arg(newaction);



     SafetYAWL::streamlog.initAllStack();

     if (newaction.indexOf(SafetYAWL::ENDOFFLOW) >= 0 ) {
         QString message =   tr("El estado de un documento no puede cambiar al estado \"%1\"")
                 .arg(SafetYAWL::ENDOFFLOW);
         message.replace("<","&lt;");
         message.replace(">","&gt;");
         SYE << message;
         queryForErrors();
         return QString("");

     }

     _listprincipaltitle = "**SAFET**" + newaction;



     parser.setStr( QString(newaction).replace("{",Safet::BRACKETOPENMARK).replace("}",Safet::BRACKETCLOSEMARK) );
     QString str = "agregar,eliminar,actualizar,mostrar";
     QString commandstr = "INSERT INTO,DELETE,UPDATE,SELECT";
     parser.setCommands(str.split(","),commandstr.split(","));

     QString xml = parser.toXml();



     SYD  << tr("*Cadena XML generada de la entrada: \n\"%1\"").arg(xml);
     if (issignaction) {
         return QString("");
     }
     QString filepath = SafetYAWL::getConfFile().getValue("Input", "input.path").toString();
     QString filename = "deftrac.xml";


     parser.processInput( xml.toLatin1() );

     parser.openXml(filepath + "/" + filename);
     //QStringList names = parser.loadNameConfs();

     parser.loadNameConfs();

     QStringList results;

     SYD  << tr("..............MainWindow::toInputForm....enviarcorreo..xml:\n|%1|")
             .arg(xml);



     /**
       * IMPLEMENTACIÓN DE FUNCIONES, TALES COMO "ENVIAR POR CORREO"
       *
       **/
     bool genjson = true;

     if (xml.indexOf("por_correo") > 0) {
         SYD  << tr("..............MainWindow::toInputForm....*****PORCORREO...enviarcorreo..OK!");
         SYD    << tr("MainWindow::evalConffileValues()...(IF2) CONFVALUE....MYNEWVALUE..SafetYAWL::getConf()[\"Email/template.1\"]:|%1|")
                   .arg(SafetYAWL::getConf()["Email/template.1"]);

         results = parser.processXml(false,withpermises);
         if ( results.count() == 0) {
             SYE << tr("No se produjo el resultado esperado de la operación.");
             return QString("");

         }

         SYD  << tr("..............MainWindow::toInputForm....results.at(0):|%1|")
                 .arg(results.at(0));


         ParsedSqlToData  data = SafetTextParser::parseSql(results.at(0),true);

         SYD  << tr("..............MainWindow::toInputForm....operationName():|%1|")
                 .arg(parser.operationName());
         SYD  << tr("..............MainWindow::toInputForm....data.map.keys():|%1|")
                 .arg(data.map.keys().count());

         foreach(QString f, data.map.keys()) {

             SYD  << tr("..............MainWindow::toInputForm.....data[\"%1\"]=\"%2\"")
                     .arg(f)
                     .arg(data.map[f]);
         }

         SYD  << tr("..............MainWindow::toInputForm.....data....DATA (2)....");

         QString mydataaction = parser.currentDataAction();

         QString mycurrent = "";


         if  (!mydataaction.isEmpty()) {

             QString adjaction = mydataaction
                     .arg(data.map["id"]);
             bool result = toInputConsole(adjaction,false);

             if (!result ) {
                 SYE << tr("No se ejecuto la acción para el envio de correo");
                 return QString("");

             }

             mycurrent = currentDATA();
             SYD << tr("....MainWindow::toInputForm....ADJDATA:|%1|")
                    .arg(mycurrent);
         }
         _currentjson = QString("{ \"result\": \"true\", %1 }");
         _currentjson = _currentjson.arg(QString(" \"current_id\":\"%1\", %%1").arg(data.map["id"]));


             SYD << tr("....MainWindow::toInputForm..DATAMAPID..***data.map.keys().count():|%1|")
                    .arg(data.map.keys().count());

            if ( buildEmail(data.map, mycurrent,data.map["id"]) ) {
                _currentjson = _currentjson.arg(" ");

                SYD << tr("............toInputForm YES_BUILDEMAIL_JSON:|%1|")
                       .arg(_currentjson);


            }
            else {
                SYD << tr("............toInputForm NO_BUILDEMAIL_JSON:|%1|")
                   .arg(_currentjson);
            }

            genjson = false;

     }
     else if (xml.indexOf("cargar_flujo_de_trabajo") > 0) {

         SYD << tr("Cargar flujo de trabajo...1...");
         results = parser.processXml(false,withpermises);
         if ( results.count() == 0) {
             SYE << tr("No se produjo el resultado esperado de la operación.");
             return QString("");

         }

         SYD  << tr("..............MainWindow::cargar_flujo_de_trabajo....results.at(0):|%1|")
                 .arg(results.at(0));


         ParsedSqlToData  data = SafetTextParser::parseSql(results.at(0),true);

         SYD  << tr("..............MainWindow::cargar_flujo_de_trabajo....operationName():|%1|")
                 .arg(parser.operationName());
         SYD  << tr("..............MainWindow::cargar_flujo_de_trabajo....data.map.keys():|%1|")
                 .arg(data.map.keys().count());

         if (data.map.contains("nombre")) {             QString oname = data.map["nombre"].section("/",-1);
             QString myname = mediaPath() + "/" + oname;

             QString mynewname = SafetYAWL::pathconf + "/" + "flowfiles" + "/" + oname;
             SYD << tr("...........MainWindow::cargar_flujo_de_trabajo....myname:|%1|").arg(myname);
             SYD << tr("...........MainWindow::cargar_flujo_de_trabajo....mynewname:|%1|").arg(mynewname);

             bool resultcopy = QFile::copy(myname,mynewname);

             if (resultcopy) {
                 SYD << tr("...........MainWindow::cargar_flujo_de_trabajo...copiado..(1)...");
                 QString myconsult = QString("operacion:agregar_flujo_de_trabajo nombre:%1")
                         .arg(mynewname);
                     toInputConfigure(myconsult);
                     return QString("Ok");
             }
             SYD << tr("...........MainWindow::cargar_flujo_de_trabajo...NO..copiado..(1)...");

             return QString("");
         }

         SYD  << tr("..............MainWindow::cargar_flujo_de_trabajo.....data....DATA (2)....");



     }
     else {
         SYD  << tr("..............MainWindow::toInputForm....*noenviarcorreo*..NO OK!");
         SYD  << tr("..............MainWindow::toInputForm....PROCESSING DEFAULT");
        results = parser.processXml(true,true);
     }

     if ( results.isEmpty() ) {
         if ( queryForErrors() ) {

                _currentjson = QString("{ \"id\": \"%1\", \"result\": \"%2\" } ")
             .arg(parser.currId())
             .arg("false"); 

			return QString("");
         }
     }

     if ( genjson ) {
        _currentjson = QString("{ \"id\": \"%1\", \"result\": \"%2\" } ")
             .arg(parser.currId())
             .arg("true");
     }

     QString mypostaction = parser.currentPostAction();
     SYD << tr(".........MainWindow::toInputForm................*RETURNING..postaction:|%1|")
            .arg(mypostaction);


     if ( !mypostaction.isEmpty() ) {


         QString currfile = SafetYAWL::pathconf + "/" + "templates" + "/" + mypostaction;
         SYD << tr("...........MainWindow::toInputForm...evaluating EVALPOSTACTION....file:|%1|")
                .arg(currfile);
         if (QFile::exists(currfile)) {
                mypostaction = SafetYAWL::readStringFromFile(currfile).constData();
                mypostaction.replace("\n"," ");

         }


         SYD  << tr(".................MainWindow::toInputForm....POSTACTION_APPEND (BEFORE)..."
                    "KEY_RETURNING...results.at(0)->:|%1|")
                 .arg(results.at(0));

         ParsedSqlToData  data = SafetTextParser::parseSql(results.at(0),true);

         SYD  << tr(".................MainWindow::toInputForm....POSTACTION_APPEND (AFTER)....*KEY_RETURNING....data.key:|%1|")
                 .arg(data.key);
         SYD  << tr(".................MainWindow::toInputForm........----->KEYVALUE_RETURNING....data.keyvalue:|%1|")
                 .arg(data.keyvalue);


         QString curremail;
         QRegExp rx(Safet::EMAIL_PATTERN);
         int pos = MainWindow::currentrealname.indexOf(rx);

         SYD << tr("....CURRREALNAME:|%1|").arg(MainWindow::currentrealname);


         if (pos == -1) {
             SYW << tr("Parece que no se encuentra configura su correo de usuario correctamente. "
                       "No se envia el correo. Verifique con el administrador");
             return QString("Ok");
         }

         curremail = rx.cap(0);

         mypostaction.replace("_EMAIL",curremail);
         mypostaction = mypostaction.arg(data.keyvalue);

         _plugs.push_back(mypostaction);
            SYD << tr(".........MainWindow::toInputForm................*RETURNING...mypostaction.append:|%1|")
                   .arg(mypostaction);

         SYD << tr(".........MainWindow::toInputForm................*RETURNING_OK");
     }


     return QString("Ok");

}



QString MainWindow::postAction() {
    if (_plugs.isEmpty()) {
        return QString("");
    }

    QString result  = _plugs.at(_plugs.count()-1);
    _plugs.pop_back();
    return result;

}

bool MainWindow::buildEmail(QMap<QString,QString>& data, const QString& cs,  QString& idkey) {



    SYD << tr("MainWindow::toInputForm.......ENTERING..evalConffileValues(1)");

              evalConffileValues();

   SYD << tr("MainWindow::toInputForm.........OUTING...evalConffileValues..(2)");


     QString mytemplate = SafetYAWL::getConf()["Email/template.1"];
     SYD << tr("...........MainWindow::toInputForm....PORCORREO TEMPLATING...loading email template:|%1|")
            .arg(mytemplate);


     QString newtemplate = parseForEmail(mytemplate,idkey,data,cs);

     //** Procesar template

     if (!data.contains("destinatarios") ) {

         SYD << tr(".....MainWindow::buildEmail............destinatarios NO CONTAIN");
         SYE << tr("El formato de los destinatarios no es correcta");

         _currentjson = QString("{ \"id\": \"%1\", \"result\": \"%2\",  \"error\": \"%3\"} ")
                  .arg("0")
                  .arg("false")
                 .arg(tr("Missing Recipients Email"));
         return false ;
      }

      QString mynewdata = QString(" \"recipients\":\"%1\", ").arg(data["destinatarios"]);
      mynewdata = mynewdata + " %1";
      _currentjson = _currentjson.arg(mynewdata);


      if (!data.contains("asunto") ) {

         SYD << tr(".....MainWindow::buildEmail............asunto NO CONTAIN");
         SYW << tr("No se ha incluido el asunto");
         data["asunto"] = "sin asunto";
         _currentjson = QString("{ \"id\": \"%1\", \"result\": \"%2\",  \"error\": \"%3\"} ")
                  .arg("0")
                  .arg("false")
                 .arg(tr("Missing Subject Email"));

         return false ;
      }

      mynewdata = QString(" \"subject\":\"%1\", ").arg(data["asunto"]);
      mynewdata = mynewdata + " %1";
      _currentjson = _currentjson.arg(mynewdata);


     SYD << tr(".....MainWindow::parseForEmail............**newtemplate:|%1|")
            .arg(newtemplate);


     data["destinatarios"].replace(Safet::BRACKETOPENMARK,"{");

     data["destinatarios"].replace(Safet::BRACKETCLOSEMARK,"}");
     QString recipients = parseForEmail(data["destinatarios"],idkey,data,cs);

     if (data.contains("adjuntos")) {
         data["adjuntos"] = parseForEmail(data["adjuntos"],idkey,data,cs);
         SYD << tr(".....MainWindow::parseForEmail............data[adjuntos]:|%1|")
                .arg(data["adjuntos"]);

     }
     mynewdata = QString(" \"attachments\":\"%1\", ").arg(data["adjuntos"]);
     mynewdata = mynewdata + " %1";
     _currentjson = _currentjson.arg(mynewdata);


     /** searching user **/

	 SYD << tr("........buildEmail...........CHECKINGFORTICKET....1...users.count():|%1|")
            .arg(users.count());
     SYD << tr("........buildEmail...........CHECKINGFORTICKET....1...recipients:|%1|")
            .arg(recipients);
     QString myuser;

     foreach(QString u, users.keys()) {
         QStringList myfields = users[u];
         if (myfields.count() < 3) {
             continue;
         }
          QString myfield = myfields.at(2);
          if (myfield.indexOf(recipients) != -1) {
              SYD << tr("...buildEmail...Setting user:|%1|")
                     .arg(myuser);
              myuser = u;
              break;
          }


     }
     SYD << tr("........buildEmail...........CHECKINGFORTICKET....2");
     QString checkuser = SafetYAWL::getConf()["Email/check.user"];

     SYD << tr("........buildEmail...........CHECKINGFORTICKET....checkuser:|%1|")
            .arg(checkuser);

     QString myticket;

     if (checkuser != "no") {

         if (myuser.isEmpty()) {
             SYE << tr("Usuario Desconocido para enviarle un correo");

             _currentjson = QString("{ \"id\": \"%1\", \"result\": \"%2\",  \"error\": \"%3\""
                                    "\"datetime\":\"%4\", \"datetime_string\":\"%5\" ")
                      .arg("0")
                      .arg("false")
                     .arg(tr("User email unknown")
                     .arg(QDateTime::currentDateTime().toTime_t()))
                     .arg(QDateTime::currentDateTime().toString(Safet::DateFormat_secs));


             return false;
         }
         SYD << tr("........buildEmail...........CHECKINGFORTICKET....3");


         //QString myticket = MainWindow::genTicket(recipients.section("@",0,0));
         myticket = MainWindow::genTicket(myuser);
     }

	
     /** searching user **/

     newtemplate.replace("_SAFETTICKET",myticket);

     SYD << tr(".....MainWindow::buildEmail............SAFETRECIPIENTS:|%1|")
            .arg(recipients);
     SYD << tr(".....MainWindow::buildEmail............SAFETNEWTEMPLATE:|%1|")
            .arg(newtemplate);

     SYD << tr(".....MainWindow::buildEmail............destinatarios:|%1|")
            .arg(data["destinatarios"]);

     SYD << tr(".....MainWindow::buildEmail............RECIPIENTS...recipients:|%1|")
            .arg(recipients);

     SYD << tr(".....MainWindow::buildEmail.........SAFETASUNTO...asunto:|%1|")
            .arg(data["asunto"]);


     /** Parseando guardado de email **/
     mynewdata = QString(" \"template\":\"%1\", ").arg(newtemplate);
     mynewdata = mynewdata + " %1";
     _currentjson = _currentjson.arg(mynewdata);

     QString savesql;
     if ( data.contains("base")) {
     SYD << tr(".....MainWindow::buildEmail.........SAFETBASE...base:|%1|")
            .arg(data["base"]);


         QString mydb = data["base"];
         QStringList mypars = mydb.split("__", QString::SkipEmptyParts);


         SYD  << tr("......buildEmail...SAVEBD:..mypars.count():%1")
                 .arg(mypars.count());
         if (mypars.count() >= 2 ) {
             QString notetable = mypars.at(0);
             QString notefield = mypars.at(1);
	     QString notekeyfield = "last_notification";
	
	     if (mypars.count() >= 3) {
			notekeyfield = mypars.at(2);
	     }

             savesql = QString("UPDATE %1 SET %2='%4' WHERE id = {#%3};")
                     .arg(notetable)
                     .arg(notefield)
		     .arg(notekeyfield)
                     .arg("%1");
             SYD  << tr("......buildEmail...**SAVEBD...1:..savesql:\"%1\"")
                     .arg(savesql);
             savesql = parseForEmail(savesql,idkey,data,cs);
             SYD  << tr("......buildEmail...**SAVEBD...2:..savesql:\"%1\"")
                     .arg(savesql);

         }
     }

    bool result =  sendEmail(recipients,data,newtemplate,savesql);

    SYD << tr("..........sendEmail...result:|%1|")
           .arg(result);

    return result;


}


QString MainWindow::parseForEmail(const QString& t, const QString& idkey, QMap<QString,QString>& data,
                                  const QString& cs) {
    int nargs;
    char** argv =NULL;
    QCoreApplication myapp(nargs,argv);
    QScriptEngine myEngine;

    QString mycs = "(function(fname) { return \"\"; })";


    if (!cs.isEmpty())  {
        mycs =     QString("(function(fname) { mydata = %1; try { return mydata[0][fname]; } catch(err) { return \"\" }})")
            .arg(cs);

    }

    SYD  << tr("..............MainWindow::parseForEmail.... mycs:|%1|")
            .arg(mycs);


    QString newtemplate = t;
    QString mytemplate = t;
    QRegExp rx;
    rx.setPattern("\\{#([a-zA-Z0-9_\\-:]+)(\\{(\\d+)\\})?\\}");
    int pos = 0;
    SYD << tr("...........MainWindow::parseForEmail....pos:|%1|...mytemplate.length():|%2|")
           .arg(pos)
           .arg(mytemplate.length());
    QString currvalue;
    while( pos < mytemplate.length()) {
        QScriptValue myfun;
        QScriptValue myvalue;
        QScriptValueList args;

        int newpos = rx.indexIn(mytemplate,pos);
        SYD << tr("...........MainWindow::parseForEmail..............newpos:|%1|")
               .arg(newpos);
        if (newpos == -1 ) {
            break;
        }

        QString myfield  =  rx.cap(1);


        SYD << tr("\n\n.....MainWindow::parseForEmail ...FIELDNAME:|%1|")
               .arg(myfield);


        QStringList mypars = myfield.split(":",QString::SkipEmptyParts);

        myfield  = mypars.at(0);

        QString mycsfield  = myfield;


        SYD << tr(".....MainWindow::buildEmail............*NUMBER:|%1|")
               .arg(rx.cap(2));

        QString mynumber = rx.cap(3);
        SYD << tr(".....MainWindow::buildEmail............*ONLY...NUMBER:|%1|")
               .arg(rx.cap(3));
        if (!mynumber.isEmpty()) {
            myfield = QString("%1{%2}")
                    .arg(myfield)
                    .arg(mynumber);
        }

        pos = newpos + myfield.length();
        args.append(mycsfield);

        try  {

           myfun = myEngine.evaluate(mycs);
           myvalue = myfun.call(QScriptValue(),args);
           currvalue = myvalue.toString();
           if ( currvalue == "undefined") {
               SYD << tr(".....MainWindow::buildEmail..........FIELDNAME:|%1|.No!.->UNDEFINED<--...")
                      .arg(myfield);
               pos = newpos + myfield.length();
               continue;
           }

        }
        catch(...) {
            SYE << tr("IMPORTANTE:Ocurrió un error (excepcion) al evaluar el script (two)");
            break ;
        }
        SYD << tr(".....MainWindow::buildEmail............FIELDNAME:|%1| : Yes! VALUE:|%2|")
               .arg(myfield)
               .arg(currvalue);

        data[ myfield ] = currvalue;

        if (mypars.count() > 1 ) {
            bool ok;
           SYD << tr("MYPARS > 1**");
            QDateTime mydate = QDateTime::fromString(data[ myfield ],Safet::DateFormat);
            if (mydate.isValid()) {

                SYD << tr("MYPARS > 1**....mypars.at(1):|%1|")
                       .arg(mypars.at(1));
                mydate = mydate.addSecs(mypars.at(1).toInt(&ok));

                data[ myfield ] = mydate.toString(Safet::DateFormat);

                SYD << tr("MYPARS > 1**....newdate:|%1|")
                       .arg(data[myfield]);
            }


        }

    }
    SYD << tr(".....\n");





    replaceDocumentid(newtemplate, idkey);

    foreach(QString key, data.keys()) {

       SYD << tr(".....MainWindow::buildEmail......KEY:|%1|...VALUE:|%2|")
              .arg(key)
              .arg(data[key]);

       QRegExp rx("([a-zA-Z0-9_\\-\\:\\+]+)(\\{(\\d+)\\})");
       //QRegExp rx("\\{#([a-zA-Z0-9_\\-\\:\\+]+)(\\{(\\d+)\\})?\\}");
       if (rx.exactMatch(key)) {
           SYD << tr(".......MainWindow::buildEmail exactMatch");
           bool ok;
           QString mynumber = rx.cap(3);
           SYD << tr(".......MainWindow::buildEmail exactMatch....mynumber:|%1|").arg(mynumber);
           uint pos = 0;
           if (!mynumber.isEmpty()) {
                pos = rx.cap(3).toUInt(&ok);
           }
           SYD << tr(".......MainWindow::buildEmail exactMatch....pos:|%1|")
                  .arg(pos);
           QStringList  mylist = data[key].split(",",QString::SkipEmptyParts);
           if (pos < mylist.count()) {
               QString myvalue = mylist.at(pos);
               newtemplate.replace(QString("{#%1}").arg(key),
                               myvalue);
           }
           else {
               SYW << tr("La  posición \"%1\" del parámetro \"%2\" no es válida")
                      .arg(pos)
                      .arg(key);
           }

       }
       else  {
           SYD << tr(".......MainWindow::buildEmail NORMAL");
           QString myvalue = data[key];
               newtemplate.replace(QRegExp(QString("\\{#%1[:\\d]*\\}").arg(key)),
                           myvalue);

       }
    }


    newtemplate.replace(QRegExp("(<RETURN/>)|(\\*\\*RETURN\\*\\*)"),"\n");


    return newtemplate;
}

int MainWindow::numberOfDocuments(const QString& t, const QString& idkey,
                                  QStringList& currvalues) {

    QRegExp rx;
    SYD << tr("............MainWindow::numberOfDocuments.....(1)...");
    rx.setPattern("\\{#safetdocumentid:([a-zA-Z0-9_\\-:]+)\\}");

    int pos = t.indexOf(rx);

    if (pos == -1) {

        SYD << tr("MainWindow::numberOfDocuments.......No se encontro DOCUMENTID en t:|%1|")
               .arg(t);
        return false;
    }

    SYD << tr("............MainWindow::numberOfDocuments.....(2)...");


    QStringList mylist = rx.cap(1).split(":");

    if (mylist.count() < 2 ) {

        SYW << tr("La etiqueta de DocumentID no se encuentra correctamente escrita en el archivo de configuraciòn safet.conf");
        return false;
    }

    QString  filename = SafetYAWL::pathconf + "/flowfiles" + "/" + mylist.at(0) +".xml";
    if (!QFile::exists(filename)) {

        SYW << tr("No es posible leer el archivo \"%1\" para colocar la etiqueta documentid")
               .arg(filename);
            return false;
    }
    QString  varname = mylist.at(1);


    SYD << tr("............MainWindow::numberOfDcouments.....filename:|%1|")
           .arg(filename);

    if (MainWindow::configurator != NULL ) {
        delete  MainWindow::configurator;
        MainWindow::configurator = NULL;
    }

    int dcount = 0;
    MainWindow::configurator = new SafetYAWL();
    MainWindow::configurator->openXML(filename);
    MainWindow::configurator->convertXMLtoObjects();
    MainWindow::configurator->openDataSources();


     evalConffileValues();

    SYD << tr("............MainWindow::numberOfDocuments.......CHECKQUOTES...(3)...");

        QList<QSqlField> fields;
    SafetDocument mydoc = MainWindow::configurator->getWorkflows().at(0)->getDocuments(varname,
                                                                                    fields, dcount,
                                                                                    SafetWorkflow::JSON, idkey);

    int result = dcount;



    SYD << tr("........MainWindow::numberOfDocuments....***result...dcount:|%1|")
           .arg(dcount);

    SYD << tr("............MainWindow::numberOfDocuments.....CSDOCUMENTS....mydoc.documents():|%1|")
           .arg(mydoc.documents());

    QString cs = "[ " +mydoc.documents() + " ]";

    // Obtener la clave ----
    int nargs;
    char** argv =NULL;
    QCoreApplication myapp(nargs,argv);
    QScriptEngine myEngine;

    for (int i=0; i < dcount; i++) {


        QString mycs = QString("(function(fname) { mydata = %1; try { return mydata[%2][fname]; } catch(err) { return \"\" }})")
                .arg(cs)
                .arg(i);


        QScriptValue myfun;
        QScriptValue myvalue;
        QScriptValueList args;


        args.append("id");
        QString currvalue = "";

        try  {

            myfun = myEngine.evaluate(mycs);
            myvalue = myfun.call(QScriptValue(),args);
            currvalue = myvalue.toString();
            if ( currvalue == "undefined") {
                SYD << tr(".....MainWindow::numberOfDocuments..........FIELDNAME:|%1|.No!.->UNDEFINED<--...")
                       .arg("id");
                SYE << tr("Ocurrio un error al contar el numero de documentos, no se encuentra el campo \"id\"");
                currvalue = "";
            }

            currvalues.append(currvalue);

        }
        catch(...) {
            SYW << tr("IMPORTANTE:Ocurrió un error (excepcion) al evaluar el script (two)");
        }
        SYD << tr(".....MainWindow::numberOfDocuments..........ID_currvalue:|%1|")
               .arg(currvalue);

    }

    return result;
}


bool MainWindow::replaceDocumentid(QString& t, const QString& idkey) {

    QRegExp rx;
    rx.setPattern("\\{#safetdocumentid:([a-zA-Z0-9_\\-:]+)\\}");

    int pos = t.indexOf(rx);

    if (pos == -1) {

        SYD << tr("No se encontro DOCUMENTID en t:|%1|")
               .arg(t);
        return false;
    }

    SYD << tr("............MainWindow::replaceDocumentid.....(1)...");


    QStringList mylist = rx.cap(1).split(":");

    if (mylist.count() < 2 ) {

        SYW << tr("La etiqueta de DocumentID no se encuentra correctamente escrita en el archivo de configuraciòn safet.conf");
        return false;
    }

    QString  filename = SafetYAWL::pathconf + "/flowfiles" + "/" + mylist.at(0) +".xml";
    if (!QFile::exists(filename)) {

        SYW << tr("No es posible leer el archivo \"%1\" para colocar la etiqueta documentid")
               .arg(filename);
            return false;
    }
    QString  varname = mylist.at(1);


    SYD << tr("............MainWindow::replaceDocumentid.....filename:|%1|")
           .arg(filename);

    if (MainWindow::configurator != NULL ) {
        delete  MainWindow::configurator;
        MainWindow::configurator = NULL;
    }

    int dcount = 0;
    MainWindow::configurator = new SafetYAWL();
    MainWindow::configurator->openXML(filename);
    MainWindow::configurator->convertXMLtoObjects();
    MainWindow::configurator->openDataSources();

     evalConffileValues();

    SYD << tr("............MainWindow::replaceDocumentid.....(2)...");

        QList<QSqlField> fields;
    SafetDocument mydoc = MainWindow::configurator->getWorkflows().at(0)->getDocuments(varname,
                                                                                    fields,dcount,
                                                                                    SafetWorkflow::JSON, idkey);



//    QList<SafetWorkflow::InfoSigner> infosigners = MainWindow::configurator->getWorkflows()
//            .at(0)->getSignersDocument(myvar,idkey,derror);


         QString derror;

     SafetVariable* myvar = MainWindow::configurator->getWorkflows().at(0)->searchVariable(varname);


     if (myvar == NULL ) {
         SYW << tr("La variable \"%1\" no se encuentra en el flujo para la etiqueta de documentid")
                .arg(varname);
         return false;
     }

     QList<SafetWorkflow::InfoSigner> infosigners = MainWindow::configurator->getWorkflows()
             .at(0)->getSignersDocument(myvar,idkey,derror);

     QString documents = mydoc.documents();
         SYD << tr("............MainWindow::replaceDocumentid.....documents:|%1|")
                .arg(documents);

     long fieldno = 0;

     QString documentid;

         SYD << tr("............MainWindow::replaceDocumentid.....(3)...**");
    myvar->getXMLDocument(idkey,fieldno, documentid);

    QString newdocumentid = documentid;

    newdocumentid.chop(5);

    SYD << tr("............MainWindow::replaceDocumentid...OBTENERDOCUMENTID..documentid:|%1|")
           .arg(documentid);

    t.replace(rx.cap(0),newdocumentid + ".html");

    InflowFirmaDoc dlg(NULL);


    QString _cursigners;
    dlg.loadSignersInfo(infosigners);
    dlg.generateJScriptreports(documents,fields,1,_cursigners, infosigners);
    bool ok;
    SYD << tr("......MainWindow::replaceDocumentid............signers:\n|%1|").arg(dlg.currentSigners());

    SYD << tr("......MainWindow::replaceDocumentid....................InflowFirmaDoc...(3)");
    dlg.executeJSCodeAfterLoad(ok);
    SYD << tr("......MainWindow::replaceDocumentid....................InflowFirmaDoc...(5)");
    dlg.loadReportTemplate(documentid, newdocumentid + ".html");
    dlg.savepdf();



    SYD << QString("...MainWindow::toInputForm....getDocuments...(After) fields.count():|%1|")
           .arg(fields.count());

    return true;
}




bool MainWindow::sendEmail(const QString& recipients, const QMap<QString,QString>& data, const QString& message,
                            QString &savesql) {
    SYD << tr(".............MainWindow::sendEmail...................(BEGIN)....");
#ifdef SAFET_SMTP

    QString  subject = data["asunto"];

    SYD << tr(".............MainWindow::sendEmail...................(BEGIN**)....");
    QString myhost = SafetYAWL::getConf()["Email/host"];
    if  (myhost.isEmpty()) {
        myhost = "localhost";
    }
    SYD << tr(".........senEmail...ATTACHDIR...myhost:|%1|")
           .arg(myhost);

    QString myport = SafetYAWL::getConf()["Email/port"];
    if  (myport.isEmpty()) {
        myport = "25";
    }

    QString attachdir = SafetYAWL::getConf()["Email/attachment.dir"];
    if  (attachdir.isEmpty()) {
        attachdir = SafetYAWL::pathconf + "/" + "attachments";
    }

    SYD << tr(".........senEmail...ATTACHDIR...attachdir:|%1|")
           .arg(attachdir);


    bool ok;

    int currport = myport.toInt(&ok);
    SYD << tr(".........senEmail...ATTACHDIR...currport:|%1|")
           .arg(currport);

  //  SmtpClient smtp(myhost,currport, SmtpClient::TcpConnection);
    SmtpClient smtp(myhost,currport, SmtpClient::TlsConnection);

    SYD << tr("sendEmail EMAIL::TLS");

    QString myuser = SafetYAWL::getConf()["Email/user"];
    if  (myuser.isEmpty()) {
        myuser = "vbravo";
    }
    QString mypass = SafetYAWL::getConf()["Email/password"];
    if  (mypass.isEmpty()) {
        mypass = "4n0d0";
    }

    SYD << tr(".............MainWindow::sendEmail...................(BEGIN***)....");
    SYD << tr("..........MainWindow::sendEmail....SENDEMAILLOCALHOST .user:|%1|")
           .arg(myuser);

    SYD << tr("..........MainWindow::sendEmail....SENDEMAILLOCALHOST .pass:|%1|")
           .arg(mypass);

    smtp.setUser(myuser);
    smtp.setPassword(mypass);

    int ishost = smtp.connectToHost();
    SYD << tr("..........MainWindow::sendEmail....SENDEMAILL.....ishost:|%1|")
           .arg(ishost);

    if (!ishost) {


        _currentjson = QString("{ \"id\": \"%1\", \"result\": \"%2\",  \"error\": \"%3\""
                               "\"datetime\":\"%4\", \"datetime_string\":\"%5\" ")
                 .arg("0")
                 .arg("false")
                .arg(tr("No connection to Host")
                .arg(QDateTime::currentDateTime().toTime_t()))
                .arg(QDateTime::currentDateTime().toString(Safet::DateFormat_secs));

        return false;
    }

        int islogin = smtp.login();
        SYD << tr("..........MainWindow::sendEmail....SENDEMAIL .islogin:|%1|")
               .arg(islogin);

        if (!islogin) {
            _currentjson = QString("{ \"id\": \"%1\", \"result\": \"%2\",  \"error\": \"%3\""
                                   "\"datetime\":\"%4\", \"datetime_string\":\"%5\" ")
                     .arg("0")
                     .arg("false")
                    .arg(tr("No login to Host")
                    .arg(QDateTime::currentDateTime().toTime_t()))
                    .arg(QDateTime::currentDateTime().toString(Safet::DateFormat_secs));



            return false;
        }




        QStringList mylist = recipients.split(QRegExp(",|;|\\s"),QString::SkipEmptyParts);

        QString hostsender = SafetYAWL::getConf()["Email/hostsender"];
        if  (hostsender.isEmpty()) {
            hostsender = "root@proxy1.cenditel.gob.ve";
        }

        QString namesender = SafetYAWL::getConf()["Email/namesender"];
        if  (namesender.isEmpty()) {
            namesender = tr("Weetup - 2016");
        }

    foreach(QString email, mylist) {
        MimeMessage emessage;

        emessage.setSender(new EmailAddress(hostsender, namesender));
        SYD << tr("..........MainWindow::sendEmail....SENDEMAIL.....email:|%1|")
               .arg(email);

        QString user = email.mid(0,email.indexOf("@"));

        SYD << tr("..........MainWindow::sendEmail....SENDEMAIL.....user:|%1|")
               .arg(user);
        QStringList infolist = getInfoOfUser(user);

        int ninfo = infolist.count();
        SYD << tr("..........MainWindow::sendEmail....SENDEMAIL..*ADDHEADERS..infolist.count():|%1|")
               .arg(ninfo);
        QString mytext = message;
        mytext.replace("_USERNAME", SafetYAWL::currentAuthUser());
        mytext.replace("_REALNAME",MainWindow::currentrealname);

        if (ninfo > 2 ) {
            QString myname = infolist.at(2);
            QRegExp  rx(Safet::EMAIL_PATTERN);
            myname.indexOf(rx);
            QString email = rx.cap(0);
            myname.replace(email,"");
            mytext.replace("{#usuario}",myname.trimmed());
            mytext.replace("{#email}",email);
        }
        else {
            mytext.replace("{#usuario}",SafetYAWL::getConf()["Email/template.title"]);
        }

        emessage.addRecipient(new EmailAddress(email));
        emessage.setSubject(subject);

        MimeText text;


            //text.setHeader();
            //text.addHeaderLine("MIME-Version: 1.0");
            //text.addHeaderLine("Content-type: text/html;charset=utf-8");

            text.setContentType("text/html");
            text.setText(mytext);

            if (!savesql.isEmpty()) {

                SYA << tr("....sendEmail...saving Email on database..initializing");
                SYD << tr("....sendEmail...saving Email on SAVEDB...database..initializing...savesql 1:\"%1\"")
                       .arg(savesql);

		QString esctext = mytext;
		esctext.replace("'","''");
                savesql = savesql.arg(esctext);

                SYD << tr("....sendEmail...saving Email SAVEDB...ESCAPESQL..on database..initializing...savesql 2:\"%1\"")
                       .arg(savesql);

                QSqlQuery query( SafetYAWL::currentDb );
                QString command = savesql;

                query.prepare(  command );
               bool executed = query.exec();
               if (!executed ) {
                    SYE << tr("Problema al ejecutar consulta de guardado de correo: \"%1\" ")
                           .arg(command);
                }
               SYA << tr("....sendEmail...saving Email on database..result: %1")
                      .arg(executed?"Correcto":"No");


            }

            emessage.addPart(&text);
            SYD << tr(".......sendmail...ATTACHDIR...adjuntos...1");

            if (data.contains("adjuntos")) {
                SYD << tr(".......sendmail...ATTACHDIR...adjuntos...2...adjuntos:|%1|")
                       .arg(data["adjuntos"]);
                QStringList myattachs = data["adjuntos"].split(QRegExp("\\s+"),QString::SkipEmptyParts);


                SYD << tr(".......sendmail...ATTACHDIR...adjuntos...myattachs.count():|%1|")
                       .arg(myattachs.count());





                foreach(QString myattach, myattachs) {
                    QString mypath = attachdir + "/" + myattach;

                    if (!QFile::exists(mypath)) {
                        SYE << tr("No existe el archivo \"%1\" para ser enviado adjunto por correo")
                               .arg(mypath);
                        continue;
                    }

                    QDir::setCurrent(attachdir);
                    SYD << tr(".......sendEmail...ATTACHDIR...attachdir:|%1|")
                           .arg(attachdir);
                    SYD << tr(".......sendEmail...ATTACHDIR...myattach:|%1|")
                           .arg(myattach);

                    SYD << tr("attach...1");

                    SYD << tr("attach...2");
                        // the file type can be setted. (by default is application/octet-stream)



                    emessage.addPart(new MimeAttachment(new QFile(myattach)));
                     SYD << tr("attach..3.....myattach:|%1|").arg(myattach);


                }
                SYD << tr("attach..4");


            }
            SYD << tr("attach..5");

            smtp.sendMail(emessage);
            if (ishost ) {
                SYA << tr("...........MainWindow::sendEmail....email:|%1|..subject:|%2|.............SENDING_OK\n")
                       .arg(email)
                       .arg(subject);
//                _currentjson = QString("{ \"id\": \"%1\", \"result\": \"%2\"} ")
//                         .arg("1")
//                         .arg("true");

                QDateTime now = QDateTime::currentDateTime();
                QString mynewdata = QString(" \"datetime\":\"%1\", ").arg(now.toTime_t());
                mynewdata = mynewdata + " %1";
                _currentjson = _currentjson.arg(mynewdata);

                mynewdata = QString(" \"datetime_string\":\"%1\", ").arg(now.toString(Safet::DateFormat_secs));
                mynewdata = mynewdata + " %1";
                _currentjson = _currentjson.arg(mynewdata);

            }
            else {
                SYA << tr("...........MainWindow::sendEmail....email:|%1|..subject:|%2|.............SENDING_NO\n")
                       .arg(email)
                       .arg(subject);
                _currentjson = QString("{ \"id\": \"%1\", \"result\": \"%2\",  \"error\": \"%3\"} ")
                         .arg("0")
                         .arg("false")
                        .arg(tr("No send mail"));
                return false;

            }
    }

          smtp.quit();
  #endif

          SYD << tr(".............MainWindow::sendEmail...................(END)....");
          return true;


}

QString MainWindow::getFlowParameters(const QString& flowfilename){
    QString result;
    SYD << tr("MainWindow::getFlowParameters: %1...._currconfpath:%2")
                  .arg(flowfilename)
                  .arg(_currconfpath);


    if ( configurator != NULL ) {
         delete configurator;
         configurator = new SafetYAWL(_currconfpath+ "/" + Safet::datadir);
    }

    configurator->setAutofilters( commands['a']);
    configurator->setRecursivefilters( commands['r']);
    SafetYAWL::streamlog.initAllStack();
    configurator->openXML(flowfilename);

    if (queryForErrors()) {
        //delete configurator;
        qDebug("...retornando.....executedParsed");
        return QString("Ocurrió un error inesperado. Consulte al administrador");
    }

    configurator->openDataSources();

    configurator->convertXMLtoObjects();
    qDebug(".....configurator->convertXMLtoObjects();....(1)...");



//           bool showdlgpar = SafetYAWL::getConf()["GeneralOptions/parameters.showdialog"]
//                             .compare("on",Qt::CaseSensitive) == 0;

    SafetWorkflow* mywf = configurator->getWorkflow();


    if (mywf != NULL ) {
        qDebug();
        int n = mywf->getParameterlist().count();
        _wfnumpars = 0;
        for(int i=0; i < n; i++) {
            SafetParameter *p = mywf->getParameterlist().at(i);
            Q_CHECK_PTR(p);
            result += QString("%1: ")
                    .arg(p->title());
            result += QString("%1<br/>\n").arg(getWidget(p));

        }
        _wfnumpars  = 0;

    }

    SYD << tr("...MainWindow::getFlowParameters....result:\n\n%1\n\n")
           .arg(result);
    return result;
}


QString MainWindow::getWidget(SafetParameter* p) {
    QMap<QString,QVariant> myconf;
    CmdWidget* mywidget = NULL;
    Q_CHECK_PTR( p );
               SYD << tr("getWidget: title:%1")
                  .arg(p->title());

    if ( !p->options().isEmpty()) {
        myconf.insert("options",p->options());
    }
    if ( !p->path().isEmpty()) {
        myconf.insert("keyvalue",p->path());
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


    QString mytype = p->type();
    DomModel::TypeField enumtype = DomModel::typeFieldFromString(mytype);

    QObject* parent = NULL;
    QString defaultvalue;
    defaultvalue = p->defaultvalue();
    QString currid;
    if (!p->configurekey().isEmpty()) {
        currid = QString("configurekey.%1")
                .arg(p->configurekey());
    }
    else {
        currid = QString("parameters.%1")
                .arg( _wfnumpars );
           _wfnumpars++;
    }

    switch (enumtype) {
    case DomModel::FileName:
        mywidget = new GetFileWidget(currid,parent,false);
        Q_CHECK_PTR( mywidget );
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        // mywidget->setValidator( validator);
        if (!defaultvalue.isEmpty()) {
//            mywidget->setText(defaultvalue);
        }
        return mywidget->html();
    case DomModel::DateTime:
        mywidget = new DateTimeWidget(currid, parent,false);
        Q_CHECK_PTR( mywidget );
        mywidget->setConf(myconf);
        if (!defaultvalue.isEmpty()) {

        }

        return mywidget->html();
    case DomModel::Output:
        mywidget = new OutputWidget(currid, parent,false);
        Q_CHECK_PTR( mywidget );
        mywidget->setConf(myconf);
        if (!defaultvalue.isEmpty()) {

        }
        return mywidget->html();

    case DomModel::String:
        mywidget = new CmdWidget(currid, NULL,false);
        mywidget->setConf(myconf);
        Q_CHECK_PTR( mywidget );
        mywidget->buildWidget();
        //  mywidget->setValidator( validator);
//        if (!defaultvalue.isEmpty()) {
//            mywidget->setText(defaultvalue);
//        }
        return mywidget->html();
    case DomModel::Suggest:
        mywidget = new SuggestWidget(currid, NULL,false);
        mywidget->setConf(myconf);
        Q_CHECK_PTR( mywidget );
//        mywidget->buildWidget();
        // mywidget->setValidator( validator);
        if (!defaultvalue.isEmpty()) {
//            mywidget->setText(defaultvalue);
        }

        return mywidget->html();

    case DomModel::Number:
        mywidget = new NumberWidget(currid, NULL, false);
        mywidget->setConf(myconf);
        Q_CHECK_PTR( mywidget );
//        mywidget->buildWidget();
        // mywidget->setValidator( validator);
        if (!defaultvalue.isEmpty()) {
//            mywidget->setText(defaultvalue);
        }

        return mywidget->html();
    case DomModel::StringBox:
        mywidget = new TextEditWidget(currid, NULL,false);
        Q_CHECK_PTR( mywidget );
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        // mywidget->setValidator( validator);
        if (!defaultvalue.isEmpty()) {
//            mywidget->setText(defaultvalue);
        }

        return mywidget->html();

     case DomModel::ComboListTable:
        qDebug("....combotable.....(1)...");
        mywidget = new ComboWidget(currid,ComboWidget::ListTable, NULL,false);
        qDebug("....combotable.....(2)...");
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        qDebug("....combotable.....(3)...");

        return mywidget->html();
     case DomModel::ComboListLiteral:
        mywidget = new ComboWidget(currid,ComboWidget::ListLiteral, parent);
        mywidget->setConf(myconf);
        mywidget->buildWidget();

        return mywidget->html();
     case DomModel::ComboColor:
        mywidget = new ComboWidget(currid,ComboWidget::ColorSafet, parent,false);
        mywidget->setConf(myconf);
        mywidget->buildWidget();

        return mywidget->html();
        break;
     case DomModel::ComboConffile:
     case DomModel::ListConffile:
        if (   enumtype  == DomModel::ComboConffile ) {
            mywidget = new ComboWidget(currid,ComboWidget::ConffileSafet, parent,false);
        }
        else {
//            mywidget = new ListWidget(currid,ListWidget::ConffileSafet, parent);

        }

        mywidget->setConf(myconf);
        mywidget->buildWidget();

        return mywidget->html();
     case DomModel::ListTable:
//        mywidget = new ListWidget(currid,ListWidget::ListTable, parent,false);
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        qDebug("ListTable...mywidget->setConf(myconf)...(2)...");
        return mywidget->html();
     case DomModel::ListLiteral:
//        mywidget = new ListWidget(currid,ListWidget::ListLiteral, parent,false);
        mywidget->setConf(myconf);
        mywidget->buildWidget();
        qDebug("ListLiteral...mywidget->setConf(myconf)...(2)...");
        return mywidget->html();
        default:
            mywidget = NULL;
    }


    if (mywidget != NULL ) {
        return mywidget->html();
    }

}


QString MainWindow::replaceMarks(const QString& s) {
    QString result = s;

    result.replace(Safet::COLONMARK,":");
    result.replace(Safet::COLONMARK_,":");
    result.replace(Safet::AMPMARK_,"&amp;");
    result.replace(Safet::PIPEMARK_,"|");
    result.replace(Safet::COMMAMARK,",");

    result.replace("Safet::aacute","á");
    result.replace("Safet::eacute","é");
    result.replace("Safet::iacute","í");
    result.replace("Safet::oacute","ó");
    result.replace("Safet::uacute","ú");
    result.replace("Safet::ntilde","ñ");
    result.replace("Safet::Ntilde","Ñ");
    return result;
}

void MainWindow::checkDefaultValuesParameters() {
    SYD << tr("...checkDefaultValuesParameters....1");
    if (MainWindow::configurator == NULL ) {
        SYD << tr("...checkDefaultValuesParameters....configurator is NULL");
        return;
    }
    SafetWorkflow* mywf = MainWindow::configurator->getWorkflows().at(0);
    if (mywf == NULL) {

        SYD << tr("...checkDefaultValuesParameters....wf is NULL");
        return;
    }


    foreach(SafetParameter* par, mywf->getParameterlist()) {

        if  (!_currparsvalues.keys().contains(par->title())) {
             if (!par->defaultvalue().isEmpty()) {
                 _currparsvalues[par->title()] = par->defaultvalue();
                 SYD << tr("....extractParameters...ADDING_PAR_DEFAULTVALUE:|%1| |%2|")
                        .arg(par->title())
                        .arg(par->defaultvalue());
             }

        }

    }


}

QString MainWindow::extractParameters(const QString& action) {
    QString result = action;

    SYD << tr("...MainWindow::extractParameters...action:|%1|")
           .arg(action);



    QRegExp rxpars("(parameters\\.)([a-zA-Z0-9_\\-\\./]+)\\s*:\\s*"
                   "([a-zA-Z_0-9áéíóúñÑ'\\*=\\.\\-\\(\\),;%#\\x3c\\x3e\\x2f\\[\\]/\\?\\s]+)");
    QRegExp rxconf("(configurekey\\.)([a-zA-Z0-9_\\-\\./]+)\\s*:\\s*([a-zA-Z_0-9áéíóúñÑ'\\*=\\.\\-\\(\\),;%#\\x3c\\x3e\\x2f\\[\\]/\\s\\{\\}:\\?]+)");
    QMap<QString,QString> mypars;
    QMap<QString,QString> myconfigs;

    // para configuraciones
    int pos = 0;
    while (pos < action.length()) {
        pos = action.indexOf(rxconf,pos);
        SYD << tr("...MainWindow::extractParameters...CONFIGUREKEY..EXTRACT..config pos:|%1|")
               .arg(pos);

        if (pos==-1) {
            break;
        }
        QString myfield  = rxconf.cap(2);
        QString myvalue  = rxconf.cap(3);

        SYD << tr("...MainWindow::extractParameters...config key:|%1|")
               .arg(myfield);


        SYD << tr("...MainWindow::extractParameters...config value:|%1|\n")
               .arg(myvalue);

        myconfigs[myfield] = myvalue;
        pos += rxconf.cap(0).length()+1;
        result.replace(rxconf.cap(0),"");

    }


    SYD << tr("...MainWindow::extractParameters...AFTER_CONFIGS_action:|%1|")
           .arg(result);

    //QStringList parlist = action.split(QRegExp("((parameters)|(configurekey))."),QString::SkipEmptyParts);
    QStringList parlist = result.split("parameters.",QString::SkipEmptyParts);
    SYD << tr("...MainWindow::extractParameters...PARLIST...*parlist.count()):|%1|")
           .arg(parlist.count());

    if (parlist.count() > 1) {
          parlist.removeFirst();
    }

    SYD << tr("...MainWindow::extractParameters...PARLIST...*parlist.count())...2:|%1|")
        .arg(parlist.count());

    pos = 0;

    foreach(QString par, parlist) {

        pos = 0;
        QString newpar = "parameters."+par;
        SYD << tr("...MainWindow::extractParameters...*PARLIST...par:|%1|")
               .arg(newpar);

        while (pos < newpar.length()) {
            pos = newpar.indexOf(rxpars,pos);
            if (pos==-1) {
                break;
            }
            QString myfield = rxpars.cap(2);
            QString myvalue = replaceMarks(rxpars.cap(3));

            SYD << tr("....par field:|%1|")
                    .arg(myfield);
            SYD << tr("....par field:|%1|\n")
                    .arg(myvalue);

            mypars[myfield] = myvalue;
            pos += rxpars.cap(0).length()+1;
            result.replace(rxpars.cap(0),"");

        }
    }
    SYD << tr("...MainWindow::extractParameters...PARLIST...AFTER_PARS...par....result:|%1|")
           .arg(result);

    SYD << tr("...MainWindow::extractParameters...mypars.count()):|%1|")
           .arg(mypars.count());


    if (mypars.count()> 0 ) {
        setParsValues(mypars);
    }
    if (myconfigs.count() > 0 ) {
        SYD << tr("...MainWindow::extractParameters...SETTING CONFIGS....");
        setConffileValues(myconfigs);
    }
    SYD << tr("...MainWindow::extractParameters...myconfigs.count():|%1|")
           .arg(myconfigs.count());




    return result;
}

bool  MainWindow::toInputConsole(const QString& action,bool withpermises) {

    SYD << tr(".........MainWindow::toInputConsole..........*action:|%1|").arg(action);

    // icono de progreso en barra de estado

    QString newaction = extractParameters(action);
    SYD << tr("...MainWindow::toInputConsole...action (extractParameters)NEWACTION:|%1|")
           .arg(newaction);

    SafetTextParser parser;



    _listprincipaltitle = SafetYAWL::LISTSEPARATORCHARACTER + newaction;

    _listprincipaltitle.replace("\n"," ");
    _listprincipaltitle.replace("\r"," ");

    parser.setStr( newaction );

    QString str = "agregar,eliminar,actualizar,mostrar";
    QString commandstr = "INSERT INTO,DELETE,UPDATE,SELECT";
    parser.setCommands(str.split(","),commandstr.split(","));

    QString xml = parser.toXml();
    SYD  << tr("Cadena XML generada de la entrada: \n\"%1\"").arg(xml);
    //myoutput->setPlainText(xml);


    parser.processInput( xml.toLatin1() );
    QString filepath = SafetYAWL::getConfFile().getValue("Input", "input.path").toString();
    QString filename = "defconsole.xml";


    SafetYAWL::streamlog.initAllStack();
    parser.openXml(filepath + "/" + filename);
    if ( queryForErrors() ) {
        return false;
    }

    QStringList results = parser.processXml(false,withpermises);
    if ( results.isEmpty() ) {
        if ( queryForErrors() ) {
            return false ;
        }
    }

    ParsedSqlToData data;
    showString = QString("<a href=\"operacion:::%1\" title=\"%1\">%1</a>&nbsp;&nbsp;&nbsp;").arg("Ver gráfico de flujo de trabajo");
    if ( results.count() > 0) {
        data = SafetTextParser::parseSql(results.at(0),true);

        SYD  << tr("MainWindow::toInputConsole.............results.at(0):|%1|")
                .arg(results.at(0));

        SYD  << tr("\n\nMainWindow::toInputConsole.............***parser.operationName():|%1|")
                .arg(parser.operationName());

        if (parser.operationName().compare("listar_usuarios") == 0) {
            SYD << "Listando usuarios...";
            QString selectrol = MainWindow::replaceMarks(data.map["Rol"]);
            SYD << tr("......MainWindow::toInputConsole....Listar_usuarios...selectrol: |%1|")
                   .arg(selectrol);
            SafetYAWL::streamlog.initAllStack();

            bool myresult = true;

            _currentjson = doListUsers(selectrol);

            SYD << tr("MainWindow::doListUsers...result:\n|%1|")
                   .arg(_currentjson);

            if ( queryForErrors() ) {
                 _currentjson = currentError();
                 myresult = false;
            }
            return myresult;
//            if ( !myresult ) {
//                _currentjson = currentError();
//                SYD << tr("......MainWindow::toInputConsole....doListUsers..._currentjson: |%1|")
//                       .arg(_currentjson);

//            }



        }
        else if (parser.operationName().compare("Listar_datos_con_autofiltro",Qt::CaseSensitive) == 0 ) {
            showString = QString("<a href=\"operacion:::%1\" title=\"%1\">%1</a>&nbsp;&nbsp;&nbsp;").arg("Ver reporte");
            QString texto = QString("-f %1 -d -v %2 -a %3")
                    .arg(data.map["Cargar_archivo_flujo"])
                    .arg(data.map["Variable"])
                    .arg(data.map["Autofiltro"]);


            SYD << tr("MainWindow::toInputConsole...***AUTOFILTER_TEXT:|%1|")
                   .arg(texto);

            parseArgs( texto );

            //                loadReportTemplate();
            if (! executeParsed() ) {
                return false;
            }



        }

        else if ( parser.operationName().startsWith("Listar_",Qt::CaseSensitive)
                && parser.operationName().indexOf("_autofiltro") == -1
                && parser.operationName().indexOf("_filtrorecursivo") == -1
                && parser.operationName().indexOf("_para_clave") == -1
                ) {
            showString = QString("<a href=\"operacion:::%1\" title=\"%1\">%1</a>&nbsp;&nbsp;&nbsp;").arg("Ver reporte");
            QString texto = QString("-f %1 -d -v %2")
                    .arg(data.map["Cargar_archivo_flujo"])
                    .arg(data.map["Variable"]);
            parseArgs( texto );

            //                loadReportTemplate();
            if (! executeParsed() ) {
                return false;
            }

        }
        else if (parser.operationName().compare("Listar_datos_con_filtrorecursivo",Qt::CaseSensitive) == 0 ) {
            showString = QString("<a href=\"operacion:::%1\" title=\"%1\">%1</a>&nbsp;&nbsp;&nbsp;").arg("Ver reporte");
            QString texto = QString("-f %1 -d -v %2 -r %3")
                    .arg(data.map["Cargar_archivo_flujo"])
                    .arg(data.map["Variable"])
                    .arg(data.map["Filtro_recursivo"]);

            parseArgs( texto );

            //                loadReportTemplate();
            if (! executeParsed() ) {
                return false;
            }



        }
        else if (parser.operationName().startsWith("Listar_datos_para_clave") ) {
            showString = QString("<a href=\"operacion:::%1\" title=\"%1\">%1</a>&nbsp;&nbsp;&nbsp;")
                    .arg("Ver reporte");
            QString texto = QString("-f %1 -d -k %2")
                    .arg(data.map["Cargar_archivo_flujo"])
                    .arg(data.map["Clave"]);

            SYD << tr("..............MainWindow::toInputCOnsole...Listar_datos_para_clave.....texto:"
                      "|%1|").arg(texto);
            parseArgs( texto );

            if (! executeParsed() ) {
                return false;
            }


        }

        else if ( parser.operationName().compare(QString::fromLatin1("Generar_gráfico_básico"),Qt::CaseSensitive) == 0) {
            QString texto = QString("-f %1 -p graphviz -g %2")
                    .arg(data.map["Cargar_archivo_flujo"]);

            parseArgs( texto );
            if (! executeParsed() ) {
                return false;
            }


            //processMainWindowThread();
            //return false;
        }
        else if ( parser.operationName().compare(QLatin1String("Generar_gráfico_coloreado"),Qt::CaseSensitive) == 0 ||
		parser.operationName().compare(QLatin1String("Generar_grafico_coloreado"),Qt::CaseSensitive) == 0 

              ) {
            QString texto = QString("-f %1 -p graphviz -g -k coloured")
                    .arg(data.map["Cargar_archivo_flujo"]);

            parseArgs( texto );
            if (! executeParsed() ) {

                return false;
            }


        }
        else if ( parser.operationName().compare("Generar_gráfico_con_autofiltro",Qt::CaseSensitive) == 0) {
            QString texto = QString("-f %1 -p graphviz -g -k coloured -a %2")
                    .arg(data.map["Cargar_archivo_flujo"])
                    .arg(data.map["Autofiltro"]);


            parseArgs( texto );
            if (! executeParsed() ) {
                return false;
            }


            //processMainWindowThread();
            //return false;

        }
        else if ( parser.operationName().compare("Generar_gráfico_con_filtrorecursivo",Qt::CaseSensitive) == 0) {
            QString texto = QString("-f %1 -p graphviz -g -k coloured -r %2")
                    .arg(data.map["Cargar_archivo_flujo"])
                    .arg(data.map["Filtro_recursivo"]);


            parseArgs( texto );

            if (! executeParsed() ) {
                return false;
            }


            //processMainWindowThread();
            //return false;

        }
        else if (parser.operationName().startsWith("Generar_gráfico_para_clave") ||
                 parser.operationName().startsWith("Generar_gráfico_de_seguimiento") ||
                parser.operationName().startsWith("Generar_grafico_para_clave")
) {

            SYD << tr("......**MainWindow::toInputConsole....Generar_gráfico_para_clave.....(1)...");


            QString currflow = "";
            QString currid = "";
            // ****         Flujo
            if (data.map.contains("Cargar_archivo_flujo")) {
                currflow = data.map["Cargar_archivo_flujo"];
            }
            else if (data.map.contains("flujo")) {
                currflow = data.map["flujo"];

            }
            else {
                SYE << tr("No existe la variable flujo para graficar");
            }

            if (data.map.contains("Clave")) {
                currid = data.map["Clave"];
            }
            else if (data.map.contains("id")) {
                currid = data.map["id"];

            }
            else {
                SYE << tr("No existe la variable id para graficar el flujo");
            }


            QString texto = QString("-f %1 -p graphviz -g -k %2 ")
                    .arg(currflow)
                    .arg(currid);

                        SYD << tr("......**MainWindow::toInputConsole....Generar_gráfico_para_clave...TEXTO..(1)..texto:|%1|")
                               .arg(texto);
            parseArgs( texto );
            if (! executeParsed() ) {
                return false;
            }

            QString nameplannedgraph = MainWindow::replaceMarks(data.map["Planificado"]);


        }
        else if (parser.operationName().startsWith("Restaurar_gráfico_de_flujo")) {

            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            SYD << tr("......**MainWindow::toInputConsole....namegraph: |%1|")
                   .arg(namegraph);
            if ( configurator == NULL ) {
                SYE << tr("No se ha cargardo el Objeto para generar flujos de trabajo");
                return false;
            }
            evalConffileValues();
            doRestoreGraph();

            QString code;
            if ( !graphs().contains(namegraph)) {
                SYE << tr("No se encuentra el nombre de grafo seleccionado : \"%1\"")
                       .arg(namegraph);
                return false;

            }
            code = graphs()[namegraph].first;
            SYD << tr("MainWindow::toInputConsole....code: |%1|")
                   .arg(code);

            QString datetag = graphs()[namegraph].second;
            doRenderGraph(code,datetag);


            //processMainWindowThread();
            //return false;
        }
        else if (parser.operationName().startsWith(tr("Siguientes_estados"))) {
            _currentjson = "**states**";
            QStringList infos;
            QString pathflow = MainWindow::replaceMarks(data.map["Cargar_archivo_flujo"]);
            SYD << tr("......***MainWindow::toInputConsole....Siguientes_estados...pathgraph: |%1|")
                   .arg(pathflow);
            QString mykeyvalue = MainWindow::replaceMarks(data.map["Clave"]);
            SYD << tr("......***MainWindow::toInputConsole....Siguientes_estados...namegraph: |%1|")
                   .arg(mykeyvalue);

            if (MainWindow::configurator != NULL ) {
                delete  MainWindow::configurator;
                MainWindow::configurator = NULL;
            }
            MainWindow::configurator = new SafetYAWL();
            Q_CHECK_PTR( MainWindow::configurator );

            MainWindow::configurator->openXML(pathflow);
            MainWindow::configurator->convertXMLtoObjects();
            MainWindow::configurator->openDataSources();

            infos = MainWindow::configurator->getWorkflows().at(0)
                    ->listNextStates(mykeyvalue,SafetWorkflow::OnlyNext,true);
            if ( true ) {
                _currentjson = " { \"Status\": [ \n";
                foreach(QString e, infos) {
                    _currentjson +=  QString("[\"%1\"],\n").arg(e);
                }
                _currentjson.chop(2);
                _currentjson += "],\n";

                QStringList mycurrent = MainWindow::configurator->getWorkflows().at(0)
                        ->listNextStates(mykeyvalue,SafetWorkflow::CurrentState,true);
                _currentjson += " \"Current\": [ \n";
                foreach(QString e, mycurrent) {
                    _currentjson +=  QString("[\"%1\"],\n").arg(e);
                }

                _currentjson.chop(2);
                _currentjson += "\n]\n}\n";


            }
            else {
                _currentjson = tr("Ocurrió un error al tratar de recuperar próximos estados. Vea el registro para detalles##SAFETMESSAGE##");
            }


        }

        else if (parser.operationName().startsWith(tr("Agregar_planificación"))) {
            SYD << tr(".........MainWindow::toInputConsole...(1)...Agregar_planificación");
            QString pathgraph = MainWindow::replaceMarks(data.map["Cargar_archivo_flujo"]);
            SYD << tr("......***MainWindow::toInputConsole....compare...pathgraph: |%1|")
                   .arg(pathgraph);

            QString idactivity = MainWindow::replaceMarks(data.map["id"]);
            SYD << tr("......***MainWindow::toInputConsole....compare...idactivity: |%1|")
                   .arg(idactivity);


            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            SYD << tr("......***MainWindow::toInputConsole....compare...namegraph: |%1|")
                   .arg(namegraph);

            if ( doGeneratePlannedGraph(pathgraph, namegraph,idactivity) ) {
                _currentjson = tr("Se guardó el flujo de trabajo de planificación \"%1\" con éxito##SAFETMESSAGE##")
                        .arg(namegraph);
            }
            else {
                _currentjson = tr("No se guardó  el grafo de planificacion. Vea el registro para detalles##SAFETMESSAGE##");
            }


        }
        else if (parser.operationName().startsWith(tr("Ver_listado_estadísticas"))) {
            bool ok;
            uint seconds = data.map["Desde"].toUInt(&ok);
            QDateTime myfromdate = QDateTime::fromTime_t(seconds);
            QString fromdate = myfromdate.toString("dd/MM/yyyy hh:mmap");
            seconds = data.map["Hasta"].toUInt(&ok);
            QDateTime mytodate = QDateTime::fromTime_t(seconds);
            QString todate = mytodate.toString("dd/MM/yyyy hh:mmap");

                SYD << tr("...MainWindow::toInputConsole....fromdate:|%1|")
                       .arg(fromdate);
                SYD << tr("...MainWindow::toInputConsole....todate:|%1|")
                       .arg(todate);

                SYD << tr("...MainWindow::toInputConsole....Incluir_todas_las_estadisticas:|%1|")
                       .arg(data.map.contains("Incluir_todas_las_estadisticas"));


                SafetBinaryRepo myrepo;

                if (!myrepo.fileExists()) {

                    _currentjson = "No hay estadisticas guardadas actualmente##SAFETMESSAGE##";

                }
                else {
                    QStringList todatelist, tospandatelist;
                    todatelist.append("timeprev");
                    todatelist.append("timecurr");
                    tospandatelist.append("secondswait");
                    myrepo.open();
                    QPair<QString,QString> myfields = myrepo.dataJSON("graphs","",
                                                                      todatelist,
                                                                      tospandatelist);

                    _currentjson += myfields.first;
                    _currentjson += "\n";
                    _currentjson += myfields.second;
                    _currentjson += "\n";

                }

            SYD << tr("..........MainWindow::toInputConsole..._currentjson:\n|%1|\n")
                   .arg(_currentjson);


        }
        else if (parser.operationName().startsWith(tr("Agregar_nodo_a_gráfico_de_flujo"))) {
            SYD << tr(".....MainWindow::toInputConsole....Agregar_nodo_a_gráfico_de_flujo...:");
            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            QString beforenode = MainWindow::replaceMarks(data.map["Nodo_anterior"]);
            QString newnamenode = MainWindow::replaceMarks(data.map["Nombre_nuevo_nodo"]);
            QString paralell = MainWindow::replaceMarks(data.map["Es_paralelo_al_nodo_anterior"]);
            QString foptions = MainWindow::replaceMarks(data.map["Campo_options"]);
            QString fquery = MainWindow::replaceMarks(data.map["Campo_query"]);
            QString ftitle = MainWindow::replaceMarks(data.map["Campo_title"]);
            QString fdocumentsource = MainWindow::replaceMarks(data.map["Campo_documentsource"]);


            SYD << tr("---MainWindow::toInputConsole....namegraph:|%1|").arg(namegraph);
            SYD << tr("---MainWindow::toInputConsole....beforenode:|%1|").arg(beforenode);
            SYD << tr("---MainWindow::toInputConsole....newnamenode:|%1|").arg(newnamenode);
            SYD << tr("---MainWindow::toInputConsole....paralell:|%1|").arg(paralell);
            SYD << tr("---MainWindow::toInputConsole....exist paralell:|%1|").arg(data.map.contains("Es_paralelo_al_nodo_anterior"));
            SYD << tr("---MainWindow::toInputConsole....foptions:|%1|").arg(foptions);
            SYD << tr("---MainWindow::toInputConsole....exist foptions:|%1|").arg(data.map.contains("Campo_options"));
            SYD << tr("---MainWindow::toInputConsole....fquery:|%1|").arg(fquery);
            SYD << tr("---MainWindow::toInputConsole....ftitle:|%1|").arg(ftitle);
            SYD << tr("---MainWindow::toInputConsole....fdocumentsource:|%1|").arg(fdocumentsource);

            bool isparalell = false;

            if (paralell == "Si") {
                isparalell = true;
            }

            SafetYAWL::streamlog.initAllStack();
            QString result = addNodeToXMLWorkflow(namegraph,beforenode,newnamenode,isparalell,foptions,
                                 fquery,ftitle,fdocumentsource);


            if (!result.isEmpty()) {
                _currentjson = tr("Agregado nodo \"%1\" a \"%2\" satisfactoriamente!\n##SAFETMESSAGE##")
                    .arg(newnamenode)
                        .arg(result);
            }
            else {
                 queryForErrors();
                 return false;
            }

        }
        else if (parser.operationName().startsWith(tr("Borrar_nodo_de_gráfico_de_flujo"))) {
            SYD << tr(".....MainWindow::toInputConsole....Borrar_nodo_de_gráfico_de_flujo...:");
            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            QString deletenode = MainWindow::replaceMarks(data.map["Nodo_a_borrar"]);

            QString result = delNodeToXMLWorkflow(namegraph,deletenode);
                    if (!result.isEmpty()) {
                        _currentjson = tr("Eliminado nodo \"%1\" de \"%2\" satisfactoriamente!\n##SAFETMESSAGE##")
                            .arg(deletenode)
                                .arg(result);
                    }
                    else {
                         queryForErrors();
                         return false;
                    }

        }
        else if (parser.operationName().startsWith(tr("Cambiar_nota_de_nodo"))) {

            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            QString taskname = MainWindow::replaceMarks(data.map["Tarea_a_cambiar"]);

            QString newnote;

            if (!data.map.contains("Nota") ) {
                newnote = "";
            }
            else {
                newnote = MainWindow::replaceMarks(data.map["Nota"]);
            }
            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_rol_de_tarea...taskname:|%1|").arg(taskname);
            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_rol_de_tarea...newnote:|%1|").arg(newnote);

            SafetYAWL::streamlog.initAllStack();
            QString result = changeRoleXMLWorkflow(namegraph,
                                                   taskname,newnote,"note");

            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_nota_de_nodo...result:|%1|").arg(result);
            if (!result.isEmpty()) {
                _currentjson = tr("Se cambió la nota de la tarea \"%1\" "
                                  "a \"%2\" satisfactoriamente!\n##SAFETMESSAGE##")
                        .arg(taskname)
                        .arg(newnote);
            }
            else {
                 queryForErrors();
                 return false;
            }



        }

        else if (parser.operationName().startsWith(tr("Cambiar_rol_de_tarea"))) {

            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            QString taskname = MainWindow::replaceMarks(data.map["Tarea_a_cambiar"]);
            QString newrol = MainWindow::replaceMarks(data.map["Nombre_del_rol"]);
            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_rol_de_tarea...taskname:|%1|").arg(taskname);
            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_rol_de_tarea...newrol:|%1|").arg(newrol);

            SafetYAWL::streamlog.initAllStack();
            QString result = changeRoleXMLWorkflow(namegraph,
                                  taskname,newrol);

            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_rol_de_tarea...result:|%1|").arg(result);
            if (!result.isEmpty()) {
                _currentjson = tr("Se cambió el rol de la tarea \"%1\" "
                                  "a \"%2\" satisfactoriamente!\n##SAFETMESSAGE##")
                        .arg(taskname)
                        .arg(newrol);
            }
            else {
                 queryForErrors();
                 return false;
            }



        }
        else if (parser.operationName().startsWith(tr("Cambiar_información_del_nodo"))) {

            SYD << tr("\n.....--->MainWindow::toInputConsole....Cambiar_información_del_nodo........(1)");
            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            QString taskname = MainWindow::replaceMarks(data.map["Tarea_a_cambiar"]);
            QString newinfo = MainWindow::replaceMarks(data.map["Informacion_del_nodo"]);

            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_información_del_nodo...namegraph:|%1|").arg(namegraph);
            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_información_del_nodo...taskname:|%1|").arg(taskname);
            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_información_del_nodo...newinfo:|%1|").arg(newinfo);

            QString result ;
            SafetYAWL::streamlog.initAllStack();
            result = changeRoleXMLWorkflow(namegraph,
                                           taskname,newinfo,"textualinfo");

            SYD << tr(".....--->MainWindow::toInputConsole....Cambiar_información_del_nodo...result:|%1|").arg(result);

            if (!result.isEmpty()) {
                _currentjson = tr("¡Cambiado textualinfo \"%2\" satisfactoriamente!##SAFETMESSAGE##")
                        .arg(result);
            }
            else {
                 queryForErrors();
                 return false;
            }

        }

        else if (parser.operationName().startsWith(tr("Cambiar_conexión_de_gráfico_de_flujo"))) {
            SYD << tr(".....MainWindow::toInputConsole....Cambiar_conexión_de_gráfico_de_flujo...:");
            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            QString nodesource = MainWindow::replaceMarks(data.map["Nodo_origen"]);
            QString nodetarget = MainWindow::replaceMarks(data.map["Nodo_destino_actual"]);
            QString newnodetarget = MainWindow::replaceMarks(data.map["Nuevo_nodo_destino"]);


            SYD << tr("---MainWindow::toInputConsole....(change)namegraph:|%1|").arg(namegraph);
            SYD << tr("---MainWindow::toInputConsole....(change)nodesource:|%1|").arg(nodesource);

            SafetYAWL::streamlog.initAllStack();
            QString result = changeConnXMLWorkflow(namegraph,
                                  nodesource,
                                  nodetarget,
                                  newnodetarget);
            if (!result.isEmpty()) {
                _currentjson = tr("Se cambió la conexión en el grafo  \"%1\" de \"%2\""
                                  "a \"%3\" satisfactoriamente!\n##SAFETMESSAGE##")
                        .arg(result)
                        .arg(nodetarget)
                        .arg(newnodetarget);
            }
            else {
                 queryForErrors();
                 return false;
            }

        }
        else if (parser.operationName().startsWith(tr("Cambiar_fecha_de_planificación"))) {
            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            SYD << tr("......**MainWindow::toInputConsole....Cambiar_fecha_de_planificación...namegraph: |%1|")
                   .arg(namegraph);
            QString nametask = MainWindow::replaceMarks(data.map["Tarea"]);
            SYD << tr("......**MainWindow::toInputConsole....Cambiar_fecha_de_planificación...nametask: |%1|")
                   .arg(nametask);

            bool ok;
            uint seconds = data.map["fecha_planificada"].toUInt(&ok);
            QDateTime mydate = QDateTime::fromTime_t(seconds+Safet::ONEDAYMINUSONE);
            QString namedate = mydate.toString("dd/MM/yyyy hh:mmap");

            SYD << tr("\n\n......**MainWindow::toInputConsole....Cambiar_fecha_de_planificación...namedate: |%1|")
                   .arg(namedate);

            doRestoreGraph(Safet::PLANNEDGRAPHSFILENAME);

            Q_ASSERT( MainWindow::graphs().contains(namegraph));
            QString code = MainWindow::graphs()[namegraph].first;

            code = doModifyDateGraph(nametask,mydate,code);
            MainWindow::graphs()[namegraph].first = code;
            doSaveCompleteMapGraphs(Safet::PLANNEDGRAPHSFILENAME);

            _currentjson = tr("¡Del grafo \"%3\" Se cambió la fecha de la tarea \"%1\" a \"%2\" satisfactoriamente!##SAFETMESSAGE##")
                    .arg(nametask)
                    .arg(namedate)
                    .arg(namegraph);


        }
        else if (parser.operationName().startsWith(tr("Copiar_fechas_de_planificación"))) {
            QString source = MainWindow::replaceMarks(data.map["Nombre_grafo_origen"]);
            QString target = MainWindow::replaceMarks(data.map["Nombre_grafo_destino"]);


            doRestoreGraph(Safet::PLANNEDGRAPHSFILENAME);

             if ( !MainWindow::graphs().contains(source) ) {
                 SYE << tr("No existe el grafo origen: \"%1\"")
                        .arg(source);
                 return false;
             }

             if ( !MainWindow::graphs().contains(target) ) {
                 SYE << tr("No existe el grafo destino: \"%1\"")
                        .arg(target);
                 return false;
             }


            QString codesource = MainWindow::graphs()[source].first;
            QString codetarget = MainWindow::graphs()[target].first;

            QMap<QString,QDateTime> mydates;

            SYD << tr("....MainWindow...toInputConsole....Copiar_fechas...");
            mydates = getDatesGraph(codesource);

            for(int i=0; i < mydates.keys().count(); i++) {
                QString mytask = mydates.keys().at(i);
                if (mydates.contains(mytask)) {
                        QDateTime mydate = mydates[ mytask ];
                        SYD << tr("...toInputConsole: %1->%2")
                               .arg(mytask)
                               .arg(mydate.toString("dd/MM/yyyy hh:mmap"));
                        codetarget = doModifyDateGraph(mytask,mydate,codetarget);
                }
            }
            SYD << tr("target:%1\n%2\n\n")
                   .arg(target)
                   .arg(codetarget);
            MainWindow::graphs()[target].first = codetarget;
            doSaveCompleteMapGraphs(Safet::PLANNEDGRAPHSFILENAME);

            _currentjson = tr("Se copiaron todas las fechas del grafo \"%1\" "
                              "al grafo \"%2\" satisfactoriamente!\n##SAFETMESSAGE##")
                    .arg(source)
                    .arg(target);

        }
        else if (parser.operationName().startsWith(tr("Comparar_gráfico_de_flujo"))) {

           // Colocando configuración de comparación por defecto
            SafetYAWL::getConf().getMap()["Plugins.Graphviz/plugins.graphviz.extrainfo.show"]
                    = "InfoText,InfoDate";
            // Colocando configuración de comparación por defecto

            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            SYD << tr("......**MainWindow::toInputConsole....compare...namegraph: |%1|")
                   .arg(namegraph);
            QString nameplannedgraph = MainWindow::replaceMarks(data.map["Planificado"]);
            SYD << tr("......**MainWindow::toInputConsole....compare...nameplannedgraph: |%1|")
                   .arg(nameplannedgraph);

            if ( configurator == NULL ) {
                SYE << tr("No se ha cargardo el Objeto para generar flujos de trabajo");
                return false;
            }
            QString code, codeplanned; // Código Real y Planificado
            evalConffileValues();

            doRestoreGraph();

            if ( !graphs().contains(namegraph)) {
                SYE << tr("No se encuentra el nombre de grafo seleccionado : \"%1\"")
                       .arg(namegraph);
                return false;

            }
            code = graphs()[namegraph].first;
            SYD << tr("MainWindow::toInputConsole....Comparar_grafico...code: |%1|")
                   .arg(code);

            QString datetag = graphs()[namegraph].second;

            // Para Gráfico Planificado ********
            doRestoreGraph(Safet::PLANNEDGRAPHSFILENAME);
            if ( !graphs().contains(nameplannedgraph)) {
                SYE << tr("No se encuentra el nombre de grafo seleccionado (Planificado) : \"%1\"")
                       .arg(nameplannedgraph);
                return false;

            }

            codeplanned = graphs()[nameplannedgraph].first;

            SYD << tr(".......................MainWindow::toInputConsole....**codeplanned**:\n%1\n-------------\n")
                   .arg(codeplanned);
             QStringList taskmark;
            QString compcode = doCompareGraphs(code,codeplanned,taskmark);




            if ( !compcode.isEmpty()) {
                SYD << QString("\n\n.........MainWindow::toInputConsole....compcode:\n%1\n\n").arg(compcode);
                doRenderGraph(compcode,datetag);
//                SYD << tr("....before addShapeToSVG...._currentjson:|%1|").arg(_currentjson);
                addShapeToSVG("/var/www/media/"+_currentjson,taskmark);
            }

            _currenttable = generateTableCompared(code,codeplanned,taskmark);

            SYD << tr("....***MainWindow::toInputConsole....regresando...(1)..._currenttable:\n%1\n")
                   .arg(_currenttable);

        }
        else if (parser.operationName().compare("Borrar_estadística",Qt::CaseSensitive) == 0 ) {

            QString namestats = data.map["Clave_completa"];
            SYD << tr("....MainWindow::toInputConsole...Borrar_estadística...(1)...namestats:"
                      "|%1|")
                   .arg(namestats);

            QString delall = data.map["borrar_todos"];
            SYD << tr("....MainWindow::toInputConsole...Borrar_estadística...(1)...borrar_todos:"
                      "|%1|")
                   .arg(delall);

            QString whereclause = "1";
            if (delall.isEmpty()) {
                whereclause = QString("completekey= '%1'")
                        .arg(namestats);
            }
            SafetBinaryRepo myrepo;

            if (!myrepo.fileExists()) {

                _currentjson = tr("La base de datos de estadísticas está vacía."
                                 "No se eliminó ningún registro.##SAFETMESSAGE##");

                return true;
            }
            myrepo.open();

            if ( myrepo.deleteRecords("graphs",whereclause) > 0 ) {
                _currentjson = tr("Se ha(n) eliminado el(los) registro(s) seleccionado(s) "
                                  "de la base de datos de Estadísticas##SAFETMESSAGE##");
            }





        }
        else if (parser.operationName().startsWith("Borrar_gráfico_de_flujo") ||
                 parser.operationName().startsWith("Borrar_planificación") ) {

            QString mygraphfilename = Safet::GRAPHSFILENAME;
            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            SYD << tr("......**MainWindow::toInputConsole....namegraph: |%1|")
                   .arg(namegraph);
            if ( configurator == NULL ) {
                SYE << tr("No se ha cargardo el Objeto para generar flujos de trabajo");
                return false;
            }
            if (parser.operationName().startsWith("Borrar_planificación") ) {
                mygraphfilename = Safet::PLANNEDGRAPHSFILENAME;
            }
            if ( deleteGraph(namegraph,mygraphfilename) ) {

                    SYA << tr("Borrado correctamente Grafo \"%1\"")
                           .arg(namegraph);
                _currentjson = tr("Borrado correctamente Grafo \"%1\"##SAFETMESSAGE##")
                        .arg(namegraph);
            }


        }

        else if (parser.operationName().startsWith("Agregar_grafo") ) {
            SYD << "Agregando grafo...";
            QString namegraph = MainWindow::replaceMarks(data.map["Nombre_grafo"]);
            SYD << tr("......MainWindow::toInputConsole....Agregar_grafo...namegraph: |%1|")
                   .arg(namegraph);
            SafetYAWL::streamlog.initAllStack();
            bool myresult =   addXmlMenuGroupForm(namegraph);

            SYD << tr("......MainWindow::toInputConsole....addXmlMenuGroupForm...result: |%1|")
                   .arg(myresult);

            queryForErrors();
            if ( !myresult ) {
                _currentjson = currentError();
                SYD << tr("......MainWindow::toInputConsole....addXmlMenuGroupForm..._currentjson: |%1|")
                       .arg(_currentjson);

            }
            return myresult;

        }
    }
    return true;


}


QString MainWindow::doListUsers(const QString& selectrol) {

    QString result = QString("[ %1 ]");

    QString listusers  = "";

    foreach(QString u, users.keys()) {
        listusers += QString("\"%1\",").arg(u);
    }
    listusers.chop(1);

    result = result.arg(listusers);

    return result;
}

QString MainWindow::doModifyDateGraph(const QString& nametask,const QDateTime& mydate,
                                      const QString& code) {
    QString result;



    QStringList mytasks = code.split("\n", QString::SkipEmptyParts);


    bool isfound = false;
    SYD << tr("\n");
    foreach(QString mytask, mytasks) {

        SYD << tr(".........MainWindow::doModifyDateGraph.....mytask:|%1|")
               .arg(mytask.split(",").at(0));
        SYD << tr(".........MainWindow::doModifyDateGraph...nametask:|%1|")
               .arg(QString("Nodo:%1").arg(nametask));
        if ( !isfound && (mytask.split(",").at(0) == QString("Nodo:%1").arg(nametask))) {
            QString patterndate = QString(",0...1...%2...%1")
                    .arg(mydate.toString("dd/MM/yyyy hh:mmap"))
                    .arg(MainWindow::currentaccount);
            mytask = mytask.section(",",0,-2)+patterndate;
            SYD << tr(".....MainWindow::doModifyDateGraph....mytask (modified):\n|%1|\n")
                   .arg(mytask);
            isfound = true;
        }
        result += mytask;
        result += "\n";
    }
    SYD << tr("\n");

    return result;
}


QMap<QString,QDateTime> MainWindow::getDatesGraph(const QString& code) {

    QMap<QString,QDateTime> result;

   QStringList mytasks = code.split("\n", QString::SkipEmptyParts);


    foreach(QString mytask, mytasks) {
        QString currtask  = mytask.split(",").at(0).mid(tr("Nodo:").length());

            QString myinfo = mytask.section(",",-1);
            QStringList myinfos = myinfo.split("...",QString::SkipEmptyParts);
            if (myinfos.count() == 4 ) {
                result[currtask ] = QDateTime::fromString(myinfos.at(3),
                                         "dd/MM/yyyy hh:mmap");
                SYD << tr("MainWindow::getDatesGraph....currtask:|%1|->|%2|")
                       .arg(currtask)
                       .arg(myinfos.at(3));
            }

    }


    return result;
}


bool MainWindow::doGeneratePlannedGraph(const QString &pathflow, const QString& name, const QString& idactivity) {

    QString texto = QString("-f %1 -p graphviz -g -k %2 ")
            .arg(pathflow)
            .arg(Safet::NOKEY);
    SYD << tr(".....MainWindow::doGeneratePlannedGraph....Texto: |%s|").arg(texto);

    parseArgs( texto );

    if (! executeParsed() ) {
        SYE << tr("No fue posible ejecutar la accion \"%1\"")
               .arg(texto);
        return false;
    }

    SYD << tr(".......MainWindow::doGeneratePlannedGraph....SafetYAWL::lastgraph..:\n|%1|\n\n")
           .arg(SafetYAWL::lastgraph);


    QStringList nodes = SafetYAWL::lastgraph.split("\n", QString::SkipEmptyParts);

    QString newgraph;
    QDateTime now = QDateTime::currentDateTime();
    foreach(QString nd, nodes) {

        QDateTime nowplus = now.addMonths(1);
        QString patterndate = QString(",0...1...%2...%1")
                .arg(nowplus.toString("dd/MM/yyyy hh:mmap"))
                .arg(MainWindow::currentaccount);
        QString n = nd.section(",",0,-2)+patterndate;

        newgraph += n +"\n";
    }

    SYD << tr("...MainWindow::doGeneratePlannedGraph....newgraph..:\n|%1|")
           .arg(newgraph);


    QString pathgraphfile = SafetYAWL::pathconf+ "/graphs" + "/"+
            Safet::PLANNEDGRAPHSFILENAME +".gph";

    SYD << tr("...MainWindow::doSaveGraph...doGeneratePlannedGraph: |%1|").arg(pathgraphfile);


    QFile myfile(pathgraphfile);
    if( !myfile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        SYE << tr("No es posible escribir en la ruta: \"%1\"")
               .arg(pathgraphfile);
        return false;
    }
    QString msg = SafetYAWL::getConf()["Graphs/infotext.format"];
    QString date = now.toString("dd MMM yyyy");
    QString time = now.toString("h:mm:ssap");
    QString datetime = now.toString("dd MMM yyyy hh:mm:ssap");
    msg.replace("%time",time);
    msg.replace("%datetime",datetime);
    msg.replace("%date",date);

    QDataStream out(&myfile);   // we will serialize the data into the file
        out << name;
        out <<  newgraph;
        out <<  msg+SafetYAWL::LISTSEPARATORCHARACTER+pathflow+SafetYAWL::LISTSEPARATORCHARACTER+idactivity;
    myfile.close();

    SYA << tr("Se guardó un gráfico de flujo de trabajo con el nombre: "
              "\"%1\"")
           .arg(name);




    return true;

}

bool MainWindow::doSaveCompleteMapGraphs(const QString& gfn) {
    QString pathgraphfile = SafetYAWL::pathconf+ "/graphs" + "/"+
            gfn +".gph";
    SYD << tr("...MainWindow::deleteGraph.....pathgraphfile: |%1|")
           .arg(pathgraphfile);
    QFile myfile(pathgraphfile);

    if( !myfile.open(QIODevice::WriteOnly)) {
        SYE << tr("No es posible escribir en la ruta: \"%1\"")
               .arg(pathgraphfile);
        return false;
    }
    QDataStream out(&myfile);   // we will serialize the data into the file

    for(int i=0; i < graphs().keys().count();i++) {
        QString title = graphs().keys().at(i);
        QPair<QString,QString> mypair = graphs()[title];
        out << title;
        out << mypair;
    }
    myfile.close();

    return true;

}

bool MainWindow::deleteGraph(const QString& n, const QString& gfn) {
    SYD << tr("...MainWindow::deleteGraph....n:|%1|")
           .arg(n);
    QString pathgraphfile = SafetYAWL::pathconf+ "/graphs" + "/"+
            gfn +".gph";
    SYD << tr("...MainWindow::deleteGraph.....pathgraphfile: |%1|")
           .arg(pathgraphfile);
    QFile myfile(pathgraphfile);
    if( !myfile.open(QIODevice::ReadOnly)) {
        SYE  << tr("No es posible leer gráficos de la ruta: \"%1\"")
                .arg(pathgraphfile);
        return false;
    }

    QDataStream in(&myfile);


    QString newn = n;
    newn.replace(QRegExp("\\s+")," ");
    graphs().clear();
    int i = 0;
    while( !in.atEnd()) {
        QString title;
        QPair<QString,QString> mypair;
        in >> title;
        in >> mypair;
        title.replace(QRegExp("\\s+")," ");

        if (title.compare(newn,Qt::CaseSensitive) == 0 ) {
            continue;
        }
        if (!title.isNull()) {
            title.replace(QRegExp("\\s+")," ");
            graphs() [ title ] = mypair;
            i++;
       }

    }


    SYD << tr("Se restauraron \"%1\" graficos de \"%2\"")
           .arg(i)
           .arg(pathgraphfile);

    myfile.close();


    if( !myfile.open(QIODevice::WriteOnly)) {
        SYE << tr("No es posible escribir en la ruta: \"%1\"")
               .arg(pathgraphfile);
        return false;
    }
    QDataStream out(&myfile);   // we will serialize the data into the file

    for(i=0; i < graphs().keys().count();i++) {
        QString title = graphs().keys().at(i);
        QPair<QString,QString> mypair = graphs()[title];
        out << title;
        out << mypair;
    }
    myfile.close();

    return true;

}


QStringList MainWindow::lastInfoGraph() {
    QStringList mylist;
    mylist.append(SafetYAWL::lastgraph);
    mylist.append(SafetYAWL::lastinfodate);
    return mylist;
}


void MainWindow::toInputSign() {
    // icono de progreso en barra de estado


    SafetTextParser parser;

    QString texto; 
//    texto = completingTextEdit->toPlainText().toLatin1();
    qDebug("toInputSign....texto: \n|%s|\n", qPrintable(texto));

    parser.setStr( texto.toLatin1() );
    QString str = "agregar,eliminar,actualizar,mostrar";
    QString commandstr = "INSERT INTO,DELETE,UPDATE,SELECT";
    parser.setCommands(str.split(","),commandstr.split(","));
    qDebug("....xml...(1): \n|%s|\n", qPrintable(parser.str()));
    QString xml = parser.toXml();
    qDebug("....xml...(2): \n|%s|\n", qPrintable(xml));
    SafetYAWL::streamlog << SafetLog::Debug << tr("Cadena XML generada de la entrada: \n\"%1\"").arg(xml);
    //myoutput->setPlainText(xml);


    qDebug("\n\n....MainWindow::toInputFormxml:\n%s", qPrintable(xml));
    parser.processInput( xml.toLatin1() );
    QString filepath = SafetYAWL::getConfFile().getValue("Input", "input.path").toString();
    QString filename = "defmanagementsignfile.xml";
    qDebug("toInputSign.filepath + \"/\" + filename: %s", qPrintable(filepath + "/" + filename));
    parser.openXml(filepath + "/" + filename);


    QStringList results = parser.processXml(false);
    if ( queryForErrors() ) {
        return;
    }


    ParsedSqlToData data;
    foreach(QString s, results) {
        data = SafetTextParser::parseSql(s);
        if ( parser.operationName().startsWith("Firmar",Qt::CaseInsensitive) ) {
            qDebug("...signDocumentsFromData...");

            if ( parser.operationName().endsWith("Tarjeta")) {
                QString nametowrite;
                QStringList list;
                SafetDocument mydoc;
                Q_CHECK_PTR( configurator );
                bool result = configurator->signDocumentsFromData(data,nametowrite,list,mydoc);
                if (result ) {
                    setPathOfSafetDocument(nametowrite);
                    successVerification(list,tr("Firma electrónica (digidoc) realizada "
                                                "exitosamente sobre \"%1\"....ok!")
                                        .arg(nametowrite), mydoc);
                    return;
                }


                QStack<QPair<QDateTime,QString> > mystack = SafetYAWL::streamlog.stopErrorStack();
                SafetYAWL::streamlog << SafetLog::Debug << trUtf8("Hay \"%1\" errores guardados en la lista. De inmediato se procede"
                                                                " a inicializar la lista de errores.")
                                                                .arg(mystack.count());
                SafetYAWL::streamlog.stopAllStack();
                qDebug("ERRORES DE VERIFICACION: %d", mystack.count());
                if ( mystack.count() > 0 ) {
                    QString message = renderMessageStack(mystack,SafetLog::Error);
                    QString messagew = renderMessageStack(mystack,SafetLog::Warning);
                    SafetYAWL::streamlog << SafetLog::Action << tr("<b>Firma Electronica</b> sobre"
                                                                "el archivo INVALIDA");


                }
                qDebug("toInputSign()");
            }
            else if ( parser.operationName().endsWith("P12")) {
                qDebug("...signDocumentsFromData..P12.");

                QString nametowrite;
                QStringList list;
                SafetDocument mydoc;
                Q_CHECK_PTR( configurator );

                bool exec = configurator->signDocumentsFromData(data,nametowrite,list,mydoc,false);

                if ( !exec ) {
                    return;
                }
                setPathOfSafetDocument(nametowrite);
                successVerification(list,tr("Firma electrónica (digidoc) realizada "
                                            "exitosamente sobre \"%1\"....ok!")
                                    .arg(nametowrite), mydoc);
                QStack<QPair<QDateTime,QString> > mystack = SafetYAWL::streamlog.stopErrorStack();
                SafetYAWL::streamlog << SafetLog::Debug << trUtf8("Hay \"%1\" errores guardados en la lista. De inmediato se procede"
                                                                " a inicializar la lista de errores.")
                                                                .arg(mystack.count());
                SafetYAWL::streamlog.stopAllStack();
                qDebug("ERRORES DE VERIFICACION: %d", mystack.count());
                if ( mystack.count() > 0 ) {
                    QString message = renderMessageStack(mystack,SafetLog::Error);
                    QString messagew = renderMessageStack(mystack,SafetLog::Warning);
                    SafetYAWL::streamlog << SafetLog::Action << tr("<b>Firma Electronica</b> sobre"
                                                                "el archivo INVALIDA");


                }
            }

        }
        else if (parser.operationName().startsWith("Verificar",Qt::CaseInsensitive)) {
            SafetYAWL::streamlog << SafetLog::Action << tr("Se va a realizar la verificacion...ok!");

            SafetDocument doc;
            Q_CHECK_PTR( configurator );
            configurator->verifyDocumentsFromData(data,doc);
            setPathOfSafetDocument(doc.getPathOfSafetDocument());
            QStringList list;
            int nsigns = doc.numberOfSignaturesOnOpenXAdESContainer();

            list << tr("Ver firmas del documento(%1)").arg(nsigns) << tr("Mostrar carpeta del contenedor") << tr("Enviar contenedor por correo-e");


            successVerification(list,tr("Verificada correctamente la <b>Firma Electrónica</b>"
                                        " sobre el archivo \"<b>%1</b>\"...ok!")
                                //.arg(digidocfilename), safetDocument);
                                .arg(doc.getPathOfSafetDocument()), doc);

            QStack<QPair<QDateTime,QString> > mystack = SafetYAWL::streamlog.stopErrorStack();
            SafetYAWL::streamlog << SafetLog::Debug << trUtf8("Hay \"%1\" errores guardados en la lista. De inmediato se procede"
                                                              " a inicializar la lista de errores.")
                    .arg(mystack.count());

            SafetYAWL::streamlog.stopAllStack();

            qDebug("**ERRORES DE VERIFICACION: %d", mystack.count());
            if ( mystack.count() > 0 ) {
                QString message = renderMessageStack(mystack,SafetLog::Error);
                QString messagew = renderMessageStack(mystack,SafetLog::Warning);
                SafetYAWL::streamlog << SafetLog::Action << tr("<b>Firma Electronica</b> sobre el archivo \"<b>%1</b>\""
                                                               " INVALIDA").arg(doc.getPathOfSafetDocument());

            }
        }
    }
    if ( false) {
        qDebug("Verificación de Firma(s) INVÁLIDA...*NOT NOT NOT*");
    }
    else {

    }

}

void MainWindow::toInputConfigure(const QString& mytexto) {

     SafetTextParser parser;
     QString texto = mytexto;
//     texto  = completingTextEdit->toPlainText().toLatin1();
     SYD << tr("....MainWindow::toInputConfigure....(1)....");

     SYD << tr("....MainWindow::toInputConfigure....(1)....texto:|%1|").arg(texto);
     parser.setStr( texto.toLatin1() );
     QString str = "agregar,eliminar,actualizar,mostrar";
     QString commandstr = "INSERT INTO,DELETE,UPDATE,SELECT";
     parser.setCommands(str.split(","),commandstr.split(","));
     qDebug("....xml...(1): \n|%s|\n", qPrintable(parser.str()));
     QString xml = parser.toXml();
     qDebug("....xml...(2): \n|%s|\n", qPrintable(xml));
     SafetYAWL::streamlog << SafetLog::Debug << tr("Cadena XML generada de la entrada: \n\"%1\"").arg(xml);
     //myoutput->setPlainText(xml);




     parser.processInput( xml.toLatin1() );
     QString filepath = SafetYAWL::getConfFile().getValue("Input", "input.path").toString();
     QString filename = "defconfigure.xml";

     parser.openXml(filepath + "/" + filename);
     SYD << tr("....MainWindow::toInputConfigure....(1)....papth:|%1|").arg(filepath + "/" + filename);

     QStringList names = parser.loadNameConfs();

     foreach(QString n, names) {
//          QMap<QString,QVariant> conf = centraledit()->findConf(n);
//          parser.addConf(conf, n);
     }

     QStringList results = parser.processXml(false);

    if ( queryForErrors() ) {
        qDebug(".......****..queryforerror....");
             return;
     }


      foreach(QString s, results) {
         SYD <<  tr("toInputConfigure.......result: |%1|").arg(s);

         proccessConfFile(s,"safet.conf",true);
     }

      if ( MainWindow::configurator  /* && user say yes */ ) {
          configurator->closeDataSources();
          delete MainWindow::configurator;
          MainWindow::configurator = new SafetYAWL();
          Q_CHECK_PTR( configurator);
          configurator->openDataSources();
      }



}


void MainWindow::proccessConfFile(const QString& sql, const QString& filename, bool multiplefields) {

     SYD << tr("...MainWindow::proccessConfFile.......");
     QString fileconf = filename;
     if ( filename.isEmpty() ) {
         fileconf = SafetYAWL::pathconf+ "/" + "safet.conf";

     }
     qDebug("....proccessConfFile...fileconf: %s", qPrintable(fileconf));

     QRegExp rx;
     QString newsql = sql;
     newsql.replace("'","");
      QString updatePattern = "UPDATE\\s+([áéíóúñÑa-zA-Z0-9_\\.\\(\\)#%][áéíóúñÑa-zA-Z0-9_,'\\.\\(\\)\\-#%\\x3c\\x3e\\x2f]*)"
                             "\\s+SET\\s+"
                             "([áéíóúñÑa-zA-Z0-9_\\.\\(\\)\\*;#%@][áéíóúñÑa-zA-Z0-9_,'\\=\\.\\(\\)\\-\\s\\*;#%@]*"
                             "[áéíóúñÑa-zA-Z0-9_,'\\=\\.\\(\\)\\-\\*;#@]\\s+)?WHERE\\s+"
                             "([áéíóúñÑa-zA-Z0-9_\\.\\(\\)\\*;#%][áéíóúñÑa-zA-Z0-9_,'\\=\\.\\(\\)\\s\\-\\*;#%\\x3c\\x3e\\x2f]*"
                             "[áéíóúñÑa-zA-Z0-9_,'\\=\\.\\(\\)\\-\\*;#%\\x3c\\x3e\\x2f])";

     //UPDATE lista SET database.user.1='vbravo' WHERE database.db.1='dbtracrootve'
     qDebug("        (###)......updatePattern: |%s|", qPrintable(updatePattern));

     QString insertPattern = "INSERT INTO\\s+([a-zA-Z0-9_][a-zA-Z0-9_\\.\\-]*)\\s+"
                             "\\(([a-zA-Z0-9_\\.\\(\\)][a-zA-Z0-9_,'\\=\\.\\(\\)\\-\\*]*)\\)\\s+"
                             "VALUES\\s+\\(([a-zA-Z0-9_'\\./\\(\\)@][a-zA-Z0-9_,'\\=\\.\\-/\\*\\s\\x3c\\x3e\\x2f@]*)\\)\\s*";


     QString deletePattern = "DELETE FROM\\s+([áéíóúñÑa-zA-Z0-9_\\.\\(\\)][áéíóúñÑa-zA-Z0-9_,'\\.\\(\\)\\-]*)"
                             "\\s+WHERE\\s+"
                             "([áéíóúñÑa-zA-Z0-9_\\.\\(\\)\\*;][áéíóúñÑa-zA-Z0-9_,'\\=\\.\\(\\)\\s\\-\\*;\\x3c\\x3e\\x2f]*"
                             "[áéíóúñÑa-zA-Z0-9_,'\\=\\.\\(\\)\\-\\*;\\x3c\\x3e\\x2f])";


     bool isdeleting = false;
     rx.setPattern(updatePattern);
     int pos = rx.indexIn(newsql);

     SYD << tr(".........MainWindow::processConfFile....newsql: |%1|.......pos:|%2|").arg(newsql).arg(pos);

     if ( pos == -1 ) {


         rx.setPattern(insertPattern);
         pos = rx.indexIn(newsql);
         if (pos == -1 ) {
             //qDebug("        (###)......deletePattern: |%s|", qPrintable(deletePattern));
             rx.setPattern(deletePattern);
             pos = rx.indexIn(newsql);
          //   qDebug("  ...deletePattern...pos: %d", pos);
             if (pos == -1 ) {

                 SYE << tr("Ocurrio un error con la sentencia SQL \"%1\" formada por la entrada"
                                                               " Realice una lectura del registro para conocer donde se encuentra"
                                                               "  el error").arg(newsql);
                 return;
             }
             else {
                 isdeleting = true;
            //     qDebug("...deletepattern: newsql: %s",qPrintable(newsql));
                 //doInsertInAuthConfFile(rx,MainWindow::DELETEPATTERN);

             }
         }
         else {

             SYD << tr("..........MainWindow::processConfFile....doInsertInAuthConfFile....(*1)...");
             doInsertInAuthConfFile(rx);


             return;
         }
     }

     QString keyfield, fields;
     if ( !isdeleting ){
         keyfield = rx.cap(3);
         fields = rx.cap(2);
     }
     else {
         keyfield = rx.cap(2);
     }


     SYD << tr(".....MainWindow::proccessConfFile...pattern: |%1|").arg(rx.pattern());
     // Para el keyfield

     SYD << tr(".....MainWindow::proccessConfFile...keyfield: |%1|").arg(keyfield);
     if ( keyfield.split("=",QString::SkipEmptyParts).count() != 2 ) {
         SYE << tr("Ocurrio un error con la sentencia SQL \"%1\" formada por la entrada."
                                                       "No se encuentra la asignación de campos con el operador '='."
                                                       " Realice una lectura del registro para conocer donde se encuentra"
                                                      "el error").arg(newsql);
         return;
     }
     QString firstkeyfield = keyfield.split("=",QString::SkipEmptyParts).at(0);
     //qDebug("         proccessConfFile...firstkeyfield...(1):%s", qPrintable(firstkeyfield));
     int numberreg = 0;
     QString prefixkey;
     QString secondkeyfield = keyfield.split("=",QString::SkipEmptyParts).at(1);

     SYD << tr(".....MainWindow::proccessConfFile...firstkeyfield: |%1|").arg(firstkeyfield);
    SYD << tr(".....MainWindow::proccessConfFile...secondkeyfield: |%1|").arg(secondkeyfield);

     if (multiplefields) {
         if (firstkeyfield.endsWith(".*")  ) {

             prefixkey = firstkeyfield.mid(0,firstkeyfield.length()-2);
             //qDebug("**prefixkey: %s", qPrintable(prefixkey));
             numberreg = SafetYAWL::getAuthConf().getNumberRegister(secondkeyfield,prefixkey,true);

             //qDebug("**numberreg: %d", numberreg);
             if (numberreg > 0 ) {
                 firstkeyfield = QString("%1.%2").arg(prefixkey)
                                 .arg(numberreg);
               //  qDebug("   (*)proccessConfFile...firstkeyfield...(2):%s", qPrintable(firstkeyfield));
             }


         }
     }

     //qDebug("   processConffile...(1)...firstkeyfield: %s", qPrintable(firstkeyfield));

     QString searchtext = QString("\\s*(%1)\\s*\\=\\s*([áéíóúñÑa-zA-Z0-9\\*\\$\\#/\\-\\._/:!\\?\\^\\$\\(\\)#%\\x3c\\x3e\\x2f@]"
                             "[áéíóúñÑa-zA-Z0-9\\*\\$\\#/\\-\\._/:!\\?\\^\\$\\s\\(\\);#%\\x3c\\x3e\\x2f@]*)"
                             "[áéíóúñÑa-zA-Z0-9\\*\\$\\#/\\-\\._/:!\\?\\^\\$\\(\\);#%\\x3c\\x3e\\x2f]")
             .arg(firstkeyfield);
     QString replacetext;
     QString currentfirstkeyfield;
     QString currentsecondkeyfield = secondkeyfield;
     if ( !isdeleting ) {
        replacetext = tr ("%1 = %2").arg(firstkeyfield).arg(secondkeyfield);

     }
     else {
          replacetext = "";
     }
     currentfirstkeyfield = firstkeyfield;


     SYD << tr(".....MainWindow::proccessConfFile...SEARCHTEXT----->**searchtext: |%1|").arg(searchtext);
     SYD << tr(".....MainWindow::proccessConfFile...----->**replacetext: |%1|").arg(replacetext);
     SafetYAWL::replaceTextInFile(fileconf,
                                  searchtext,
                                  replacetext);




     // Para los otros campos que no son claves

     if (isdeleting) {
        fields = searchFieldsInAuthConf(firstkeyfield);
    }

     //qDebug("   processConffile...(2)...fields.count():%d",fields.count());
     QStringList listfields;
     listfields = fields.split(",",QString::SkipEmptyParts);


     foreach( QString s, listfields ) {
         if ( s.split("=",QString::SkipEmptyParts).count() != 2 && !isdeleting) {
             continue;
         }
         if (!isdeleting) {
            firstkeyfield = s.split("=",QString::SkipEmptyParts).at(0);
        }
         else {
            firstkeyfield = s;
         }
         //qDebug("...firstkeyfield: %s", qPrintable(firstkeyfield));
         if (multiplefields && !isdeleting) {
             //qDebug("numberreg: %d", numberreg);
             if (firstkeyfield.endsWith(".*")  && numberreg > 0 ) {
                 firstkeyfield = QString("%1.%2").arg(firstkeyfield.mid(0,firstkeyfield.length()-2))
                                 .arg(numberreg);
                 //qDebug("...proccessConfFile...firstkeyfield...(2):%s", qPrintable(firstkeyfield));


             }
         }
         if (!isdeleting) {
            secondkeyfield = s.split("=",QString::SkipEmptyParts).at(1).trimmed();
        }
         else {
             secondkeyfield = "";
         }
         searchtext = tr("\\s*(%1)\\s*\\=\\s*([áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f@]"
                         "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$\\s,;\\x3c\\x3e\\x2f@]*"
                         "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\x3c\\x3e\\x2f@])").arg(firstkeyfield);
         if (!isdeleting) {
            replacetext  = tr("%1 = %2").arg(firstkeyfield).arg(secondkeyfield);

        }
         else {
             //qDebug("searchtext: %s", qPrintable(searchtext));
             replacetext ="";
         }

        // qDebug("           ...processConfUsers...replacetext:|%s|",qPrintable(replacetext));
         SafetYAWL::replaceTextInFile(fileconf,
                                      searchtext,
                                      replacetext);

     }

     // Reorganizar los otros campos mayotres a numberreg
     if (isdeleting) {

         numberreg++;
         int result;
         listfields.push_front(currentfirstkeyfield);
         while(true){


             foreach( QString s, listfields ) {
             QString currprefkey = s.split(".").at(0)+"."+
                                  s.split(".").at(1);
             QString nextkey = QString("%1.%2").arg(currprefkey)
                               .arg(numberreg);

             //qDebug("...nextkey: %s",qPrintable(nextkey));
             searchtext = tr("\\s*(%1)\\s*\\=\\s*([áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\(\\)\\x3c\\x3e\\x2f@]"
                             "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$\\s,;\\(\\)\\x3c\\x3e\\x2f@]*"
                             "[áéíóúñÑa-zA-Z0-9\\$\\#/\\-\\._/:!\\?\\^\\$,;\\(\\)\\x3c\\x3e\\x2f@])").arg(nextkey);

             QString replacekey = QString("%1.%2").arg(currprefkey)
                                  .arg(numberreg-1);
             //qDebug("...replacekey: %s",qPrintable(replacekey));

             replacetext  = tr("%1 = ||cap||").arg(replacekey);
             result = SafetYAWL::replaceTextInFile(fileconf,
                                          searchtext,
                                          replacetext,
                                          Qt::CaseSensitive,
                                          2);


             }
             if ( !result ) {



                 searchtext = QString("[^;](%1;)([a-z0-9_]+[a-z0-9_;]*)").arg(currentsecondkeyfield);
                 SYD << tr(".........**DELETING FROM OPERATIONS......searchtext:|%1|")
                        .arg(searchtext);

                 replacetext = QString("||cap||");

                 SafetYAWL::replaceTextInFile(fileconf,
                                           searchtext,
                                           replacetext,
                                           Qt::CaseSensitive,
                                           2);

                 searchtext = QString("([a-z0-9_;]*[a-z0-9_]+)(;%1)$").arg(currentsecondkeyfield);
                 SYD << tr(".........**DELETING FROM OPERATIONS......searchtext:|%1|")
                        .arg(searchtext);

                 replacetext = QString("||cap||");

                 SafetYAWL::replaceTextInFile(fileconf,
                                           searchtext,
                                           replacetext,
                                           Qt::CaseSensitive,
                                           1);

                 searchtext = QString("(\\=\\s*)(%1)$").arg(currentsecondkeyfield);
                 SYD << tr(".........**DELETING FROM OPERATIONS......searchtext:|%1|")
                        .arg(searchtext);

                 replacetext = QString("||cap||admin");

                 SafetYAWL::replaceTextInFile(fileconf,
                                           searchtext,
                                           replacetext,
                                           Qt::CaseSensitive,1);

                 searchtext = QString("([a-z0-9_;]*[a-z0-9_]+)(;%1;)([a-z0-9_]+[a-z0-9_;]*)").arg(currentsecondkeyfield);
                 SYD << tr(".........**DELETING FROM OPERATIONS......searchtext:|%1|")
                        .arg(searchtext);

                 replacetext = QString("||cap||;||cap0||");


                 SafetYAWL::replaceTextInFile(fileconf,
                                           searchtext,
                                           replacetext,
                                           Qt::CaseSensitive,
                                           1,3);

                 break;
             }
             numberreg++;


         }
     }

}


QString MainWindow::searchFieldsInAuthConf(const QString& key) {
    qDebug("searchFieldsInAuthConf...Clave...key: %s", qPrintable(key));
    QString retfields;
    QString exp;

    if ( key.split(".").count() < 3 ) {
        return QString("");
    }
    exp = "[a-zA-Z0-9]+/("+key.split(".").at(0)+"\\.[a-zA-Z0-9]+\\."+key.split(".").at(2)+")";

    QRegExp rx(exp);
    //qDebug("searchFieldsInAuthConf...Clave...RegExp: %s", qPrintable(exp));
    for(int i =0; i < SafetYAWL::getAuthConf().keys().count() ; i++){
        QString currentkey = SafetYAWL::getAuthConf().keys().at(i);

        int pos = rx.indexIn(currentkey);
        if ( pos >= 0 ) {
            if ( rx.cap(1) != key ) {
                retfields += rx.cap(1) +",";
                //qDebug("searchFieldsInAuthConf...Clave...CurrentKey: %s", qPrintable(rx.cap(1)));
            }
        }
    }
    if ( !retfields.isEmpty()){
        retfields.chop(1);
    }

    qDebug("...retfields: %s", qPrintable(retfields));
    return retfields;

}


void MainWindow::doInsertInAuthConfFile(QRegExp& rx) {


    SYD << tr("..........MainWindow::processConfFile....WITHOUTREPLACECONF....doInsertInAuthConfFile....rx.pattern:|%1|...")
           .arg(rx.pattern());

    SYD << tr("..........MainWindow::processConfFile....WITHOUTREPLACECONFdoInsertInAuthConfFile....rx.cap(1):|%1|...")
           .arg(rx.cap(1));
    SYD << tr("..........MainWindow::processConfFile....doInsertInAuthConfFile....rx.cap(2):|%1|...")
           .arg(rx.cap(2));

    SYD << tr("..........MainWindow::processConfFile....doInsertInAuthConfFile....rx.cap(3):|%1|...")
           .arg(rx.cap(3));

    QStringList fields, values;

    fields = rx.cap(2).split(",");
    values = rx.cap(3).split(",");

    QString fileconf = SafetYAWL::pathconf+ "/" + "auth.conf";
        int countuser = 1;
        QString replacetext,newfield;

        bool isreplacing = true;
        QString sectiontext;
        while (isreplacing ) {

            QString  newtext;

            if (sectiontext.isEmpty() && fields.count() > 0 ) {
                QString firstfield = fields.at(0);
                sectiontext = firstfield.mid(0,1).toUpper()+firstfield.mid(1);
                sectiontext = sectiontext.split(".").at(0);
                //qDebug("    ..........sectiontext:%s", qPrintable(sectiontext));
            }

            for(int i = 0; i < fields.count(); i++) {
                newfield = fields.at(i);
                newfield.chop(2);
                SYD << tr(".........MainWindow::doInsertAuthConfFile........fields.at(%1): |%2|").arg(i).arg(newfield);
                QString subfield = newfield.mid(0,1).toUpper()+newfield.mid(1);
                newfield = QString("%1.%2").arg(subfield).arg(countuser);

                SYD << tr(".........MainWindow::doInsertAuthConfFile.......(countuser)....newfield: |%1|").arg(newfield);
                QString firstfield = newfield.split(".").at(0);
                QString key = firstfield +"/"+newfield.mid(firstfield.length()+1);

                SYD << tr(".........MainWindow::doInsertAuthConfFile.....firstfield: |%1|").arg(firstfield);
                SYD << tr(".........MainWindow::doInsertAuthConfFile.....key: |%1|").arg(key);
                newfield = newfield.toLower();
                if (!SafetYAWL::getAuthConf().contains(key) ) {
                    newtext = QString("%1").
                              arg(newfield)
                              +" = " + values.at(i) + "\n";
                    replacetext += newtext;
                   // qDebug("! replacing:%s",qPrintable(newtext));
                    isreplacing = false;
                    SYD << tr(".........MainWindow::doInsertAuthConfFile........NO CONTIENE...(1)...newtext:|%1|")
                           .arg(newtext);
                }
                else {
                    SYD << tr(".........MainWindow::doInsertAuthConfFile..........SI CONTIENE...(1)...(newfield):|%1|")
                           .arg((newfield));
                    replacetext += QString("%1").arg(newfield)
                                   +" = " + SafetYAWL::getAuthConf()[ key ] + "\n";
                    SYD << tr(".........MainWindow::doInsertAuthConfFile..........SI CONTIENE...(1)...key:|%1|")
                           .arg(key);
                   // SYD << tr(".........MainWindow::doInsertAuthConfFile..........REPLACETEXT:|%1|")
                     //      .arg(replacetext);
                }
            }

            replacetext += "\n";
            countuser++;


        }



        qDebug();
        SYD << tr("......MainWindow::doInsertInAuthConfFile....replaceSectionInFile...replacetext:|%1|")
               .arg(replacetext);

        SYD << tr("......MainWindow::doInsertInAuthConfFile....replaceSectionInFile...sectiontext:|%1|")
               .arg(sectiontext);
        SafetYAWL::replaceSectionInFile(fileconf,sectiontext,replacetext);

}



void MainWindow::restoreWindowState()
{
}



void MainWindow::saveWindowState()
{
}

/*
void MainWindow::closeEvent(QCloseEvent *event)
{
}
*/

void MainWindow::toChangeUser() {
    qDebug("...MainWindow::toChangeUser()...");
            checkGoPrincipal();

}
/*
void MainWindow::resizeEvent(QResizeEvent *event) {
}
*/

void MainWindow::checkGoPrincipal() {

}

/*
void MainWindow::moveEvent(QMoveEvent *event)
{
}
*/


/*
void MainWindow::showEvent(QShowEvent *event)
{
}
*/

void MainWindow::doExit() {
}

bool MainWindow::maybeSave() {

        return false;
}


void MainWindow::setupStackedWebviews(const QIcon& icon, const QString& name, const QString& desc) {



}

bool MainWindow::searchInHistoryList(const QString& str) {

}

void MainWindow::saveToHistoryList() {
}

void MainWindow::addToHistoryList() {


}

QMap<QString,QString> MainWindow::loadEditActions() {
    QMap<QString,QString> result;

    QStringList actions  = SafetYAWL::getConf()["GeneralOptions/consoleactions.*"].split(SafetYAWL::LISTSEPARATORCHARACTER,QString::SkipEmptyParts);

    foreach(QString s, actions) {
        QStringList tlist = s.split(";",QString::SkipEmptyParts);
        if ( tlist.count() < 2 ) continue;
        QString mykey = tlist.at(0);
        QString myvalue = tlist.at(1);
        result[mykey] = myvalue;
    }
    return result;
}


void MainWindow::editToHistoryList() {

}

void MainWindow::delToHistoryList() {
}

void MainWindow::setupTabWidget() {

    // Para la consola de salida


}

QString MainWindow::drawWorkflow(const QString& filename) {

    QString newfile = filename.section("/",-1)+"."+SafetYAWL::getConf()["Plugins.Graphviz/graphtype"];

    SYD << tr("...........MainWindow::drawWorkflow....filename:|%1|")
           .arg(filename);
    SYD << tr("...........MainWindow::drawWorkflow....mediaPath():|%1|")
           .arg(mediaPath());
    SYD << tr("...........MainWindow::drawWorkflow..../newfile:|/%1|")
           .arg(newfile);

    QFile::copy(filename,mediaPath()+ "/"+  newfile);
    return newfile;

}


void MainWindow::setMediaPath(const QString& m) {
    _mediapath = m;
    if (_mediapath.endsWith("/")) {
        _mediapath.chop(1);
    }

}
QString MainWindow::mediaPath() {
    return _mediapath;
}


void MainWindow::setHostMediaPath(const QString& m) {
    _hostmediapath = m;
    if (_hostmediapath.endsWith("/")) {
        _hostmediapath.chop(1);
    }

}

QString MainWindow::hostMediaPath() {
    return _hostmediapath;
}


void MainWindow::buildModel(QMap<QString,QVariant>& mymap) {



}


void MainWindow::refreshListView(const QString& doc) {

}











void MainWindow::doWidgetsOptions() {

}

void MainWindow::doGeneralOptions() {
}


bool MainWindow::loadWidgetPlugins(const QString& f, bool add) {
    if (!configurator->loadWidgetPlugins(f)) {
        SafetYAWL::streamlog
                << SafetLog::Warning
                << tr("No se cargo correctamente el plugins: \"%1\"")
                .arg(f);
        return false;
    }
     QMap<QString,QVariant> conf;
     qDebug("....MainWindow::loadWidgetPlugins...f: %s", qPrintable(f));
     if ( add ) {
          Q_CHECK_PTR( SafetYAWL::listDynWidget );

          Q_ASSERT( SafetYAWL::listDynWidget->count() > 0 );
          WidgetInterface* mywid = SafetYAWL::listDynWidget->at(SafetYAWL::listDynWidget->count()-1);
          Q_CHECK_PTR( mywid );
          conf = mywid->conf();
          qDebug("....loadWidgetPlugins...conf[namewidget]: %s f: %s",
                    qPrintable(conf["namewidget"].toString()), qPrintable(f));
          //Q_ASSERT( conf["namewidget"] == f );
     }
     return true;
}



void MainWindow::createMenu() {

}

bool MainWindow::doSaveGraph(const QStringList& mypars) {



   SYD << tr(".............MainWindow::doSaveGraph.............mypars.count():|%1|").arg(mypars.count());

   if ( mypars.count() < 4 ) {
       SYE << tr("El nùmero de parametros a guardar debe ser cuatro (4): nombre, codigo, fecha + ruta del flujo y clave");
       return false;
   }

   QString pathgraphfile = SafetYAWL::pathconf+ "/graphs" + "/"+
               Safet::GRAPHSFILENAME +".gph";
    SYD << tr("...MainWindow::doSaveGraph.....pathgraphfile: |%1|")
           .arg(pathgraphfile);
    QFile myfile(pathgraphfile);
    SYD << tr("............MainWindow::doSaveGraph..........");
    if( myfile.open(QIODevice::ReadOnly)) {

        QDataStream in(&myfile);

        graphs().clear();
        int i = 0;
        bool graphexists = false;

        QString newtitle = mypars.at(0);
        newtitle.replace(QRegExp("\\s+")," ");

        while( !in.atEnd()) {
            QString title;
            QPair<QString,QString> mypair;
            in >> title;
            in >> mypair;
            title.replace(QRegExp("\\s+")," ");


            if (title.compare(newtitle,Qt::CaseSensitive) == 0 ) {
                mypair.first = mypars.at(1);

                mypair.second = mypars.at(2) + SafetYAWL::LISTSEPARATORCHARACTER + mypars.at(3);
                graphexists = true;
            }

            if (!title.isNull()) {
                title.replace(QRegExp("\\s+")," ");
                graphs() [ title ] = mypair;
                i++;
            }

        }

        if (!graphexists ) {
            QPair<QString,QString> mypair;
            mypair.first = mypars.at(1);
            mypair.second = mypars.at(2) + SafetYAWL::LISTSEPARATORCHARACTER + mypars.at(3);
            graphs()[ newtitle ] = mypair;
        }



    }
    else {
        QPair<QString,QString> mypair;
        QString newtitle = mypars.at(0);
        newtitle.replace(QRegExp("\\s+")," ");
        mypair.first = mypars.at(1);
        mypair.second = mypars.at(2) + SafetYAWL::LISTSEPARATORCHARACTER + mypars.at(3);
        graphs()[ newtitle ] = mypair;

    }
    SYD << tr("............MainWindow::doSaveGraph..........(2)");
        myfile.close();

        SYD << tr("............MainWindow::doSaveGraph..........(3)");

    if( !myfile.open(QIODevice::WriteOnly| QIODevice::Append)) {
        SYE << tr("Para guardar el grafo No es posible escribir en la ruta: \"%1\"")
               .arg(pathgraphfile);
        return false;
    }

    SYD << tr("............MainWindow::doSaveGraph..........(4)");
    QDataStream out(&myfile);   // we will serialize the data into the file


    for(int i=0; i < graphs().keys().count();i++) {
        QString title = graphs().keys().at(i);
        QPair<QString,QString> mypair = graphs()[title];
        out << title;
        out << mypair;
        SYD << tr("Escribiendo el grafo:|%1|").arg(title);
    }
    myfile.close();

    return true;


}


void MainWindow::doRestoreGraph(const QString& gfn) {
    QString pathgraphfile = SafetYAWL::pathconf+ "/graphs" + "/"+
            gfn +".gph";
    SYD << tr("...MainWindow::doRestoreGraph.....pathgraphfile: |%1|")
           .arg(pathgraphfile);
    QFile myfile(pathgraphfile);
    if( !myfile.open(QIODevice::ReadOnly)) {
        SYE  << tr("No es posible leer gráficos de la ruta: \"%1\"")
                .arg(pathgraphfile);
        return;
    }

    QDataStream in(&myfile);
    QString title;

    graphs().clear();
        int i = 0;
    while( !in.atEnd()) {
        QPair<QString,QString> mypair;
        in >> title;
        in >> mypair;
        SYD << tr("...MainWindow::doRestoreGraph....title:|%1|").arg(title);
        title.replace(QRegExp("\\s+")," ");
        if (!title.isNull()) {
            graphs() [ title ] = mypair;
            i++;
        }
    }
    SYD << tr("Se restauraron \"%1\" graficos de \"%2\"")
           .arg(i)
           .arg(pathgraphfile);


}

void MainWindow::doRenderGraph(const QString& code, const QString& datetag) {
    Q_CHECK_PTR( configurator );
    if ( configurator == NULL ) {

        SYD << tr("MainWindow::doRenderGraph...configurator is null");
        return;
    }

    if ( SafetYAWL::curOutputInterface == NULL ) {

        SYD << tr("...curOutputInterface is null");
        configurator->loadPlugins("graphviz");
    }

    //QMap<QString,QString> mymap = SafetYAWL::getConfFile().readFile(SafetYAWL::pathconf + "/" + SafetYAWL::fileconf);
    QMap<QString,QString> mymap =  SafetYAWL::getConf().getMap();

    SYD << tr("...MainWindow::doRenderGraph...Plugins.Graphviz/extrainfo.show:|%1|")
           .arg(SafetYAWL::getConf()["Plugins.Graphviz/extrainfo.show"]);

    SYD << tr("trying curOutputInterface...(1)");
    Q_ASSERT ( SafetYAWL::curOutputInterface == NULL );
    QString parsedCodeGraph = SafetYAWL::curOutputInterface->parseCodeGraph(code, mymap);
    SYD << tr("trying configurator...(2)");
    SafetYAWL::filelog.close();
    SafetYAWL::filelog.open(QIODevice::Append);
    SafetYAWL::streamlog.setDevice(&SafetYAWL::filelog);

    char type[4];

    strncpy(type,qPrintable(SafetYAWL::getConf()["Graphviz.Plugins/graphtype"]),3);
    //********* QUITAR *************
    strncpy(type,"svg",3);
    //********* QUITAR *************
            type[3] = 0;


    SYD << tr("...*MainWindow::doRenderGraph.....graphtype:|%1|")
                   .arg(type);
            QString img = SafetYAWL::curOutputInterface->renderGraph(parsedCodeGraph, type, mymap);

    if (img.startsWith("ERROR")) {
            SYE << tr("Ocurrió el siguiente error al generar el gráfico de flujo \"%1\"")
            .arg(img);
        queryForErrors();
        return ;
    }
    SafetYAWL::filelog.close();
    SafetYAWL::filelog.open(QIODevice::Append);
    SafetYAWL::streamlog.setDevice(&SafetYAWL::filelog);

    SafetYAWL::lastgraph = code;
    SafetYAWL::lastinfodate = datetag;


    _currentjson = drawWorkflow(img); // Dibujar el flujo

    SafetYAWL::streamlog.stopAllStack();


}

QString MainWindow::doCompareGraphs(const QString& firstgraph, const QString& secondgraph,  QStringList& taskmark) {

    QString newgraph = generateGraphCompared(firstgraph, secondgraph, taskmark);

    return newgraph;

}


void MainWindow::addShapeToSVG(const QString& filename, const QStringList& list) {


    SYD << tr(".....MainWindow::addShapeToSVG......entering...(1)....filename:|%1|").arg(filename);
    QString tempname = SafetYAWL::getTempNameFiles(1).at(0);
    QFile file( filename );
    QFile tempfile ( tempname );

    bool isopen = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if ( !isopen ) {
        SYE  << tr("Ocurrio un error al tratar de abrir el archivo \"%1\" para lectura").arg(filename);
        return;
    }
    bool isopentemp = tempfile.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Truncate);
    if ( !isopentemp ) {
        SYE << tr("Ocurrio un error al tratar de abrir archivo temporal \"%1\" para escritura").arg(tempname);
        return;
    }



    QTextStream in(&file);
    in.setCodec("ISO 8859-1");
    QTextStream out(&tempfile);
    out.setCodec("ISO 8859-1");
    QString pattern;

    QStringList newlist;
    foreach(QString s, list) {
        QString news = s.mid(QString("Nodo:").length());
        SYD << tr(".........addShapeToSVG...node:|%1|").arg(news.trimmed());
        newlist.append(news);
    }

    bool hasviewbox = false;

    while ( !in.atEnd()) {
        QString line = in.readLine();
        int pos = -1;
        QRegExp rx;
        foreach(QString s, newlist) {
            pattern = QString("<text.+x\\=\"([\\d\\-\\.]+)\".+y\\=\"([\\d\\-\\.]+)\".+%1.*</text>").arg(s);            
            rx.setPattern(pattern);
            pos = rx.indexIn(line);
//            SYD << tr("..........addShapeToSVG....pattern:|%1|").arg(pattern);
//            SYD << tr("..........addShapeToSVG....pos:|%1|").arg(pos);
            if (pos != -1) {
                break;
            }
         }
            if (pos != -1 ) {
                SYD << tr("..........addShapeToSVG....x:|%1|").arg(rx.cap(1));
                SYD << tr("..........addShapeToSVG....y:|%1|").arg(rx.cap(2));
                SYD << tr("..........addShapeToSVG....line:|%1|").arg(rx.cap(0));
                bool ok;
                double posX = rx.cap(1).toDouble(&ok);
                double posY = rx.cap(2).toDouble(&ok);
                QString linestring = QString("<line x1=\"%1\" y1=\"%2\" x2=\"%3\" y2=\"%4\"  style=\"stroke:rgb(0,0,0);stroke-width:3\"/>\n")
                        .arg(posX-35).arg(posY+27).arg(posX+35).arg(posY-27);

                QString newstring = line+"\n"
                        +linestring;

                 out << newstring;

            } else {
                if (line.indexOf("id=\"graph0\"")!= -1 ) {
//                    out << QString("<g id=\"graph0\" class=\"graph\" transform=\"scale(1 1) rotate(0) translate(4 2120)\">") + "\n";
                      out << line + "\n";

                }
                else if (line.indexOf("</svg>")!= -1 ) {
                   out << Safet::TEMPLATELEGEND + "\n";
                    out << line + "\n";
                }
                else {
                    out << line + "\n";
                }
//                if (!hasviewbox && line.indexOf("viewBox=") != -1 ) {
//                    hasviewbox = true;
//                    out << Safet::TEMPLATELEGEND;

//                }
            }



    }

    file.close();
    tempfile.close();
    SYE <<  tr("Se escribio el archivo con texto reemplazado: \"%1\"")
            .arg(tempname);
    bool result = QFile::remove(filename);
    qDebug("...borrando...: %s", qPrintable(filename));
    if (!result ) {
        SYE << QObject::tr("No es posible eliminar el archivo en la ubicacion: \"%1\"").arg(filename);
    }
    result = QFile::rename(tempname, filename);
    qDebug("...moviendo...: \"%s\" a \"%s\"", qPrintable(tempname),qPrintable(filename));
    if ( !result ) {
        SYE << QObject::tr("No es posible copiar el archivo en la ubicacion: \"%1\"").arg(tempname);
        return;
    }


    SYD << tr("...addShapeToSVG... copiando correctamente archivo...ok!");

    return;



}


QString MainWindow::addInfoGraphDateText() {
    QString result;
    QString inc = SafetYAWL::getConf()["Graphs/infotext.include"].trimmed();

    if ( inc.compare("on") != 0 ) {
        SYD << tr("MainWindow::addInfoGraphDateText....inc:|%1|")
               .arg(inc);
        return result;
    }

    result = SafetYAWL::getConf()["Graphs/infotext.format"];
    QDateTime now = QDateTime::currentDateTime();
    QString date = now.toString("dd MMM yyyy");
    QString time = now.toString("h:mm:ssap");
    QString datetime = now.toString("dd MMM yyyy hh:mm:ssap");
    result.replace("%time",time);
    result.replace("%datetime",datetime);
    result.replace("%date",date);

    SafetYAWL::lastinfodate = result;
    SYD << tr("............*MainWindow::addInfoGraphDateText....SafetYAWL::lastinfodate:|%1|")
           .arg(result);

    return result;
}



QString MainWindow::currentGraphTitle() {
    QString curtitle = SafetYAWL::getConf()["GeneralOptions/currentflowtitle"].trimmed();
    return curtitle;
}



QString MainWindow::generateTableCompared(const QString& first, const QString& second, QStringList& taskmark) {


    QString result;



    QStringList myfirstnodelist, mysecondnodelist;
    myfirstnodelist = first.split("\n",QString::SkipEmptyParts);
    mysecondnodelist = second.split("\n",QString::SkipEmptyParts);

    bool ok;
    int prodays = SafetYAWL::getConf()["DefaultValues/graphs.daysproject"].toInt(&ok);
    if (prodays <= 0 ) {
        SYE << tr("El parámetro \"%1\" no tiene un valor válido")
               .arg("DefaultValues/graphs.daysproject");
        return QString();
    }

    QString tasks = QString("{ \n");


    QString  taskscompleted = QString("[ ");

    QString   tasksupcoming = QString("[ ");

    QString   taskcurrent = QString("[\n ");


// { \"Nombre\": \"%1\", \"Entrega\": \"%2\", \"Planeada\": \"%3\",\"Info\": \"%4\" %5},\n")

        QString taskscolumn = QString(" \"columns\" : ["
                                      " {\"sTitle\": \"Status\"},\n"
                                      " {\"sTitle\": \"Nombre\"},\n"
                                      " {\"sTitle\": \"Entrega\"},\n"
                                      " {\"sTitle\": \"Planeada\"},\n"
                                      " {\"sTitle\": \"Info\"},\n"
                                      " {\"sTitle\": \"Rol\"},\n"
                                      " ],\n"
                                      );

    QString currentnode;
    for(int i=0; i < myfirstnodelist.count(); i++ ) {
        QString myfirst = myfirstnodelist.at(i);
        QStringList myfirstlist = myfirst.split(",",QString::SkipEmptyParts);
        int rlnodes = myfirstlist.count();
        Q_ASSERT(rlnodes > 0 );
        QStringList statsreal = myfirstlist.at(rlnodes-1).split("...",QString::SkipEmptyParts);
        if (statsreal.count() < 2) {
            continue;
        }

        int ntokens = statsreal.at(0).toInt(&ok);
        if (ntokens > 0 ) {
            currentnode = myfirstlist.at(0);
            break;
        }

    }
    SYD << tr(".....MainWindow::generateTableCompared....currentnode:|%1|")
           .arg(currentnode);


    if ( currentnode.isEmpty()) {
        SYW << trUtf8("NO se encuentra el nodo donde se encuentra la clave para el gráfico a comparar");
        currentnode = "inicial";
    }

    bool passnode = false;

    for(int i=0; i < myfirstnodelist.count(); i++ ) {
        int days = 0;
        SYD << tr ("....MainWindow::generateTableCompared...:|%1|")
               .arg(i);

        QPair<double,int>  mypair;
        mypair.first = 0;
        mypair.second = 0;
        QString myfirst = myfirstnodelist.at(i);
        QString mysecond;
        QStringList myfirstlist = myfirst.split(",",QString::SkipEmptyParts);


    bool uncomplete = false;
        QRegExp rx;

        rx.setPattern("Reporte:\\s*(yes|no)\\s*,?");


        if (rx.indexIn(myfirst)) {
            if (rx.cap(1) == "no") {
                continue;
            }

        }

        bool issecondfound = false;
        for(int j=0; j < mysecondnodelist.count(); j++ ) {
            if ( myfirstlist.at(0) == mysecondnodelist.at(j).section(",",0,0) ) {
                mysecond =  mysecondnodelist.at(j);
                issecondfound = true;
                break;
            }
        }
        if ( !issecondfound ) {
            SYW << tr(" Generando comparación de grafos: no se encontró el \"%1\"")
                   .arg(myfirstlist.at(0));
            continue;
//            return QString();
        }
        QStringList mysecondlist = mysecond.split(",",QString::SkipEmptyParts);

        if (myfirstlist.count() == 0 ) {
            SYD << tr(" Error 1");
            break;
        }
        if (mysecondlist.count() == 0 ) {
            SYD << tr(" Error 2");
            break;
        }
        if (myfirstlist.count() != mysecondlist.count()) {
            SYW << tr("Nodo:|%3| Cuenta nodo guardado:%1....Cuenta planificado:%2").arg(myfirstlist.count())
                   .arg(mysecondlist.count())
                   .arg(myfirstlist.at(0));
            //break;
        }

        QString realnode = myfirstlist.at(0).trimmed();

        QString namenode = realnode.mid(QString("Nodo:").length());

        if (realnode == "Nodo:final" || realnode == "Nodo:inicial") {
            continue;
        }

        rx.setPattern("Rol:\\s*([a-zA-Z0-9_\\-]+)\\s*,?");


        QString rolnode;
        if (rx.indexIn(myfirst)) {
            rolnode = rx.cap(1);

        }

        QString plannode = mysecondlist.at(0).trimmed();
        SYD << tr("....MainWindow::generateTableCompared....realnode:|%1|...plannode:|%2|")
               .arg(realnode)
               .arg(plannode);
        if ( realnode != plannode ) {
            SYE     << tr("Los gráficos seleccionados son diferentes."
                          " No tienen los mismos nodos en la misma posición."
                          " El primer nodo tiene nombre \"%1\". "
                          " El segundo nodo tiene nombre \"%2\"." )
                       .arg(myfirstlist.at(0))
                       .arg(mysecondlist.at(0));
            return QString();

        }

        int rlnodes = myfirstlist.count();
        int plnodes = mysecondlist.count();
        QStringList statsreal = myfirstlist.at(rlnodes-1).split("...",QString::SkipEmptyParts);

    if (myfirstlist.at(rlnodes-1).indexOf("[*]") >= 0 ) {
        SYD << tr("....tablecompared...UNCOMPLETED:|%1|")
        .arg(myfirstlist.at(rlnodes-1));

        uncomplete = true;
    }
        QStringList statsplanned = mysecondlist.at(plnodes-1).split("...",QString::SkipEmptyParts);




        int nstatsreal = 0;
        if ( statsreal.count() > 2 ) {
             nstatsreal = 1;
        }
        double porc = 0;

        QString datestrp;
        QDateTime dateplanned;


        QString statsinfo;

        bool isdelay = false;


        if (statsreal.count() < 4 ) {

            if ( statsplanned.count() < 4 ) {
                   statsinfo =  tr("0...1...n/a...n/a\n");
            }
            else {
                QDateTime mynow = QDateTime::currentDateTime();
                datestrp  = statsplanned.at(3);
                dateplanned = QDateTime::fromString(datestrp,"dd/MM/yyyy hh:mmap");

                if (mynow < dateplanned) {

                    statsinfo = tr("0...1...n/a...Debe entregar en <br/>%1<br/>(%2)\n")
                        .arg(SafetWorkflow::humanizeDate(days,mynow
                                                         .toString("dd/MM/yyyy hh:mmap"),
                                                         "dd/MM/yyyy hh:mmap",
                                                         dateplanned),
                                                         datestrp);

                          taskcurrent += QString(" [ \"%6\",\"%1\", \"%2\",  \"%3\", \"%4\" %5],\n")
                            .arg(namenode)
                            .arg("n/a")
                            .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                            .arg(uncomplete?tr("A tiempo"):tr("A tiempo"))
                            .arg(rolnode.length()>0?QString(" ,\"%1\" ").arg(rolnode):",\"-\"")
                            .arg(tr("Pendiente"));


                }
                else {
                    statsinfo = tr("0...1...n/a...Debio entregar hace <br/>%1<br/>(%2)\n")
                        .arg(SafetWorkflow::humanizeDate(days,dateplanned
                                                         .toString(Safet::DateFormat),
                                                         Safet::DateFormat,
                                                         mynow),
                                                         datestrp);
                    isdelay = true;
                taskcurrent += QString(" [ \"%6\",\"%1\", \"%2\",  \"%3\", \"%4\" %5],\n")
                            .arg(namenode)
                            .arg("n/a")
                            .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                            .arg(tr("Atraso"))
                            .arg(rolnode.length()>0?QString(" ,\"%1\" ").arg(rolnode):",\"-\"")
                        .arg(tr("Pendiente"));

                }
            }

            rx.setPattern("info\\.task\\.color:\\s*([\\-a-zA-Z0-9_\\.]+)\\s*,?");

            for(int j=0; j < myfirstlist.count()-nstatsreal; j++) {
                QString nextfield =  myfirstlist.at(j);
                if ( rx.indexIn( nextfield ) != -1 ) {
                    if ( days >= prodays ) {
                        porc = 1;
                    }
                    else {
                        porc = double(days)/double(prodays);
                    }

                    porc = 1-porc;

                    if ( isdelay) {
                        porc = porc*-1;
                    }


                    nextfield = QString("info.task.color:%1")
                            .arg(porc,0,'g',2);

                }

                result += nextfield;
                result += ",";

            }

            if (realnode == QLatin1String("Nodo:inicial")) {
                result += "\n";
                continue;

            }


                result += statsinfo;

            continue;
        }


        if  (statsplanned.count() < 4 ) {
            SYD << tr(" Error 20");
            break;
        }


        if  (statsreal.count() < 4 ) {
            SYD << tr(" Error 5");
            break;
        }

        datestrp  = statsplanned.at(3);
        dateplanned = QDateTime::fromString(datestrp,"dd/MM/yyyy hh:mmap");



        QString datestr = statsreal.at(3).section("<br/>",0,0);
        QDateTime  datereal = QDateTime::fromString(datestr,"dd/MM/yyyy hh:mmap");




        if ( realnode == currentnode) {

            passnode = true;
            if ( datereal < dateplanned) {
                statsinfo = tr("0...1...%4.. **AQUI**...%1<br/>entrega:%2<br/>plan.:%3\n")
                        .arg(SafetWorkflow::humanizeDate(days,datestr,"dd/MM/yyyy hh:mmap",
                                                         dateplanned))
                        .arg(datestr)
                        .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
            .arg(uncomplete?"incompleta":"a tiempo");


                taskcurrent += QString(" [ \"%6\",\"%1\", \"%2\",  \"%3\", \"%4\" %5],\n")
                        .arg(namenode)
                        .arg(datestr)
                        .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                        .arg(uncomplete?tr("A tiempo"):tr("A tiempo"))
                         .arg(rolnode.length()>0?QString(" ,\"%1\" ").arg(rolnode):",\"-\"")
                        .arg(tr("Actual"));

                if(!uncomplete)
                    taskmark.append(realnode);
            }
            else {
                statsinfo = tr("0...1...(%4) AQUI...%1<br/>actual:%3<br/>plan.:%2\n")
                        .arg(SafetWorkflow::humanizeDate(days,datestrp,"dd/MM/yyyy hh:mmap",
                                                         datereal))
                             .arg(datestrp)
                             .arg(datereal.toString("dd/MM/yyyy hh:mmap"))
                .arg(uncomplete?tr("atraso/incompleta"):tr("atraso"));
                  isdelay = true;

                if(!uncomplete)
                  taskmark.append(realnode);


                  taskcurrent += QString(" [ \"%6\",\"%1\", \"%2\",  \"%3\", \"%4\" %5],\n")
                          .arg(namenode)
                          .arg(datestr)
                          .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                          .arg(tr("Atraso"))
                          .arg(rolnode.length()>0?QString(" , \"%1\" ").arg(rolnode):",\"-\"")
                          .arg(tr("Actual"));

            }
        }
        else {
            if (!passnode) {
                if ( datereal < dateplanned) {
                    statsinfo = tr("0...1...%4...%1(adelanto)<br/>entrega:%2<br/>plan.:%3\n")
                            .arg(SafetWorkflow::humanizeDate(days,datestr,"dd/MM/yyyy hh:mmap",
                                                             dateplanned))
                            .arg(datestr)
                            .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                .arg(uncomplete?"incompleta":"a tiempo");

                if(!uncomplete)
                    taskmark.append(realnode);

                  taskcurrent += QString(" [ \"%6\",\"%1\", \"%2\",  \"%3\", \"%4\" %5],\n")
                            .arg(namenode)
                            .arg(datestr)
                            .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                            .arg(uncomplete?tr("A tiempo"):tr("A tiempo"))
                            .arg(rolnode.length()>0?QString(" ,\"%1\" ").arg(rolnode):",\"-\"")
                            .arg(uncomplete?tr("Incompleta"):tr("Completada"));
                }
                else {
                    statsinfo = tr("0...1...%4...%1<br/>actual:%3<br/>plan.:%2\n")
                            .arg(SafetWorkflow::humanizeDate(days,datestrp,"dd/MM/yyyy hh:mmap",
                                                             datereal))
                            .arg(datestrp)
                            .arg(datereal.toString("dd/MM/yyyy hh:mmap"))
                .arg(uncomplete?tr("atraso/incompleta"):tr("atraso"));

                    isdelay = true;

                if(!uncomplete)
                    taskmark.append(realnode);
                    taskcurrent += QString(" [ \"%6\",\"%1\", \"%2\",  \"%3\", \"%4\" %5],\n")
                            .arg(namenode)
                            .arg(datestr)
                            .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                            .arg(tr("Atraso"))
                            .arg(rolnode.length()>0?QString(" ,\"%1\" ").arg(rolnode):",\"-\"")
                            .arg(uncomplete?tr("Incompleta"):tr("Completada"));


                }
            }
            else {
                if ( datereal < dateplanned) {
                    statsinfo = tr("0...1...%4...%1(adelanto)<br/>entrega:%2<br/>plan.:%3\n")
                            .arg(SafetWorkflow::humanizeDate(days,datestr,"dd/MM/yyyy hh:mmap",
                                                             dateplanned))
                             .arg(datestr)
                             .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                 .arg(uncomplete?"incompleta":"a tiempo");

                if(!uncomplete)
                    taskmark.append(realnode);
                    taskcurrent += QString(" [\"%6\",\"%1\", \"%2\",  \"%3\", \"%4\" %5],\n")
                            .arg(namenode)
                            .arg(datestr)
                            .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                            .arg(uncomplete?tr("A tiempo"):tr("A tiempo"))
                            .arg(rolnode.length()>0?QString(" ,\"%1\" ").arg(rolnode):",\"-\"")
                            .arg(uncomplete?tr("Incompleta"):tr("Completada"));
                }
                else {
                    statsinfo = tr("0...1...%4...%1<br/>actual:%3<br/>plan.:%2\n")
                            .arg(SafetWorkflow::humanizeDate(days,datestrp,"dd/MM/yyyy hh:mmap",
                                                             datereal))
                             .arg(datestrp)
                            .arg(datereal.toString("dd/MM/yyyy hh:mmap"))
                .arg(uncomplete?tr("atraso/incompleta"):tr("atraso"));
                    isdelay = true;

                if(!uncomplete)
                    taskmark.append(realnode);
                    taskcurrent += QString(" [\"%6\",\"%1\", \"%2\",  \"%3\", \"%4\" %5],\n")
                            .arg(namenode)
                            .arg(datestr)
                            .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                            .arg(tr("Atraso"))
                            .arg(rolnode.length()>0?QString(" ,\"%1\" ").arg(rolnode):",\"-\"")
                            .arg(uncomplete?tr("Incompleta"):tr("Completada"));


                }

            }

        }

        //QRegExp rx;
        rx.setPattern("info\\.task\\.color:\\s*([a-zA-Z0-9_\\.]+)\\s*,?");

        for(int j=0; j < myfirstlist.count()-nstatsreal; j++) {
            QString nextfield =  myfirstlist.at(j);

            if ( rx.indexIn( nextfield ) != -1 ) {
                if ( days >= prodays ) {
                    porc = 1;
                }
                else {
                    porc = double(days)/double(prodays);
                }

                porc = 1-porc;
                if (isdelay ) {
                  porc = porc*-1;
                }


                nextfield = QString("info.task.color:%1")
                        .arg(porc,0,'g',2);

            }

            result += nextfield;
            result += ",";

        }


            result += statsinfo;



    }
    taskscompleted += "\n]";

    tasksupcoming += "\n]";

   taskcurrent += "\n]";
//   tasks += QString(" \"completed\": %1, \n \"upcoming\": %2, \n \"current\": %3 \n}")
//           .arg(taskscompleted)
//           .arg(tasksupcoming)
//           .arg(taskcurrent);

   //tasks += QString(" \"current\": %3 \n}")
     //      .arg(taskcurrent);

           //taskscompleted +"\n\n" + tasksupcoming + "\n\n" + taskcurrent;
   tasks = QString("{ %1\n  \"tasks\":%2\n }")
           .arg(taskscolumn)
           .arg(taskcurrent);


   result = tasks;

    return result;
}



QString MainWindow::generateGraphCompared(const QString& first, const QString& second, QStringList& taskmark) {
    QString result;
    QStringList myfirstnodelist, mysecondnodelist;
    myfirstnodelist = first.split("\n",QString::SkipEmptyParts);
    mysecondnodelist = second.split("\n",QString::SkipEmptyParts);
    bool ok;
    int prodays = SafetYAWL::getConf()["DefaultValues/graphs.daysproject"].toInt(&ok);
    if (prodays <= 0 ) {
        SYE << tr("El parámetro \"%1\" no tiene un valor válido")
               .arg("DefaultValues/graphs.daysproject");
        return QString();
    }


    QString currentnode;
    for(int i=0; i < myfirstnodelist.count(); i++ ) {
        QString myfirst = myfirstnodelist.at(i);
        QStringList myfirstlist = myfirst.split(",",QString::SkipEmptyParts);
        int rlnodes = myfirstlist.count();
        Q_ASSERT(rlnodes > 0 );
        QStringList statsreal = myfirstlist.at(rlnodes-1).split("...",QString::SkipEmptyParts);
        if (statsreal.count() < 2) {
            continue;
        }

        int ntokens = statsreal.at(0).toInt(&ok);
        if (ntokens > 0 ) {
            currentnode = myfirstlist.at(0);
            break;
        }

    }
    SYD << tr(".....MainWindow::generateGraphCompared....currentnode:|%1|")
           .arg(currentnode);


    if ( currentnode.isEmpty()) {
        SYW << trUtf8("NO se encuentra el nodo donde se encuentra la clave para el gráfico a comparar");
        currentnode = "inicial";
    }

    bool passnode = false;

    for(int i=0; i < myfirstnodelist.count(); i++ ) {
        int days = 0;
        SYD << tr ("....MainWindow::generateGraphCompared...:|%1|")
               .arg(i);

    bool uncomplete = false;

        QPair<double,int>  mypair;
        mypair.first = 0;
        mypair.second = 0;
        QString myfirst = myfirstnodelist.at(i);
        QString mysecond;
        QStringList myfirstlist = myfirst.split(",",QString::SkipEmptyParts);

        bool issecondfound = false;
        for(int j=0; j < mysecondnodelist.count(); j++ ) {
            if ( myfirstlist.at(0) == mysecondnodelist.at(j).section(",",0,0) ) {
                mysecond =  mysecondnodelist.at(j);
                issecondfound = true;
                break;
            }
        }
        if ( !issecondfound ) {
            SYE << tr(" Generando comparación de grafos: no se encontró el \"%1\"")
                   .arg(myfirstlist.at(0));
            return QString();
        }
        QStringList mysecondlist = mysecond.split(",",QString::SkipEmptyParts);

        if (myfirstlist.count() == 0 ) {
            SYD << tr(" Error 1");
            break;
        }
        if (mysecondlist.count() == 0 ) {
            SYD << tr(" Error 2");
            break;
        }
        if (myfirstlist.count() != mysecondlist.count()) {
            SYW << tr("Nodo:|%3| Cuenta nodo guardado:%1....Cuenta planificado:%2").arg(myfirstlist.count())
                   .arg(mysecondlist.count())
                   .arg(myfirstlist.at(0));
            //break;
        }

        QString realnode = myfirstlist.at(0).trimmed();

        if (realnode == "Nodo:final") {
            continue;
        }

        QString plannode = mysecondlist.at(0).trimmed();
        SYD << tr("....MainWindow::generateGraphCompared....realnode:|%1|...plannode:|%2|")
               .arg(realnode)
               .arg(plannode);
        if ( realnode != plannode ) {
            SYE     << tr("Los gráficos seleccionados son diferentes."
                          " No tienen los mismos nodos en la misma posición."
                          " El primer nodo tiene nombre \"%1\". "
                          " El segundo nodo tiene nombre \"%2\"." )
                       .arg(myfirstlist.at(0))
                       .arg(mysecondlist.at(0));
            return QString();

        }

        int rlnodes = myfirstlist.count();
        int plnodes = mysecondlist.count();
        QStringList statsreal = myfirstlist.at(rlnodes-1).split("...",QString::SkipEmptyParts);

    if (myfirstlist.at(rlnodes-1).indexOf("[*]") >= 0 ) {
        SYD << tr(".........generateGraphCompared...MYFIRSTLIST:|%1|")
        .arg(myfirstlist.at(rlnodes-1));
        uncomplete = true;
    }
    else {
        SYD << tr(".........***generateGraphCompared...MYFIRSTLIST:|%1|")
        .arg(myfirstlist.at(rlnodes-1));


    }

        QStringList statsplanned;
    if ( (plnodes-1) < mysecondlist.count() ) {
    statsplanned = mysecondlist.at(plnodes-1).split("...",QString::SkipEmptyParts);
        SYD << tr(".........***generateGraphCompared...MYSECONDLIST:|%1|")
        .arg(mysecondlist.at(rlnodes-1));
    }


        int nstatsreal = 0;
        if ( statsreal.count() > 2 ) {
             nstatsreal = 1;
        }
        double porc = 0;

        QString datestrp;
        QDateTime dateplanned;


        QString statsinfo;

        bool isdelay = false;


        if (statsreal.count() < 4 ) {

            if ( statsplanned.count() < 4 ) {
                   statsinfo =  tr("0...1...n/a...n/a\n");
            }
            else {
                QDateTime mynow = QDateTime::currentDateTime();
                datestrp  = statsplanned.at(3);
                dateplanned = QDateTime::fromString(datestrp,"dd/MM/yyyy hh:mmap");

                if (mynow < dateplanned) {

                    statsinfo = tr("0...1...n/a...Debe entregar en <br/>%1<br/>(%2)\n")
                        .arg(SafetWorkflow::humanizeDate(days,mynow
                                                         .toString("dd/MM/yyyy hh:mmap"),
                                                         "dd/MM/yyyy hh:mmap",
                                                         dateplanned),
                                                         datestrp);

                }
                else {
                    statsinfo = tr("0...1...n/a...Debio entregar hace <br/>%1<br/>(%2)\n")
                        .arg(SafetWorkflow::humanizeDate(days,dateplanned
                                                         .toString(Safet::DateFormat),
                                                         Safet::DateFormat,
                                                         mynow),
                                                         datestrp);
                    isdelay = true;

                }
            }
            QRegExp rx;
            rx.setPattern("info\\.task\\.color:\\s*([\\-a-zA-Z0-9_\\.]+)\\s*,?");

            for(int j=0; j < myfirstlist.count()-nstatsreal; j++) {
                QString nextfield =  myfirstlist.at(j);
                if ( rx.indexIn( nextfield ) != -1 ) {
                    if ( days >= prodays ) {
                        porc = 1;
                    }
                    else {
                        porc = double(days)/double(prodays);
                    }

                    porc = 1-porc;

                    if ( isdelay) {
                        porc = porc*-1;
                    }


                    nextfield = QString("info.task.color:%1")
                            .arg(porc,0,'g',2);

                }

                result += nextfield;
                result += ",";

            }

            if (realnode == QLatin1String("Nodo:inicial")) {
                SYD << tr(".........*MainWindow::generateGraphCompared....result:|%1|")
                       .arg(result);
                SYD << tr(".........*MainWindow::generateGraphCompared....statsinfo:|%1|")
                       .arg(statsinfo);
                result += "\n";
                continue;

            }


                result += statsinfo;

            continue;
        }


        if  (statsplanned.count() < 4 ) {
            SYD << tr(" Error 20");
            break;
        }


        if  (statsreal.count() < 4 ) {
            SYD << tr(" Error 5");
            break;
        }

        datestrp  = statsplanned.at(3);
        dateplanned = QDateTime::fromString(datestrp,"dd/MM/yyyy hh:mmap");



        QString datestr = statsreal.at(3).section("<br/>",0,0);
        QDateTime  datereal = QDateTime::fromString(datestr,"dd/MM/yyyy hh:mmap");




        if ( realnode == currentnode) {

            passnode = true;
            if ( datereal < dateplanned) {
                statsinfo = tr("0...1...(%4) AQUI...%1<br/>entrega:%2<br/>plan.:%3\n")
                        .arg(SafetWorkflow::humanizeDate(days,datestr,"dd/MM/yyyy hh:mmap",
                                                         dateplanned))
                        .arg(datestr)
                        .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
            .arg(uncomplete?"incompleta":"a tiempo");

                if(!uncomplete)
             taskmark.append(realnode);
            }
            else {
                statsinfo = tr("0...1...(%4) AQUI...%1<br/>actual:%3<br/>plan.:%2\n")
                        .arg(SafetWorkflow::humanizeDate(days,datestrp,"dd/MM/yyyy hh:mmap",
                                                         datereal))
                             .arg(datestrp)
                             .arg(datereal.toString("dd/MM/yyyy hh:mmap"))
                 .arg(uncomplete?tr("atraso/incompleta"):tr("atraso"));
                  isdelay = true;

                if(!uncomplete)
                  taskmark.append(realnode);

            }
        }
        else {
            if (!passnode) {
                if ( datereal < dateplanned) {
                    statsinfo = tr("0...1...%4...%1(adelanto)<br/>entrega:%2<br/>plan.:%3\n")
                            .arg(SafetWorkflow::humanizeDate(days,datestr,"dd/MM/yyyy hh:mmap",
                                                             dateplanned))
                            .arg(datestr)
                            .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
            .arg(uncomplete?"incompleta":"a tiempo");

                if(!uncomplete)
                    taskmark.append(realnode);
                }
                else {
                    statsinfo = tr("0...1...%4...%1<br/>actual:%3<br/>plan.:%2\n")
                            .arg(SafetWorkflow::humanizeDate(days,datestrp,"dd/MM/yyyy hh:mmap",
                                                             datereal))
                            .arg(datestrp)
                            .arg(datereal.toString("dd/MM/yyyy hh:mmap"))
                .arg(uncomplete?tr("atraso/incompleta"):tr("atraso"));

                    isdelay = true;

                if(!uncomplete)
                    taskmark.append(realnode);

                }
            }
            else {
                if ( datereal < dateplanned) {
                    statsinfo = tr("0...1...%4...%1(adelanto)<br/>entrega:%2<br/>plan.:%3\n")
                            .arg(SafetWorkflow::humanizeDate(days,datestr,"dd/MM/yyyy hh:mmap",
                                                             dateplanned))
                             .arg(datestr)
                             .arg(dateplanned.toString("dd/MM/yyyy hh:mmap"))
                .arg(uncomplete?"incompleta":"a tiempo");

                if(!uncomplete)
                    taskmark.append(realnode);
                }
                else {
                    statsinfo = tr("0...1...%4...%1<br/>actual:%3<br/>plan.:%2\n")
                            .arg(SafetWorkflow::humanizeDate(days,datestrp,"dd/MM/yyyy hh:mmap",
                                                             datereal))
                             .arg(datestrp)
                            .arg(datereal.toString("dd/MM/yyyy hh:mmap"))
                .arg(uncomplete?tr("atraso/incompleta"):tr("atraso"));
                    isdelay = true;

                if(!uncomplete)
                    taskmark.append(realnode);

                }

            }

        }

        QRegExp rx;
        rx.setPattern("info\\.task\\.color:\\s*([a-zA-Z0-9_\\.]+)\\s*,?");

        for(int j=0; j < myfirstlist.count()-nstatsreal; j++) {
            QString nextfield =  myfirstlist.at(j);

            if ( rx.indexIn( nextfield ) != -1 ) {
                if ( days >= prodays ) {
                    porc = 1;
                }
                else {
                    porc = double(days)/double(prodays);
                }

                porc = 1-porc;
                if (isdelay ) {
                  porc = porc*-1;
                }


                nextfield = QString("info.task.color:%1")
                        .arg(porc,0,'g',2);

            }

            result += nextfield;
            result += ",";

        }


            result += statsinfo;



    }


    return result;
}








QAbstractItemModel *MainWindow::modelFromFile(const QString& fileName)
{
//    QFile file(fileName);
//    if (!file.open(QFile::ReadOnly))
//        return new QStringListModel(completer);

//    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//    QStringList words;

//    while (!file.atEnd()) {
//        QByteArray line = file.readLine();
//        if (!line.isEmpty())
//            words << line.trimmed();
//    }

//    QApplication::restoreOverrideCursor();
    //    return new QStringListModel(words, completer);
    return NULL;

}



void MainWindow::about()
{

    QString alpha, beta, release;
    QRegExp rx;
    rx.setPattern("([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)");

    int pos = rx.indexIn(SafetYAWL::getNumberVersion());
    qDebug("...version: pos: %d",pos);
    if (pos >= 0 ) {
        alpha = rx.cap(2);
        beta = rx.cap(3);
        release = rx.cap(4);
    }

}

// *****************************************************************************************
// Método para la Consola


void MainWindow::doGetSignDocument() {

}


void MainWindow::doSendSignDocument() {

}


bool MainWindow::executeParsed() {


      if  (!commands.contains('f') ) {
           if ( !commands.contains('h') && !commands.contains('V') && !commands.contains('T') ) {
               streamText << tr("*** No se especifico la ruta del archivo de flujo de trabajo (.xml) *** \n");
               streamText  <<  tr("Opcion: -f <archivo> o --file <archivo> \n");
               sendStreamToOutput();
               return false;
          }
      } else {
           if ( configurator != NULL ) {
                delete configurator;
                configurator = new SafetYAWL(_currconfpath+ "/" + Safet::datadir);
           }

           configurator->setAutofilters( commands['a']);
           if (commands.contains('r')) {
               SYD << tr("COMMANDS....r:|%1|").arg(commands['r']);
           }
           configurator->setRecursivefilters( commands['r']);
           SafetYAWL::streamlog.initAllStack();
           configurator->openXML(commands['f']);

           if (queryForErrors()) {
               //delete configurator;
               qDebug("...retornando.....executedParsed");
               return false;
           }

           configurator->openDataSources();

           configurator->convertXMLtoObjects();
           evalConffileValues(); // Evaluar el mapa de configuración actual
           Q_CHECK_PTR(configurator);
           SafetWorkflow* mywf = configurator->getWorkflow();
           Q_CHECK_PTR(mywf);
           SYD << tr("check ADDING_PAR...1");

           checkDefaultValuesParameters();

           SYD << tr("check ADDING_PAR...2");

           evalParsValues(mywf);

      }



     if ( commands.contains('c') ) {
          streamText << tr("Chequeado...!") << endl;
     }

   QMap<int, QString>::const_iterator i;
   for (i = commands.constBegin(); i != commands.constEnd(); ++i) {
        if ( i.key() != 'f' ) {
                bool result = processCommand(i.key() );
                if ( !result ) {
                    return false;
                }
        }
  }

   sendStreamToOutput();

   return true;



}


bool MainWindow::parseArgs(const QString& a) {

    commands.clear();
    QStringList args = a.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    int currentOption = 0;
    foreach( QString par, args) {
         QRegExp rx;
         rx.setPattern("\\-[a-zA-Z]");
         int pos = rx.indexIn(par);
         if ( pos > -1 )  {
               Q_ASSERT( par.length() == 2 );
               currentOption = (int)(par.at(1).toAscii());
               qDebug("currentOption: %c", currentOption);
               commands.insert(currentOption,"");
         }
         else {
              Q_ASSERT( commands.contains(currentOption) );
              qDebug("commands['%c'] = %s", currentOption, qPrintable(par));
              commands[currentOption] = par;
         }

    }

}

bool MainWindow::parse(int &argc, char **argv) {
       int c;
       int verbose_flag = 0;
       qDebug("argv[0]: |%s|", argv[0]);
       qDebug("argv[1]: |%s|", argv[1]);
        parsed = true;
       while (1)
         {
             struct option long_options[] =
             {
               /* These options set a flag. */
               {"verbose", no_argument,       &verbose_flag , 1},
               {"brief",   no_argument,       &verbose_flag, 0},
               /* These options don't set a flag.
                  We distinguish them by their indices. */
               {"listtasks",   no_argument,       0, 'l'},
               {"listconditions",   no_argument,       0, 'C'},
               {"check",    no_argument,       0, 'c'},
               {"listdocuments",    no_argument,       0, 'd'},
               {"data",    no_argument,       0, 'D'},
               {"file",  required_argument, 0, 'f'},
               {"autofilter",  required_argument, 0, 'a'},
               {"recursivefilter",  required_argument, 0, 'r'},
               {"task",  required_argument, 0, 't'},
                {"template",  no_argument, 0, 'T'},
               {"variable",  required_argument, 0, 'v'},
               {"json",  required_argument, 0, 'j'},
               {"graph",  required_argument, 0, 'g'},
               {"sign",  no_argument, 0, 's'},
               {"keyvalue",  required_argument, 0, 'k'},
               {"plugins",  required_argument, 0, 'p'},
               {"stat",  required_argument, 0, 'S'},
               {"help",  no_argument, 0, 'h'},
               {"version",  no_argument, 0, 'V'},
               {0, 0, 0, 0}
             };
           /* getopt_long stores the option index here. */
           int option_index = 0;

           c = getopt_long (argc, argv, "lCcf:a:t:Tj:dv:g:k:sS:hVD:p:",
                            long_options, &option_index);

           if (c == -1)
             break;
//           qDebug("optarg: %s", optarg);
           switch (c)
             {
             case 0:
               /* If this option set a flag, do nothing else now. */
               if (long_options[option_index].flag != 0)
                 break;
               streamText << tr("Opcion: ") << long_options[option_index].name ;
               if (optarg)
                 streamText << tr(" con argumento: ") <<  optarg << endl;
               break;
             case 'l':
                        commands['l'] = "list";
               break;
             case 'C':
                        commands['C'] = "list";
               break;
             case 'd':
                        commands['d'] = "documents";
               break;
             case 'D':
                        commands['D'] = optarg;
               break;
             case 'c':
                        commands['c'] = "check";
                       break;
             case 'p':
                        commands['p'] = optarg;
                       break;
              case 'f':
                        commands['f'] = optarg;
                       break;
              case 'a':
                       if (!commands.contains('a') ) {
                            commands['a'] = optarg;
                       }
                       else {
                            commands['a'] = commands['a']+"," +QString(optarg);
                       }
                       break;
              case 'r':
                       if (!commands.contains('r') ) {
                            commands['r'] = optarg;
                       }
                       else {
                            commands['r'] = commands['r']+"," +QString(optarg);
                       }
                       break;
              case 'g':
                        commands['g'] = optarg;
                       break;
             case 'v':
                        commands['v'] = optarg;
                        //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!commands['v'].trimmed().startsWith("-"), tr("Debe especificar un nombre de variable, falta este parametro")) );
                        if (commands['v'].trimmed().startsWith("-") ) {
                            SafetYAWL::streamlog
                                    << SafetLog::Error
                                    <<
                            tr("Debe especificar un nombre de variable, falta este parametro");
                        }
                       break;
             case 'k':
                        commands['k'] = optarg;
//			(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!commands['k'].trimmed().startsWith("-"), tr("Debe especificar un valor para la clave, falta este parametro")) );
                        break;
             case 't':
                        commands['t'] = optarg;
                        break;
             case 'T':
                        commands['T'] = "template";
                        break;
             case 's':
                        commands['s'] = "sign";
                        break;
             case 'S':
                        commands['S'] = optarg;
                        break;
             case 'h':

                        commands['h'] = "help";
                        break;
             case 'V':
                        commands['V'] = "version";
             case '?':
               /* getopt_long already printed an error message. */
               break;

             default:
                        parsed = false;
             }
         }

       //	qDebug("!commands.contains('f'): %d", !commands.contains('f'));
        if ( commands.contains('h') || commands.contains('V') || commands.contains('T'))  {
            return true;
        }
        if ( !commands.contains('f')  ) {
                streamText << tr("*** No se especifico la ruta del archivo de flujo de trabajo (.xml) *** \n");
                streamText  <<  tr("Opcion: -f <archivo> o --file <archivo> \n");

                sendStreamToOutput();
                parsed = false;
                processCommand('f');
        }

       if (verbose_flag)
         streamText << tr("La opción (verbose) está activa\n");

       /* Print any remaining command line arguments (not options). */
       if (optind < argc)
         {
//	   qDebug("optind: %d argc: %d", optind, argc);
           streamText << tr("Parametro(s) invalido(s): ");
           while (optind < argc)
             streamText <<  argv[optind++];
             streamText << endl;
                parsed = false;
         }

        if (!parsed ) return false;

        configurator->openXML(commands['f']);
  //      configurator->openDataSources();
        configurator->convertXMLtoObjects();

        // llamada a la funcion que habilita las fuentes de datos
        //qDebug("MainWindow: configurator->openDataSources()");

        //qDebug("SafetYAWL::databaseOpen: %d", SafetYAWL::databaseOpen);

        configurator->loadPlugins();

        if ( commands.contains('c') ) {
                streamText << tr("Chequeado...!") << endl;
        }

        return parsed;
}


void MainWindow::sendStreamToOutput() {
     streamText.flush();
    outputText = "";

}


bool MainWindow::processCommand(int command) {
        bool result = true;
        Q_CHECK_PTR( configurator->getWorkflows().at(0) );
        QDateTime fromdate;

        switch ( command ) {
        case 'f':
            if (commands['f'].length() == 0 ) {
                return false;
            }
                break;
        case 'l':
                listTasks();
                break;
        case 'd':
                SYD << tr("...MainWindow::processCommand......listDocuments...(1)...");
                listDocuments();
                break;
        case 'D':
                manageData();
                break;
        case 'g':
                fromdate = QDateTime::currentDateTime();
                result = genGraph();                
                SYD << tr("......MainWindow::proccessCommand...seconds:|%1|").arg(fromdate.secsTo(QDateTime::currentDateTime()));

                break;
        case 's':
                signDocument();
                break;
        case 'S':
                calStatistics();
                break;
        case 'T':
                /* now Nothing */
                break;
        case 'h':
                displayUsage();
                break;
        case 'V':
                version();
                break;

        default:;
        }

        return result;
}



void MainWindow::displayUsage() {
streamText << tr("Uso: safet --file <archivo de flujo de trabajo> [comandos]") << endl;
streamText << tr("     safet -f <archivo de flujo de trabajo> [comandos]") << endl;
streamText << tr("Cliente Safet de linea de comandos, version 0.1.beta") << endl;
streamText << tr("Tipee 'safet --help ' para ayuda sobre los comandos.") << endl;
streamText << tr("Tipee 'safet --version' o 'safet -V' para ver la version del cliente de linea de comandos.") << endl;
streamText << endl;
streamText << endl;
streamText << tr("Los comandos reciben parametros de tipo variable, clave o subcomando, o algunos") << endl;
streamText << tr("de los comandos no requieren ningun parametro. Si no se proveen parámetros a") << endl;
streamText << tr("estos comandos, se generara un mensaje de error.") << endl;
streamText << endl;
streamText << tr("Comandos disponibles:") << endl;
streamText << endl;
streamText << tr("-f --file=ARCHIVO\tIndica el archivo (ARCHIVO) de flujo de trabajo a procesar]") << endl;
streamText << tr("\t\t\tGeneramente debe tener extension .xml y se valida con el archivo de") << endl;
streamText << tr("\t\t\tdefinicion yawlworkflow.dtd") << endl;
streamText << endl;
streamText << tr("-v --variable=NOMBRE\tSelecciona el nombre de la variable a procesar. La variable") << endl;
streamText << tr("\t\t\tdebe estar incluida en el archivo de flujo de trabajo a procesar, e identificada") << endl;
streamText << tr("\t\t\tcon el atributo 'id'.") << endl;
streamText << endl;
streamText << tr("-k --key=CLAVE\t\tIndica la clave para un comando de firma, u otro comando que") << endl;
streamText << tr("\t\t\tlo requiera, generalmente debe ir acompanada del parametro que selecciona el nombre") <<endl;
streamText << tr("\t\t\tde la variable") << endl;
streamText << endl;
streamText << tr("-s --stat=SUBCOMANDO\tGenera la estadistica seleccionada en el subcomando SUBCOMANDO.") << endl;
streamText << tr("\t\t\tLos subcomandos posibles son \"ndocs\" (lista la cantidad de documentos para todas las") << endl;
streamText << tr("\t\t\tvariables) y \"path\" (indica en que actividad/tarea se encuentra el token segun") << endl;
streamText << tr("\t\t\tla clave indicada con el comando -k o --key)") << endl;
streamText << endl;
streamText << tr("-g --graph=ARCHIVO\tGenera un archivo grafico (png,svg,etc.) del flujo") << endl;
streamText << tr("\t\t\tde trabajo. La extension del archivo indica el formato en que se escribe.") << endl;
streamText << tr("\t\t\tSi no se especifica parametro se genera por defecto un archivo con ") << endl;
streamText << tr("\t\t\tel nombre 'output.png' en el directorio actual") << endl;
streamText << endl;
streamText << tr("-c --check\t\tChequea que el archivo de flujo de trabajo indicado") << endl;
streamText << tr("\t\t\ten la opcion -f o --file cumpla todas las reglas (Sintaxis,enlace con") << endl;
streamText << tr("\t\t\trepositorio de datos, etc.) para ser procesado por un comando") << endl;
streamText << endl;
streamText << tr("-l --listtasks\t\tLista todas las tareas/actividades que contiene el archivo") << endl;
 streamText << tr("\t\t\tde flujo de trabajo indicado en la opcion -f o --file") << endl;
streamText << endl;
streamText << tr("-V --version\t\tMuestra la version actual de la aplicacion de linea ") << endl;
streamText << tr("\t\t\tde comandos 'safet'.") << endl;
streamText << endl;
streamText << tr("-h --help\t\tMuestra este texto de ayuda") << endl;
streamText << endl;
streamText << tr("-d --listdocuments\tMuestra los documentos presentes en la variable indicada") << endl;
streamText << tr(" \t\t\tcon el parametro -v o --variable") << endl;
streamText << endl;
streamText << endl;
streamText << tr("Safet es una herramienta para gestion de flujos de trabajos basados en Redes de ") << endl;
streamText << tr("Petri y patrones (AND,OR,XOR, etc), y que incorpora Firma ") << endl;
streamText << tr("Electronica bajo el modelo de Infraestructura de Clave Publica.") << endl;
streamText << endl;
streamText << endl;
streamText << tr("  Software utilizado principalmente en safet:") << endl;
streamText << endl;
streamText << tr("  Libreria Qt:  referirse a: http://www.trolltech.com") << endl;
streamText << tr("   LibDigidoc:  referirse a: http://www.openxades.org") << endl;
streamText << tr("     Graphviz:  referirse a: http://www.graphviz.org") << endl;
streamText << tr("        DbXml:  referirse a: http://www.sleepycat.com") << endl;
streamText << endl;

}


void MainWindow::calStatistics() {

        if ( commands['S'].compare("ndocs", Qt::CaseInsensitive ) == 0 ) {
                streamText << "Estadisticas para el archivo: <" << commands['f'] << ">:" << endl;
                foreach(QString name,configurator->getWorkflows().at(0)->variablesId()){
                        SafetVariable *v = configurator->getWorkflows().at(0)->searchVariable( name );
                        Q_CHECK_PTR( v );
                        streamText << tr("Variable <") << name << tr(">  Numero de fichas: <") << configurator->getWorkflows().at(0)->numberOfTokens(*v) << ">" << endl;
                }
                streamText << endl;
        } else if ( commands['S'].compare("path", Qt::CaseInsensitive ) == 0 ) {
                //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(commands['k'].length() > 0 , tr("No se especifico el valor de la clave (--keyvalue <valor>) para firmar (sign) el documento")) );
                SafetYAWL::evalAndCheckExit(commands['k'].length() > 0 , tr("No se especifico el valor de la clave (--keyvalue <valor>) para firmar (sign) el documento"));

                QString info = 	commands['k'];
                (*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(commands['t'].length() > 0 , tr("No se especifico un nombre de tarea  (--task <nombre>) para calcular e imprimir los caminos")) );
                QString result = configurator->getWorkflows().at(0)->printPaths(commands['t'],info);
                streamText << tr("Camino(s) para : ") << info << endl;
                streamText << result << endl;
        } else {
                streamText << tr("Argumento del comando Estadisticas -S <arg> o -stat <arg> invalido. Opciones: ndocs, percent, path") << endl;
        }
}



void MainWindow::signDocument() {
        //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(commands['k'].length() > 0 , tr("No se especifico el valor de la clave (--keyvalue <valor>) para firmar (sign) el documento")) );
        SafetYAWL::evalAndCheckExit(commands['k'].length() > 0 , tr("No se especifico el valor de la clave (--keyvalue <valor>) para firmar (sign) el documento"));

        //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(commands['v'].length() > 0 , tr("No se especifico un nombre de variable  (--variable <nombre>) para firmar (sign) el documento")) );
        SafetYAWL::evalAndCheckExit(commands['v'].length() > 0 , tr("No se especifico un nombre de variable  (--variable <nombre>) para firmar (sign) el documento"));


        SafetVariable *v = configurator->getWorkflows().at(0)->searchVariable( commands['v'] );

        //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(v != NULL , tr("No existe la variable : \"%1\" en el documento de flujo de trabajo").arg(commands['v'])) );
        SafetYAWL::evalAndCheckExit(v != NULL , tr("No existe la variable : \"%1\" en el documento de flujo de trabajo").arg(commands['v']));

        QString derror;
        QString documentid = configurator->getWorkflows().at(0)->signDocument(v, commands['k'],derror);
        if (documentid.length() > 0 ) {
          streamText << tr("Documento con id:")  << documentid << " firmado correctamente OK!" << endl;
        }
        else {
           streamText << " NO se introdujo la CLAVE del documento a firmar. NO se realizo la FIRMA." << endl;
        }

}


bool MainWindow::genGraph() {
       char buffer[512];



        QString info;
        if ( commands.contains('p') ) {
            qDebug("***...command['p']: |%s|",qPrintable(commands['p']));
          configurator->loadPlugins(commands['p']);
        }

        if ( commands.contains('k') ) {
                info = 	commands['k'];
        }

        strncpy(buffer, qPrintable(SafetYAWL::getConf()["Plugins.Graphviz/graphtype"]), 4);
        SYD << tr("...MainWindow::doRenderGraph.....buffer:|%1|")
               .arg(buffer);

        SafetYAWL::streamlog.initAllStack();
        QString myjson;
	SYD << tr("............MainWindow::genGraph()....TIMECHECK. (1)...........");
        QString img = configurator->getWorkflows().at(0)->generateGraph(buffer, myjson,info);

	SYD << tr("............MainWindow::genGraph()....TIMECHECK. (2)...........");

       SYD << tr("\n\n...MainWindow::genGraph().......:\n|%1|")
		.arg(img);

       _currenttable = myjson;

       SYD << tr("\n\n...MainWindow::genGraph()..............currentTable:\n|%1|")
        .arg(_currenttable);

       if ( img.isEmpty() ) {
               queryForErrors();

            return false ;
        }

        //SafetYAWL::lastinfodate = "";

        _currentjson =  drawWorkflow(img); // Dibujar el flujo
        QString showjson = SafetYAWL::getConf()["Result/graph.formatjson"];

        if (showjson.compare("on",Qt::CaseSensitive) == 0) {

            QString myid = configurator->getWorkflows().at(0)->id();
            SYD << tr("MainWindow.:genGraph...*MYID (1):|%1|")
                   .arg(myid);

            //myid = MainWindow::replaceMarks(myid);
            SYD << tr("MainWindow.:genGraph...*MYID (2):|%1|")
                   .arg(myid);

            QString mydata = QString("{ \"filename\": \"%1\", \"id\": \"%3\",\"data\": %2 }")
                                .arg(_currentjson)
                                .arg(configurator->getWorkflows().at(0)->currentGraphJSON(""))
                                .arg(myid);
            QString searchid = MainWindow::normalize(myid);
            SYD << tr("MainWindow.:genGraph...GENGRAPH...SCRIPTENGINE...*MYID (2):|%1|")
                   .arg(searchid);

            QString myscript = searchScript(searchid);

            QString alltips = "";
            if (  !myscript.isEmpty() ) {
                    SYD << tr("...EVALTIPS....GENGRAPH...SCRIPTENGINE...mydir....(1) exist");
                    SYD << tr("...EVALTIPS....GENGRAPH...SCRIPTENGINE...mydir....(1) myscript:\n|%1|")
                           .arg(myscript);

                    QStringList mytips = generateTips(mydata,myscript);
                    SYD << tr("...EVALTIPS.....GENGRAPH...SCRIPTENGINE..mydir....(1) mytips.count():\n|%1|")
                           .arg(mytips.count());
                    if (mytips.count() > 0) {
                        QString tipsresult;
                        tipsresult += "  \"safettips\": [";
                        foreach(QString t, mytips) {
                            tipsresult += "{ \"tip\": \"" + t +"\"},";
                        }
                        tipsresult.chop(1);
                        tipsresult += "],\n";

                        alltips += " ";
                        alltips += tipsresult;
                        alltips += "";


                    }

            }

            SYD << tr("MainWindow.:genGraph...GENGRAPH...SCRIPTENGINE...*alltips (2):|%1|")
                   .arg(alltips);


            _currentjson = QString("{ \"filename\": \"%1\", \"id\": \"%3\", %4 \"data\": %2 }")
                    .arg(_currentjson)
                    .arg(configurator->getWorkflows().at(0)->currentGraphJSON(""))
                    .arg(myid)
                    .arg(alltips);

        }




        return true;
}

QString MainWindow::normalize(const QString& text) {
    QString normtl  = text;

    normtl.replace(QRegExp("\\s+")," ");
    normtl.replace("á","a");
    normtl.replace("é","e");
    normtl.replace("í","i");
    normtl.replace("ó","o");
    normtl.replace("ú","u");
    normtl.replace("ñ","n");
    normtl.replace("Ñ","N");
    normtl.replace(" ","_");

    return normtl;
}


void MainWindow::doCipherFile(bool iscipher) {

#ifdef SAFET_OPENSSL // Criptosistema OpenSSL
    // ** Cifrando
    if (iscipher) {
        QString fileconf = SafetYAWL::pathconf+ "/" + "auth.conf";
        SafetCipherFile myfile(fileconf);
        qDebug("doCipherFile fileconf: %s", qPrintable(fileconf));
        myfile.open(QIODevice::ReadOnly);
        myfile.setPassword("caracas");
        QString fileconfcipher = SafetYAWL::pathconf+ "/" + "auth.conf.bin";
        qDebug("doCipherFile fileconfcipher: %s", qPrintable(fileconfcipher));
        myfile.cipherInFile(fileconfcipher);
        qDebug("...ok!");
        myfile.close();
    }
    else {
        // ****** Leyendo cifrado
        QString fileconf = SafetYAWL::pathconf+ "/" + "auth.conf.bin";
        SafetCipherFile myfile(fileconf);
        myfile.open(QIODevice::ReadOnly);
        myfile.setPassword("caracas");
        myfile.readAllCipher();
        QString line;
        while( true ) {
            line = myfile.readCipherLine();
            if (line.isNull()) {
                break;
            }
            qDebug("%s",qPrintable(line));
        }

    }
#endif // OpenSSL
}
void MainWindow::setParsValues(const QMap<QString,QString>& values) {
    SYD << tr("..........MainWindow::setParsValues.....values.count():|%1|")
           .arg(values.count());

    _currparsvalues = values;
}


void MainWindow::evalParsValues(SafetWorkflow *wf) {

               SYD << tr("MainWindow::evalParsValues()..._currparsvalues.keys().count():%1")
                  .arg(_currparsvalues.keys().count());
    Q_CHECK_PTR(wf);
  //  QMap<QString,QString> mypars;
  //  bool ok;
   // QVector<QString> mytmpvt(20);
    int countpars = _currparsvalues.keys().count();
    if ( countpars > 20 ) {
                   SYE << tr("El número de parametros del flujo de trabajo es excesivo: \"%1\""
                         ".Tiene que ser menor o igual a veinte (20)")
                      .arg(countpars);
        return;
    }


    wf->putParameters(_currparsvalues);
    SYD << tr("....evalParsValues...AUTO_REPLACE*");
    wf->evalAutofilters();
    wf->evalRecursivefilters();

}

void MainWindow::setConffileValues(const QMap<QString,QString>& values) {
    _currconfvalues = values;
}

void MainWindow::evalConffileValues() {


           SYD    << tr("MainWindow::evalConffileValues()...EVALCONFFILEVALUES....keys().count():%1")
                  .arg(_currconfvalues.keys().count());


    SafetWorkflow* mywf = configurator->getWorkflow();
    SYD << tr("....MainWindow::evalConffileValues...mywf(1)...");
  //  Q_CHECK_PTR(mywf);
    SYD << tr("....MainWindow::evalConffileValues...mywf(2)...LOADINGCONF");

    if (mywf == NULL ) {
        return;
    }
    QList<SafetParameter*> mypars = mywf->getParameterlist();

    SYD << tr("....MainWindow::evalConffileValues...mywf(3)..LOADINGCONF.");
    QMap<QString,QString>  mymap;

    QList<SafetParameter*>::const_iterator it;


    for(it = mypars.constBegin(); it != mypars.constEnd(); ++it) {
        SafetParameter *p = *it;
        if (!p->defaultvalue().isEmpty()) {
            mymap[p->configurekey()] = p->defaultvalue();
            SYD    << tr("MainWindow::evalConffileValues()...(DEFAULTVALUE)...configurekey:|%1|....value:|%2|")
                      .arg(p->configurekey())
                      .arg(p->defaultvalue());
            if ( SafetYAWL::getConf().getMap().contains(p->configurekey())) {
                    SafetYAWL::getConf().getMap()[p->configurekey()] = p->defaultvalue();
            }
        }
    }

    SYD << tr("....MainWindow::evalConffileValues...MYWF...mywf(4)..._currconfvalues.keys().count():|%1|")
           .arg(_currconfvalues.keys().count());

//    foreach(QString key, SafetYAWL::getConf().getMap().keys()) {

//        SYD << tr("CONF_KEY...key:|%1|...value:|%2|")
//               .arg(key)
//               .arg(SafetYAWL::getConf().getMap()[key]);


//    }


    for(int i=0; i< _currconfvalues.keys().count();i++) {
        QString mykey = _currconfvalues.keys().at(i);
        QString myvalue = _currconfvalues[mykey];
        SYD    << tr("MainWindow::evalConffileValues()...CONTAINS....**mykey:|%1|")
                  .arg(mykey);
        if ( SafetYAWL::getConf().getMap().contains(mykey)) {
            SYD    << tr("MainWindow::evalConffileValues()...CURRVALUE....mycurrvalue:|%1|")
                      .arg(SafetYAWL::getConf().getMap()[mykey]);
            SYD    << tr("MainWindow::evalConffileValues()...CONFKEY....mykey:|%1|")
                      .arg(mykey);
            SYD    << tr("MainWindow::evalConffileValues()...***CONFVALUE....myvalue:|%1|")
                      .arg(myvalue);
            SafetYAWL::getConf().getMap()[mykey] = myvalue;

                        SYD    << tr("MainWindow::evalConffileValues()...CONFVALUE....SETTING...");


        }
    }



    _currconfvalues.clear();
    SYD << tr("....MainWindow::evalConffileValues...mywf(5)...");

}

void MainWindow::listTasks()
{
        Q_CHECK_PTR( MainWindow::configurator );
        QString outtasks = configurator->getWorkflows().at(0)->listTasks( commands['C'].length() > 0 );
        streamText << "Lista de identificadores  de tareas/actividades:"  << endl;
                streamText << outtasks;
}



void MainWindow::manageData() {
//	streamText << "Data: <" << commands['D'] << ">" << endl;
        SafetTextParser parser;
        parser.setStr( commands['D'].toUtf8() );
        QString str = "agregar,eliminar,actualizar,mostrar";
        QString commandstr = "INSERT INTO,DELETE,UPDATE,SELECT";
        parser.setCommands(str.split(","),commandstr.split(","));
        QString xml = parser.toXml();
        SafetYAWL::streamlog << SafetLog::Debug << tr("Cadena XML generada de la entrada: \n\"%1\"").arg(xml);
//	qDebug("\n\n....xml:\n%s", qPrintable(xml));
        parser.processInput( xml );
        parser.openXml();
        parser.processXml();
}


void MainWindow::version() {
streamText << SafetYAWL::toUcode(QString(INFO_VERSION)) << endl;

}


QMap<QString,QDateTime> MainWindow::doGetPlannedGraph(const QString& namegraph) {
    QMap<QString,QDateTime> result;

    doRestoreGraph(Safet::PLANNEDGRAPHSFILENAME);
    if (graphs().contains(namegraph)) {
        SYW << tr("El nombre de grafo \"%1\" no se encuentra en el repositorio")
               .arg(namegraph);
        return result;
    }

    QString code =  graphs()[namegraph].first;

    if (code.isEmpty()) {
        SYW << tr("El registro para el grafo \"%1\" está vacío")
               .arg(namegraph);
        return result;
    }

    QStringList mylist = code.split("\n",QString::SkipEmptyParts);

    foreach(QString node, mylist) {
        QStringList flist = node.split(",",QString::SkipEmptyParts);
        QString s = flist.at(flist.count()-1);
        SYD << tr(".......MainWindow::doGetPlannedGraph...s:|%1|")
               .arg(s);
        QStringList parlist = s.split("...",QString::SkipEmptyParts);
        Q_ASSERT(parlist.count() > 3 );
        SYD << tr("parlist.count() > 3 ");
        QDateTime mydate;
        mydate.fromString(parlist.at(3));


    }

    return result;
}


void MainWindow::listDocuments(const QString& key) {
//    char buffer[20];
//    strncpy(buffer, qPrintable(SafetYAWL::getConf()["Graphviz.Plugins/graphtype"]), 3);
//    buffer[3] = 0;
    QString code = configurator->getWorkflows().at(0)->generateCodeGraph("svg",key,false);



    SYD << tr("..............****MainWindow::listDocuments....");

    SYD << tr(".....listDocuments...MainWindow::listDocuments....code:\n|%1|\n")
           .arg(code);
    SYD << tr("..............****MainWindow::listDocuments....");



    QList<SafetWorkflow::Miles> ordermiles;

    QString namefields = "completekey,wftask,currkey,rol,timeprev,timecurr,secondswait,porc";

    QString namevalues;



    if(!code.isEmpty()) { // !code.isEmpty
        SYD << tr("1.Existe: %1").arg(QFile::exists("/home/vbravo/.safet/stats/stats.db"));
        SafetBinaryRepo myrepo;

        if (!myrepo.fileExists()) {
            myrepo.open();
            if (!myrepo.createTable("graphs","completekey text primary key,wftask text, currkey text,"
                                    "rol text,timeprev integer, timecurr "
                               "integer, secondswait integer, porc integer"))  {
               SYW << tr("...MainWindow::listDocuments....No se pudo crear el archivo de estadísticas");
            }
            else {
               SYW << tr("...MainWindow::listDocuments....Creado el archivo de estadísticas");
            }

        }
        {
            myrepo.open();
        }

        QMap<QString,QDateTime> myplanneds = doGetPlannedGraph("POA Proyecto Seguridad Formato DDOC 2011 Version 1");
        SafetWorkflow::ExtraInfoSearchKey& sk = configurator->getWorkflows().at(0)->searchKey();

        QDateTime previewdate;

        for( int i = 0; i < sk.extrainfo.keys().count(); i++) {
            QString mykey = sk.extrainfo.keys().at(i);
            SYD << tr("...%1->%2")
                   .arg(mykey)
                   .arg(sk.extrainfo[ mykey]);

            QString mydata = sk.extrainfo[ mykey];
            QStringList mylist = mydata.split(QRegExp(";|<br/>"));
            if (mylist.count() < 3 ) {
                continue;
            }
            QDateTime mydate = QDateTime::fromString(mylist.at(1),"dd/MM/yyyy hh:mmap");
            if (!mydate.isValid()) {
                SYD << tr(".....listDocuments....no Valid datetime");
                continue;
            }

            SafetWorkflow::Miles mymile;
            mymile.nametask = mykey;

            mymile.ts = mydate;

            mymile.secondswait = 0;


            mymile.humandate = mylist.at(2);
            QStringList myinfolist = mylist.at(0).split(QRegExp("\\s+"),QString::SkipEmptyParts);
            mymile.rol = myinfolist.at(0);
            mymile.porc = "";
            if (myinfolist.count() > 1 ) {
                mymile.porc = myinfolist.at(1);
            }

            int pos = -1;
            for(int j=0; j < ordermiles.count(); j++) {
                if ( mymile.ts  < ordermiles.at(j).ts ) {
                    pos = j;
                    break;

                }
            }
            if (pos == -1 ) {
                pos = ordermiles.count();
            }
            SYD << tr ("....MainWindow::listDocuments insertando en la posicion: %1")
                   .arg(pos);
            ordermiles.insert(pos,mymile);
            previewdate  = mydate; // Colocar fecha anterior
        }


        for(int i = 0; i< ordermiles.count();i++) {
            int days;
            SafetWorkflow::Miles &m = ordermiles[i];

            m.humanwait = tr("n/a");

            namevalues =  "'"+configurator->getWorkflows().at(0)->id()+m.nametask+key+"',";
            namevalues += "'"+configurator->getWorkflows().at(0)->id()+m.nametask+"',";
            namevalues += "'"+key+"',";
            namevalues += "'"+m.rol +"',";
            if ( i > 0 ) {
                SafetWorkflow::Miles &prev = ordermiles[i-1];
                m.secondswait = m.ts.toTime_t()-prev.ts.toTime_t();
                m.humanwait = SafetWorkflow::humanizeDate(days,prev.ts.toString("dd/MM/yyyy hh:mm:ssap"),
                                                             "dd/MM/yyyy hh:mm:ssap",
                                                             m.ts,
                                                             SafetWorkflow::WaitTime);

                SYD << tr("...........m.prevdate:|%1|").arg(prev.ts.toTime_t());
                SYD << tr("m.secondstowait (prev):|%1|").arg(prev.secondswait);

                namevalues += QString("%1,").arg(prev.ts.toTime_t());
                namevalues += QString("%1,").arg(m.ts.toTime_t());
                namevalues += QString("%1,").arg(m.ts.toTime_t()-prev.ts.toTime_t());
                QString mporc = m.porc;
                if (mporc.endsWith("%")) {
                    mporc.chop(1);
                }
                namevalues += QString("%1").arg(mporc);

            }
            else {
                namevalues += QString("%1,").arg(0);
                namevalues += QString("%1,").arg(m.ts.toTime_t());
                namevalues += QString("%1,").arg(0);
                QString mporc = m.porc;
                if (mporc.endsWith("%")) {
                    mporc.chop(1);
                }
                namevalues += QString("%1").arg(mporc);
            }

            SYD << tr("...........m.task:|%1|").arg(m.nametask);
            SYD << tr("...........m.date:|%1|").arg(m.ts.toTime_t());
            SYD << tr("...........m.porc:|%1|").arg(m.porc);
            SYD << tr("m.rol:|%1|").arg(m.rol);

            myrepo.append("graphs",namefields, namevalues);

            SYD << tr(".........MainWindow::listDocuments....namefields:|%1|")
                   .arg(namefields);
            SYD << tr(".........MainWindow::listDocuments....namevalues:|%1|")
                   .arg(namevalues);

        }



        QString myvalues = myrepo.search("graphs");
        SYD << tr("........MainWindow::listDocuments...values...(1):\n|%1|\n")
               .arg(myvalues);


        QList<QSqlField> myfields;
        _listprincipalcount = ordermiles.count();
        _listprincipaltitle = tr("Lista por Clave \"%1\"")
                                 .arg(key)+_listprincipaltitle;
        QString myjson = SafetWorkflow::getJSONMiles(ordermiles,myfields);

         SYD << tr("...MainWindow::listDocuments...myjson (documents):\n|%1|\n")
                .arg(myjson);
        _listprincipalvariable = "";
        _listprincipalkey = key;

        SYD << tr("...antes de ejecutar: executeJSCodeAfterLoad");
       // generateJScriptreports(myjson,myfields);
   //     executeJSCodeAfterLoad();
        SYD << tr("...después de ejecutar: executeJSCodeAfterLoad");



    } // !code.isEmpty



}


void MainWindow::listDocuments() {

     SYD << tr("...*listDocuments...commands['v']: |%1|").arg(commands['v']);

     QString info;
     if ( commands.contains('k') ) {
             info = 	commands['k'];
             listDocuments(info);
             SYD << tr("......MainWindow::listDocuments.........LISTDOCUMENTSINFO: |%1|")
                    .arg(info);

             return;
     }

        SafetVariable* var = configurator->getWorkflows().at(0)->searchVariable( commands['v'] );

        if (var == NULL ) {
                streamText << tr("La variable \"%1\" NO existe en la especificacion de flujo de trabajo. Revise el nombre en el documento de flujo de trabajo").arg(commands['v']) << endl;
                SYE
                        <<  tr("La variable \"%1\" NO existe en la especificacion de flujo de trabajo."
                               "Revise el nombre en el documento de flujo de trabajo")
                        .arg(commands['v']);
                return;
        }

        if ( commands.contains('p') ) {
          configurator->loadPlugins(commands['p']);
        }


        SafetVariable *myvar = configurator->getWorkflows().at(0)->searchVariable((commands['v']));

        if ( myvar != NULL ) {
            if ( !myvar->description().isEmpty()) {
                _listprincipaltitle = myvar->description()+_listprincipaltitle;
            }
            else {
                _listprincipaltitle = QString("%1%2").arg(commands['v']).arg(_listprincipaltitle);
            }
        }
        else {
            _listprincipaltitle = QString("%1%2").arg(commands['v']).arg(_listprincipaltitle);

        }

        QList<QSqlField> fields, fields1;
         QString documents = configurator->getWorkflows().at(0)->getDocuments(commands['v'],
        fields,_listprincipalcount,
        SafetWorkflow::JSON, info).documents();


         currentADocuments = configurator->getWorkflows().at(0)->getDocuments(commands['v'],
        fields1,_listprincipalcount,
        SafetWorkflow::JSONARRAY, info).documents();

         SYD << tr("....MainWindow::listDocuments...........LISTDOCUMENTS...ADOCUMENTS:|%1|")
                .arg(currentADocuments);


         _listprincipalvariable = commands['v'];
         _listprincipalkey = "";

        generateJScriptreports(documents,fields);
        executeJSCodeAfterLoad();

}


void MainWindow::iterateMap(const QMap<QString,QVariant>& map) {

     QList<QVariant> mylist = map.values("record");
     for ( int i = 0 ; i < mylist.count(); i++) {
          qDebug("\n..%d. Registro:", i+1);
          qDebug("id: %s", qPrintable(mylist.at(i).toMap()[ "id" ].toString()));
          qDebug("description: %s", qPrintable(mylist.at(i).toMap()[ "description" ].toString()));
          qDebug("summary: %s", qPrintable(mylist.at(i).toMap()[ "summary" ].toString()));
     }

}







void MainWindow::successVerification(QStringList list, const QString& msg) {
/*  // funcion comentada para eliminar dependencia de digidocpp
    qDebug("...........**......MainWindow::successVerification(QStringList list, const QString& msg) {");
    Q_CHECK_PTR(dockShowResult);
    QString message;
    if ( msg.isEmpty()) {
        message = QString("<table><tr><td><font color=green>%1</font></td></tr></table>").arg(tr("Verificación exitosa....<b>ok!</b>"));
    }
    else {
        message = QString("<table><tr><td><font color=green>%1</font></td></tr></table>").arg(msg);
    }

    dockShowResult->addHeadHtml(message);
    qDebug("....successVerification........message: %s",qPrintable(message));
    dockShowResult->addOptionsDigidoc(digiDocument,list);
    showShowResultWidget(DockSbMenu::Show);
*/
}

void MainWindow::successVerification(QStringList list, const QString& msg, SafetDocument doc){


    QString message;
    if ( msg.isEmpty()) {
        message = QString("%1").arg(tr("Verificación exitosa"));
    }
    else {
        message = QString("%1").arg(msg);
    }


}

QString MainWindow::renderMessageStack(QStack<QPair<QDateTime,QString> >& stack, SafetLog::Level l){
    QString result = "";

    if ( stack.isEmpty()) {

        return result;
    }


    while ( !stack.isEmpty() ) {
        QPair<QDateTime,QString> c = stack.pop();
        result += c.second + "\n";
    }
    if (result.length() > 0 )  {
        result.chop(1);
    }

    return result;
}

// ** Servicios de Escritorio (QDesktopServices)

void MainWindow::browse( const QUrl &url )
{
        QUrl u = url;
        u.setScheme( "file" );
        bool started = false;
#if defined(Q_OS_WIN32)
        started = QProcess::startDetached( "cmd", QStringList() << "/c" <<
                QString( "explorer /select,%1" ).arg( QDir::toNativeSeparators( u.toLocalFile() ) ) );
#elif defined(Q_OS_MAC)
        started = QProcess::startDetached("/usr/bin/osascript", QStringList() <<
                                                                          "-e" << "on run argv" <<
                                                                          "-e" << "set vfile to POSIX file (item 1 of argv)" <<
                                                                          "-e" << "tell application \"Finder\"" <<
                                                                          "-e" << "select vfile" <<
                                                                          "-e" << "activate" <<
                                                                          "-e" << "end tell" <<
                                                                          "-e" << "end run" <<
                                                                          // Commandline arguments
                                                                          u.toLocalFile());
#endif
        if( started )
                return;
        //QDesktopServices::openUrl( QUrl::fromLocalFile( QFileInfo( u.toLocalFile() ).absolutePath() ) );
}

void MainWindow::mailTo( const QUrl &url )
{
#ifdef SAFET_ESMTP // correo electronico
#if defined(Q_OS_WIN32)
        QString file = url.queryItemValue( "attachment" );
        QByteArray filePath = QDir::toNativeSeparators( file ).toLatin1();
        QByteArray fileName = QFileInfo( file ).fileName().toLatin1();
        QByteArray subject = url.queryItemValue( "subject" ).toLatin1();

        MapiFileDesc doc[1];
        doc[0].ulReserved = 0;
        doc[0].flFlags = 0;
        doc[0].nPosition = -1;
        doc[0].lpszPathName = filePath.data();
        doc[0].lpszFileName = fileName.data();
        doc[0].lpFileType = NULL;

        // Create message
        MapiMessage message;
        message.ulReserved = 0;
        message.lpszSubject = subject.data();
        message.lpszNoteText = "";
        message.lpszMessageType = NULL;
        message.lpszDateReceived = NULL;
        message.lpszConversationID = NULL;
        message.flFlags = 0;
        message.lpOriginator = NULL;
        message.nRecipCount = 0;
        message.lpRecips = NULL;
        message.nFileCount = 1;
        message.lpFiles = (lpMapiFileDesc)&doc;

        QLibrary lib("mapi32");
        typedef ULONG (PASCAL *SendMail)(ULONG,ULONG,MapiMessage*,FLAGS,ULONG);
        SendMail mapi = (SendMail)lib.resolve("MAPISendMail");
        if( mapi )
        {
                mapi( NULL, 0, &message, MAPI_LOGON_UI|MAPI_DIALOG, 0 );
                return;
        }
#elif defined(Q_OS_MAC)
        CFURLRef emailUrl = CFURLCreateWithString(kCFAllocatorDefault, CFSTR("mailto:info@example.com"), NULL), appUrl = NULL;
        bool started = false;
        if(LSGetApplicationForURL(emailUrl, kLSRolesEditor, NULL, &appUrl) == noErr)
        {
                CFStringRef appPath = CFURLCopyFileSystemPath(appUrl, kCFURLPOSIXPathStyle);
                if(appPath != NULL)
                {
                        if(CFStringCompare(appPath, CFSTR("/Applications/Mail.app"), 0) == kCFCompareEqualTo)
                        {
                                started = QProcess::startDetached("/usr/bin/osascript", QStringList() <<
                                        "-e" << "on run argv" <<
                                        "-e" << "set vattachment to (item 1 of argv)" <<
                                        "-e" << "set vsubject to (item 2 of argv)" <<
                                        "-e" << "tell application \"Mail\"" <<
                                        "-e" << "set composeMessage to make new outgoing message at beginning with properties {visible:true}" <<
                                        "-e" << "tell composeMessage" <<
                                        "-e" << "set subject to vsubject" <<
                                        "-e" << "tell content" <<
                                        "-e" << "make new attachment with properties {file name: vattachment} at after the last paragraph" <<
                                        "-e" << "end tell" <<
                                        "-e" << "end tell" <<
                                        "-e" << "activate" <<
                                        "-e" << "end tell" <<
                                        "-e" << "end run" <<
                                        // Commandline arguments
                                        url.queryItemValue("attachment") <<
                                        url.queryItemValue("subject"));
                        }
                        else if(CFStringFind(appPath, CFSTR("Entourage"), 0).location != kCFNotFound)
                        {
                                started = QProcess::startDetached("/usr/bin/osascript", QStringList() <<
                                        "-e" << "on run argv" <<
                                        "-e" << "set vattachment to (item 1 of argv)" <<
                                        "-e" << "set vsubject to (item 2 of argv)" <<
                                        "-e" << "tell application \"Microsoft Entourage\"" <<
                                        "-e" << "set vmessage to make new outgoing message with properties" <<
                                        "-e" << "{subject:vsubject, attachments:vattachment}" <<
                                        "-e" << "open vmessage" <<
                                        "-e" << "activate" <<
                                        "-e" << "end tell" <<
                                        "-e" << "end run" <<
                                        // Commandline arguments
                                        url.queryItemValue("attachment") <<
                                        url.queryItemValue("subject"));
                        }
                        else if(CFStringCompare(appPath, CFSTR("/Applications/Thunderbird.app"), 0) == kCFCompareEqualTo)
                        {
                                // TODO: Handle Thunderbird here? Impossible?
                        }
                        CFRelease(appPath);
                }
                CFRelease(appUrl);
        }
        CFRelease(emailUrl);
        if( started )
                return;
#elif defined(Q_OS_LINUX)
        QByteArray thunderbird;
        QProcess p;
        QStringList env = QProcess::systemEnvironment();
        if( env.indexOf( QRegExp("KDE_FULL_SESSION.*") ) != -1 )
        {
                p.start( "kreadconfig", QStringList()
                        << "--file" << "emaildefaults"
                        << "--group" << "PROFILE_Default"
                        << "--key" << "EmailClient" );
                p.waitForFinished();
                QByteArray data = p.readAllStandardOutput().trimmed();
                if( data.contains("thunderbird") )
                        thunderbird = data;
        }
        else if( env.indexOf( QRegExp("GNOME_DESKTOP_SESSION_ID.*") ) != -1 )
        {
                p.start( "gconftool-2", QStringList()
                        << "--get" << "/desktop/gnome/url-handlers/mailto/command" );
                p.waitForFinished();
                QByteArray data = p.readAllStandardOutput();
                if( data.contains("thunderbird") )
                        thunderbird = data.split(' ').value(0);
        }
        if( !thunderbird.isEmpty() )
        {
                bool started = p.startDetached( thunderbird, QStringList() << "-compose"
                        << QString( "subject='%1',attachment='%2,'" )
                                .arg( url.queryItemValue( "subject" ) )
                                .arg( QUrl::fromLocalFile( url.queryItemValue( "attachment" ) ).toString() ) );
                if( started )
                        return;
        }
#endif
        QDesktopServices::openUrl( url );
#endif // Correo Electronico
}

void MainWindow::manageDataSources(){

    //QString driver = "database.driver.";
    QString hostName = "database.host.";
    QString dataBaseName = "database.db.";
    QString userName = "database.user.";
    QString portName = "database.port.";

    int i = 0;
//    QString driverConf = SafetYAWL::getAuthConf()
//                         .getValue("Database", driver+QString::number(i+1)).toString();
    QString hostNameConf = SafetYAWL::getAuthConf()
                           .getValue("Database", hostName+QString::number(i+1)).toString();
    //qDebug("hostname: %s", qPrintable(hostNameConf));

    QString dataBaseNameConf = SafetYAWL::getAuthConf()
                       .getValue("Database", dataBaseName+QString::number(i+1)).toString();
    //qDebug("database: %s", qPrintable(dataBaseNameConf));
    QString userNameConf = SafetYAWL::getAuthConf()
                           .getValue("Database", userName+QString::number(i+1)).toString();
    //qDebug("user: %s", qPrintable(userNameConf));
    QString portConf = SafetYAWL::getAuthConf()
                       .getValue("Database", portName+QString::number(i+1)).toString();
    //qDebug("port: %s", qPrintable(portConf ));

    /*if ( result != QDialog::Accepted ){
            return;
    }*/
    
    return;
}

void MainWindow::compresstar(const QStringList& files, const QString& filename) {
#ifdef SAFET_TAR
    int argc = files.count()+5;
    char **argv = NULL;

    argv = new char*[argc];
    argv[0] = new char[20];
    strcpy(argv[0],"inflow");
    argv[1] = new char[20];
    strcpy(argv[1],"-C");

    argv[2] = new char[1024];
    strcpy(argv[2],"");

    argv[3] = new char[30];
    strcpy(argv[3],"-c");

    argv[4] = new char[150];
    strcpy(argv[4],qPrintable(filename));

    SYD << tr("...compresstar...(1)...files.count():%1").arg(files.count());
           SYD << tr("...compresstar...(1)...:|%1|").arg(filename);
    for (int i =0;i < files.count(); i++ ) {
        argv[5+i] = new char[1024];
        //strcpy(argv[5+i],qPrintable(files.at(i)));
        QString newfile = files.at(i);
        QString absfile = newfile.section("/",-1);
        strcpy(argv[5+i],qPrintable(absfile));
        strcpy(argv[2],qPrintable(newfile.section(absfile,0,0)));

        SYD << tr("...compresstar...(2)...:%1").arg(absfile);
        SYD << tr("...compresstar...(2)...:%1").arg(newfile.section(absfile,0,0));

    }
    dotar(argc,argv);
    for(int i=0; i < argc; i++) {
        delete argv[i];
    }
    delete[] argv;
#endif
}


QStringList MainWindow::uncompresstar(const QString& filename) {
    
QStringList files;
#ifdef SAFET_TAR
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
                << tr("Error descomprimiendo configuraciÃ³n."
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
#endif
    return files;


}



void MainWindow::setEnableCompletingButtons(bool b){

//    qDebug("MainWindow::setEnableCompletingButtons(bool b)");
//    // habilitar el boton de enviar
//    completingButtonForm->setEnabled(b);
//    completingButtonCons->setEnabled(b);
//    //completingButtonSign->setEnabled(false);
//    completingButtonConf->setEnabled(b);
//    completingButtonUsers->setEnabled(b);
}


void MainWindow::doLoadConfiguration(const QString &fileName) {

    qDebug("algo");
    SYD << tr(".........MainWindow::doLoadConfiguration.................(*1)....");
#ifdef SAFET_TAR

    SYD << tr(".........MainWindow::doLoadConfiguration.................(1)....");
    if (!QFile::exists(fileName)) {
        SYE << tr("El archivo \"%1\" no existe.").arg(fileName);
        return;
    }
    QStringList mylist = MainWindow::uncompresstar(fileName);
    qDebug("...(*)...mylist: %d", mylist.count());
    QStringList newlist;

    int maxlenstr = 0;
    QString principaldir;
    foreach(QString s, mylist) {
        if (maxlenstr == 0 ) {
            newlist.push_front(s);
            maxlenstr = s.length();
        }
        else if (s.length() < maxlenstr)  {
            maxlenstr = s.length();
            newlist.push_front(s);
        }
        else{
            newlist.push_back(s);
        }


    }
    Q_ASSERT(newlist.count() > 0 );

    int i = 0;
    principaldir = newlist.at(0);
    foreach(QString s, newlist) {
        qDebug("file: %s", qPrintable(s));
        if ( !processDirTar(s, i == 0) ) {
            return;
        }

        i++;
   }
    
#endif
}

bool MainWindow::processDirTar(const QString& f, bool isprincipal) {
#ifdef SAFET_TAR
    if( !f.endsWith(".tar")) {
     SYE
               << tr("El archivo de configuracion no tiene el formato "
                     "correcto (%1)")
               .arg(f);
               return false;
    }
    QString newdir = f;
    newdir = f.section("/",-1);
    QString subdir = "."+ newdir.left(newdir.length()-3).replace(".","/");
    newdir = QDir::homePath()+"/"+ subdir;

    bool r;
    QDir mydir(QDir::homePath());
    if (isprincipal && QFile::exists(newdir)) {

        doDelAllFilesOnDirectory(QDir::homePath()+"/"+subdir);


        if ( !QFile::exists(subdir+"/xmlrepository")) {
            r = mydir.mkdir(subdir+"/xmlrepository");
            r = mydir.mkdir(subdir+"/xmlrepository/container1");
        }

        if ( !QFile::exists(subdir+"/log")) {
            r = mydir.mkdir(subdir+"/log");
        }



    }
    else {
        if (!QFile::exists(newdir)) {
            r = mydir.mkdir(subdir);
        }
        else {
            if (!subdir.endsWith("log")) {
                doDelAllFilesOnDirectory(QDir::homePath()+"/"+subdir);
            }
        }

    }


    QStringList mylist = MainWindow::uncompresstar(f);
    QString searchhome = "/home/[a-zA-Z0-9\\.]+";

    QString replacehome = QDir::homePath();

    foreach(QString source, mylist) {

        QString target = newdir + source.section("/",-1);


        if (source.endsWith(".xml") || source.endsWith(".conf") ) {
               SafetYAWL::copyWithRe(source,target,searchhome,
                                     replacehome);
        }
        else if(source.endsWith(".gph") ) {

            target = newdir + QDir::homePath().section("/",-1)+".gph";
            r = QFile::copy(source,target);

        }
        else {
             r = QFile::copy(source,target);
        }

    }
#endif
    return true;
}




void MainWindow::doDelAllFilesOnDirectory(const QString &d) {
    QDir dir(d);
    if ( !dir.exists() ) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("No se puede eliminar archivos de un directorio vacío: \"%1\"")
                .arg(d);
        return;
    }

    dir.setFilter( QDir::Files );
    dir.setSorting(QDir::Size | QDir::Reversed);
    QStringList filters;
    filters << "*";
    dir.setNameFilters( filters );
    QStringList filelist = dir.entryList(QDir::Files);
    SafetYAWL::streamlog
            << SafetLog::Debug
            << tr("(Carga de configuración) "
                  ". Número de archivos a eliminar: \"%1\"").arg(filelist.count());
    foreach(QString f, filelist) {
        bool isdeleting = dir.remove(f);
        if ( !isdeleting) {
            SafetYAWL::streamlog
                    << SafetLog::Warning
                    <<
            tr("(Carga de la configuración) No se borró el archivo: \"%1\"")
                   .arg(f);
        }

    }


}

QString MainWindow::generateListForTar(const QString& folder, QStringList& myfiles,
                                       const QStringList& exts) {

        QString result;
#ifdef SAFET_TAR

    QString newdir = Safet::datadir;
    if (newdir.startsWith(".")) {
        newdir = newdir.mid(1);
    }
    if (!folder.isEmpty()) {
        result =  QDir::tempPath()+"/"+newdir +"."+folder+".tar";
    }
    else {
        result =  QDir::tempPath()+"/" + newdir +".tar";
    }
    QString confpath = _currconfpath + "/" + Safet::datadir + "/" + folder;

    SYD << tr("MainWindow::generateListForTar.....confpath:|%1|").arg(confpath);

    QDir dirconf(confpath);
    dirconf.setFilter(QDir::Files | QDir::NoSymLinks);
    if (!dirconf.exists()) {
        SYE
                << tr("Error al intentar abrir el directorio para empaquetado: \"%1\"")
                .arg(confpath);
        return QString("");

    }


    QFileInfoList mylist = dirconf.entryInfoList();

    for (int i=0; i<mylist.size(); i++)
    {
        QFileInfo fileInfo = mylist.at(i);
        foreach(QString e, exts) {
            if (fileInfo.filePath().endsWith("."+e)) {
               myfiles.append(fileInfo.filePath());
               //qDebug("...fileInfo.filePath(): %s", qPrintable(fileInfo.filePath()));
               break;
            }
        }
    }

    MainWindow::compresstar(myfiles,result);
    SYD << tr("...................MainWindow::generateListForTar.....result:|%1|")
           .arg(result);
#endif // SAFET_TAR
    return result;

}

void MainWindow::loadAuthorization() {

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

void MainWindow::loadInfo() {


    int countuser = 1;

    while ( true ) {
        QString account = SafetYAWL::getAuthConf()[QString("Auth/account.%1").arg(countuser)];
        if ( account.trimmed().length() == 0 ) {
            break;
        }
        QString pass = SafetYAWL::getAuthConf()[QString("Auth/pass.%1").arg(countuser)];
        QString role = SafetYAWL::getAuthConf()[QString("Auth/role.%1").arg(countuser)];
        QString realname = SafetYAWL::getAuthConf()[QString("Auth/realname.%1").arg(countuser)];
        QString tickets = SafetYAWL::getAuthConf()[QString("Auth/tickets.%1").arg(countuser)];
        if ( pass.trimmed().length() > 0 ) {
            QStringList mylist;
            mylist.append(pass);
            mylist.append(role);
            mylist.append(realname);
            mylist.append(tickets);
            users[ account ] = mylist;
        }
        countuser++;
    }

}


bool MainWindow::logout() {
   if ( MainWindow::currentaccount.isEmpty() ) {
	return false;
   }
   MainWindow::currentaccount = "";
   MainWindow::currentrole = "";
   MainWindow::currentrealname = "";
   return true;

}


QStringList MainWindow::getInfoOfUser(const QString& user) {

    QStringList result;
    SYD << tr("...........MainWindow::getInfoOfUser....user:|%1|").arg(user);
    if (user.compare("*") == 0) {

        SYD << tr("...........MainWindow::getInfoOfUser....user is keys:%1")
               .arg(users.keys().count());
        foreach(QString mykey, users.keys()) {
            QStringList mylist = users[mykey];
            QString newresult = "{ \n ";
            newresult += QString("\"account\" : \"%1\", \n").arg(mykey);
            for(int i = 0; i < mylist.count(); i++) {
                QString myname = "";
                QString myemail = "";
                QRegExp rx(Safet::EMAIL_PATTERN);
                int pos  = 0;

                switch (i) {
                case 0:
                    newresult += QString("\"pass\" : \"%1\",\n").arg(mylist.at(i));
                    break;
                case 1:
                    newresult += QString("\"groups\" : \"%1\",\n").arg(mylist.at(i));
                    break;
                case 2:
                    myname = mylist.at(i);
                    pos = myname.indexOf(rx);
                    if (pos > 0) {
                        myemail = rx.cap(0);
                        myname = myname.replace(myemail,"").trimmed();
                    }
                    newresult += QString("\"name\" : \"%1\",\n").arg(myname);
                    newresult += QString("\"email\" : \"%1\",\n").arg(myemail);

                    break;
                default:
                    break;
                }
            }
             newresult.chop(2);
             newresult += " \n}";

             result.append(newresult);
        }

        return result;

    }

    SYD  << tr("..........MainWindow::getInfoOfUser.......(1)...");
    if (!users.contains(user)) {
        return QStringList();
    }

        SYD  << tr("..........MainWindow::getInfoOfUser....user:|%1|").arg(user);
    foreach (QString s, users[ user ]) {
        SYD  << tr("..........MainWindow::getInfoOfUser....s:|%1|").arg(s);
    }

    return users[ user ];

}


bool MainWindow::login(const QString& name, const QString& pass) {

    QString curuser;
    bool isticket = false;
    SYD << tr("....MainWindow::login....testing ticketvalid...(1)...");

    if (pass.isEmpty()) {
        SYD << tr("....MainWindow::login....testing ticketvalid...USERSKEYSCOUNT(1)...users.keys().count():|%1|")
               .arg(users.keys().count());

        foreach(QString mykey, users.keys() ) {

            QStringList mylist = users[mykey];

            SYD << tr("\n....MainWindow::login...mykey:|%1|").arg(mykey);
            if (mylist.count() > 3 ) {
                QStringList curtickets = mylist.at(3).split(";",QString::SkipEmptyParts);
                SYD << tr("....MainWindow::login...PROBE_WITHOUT__CURTICKETS...curtickets...mylist:|%1|")
                       .arg(mylist.at(3));
                SYD << tr("....MainWindow::login...CURTICKETS...curtickets.count()..:|%1|")
                       .arg(curtickets.count());
                SYD << tr("....MainWindow::login...CURTICKETS...curtickets.count()..name:|%1|")
                       .arg(name);


                if (curtickets.contains(name)) {
                    curuser = mykey;
                    isticket = true;
                    SYA << tr("TICKET_LOGIN user: |%1| with TICKET: |%2|")
                           .arg(curuser)
                           .arg(name);
                    break;
                }
            }

        }

        SYD << tr("....MainWindow::login....testing ticketvalid...(1)...CURUSER:|%1|")
               .arg(curuser);
        SYD << tr("....MainWindow::login....testing ticketvalid...(1)...TICKET NO CONTAINS");

    }
    else {
         //pass empty
            curuser = name;
    }


    if ( !users.contains(curuser) ) {

        SYE  <<
        tr("El usuario \"%1\" no existe en la base de datos de autorización")
                .arg(curuser);

        return false;

    }
    Q_ASSERT( users[curuser].count() > 0 );

    QByteArray textvalue = QByteArray(qPrintable(pass));
    QByteArray hashvalue = QCryptographicHash::hash( textvalue, QCryptographicHash::Sha1 );

    if ( users[curuser].at(0) != QString(hashvalue.toHex().data()) && !isticket) {
        SYE  <<
            trUtf8("La contraseña o usuario \"%1\" no son correctos para ingresar al sistema.");
        SYD << tr("....MainWindow::login....contraseña incorrecta");
        return false;
    }

    if (users[curuser].count() > 3 ) {
        QStringList curtickets = users[curuser].at(3).split(";",QString::SkipEmptyParts);
        if (curtickets.count() > 0 )  {
            _currentjson = QString("{ \"ticket\": \"%1\", \"result\": \"%2\" , \"currentuser\": \"%3\"} ")
                    .arg(curtickets.at(0))
                    .arg("true")
		    .arg(curuser);
        }
        else {
            _currentjson = QString("{ \"ticket\": \"%1\", \"result\": \"%2\" , \"currentuser\": \"%3\"} ")
                    .arg("n/t")
                    .arg("false")
		    .arg(curuser);

        }
    }
    else {
        _currentjson = QString("{ \"ticket\": \"%1\", \"result\": \"%2\" } ")
                .arg("n/a")
                .arg("false");

    }


    MainWindow::currentaccount = curuser;
    SafetYAWL::setCurrentAuthUser(MainWindow::currentaccount);
            SYA << tr("El usuario (sistema de autorizacion a cambiado a \"%1\"")
            .arg(SafetYAWL::currentAuthUser());

    if ( users[curuser].count() > 1 ) {
        MainWindow::currentrole = users[curuser].at(1);
        if ( users[curuser].count() > 2 ) {
            MainWindow::currentrealname = users[curuser].at(2);            
        }
    }
    else {
        MainWindow::currentrole = "";
    }

    return true;
}


void MainWindow::doSaveConfiguration(const QString& fileName) {

   SYD << tr(".........MainWindow::doSaveConfiguration.................(outside)....");
#ifdef SAFET_TAR

    SYD << tr(".........MainWindow::doSaveConfiguration.................(1)....");
    if(fileName.isEmpty()) {
        SYE << tr("El nombre de archivo \"%1\" colocado como parámetro no puede ser vacío")
               .arg(fileName);
        return;
    }
    QStringList myfiles;
    QStringList exts;

    exts.append("xml");

    QString nametar1 = generateListForTar("flowfiles",myfiles,exts);


    myfiles.clear();
    QString nametar2 = generateListForTar("input",myfiles,exts);

    qDebug();
    myfiles.clear();
    exts.clear();
    exts.append("dtd");
    QString nametar3 = generateListForTar("dtd",myfiles,exts);

    myfiles.clear();
    exts.clear();
    exts.append("gph");
    QString nametar4 = generateListForTar("graphs",myfiles,exts);



    myfiles.clear();
    exts.clear();
    exts.append("html");
    exts.append("png");
    exts.append("gif");
    exts.append("js");
    exts.append("css");
    QString nametar5 = generateListForTar("reports",myfiles,exts);

    myfiles.clear();
    exts.clear();
    exts.append("png");
    QString nametar6 = generateListForTar("images",myfiles,exts);


    myfiles.clear();
    exts.clear();
    exts.append("pem");
    exts.append("crl");
    QString nametar7 = generateListForTar("certs",myfiles,exts);



    myfiles.clear();
    exts.clear();
    exts.append("conf");
    exts.append("bin");
    exts.append("db");
    QString nametar8 = generateListForTar("",myfiles,exts);


    QStringList tarfiles;
    tarfiles.append(nametar1);
    tarfiles.append(nametar2);
    tarfiles.append(nametar3);
    tarfiles.append(nametar4);
    tarfiles.append(nametar5);
    tarfiles.append(nametar6);
    tarfiles.append(nametar7);
    tarfiles.append(nametar8);

    SYD << tr(".........MainWindow::doSaveConfiguration.................compresstar...(enter)....");
    MainWindow::compresstar(tarfiles,fileName);
    SYD << tr(".........MainWindow::doSaveConfiguration.................compresstar...(exit)....");

#endif // SAFET_TAR

}

QString MainWindow::currentError() {
      qDebug("...*****MainWindow::currentError...");
      qDebug("_currenterror: |%s|", qPrintable(_currenterror));
      QString result = _currenterror;
      _currenterror = "";
      return result;
}

QString MainWindow::currentJSON()   {
    QString result = _currentjson;

    SYD << tr("....MainWindow::currentJSON....JSON:\n %1\n")
           .arg(result);
    _currentjson = QLatin1String("");
    return result;

}

QString MainWindow::currentDATA()  const {
    return _currentdata;

}

QString MainWindow::currentADATA()  const {
    return QString("[ %1 ]").arg(currentADocuments);

}

QString MainWindow::currentTable() {
    QString result = _currenttable;

    SYD << tr("\n\n...MainWindow::currentTable....:\n%1\n")
           .arg(result);
    _currenttable = QLatin1String("");
    return result;

}



QString MainWindow:: hostURL() {
    if (_hosturl.endsWith("/")) {
        _hosturl.chop(1);
    }
    return _hosturl;
}

void MainWindow::setHostURL(const QString& u) {
    _hosturl = u;
    if (_hosturl.endsWith("/")) {
        _hosturl.chop(1);
    }

}


QString MainWindow::delNodeToXMLWorkflow(const QString& fname,
                          const QString& nodename) {
    QString result;
    QDomDocument doc;
    QFile file(fname);
    if (!QFile::exists(fname) ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no existe en el sistema de archivos"
                  ".No es posible agregar un estado al archivo")
               .arg(fname);

        return result;
    }

    bool isopen = file.open(QIODevice::ReadOnly);
    if (!isopen ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no se puede leer"
                  ".No es posible agregar un estado al archivo")
               .arg(fname);


        return result;
    }


    bool iscontent = doc.setContent(&file);

    if (!iscontent) {
        SYE << tr("El formato del archivo XML \"%1\" no es válido")
               .arg(fname);
        return result;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode root = docElem.firstChild();


    QDomNode n = root.firstChild();

    QDomElement mydelelement, oldbeforeelement;

    QList<QDomElement> beforeelements;
    QString afternode;

    while( !n.isNull() ) {

        if ( n.isElement() ) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            QDomNamedNodeMap attributeMap = e.attributes();
            QDomNode attribute = attributeMap.namedItem("id");
            if (!attribute.isNull()) {
                QString myid = attribute.nodeValue();
                qDebug("...***MainWindow::delNodeToXMLWorkflow..id:|%s|",
                       qPrintable(myid));

                QDomElement myport = e.firstChildElement("port");
                QDomElement myconn = myport.firstChildElement("connection");

                if (myid == nodename ) {
                    mydelelement  = e;
                    afternode = myconn.attribute("source").simplified();
                    n = n.nextSibling();
                    continue;
                }

                while( !myconn.isNull()) {
                    if (myconn.attribute("source").simplified() == nodename) {
                        qDebug("....delNodeToXmlWorkflow...beforenode..:|%s|",
                               qPrintable(myid));
                        beforeelements.append(e);
                    }
                    myconn = myconn.nextSiblingElement("connection");
                }
            }
            else {
                qDebug("...No atributo ID");

            }

        }

        n = n.nextSibling();
    }

    if (afternode.isEmpty()) {
        SYE << tr("Error en los enlaces del flujo de trabajo (enlace del siguiente nodo)");
        return result;
    }



    foreach(QDomElement beforeelement, beforeelements) {
        // cambiar enlace del anterior  (beforenode)
        oldbeforeelement = beforeelement;
        QDomElement myport = beforeelement.firstChildElement("port");
        QDomElement oldport = myport;
        QDomElement myconn, oldconn, searchconn;
        oldconn = myconn;
        int howconns = 0;

        myconn = myport.firstChildElement("connection");

        while(!myconn.isNull()) {
            qDebug("....before...myconn.attribute:|%s|", qPrintable(myconn.attribute("source")));
            if ( myconn.attribute("source").simplified() == nodename ) {
                searchconn = myconn;
                oldconn = myconn;
            }
            howconns++;
            myconn = myconn.nextSiblingElement("connection");
        }

        Q_ASSERT(howconns >= 1);
        Q_ASSERT(!searchconn.isNull());

        if ( howconns > 1 ) {

            myport.removeChild(searchconn);
            howconns--;
            if (howconns == 1 ) {
                myport.setAttribute("pattern","none");
            }
        }
        else {
            // Obtener el siguiente elemento
            QDomElement mydelport = mydelelement.firstChildElement("port");
            QDomElement mydelconn = mydelport.firstChildElement("connection");

            searchconn.setAttribute("options",mydelconn.attribute("options"));
            searchconn.setAttribute("query",mydelconn.attribute("query"));
            searchconn.setAttribute("source",mydelconn.attribute("source"));
            myport.replaceChild(searchconn,oldconn);

        }

        beforeelement.replaceChild(myport, oldport);
        root.replaceChild(beforeelement,oldbeforeelement);
    }



    // eliminar el nodo

    root.removeChild(mydelelement);

//        QDomElement oldautofilter = myautofilter;
//        myautofilter.setAttribute("source",nodename);
//        mydelelement.replaceChild(myautofilter,oldautofilter);
//        myautofilter = myautofilter.nextSiblingElement("autofilter");

//    root.replaceChild(beforeelement, olde);


    qDebug("...MainWindow::delNodeToXMLWorkflow.....(2)...:|%s|",
           qPrintable(fname));
    QFile myfile(fname);
    if ( !myfile.open(QIODevice::WriteOnly))  {
                SYE << tr("No es posible escribir el archivo \"%1\" "
                          "con el nodo \"%2\" agregado")
                .arg(fname)
                       .arg(nodename);
                qDebug("error open file writeonly");
        return result;
    }

    result = fname;

    QTextStream out(&myfile);

    out << doc.toString();

    myfile.close();

    return result;
}

QString MainWindow::changeConnXMLWorkflow(const QString& fname,
                                          const QString& currnode,
                                          const QString& nextnode,
                                          const QString& newnode,
                                          const QString& newoptions,
                                          const QString& newquery) {
    QString result;
    QDomDocument doc;
    QFile file(fname);
    if (!QFile::exists(fname) ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no existe en el sistema de archivos"
                  ".No es posible cambiar la conexión en el archivo")
               .arg(fname);
        return result;
    }

    bool isopen = file.open(QIODevice::ReadOnly);
    if (!isopen ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no se puede leer"
                  ".No es posible cambiar la conexión en el archivo")
               .arg(fname);

        return result;
    }


    bool iscontent = doc.setContent(&file);
    if (!iscontent) {
        SYE << tr("El formato del archivo XML \"%1\" no es válido")
               .arg(fname);
        return result;
    }

    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode root = docElem.firstChild();

    qDebug("...MainWindow::changeConnXMLWorkflow.....(1)...");


    QDomNode n = root.firstChild();


    QDomElement mynewconn, oldport, olde;

    QString afternode;
    while( !n.isNull() ) {

        if ( n.isElement() ) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            olde = e;
            QDomNamedNodeMap attributeMap = e.attributes();
            QDomNode attribute = attributeMap.namedItem("id");
            if (!attribute.isNull()) {
                QString myid = attribute.nodeValue();
                qDebug("...*MainWindow::changeNodeXMLWorkflow..id:|%s|",
                       qPrintable(myid));

                if (myid == currnode) {
                    QDomElement myport = e.firstChildElement("port");
                    oldport = myport;
                    QDomElement myconn = myport.firstChildElement("connection");
                    while(!myconn.isNull()) {
                            afternode = myconn.attribute("source");
                            if (afternode == nextnode) {
                                mynewconn = myconn;
                                mynewconn.setAttribute("source",newnode);
                                if (newoptions.isEmpty()) {
                                   mynewconn.setAttribute("options",newnode);
                                }
                                else {
                                   mynewconn.setAttribute("options",newoptions);
                                }

                                if (!newquery.isEmpty()) {
                                   mynewconn.setAttribute("query",newquery);
                                }
                                myport.replaceChild(mynewconn,myconn);
                                e.replaceChild(myport,oldport);
                                root.replaceChild(e,olde);
                                break;
                            }
                            myconn = myconn.nextSiblingElement("connection");
                    }
                }

            }

        }

        n = n.nextSibling();
    }

    qDebug("...MainWindow::changeConnXMLWorkflow.....(2)...:|%s|",
           qPrintable(fname));
    QFile myfile(fname);
    if ( !myfile.open(QIODevice::WriteOnly))  {
                SYE << tr("No es posible escribir el archivo \"%1\" con el nodo \"%2\" cambiado")
                .arg(fname)
                       .arg(currnode);
                qDebug("error open file writeonly");
        return result;
    }

    result = fname;

    QTextStream out(&myfile);

    out << doc.toString();

    myfile.close();


    return result;

}

QString MainWindow::changeRoleXMLWorkflow(const QString& fname,
                                          const QString& currnode,
                                          const QString& newrol, const QString& ATTRNAME) {
    QString result;
    QDomDocument doc;
    QFile file(fname);
    if (!QFile::exists(fname) ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no existe en el sistema de archivos"
                  ".No es posible cambiar la conexión en el archivo")
               .arg(fname);
        return result;
    }

    bool isopen = file.open(QIODevice::ReadOnly);
    if (!isopen ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no se puede leer"
                  ".No es posible cambiar la conexión en el archivo")
               .arg(fname);

        return result;
    }


    bool iscontent = doc.setContent(&file);
    if (!iscontent) {
        SYE << tr("El formato del archivo XML \"%1\" no es válido")
               .arg(fname);
        return result;
    }

    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode root = docElem.firstChild();

    qDebug("...MainWindow::changeConnXMLWorkflow.....(1)...");


    QDomNode n = root.firstChild();


    QDomElement olde;

    while( !n.isNull() ) {

        if ( n.isElement() ) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            olde = e;
            QDomNamedNodeMap attributeMap = e.attributes();
            QDomNode attribute = attributeMap.namedItem("id");
            if (!attribute.isNull()) {
                QString myid = attribute.nodeValue();
                qDebug("...*MainWindow::changeNodeXMLWorkflow..id:|%s|",
                       qPrintable(myid));

                if (myid == currnode) {

                    root.replaceChild(e,olde);
                    e.setAttribute(ATTRNAME,newrol);
                }

            }

        }

        n = n.nextSibling();
    }

    qDebug("...MainWindow::changeConnXMLWorkflow.....(2)...:|%s|",
           qPrintable(fname));
    QFile myfile(fname);
    if ( !myfile.open(QIODevice::WriteOnly))  {
                SYE << tr("No es posible escribir el archivo \"%1\" con el nodo \"%2\" cambiado")
                .arg(fname)
                       .arg(currnode);
                qDebug("error open file writeonly");
        return result;
    }

    result = fname;

    QTextStream out(&myfile);

    out << doc.toString();

    myfile.close();


    return result;

}



QString MainWindow::addNodeToXMLWorkflow(const QString& fname,
                          const QString& beforenode,
                          const QString& nodename,
                          bool isparallel,
                          const QString& options,
                          const QString& query,
                          const QString& nodetitle,
                          const QString &documentsource
                          ) {

    qDebug("... MainWindow::addNodeToXMLWorkflow....");
    QString result;
    QDomDocument doc;
    QFile file(fname);
    if (!QFile::exists(fname) ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no existe en el sistema de archivos"
                  ".No es posible agregar un estado al archivo")
               .arg(fname);

        return result;
    }

    bool isopen = file.open(QIODevice::ReadOnly);
    if (!isopen ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no se puede leer"
                  ".No es posible agregar un estado al archivo")
               .arg(fname);


        return result;
    }


    bool iscontent = doc.setContent(&file);

    if (!iscontent) {
        SYE << tr("El formato del archivo XML \"%1\" no es válido")
               .arg(fname);
        return result;
    }

    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode root = docElem.firstChild();

    qDebug("...MainWindow::addNodeToXMLWorkflow.....(1)...");


    QDomNode n = root.firstChild();


    QDomElement mynewelement, beforeelement, afterelement;

    QString afternode;
    while( !n.isNull() ) {

        if ( n.isElement() ) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            QDomNamedNodeMap attributeMap = e.attributes();
            QDomNode attribute = attributeMap.namedItem("id");
            if (!attribute.isNull()) {
                QString myid = attribute.nodeValue();
                qDebug("...*MainWindow::addNodeToXMLWorkflow..id:|%s|",
                       qPrintable(myid));

                if (myid == beforenode) {
                    beforeelement = e;
                    QDomElement myport = beforeelement.firstChildElement("port");
                    QDomElement myconn = myport.firstChildElement("connection");
                    afternode = myconn.attribute("source");
                }
                if (myid == afternode) {
                    afterelement = e;
                    break;
                }


            }
            else {
                qDebug("...No atributo ID");

            }

        }

        n = n.nextSibling();
    }

    qDebug("...afternode:|%s|",qPrintable(afternode));
    if (afternode.isEmpty()) {
        SYE << tr("Error en los enlaces del flujo de trabajo");
        qDebug("afternode is empty");
        return result;
    }
    QDomElement olde = beforeelement;

    // Connection After Node
    QDomElement myafterport, oldp;
    QDomElement myafterconn, oldc;
    myafterport = afterelement.firstChildElement("port");
    myafterconn = myafterport.firstChildElement("connection");
    QString myaftersource = myafterconn.attribute("source");
    QString myafteroptions = myafterconn.attribute("options");
    QString myafterquery = myafterconn.attribute("query");
    qDebug("................myaftersource:|%s|", qPrintable(myaftersource));
    qDebug("...............myafteroptions:|%s|", qPrintable(myafteroptions));



    mynewelement = beforeelement.cloneNode().toElement();
    QString oldnodename = mynewelement.attribute("id");
    mynewelement.setAttribute("id",nodename);
    mynewelement.setAttribute("title",nodetitle);
    if ( isparallel) {
        myafterport = mynewelement.firstChildElement("port");
        oldp = myafterport;
        myafterconn = myafterport.firstChildElement("connection");
        oldc = myafterconn;
        myafterconn.setAttribute("source",myaftersource);
        myafterconn.setAttribute("query",myafterquery);
        myafterconn.setAttribute("options",myafteroptions);
        myafterport.replaceChild(myafterconn,oldc);
        mynewelement.replaceChild(myafterport,oldp);
    }



    // Variable
    QDomElement myvariable = mynewelement.firstChildElement("variable");
    QDomElement oldvariable = myvariable;

    myvariable.setAttribute("id",QString("v%1")
                            .arg(nodename));
    if (!documentsource.isEmpty()) {
        myvariable.setAttribute("documentsource",
                                documentsource);
    }
    QString myrolfield = myvariable.attribute("rolfield");
    QString timestampfield = myvariable.attribute("timestampfield");

    myrolfield.replace(QString("'%1'").arg(oldnodename),QString("'%1'").arg(nodename));
    timestampfield.replace(QString("'%1'").arg(oldnodename),QString("'%1'").arg(nodename));
    myvariable.setAttribute("rolfield",myrolfield);
    myvariable.setAttribute("timestampfield",timestampfield);

    // reemplazando variable
    mynewelement.replaceChild(myvariable,oldvariable);


    root.insertAfter(mynewelement,beforeelement);

    // Connection
    QDomElement myport = beforeelement.firstChildElement("port");
    QDomElement oldport = myport;
    QDomElement myconn, oldconn;
    myconn = myport.firstChildElement("connection");
    oldconn = myconn;
    if (isparallel ) {
        myconn = myconn.cloneNode().toElement();
    }

    myconn.setAttribute("source",nodename);
    if (!query.isEmpty()) {
        myconn.setAttribute("query",query);
    }
    if (!options.isEmpty()) {
        myconn.setAttribute("options",options);
    }
    else {
        myconn.setAttribute("options",nodename);
    }
    if (isparallel) {
        myport.appendChild(myconn);
        myport.setAttribute("pattern", "or");
        beforeelement.replaceChild(myport, oldport);
    }
    else {
        myport.replaceChild(myconn,oldconn);
    }
    beforeelement.replaceChild(myport, oldport);

    // Autofilter
    if (isparallel) {
        QDomElement myautofilter = mynewelement.firstChildElement("autofilter");
        while(!myautofilter.isNull()) {

            QDomElement oldautofilter = myautofilter;
            myautofilter.setAttribute("source",myaftersource);
            mynewelement.replaceChild(myautofilter,oldautofilter);
            myautofilter = myautofilter.nextSiblingElement("autofilter");
        }

    }
    else {
        QDomElement myautofilter = mynewelement.firstChildElement("autofilter");
        while(!myautofilter.isNull()) {

            QDomElement oldautofilter = myautofilter;
            myautofilter.setAttribute("source",nodename);
            mynewelement.replaceChild(myautofilter,oldautofilter);
            myautofilter = myautofilter.nextSiblingElement("autofilter");
        }
    }

    root.replaceChild(beforeelement, olde);


    qDebug("...MainWindow::addNodeToXMLWorkflow.....(2)...");

    //QString mynamefile = SafetYAWL::getTempNameFiles(1).at(0);

    qDebug("...MainWindow::addNodeToXMLWorkflow.....(2)...:|%s|",
           qPrintable(fname));
    QFile myfile(fname);
    if ( !myfile.open(QIODevice::WriteOnly))  {
                SYE << tr("No es posible escribir el archivo \"%1\" con el nodo \"%2\" agregado")
                .arg(fname)
                       .arg(nodename);
        return result;
    }

    result = fname;

    QTextStream out(&myfile);

    out << doc.toString();

    myfile.close();

    return result;
}


bool MainWindow::delXmlMenuGroupForm(const QString& newff) {
    qDebug("delXmlMenuGroupForm....(1)...");
//    QString myformatfile = newff;
//    myformatfile = myformatfile.replace(" ","_").toLower();

//    QString mypathfile = _currconfpath+ "/" + Safet::datadir + "/flowfiles/" + myformatfile +".xml";

    QString mypathfile = newff;

    QString myformatfile = newff.section("/",-1).replace(".xml","");

    qDebug("....MainWindow::delXmlMenuGroupForm...myformatfile:|%s|", qPrintable(myformatfile));
    bool result = QFile::remove(mypathfile);

    if (!result) {
        qDebug("no es posible eliminar:|%s|",qPrintable(mypathfile));
        SYE << tr("it's no possible remove story \"%1\"")
               .arg(newff);
        return false;
    }

    qDebug("....MainWindow::delXmlMenuGroupForm...removed:|%s|", qPrintable(mypathfile));

    QString fname = filecompleters.at(0);

    QDomDocument doc;
    QFile file(fname);
    if (!QFile::exists(fname) ) {
        SYE << tr("El archivo de de entrado \"%1\" no existe en el sistema de archivos."
                  "No es posible agregar un grupo al menu")
               .arg(fname);

        return false;
    }

    bool isopen = file.open(QIODevice::ReadOnly);
    if (!isopen ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no se puede leer"
                  ".No es posible agregar un grupo al menu")
               .arg(fname);

        qDebug("...no se lee");
        return false;
    }


    bool iscontent = doc.setContent(&file);

    if (!iscontent) {
        SYE << tr("El formato del archivo XML de menú de formularios \"%1\" no es válido")
               .arg(fname);
        qDebug("...no ta bien");
        return false;
    }

    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode root = docElem.firstChild();


    QDomNode n = root;


    QString mysearchitem = tr("New_step_%1").arg(myformatfile);

    qDebug("...MainWindow::delXmlMenuGroupForm.....(1)...mysearchitem:|%s|",qPrintable(mysearchitem));
    QDomElement beforeelement;

    while( !n.isNull() ) {

        if ( n.isElement() ) {
            beforeelement = n.toElement();
            QString myname = beforeelement.attribute("name");
            qDebug(".....delXmlMenuGroupForm....n.name:|%s|",qPrintable(myname));
            if (myname.compare(mysearchitem,Qt::CaseInsensitive) ==0 ) {
                  qDebug("...delXmlMenuGroupForm....break New step");
                break;
            }

        }

        n = n.nextSibling();
    }



    if (!beforeelement.isNull()) {

        docElem.removeChild(beforeelement);
        qDebug("...delXmlMenuGroupForm....se removio un elemento del archivo XML...");

    }

    QFile myfile(fname);
    if ( !myfile.open(QIODevice::WriteOnly))  {
        SYE << tr("No es posible escribir el archivo \"%1\" con el nodo \"%2\" eliminado")
               .arg(fname)
               .arg(mysearchitem);
        return false;
    }

    QTextStream out(&myfile);

    out << doc.toString();

    qDebug("...delXmlMenuGroupForm...escribiendo...doc.toString():\n|%s|",qPrintable(doc.toString()));

    myfile.close();
    return true;

}

bool MainWindow::addXmlMenuGroupForm(const QString& newff) {

    qDebug("...addXmlMenuGroupForm...fname:|%s|",qPrintable(newff));
    QString myformatfile = newff;
    myformatfile = myformatfile.replace(" ","_").toLower();

    QString mypathfile = _currconfpath+ "/" + Safet::datadir + "/flowfiles/" + myformatfile +".xml";

    if (filecompleters.count() == 0) {
        SYE <<  tr("error filecompleters.count() == 0");
        return false;
    }
    qDebug("...addXmlMenuGroupForm...filename form:|%s|",qPrintable(filecompleters.at(0)));

    QString fname = filecompleters.at(0);

    QString nodename;
    QDomDocument doc;
    QFile file(fname);
    if (!QFile::exists(fname) ) {
        SYE << tr("El archivo de de entrado \"%1\" no existe en el sistema de archivos"
                  ".No es posible agregar un grupo al menu")
               .arg(fname);

        return false;
    }

    bool isopen = file.open(QIODevice::ReadOnly);
    if (!isopen ) {
        SYE << tr("El archivo de flujo de trabajo \"%1\" no se puede leer"
                  ".No es posible agregar un grupo al menu")
               .arg(fname);

        qDebug("...no se lee");
        return false;
    }


    bool iscontent = doc.setContent(&file);

    if (!iscontent) {
        SYE << tr("El formato del archivo XML de menú de formularios \"%1\" no es válido")
               .arg(fname);
        qDebug("...no ta bien");
        return false;
    }

    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode root = docElem.firstChild();

    qDebug("...MainWindow::addXmlMenuGroupForm.....(1)...");

        qDebug("...MainWindow::addXmlMenuGroupForm.....(1)...root:|%s|",qPrintable(root.nodeName()));

    QDomNode n = root;

    qDebug("...MainWindow::addXmlMenuGroupForm.....(1)...n:|%s|",qPrintable(n.nodeName()));


    QDomElement mynewelement, beforeelement;

    while( !n.isNull() ) {

        if ( n.isElement() ) {
            beforeelement = n.toElement();
            QString myname = beforeelement.attribute("name");
            qDebug(".....addXmlMenuGroupForm....n.name:|%s|",qPrintable(myname));
            if (myname.startsWith(tr("Siguiente_estado"))) {
                qDebug("...break New step");
                break;
            }

        }

        n = n.nextSibling();
    }



    mynewelement = beforeelement.cloneNode().toElement();


    QString newnodename = "Siguiente_estado_"+myformatfile;
    mynewelement.setAttribute("name",newnodename);



    QDomElement mycommand = mynewelement.firstChild().toElement();
    QDomElement oldcommand = mycommand;
    qDebug("...MainWindow::addXmlMenuGroupForm.....(2)...myfield1:|%s|",
           qPrintable(mycommand.nodeName()));
    QDomElement myfieldlist = mycommand.firstChild().toElement();
    QDomElement oldfieldlist = myfieldlist;
    qDebug("...MainWindow::addXmlMenuGroupForm.....(2)...myfield2:|%s|",
           qPrintable(myfieldlist.nodeName()));

    QDomNode mycurrfield = myfieldlist.firstChild();

    while (!mycurrfield.isNull()) {


        QDomElement myfield  = mycurrfield.toElement();
        QDomElement oldmyfield = myfield;
        QString mytitle = myfield.attribute("title");

        if ( mytitle== "id") {
            myfield.setAttribute("options",QString("id:actividad_por_accion:where tipo ='%1'::'(' || id || ') ' || nombre").arg(myformatfile));
            myfieldlist.replaceChild(myfield,oldmyfield);
        }
        else if ( mytitle.compare("Siguiente_Estado", Qt::CaseInsensitive) == 0 ) {
            myfield.setAttribute("path",QString("%1").arg(mypathfile));
            myfieldlist.replaceChild(myfield,oldmyfield);
        }
        mycurrfield = myfield.nextSibling();
    }

    mycommand.replaceChild(myfieldlist,oldfieldlist);
    mynewelement.replaceChild(mycommand,oldcommand);

    if ( docElem.insertAfter(mynewelement,beforeelement).isNull() ) {

        SYE << tr("No pudo insertarse el elemento en el menu");
        qDebug("falla insercion menu");
        return false;
    }

    qDebug("...MainWindow::addXmlMenuGroupForm.....(2)...myformatfile:|%s|",
           qPrintable(myformatfile));
    QFile myfile(fname);
    if ( !myfile.open(QIODevice::WriteOnly))  {
        SYE << tr("No es posible escribir el archivo \"%1\" con el nodo \"%2\" agregado")
               .arg(fname)
               .arg(nodename);
        return false;
    }


    QTextStream out(&myfile);

    out << doc.toString();

    qDebug("...addXmlMenuGroupForm...escribiendo...doc.toString():\n|%s|",qPrintable(doc.toString()));

    myfile.close();
    QFile myflowfile(mypathfile);
    if ( !myflowfile.open(QIODevice::WriteOnly))  {
        SYE << tr("No es posible escribir el archivo \"%1\"")
               .arg(mypathfile);
        return false;
    }
    QTextStream outflow(&myflowfile);
    outflow << Safet::TEMPLATEFLOWFILE
               .arg(myformatfile)
               .arg(tr("Proyecto ")+myformatfile);

    myflowfile.close();
    return true;
}






#ifdef SAFET_ESMTP // correo electronico
#include <auth-client.h>
#include <libesmtp.h>
#endif // ESTMP

#include <fcntl.h>
#include <signal.h>
#include <openssl/ssl.h>
#include <ctype.h>

#ifdef SAFET_ESMTP // correo electronico
static void
build_message(FILE *fp);

int
handle_invalid_peer_certificate(long vfy_result)
{
  const char *k ="rare error";
  switch(vfy_result) {
  case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
    k="X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT"; break;
  case X509_V_ERR_UNABLE_TO_GET_CRL:
    k="X509_V_ERR_UNABLE_TO_GET_CRL"; break;
  case X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE:
    k="X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE"; break;
  case X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE:
    k="X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE"; break;
  case X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY:
    k="X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY"; break;
  case X509_V_ERR_CERT_SIGNATURE_FAILURE:
    k="X509_V_ERR_CERT_SIGNATURE_FAILURE"; break;
  case X509_V_ERR_CRL_SIGNATURE_FAILURE:
    k="X509_V_ERR_CRL_SIGNATURE_FAILURE"; break;
  case X509_V_ERR_CERT_NOT_YET_VALID:
    k="X509_V_ERR_CERT_NOT_YET_VALID"; break;
  case X509_V_ERR_CERT_HAS_EXPIRED:
    k="X509_V_ERR_CERT_HAS_EXPIRED"; break;
  case X509_V_ERR_CRL_NOT_YET_VALID:
    k="X509_V_ERR_CRL_NOT_YET_VALID"; break;
  case X509_V_ERR_CRL_HAS_EXPIRED:
    k="X509_V_ERR_CRL_HAS_EXPIRED"; break;
  case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
    k="X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD"; break;
  case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
    k="X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD"; break;
  case X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD:
    k="X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD"; break;
  case X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD:
    k="X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD"; break;
  case X509_V_ERR_OUT_OF_MEM:
    k="X509_V_ERR_OUT_OF_MEM"; break;
  case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
    k="X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT"; break;
  case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
    k="X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN"; break;
  case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY:
    k="X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY"; break;
  case X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE:
    k="X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE"; break;
  case X509_V_ERR_CERT_CHAIN_TOO_LONG:
    k="X509_V_ERR_CERT_CHAIN_TOO_LONG"; break;
  case X509_V_ERR_CERT_REVOKED:
    k="X509_V_ERR_CERT_REVOKED"; break;
  case X509_V_ERR_INVALID_CA:
    k="X509_V_ERR_INVALID_CA"; break;
  case X509_V_ERR_PATH_LENGTH_EXCEEDED:
    k="X509_V_ERR_PATH_LENGTH_EXCEEDED"; break;
  case X509_V_ERR_INVALID_PURPOSE:
    k="X509_V_ERR_INVALID_PURPOSE"; break;
  case X509_V_ERR_CERT_UNTRUSTED:
    k="X509_V_ERR_CERT_UNTRUSTED"; break;
  case X509_V_ERR_CERT_REJECTED:
    k="X509_V_ERR_CERT_REJECTED"; break;
  }
  printf("SMTP_EV_INVALID_PEER_CERTIFICATE: %ld: %s\n", vfy_result, k);
  return 1; /* Accept the problem */
}


void event_cb (smtp_session_t session, int event_no, void *arg,...)
{
  va_list alist;
  int *ok;

  va_start(alist, arg);
  switch(event_no) {
  case SMTP_EV_CONNECT:
  case SMTP_EV_MAILSTATUS:
  case SMTP_EV_RCPTSTATUS:
  case SMTP_EV_MESSAGEDATA:
  case SMTP_EV_MESSAGESENT:
  case SMTP_EV_DISCONNECT: break;
  case SMTP_EV_WEAK_CIPHER: {
    int bits;
    bits = va_arg(alist, long); ok = va_arg(alist, int*);
    printf("SMTP_EV_WEAK_CIPHER, bits=%d - accepted.\n", bits);
    *ok = 1; break;
  }
  case SMTP_EV_STARTTLS_OK:
    puts("SMTP_EV_STARTTLS_OK - TLS started here."); break;
  case SMTP_EV_INVALID_PEER_CERTIFICATE: {
    long vfy_result;
    vfy_result = va_arg(alist, long); ok = va_arg(alist, int*);
    *ok = handle_invalid_peer_certificate(vfy_result);
    break;
  }
  case SMTP_EV_NO_PEER_CERTIFICATE: {
    ok = va_arg(alist, int*);
    puts("SMTP_EV_NO_PEER_CERTIFICATE - accepted.");
    *ok = 1; break;
  }
  case SMTP_EV_WRONG_PEER_CERTIFICATE: {
    ok = va_arg(alist, int*);
    puts("SMTP_EV_WRONG_PEER_CERTIFICATE - accepted.");
    *ok = 1; break;
  }
  case SMTP_EV_NO_CLIENT_CERTIFICATE: {
    ok = va_arg(alist, int*);
    puts("SMTP_EV_NO_CLIENT_CERTIFICATE - accepted.");
    *ok = 1; break;
  }
  default:
    printf("Got event: %d - ignored.\n", event_no);
  }
  va_end(alist);
}



/* Callback to prnt the recipient status */
void
print_recipient_status (smtp_recipient_t recipient,
                        const char *mailbox, void *arg)
{
  const smtp_status_t *status;

  status = smtp_recipient_status (recipient);
  printf ("%s: %d %s", mailbox, status->code, status->text);
}

static int
_mutt_libesmtp_auth_interact (auth_client_request_t request,
                              char **result, int fields, void *arg)
{
  int i;

  for (i = 0; i < fields; i++) {
    if (request[i].flags & AUTH_USER) {
        result[i] = "vbravo";
    }
    else if (request[i].flags & AUTH_PASS) {
        result[i] = "4cajicA!";
    }
  }

  return 1;
}
#endif // ESMTP

void SAFETsendMail(const char *messageContents)
{
#ifdef SAFET_ESMTP // correo electronico
        smtp_session_t session;
        smtp_message_t message;
        smtp_recipient_t recipient;
        const smtp_status_t *status;
        struct sigaction sa;
        char *host = "mail.cenditel.gob.ve:25";
        char *from = "vbravo@cenditel.gob.ve";
        char *subject = "Prueba con esmtp";
        const char *recipient_address = "victorrbravo@gmail.com";
         auth_context_t authctx = NULL;
        char tempFileName[1000];
        int tempFd;
        FILE *fp;
        enum notify_flags notify = (notify_flags) (Notify_SUCCESS | Notify_FAILURE);

        auth_client_init();
        session = smtp_create_session();
        message = smtp_add_message(session);

        /* Ignore sigpipe */
        sa.sa_handler = SIG_IGN;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGPIPE, &sa, NULL);

        /* Set the Subject: header.  For no reason, we want the supplied subject
         to override any subject line in the message headers. */
//        if (subject != NULL) {
//            smtp_set_header(message, "Subject", "prueba");
//                smtp_set_header_option(message, "Subject", Hdr_OVERRIDE, 1);
//        }

        smtp_starttls_enable(session,Starttls_ENABLED);
        smtp_set_server(session, host);
        authctx = auth_create_context ();
        auth_set_mechanism_flags (authctx, AUTH_PLUGIN_PLAIN, 0);
        auth_set_interact_cb (authctx, _mutt_libesmtp_auth_interact, NULL);
        smtp_auth_set_context (session, authctx);





        smtp_set_eventcb(session, event_cb, NULL);

        /* Set the reverse path for the mail envelope.  (NULL is ok)
         */

        smtp_set_header(message, "From", "vbravo",from);
        smtp_set_header(message, "To", "Victor Bravo","victorrbravo@gmail.com");
//        smtp_set_header(message, "Subject", "Prueba de mensaje");
//        smtp_set_header(message, "Message-Id", NULL);
        smtp_set_reverse_path(message, from);


        /* Prepare message */
        memset(tempFileName, 0, sizeof(tempFileName));
        sprintf(tempFileName, "/tmp/messageXXXXXX");
        tempFd = mkstemp(tempFileName);
        fp = fdopen(tempFd, "w");
        build_message(fp);
//        fprintf(fp, "%s", messageContents);
        fclose(fp);
        fp = fopen(tempFileName, "r");
        smtp_set_message_fp(message, fp);





        recipient = smtp_add_recipient(message, recipient_address);

        smtp_dsn_set_notify (recipient, notify);

        /* Initiate a connection to the SMTP server and transfer the
         message. */
        if (!smtp_start_session(session)) {
                char buf[128];

                fprintf(stderr, "SMTP server problem %s\n", smtp_strerror(smtp_errno(),
                                buf, sizeof buf));
        } else {
                /* Report on the success or otherwise of the mail transfer.
                 */

            status = smtp_message_transfer_status(message);
            printf("%d %s", status->code, (status->text != NULL) ? status->text
                                                                 : "\n");
            smtp_enumerate_recipients(message, print_recipient_status, NULL);
        }

        /* Free resources consumed by the program.
         */
        smtp_destroy_session(session);
        fclose(fp);
        auth_client_exit();
#endif
}


#ifdef SAFET_ESMTP // correo electronico
static void
build_message(FILE *fp)
{

  fprintf(fp, "Return-Path: <%s>\r\n", "vbravo@cenditel.gob.ve");
  fprintf(fp, "Subject: %s\r\n", "Correo de activacion Gestion");
//  fprintf(fp, "From: %s %s\r\n", "vbravo","vbravo@cenditel.gob.ve");
//  fprintf(fp, "To: %s %s\r\n", "Victor Bravo","victorrbravo@gmail.com");
  fprintf(fp, "MIME-Version: 1.0\r\n");
  fprintf(fp, "Content-Type: text/plain;\r\n");
  fprintf(fp, "  charset=iso-8859-1\r\n");
  fprintf(fp, "Content-Transfer-Encoding: 7bit\r\n");
//  fprintf(fp, "X-send-pr-version: Evolution \r\n");
//  fprintf(fp, "X-GNATS-Notify: \r\n");
//  fprintf(fp, "\r\n\r\n");
//  fprintf(fp, ">Submitter-Id:	%s \r\n", "1");
//  fprintf(fp, ">Originator:	%s \r\n", "vbravo@cenditel.gob.ve");
//  fprintf(fp, ">Organization:	%s \r\n", "CENDITEL");
//  fprintf(fp, ">Confidential:	no \r\n");
//  fprintf(fp, ">Synopsis:	%s \r\n", "Correo de activacion");
//  fprintf(fp, ">Severity:	%s \r\n", "prueba6");
//  fprintf(fp, ">Priority:	%s \r\n", "(Highest)");
//  fprintf(fp, ">Category:	%s \r\n", "prueba8");
//  fprintf(fp, ">Class:		%s \r\n", "prueba9");
//  fprintf(fp, ">Release:	%s \r\n", "prueba10");

  fprintf(fp, ">Environment:\r\n");
  fprintf(fp, "\r\n\r\n");
  fprintf(fp, "%s\r\n", "prueba11");
  fprintf(fp, "\r\n\r\n");

  fprintf(fp, ">Description:\r\n");
  fprintf(fp, "\r\n\r\n");
  fprintf(fp, "%s\r\n", "Mensaje de activacion");
  fprintf(fp, "\r\n\r\n");

  fprintf(fp, ">How-To-Repeat:\r\n");
  fprintf(fp, "\r\n\r\n");
  fprintf(fp, "%s\r\n", "prueba13");
  fprintf(fp, "\r\n\r\n");

  fprintf(fp, ">Fix:\r\n");
  fprintf(fp, "\r\n\r\n");
  fprintf(fp, "%s\r\n", "prueba14");
  fprintf(fp, "\r\n\r\n");

}
#endif // ESMTP
