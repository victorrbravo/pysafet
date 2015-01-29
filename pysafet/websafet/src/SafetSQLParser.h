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

#ifndef SAFETSQLPARSER_H_
#define SAFETSQLPARSER_H_

#include "SafetParser.h"

class SafetWorkflow;
class SafetVariable;


class SafetSQLParser : public SafetParser {
public:
	enum addType { Begin, End };
private:
	Q_OBJECT
	
	QStringList _fields;
	QSet<QString> _whereclauses;
	QSet<QString> _joinclauses;
	QSet<QString> _signclauses;
	QString _tablesource, _signclause, _signers;
    QString _currentflowname;

	QRegExp rxSql;
	void clean();
        QRegExp rxSqlSign;
        SafetWorkflow* wf;
public:
        SafetSQLParser();
	QString sql() const { return str();}
	void setSql(const QString& s) { setStr(s.toUtf8()); }
        virtual bool parse(const QString& s = "");
        QStringList getFields() const;
	QString getTablesource() const;
	QSet<QString> getWhereclauses() const;
	QSet<QString> getSignclauses() const;
        QSet<QString> getJoinclauses() const;		
        void addFieldToList(const QString& f);
	QString addField(const QString& f, addType t = SafetSQLParser:: Begin);
	QString checkBrackets(const QString& sql, bool extract = false);
	QString dropWhereClauses(bool drop = false);
        QString getWhereClause();
	QString signClause() const { return _signclause; }
    QString currentFlowName() const { return _currentflowname; }
	QString signers() const { return _signers;}
        QString replaceVariableName(const QString& n);
        void setWorkflow(SafetWorkflow *w) { wf = w; }
        SafetWorkflow* workflow() const { return wf;}
};

#endif /*SAFETSQLPARSER_H_*/
