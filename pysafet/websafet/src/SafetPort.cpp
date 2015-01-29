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

#include "SafetYAWL.h"
#include "SafetPort.h"
#include "SafetConnection.h"

SafetPort::SafetPort() {
	_pattern = "none";
}



void SafetPort::addChild(SafetXmlObject* o) {
	Q_CHECK_PTR(o);
        SafetConnection *myconn;
	SafetXmlObject::addChild(o);
	switch(SafetYAWL::getClassReference(SafetYAWL::translateNS(o->metaObject()->className()))) {
		case 6:
                        myconn = qobject_cast<SafetConnection*>(o);
                        Q_CHECK_PTR( myconn);
                        myconn->setOptions(SafetYAWL::replaceArgsflow(myconn->options()));
                        if (myconn->back() == "yes") {
                            backlist.push_back(myconn);
                        }
                        else {
                            connectionlist.push_back(myconn);
                        }
			break;
		default:;
	}
}
void SafetPort::addConnection(SafetConnection* c) {
    Q_CHECK_PTR( c  );
    c->setParent( qobject_cast<SafetXmlObject*>(this));
    if (c->back() == "yes") {
        backlist.push_back( c );
    }
    else {
        connectionlist.push_back( c );
    }

}
QString SafetPort::query(int npath) const {
     SafetNode* mynode = qobject_cast<SafetNode*> (parent());
     Q_CHECK_PTR( mynode );
     if ( npath >= connectionlist.count() ) {         

         int bpath = npath - connectionlist.count();
         if ( bpath >= backlist.count() ) {
            SYE << tr("Ocurrio un error al acceder a la conexion nro \"%1\" del nodo \"%2\"").arg(npath).arg(mynode->id());
          return QString("");
         }
         else {
             Q_ASSERT(bpath < backlist.count());
             SafetConnection *myconnection = backlist.at( bpath);
             Q_CHECK_PTR( myconnection );
             return myconnection->query();

         }
     }
     Q_ASSERT(npath < connectionlist.count());
     SafetConnection *myconnection = connectionlist.at( npath);
     Q_CHECK_PTR( myconnection );
     return myconnection->query();
}

QString SafetPort::options(int npath) const {
     SafetNode* mynode = qobject_cast<SafetNode*> (parent());
     Q_CHECK_PTR( mynode );
     if ( npath >= connectionlist.count() ) {
         int bpath = npath - connectionlist.count();
         if ( bpath >= backlist.count() ) {
              SYE << tr("Ocurrio un error al acceder a la conexion nro \"%1\" del nodo \"%2\"").arg(npath).arg(mynode->id());
              return QString("");
         }
         else {
             Q_ASSERT(bpath < backlist.count());
             SafetConnection *myconnection = backlist.at( bpath);
             Q_CHECK_PTR( myconnection );
             return myconnection->options();
         }

     }



     // Chequear para errores en el documento XML
     Q_ASSERT(npath < connectionlist.count());
     SafetConnection *myconnection = connectionlist.at( npath);
     Q_CHECK_PTR( myconnection );
     return myconnection->options();
}


QString SafetPort::source(int npath) const {
     SafetNode* mynode = qobject_cast<SafetNode*> (parent());
     Q_CHECK_PTR( mynode );
     if ( npath >= connectionlist.count() ) {
            int bpath = npath - connectionlist.count();

          if ( bpath >= backlist.count() ) {
          SYE <<
                tr("Ocurrio un error al acceder a la conexion nro \"%1\" del nodo \"%2\"").arg(npath).arg(mynode->id());
          return QString("");
          }
          else {
              // Chequear para errores en el documento XML
              Q_ASSERT(bpath < backlist.count());
              SafetConnection *myconnection = backlist.at( bpath);
              Q_CHECK_PTR( myconnection );
              return myconnection->source();
          }


     }

     QString nameparent = mynode->id();
     // Chequear para errores en el documento XML
     Q_ASSERT(npath < connectionlist.count());
     SafetConnection *myconnection = connectionlist.at( npath);
     Q_CHECK_PTR( myconnection );
     return myconnection->source();

}


void  SafetPort::clearConn() {
    connectionlist.clear();
    backlist.clear();


}

QList<SafetConnection*>& SafetPort::getConnectionlist() {

    return connectionlist;

}
QList<SafetConnection *>& SafetPort::getBackList()  {
    return backlist;
}

QList<SafetConnection *> SafetPort::getConnBackList()  {
        QList<SafetConnection*> result;
        result = connectionlist;
        result += backlist;
        return result;

}

QString SafetPort::tokenlink(int npath) const {
     SafetNode* mynode = qobject_cast<SafetNode*> (parent());
     Q_CHECK_PTR( mynode );
     if ( npath >= connectionlist.count() ) {
        int bpath = npath - connectionlist.count();

        if ( bpath >= backlist.count() ) {
           SYE << tr("Ocurrio un error al acceder a la conexion nro \"%1\" del nodo \"%2\"").arg(npath).arg(mynode->id());
              return QString("");
        }
        else {
            Q_ASSERT(bpath < backlist.count());
            SafetConnection *myconnection = backlist.at( bpath);
            Q_CHECK_PTR( myconnection );
            return myconnection->tokenlink();

        }
     }


     Q_ASSERT(npath < connectionlist.count());
     SafetConnection *myconnection = connectionlist.at( npath);
     Q_CHECK_PTR( myconnection );
     return myconnection->tokenlink();
}
