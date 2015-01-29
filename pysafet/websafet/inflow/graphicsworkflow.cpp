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

#include <limits>

#include "graphicsworkflow.h"
#include <QSvgRenderer>

GraphicsWorkflow::GraphicsWorkflow(QWidget * parent )
: QGraphicsView(parent)
{
     mw = qobject_cast<MainWindow*>( parent );
     _centralWidget = NULL;
     _currentgroup = NULL;
     initialize();

}

GraphicsSvgItemLive* GraphicsWorkflow::searchItemForCmds(const QMap<int,QString>& cmds) {

          for (int i = 0; items().count(); i++) {
          QGraphicsSvgItem* gitem = dynamic_cast<QGraphicsSvgItem*>(items().at(i));
//          GraphicsSvgItemLive* curitem = qobject_cast<GraphicsSvgItemLive*>(gitem);
//          if (curitem == NULL) continue;
//          Q_CHECK_PTR( curitem );
//          if  ( curitem->commands() == cmds ) {
//               return curitem;
//          }
     }

     return NULL;
}



QGraphicsSvgItem* GraphicsWorkflow::addAllSvgItems( const QString& name,  const QMap<int,QString>& cmds) {


     QGraphicsSvgItem *myitem = NULL;

     if ( myitem == NULL  ) {

         QByteArray myxml = SafetYAWL::readStringFromFile(name);
          QMap<QString,QPair<QPointF,QString> > ids = getXmlIds(myxml);

          QString curtitle = SafetYAWL::getConf()["GeneralOptions/currentflowtitle"];
         // qDebug(".......curtitle: |%s|", qPrintable(curtitle));
          QGraphicsTextItem* titleitem = NULL;
          QString includetitle = SafetYAWL::getConf()["GeneralOptions/currentflowtitle.include"];
          qDebug("includetitle: %s", qPrintable(includetitle));

          if ( includetitle.compare("on") == 0 ) {
            titleitem = NULL;
            addInfoGraphText(ids,curtitle,titleitem);
            if ( titleitem ) {
                _items.append(titleitem);
            }

          }
          QString includedate = SafetYAWL::getConf()["Graphs/infotext.include"];

          if ( includedate.compare("on") == 0 ) {
              SYD << tr("QGraphicsTextItem* myitem = addInfoGraphDateText(ids);");
              QGraphicsTextItem* myitem = addInfoGraphDateText(ids);
              if ( myitem) {
                  _items.append(myitem);
              }

          }


          QMap<QString,QString>  svgs = extractSvgElements(myxml);
            for(int i =0; i < svgs.keys().count(); i++) {
                QString key = svgs.keys().at(i);
                QString svg = svgs[ key];
                GraphicsSvgItemLive localitem(svg);
                myitem = localitem.loadSvgItem();
                if ( MainWindow::configurator->getWorkflow() != NULL ) {
                    QString info = MainWindow::configurator->getWorkflow()->infoModeMap()[ids[key].second];
                    if (key.startsWith("node")) {
                        myitem->setToolTip(tr("%1").arg(info));
                    }
                }
                myitem->setFlags(QGraphicsItem::ItemClipsToShape| QGraphicsItem::ItemIsMovable);
                myitem->setCacheMode(QGraphicsItem::NoCache);
                myitem->setZValue(0);                

                scene()->addItem(myitem);

                myitem->setPos(ids[key].first);
                _items.append(myitem);
            }

      }
     else {
          //QSvgRenderer *renderer = new QSvgRenderer(name);
     }
     qDebug("...addAllSvgItems..........(6)...");
     SafetYAWL::streamlog << SafetLog::Action
             << tr("Gráfico de \"Flujo de Trabajo\" generado correctamente...ok!");
     doGroup();
     SafetYAWL::streamlog << SafetLog::Action
             << tr("Agrupando Nodos...ok!");

     return myitem;
}

QGraphicsTextItem* GraphicsWorkflow::addInfoGraphDateText(const QMap<QString,QPair<QPointF,QString> >& ids) {

    QPointF infopos(0,0);
    qreal leftx = 0, rightx = 0;

    for (int i=0; i< ids.keys().count(); i++ ) {
        QString key = ids.keys().at(i);
        if ( ids[key].first.x() < infopos.x() ) {
            infopos.setX(ids[key].first.x());
            leftx =   ids[key].first.x();
        }
        if ( ids[key].first.x() > rightx ) {
            rightx =   ids[key].first.x();
        }

        if ( ids[key].first.y() < infopos.y() ) {
            infopos.setY(ids[key].first.y());
        }

    }

    qDebug("right: %0.2f",rightx);
    qDebug("leftx: %0.2f",leftx);

    QGraphicsTextItem *infotext = new QGraphicsTextItem();
    QString msg = SafetYAWL::getConf()["Graphs/infotext.format"];
    QDateTime now = QDateTime::currentDateTime();
    QString date = now.toString("dd MMM yyyy");
    QString time = now.toString("h:mm:ssap");
    QString datetime = now.toString("dd MMM yyyy hh:mm:ssap");
    msg.replace("%time",time);
    msg.replace("%datetime",datetime);
    msg.replace("%date",date);

    scene()->addItem(infotext);
    qDebug("...addInfoGraphText...SafetYAWL::lastinfodate:|%s|",
           qPrintable(SafetYAWL::lastinfodate));
    if ( !SafetYAWL::lastinfodate.isEmpty() ) {
        infotext->setHtml(SafetYAWL::lastinfodate);
    }
    else {
        infotext->setHtml(msg);
    }
    SafetYAWL::lastinfodate = msg;

    bool ok;
    qreal size = SafetYAWL::getConf()["Graphs/infotext.separation"].toDouble(&ok);
    qDebug("......separation:|%0.2f|",size);
    QString fontname = SafetYAWL::getConf()["Graphs/infotext.font"];
    int fontsize = SafetYAWL::getConf()["Graphs/infotext.size"].toInt(&ok);
    QFont myfont;
    ok = false;
    if ( !fontname.isEmpty()) {
        myfont.setFamily(fontname);
        ok = true;
    }

    qDebug(".....(1)..........");
    if ( fontsize > 1 ) {
        myfont.setPixelSize(fontsize);
        ok = true;
    }
    if ( ok ) {
        infotext->setFont(myfont);
    }
   infopos.setY(infopos.y()-int(size));
   infotext->setPos(infopos);
    return infotext;
}

QGraphicsTextItem* GraphicsWorkflow::addInfoGraphText(const QMap<QString,QPair<QPointF,QString> >& ids,
                                                      const QString& title,
                                                      QGraphicsTextItem*& titleitem) {
    QPointF infopos(0,0);
    QPointF titlepos(0,0);
    qreal leftx = 0, rightx = 0;

    for (int i=0; i< ids.keys().count(); i++ ) {
        QString key = ids.keys().at(i);
        if ( ids[key].first.x() < infopos.x() ) {
            infopos.setX(ids[key].first.x());
            leftx =   ids[key].first.x();
        }
        if ( ids[key].first.x() > rightx ) {
            rightx =   ids[key].first.x();
        }

        if ( ids[key].first.y() < infopos.y() ) {
            infopos.setY(ids[key].first.y());
        }

    }

    qDebug("right: %0.2f",rightx);
    qDebug("leftx: %0.2f",leftx);
    titlepos.setX((rightx-leftx)/2.0);
    QGraphicsTextItem *infotitle = NULL;

    infotitle = new QGraphicsTextItem();
    infotitle->setHtml(title);

    infopos.setY(infopos.y()-15);

    if ( infotitle) {
        scene()->addItem(infotitle);
    }
    bool ok;
    qreal size = SafetYAWL::getConf()["GeneralOptions/currentflowtitle.separation"].toDouble(&ok);
    QString fontname = SafetYAWL::getConf()["GeneralOptions/currentflowtitle.font"];
    int fontsize = SafetYAWL::getConf()["GeneralOptions/currentflowtitle.size"].toInt(&ok);

    QFont myfont;
    ok = false;
    if ( !fontname.isEmpty()) {
        myfont.setFamily(fontname);
        ok = true;
    }

    if ( fontsize > 1 ) {
        myfont.setPixelSize(fontsize);
        ok = true;
    }

    if ( infotitle) {
        int titlesize = fontsize+3;
        myfont.setPixelSize(titlesize);
        infotitle->setFont(myfont);
    }



    infopos.setY(infopos.y()-size);
    // Colocando posicion del titulo
    titlepos.setX(titlepos.x());
    titlepos.setY(infopos.y()-size*2.5);


    if (infotitle) {
            infotitle->setPos(titlepos);
            titleitem = infotitle;
    }

    //qDebug("...infotext %p", infotext);
    return infotitle;

}


QMap<QString,QString>  GraphicsWorkflow::extractSvgElements(const QString& xmltext) {
    QDomDocument doc;
    QMap<QString,QString>  result;

    bool isopen = doc.setContent(xmltext);
    if ( !isopen) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("Se presentó un error al tratar de extraer los elementos del archivo de flujo "
                      "de trabajo tipo SVG");
        return result;
    }

    QDomNodeList list = doc.elementsByTagName("g");

    for(int i = 0; i < list.count();i++) {
        QString nodeid, nodetext;
        QTextStream mystream(&nodetext);

        QDomNode node = list.at(i);
        QDomNamedNodeMap attrs = node.attributes();
        nodeid =  attrs.namedItem("id").nodeValue().trimmed();
        if ( nodeid != "graph0") {
            node.save(mystream,0);
            mystream.flush();
//            SafetYAWL::streamlog
//                    << SafetLog::Debug
//                    << tr("Grafico SVG nodetext numero \"%2\": \"%1\"")
//                    .arg(nodetext)
//                    .arg(i);
            QString newdoc = Safet::templateSVGString1.arg(nodetext);
            //result[nodeid] = localeNormalize(newdoc);
            result[nodeid] = newdoc;

        }
    }
    return result;
}


QString GraphicsWorkflow::localeNormalize(const QString& s) {
    QString result = s;
    QRegExp rxPrincipal("\\-?[0-9]+,([0-9,e\\-]+)");

    int pos = 0;    
    while ( true ) {
        int cur = s.indexOf(rxPrincipal,pos);
        if ( cur == -1 ) break;
        qDebug("..pos: %d",cur);
        QString oldright = rxPrincipal.cap(1);
        QString newright = rxPrincipal.cap(1);
        pos = cur+oldright.length()+1;
        newright.replace(",",".");
        result.replace(oldright,newright);
    }


    QRegExp rxRadio("\"\\-?[0-9]+,[0-9e\\-]+\"");
    pos = 0;
    while ( true ) {
        int cur = s.indexOf(rxRadio,pos);
        if ( cur == -1 ) break;
        qDebug("..pos: %d",pos);
        QString oldright = rxPrincipal.cap(1);
        QString newright = rxPrincipal.cap(1);
        pos = cur + oldright.length()+1;
        newright.replace(",",".");
        result.replace(oldright,newright);
    }

    return result;
}

void GraphicsWorkflow::doGroup() {


//    QList<QGraphicsItem *> currentitems;
//    foreach(QGraphicsItem* item, _items) {
//        currentitems.append(item);
//
//    }

    if ( _currentgroup != NULL ) {
        _listgroup.append(_currentgroup);
    }

    _currentgroup = scene()->createItemGroup(_items);
    _currentgroup->setFlag(QGraphicsItem::ItemIsMovable);
    _currentgroup->setFlag(QGraphicsItem::ItemIsSelectable);
    _currentgroup->setPos(MainWindow::gviewoutputwidth/2.0-+_currentgroup->boundingRect().width()/2.0,MainWindow::gviewoutputheight/2.0
                          +_currentgroup->boundingRect().height()/2.0);
    _items.clear();




}

void GraphicsWorkflow::unGroup() {

    if ( _currentgroup != NULL ) {
         scene()->destroyItemGroup(_currentgroup);
    }
}

GraphicsSvgItemLive* GraphicsWorkflow::redrawItem(const QString& name, GraphicsSvgItemLive* item) {

//     QStringList ids = getXmlIds(name);
//
//      QSvgRenderer *renderer = item->renderer();
//      renderer->load(name);
//      Q_CHECK_PTR( renderer );
//      item->setSharedRenderer(renderer);
     return item;
}

QPointF GraphicsWorkflow::topLeftFromPoints(const QString& p) {
    QRegExp rxPoint("([0-9\\-\\.e]+),([0-9\\-\\.e]+)");
    int current = 0;
    QPointF result;

    qreal top= std::numeric_limits<float>::max(),left = std::numeric_limits<float>::max();
    bool ok;
    while ( true ) {
        current = p.indexOf(rxPoint,current);
        if ( current == -1 ) break;
        current += rxPoint.cap(0).length();
        qreal x = rxPoint.cap(1).toFloat(&ok);
        qreal y = rxPoint.cap(2).toFloat(&ok);
        if ( x < left ) left = x;
        if ( y < top )  top = y;
    }
    result.setX(left);
    result.setY(top);
    return result;

}

QPointF GraphicsWorkflow::topLeftFromEllipse(const QString& p) {
    QRegExp rxCX("cx\\s*=\\s*\"([0-9\\-\\.,e\\s]+)\"");
    QRegExp rxCY("cy\\s*=\\s*\"([0-9\\-\\.,e\\s]+)\"");
    QRegExp rxRX("rx\\s*=\\s*\"([0-9\\-\\.,e\\s]+)\"");
    QRegExp rxRY("ry\\s*=\\s*\"([0-9\\-\\.,e\\s]+)\"");
    int current = 0;
    QPointF result;
    qreal top, left;
    qreal radiox = 0, radioy=0;
    bool ok;
    current = p.indexOf(rxRX);
    if ( current != -1 ) {
        radiox = rxRX.cap(1).toFloat(&ok);
    }
    current = p.indexOf(rxRY);
    if ( current != -1 ) {
        radioy = rxRY.cap(1).toFloat(&ok);
    }

    current = p.indexOf(rxCX);
    if ( current != -1 ) {
        left = rxCX.cap(1).toFloat(&ok)-radiox;
    }
    current = p.indexOf(rxCY);
    if ( current != -1 ) {
        top = rxCY.cap(1).toFloat(&ok)-radioy;
    }

    result.setX(left);
    result.setY(top);
    return result;


}

QMap<QString,QPair<QPointF,QString> > GraphicsWorkflow::getXmlIds(const QString& xml) {
     QMap<QString,QPair<QPointF,QString> > result;
     int current = 0, currentp = 0;
     QString keyitem;
     int posnextid;
     QRegExp rx("id\\s*=\\s*\"([a-zA-Z0-9]+)\"");
     QRegExp rxPoints("(points|d)\\s*=\\s*\"([0-9\\-\\.,MCe\\s]+)\"");
     QRegExp rxEllipse("<ellipse\\s+([a-zA-Z0-9#\\.e=\"\\-,;\\:\\s]+)/>");
     QRegExp rxText("<text[a-zA-Z0-9#\\.=\"\\-,;\\:\\s_]*>([a-zA-Z0-9#\\.\\-_]+)</text>");
     while ( true ) {
           current = xml.indexOf( rx, current);
           if ( current != -1) {
               QString namenode = rx.cap(1);
               current += namenode.length();
               if ( namenode.startsWith("graph")) {
                        continue;
                }
               currentp = xml.indexOf( rxPoints, current);
               posnextid = xml.indexOf("id=",current);               
               if ( currentp != -1 ) {
                   QPair<QPointF,QString> pair;
                   // **** Para chequear los operadores JOIN
                   int currentText = current;
                   while( true ) {
                       currentText = xml.indexOf(rxText,currentText);
                       if (currentText == -1 )  {
                           break;
                       }
                       currentText += (rxText.cap(0).length()+1);
                       QString mytext = rxText.cap(1);
                       if ( SafetYAWL::PATTERNS.split(",").contains(mytext) ) {
                           continue;
                       }
                       keyitem = rxText.cap(1);
                       break;
                   }
                   // **** Para chequear los operadores JOIN

                 if ( posnextid == -1 || (posnextid > currentp) ) {
                   QString points = rxPoints.cap(2);
                   QPointF topleft = topLeftFromPoints(points);
                   pair.first = topleft;
                   pair.second = keyitem;
                   result [ namenode ] = pair;
                  }
                 else {
                     currentp = xml.indexOf( rxEllipse, current);
                     if ( currentp != -1) {
                         QString points = rxEllipse.cap(1);
                         qDebug("nodo:|%s|....rxEllipse:|%s|",qPrintable(namenode),qPrintable(points));
                         QPointF topleft = topLeftFromEllipse(points);
                         pair.first = topleft;
                         pair.second = keyitem;
                         result [ namenode ] = pair;
                     }
                     else {
                         qDebug("ERROR DE ELLIPSE");
                     }

                 }
               }

           }
           else {
               break;
           }

     }
     //result.append("graph0");
     return result;

}

void GraphicsWorkflow::initialize() {
    if ( _currentgroup != NULL ) {
        unGroup();
        removeItems();
        setScene(NULL);
    }
    SceneWorkflow *scene = new SceneWorkflow(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    Q_CHECK_PTR(scene);
   // scene->setSceneRect(0,0,800,600);

}

void GraphicsWorkflow::deleteCurrentWorkflow() {
    if ( scene() == NULL ) return;
    foreach (QGraphicsItem *item, scene()->selectedItems()) {
           scene()->removeItem(item);
       }

}

void GraphicsWorkflow::addItem(const QString& name, const QMap<int,QString>& cmds, ItemType t) {
    //qDebug("...**agregando item....");
    QVariant mykey;
    QPair<QString,QString> mypair;
      switch( t ) {
           case SVGITEM:
                  qDebug("*******...**agregando SVGITEM item....");
                  _centralWidget = addAllSvgItems(name,cmds);
                  mykey.setValue(name);

                  Q_ASSERT ( currentGroup() != NULL );
                  qDebug("*******....GraphicsWorkflow::addItem..."
                         "agregando:|%s|",
                         qPrintable(name));
                  Q_CHECK_PTR (MainWindow::mymainwindow );
//                  qDebug("...GraphicsWorkflow::addItem...(1)..");
                  currentGroup()->setData(0,mykey);
//                  qDebug("...GraphicsWorkflow::addItem...(2)..");

                  mypair.first = SafetYAWL::lastgraph;
                  mypair.second = SafetYAWL::lastinfodate;
                  MainWindow::mymainwindow ->graphs()[name ] = mypair;


//                  qDebug("...GraphicsWorkflow::addItem...lastgraph\n|%s|..",
//                         qPrintable(MainWindow::mymainwindow ->graphs()[name ]));
                   break;
           case TEXTITEM:
           break;
              default:;
                   break;
      }


 }


bool GraphicsWorkflow::find(const QString& name) {

      return false;
 }

 void GraphicsWorkflow::removeItems() {

//      foreach(GraphicsSvgItemLive* myitem, _items) {
//           scene()->removeItem( myitem );
//      }
//      _centralWidget = NULL;
//      _items.clear();

 }


void GraphicsWorkflow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        _centralWidget->moveBy(0, -2);
        break;
    case Qt::Key_Down:
        _centralWidget->moveBy(0, 2);
        break;
    case Qt::Key_Left:
        _centralWidget->moveBy(-2, 0);
        break;
    case Qt::Key_Right:
        _centralWidget->moveBy(2, 0);
        break;
    case Qt::Key_Plus:
        //_centralWidget->scale(1.1,1.1);
        if ( scene()  ) {
            qDebug(".......+scene()->selectedItems().count():%d",scene()->selectedItems().count());
            if ( scene()->selectedItems().count() > 0) {
                scene()->selectedItems().first()->scale(1.01,1.01);
            }
        }
        //scaleView(qreal(1.1));
        break;
    case Qt::Key_Minus:
        if ( scene() && scene()->selectedItems().count() > 0 ) {
            scene()->selectedItems().first()->scale(1/1.01,1/1.01);
        }

        //scaleView(1 / qreal(1.1));
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
//        foreach (QGraphicsItem *item, scene()->items()) {
//            if (qgraphicsitem_cast<Node *>(item))
//                item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
//        }
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}


void GraphicsWorkflow::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 360.0));
}


void GraphicsWorkflow::scaleView(qreal scaleFactor)
{
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

