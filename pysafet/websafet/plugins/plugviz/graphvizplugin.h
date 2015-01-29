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

#ifndef PLUGINVIZ_H
#define PLUGINVIZ_H

#include <QtCore>
#include <gvc.h>

#include "SafetInterfaces.h"
#include "SafetPlugin.h"




class graphvizPlugin : public SafetPlugin, public OutputInterface
{
    Q_OBJECT
    Q_INTERFACES(OutputInterface)

    // Valores por defecto
       QString defaultcolor;
       QString traccolor;
       QString linecolor;
       QString _typegraph;
       qreal distanceX;
       qreal distanceY;
       int distancecount;

public:

   // Tipos de datos
       enum ExtraInfoShow { Porc = 0x01, Tokens = 0x02, Total = 0x04, InfoText = 0x08,
                            InfoDate = 16};



              /**
                * \brief Obtiene la lista de documentos para un formato dado
                *
                */
   graphvizPlugin();

   virtual QString renderDocuments(const QString& docs, const QMap<QString,QString>& context) const;
   virtual QString renderGraph(const QString&, const QString&, const QMap<QString,QString>&);
   virtual QString parseCodeGraph(const QString&, const QMap<QString,QString>&) const;
   static QStringList  getTempNameFiles(int n);
   static QString processColor(const QString& c);



   // plugins.graphviz.extrainfo.show = Porc,Tokens,Total,InfoText,InfoDate
   // \param sshow parametro de la configuraci�n, linea completa en el archivo safet.conf
   static int checkExtrainfoShow(const QString& sshow);

   /**
     \brief Chequear si se coloca la cadena o no en el cuadro de Informaci�n extra
            Chequear "String Extra Show" (Si se muestra la cadena de informaci�n)
     */
   static QString checkSES(const QString &s, ExtraInfoShow ex, int curconf);
//   void buildGraph(const QString& code, const QString& info,
//                   const QMap<QString,QString>& context,QPainter &painter);
//   QPainterPath getPainterPath(const QString &title);
};


#endif
