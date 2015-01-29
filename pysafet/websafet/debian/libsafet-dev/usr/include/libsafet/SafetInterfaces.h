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
#ifndef SAFETINTERFACES_H
#define SAFETINTERFACES_H

#include <QtPlugin>
QT_BEGIN_NAMESPACE
class QWidget;
class QString;
class QStringList;
QT_END_NAMESPACE
#include "SafetXmlObject.h"
#include "../inflow/cmdwidget.h"

   /*
    * \brief Interfaz para complementos de visualización o salida
    */
class OutputInterface
{
public:
    virtual ~OutputInterface() {}

    virtual QString renderDocuments(const QString&, const QMap<QString,QString>&) const = 0;
    virtual QString parseCodeGraph(const QString&, const QMap<QString,QString>&) const = 0;
    virtual QString renderGraph(const QString&, const QString&, const QMap<QString,QString>&) = 0;
  
};
//! [0]

   /*
     * \brief Interfaz para implementar varios widgets que se cargan de forma din�mica
     */


class WidgetInterface {
     public:
     virtual QString text() const = 0;
     virtual void buildWidget()  = 0;
     virtual void setFocus(Qt::FocusReason reason) = 0;
     virtual int id() const = 0;
     virtual QString descriptor() const = 0;
     virtual bool doAction(const QString& key, const QString& info) = 0;
     virtual QString path() const = 0;
     virtual void setPath(const QString& p)  = 0;
     virtual CmdWidget* getWidget(const QString& field, QWidget *parent) = 0;
     virtual void setConf(QMap<QString, QVariant>&) = 0;
     virtual QMap<QString, QVariant>& conf()  = 0;


};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(OutputInterface,
                    "safet.libsafet.OutputInterface/1.0")
Q_DECLARE_INTERFACE(WidgetInterface,
                    "safet.libsafet.WidgetInterface/1.0")

QT_END_NAMESPACE

#endif
