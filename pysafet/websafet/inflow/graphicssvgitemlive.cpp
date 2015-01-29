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

#include "graphicssvgitemlive.h"



GraphicsSvgItemLive::GraphicsSvgItemLive ( QGraphicsItem* parent)
 {
     value = 0;
     qDebug("...start....");
     //thConsole = NULL;
     //timerId = startTimer(1000/ 10);
     privateItem = NULL;



}


GraphicsSvgItemLive::GraphicsSvgItemLive (  QString& svgtext, QGraphicsItem* parent)
 {
     value = 0;     
     privateItem = NULL;
     rootItem = new QGraphicsSvgItem(NULL);
     genSvgItem(svgtext);
}


GraphicsSvgItemLive::~GraphicsSvgItemLive ( ) {

//     if ( thConsole != NULL ) {
//          delete thConsole;
//     }


}




void GraphicsSvgItemLive::setMw(MainWindow* m) {

     //thConsole = new ThreadConsole( m );

}
void GraphicsSvgItemLive::setCommands(const QMap<int,QString>& cmds) {
     //Q_CHECK_PTR( thConsole );
//     qDebug("...GraphicsSvgItemLive...cmds...count: %d", cmds.count() );
  //   thConsole->setCommands( cmds );

}

QMap<int,QString>& GraphicsSvgItemLive::commands() {
     //return thConsole->commands();
}

//void GraphicsSvgItemLive::start(QThread::Priority p) {
//    // Q_CHECK_PTR( thConsole);
//     qDebug("GraphicsSvgItemLive::run()...");
//     //thConsole->start(p);
//}


GraphicsWorkflow *GraphicsSvgItemLive::graphWf() {
     GraphicsWorkflow *result = NULL;
//     SceneWorkflow* scenewf = qobject_cast<SceneWorkflow*>(scene());
//     QList<QGraphicsView*> mylist = scenewf->views();
//     if ( mylist.count() == 0 ) return NULL;
//     result = qobject_cast<GraphicsWorkflow*>(mylist.at(0));
     return result;
}

MainWindow* GraphicsSvgItemLive::mainWindow() {
     MainWindow *result = NULL;
     GraphicsWorkflow* g = graphWf();
     if ( g == NULL ) return NULL;
     result = qobject_cast<MainWindow*>(g->parent() );
     return result;
}

void GraphicsSvgItemLive::genSvgItem(QString& svgtext) {
    if ( privateItem != NULL ) {
           delete privateItem;
           privateItem = NULL;
    }
    QStringList fileslist = SafetYAWL::getTempNameFiles(1);
    _filename = fileslist.at(0);
    QFile file(_filename);
    if ( !file.open(QIODevice::WriteOnly))  {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("Fallo carga del item SVG, no es posible escribir el archivo \"%1\"")
                .arg(_filename);
        return;
    }
#if QT_VERSION > 0x040400


#endif

    QTextStream out(&file);
    out << svgtext;
    file.close();
//    SafetYAWL::streamlog <<
//            SafetLog::Debug <<
//            tr("Escribiendo correctamente Item SVG en el archivo temporal \"%1\"")
//            .arg(_filename);

}

QGraphicsSvgItem* GraphicsSvgItemLive::loadSvgItem() {
    if ( privateItem == NULL ) {
        //_filename = QString("/tmp/prueba.svg");
            privateItem =  new QGraphicsSvgItem(_filename);    
    }
    return privateItem;
}


