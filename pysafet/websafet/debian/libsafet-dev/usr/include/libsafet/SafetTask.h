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

#ifndef SAFETTASK_H_
#define SAFETTASK_H_


#include "SafetNode.h"
#include "SafetConnection.h"
#include "SafetPort.h"
#include "SafetVariable.h"
#include "SafetAutofilter.h"
#include <QtXml>

/**
* \brief Clase SafetTask
* 
* Esta Clase corresponde al objeto  Tarea o Actividad. El objeto Tarea (actividad)
* especifica el proceso,conjunto de pasos vistos desde una misma acción, es 
* un elemento del modelo YAWL
*  
* Los objetos SafetTask son contenidos dentro de el objeto SafetWorkflow 
* 
* Deriva de la Clase SafetNode, lo que quiere decir que se considera un 
* nodo en el grafo del flujo de trabajo
*  
*/

class SafetAutofilter;

class SafetTask : public SafetNode  {
	
	Q_OBJECT	
	Q_PROPERTY(QString id READ id WRITE setId)

    Q_PROPERTY(QString multiplicity READ multiplicity WRITE setMultiplicity)
    Q_PROPERTY(QString type READ type WRITE setType)

    Q_PROPERTY(QString role READ type WRITE setRole)
    Q_PROPERTY(QString note READ type WRITE setNote)
    Q_PROPERTY(QString hideuntil READ type WRITE setHideuntil)

	
    QList<SafetVariable*> variablelist;
        QString _id, _title, _multiplicity, _type;
        QList<SafetTask*> aflist; // Tareas para el autofiltro
        QList<SafetTask*> rflist; // Tareas para el filtro Recursivo
        QString _tsvalue;
        QString _role;
         QString _note;
	QString _hideuntil;

public:
	SafetTask();
        ~SafetTask();
	virtual void addChild(SafetXmlObject* o);

        QString getPrevTask();
        /*
          * \brief Agrega un objeto puerto (SafetPort) a la tarea (SafetTask).
          * \param p Puerto a agregar
          */
        void addPort(SafetPort* p );

        /*
          * \brief Agrega un objeto puerto (SafetPort) a la tarea (Task).
          * \param v Variable a agregar
          */
        void addVariable(SafetVariable* v );

        QList<SafetVariable*>& getVariables();
        /*
          * \brief Sincronización del puerto de "control" con el puerto a añadir en la tarea
          * \param p Puerto a ser sincronizado con el puerto "control"
          */
        void syncPort(SafetPort* p);

	
	QString id() const {return _id; };
	void setId(const QString& i) { _id = i; }

	
	QString multiplicity() const {return _title; };
	void setMultiplicity(const QString& i) { _multiplicity = i; }

	QString type() const {return _type; };
	void setType(const QString& i) { _type = i; }

    QString role() const {return _role; };
    void setRole(const QString& i) { _role = i; }

    QString note() const {return _note; };
    void setNote(const QString& i) { _note = i; }

    QString hideuntil() const {return _hideuntil; };
    void setHideuntil(const QString& i) { _hideuntil = i; }


        QString tsvalue() { return _tsvalue;}
        void setTsValue(const QString& t) { _tsvalue = t;}


 };

#endif /*SAFETTASK_H_*/
