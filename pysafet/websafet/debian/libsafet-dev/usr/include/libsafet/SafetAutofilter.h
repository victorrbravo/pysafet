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
#ifndef SAFETAUTOFILTER_H
#define SAFETAUTOFILTER_H

#include "SafetXmlObject.h"
#include "SafetTask.h"
#include "SafetSQLParser.h"

class SafetSQLParser;
class SafetTask;
class SafetWorkflow;



class SafetAutofilter : public SafetXmlObject
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
     Q_PROPERTY(QString subreport READ report WRITE setSubreport)
     Q_PROPERTY(QString recursive READ recursive WRITE setRecursive)
     QString _id, _side, _source, _type,  _query, _options, _tokenlink,_pattern, _report, _subreport,_recursive;

     QStringList getoptions; // Para evaluar las opciones generadas en el autofiltro
     QStringList getvaluesoptions; // Para ver las descripciones generadas en el autofiltro
     QMap<QString,QStringList> getsecondoptions;
     int _countoptions;
     SafetSQLParser localparser;
     SafetTask *filtertask;
     QString _subsql;
public:
     enum FilterType { None, String, Date, DateTime, Integer, Float};
     enum DateFormat { ISO, DATESTRING, SECONDS };
private:
     FilterType _filterType;
public:

     SafetAutofilter();
    QString id() const {return _id; };
    void setId(const QString& i) { _id = i; }
    QString side() const {return _side; };
    void setSide(const QString& i) { _side = i; }
    QString source() const {return _source; };
    void setSource(const QString& i) { _source = i; }
    QString type() const {return _type; };
    void setType(const QString& i) { _type = i; }
    void setQuery(QString s) { _query = s; }
    QString query() const { return _query;}
    void setOptions(QString s) { _options = s; }
    QString options() const { return _options;}
    void setTokenlink(QString s) { _tokenlink = s; }
    QString tokenlink() const { return _tokenlink;}
    QString pattern() const {return _pattern; };    
    void setPattern(const QString& i) { _pattern = i; }

    QString report() const {return _report; };
    void setReport(const QString& i) { _report = i; }

    QString subreport() const {return _subreport; };
    void setSubreport(const QString& i) { _subreport = i; }

    QString recursive() const {return _recursive; };
    void setRecursive(const QString& i) { _recursive = i; }

    FilterType filterType() const { return _filterType; }
    void setFilterType(FilterType t)  { _filterType = t; }

    /**
      * \brief Crea el conjunto subtareas creada para una tarea
      del AutoFiltro
      */
    QList<SafetTask*> createSubTasks(SafetTask* task, QStringList subtasks, const QString& prefix = QString());
    /*
         * \brief Crea el conjunto de tareas (SafetTask) que genera el autofiltro
         * \return Lista de tareas (SafetTask) generada
         */
    QList<SafetTask*> createTasks(const QString& prefix = "");
    /*
      * \brief reinicia el contador para las opciones generadas del autofiltro
      *
      */
    void resetOptions() { _countoptions = 0; }

    /*
      * \param Coloca la tarea a utilizar para generar el autofiltro
      * \brief t tarea colocada para generar el autofiltro
      */
    void setFiltertask(SafetTask* t);
    /*
          * \brief Modifica la lista de conexiones del Puerto correspondiente de la tarea
          * \param lt Lista de Tareas para agregar las conexiones
    */
    void modifyForAutofilter(const QList<SafetTask*>& lt);

    /*
      * \brief Obtiene la siguiente opcion del filtro
      * \return Retorna la proxima opción del filtro, se reinicia al llegar al último
      *
      */

    QString getNextOption();

    /*
      * \brief Genera la lista de opciones para el autofiltro
      *
      */
    QStringList generateFilterOptions();


    void generateDateSubFilterOptions(const QString& firstfield,const QString& secondfield,const QString& table);

    /*
      * \brief coloca la consulta (query)  y las opciones (options) en funcion de la tarea (SafetTask*) antecesora
      * \param tasks Lista de las nuevas tareas creadas por el autofiltro
      */
    void normalizeTasks(const QList<SafetTask*>& tasks);


    /*
      * \brief Genera la lista de opciones del autofiltro, cuando el dato es tipo fecha/hora
      * Afecta a la lista "getoptions"
      * \param error Si no ocurrio error el valor es igual 0, en caso contrario es mayor que 1
      */
    bool generateDateFilters(const QSqlQuery& query, const QString& table, int &error);

};

#endif // SAFETAUTOFILTER_H
