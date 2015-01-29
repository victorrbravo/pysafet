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
//
// C++ Interface: SafetTextParser
//
// Description: 
//
//
// Author: VÃ­ctor R. Bravo, Antonio Araujo Brett <vbravo@cenditel.gob.ve,aaraujo@cenditel.gob.ve>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SAFETTEXTPARSER_H
#define SAFETTEXTPARSER_H

#include <QPair>
#include <QStack>
#include <QtXml>
#include "SafetParser.h"
#include "SafetYAWL.h"
#include "cmdwidget.h"
#include "dommodel.h"


/**
	@author VÃ­ctor R. Bravo, Antonio Araujo Brett <vbravo@cenditel.gob.ve,aaraujo@cenditel.gob.ve>
*/
class SafetTextParser : public SafetParser {
Q_OBJECT

	QSet<QString> _values;
	QSet<QString> _fields;
	QMap<QString,QString> _commands;

	QString _xmlstring;
        QString operationname;
	int countxml;
        QDomDocument *domDocument, *inputDomDocument;       
    	bool docok;
	QString currcommand, currnametable;
        QString _curvaluekey;



        QPair<QString,QString> fieldspair;

	QString _texto;
        int currentcommand;
       QPair<QString,QString> nextXmlWord();
       // Estructura para las acciones (post) despues de ejecutar la sentencia SQL
       struct Action {
            QString type;
            QString key;
            QString info;
            Action(const QString& t, const QString& k, const QString& i) {
                 type = t;
                 key = k;
                 info = i;
            }
       };
       QList<Action> _actions;
       QMap< QString, QMap<QString,QVariant> > _confs;


       QString _currdeffile;

       QString _currentdataaction;
       QString _currentpostaction;
       DomModel *_dommodel;

public:
       // Validacion de campos obligatorios
       enum MandatoryClass  { OneOfAll, TwoOfAll };


       
        bool checkIsField(const QString& f);

	void setTexto(const QString& t) { _texto = t;}
	QString texto() const { return _texto;}

        /**
          \brief Devuelve el nombre de la operación de último XML de entrada leído
          */
        QString operationName() const { return operationname; }
        /**
          \brief Coloca el nombre de la operación de último XML de entrada leído
          \param s Nombre de la operación
          */
        void setOperationName(const QString& s) { operationname = s;}
        QStringList loadNameConfs();        
        void addConf(const QMap<QString,QVariant>& c, const QString& name) { _confs[name] = c; }
	SafetTextParser(QObject *parent = 0);
	~SafetTextParser();
        virtual bool parse(const QString& s = "");
	QMap<QString,QString>& commands()  { return _commands;}	
	void setCommands(const QStringList& list, const QStringList& otherlist);
	QString toXml(const QString& s = "");
        void openXml(const QString& f = "");
        QStringList processXml(bool doquery = true, bool dopermises = false);
        // Valores de la clave que se esta procesando
        QString curValuekey() { QString result = _curvaluekey; _curvaluekey = ""; return result;}
        void setCurValuekey(const QString& v) { _curvaluekey = v; }

        void setCurrentCommand(int n) { currentcommand = n;}
        int currentCommand() const { return currentcommand;}

//        void setCurrentAction(const QString& a) { _currentaction = a;}
        QString currentDataAction() const { return _currentdataaction;}
        QString currentPostAction() const { return _currentpostaction;}

        void processInput(const QString &s);
	bool executeSQL(const QString& s);
        QString getValueForFunction(const QString& v, const QString &format,
                                    const QMap<QString,QString> &vmap, const QString& keyvalue = QString(""));
        bool doAction(const QString& type, const QString& key, const QString& info);
        // getKey
        enum KeyType  { Sequence, Number, String };
        QString getValueOfKey(KeyType t, const QString& v);

        void checkWidgetsVarGlobals();
        /**
        ** \brief Metodo para validar campo y valor consultando el widget correspondiente
           \param namefield contiene el nombre de campo
           \param value respectivo valor del campo (lo cambia si es un titulo
           \param keyvalue valor actual de la clave
           \return "true" si El valor es valido para el campo, "false" en caso contrario
           */
        bool doFieldValidation(const QString& nameoperation, const QString& namefield, QString& value,
                               QString& usertooltip, QString keyvalue = QString()) const;
        /**
           * \brief Analiza sintácticamente una expresión SQL, y toma los datos (campos,nombre de la tabla, campo clave, etc) y
                    los coloca en la estructura de tipo ParsedSqlToData
           * \param s Cadena en formato SQL  (INSERT INTO...,UPDATE..,DELETE FROM...)
           * \return Estructura con los campos analizados
           */
         static ParsedSqlToData parseSql(const QString& s, bool parsetomap = false);

         /**
            * \brief Chequea la validez de un conjunto de campos obligatorios
             \param fields Lista de campos a validar
             \param ninput entrada a  validar
             \param t Tipo de validez
             \return Retorna True existe una condicion de obligatoriedad que no se cumple,
                         en caso contrario retorna falso

            */

         bool checkMandatoryClass(const QStringList& fields, const QDomNode& ninput,
                                                   SafetTextParser::MandatoryClass
                                                   t = OneOfAll);


         /**
           * \brief (Estatica) Reemplaza caracteres especiales para ser procesados en un documento XML (<> /b)
           * \param xml Cadena XML a ser procesado
           * \param in True si se procesa el original y False si se procesa el archivo con los caracteres reemplazados
           * \return Cadena XML procesada
           */
         QString replaceXmlCharSpecial(const QString& xml, bool in = true);

private:
         /*
           \brief Chequea si un determinado campo (f) es un campo a incluir en la consulta
           a una base de datos o es un campo a incluir para una accion (Action)
           \param fieldname Nombre del tipo de campo a consultar

           */
         bool checkIsActionsField(const QString& fieldname,
                                  const QString& key,
                                  const QString& info,
                                  const QMap<QString,QString>& fieldtypes);
	QDomNode findOperation(const QString& name);
	QPair<QString,QString> buildFields(const QDomElement& ecommand, const QDomNode& ninput, QMap<QString,QString>& keysforeignmap);

	QList<QPair<QString,QString> > getFieldsValues(const QDomElement& ecommand, const QDomNode& ninput, QMap<QString,QString>& keysforeignmap);

	QString processCommand(const QDomElement& e, const QDomNode& ninput,QMap<QString,QString>& keysforeignmap);
};

#endif

