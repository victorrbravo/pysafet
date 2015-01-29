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
#include "SafetPlugin.h"

SafetPlugin::SafetPlugin()
{
}


QString SafetPlugin::conf() const {
     QString newname;
     QString name, allname = QString(metaObject()->className());
     int n = allname.indexOf("Plugin");
     Q_ASSERT_X( n >= 0, "Plugins", qPrintable(tr("El Plugins llamado \"%1\" no finaliza con la palabra \"Plugin\"").arg(allname)));
     name = allname.mid(0,n);
     Q_ASSERT_X( n > 1, "Plugins", qPrintable(tr("El nombre del complemento \"%1\" debe tener una longitud mayor o igual a dos (2)").arg(name)));
     name = name.mid(0,1).toUpper()+name.mid(1,name.length()-1);

     newname = QString("Plugins") + QString(".") + name;
     newname = newname + "/" + newname.toLower() + ".";
     return newname;

}
