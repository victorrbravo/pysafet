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
#include "threadconsole.h"
#include "mainwindow.h"

OutputInterface* ThreadConsole::curOutputInterface = NULL; // Interface de Salida seleccionada

ThreadConsole::ThreadConsole(QObject* parent)
: QThread(parent)
{
/*
    mw = qobject_cast<MainWindow*>( parent );
     Q_CHECK_PTR( mw );
*/

//    mw = MainWindow::mymainwindow;

//    this->moveToThread(this);
}


void ThreadConsole::run() {

    qDebug("*...ThreadConsole...runreando...");
    //executedParsed();
    //QTimer::singleShot( 0, this, SLOT(executedParsedSlot()) );
    //qDebug("...ThreadConsole...**************************************1");
    executedParsedSlot();
    //this->exec();
    //qDebug("...ThreadConsole...**************************************2");
}

/*
void ThreadConsole::executedParsed() {

     while(true) {
          sleep(5);
          if ( mw == NULL ) return;
          if ( mw->mutexConsole() == NULL ) return;

          qDebug("...parado...mw->mutexConsole()->lock()...ok!");

          mw->mutexConsole()->lock();

          if  (!_commands.contains('f') ) {
               if ( !_commands.contains('h') && !_commands.contains('V') && !_commands.contains('T') ) {
                    mw->streamText << tr("*** No se especifico la ruta del archivo de flujo de trabajo (.xml) *** \n");
                    mw->streamText  <<  tr("Opcion: -f <archivo> o --file <archivo> \n");
                    mw->sendStreamToOutput();
                    mw->mutexConsole()->unlock();
                    return;
               }
          } else {
               if ( mw->configurator != NULL ) {
                    delete mw->configurator;
                    mw->configurator = new SafetYAWL();
               }

               mw->configurator->setAutofilters( _commands['a']);
               mw->configurator->openXML(_commands['f']);
               mw->configurator->openDataSources();
               mw->configurator->convertXMLtoObjects();
          }

          if ( _commands.contains('c') ) {
               mw->streamText << tr("Chequeado...!") << endl;
          }

          QMap<int, QString>::const_iterator i;
          for (i = _commands.constBegin(); i != _commands.constEnd(); ++i) {
               if ( i.key() != 'f' ) {
                    mw->processCommand(i.key() );
               }
          }
          qDebug("... mw->sendStreamToOutput()...");
          mw->sendStreamToOutput();
          qDebug("... mw->sendStreamToOutput()...(2)");
          mw->mutexConsole()->unlock();
          qDebug("... mw->sendStreamToOutput()...(3)");
     }

}
*/


void ThreadConsole::executedParsedSlot() {

    if ( mw == NULL ) {
        return;
    }
//    if ( mw->mutexConsole() == NULL ) return;

    _commands = mw->commands;

     //while(true) {
          //sleep(5);


          qDebug("...parado...mw->mutexConsole()->lock()...ok!");

          //mw->mutexConsole()->lock();
          qDebug("paso el lock");
          if  (!_commands.contains('f') ) {
               if ( !_commands.contains('h') && !_commands.contains('V') && !_commands.contains('T') ) {
                   mw->streamText << tr("*** No se especifico la ruta del archivo de flujo de trabajo (.xml) *** \n");
                   mw->streamText  <<  tr("Opcion: -f <archivo> o --file <archivo> \n");
                   mw->sendStreamToOutput();
                   //mw->mutexConsole()->unlock();
                   //qDebug("Antes de return");
                   return;
              }
          } else {
               if ( mw->configurator != NULL ) {
                    delete mw->configurator;
                    mw->configurator = new SafetYAWL();
                    //qDebug("mw->configurator != NULL");
               }

               mw->configurator->setAutofilters( _commands['a']);
               mw->configurator->setRecursivefilters( _commands['r']);
               mw->configurator->openXML(_commands['f']);
               mw->configurator->openDataSources();
               mw->configurator->convertXMLtoObjects();
               //qDebug("paso mw->configurator");
          }


         if ( _commands.contains('c') ) {
              mw->streamText << tr("Chequeado...!") << endl;
              //qDebug("mw->streamText");
         }


       QMap<int, QString>::const_iterator i;
       for (i = _commands.constBegin(); i != _commands.constEnd(); ++i) {
            if ( i.key() != 'f' ) {
                    qDebug("procesando... executeSlot");
                    mw->processCommand(i.key() );
                    //qDebug("dentro de if");
            }
            //qDebug("dentro del for");
       }


       //mw->sendStreamToOutput();

        //qDebug("... mw->sendStreamToOutput()...");
        mw->sendStreamToOutput();
        //qDebug("... mw->sendStreamToOutput()...(2)");
        //mw->mutexConsole()->unlock();
        //qDebug("... mw->sendStreamToOutput()...(3)");
        //this->exit();
        if ( ThreadConsole::curOutputInterface != NULL ) {
             delete ThreadConsole::curOutputInterface;
             ThreadConsole::curOutputInterface = NULL;
        }

    //}
        //this->exit();


}
