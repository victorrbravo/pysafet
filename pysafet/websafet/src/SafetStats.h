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
#ifndef SAFETSTATS_H
#define SAFETSTATS_H


#include <QObject>
#include <QMap>
#include <QVariant>
#include <QFile>
#include <QRegExp>
#include <QTextStream>

#include "SafetWorkflow.h"

//class SafetWorkflow;
//class SafetTask;
//class SafetVariable;

/**
        @author Vi�ctor Bravo, Antonio Araujo <vbravo@cenditel.gob.ve>
*/
namespace Safet {


class SafetStats : public QObject {
     Q_OBJECT

     SafetWorkflow* mywf;
     long int stats;
     int _totaltokens;

     bool _isreportvalue;

public:
    /* Enumerados para el manejo de estadísticas */

     /* Manejo de la Salida de la Estadistica */
    enum AttrType { TaskColor, TaskSize, TaskCustom, TaskLine,  ArrowColor, ArrowSize, ArrowCustom, ArrowLine, AttrNone };

    /* Manejo del tipo de estadística */
    enum StatsType { Coloured = 2, ForTokenKey = 4, None = 8 };

    SafetStats(SafetWorkflow *wf = NULL, QObject *parent = 0);
    void setWorkflow(SafetWorkflow *wf);
    AttrType outputType() const { return _outputtype; }
    void setOutputType(AttrType o) { _outputtype = o; }
    QString getInfoString(SafetStats::AttrType o );
    void reset() { mywf = NULL; }
    int numberOfTokens(const QString& idtask, bool norender, const QString& info = QString(""));
    /*
      * \brief Calcula el numero de elementos dado una clave de una variable especifica
      * \param v Variable a calcular
      * \param info Clave a evaluar
      * \return numero de elementos
      */
    int tokensForKey(SafetVariable& v, const QString& info, bool norender);

    QStringList processInfo(SafetNode* node, SafetStats::StatsType t, const QString& a,
                            bool norender,
                            const QString& info = QString(""));
    /*
      * \brief Calcula el numero de fichas del flujo de trabajo
      * \param all Indica si se incluyen todas las tareas o solo la que tengan la opcion report en "yes"
      * \return Retorna el numero total de fichas del flujo de trabajo
      */
    int numberAllOfTokens(bool norender, const QString& info = QString("") );
    SafetStats::AttrType parseAttrType(const QString& a);
    bool isWorkflow() { return mywf != NULL; }
    ~SafetStats();

    private:
     AttrType _outputtype;
     /**
       * \brief Chequea si exite informacion extra (Rol y estampilla de tiempo)
       * \param wf Objeto flujo de trabajo actual
       * \param fields campos de la consulta actual
       * \param info clave actual o informacion coloured
       *\return true si se agregan información extra, false en caso contrario
       */
     bool checkExtraInfo(SafetWorkflow *wf, QStringList& fields, const QString& info);

};

}

#endif // SAFETSTATS_H

