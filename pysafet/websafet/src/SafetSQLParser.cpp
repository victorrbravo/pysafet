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

#include "SafetYAWL.h"
#include "SafetWorkflow.h"
#include "SafetSQLParser.h"


SafetSQLParser::SafetSQLParser()
{
	 _error = CORRECT;
         wf = NULL;
         rxSql.setPattern("\\(?\\s*SELECT\\s+([a-zA-Z0-9_\\.\\(\\)][a-zA-Z0-9_,'\\s\\.\\(\\)\\-\\|/]*)\\s+FROM\\s+([a-zA-Z0-9_=\\.\\(\\)]+)\\s?(\\s*WHERE\\s+(.+))?(\\s*(SIGN|PASS)\\s*([a-zA-Z0-9_,;\\s\\.\\(\\)\\*#!\\|]+)?)?");
         rxSqlSign.setPattern("\\s*([a-zA-Z0-9_][a-zA-Z0-9_\\.\\-]*)(\\s+(SIGN|PASS)\\s+[a-zA-Z0-9_#\\*!\\|][a-zA-Z0-9_\\.\\s,;\\*#!\\|]+)");

    	 rxSql.setCaseSensitivity(Qt::CaseInsensitive);
         rxSqlSign.setCaseSensitivity(Qt::CaseInsensitive);
}


QString SafetSQLParser::checkBrackets(const QString& sql, bool extract) {
	int balanced = 0;
	QString mysql = sql;
	for ( int i = 0 ; i < mysql.length(); i++ ) {
		if ( mysql.at(i) == '(' ) {
			balanced++;
		}
		else if ( mysql.at(i) == ')' ) {
			balanced--;
		}	
	}
	if ( extract ) {
		mysql.remove(QRegExp("[\\(\\)]."));
	} 
	_error = (balanced == 0 ) ? CORRECT : BRACKETSNOBALANCE;
	return mysql;
}

QString SafetSQLParser::replaceVariableName(const QString& n) {
       QString result;
       SafetVariable* v = wf->searchVariable(n);

       if ( v != NULL ) {
            result = v->documentsource();
       }


       return result;

}

bool SafetSQLParser::parse(const QString& s) {
	clean();
          QString news;
          if ( s.length() > 0 ) {
              news = s;
         }
          else {
              news = sql();
          }

          SYD << tr("........SafetSQLParser::parse....NEWS SQL:|%1|")
                 .arg(news);
         // news.replace("|",Safet::PIPEMARK);
          setSql(news);

        if ( wf != NULL ) {
             int pos = rxSqlSign.indexIn( sql() ) ;
             if ( pos >= 0 ) {
                  QString namevar = rxSqlSign.cap(1);
                  QString postclause = rxSqlSign.cap(2).trimmed();

                  SYD << tr("........SafetSQLParser::parse....before sign sql:|%1|....***namevar:|%2|....cap3:|%3|")

                         .arg(sql())
                         .arg(namevar)
                         .arg(postclause);

                  QString varsql;
                  if ( postclause.startsWith("PASS ")) {
                      _currentflowname = namevar;
                      varsql = QString("SELECT true FROM %1 %2")
                              .arg(_currentflowname)
                              .arg(postclause);
                      setSql(varsql);
                  }
                  else {
                      varsql = replaceVariableName(namevar);
                      if ( varsql.length() > 0 ) {
                           varsql = varsql + rxSqlSign.cap(2);
                           setSql(varsql);
                      }
                  }
             }
        }

	rxSql.setCaseSensitivity( Qt::CaseInsensitive ); 
	QString mysql = sql();
        int pos = rxSql.indexIn(mysql);
	if (pos < 0 ) { 

                   SYE << tr("Analizador SQL: La expresion \"%1\" no es una expresi�n SQL correcta")
                         .arg(mysql);
                _error = INCORRECT;
                return false;
	}

	Q_ASSERT_X(pos > -1,"parse", qPrintable(tr("La cadena evaluada (%1) NO es una sentencia SQL").arg(_str)));
	QString fields = rxSql.cap(1);
	_fields  = fields.split(QRegExp("\\s*,\\s*"));
	_tablesource = rxSql.cap(2);
	if (rxSql.cap(3).length() > 0 ) {
        Q_ASSERT_X(rxSql.cap(4).length( )>0,"parse",qPrintable(tr("La clausula WHERE est� incompleta")));
		QString clauses = rxSql.cap(4);
		QRegExp rxWhere("\\s+(AND|OR|XOR)\\s+");
		rxWhere.setCaseSensitivity(Qt::CaseInsensitive);
		_whereclauses = QSet<QString>::fromList(clauses.split(rxWhere));
	}
        if (rxSql.cap(5).length() > 0 ) {
		Q_ASSERT_X(rxSql.cap(6).length()>0,"parse",qPrintable(tr("La clausula SIGN esta incompleta: <%1>").arg(rxSql.cap(5))));
                _signclause = rxSql.cap(5);
//                qDebug("_signclause: %s", qPrintable(_signclause));
                QRegExp rx;
                rx.setPattern("\\s*WHERE.*");
                rx.setCaseSensitivity(Qt::CaseInsensitive);
                _signclause.remove(rx);
  //              qDebug("_signclause: %s", qPrintable(_signclause));
                rx.setPattern("\\s*JOIN(.*)");
		_signclause.remove(rx);
                _signclause = _signclause.trimmed();
    //            qDebug("_signclause: %s", qPrintable(_signclause));
                _signers =  rxSql.cap(7);
                rx.setPattern("\\s*WHERE\\s+.*");
                _signers.remove(rx);
                rx.setPattern("\\s*JOIN(.*)");
                _signers.remove(rx);
                _signers = _signers.trimmed();
//                qDebug("_signers: %s", qPrintable(_signers));
                QRegExp rxSign("\\s*(,)\\s*");
		rxSign.setCaseSensitivity(Qt::CaseInsensitive);
		_signclauses = QSet<QString>::fromList(_signers.split(rxSign));

//		qDebug("......._signclauses: %d", _signclauses.count());
	}
	_error = CORRECT;
        return true;
}
void SafetSQLParser::addFieldToList(const QString& f) {
    _fields.append(f);
}

QString SafetSQLParser::addField(const QString& f, addType t ) {
              Q_ASSERT_X(f.length() > 0, "addField", 
		qPrintable(tr("La longitud del campo es igual a cero (0)")));
              QRegExp rxSelect("^(\\s*SELECT)(\\b|\\s)+");
	      rxSelect.setCaseSensitivity( Qt::CaseInsensitive );
	      QString sqlmod = sql();
	      sqlmod.replace(rxSelect,"SELECT "+f+",");
	     setSql( sqlmod );
	     return sql();
		
}
QStringList SafetSQLParser::getFields() const {

		return _fields;
}

QSet<QString> SafetSQLParser::getWhereclauses() const {
		return _whereclauses;
}

QSet<QString> SafetSQLParser::getSignclauses() const {
		return _signclauses;
}

QSet<QString> SafetSQLParser::getJoinclauses() const {
	 	return _joinclauses;
}

QString SafetSQLParser::getTablesource() const {
	return _tablesource;
}


QString SafetSQLParser::dropWhereClauses(bool drop) {
	QString str = sql();
		QRegExp rx;
                rx.setPattern("\\s*WHERE\\s+([a-zA-Z0-9\\s_\\{\\}\\.\\=<>'\"\\(\\)\\!]+)\\s*");
        rx.setCaseSensitivity( Qt::CaseInsensitive );
	if ( getWhereclauses().count() > 0 ) str.replace(rx," ");

	if (drop) setSql( str );
	return str;
}

QString SafetSQLParser::getWhereClause() {
        QString str = sql();
                QRegExp rx;
        rx.setPattern("\\s*WHERE\\s+([a-zA-Z0-9\\s_\\{\\}#\\%\\.\\=<>'\"\\(\\)\\!]+)\\s*");
        rx.setCaseSensitivity( Qt::CaseInsensitive );
        int pos = str.indexOf(rx);
        if ( pos >= 0 ) {
            return rx.cap(0);
        }

        return QString("");
}


void SafetSQLParser::clean() {
	_fields.clear();
	_whereclauses.clear();
	_joinclauses.clear();
	_signclauses.clear();
	_tablesource.clear();

}
