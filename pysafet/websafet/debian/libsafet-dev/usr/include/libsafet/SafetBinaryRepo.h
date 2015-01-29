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

#ifndef SAFETBINARYREPO_H
#define SAFETBINARYREPO_H

#include <QMetaType>
#include <QSqlDatabase>
#include "SafetXmlObject.h"

class SafetBinaryRepo : public SafetXmlObject
{
    //QFile _filedata;
    QList<QMetaType::Type> _typedata;
    QString _namedata;
    QSqlDatabase _binaryDb;
    QString _templatecreate;
    QString _templateinsert;
    QString _templatedrop;
    QString _templatedeleteall;
    QString _templatedeleteone;
    QString _templatesearch;
    QString _templatesearchall;
    QString _currpathname;
    bool _fileexists;



public:
    SafetBinaryRepo(const QString& name = QString("stats.db"));

    QSqlDatabase& binaryDb() { return _binaryDb; }

    bool open();
    bool  createTable(const QString &nametable, const QString &fields);

    /**
      \brief Obtiene los datos en formato JSON, filter, es la clausula "where" de la sentencia
             SQL
             \return primer campo datos, segundo campo nombre de los campos de la tabla
      */
    QPair<QString,QString> dataJSON(const QString &nametable,
                                    const QString& filter = "",
                                    const QStringList& todatefields = QStringList(),
                                    const QStringList& tospandatefields = QStringList()
                                    );
    void append(const QString &nametable, const QString &fields, const QString& values);
    QString search(const QString &nametable, const QString &key = "");
    bool modify(const QString &key);
    int deleteRecords(const QString &nametable, const QString &whereclause = QString("1"));
    bool fileExists() { return _fileexists; }

    QString calculateAverage(const QString& nametable,const QString& key,
                             int fromdate = -1,
                             int todate = -1);

};

#endif // SAFETBINARYREPO_H
