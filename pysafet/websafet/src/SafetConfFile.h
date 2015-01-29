





//

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

// C++ Interface: SafetConfFile
//
// Description: 
//
//
// Author: Víctor Bravo, Antonio Araujo <vbravo@cenditel.gob.ve>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SAFETCONFFILE_H
#define SAFETCONFFILE_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include "SafetCipherFile.h"

/**
	@author Víctor Bravo, Antonio Araujo <vbravo@cenditel.gob.ve>
*/
class SafetConfFile : public QObject
{
Q_OBJECT
private:
        QMap<QString,QString> vmap;

    QString traduceKey(const QString& s,int type = 0) const;
public:

    SafetConfFile(QObject *parent = 0);
    QMap<QString,QString>& readFile(const QString& s,  bool iscipher = false );
    QVariant getValue(const QString& section, const QString& name);
    QVariant getValue(const QString& sectionname);
    ~SafetConfFile();
    QString operator[](const QString& sn);
    bool contains(const QString& k) const;
    QList<QString> keys() const { return vmap.keys(); }
    //int operator[](const QString& sn);
    bool add(const QString& key, const QString& value);
    int change(const QString& value,const QString& newvalue, const QString& prefixkey,bool traduce = true);
    int getNumberRegister(const QString& value,const QString& prefixkey,bool traduce = true);
    QMap<QString,QString>& getMap() { return vmap;}
};

#endif
