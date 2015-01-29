/*
* SAFET Sistema Automatizado para la Firma Electr√≥nica y Estampado de Tiempo
* Copyright (C) 2008 V√≠ctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*     
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigaci√≥n en Tecnolog√≠as Libres 
*  
* Este programa es software libre; Usted puede usarlo bajo los t√©rminos de la licencia de 
* software GPL versi√≥n 2.0 de la Free Software Foundation. 
* 
* Este programa se distribuye con la esperanza de que sea √∫til, pero SI NINGUNA GARANT√çA;
* tampoco las impl√≠citas garant√≠as de MERCANTILIDAD o ADECUACI√ìN A UN PROP√ìSITO PARTICULAR.
* Consulte la licencia GPL para m√°s detalles. Usted debe recibir una copia de la GPL junto 
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5¬∫ Piso, Boston, MA 02110-1301, USA.
*
*/

//
// C++ Implementation: SafetConfFile
//
// Description: 
//
//
// Author: V√≠ctor Bravo, Antonio Araujo <vbravo@cenditel.gob.ve>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QtCore>
#include "SafetConfFile.h"
#include "SafetYAWL.h"

SafetConfFile::SafetConfFile(QObject *parent)
 : QObject(parent) {

}


SafetConfFile::~SafetConfFile() {

}

QMap<QString,QString>& SafetConfFile::readFile(const QString& s, bool iscipher) {
     //qDebug("................SafetConfFile::readFile.....: |%s|", qPrintable(s));
     vmap.clear();
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
     SafetCipherFile file(s);
#else
    QFile file(s);
#endif // OpenSSL

     bool isopen;
     if (iscipher ) {
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
        isopen = file.open(QIODevice::ReadOnly);
#endif // OpenSSL
    }
     else {
         isopen = file.open(QIODevice::ReadOnly | QIODevice::Text);
     }
     Q_ASSERT_X(isopen == true,"readFile",qPrintable(tr("No es posible abrir el archivo de configuraci√≥n %1").arg(s)));    
     QRegExp rxComment("\\s*\\#(.*)");
     QRegExp rxSection("\\s*\\[([a-zA-Z0-9\\s\\.\\-]+)\\]");
     QRegExp rxValue(trUtf8("\\s*([a-zA-Z0-9\\$\\./\\*;]+)\\s*\\="
                        "\\s*([\\*\\[\\]a-zA-Z0-9·ÈÌÛ˙Ò—'\\$\\#/\\-\\._/:!\\?\\^\\$%%\\(\\)<>/\\+]"
                        "[\\{\\}@\\*\\[\\]a-zA-Z0-9·ÈÌÛ˙Ò—;'\\$\\#/\\-\\._/:!\\?\\^\\$\\s%%\\(\\),;<>/\\s\\+\\?]*)"));

     if (iscipher) {
//         qDebug("........iscipher....");
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
         file.setPassword("caracas");
         file.readAllCipher();
#endif
     }

     if (iscipher ) {
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
         int pos, nline = 0, nlinecomments = 0;
         QString section, path;
         while (true) {
             nline++;
             //qDebug("...leyendo la linea: %d", nline);
             QString line = file.readCipherLine();
             if (line.isNull()) {
                 break;
             }

             if ( line.trimmed().length() == 0 ) {
                 continue;
             }
             //qDebug("...line:\n%s", qPrintable(line));
             pos = rxComment.indexIn(line);	// pos == -1 no coincide con la expresi√≥n regular
             if ( pos != -1 ) nlinecomments++;
             pos = rxSection.indexIn(line);	// pos == -1 no coincide con la expresi√≥n regular
             if ( pos != -1 ) {
        //            qDebug("\nSeccion en la linea %d", nline);
          //          qDebug("Seccion:%s", qPrintable(rxSection.cap(1)));
                    section = rxSection.cap(1);

             }
             pos = rxValue.indexIn(line);	// pos == -1 no coincide con la expresi√≥n regular

             if ( pos != -1 ) {
                   //qDebug("  **line: %s", qPrintable(rxValue.cap(2).trimmed()));
                    Q_ASSERT_X( section.length() > 0, "readFile",
                                qPrintable(tr("Existe un error en el archivo."
                                              "Falta definir por lo menos una seccion")) );
                    path = section + "/" + rxValue.cap(1);

                    if ( path.endsWith(".*") ) {
                        if ( !vmap.contains( path ) ) {
                            vmap[ path ] = rxValue.cap(2).trimmed();
                        }
                        else {
                            vmap[ path ] = vmap[ path ]  + SafetYAWL::LISTSEPARATORCHARACTER + rxValue.cap(2);
                        }
                    }
                    else {
                        if (!vmap.contains( path ) ) {
                  //          qDebug("...%s = %s",qPrintable(path), qPrintable(rxValue.cap(2).trimmed()));
                            vmap[ path ] = rxValue.cap(2).trimmed();
                        }
                    }
             }
        }
#endif // OpenSSL
     }
     else {
         QTextStream in(&file);
         in.setCodec("ISO 8859-1");
         int pos, nline = 0, nlinecomments = 0;
         QString section, path;
         while (!in.atEnd()) {
             nline++;
             QString line = in.readLine();
             if ( line.trimmed().length() == 0 ) continue;
             pos = rxComment.indexIn(line);	// pos == -1 no coincide con la expresi√≥n regular
             if ( pos != -1 ) nlinecomments++;
             pos = rxSection.indexIn(line);	// pos == -1 no coincide con la expresi√≥n regular
             if ( pos != -1 ) {
        //            qDebug("\nSeccion en la linea %d", nline);
          //          qDebug("Seccion:%s", qPrintable(rxSection.cap(1)));
                    section = rxSection.cap(1);

             }
             pos = rxValue.indexIn(line);	// pos == -1 no coincide con la expresi√≥n regular
             if ( pos != -1 ) {
      //             qDebug("  **line: %s", qPrintable(rxValue.cap(2)));
                    Q_ASSERT_X( section.length() > 0, "readFile", qPrintable(tr("Existe un error en el archivo. Falta definir por lo menos una seccion")) );
                    path = section + "/" + rxValue.cap(1);
    //                qDebug("...path : %s",qPrintable(path));
//                    qDebug("...%s = %s",qPrintable(path), qPrintable(rxValue.cap(2).trimmed()));
                    if ( path.endsWith(".*") ) {
                        if ( !vmap.contains( path ) ) {
                            vmap[ path ] = rxValue.cap(2).trimmed();
                        }
                        else {
                            vmap[ path ] = vmap[ path ]  + SafetYAWL::LISTSEPARATORCHARACTER + rxValue.cap(2);
                        }
                    }
                    else {
                        vmap[ path ] = rxValue.cap(2).trimmed();
                    }
             }
        }
     }
     return vmap;
}

QVariant SafetConfFile::getValue(const QString& section, const QString& name) {

	if ( SafetYAWL::evalExit() != NULL ) {	
            if (vmap.contains(section.trimmed() + "/" + name.trimmed()) == false) {
                SafetYAWL::streamlog << SafetLog::Warning << tr("No existe la clave \"%1\" "
                                                                "en el archivo de configuraci√≥n")
                        .arg(section.trimmed() + "/" + name.trimmed());
            }
	}
        QVariant var = vmap[ section.trimmed() + "/" + name.trimmed() ];
	if ( SafetYAWL::evalExit() != NULL ) {	
            if ( var.type() == QVariant::Invalid ) {
                SafetYAWL::streamlog << SafetLog::Warning << tr("Tipo de dato inv√°lido para la el valor:  \"%1\" en el archivo de configuracion \"safet.conf\"").arg(section.trimmed() + "/" + name.trimmed());
            }
	}
	return var;
}

bool SafetConfFile::contains(const QString& k) const {
    QString s0 = k.section("/",0,0);
    QString s1 = k.section("/",1,1);
    if (!s1.startsWith(s0.toLower()) ) {
         s1 = s0.toLower() + "."+ s1;
    }
    QString newsn = s0+"/"+s1;
    return vmap.contains( newsn );

}

bool SafetConfFile::add(const QString& key, const QString& value) {


    bool result =  vmap.contains(key);
    vmap[ traduceKey(key) ] = value;

    return result;

}

QString SafetConfFile::traduceKey(const QString& s, int type) const
{

    QString newsn,news0;
    QString s0,s1;


    switch ( type ) {
    case 0:
        s0 = s.section("/",0,0);
        s1 = s.section("/",1,1);
        if (!s1.startsWith(s0.toLower()) ) {
             s1 = s0.toLower() + "."+ s1;
        }
        newsn = s0+"/"+s1;
        break;
    case 1:
        if (s.length() < 2 ) {
            return "";
        }
        s0 = s.section(".",0,0);
        s1 = s0.toLower()+"."+s.section(".",1,1);

        news0 = s0.mid(0,1).toUpper()+s0.mid(1);

        newsn = news0+"/"+s1;

        break;


    }
    return newsn;

}



int  SafetConfFile::change(const QString& value,const QString& newvalue, const QString& prefixkey, bool traduce ) {

    QString searchkey;

    int result = 0;
    int count = 1;
    while(true ) {
        QString traducekey = (traduce?traduceKey(prefixkey):prefixkey);
        searchkey = QString("%1.%2").arg(traducekey).arg(count);
        if ( !vmap.contains(searchkey)) {
            break;
        }
        if ( vmap[searchkey] == value ) {
             vmap[searchkey] = newvalue;
             result = count;
             break;
        }
        count++;
    }

    return result;
}

int  SafetConfFile::getNumberRegister(const QString& value, const QString& prefixkey, bool traduce ) {

    QString searchkey;

    int result = 0;
    int count = 1;
    while(true ) {
        QString traducekey = (traduce?traduceKey(prefixkey,1):prefixkey);
        searchkey = QString("%1.%2").arg(traducekey,1).arg(count);
        if ( !vmap.contains(searchkey)) {
            break;
        }
        if ( vmap[searchkey] == value ) {
             result = count;
             break;
        }
        count++;
    }

    return result;
}

QString SafetConfFile::operator[](const QString& sn) {
        if ( !vmap.contains(traduceKey(sn)) ) {
            SafetYAWL::streamlog << SafetLog::Warning << tr("No existe la clave \"%1\" en el archivo de configuracion \"safet.conf\"").arg( traduceKey(sn));
            return "";
        }

        QVariant var = vmap[ traduceKey(sn) ];
        if ( var.type() == QVariant::Invalid ) {
            SafetYAWL::streamlog  << SafetLog::Warning << tr("Tipo de dato invalido para la el valor:  \"%1\" en el archivo de configuracion \"safet.conf\"").arg( traduceKey(sn));
        }

        return var.toString();

}





QVariant SafetConfFile::getValue(const QString& sectionname) {


    if ( vmap.contains(sectionname) != true ) {
        SafetYAWL::streamlog
                << SafetLog::Error
                <<
            tr("No existe la clave (secci√≥n)  \"%1\" en el archivo de configuracion \"safet.conf\"").arg(sectionname);

    }

		//SafetYAWL::evalAndCheckExit(vmap.contains(sectionname) == true, tr("No existe la clave  \"%1\" en el archivo de configuracion \"safet.conf\"").arg(sectionname));

        QVariant var = vmap[ sectionname.trimmed() ];
        if (var.type() == QVariant::Invalid) {

            SafetYAWL::streamlog
                    << SafetLog::Error <<
                tr("Tipo de dato inv√°lido para el valor:  \"%1\" en el archivo de configuracion \"safet.conf\"").arg(sectionname);
	}
	return var;
}

