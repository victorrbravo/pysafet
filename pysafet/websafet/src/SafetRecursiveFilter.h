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
#ifndef SAFETRECURSIVEFILTER_H
#define SAFETRECURSIVEFILTER_H

#include "SafetXmlObject.h"
#include "SafetTask.h"
#include "SafetSQLParser.h"

class SafetSQLParser;
class SafetTask;
class SafetWorkflow;



class SafetRecursivefilter : public SafetXmlObject
{

     Q_OBJECT
     Q_PROPERTY(QString id READ id WRITE setId)
     Q_PROPERTY(QString side READ side WRITE setSide)
     Q_PROPERTY(QString source READ source WRITE setSource)
     Q_PROPERTY(QString type READ type WRITE setType)
     Q_PROPERTY(QString query READ query WRITE setQuery)
     Q_PROPERTY(QString options READ options WRITE setOptions)
     Q_PROPERTY(QString tokenlink READ tokenlink WRITE setTokenlink)
     Q_PROPERTY(QString pattern READ pattern WRITE setPattern)
     Q_PROPERTY(QString report READ report WRITE setReport)
     Q_PROPERTY(QString initial READ initial WRITE setInitial)
     Q_PROPERTY(QString filter READ filter WRITE setFilter)
     Q_PROPERTY(QString onlyfathers READ onlyfathers WRITE setOnlyfathers)
     QString _id, _side, _source, _type,  _query, _options, _tokenlink,_pattern, _report, _initial,
     _filter, _onlyfathers;

     QStringList getoptions; // Para evaluar las opciones generadas en el autofiltro
     QStringList getvaluesoptions; // Para ver las descripciones generadas en el autofiltro

     int _countoptions;
     SafetSQLParser localparser;
     SafetTask *filtertask;
    QList<SafetTask*> listnewtasks;
    bool wasconnsdeleted;
public:
     enum FilterType { None, String, Date, DateTime, Integer, Float};
     enum DateFormat { ISO, DATESTRING, SECONDS };
     enum CurrentQueryType { INITIAL, CURRENTREG };
private:
     FilterType _filterType;
public:

     SafetRecursivefilter();
    QString id() const {return _id; }
    void setId(const QString& i) { _id = i; }
    QString side() const {return _side; }
    void setSide(const QString& i) { _side = i; }
    QString source() const {return _source; }
    void setSource(const QString& i) { _source = i; }
    QString type() const {return _type; }
    void setType(const QString& i) { _type = i; }
    void setQuery(QString s) { _query = s; }
    QString query() const { return _query;}
    void setOptions(QString s) { _options = s; }
    QString options() const { return _options;}
    void setTokenlink(QString s) { _tokenlink = s; }
    QString tokenlink() const { return _tokenlink;}
    QString pattern() const {return _pattern; }
    void setPattern(const QString& i) { _pattern = i; }

    QString report() const {return _report; }
    void setReport(const QString& i) { _report = i; }

    QString initial() const {return _initial; }
    void setInitial(const QString& i) { _initial = i; }

    QString filter() const {return _filter; }
    void setFilter(const QString& i) { _filter = i; }
    QString onlyfathers() const {return _onlyfathers; }
    void setOnlyfathers(const QString& o) { _onlyfathers = o; }

    FilterType filterType() const { return _filterType; }
    void setFilterType(FilterType t)  { _filterType = t; }

    /*
         * \brief Crea el conjunto de tareas (SafetTask) que genera el autofiltro
         * \return Lista de tareas (SafetTask) generada
         */
    QList<SafetTask*> createTasks(const QString& prefix = "");

    /**
      * \brief Crear una tarea
      */
    SafetTask* createTask(const QString& title, const QString& namefield,
                          const QString& info = QString(tr("No_info")));


    /*
      * \param Coloca la tarea a utilizar para generar el autofiltro
      * \brief t tarea colocada para generar el autofiltro
      */
    void setFiltertask(SafetTask* t);

    /*
      * \brief Construye las tareas recursivas según el campo especificado en el archivo de flujo de trabajo
      *
      */
    void generateRecursiveTask();



    /**
      * \brief Enlazando tareas
      */
    void linkTasks(SafetTask* tasksource, SafetTask* tasktarget, const QString& sql, const QString& opts);


    QPair<QString,QString> getFieldsFromQuery(const QString& q, QString& basicsql);
    bool hasChildren(const QString& sql);
};

#endif // SAFETRECURSIVEFILTER_H
