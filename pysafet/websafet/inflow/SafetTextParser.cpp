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
// //
// C++ Implementation: SafetTextParser
//
// Description: 
//
//
// Author: VÃ­ctor R. Bravo, Antonio Araujo Brett <vbravo@cenditel.gob.ve,aaraujo@cenditel.gob.ve>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QRegExp>
#include "SafetTextParser.h"
#include "mainwindow.h"



SafetTextParser::SafetTextParser(QObject *parent)
 : SafetParser(parent) {
	domDocument = NULL;
	inputDomDocument = NULL;
        currentcommand = -1;
	_currid = "-1";
        _dommodel = NULL;
}


SafetTextParser::~SafetTextParser() {
    if (_dommodel != NULL ) {
        delete _dommodel;
    }
    if (domDocument != NULL ) {
        delete domDocument;
    }
    if (inputDomDocument != NULL ) {
        delete inputDomDocument;
    }


}

bool SafetTextParser::parse(const QString& s) {
    return true;
}

void SafetTextParser::setCommands(const QStringList& list, const QStringList& otherlist ) {
	int i = 0;
	foreach(QString str,list) {
		if (i  < otherlist.count()  ) 
			commands()[ str ] = otherlist.at( i++ );
		else 
			commands()[ str ] = "*";
	}
}

QString SafetTextParser::toXml(const QString& s) {

        qDebug("....SafetTextParser::toXml()....(1)....");
        if ( s.length() > 0 )  {
		_str.append(s);
	}



        QString textoxml = _str;
	
	QPair<QString,QString> myxml, pairnull;
	_xmlstring = "";
	QString newxml;
	countxml = 0;
	QStack<QString> mystack;

	while( true ) {
            myxml = nextXmlWord();
            if (myxml.first.trimmed().isEmpty()) {
                qDebug("...first is empty");
                break;
            }
            if (myxml == pairnull  ) {
                break;
            }
            if (myxml.first.right(1) == ":" ) {
                        myxml.first.chop(1);
            }
	    if ( myxml.first == tr("operacion") ) {	    
                        newxml = "<" + myxml.first + " parameter='" + myxml.second + "'>\n";
			mystack.push( myxml.first );			
			currcommand = myxml.second;
                        operationname =  myxml.second;
	     }
	     else {
            QString mysecond = myxml.second;
            SYD << tr("........SafetTexParser;;toXml....myxml.second:|%1|").arg(mysecond);
            if (mysecond.isEmpty()) {
                SYD << tr("........SafetTexParser;;toXml....myxml.second ISEMPTY break");
                break;
            }

            if (myxml.first.indexOf(tr("Contrasena")) >= 0 )  {
                if ( mysecond.startsWith("_") ) {
                    if ( mysecond.indexOf("_",1) == -1 ) {
                        mysecond = myxml.second.mid(1);
                    }

                }
                else {
                    mysecond  = QCryptographicHash::hash(QByteArray(qPrintable(myxml.second)),
                                                         QCryptographicHash::Sha1).toHex().data();
                }
                SYD << tr("........(2)...SafetTexParser;;toXml....mysecond:|%1|").arg(mysecond);
                newxml = "<" + myxml.first + ">\n " + mysecond + "\n</" + myxml.first + ">\n";
            }
            else {
                newxml = "<" + myxml.first + ">\n " + replaceXmlCharSpecial(mysecond)+ "\n</" + myxml.first + ">\n";
            }

	     }

	    _xmlstring += newxml;
	}

	while ( !mystack.isEmpty() ) {
		newxml = "</" + mystack.pop() + ">\n";
		_xmlstring += newxml;
	}
	return _xmlstring;
}

QPair<QString,QString> SafetTextParser::nextXmlWord() {
    QPair<QString,QString> result;
    QString first, second;
    QRegExp rx(tr("([a-zA-Z_0-9áéíóúñÑ¿\\?'\\*=\\.\\-\\(\\),;%#\\x3c\\x3e\\x2f\\[\\]/@\\|\\{\\}&]+"
                  "|[/a-zA-Z_0-9áéíóúñÑ'¿\\?=\\.\\-\\(\\),;%#\\x3c\\x3e\\x2f\\[\\]/@\\|\\{\\}&]+:)").toLatin1());

    int pos = countxml;

    pos = rx.indexIn(_str, pos);
    if ( pos == -1 ) {
        return result;
    }
    pos += rx.matchedLength();
    countxml = pos;
    first = rx.cap(1);

    if (first.right(1) != ":") {

        SYE
                <<
                tr("La palabra de entrada \"%1\" no es un nombre de campo valido").arg(first);
        return result;
    }

    int words = 0;
    int oldpos;
    bool haspoints = false, hasnextpoints = false;
    while (true) {
        oldpos = pos;
        pos = rx.indexIn(_str, pos);

        if ( pos == -1  ) {
            if (words > 0 ) {
                countxml = oldpos;
                break;
            }

            if ( first.right(1) != ":") {
                SafetYAWL::streamlog
                        << SafetLog::Error
                        << tr("Falta una palabra de entrada para el campo \"%1\"").arg(first);
                return result;
            }
        }
        oldpos = pos;
        pos += rx.matchedLength();
        countxml = pos;



        if ( rx.cap(1).right(1) == ":" ) {
            if ( checkIsField(rx.cap(1))) {
                    countxml = oldpos;
                    break;
                }
            else {
                haspoints = true;
            }

        }

        if (hasnextpoints ) {
            second =  second + "" + rx.cap(1).toLatin1();
            hasnextpoints = false;
        }
        else {
            second =  second + " " + rx.cap(1).toLatin1();
        }
	
        if (haspoints) {
            hasnextpoints = true;
            haspoints = false;
        }

        qDebug("...second: |%s|",qPrintable(rx.cap(1)));
        words++;
    }
    result.first = first.trimmed();
    result.second = second.trimmed();
    result.second.replace(QRegExp("(&crarr;|crarr|crarr;)"),"\\n");
    return result;
}


void SafetTextParser::openXml(const QString& f) {

        QString filepath, filename, fullpath;
        if (f.isEmpty() ) {
            filepath = SafetYAWL::getConfFile().getValue("Input", "input.path").toString().trimmed();
            filename = SafetYAWL::getConfFile().getValue("Input", "input.file").toString();

            if (filepath.isEmpty()) {
                filepath = QDir::homePath() + Safet::datadir + "/" + "input";
            }
            fullpath = filepath + "/" + filename;
        }
        else {
            fullpath = f;
        }

        if (!QFile::exists(fullpath)) {
            SYE << tr("El archivo \"%1\" para cargar las acciones de usuario no se puede leer Colocando VALOR POR DEFECTO");

                fullpath = "/home/panelapp/" + Safet::datadir + "/" + "input" + "/" + "defusers.xml";
        }

        SYA
                << tr("ABRIENDO el archivo para manipulacion de entrada: ->\"%1\"").arg(fullpath);

        QFile file(fullpath);
	if ( domDocument == NULL ) 
		domDocument = new QDomDocument("document");
	Q_CHECK_PTR( domDocument );

        if ( !file.open(QIODevice::ReadOnly) ) {
            SafetYAWL::streamlog
                    << SafetLog::Error
                    <<
                tr("No es posible leer  archivo: \"%1\" ").arg(filepath + "/" + filename);
            return;
        }
	 bool nameproccesing =true;
	 QString errorMsg;
	 int errorline, errorcol;
         bool result = domDocument->setContent(&file,nameproccesing,
                                               &errorMsg,
                                               &errorline,
                                               &errorcol);
         if (!result ) {
            SYE <<
             tr("\"%1\" presenta un error en la (linea,columna) (%2,%3)."
                "Mensaje de error: \"%4\"")
             .arg(filename)
             .arg(errorline)
             .arg(errorcol)
             .arg(errorMsg);
             return;
         }


 	file.close();
        _currdeffile = f;
        if ( _dommodel == NULL ) {
            SYD << tr("...Cargado SafetTextParser...openXML DomModel (1)...");
            _dommodel = new DomModel(_currdeffile,NULL);
        }
	docok = true;
}

QString SafetTextParser::replaceXmlCharSpecial(const QString& xml, bool in) {
    QString result = xml;

    QRegExp rx;
    int pos = 0;
//    QRegExp rxDate;
//    rxDate.setPattern("\\d{2}/\\d{2}/\\d{4}");

    if ( result.trimmed().count() == 10)  {

         QDateTime mydate;
         mydate = QDateTime::fromString(result,"dd/MM/yyyy");
         if (mydate.isValid()) {
            uint secs = mydate.toTime_t();
             SafetYAWL::streamlog
                    << SafetLog::Debug <<
                       tr("replaceXmlCharSpecial...date: |%2| toString():|%3| time_t: |%1|")
                       .arg(secs)
                       .arg(result)
                       .arg(mydate.toString("dd/MM/yyyy"));

            return QString("%1").arg(secs);
        }
    }
    if ( in ) { // Cadena XML Original
      rx.setPattern("</([a-zA-Z])+>");
      pos = rx.indexIn(result);
      if (pos >= 0 ) {
          result.replace("</"+rx.cap(1)+">","\\x3c\\x2f"+rx.cap(1)+"\\x3e");
      }
      rx.setPattern("<([a-zA-Z=#'0-9])+>");
      pos = rx.indexIn(result);
      if (pos >= 0 ) {
          result.replace("<"+rx.cap(1)+">","\\x3c"+rx.cap(1)+"\\x3e");
      }
    }
    else {
        result.replace("\\x3c\\x2f","</");
        result.replace("\\x3c","<");
        result.replace("\\x3e",">");

    }

     result = MainWindow::replaceMarks(result);
     return result;


}

void SafetTextParser::processInput(const QString& s) {

	Q_ASSERT( inputDomDocument == NULL );
	inputDomDocument = new QDomDocument("document");
	Q_CHECK_PTR( inputDomDocument );
	 bool nameproccesing =true;
	 QString errorMsg;
	 int errorline, errorcol;
         QString reformated = s;
         //qDebug("...reformated:\n%s",qPrintable(reformated));
         bool result = inputDomDocument->setContent(reformated,
                                                    nameproccesing,
                                                    &errorMsg,
                                                    &errorline, &errorcol);
         if (!result ) {

             SafetYAWL::streamlog
                     << SafetLog::Error
                     <<
                tr("El documento de entrada \"%1\" presenta un error En la linea %2 y columna %3 ocurrio el siguiente error: %4")
                .arg(s).arg(errorline).arg(errorcol).arg(errorMsg);
             return;
        }

        SafetYAWL::streamlog
                << SafetLog::Action
                << tr("La  entrada de Datos fue procesado correctamente...ok");
	docok = true;

}




QDomNode SafetTextParser::findOperation(const QString& name) {
	Q_CHECK_PTR( domDocument);  // Documento Abierto
        QDomElement docElem = domDocument->documentElement();
        QDomNode n = docElem.firstChild();
	while ( !n.isNull() ) {	
		QDomNamedNodeMap attrs = n.attributes();
                if ( attrs.namedItem("name").nodeValue() == name) {
                    break;
                }
		n = n.nextSibling();
	}	
	return n;

}

// buildFields --> INSERT INTO



QStringList SafetTextParser::loadNameConfs() {
     QStringList names;
     if ( SafetYAWL::listDynWidget == NULL ) return names;
     for(int i = 0; i< SafetYAWL::listDynWidget->count(); i++) {
          WidgetInterface* w = SafetYAWL::listDynWidget->at( i ) ;
          if ( w == NULL ) continue;
          QMap<QString,QVariant> c = w->conf();
          QString namew = c["namewidget"].toString();

          names.append(namew);
     }

     return names;
}


bool SafetTextParser::doAction(const QString& type, const QString& key, const QString& info) {

    if ( SafetYAWL::listDynWidget == NULL ) {
         return false;
     }
 QMap<QString,QVariant> newconf;
 for(int i = 0; i< SafetYAWL::listDynWidget->count(); i++) {
     WidgetInterface* w = SafetYAWL::listDynWidget->at( i ) ;
     if ( w->descriptor() == type ) { //here
         SafetYAWL::streamlog << SafetLog::Debug
                 << tr("Ejecutando en \"doAction\" la acción (action) para el widget: \"%1\"")
                 .arg(type);
         QMap<QString,QVariant> c = w->conf();
         QString namew = c["namewidget"].toString();
         newconf = _confs[ namew ];
         w->setConf( newconf);
         return w->doAction(key,info);
     }
  }
  return true;
}

QList<QPair<QString,QString> > SafetTextParser::getFieldsValues(const QDomElement& ecommand,
                                                                const QDomNode& ninput,
                                                                QMap<QString,
                                                                QString>& keysforeignmap) {

    QList<QPair<QString,QString> > fieldspairnull,listfieldspair;


    QDomNodeList fieldlist = ecommand.firstChild().childNodes();

    QMap<QString,int> mandatories;
    QMap<QString,int> labelsmandatories;
    QMap<QString,bool> repetibles;
    QMap<QString,QString> realvalues;
    QMultiMap<QString,QString> titles;
    QString theprimarykey;
    QString keyvalue;
    QString fieldkeysvalues;
    QString nameoperation = ninput.attributes().namedItem("parameter").nodeValue();

    SYD << tr("SafetTextParser::getFieldsValues....nameoperation: |%1|")
           .arg(nameoperation);
    QMap<QString,QString> valuemap;
    QMap<QString,QString> currmap;
    SafetYAWL::fieldsname = "";
    SafetYAWL::fieldsvalue = "";
     QStringList indexs;
    for(int i=0,j=fieldlist.count()-1; i < fieldlist.count(); i++,j--) {
        Q_ASSERT(fieldlist.at(i).isElement());
        QDomElement e = fieldlist.at(i).toElement();
        QString namefield = e.firstChild().nodeValue().trimmed();
        QDomNamedNodeMap attrs = e.attributes ();
        QDomNode titleNode = attrs.namedItem("title");
        QString currtitle = titleNode.nodeValue().simplified();

        QString titleString;
        if ( !titleNode.isNull() ) {
            titleString = titleNode.nodeValue().simplified();

            titles.insert(namefield, titleString);
        }

        if (attrs.namedItem("type").nodeValue().trimmed() == "comboflow" ) {

            if (MainWindow::_issigning) {
                continue;
            }
        }


        QDomNode typeNode = attrs.namedItem("type");
        SYD << tr("....SafetTextParser::buildFields...TYPENODE:|%1|")
               .arg(typeNode.nodeValue().simplified());
        SYD << tr("....SafetTextParser::buildFields...TYPENODE...namefield:|%1|")
               .arg(namefield);


        QDomNode inputAttr = attrs.namedItem("input");
        if (!inputAttr.isNull() ) {
            QString str = inputAttr.nodeValue().trimmed();
            QStringList mylist = str.split("::");
            if (mylist.count()==2 && mylist.at(0)=="cn"){
                QDomElement einput = ninput.firstChildElement(namefield);
                if (einput.isNull()) {
                    einput = ninput.firstChildElement(currtitle);
                }
                SYD << tr ("............getFieldsValues...MainWindow:: einput.isNull():|%1|")
                       .arg(einput.isNull());
                SYD << tr ("............getFieldsValues...MainWindow:: namefield:|%1|")
                       .arg(namefield);
                SYD << tr ("............getFieldsValues...MainWindow:: currtitle:|%1|")
                       .arg(currtitle);
                MainWindow::_currentCommonName = str + "::" + einput.firstChild().nodeValue().trimmed();

            }
            else if ( str != "yes" && str != "no") {

                SYD << tr("...getFieldsValues...MainWindow::_currentCommonName:|%1|")
                       .arg(MainWindow::_currentCommonName);

                SYD << tr("...mylist.count():%1").arg(mylist.count());
                if (mylist.count() != 3 ) {
                    continue;
                }
                QStringList currvalues = MainWindow::_currentCommonName.split("::");
                if (currvalues.count() != 3) {
                    continue;
                }
                QString mynumber = currvalues.at(1).trimmed();
                QString currnumber = mylist.at(1).trimmed();

                QString myvalue = currvalues.at(2);
                SYD << tr("......SafetTextParser::getFieldsValues...INPUT_MYVALUE: |%1|").arg(myvalue);
                SYD << tr("...SafetTextParser::getFieldsValues...INPUT_MYLIST(2): |%1|").arg(mylist.at(2));
                if (mylist.at(2) == myvalue) {

                }
                else {
                    continue;
                }

            }
        }


        QDomNode attrprimary = attrs.namedItem("primarykey");
        if (attrprimary.nodeValue().length() > 0 &&
            attrprimary.nodeValue().trimmed().compare("yes",Qt::CaseSensitive) == 0 ) {

            if ( attrs.namedItem("sequence").nodeValue().length() > 0 ) {
                keysforeignmap[currnametable + "." + e.firstChild().nodeValue().trimmed()] ="currval('" + attrs.namedItem("sequence").nodeValue()+ "')";

            }
            else {


                QString infieldname;
                if (!titleString.isEmpty()) {
                    infieldname = titleString;
                }
                else {
                    infieldname = e.firstChild().nodeValue().trimmed();
                }

                SYD << tr("SafetTextParser getFieldsValues:...infieldname...:|%1|")
                       .arg(infieldname);
                QDomElement einput = ninput.firstChildElement(infieldname);
               if ( einput.isNull() ) {		

                           SYW << tr("....Entrada (Clave Primaria) no se encuentra en los "
                                 "datos,para el campo (field):"
                                 "\"%1\"").arg(infieldname);
                   for(int k=0; k < ninput.childNodes().count();k++){
                       QDomNode node = ninput.childNodes().at(k);

                               SYD << tr("SafetTextParser...Nombre Nodo: |%1|")
                                  .arg(node.nodeName().trimmed());
                   }
                   //return fieldspairnull;
		}
                keyvalue = einput.firstChild().nodeValue().trimmed();
                SafetYAWL::combovarglobal0 = einput.firstChild().nodeValue().trimmed();
                keysforeignmap[currnametable + "." + e.firstChild().nodeValue().trimmed()] = einput.firstChild().nodeValue().trimmed();
                        SYD << tr("**SafetTextParser getFieldsValues: keyvalue: |%1|").arg(keyvalue);
            }
            theprimarykey =  e.firstChild().nodeValue().trimmed();


        }

        if (attrs.namedItem("changekey").nodeValue().length() > 0 ) {
            QString infieldname;
            if (!titleString.isEmpty()) {
                infieldname = titleString;                
            }
            else {
                infieldname = e.firstChild().nodeValue().trimmed();
            }

            QDomElement einput = ninput.firstChildElement(infieldname);
            SYD << tr("SafetTextParser::getFieldsValues...changekey...infieldname:|%1|")
                   .arg(namefield);
            if (!einput.isNull() ) {
                QString key = einput.firstChild().nodeValue().trimmed();
                QString newfield = infieldname+":"+key;
                SYD << tr(".........SafetTextParser::getFieldsValues..EXTRACTKEY_NEWFIELD...newfield:|%1|")
                       .arg(newfield);
                if (!fieldkeysvalues.isEmpty()) {
                    fieldkeysvalues += ",";
                }
                fieldkeysvalues += newfield;
                SYD << tr("SafetTextParser::getFieldsValues....fieldkeysvalues:|%1|")
                       .arg(fieldkeysvalues);
            }


        }

        if (  !attrs.namedItem("repetible").isNull() )  {
            repetibles[ namefield ] = (attrs.namedItem("repetible").nodeValue().trimmed().compare("yes", Qt::CaseInsensitive) == 0 );
        }
        if (  !attrs.namedItem("index").isNull() )  {

            bool ok;
            if (!indexs.contains(namefield)) {
                indexs.insert(attrs.namedItem("index").nodeValue().trimmed().toInt(&ok),
                              namefield);
            }
        }
        else {
            if (!indexs.contains(namefield)) {
                indexs.push_back(namefield);
            }
        }
        if ( !attrs.namedItem("mandatory").isNull() ) {
            QString mymandatory = attrs.namedItem("mandatory").nodeValue().trimmed();
            if ( mymandatory.indexOf(QRegExp("yes|no")) == -1 ) {
                if ( !labelsmandatories.contains(mymandatory)) {
                    labelsmandatories [ mymandatory ] = labelsmandatories.count()+2;
                }

                int classnumber = labelsmandatories [ mymandatory ];
                mandatories[ namefield ]  = classnumber;
            }
            else {
                mandatories[ namefield ] = (mymandatory.compare("yes", Qt::CaseInsensitive) == 0 );
            }
        }

        if ( !attrs.namedItem("options").isNull() ) {
            QString options = attrs.namedItem("options").nodeValue().trimmed();
	    options.replace("_USERNAME", SafetYAWL::currentAuthUser());
        SYD << tr("...replacing options: (USERNAME)...|%1|")
               .arg(SafetYAWL::currentAuthUser());

            if (options.indexOf("type::radio") == -1 && options.indexOf("type::checkbox") == -1) {
                        realvalues[ namefield ] = options;
            }
        }
        if ( ninput.firstChildElement(namefield).isNull() ) {
            if (ninput.firstChildElement(titleNode.nodeValue().trimmed()).isNull()) {
                currmap[namefield ] = "null";
            }
            else {
                currmap[namefield ] = ninput.firstChildElement(titleNode.nodeValue().trimmed()).firstChild().nodeValue().trimmed();
            }
        }
        else {
           currmap[namefield ] = ninput.firstChildElement(namefield).firstChild().nodeValue().trimmed();
        }




        if ( attrs.namedItem("default").nodeValue().length() == 0) {
            valuemap[e.firstChild().nodeValue().trimmed()] = "";
            if ( attrs.namedItem("function").nodeValue().length() > 0 ) {
                QString format = attrs.namedItem("format").nodeValue().simplified();
                if ( format.length() == 0 ) format = "default";
                QString func = attrs.namedItem("function").nodeValue().simplified();
                QString value = getValueForFunction( func,format,currmap,keyvalue);
                valuemap[namefield] = value;




            } else if ( attrs.namedItem("literal").nodeValue().length() > 0 ) {
                QString literal = attrs.namedItem("literal").nodeValue();
                if ( literal.startsWith("_") ) {
                    QString s = SafetYAWL::searchLiteral( literal );
                    valuemap[namefield] = s;
                }
                else {
                    valuemap[namefield] = "'"+literal+"'";
                }
            } else if ( attrs.namedItem("sequence").nodeValue().length() > 0 ) {
                QString typesequence = attrs.namedItem("typesequence").nodeValue();

                if (typesequence == "current") {
                    valuemap[e.firstChild().nodeValue().trimmed()] = "currval('" +attrs.namedItem("sequence").nodeValue()+ "')";
                }
                else {
                    valuemap[e.firstChild().nodeValue().trimmed()] = "nextval('" +attrs.namedItem("sequence").nodeValue()+ "')";
                }


            } else if ( attrs.namedItem("foreignkey").nodeValue().length() > 0 ) {
                valuemap[e.firstChild().nodeValue().trimmed()] = keysforeignmap[attrs.namedItem("foreignkey").nodeValue()];
            }
        }

        if (typeNode.nodeValue().simplified() == "number") {
            SYD << tr("....SafetTextParser::buildFields...***TYPENODE:|NUMBER|");
                QDomElement einput = ninput.firstChildElement(namefield);
                if (einput.isNull()) {
                    einput = ninput.firstChildElement(currtitle);
                }
                if (!einput.isNull()) {

                    SYD << tr("....SafetTextParser::buildFields...checkprice...TYPENODE...(1)...");
                    QString newnumber = einput.firstChild().nodeValue().trimmed();
                    newnumber.replace("Bs.","");
                    newnumber.replace(".","");
                    //newnumber.replace(",",".");
                    if ((namefield.compare("Precio",Qt::CaseInsensitive) == 0 || namefield.compare("price",Qt::CaseInsensitive) == 0)
                            && newnumber.indexOf(",")==-1) {
                        SYD << tr("....SafetTextParser::buildFields...checkprice...TYPENODE...(2)...");
                            newnumber =   newnumber + ",00";
                    }
                    newnumber = QString("'%1'").arg(newnumber);



                    SYD << tr("....SafetTextParser::buildFields.TYPENODE namefield:|%1|..\n****value(newnumber):|%2|")
                           .arg(namefield)
                           .arg(newnumber);
                    valuemap[namefield] = newnumber;
                }
                //continue;

        }



        QString currvalidation = attrs.namedItem("validation").nodeValue();
        SYD << tr("..:SafetTextParser::getFieldsValues..CURRVALIDATION.currvalidation:|%1|")
               .arg(currvalidation);
        if ( currvalidation.startsWith("validatefunction:")) {
            QString format = attrs.namedItem("format").nodeValue().simplified();
            if ( format.length() == 0 ) format = "default";
            QString func = currvalidation.mid(QString("validatefunction:").length());
            QString value = getValueForFunction( func,format,currmap,keyvalue);
            SYD << tr("......:SafetTextParser::getFieldsValues..CURRVALIDATION...value:|%1|")
                   .arg(value);
            QDomElement einput = ninput.firstChildElement(namefield);
            if (einput.isNull()) {
                einput = ninput.firstChildElement(currtitle);
            }
            if (!einput.isNull()) {

                QString newvalue = einput.firstChild().nodeValue().trimmed();

                if (value == "false") {
                    valuemap[namefield] = newvalue +"::safetvalidatefalse";
                }
                else if (value == "true") {
                    valuemap[namefield] = newvalue +"::safetvalidatetrue";
                }
            }


        }


    }

    bool nomandatoryfieldnullcontinue; // Para evaluar cuando el campo es nulo

    QString usertip;
    // Evaluando valuemaps...
    int i = 0;


    for ( int inccount = 0; inccount < valuemap.keys().count(); inccount++ ) {

        i = valuemap.keys().indexOf(indexs.at(inccount));
        qDebug("      valuemap.keys().at(inccount):%s",qPrintable(valuemap.keys().at(inccount)));
        qDebug("      ...incount..i:%d",i);
        if (i == -1 ) {
            qDebug("ERROR ERROR ERROR (getFieldsValues)");
            return fieldspairnull;
        }

        QPair<QString, QString> mypair;
        QString newfieldname;
        nomandatoryfieldnullcontinue = false;

        mypair.first =  valuemap.keys().at(i);
        newfieldname = valuemap.keys().at(i);

        QString containvalidatefunction = "";

        if ( valuemap[ valuemap.keys().at(i) ].length() == 0 ) {
            QString newfieldname = valuemap.keys().at(i);
            QDomElement einput = ninput.firstChildElement(valuemap.keys().at(i));


            QString infotitles;
            if (titles.contains(newfieldname)) {
                for(int j=0; j< titles.values(newfieldname).count();j++) {
                    einput = ninput.firstChildElement(titles.values(newfieldname)
                                                      .at(j));
                    infotitles += titles.values(newfieldname)
                                  .at(j) + " o ";
                    if (!einput.isNull() ) {
                        break;
                    }

                }

                infotitles.chop(3);
            }else{
                einput = ninput.firstChildElement(newfieldname);
            }

            bool ismandatory = false;

            if ( mandatories.contains(mypair.first) )  {

                if ( mandatories[mypair.first] > 1 ) {
                    QStringList classfields;
                    int findclass = mandatories[mypair.first];

                    for( int j=0; j < mandatories.keys().count();j++) {
                        if (mandatories[mandatories.keys().at(j)] == findclass) {
                            classfields.append(mandatories.keys().at(j));
                        }
                    }
                    qDebug("   ****.......checkMandatoryClass(classfields,ninput)......");
                    ismandatory = checkMandatoryClass(classfields,ninput);

                }
                else {
                    ismandatory = mandatories[mypair.first];
                }
            }


            if (einput.isNull() ) {

                if ( ismandatory ) {
                    QString leftfield = newfieldname;
                    if ( titles.contains(newfieldname)) {
                        leftfield = infotitles;
                    }

                            SYE << tr(
                                  "No ha ingresado el campo "
                                  "\"%1\" (es obligatorio para procesar la acción)").arg(leftfield);
                    return fieldspairnull;
                }
                continue;
            }

            QString newvalue;
            if (realvalues.contains(mypair.first)) {

                QString curvalue = einput.firstChild().nodeValue().trimmed();
                for (int j = 0; j < realvalues[mypair.first].split(",").count();j++) {
                    QString value = realvalues[mypair.first].split(",").at(j);
                    if ( value.split("::").at(0).compare(curvalue) == 0 ) {
                        if ( value.split("::").count() > 2 ) {
                            curvalue = value.split("::").at(2);
                        }
                        else {
                            curvalue = value.split("::").at(0);
                        }
                        break;
                    }
                }

                newvalue = curvalue;

            }else{

                newvalue = einput.firstChild().nodeValue().trimmed();
            }

            mypair.second = newvalue;
        }
        else {



            mypair.second = valuemap[ valuemap.keys().at(i) ].trimmed();



            if (mypair.second.endsWith("::safetvalidatefalse")) {
               mypair.second.replace("::safetvalidatefalse","");
               containvalidatefunction = "::safetvalidatefalse";

            }
            else if  (mypair.second.endsWith("::safetvalidatetrue")) {
                containvalidatefunction = "::safetvalidatetrue";
                 mypair.second.replace("::safetvalidatetrue","");
            }

            QRegExp rx;
            rx.setPattern("(\\.png|\\.jpg|\\.gif|\\.ogg|\\.mov|\\.mp3)$");

            if ( mypair.second.contains(rx)  && QFile::exists( mypair.second ) ) {
                mypair.second = "lo_import('"+mypair.second+"')";
            }
        }
        SafetYAWL::fieldsname = SafetYAWL::fieldsname +mypair.first +",";
        bool addquotes =  !(SafetYAWL::canTypeConvert(mypair.second, QVariant::Bool)
                            || SafetYAWL::canTypeConvert(mypair.second, QVariant::Int));

        if ( addquotes) {
            SafetYAWL::fieldsvalue = SafetYAWL::fieldsvalue +"'"+mypair.second+"',";
        }
        else {
            SafetYAWL::fieldsvalue = SafetYAWL::fieldsvalue +mypair.second+",";
        }



        if (  mandatories.contains(valuemap.keys().at(i)) ) {
            QString namefield = valuemap.keys().at(i);
            QString value = mypair.second.replace("'","");                                        
            bool resultDoValidation;
            QString origValue = value;
            if (containvalidatefunction.endsWith("false")) {

                QString namefield = valuemap.keys().at(i);
                QString value = mypair.second.replace("'","");

                usertip = "n/a";
                SYE << tr("Error de validación: No es válido el valor "
                          "\"%2\" del campo \"%1\" :\"%3\"")
                       .arg(namefield)
                       .arg(value)
                       .arg(usertip);

                return fieldspairnull;
            }
            else if (containvalidatefunction.endsWith("true")) {

            }
            else {
                if ( titles.contains(valuemap.keys().at(i))) {
                    for(int j = 0; j < titles.values(valuemap.keys().at(i)).count();j++) {
                        namefield = titles.values(valuemap.keys().at(i)).at(j);
                        resultDoValidation = doFieldValidation(nameoperation, namefield,value,usertip,
                                                               fieldkeysvalues);
                        if (resultDoValidation) {
                            break;
                        }
                    }
                }
                else {
                    resultDoValidation = doFieldValidation(nameoperation, namefield,value,usertip,
                                                           fieldkeysvalues);
                }

                if (!resultDoValidation  && currentCommand()== 0) {

                    if (usertip.isEmpty()) {
                                SYE << tr("Error de validación: No es válido el valor "
                                      "\"%2\" del campo \"%1\"")
                                .arg(namefield)
                                .arg(value);
                    }
                    else {

                                SYE << tr("Error de validación: No es válido el valor "
                                      "\"%2\" del campo \"%1\" :\"%3\"")
                                .arg(namefield)
                                .arg(value)
                                .arg(usertip);
                    }
                    return fieldspairnull;

                }
                else {
                    if ( origValue != value ) {
                        int lastpos = mypair.second.lastIndexOf(origValue);
                        if ( lastpos < 0 ) {
                            // Error
                            return fieldspairnull;
                        }
                        mypair.second.replace(lastpos, origValue.length(),value);
                    }
                }

            }
        }
        if ( mypair.first == theprimarykey ) {
            listfieldspair.push_front(mypair);
        }
        else {

            listfieldspair.push_back(mypair);
        }


    }
    SafetYAWL::fieldsname.chop(1);
    SafetYAWL::fieldsvalue.chop(1);
    return listfieldspair;
}

bool SafetTextParser::checkMandatoryClass(const QStringList& fields, const QDomNode& ninput,
                                          SafetTextParser::MandatoryClass t) {
    bool result = false;
    if ( t == OneOfAll ) {
        QDomNode n = ninput.firstChild();
        int mustone = 0;
        while (!n.isNull()) {
            if (n.isElement()) {
                QDomElement e = n.toElement();
                qDebug("Element name: %s",qPrintable(e.tagName()));
                if (fields.contains(e.tagName()) ) {
                    mustone++;
                }
            }
            n = n.nextSibling();
        }
        qDebug("......mustone: %d", mustone);
        if ( mustone != 1) {
            return true;
        }
    }
    else {

    }

    return result;
}

QPair<QString,QString> SafetTextParser::buildFields(const QDomElement& ecommand,
                                                    const QDomNode& ninput,
	QMap<QString,QString>& keysforeignmap) {
        qDebug("  .......** entrando a buildFields **..........");
        fieldspair.first = "";
        fieldspair.second = "";
        QPair<QString,QString> fieldsnullpair;
	QDomNodeList fieldlist = ecommand.firstChild().childNodes();	
    QString nameoperation = ninput.attributes().namedItem("parameter").nodeValue();
        QMap<QString,int> mandatories;
        QMap<QString,int> labelsmandatories;
        QMap<QString,bool> repetibles;
	QMap<QString,QString> realvalues;
        QMultiMap<QString,QString> titles;

         QString key; // Para acciones externas en los widgets

         QString keyvalue;
         QString fieldkeysvalues;

         QStringList indexs;
        QMap<QString,QString> valuemap;
        QMap<QString,QString> currmap;

        SafetYAWL::fieldtypes.clear();
        for(int i=0,j=fieldlist.count()-1; i < fieldlist.count(); i++,j--) {

            Q_ASSERT(fieldlist.at(i).isElement());
            QDomElement e = fieldlist.at(i).toElement();
            QDomNamedNodeMap attrs = e.attributes ();
            QString namefield = e.firstChild().nodeValue().trimmed();
            QDomNode titleNode = attrs.namedItem("title");
            QDomNode typeNode = attrs.namedItem("type");
            QString currtitle = titleNode.nodeValue().simplified();
            if ( !titleNode.isNull() ) {

                titles.insert(namefield, currtitle);
            }
            if ( !typeNode.isNull() ) {
                SafetYAWL::fieldtypes.insert(namefield, typeNode.nodeValue().simplified());
            }
            QDomNode inputAttr = attrs.namedItem("input");

            if (!inputAttr.isNull() ) {
                QString str = inputAttr.nodeValue().trimmed();
                QStringList mylist = str.split("::");
                if (mylist.count()==2 && mylist.at(0)=="cn"){
                    QDomElement einput = ninput.firstChildElement(namefield);
                    if (einput.isNull()) {
                        einput = ninput.firstChildElement(currtitle);
                    }
                    SYD << tr ("............buildFields...MainWindow:: einput.isNull():|%1|")
                           .arg(einput.isNull());
                    SYD << tr ("............buildFields...MainWindow:: namefield:|%1|")
                           .arg(namefield);
                    SYD << tr ("............buildFields...MainWindow:: currtitle:|%1|")
                           .arg(currtitle);
                    MainWindow::_currentCommonName = str + "::" + einput.firstChild().nodeValue().trimmed();
                    SYD << tr("..............mylist.count()==2....buildFields..."
                              "MainWindow::_currentCommonName:|%1|")
                           .arg(MainWindow::_currentCommonName);

                }
                else if ( str != "yes" && str != "no") {

                    SYD << tr("...buildFields...MainWindow::_currentCommonName:|%1|")
                           .arg(MainWindow::_currentCommonName);

                    SYD << tr("...mylist.count():%1").arg(mylist.count());
                    if (mylist.count() != 3 ) {
                        continue;
                    }
                    QStringList currvalues = MainWindow::_currentCommonName.split("::");
                    if (currvalues.count() != 3) {
                        continue;
                    }
                    QString mynumber = currvalues.at(1).trimmed();
                    QString currnumber = mylist.at(1).trimmed();

                    QString myvalue = currvalues.at(2);
                    SYD << tr("......SafetTextParser::buildFields...mynumber: |%1|").arg(mynumber);
                    SYD << tr("...SafetTextParser::buildFields...currnumber: |%1|").arg(currnumber);
                    if (mylist.at(2) == myvalue) {
                        qDebug("...mylist.at(2):%s",
                               qPrintable(mylist.at(2)));
                        qDebug("...myvalue:%s",
                               qPrintable(myvalue));

                    }
                    else {
                        continue;
                    }

                }
            }


            if (attrs.namedItem("primarykey").nodeValue().length() > 0 ) {
                if ( attrs.namedItem("sequence").nodeValue().length() > 0 ) {
                    keysforeignmap[currnametable + "." + namefield] ="currval('" + attrs.namedItem("sequence").nodeValue()+ "')";
                }
                else {
                    qDebug("...namefield..:%s",qPrintable(namefield));
                    QDomElement einput = ninput.firstChildElement(namefield);
                    if (einput.isNull() ) {
                        SafetYAWL::streamlog
                                << SafetLog::Debug
                                << tr("Una entrada es vacia en este punto");
                    }


                    keysforeignmap[currnametable + "." + namefield] = einput.firstChild().nodeValue().trimmed();
                    key = einput.firstChild().nodeValue().trimmed();

                    keyvalue = einput.firstChild().nodeValue().trimmed();
                    SafetYAWL::streamlog
                            << SafetLog::Debug
                            << tr("SafetTextParser buildFields: keyvalue: |%1|")
                               .arg(keyvalue);

                }

            }
            if (attrs.namedItem("changekey").nodeValue().length() > 0 ) {
                QDomElement einput = ninput.firstChildElement(namefield);
                if (!einput.isNull() ) {
                    QString key = einput.firstChild().nodeValue().trimmed();
                    QString newfield = titleNode.nodeValue().simplified()+";"+key;
                    if (!fieldkeysvalues.isEmpty()) {
                        fieldkeysvalues += ",";
                    }
                    fieldkeysvalues += newfield;
                    SYD << tr("SafetTextParser::buildFields....**(CURR**)fieldkeysvalues:|%1|")
                           .arg(fieldkeysvalues);

//                    if (fieldkeysvalues.startsWith("Cargar_archivo_flujo;")) {
//                        fieldkeysvalues = fieldkeysvalues.mid(QString("Cargar_archivo_flujo;").length());
//                    }
//                    SYD << tr("SafetTextParser::buildFields....**(CURR**) (2)fieldkeysvalues:|%1|")
//                           .arg(fieldkeysvalues);

                }


            }

            if (  !attrs.namedItem("repetible").isNull() )  {
                repetibles[ namefield ] = (attrs.namedItem("repetible").nodeValue().trimmed().compare("yes", Qt::CaseInsensitive) == 0 );
            }
            if (  !attrs.namedItem("index").isNull() )  {

                bool ok;
                if (!indexs.contains(namefield)) {
                    indexs.insert(attrs.namedItem("index").nodeValue().trimmed().toInt(&ok),
                                  namefield);
                }
            }
            else {
                if (!indexs.contains(namefield)) {
                    indexs.push_back(namefield);
                }
            }


            if ( !attrs.namedItem("mandatory").isNull() ) {
                QString mymandatory = attrs.namedItem("mandatory").nodeValue().trimmed();
                qDebug("........++mymandatory=%s",qPrintable(mymandatory));
                if ( mymandatory.indexOf(QRegExp("yes|no")) == -1 ) {
                    qDebug("........yes|no igual a -1");
                    if ( !labelsmandatories.contains(mymandatory)) {
                        labelsmandatories [ mymandatory ] = labelsmandatories.keys().count()+2;
                    }

                    int classnumber = labelsmandatories [ mymandatory ];
                    qDebug("     ......classNumber.........:%d",classnumber);
                    mandatories[ namefield ]  = classnumber;
                    qDebug("     ......namefield.........:%s",qPrintable(namefield));
                }
                else {
                    mandatories[ namefield ] = (mymandatory.compare("yes", Qt::CaseInsensitive) == 0 );
                }
            }
            if ( !attrs.namedItem("options").isNull() ) {
                QString options = attrs.namedItem("options").nodeValue().trimmed();
                options.replace("_USERNAME", SafetYAWL::currentAuthUser());
                SYD << tr("...replacing optionsi(2): (USERNAME)...|%1|")
                       .arg(SafetYAWL::currentAuthUser());

                if (options.indexOf("type::radio") == -1 && options.indexOf("type::checkbox") == -1) {
                        realvalues[ namefield ] = options;
                }
                qDebug("...realvalues->namefield: |%s|",
                       qPrintable(namefield));
                qDebug("...realvalues->options: |%s|",
                       qPrintable(options));

            }


            if ( ninput.firstChildElement(namefield).isNull() ) {
                if (ninput.firstChildElement(titleNode.nodeValue().trimmed()).isNull()) {
                    currmap[namefield ] = "null";
                }
                else {
                    currmap[namefield ] = ninput.firstChildElement(titleNode.nodeValue().trimmed()).firstChild().nodeValue().trimmed();
                }
            }
            else {
               currmap[namefield ] = ninput.firstChildElement(namefield).firstChild().nodeValue().trimmed();
            }

            if ( attrs.namedItem("default").nodeValue().length() == 0) {


                valuemap[namefield] = "";
                if ( attrs.namedItem("function").nodeValue().length() > 0 ) {
                    QString format = attrs.namedItem("format").nodeValue().simplified();
                    if ( format.length() == 0 ) format = "default";
                    QString func = attrs.namedItem("function").nodeValue().simplified();
                    qDebug("          buildFields*****....format: %s",qPrintable(format));
                    QString value = getValueForFunction( func,format,currmap, keyvalue);
                    valuemap[namefield] = value;
                }
                else if ( attrs.namedItem("literal").nodeValue().length() > 0 ) {

                    QString literal = attrs.namedItem("literal").nodeValue();
                    if ( literal.startsWith("_") ) {
                        QString s = "'"+ SafetYAWL::searchLiteral( literal ) + "'";
                        valuemap[namefield] = s;
                    }
                    else {
                        valuemap[namefield] = "'"+literal+"'";
                    }
                }
                else if ( attrs.namedItem("sequence").nodeValue().length() > 0 ) {
                    QString typesequence = attrs.namedItem("typesequence").nodeValue();

                    if (typesequence == "current") {
                        valuemap[namefield]= "currval('" +attrs.namedItem("sequence").nodeValue()+ "')";
                    }
                    else {
                        valuemap[namefield]= "nextval('" +attrs.namedItem("sequence").nodeValue()+ "')";
                    }
                    if ( key == namefield ) {
                        key = attrs.namedItem("sequence").nodeValue();
                    }
                    if (attrs.namedItem("primarykey").nodeValue().length() > 0 ) {
                        key = getValueOfKey(SafetTextParser::Sequence,attrs.namedItem("sequence").nodeValue());
                        qDebug("key:...|%s|",qPrintable(key));
                    }

                } else if ( attrs.namedItem("foreignkey").nodeValue().length() > 0 ) {
                    valuemap[namefield] = keysforeignmap[attrs.namedItem("foreignkey").nodeValue()];

                }
            }


        }
        //QString fieldsname, fieldsvalue;
        SafetYAWL::fieldsname = "";
        SafetYAWL::fieldsvalue = "";



        bool nomandatoryfieldnullcontinue; // Para evaluar cuando el campo es nulo
        int i = 0;
        for ( int inccount = 0; inccount < valuemap.keys().count(); inccount++ ) {
             i = valuemap.keys().indexOf(indexs.at(inccount));
             //i = indexs.indexOf(valuemap.keys().at(inccount));
              qDebug("      valuemap.keys().at(inccount):%s",qPrintable(valuemap.keys().at(inccount)));
              qDebug("      ...incount..i:%d",i);
              if (i == -1 ) {
                  qDebug("ERROR ERROR ERROR");
                  return fieldsnullpair;
              }
            qDebug("...valuemap.keys().at(i):|%s|",qPrintable(valuemap.keys().at(i)));
            QDomElement einput;
            QString newfieldname, newvalue;
            nomandatoryfieldnullcontinue = false;
            QString usertip; // Tool de validacion
            while( true ) {
                 QPair<QString, QString> mypair;
                mypair.first = valuemap.keys().at(i);
                newfieldname = valuemap.keys().at(i);


                qDebug();
                qDebug("...mypair.first:|%s|",qPrintable(mypair.first));


                if ( valuemap[ valuemap.keys().at(i) ].length() == 0 ) {


                    qDebug("...valuemap[ valuemap.keys().at(i) ].length() == 0");
                    if ( einput.isNull() ) {

                        qDebug("...einput.isNull():true....newfieldname:%s",
                               qPrintable(newfieldname));
                        QString infotitles;
                        if (titles.contains(newfieldname)) {



                            for(int j=0; j< titles.values(newfieldname).count();j++) {
                                einput = ninput.firstChildElement(titles.values(newfieldname)
                                                                  .at(j));
                                infotitles += titles.values(newfieldname)
                                              .at(j) + " o ";
                                if (!einput.isNull() ) {
                                    break;
                                }

                            }

                            infotitles.chop(3);


                        }
                        else{
                            einput = ninput.firstChildElement(newfieldname);
                        }



                        qDebug("....trying einput...(1)...");

                        if ( einput.isNull()) {

                            qDebug("...einput.isNull():true...(2)...");
                            bool ismandatory = false;

                            int fieldnamecount = 0;
                            qDebug("             mypair.first....:%s",qPrintable(mypair.first) );
                            qDebug("             newfieldname....:%s",qPrintable(newfieldname) );
                            qDebug("             fieldnamecount....:%d",fieldnamecount );
                            if ( fieldnamecount == 0 &&  mandatories[mypair.first] == 1 ) {

                                ismandatory = true;
                            }
                            else  {
                                if ( mandatories[mypair.first] > 1 ) {
                                    ismandatory = false;
                                }
                            }
                            if ( ismandatory ) {
                                QString leftfield = newfieldname;
                                if ( titles.contains(newfieldname)) {
                                    leftfield = infotitles;
                                }

                                    SafetYAWL::streamlog
                                            << SafetLog::Error
                                            << tr("ERROR DE VALIDACIÓN:"
                                                  "No ha ingresado el valor de "
                                                  "\"%1\" (obligatorio)").arg(leftfield);


                                return fieldsnullpair;
                            }

                            break;

                        }
                        else {
                           qDebug("....trying einput...(1)...");
                           newvalue = einput.firstChild().nodeValue().trimmed();
                           qDebug("...realvalues():true...(2)...");

                           if (realvalues.contains(mypair.first)) {
                               qDebug("realvalues...true");
                               QString curvalue = einput.firstChild().nodeValue().trimmed();
                               for (int j = 0; j < realvalues[mypair.first].split(",").count();j++) {
                                   QString value = realvalues[mypair.first].split(",").at(j);
                                   if ( value.split("::").at(0).compare(curvalue) == 0 ) {
                                       if ( value.split("::").count() > 2 ) {
                                           curvalue = value.split("::").at(2);
                                       }
                                       else {
                                           curvalue = value.split("::").at(0);
                                       }
                                       break;
                                   }
                               }

                               newvalue = curvalue;
                           }

                           newvalue.replace("'","\\'");

                           qDebug("...realvalues():true...(3)...");

                           if (newvalue.contains("\\n") ) {
                               newvalue = "E'"+newvalue+"'";
                           } else {
                               newvalue = "'"+newvalue+"'";
                           }
                           if (!checkIsActionsField(newfieldname,key, newvalue,SafetYAWL::fieldtypes) ) {

                               SafetYAWL::fieldsname = SafetYAWL::fieldsname + newfieldname + ",";
                               SafetYAWL::fieldsvalue = SafetYAWL::fieldsvalue + newvalue +  ",";
                           }
                           break;

                        }


                    } else {
                        if (titles.contains(newfieldname)) {
                            einput = einput.nextSiblingElement(titles.values(newfieldname).at(0));
                        }else{
                            einput = einput.nextSiblingElement(newfieldname);
                        }
                    }

                    qDebug("...realvalues.contains(mypair.first):|%s|",
                           qPrintable(mypair.first));

                    newvalue = einput.firstChild().nodeValue().trimmed().replace("'","\\'");


                    if (newvalue.contains("\\n") ) {
                        newvalue = "E'"+newvalue+"'";
                    } else {
                        newvalue = "'"+newvalue+"'";
                    }
                    mypair.second = newvalue;

		}
		else {
                    qDebug("......(4)...");

                        newvalue = valuemap[ valuemap.keys().at(i) ];
		}

                if (! checkIsActionsField(newfieldname,key, newvalue, SafetYAWL::fieldtypes)) {

                    SafetYAWL::fieldsname = SafetYAWL::fieldsname + newfieldname + ",";
                    SafetYAWL::fieldsvalue = SafetYAWL::fieldsvalue + newvalue+ ",";
                }
                if ( !repetibles.contains(mypair.first ) ) {
                    break;
                }
            } // Fin del while (true)

            // *** Aqui incluir validación
            // ********************************



            QMap<int,QStringList> findclasseds;

            for( int i=0; i < mandatories.keys().count();i++) {
                  if ( mandatories[mandatories.keys().at(i)] > 1 ) {
                    findclasseds [ mandatories[mandatories.keys().at(i)] ]
                            .append(mandatories.keys().at(i));

                }
            }
            for(int i=0; i <findclasseds.keys().count();i++) {
                if ( checkMandatoryClass(findclasseds[findclasseds.keys().at(i)],ninput) ) {
                    QString listfields;
                    foreach(QString s, findclasseds[findclasseds.keys().at(i)]) {
                        listfields += s;
                        listfields += ", ";
                    }
                    listfields.chop(2);

                      SafetYAWL::streamlog
                           << SafetLog::Error
                            << tr("ERROR DE VALIDACIÓN (Campos exclusivo):"
                                  "No ha ingresado un único (1) campo de esta "
                                  "lista: <b>%1</b> (obligatorio)")
                            .arg(listfields);
                    return fieldsnullpair;
                }
            }


            // Chequear Obligatoriedad Especial (OneOfAll)
            // ***************************************************

            if (  mandatories.contains(valuemap.keys().at(i))
                && mandatories[valuemap.keys().at(i)] == true) {

                QString namefield = valuemap.keys().at(i);
                QString value = newvalue.replace("'","");
                bool resultDoValidation ;
                QString origValue = value;
                if ( titles.contains(valuemap.keys().at(i))) {
                    for(int j = 0; j < titles.values(valuemap.keys().at(i)).count();j++) {
                        namefield = titles.values(valuemap.keys().at(i)).at(j);
                        resultDoValidation = doFieldValidation(nameoperation, namefield,value,
                                                               usertip,fieldkeysvalues);
                        if (resultDoValidation) {
                            break;
                        }
                    }
                }
                else {
                    resultDoValidation = doFieldValidation(nameoperation, namefield,value,usertip,
                                                           fieldkeysvalues);
                }


                if (!resultDoValidation && currentCommand()== 0)  {

                    if (usertip.isEmpty()) {
                            SYE <<  tr("NO es válido el valor "
                                  "<b>\"%2\"</b> del campo <b>%1</b>. Escriba según "
                                  "la especificación del campo")
                            .arg(namefield)
                            .arg(value);
                    return fieldsnullpair;
                    }
                    else {
                                SYE << tr("NO es válido el valor "
                                      "<b>\"%2\"</b> del campo <b>%1</b>.:\"%3\"")
                                .arg(namefield)
                                .arg(value)
                                .arg(usertip);
                        return fieldsnullpair;

                    }
                }
                else {
                    if ( origValue != value ) {
                        int lastpos = SafetYAWL::fieldsvalue.lastIndexOf(origValue);
                        if ( lastpos < 0 ) {
                            // Error
                            return fieldsnullpair;
                        }
                        qDebug("   **(*)...current origValue: %s", qPrintable(SafetYAWL::fieldsvalue));
                        SafetYAWL::fieldsvalue.replace(lastpos, origValue.length(),value);
                        qDebug("  **(*)...current origValue: %s", qPrintable(SafetYAWL::fieldsvalue));
                    }
                }

            }
            else { // No es un campo obligatorio
                QString namefield = valuemap.keys().at(i);
                QString value = newvalue.replace("'","");
                bool resultDoValidation;
                if ( titles.contains(valuemap.keys().at(i))) {
                    for(int j = 0; j < titles.values(valuemap.keys().at(i)).count();j++) {
                        namefield = titles.values(valuemap.keys().at(i)).at(j);
                        resultDoValidation = doFieldValidation(nameoperation, namefield,value,
                                                               usertip,fieldkeysvalues);
                        if (resultDoValidation) {
                            break;
                        }
                    }

                }
                else {
                    resultDoValidation = doFieldValidation(nameoperation, namefield,value,usertip,
                                                           fieldkeysvalues);
                }
                QString origValue = value;
                if ( !value.isEmpty() ) {
                    if (  !resultDoValidation
                          && currentCommand() == 0) {
                        if (usertip.isEmpty()) {
                            SafetYAWL::streamlog
                                    << SafetLog::Error
                                    << tr("Error de validación (Campo No obligatorio): "
                                          "No es válido el valor \"%2\" del campo \"%1\"")
                                    .arg(namefield)
                                    .arg(value);
                            return fieldsnullpair;
                        }
                        else {
                            SafetYAWL::streamlog
                                    << SafetLog::Error
                                    << tr("Error de validación (Campo No obligatorio): "
                                          "No es válido el valor \"%2\" del campo \"%1\""
                                          ":\"%3\"")
                                    .arg(namefield)
                                    .arg(value)
                                    .arg(usertip);
                            return fieldsnullpair;

                        }

                    }
                    else {
                        if ( origValue != value ) {
                            int lastpos = SafetYAWL::fieldsvalue.lastIndexOf(origValue);
                            if ( lastpos < 0 ) {
                                // Error
                                return fieldsnullpair;
                            }
                            qDebug("...current origValue: %s", qPrintable(SafetYAWL::fieldsvalue));
                            SafetYAWL::fieldsvalue.replace(lastpos, origValue.length(),value);
                            qDebug("...current origValue: %s", qPrintable(SafetYAWL::fieldsvalue));
                        }
                    }

                }
            }
            // ********************************
            // *** Aqui incluir validación



	}

        fieldspair.first  = SafetYAWL::fieldsname;
        fieldspair.second = SafetYAWL::fieldsvalue;
	return fieldspair;
}

bool SafetTextParser::doFieldValidation(const QString& nameoperation,
                                        const QString& namefield, QString& value,
                                        QString& usertooltip,
                                        QString keyvalue) const {



    SYD << tr("SafetTextParser::doFieldValidation..........value...(1)...:|%1|")
           .arg(value);
    SYD << tr("....(BEFORE)....SafetTextParser::doFieldValidation......................keyvalue:|%1|")
           .arg(keyvalue);


     QString _dirInput = SafetYAWL::getConf()["Input/path"];
     if ( !_dirInput.endsWith("/") ) _dirInput.append("/");

     if (_dommodel == NULL ) {
         SYE << tr("El archivo de ACCIONES de modificación no se ha cargado!");
        return false;
     }

     _dommodel->setStringToParse(str());

     SYD << tr("***(AFTER)...SafetTextParser::doFieldValidation..........value...(2)...:|%1|")
            .arg(value);



    CmdWidget *mywidget = _dommodel->selWidgetFromField(nameoperation,namefield,keyvalue);

    SYD << tr(".........SafetTextParser::doFieldValidation............path: |%1|").arg(_currdeffile);

    SYD << tr("......SafetTextParser::doFieldValidation...nameoperation: |%1|,namefield: |%2|,value:|%3|")
           .arg(nameoperation)
           .arg(namefield)
           .arg(value);


    if ( mywidget == NULL ) {
        if (keyvalue.isEmpty()) {
            SYW << tr("KeyValue esta vacia");
            return true;
        }

       SYW
                << tr("El widget para el campo "
                      "con nombre \"%1\" no se pudo crear para efectos de validación")
                .arg(namefield);
            return false;
    }
    usertooltip = mywidget->userTooltip();
    if ( mywidget->conf().contains("literal")) {
        qDebug("...literal...(1)...");
        QString myliteral = mywidget->conf()["literal"].toString();
        qDebug("...literal...(2)...");
        if ( myliteral == "_COMBOVARGLOBAL0") {
            value = SafetYAWL::combovarglobal0;
            qDebug("               literal: %s", qPrintable(value));
        }

    }



    if (mywidget->isValid(value)) {
        delete mywidget;

        return true;
    }
    delete mywidget;

    return false;

}

void SafetTextParser::checkWidgetsVarGlobals() {

//    if ( varvalues.keys().contains(curvalue)) {
//        SafetYAWL::combovarglobal0 = getVarValue(curvalue);
//        qDebug("...**insertAndClose():varcomboglobal: %s", qPrintable(SafetYAWL::combovarglobal0 ));
//    }
}

bool SafetTextParser::checkIsActionsField(const QString& fieldname,
                                          const QString& key,
                                          const QString& info,
                                          const QMap<QString,QString>& fieldtypes) {

/*
    if (fieldname.isEmpty() ) {
        return true;
    }
    if (fieldname.startsWith("_")) {
        return true;
    }
    TextEdit* currentedit = MainWindow::mymainwindow->currentTextEdit();
    if (currentedit == NULL) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("Error de PUNTERO NULO: Archivo: %s Línea : %d",
                      __FILE__,__LINE__);
        return false;
    }


    DomModel* mymodel = currentedit->dommodel();
    if (mymodel == NULL) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("Error de PUNTERO NULO: Archivo: %s Línea : %d",
                      __FILE__,__LINE__);
        return false;
    }
    if (!fieldtypes.contains(fieldname)) {
        return false;

    }

    bool result = mymodel->isDynWidget(fieldtypes[fieldname]);
    if ( result ) {
        _actions.append(Action(fieldtypes[fieldname],key,info));
    }
    return result;
*/
return false;

}

QString SafetTextParser::getValueOfKey(KeyType t, const QString& v) {
     QString result;
     if ( t == SafetTextParser::Sequence ) {
          QSqlQuery query( SafetYAWL::currentDb );
          QString command = QString("SELECT nextval('%1')").arg(v);

          query.prepare(  command );
         bool executed = query.exec();
         if (!executed ) {
              SafetYAWL::streamlog << SafetLog::Error << tr("Problema con la secuencia \"%1\", no se ejecutó correctamente la sentencia SQL: \"%2\"").arg(v).arg(command);
              return result;
          }

         bool isnext = query.next();
         SafetYAWL::evalAndCheckExit(isnext,tr("No hay registros para ejecutar la secuencia \"%1\"").arg(v));         
         bool ok;
         result = query.value(0).toString();

         command = QString("SELECT setval('%1',%2)").arg(v).arg(result.toInt(&ok)-1);
         query.prepare( command );
         executed = query.exec();
         if (!executed ) {
              SafetYAWL::streamlog << SafetLog::Warning << tr("Problema restaurando la secuencia \"%1\", no se ejecutó correctamente la sentencia SQL: \"%2\"").arg(v).arg(command);
          }

    }
     return result;
}


QString SafetTextParser::getValueForFunction(const QString& v, const QString &format,
                                             const QMap<QString,QString> &vmap, const QString& keyvalue ) {


    SYD << tr("\n");
    SYD << tr ("**********........SafetTextParser::getValueForFunction.......");
    foreach(QString k, vmap.keys()) {

        SYD << tr("......key:|%1|: |%2|....")
               .arg(k)
               .arg(vmap[k]);

    }
    SYD << tr("\n\n");
     QString result = "''";
     if ( format == "time_t" ) {
          result = "0";
     }

     else if ( format == "integer") {
          result = "0";
     }
     SYD << tr("....evaluating KEYVALUE: |%1|")
            .arg(keyvalue);
     QString newv = v;
     if (!keyvalue.isEmpty()) {
         newv.replace("{#keyvalue}",keyvalue);
         SYD << tr("....evaluating newv: |%1|")
                .arg(newv);
     }


     foreach(QString k, vmap.keys()) {

         SYD << tr("......key:|%1|: |%2|....")
                .arg(k)
                .arg(vmap[k]);
          QString mypattern = QString("{#%1}").arg(k);
          newv.replace(mypattern,vmap[k]);
     }

     if (newv.indexOf("{#currvalue#")!= -1) {
         SafetWorkflow* mywf = MainWindow::configurator->getWorkflows().at(0);
         Q_CHECK_PTR(mywf);
         QRegExp rx("\\{#currvalue#([a-zA-Z0-9_]+)\\}");

         int pos = newv.indexOf(rx);
         if (pos == -1) {
             SYE << tr("No se especificó correctamente el atributo 'function' (getValueFunction) de un campo del archivo  \"deftrac.xml\"");
             return "";

         }
         QString mykeyvalue = rx.cap(1);
         SYD << tr("SafetTextParser::getValueForFunction...mykeyvalue (currvalue):")
                .arg(mykeyvalue);
         if ( !vmap.contains(mykeyvalue)) {
             SYE << tr("El atributo \"function\" no se encuentra correctamente definido. El campo \"%1\" no se encuentra en la base de datos")
                    .arg(mykeyvalue);
             return "";
         }

        QStringList infos = mywf->textualInfos(vmap[mykeyvalue]);
        QString mycurrvalue = "";
        if (infos.count() > 0) {
            mycurrvalue = infos.at(0);
        }
        SYD << tr("SafetTextParser::getValueForFunction...currvalue:|%1|").arg(mycurrvalue);
        newv.replace(rx,mycurrvalue);

     }

     newv.replace("_USERNAME", SafetYAWL::currentAuthUser());
     QString command = "SELECT "+newv+";";
     QSqlQuery query(SafetYAWL::currentDb ); // <-- puntero a db actual
     SYD << tr("SafetTextParser::getValueForFunction...Sentencia SQL PARA EVALUAR LA FUNCION \"%2\" a ejecutarse: \n \"%1\"")
            .arg( command ).arg(v);
     command.replace("id='coloured'","id>=0");
     query.prepare(  command );
     bool executed = query.exec();
     if ( !executed ) {
          SYW <<  tr("La Sentencia SQL para ejecutar la función:\n\" %1 \" \nes incorrecta. Ejecute la sentencia en un cliente del gestor de BD, y compruebe resultados").arg(command);
          return result;
     }
     SYA  << tr("Ejecutada correctamente la sentencia SQL para la función: \"%1\"").arg(v);
     executed = query.next();

     if (!executed ) {
          SYW << tr("No se obtuvieron registros al ejecutar la sentencia SQL para la funcion: \"%1\"").arg(v);
          return result;
     }
     if ( format == "time_t" ) {
           QDateTime mydate;
           QString s = query.record().value(0).toString();
           mydate= QDateTime::fromString(s, Qt::ISODate);
           result = QString("%1").arg(mydate.toTime_t());
     }
     else if ( format == "integer") {
         result = query.record().value(0).toString();
     }
     else if ( format == "quotes") {
         result = "'" + query.record().value(0).toString() + "'";
     }
     else {
         result = query.record().value(0).toString();
         SYD << tr("....SafetTextParser::getValueForFunction... otro formato...");
     }
     SYD << tr("SafetTextParser::getValueForFunction...result:|%1|\n\n")
            .arg(result);
     return result;
}




QString SafetTextParser::processCommand(const QDomElement& e, const QDomNode& ninput,
	QMap<QString,QString>& keysforeignmap) {
	QDomNamedNodeMap attrs = e.attributes ();
	
	currnametable = attrs.namedItem("table").nodeValue();		


	QString curr = commands()[ attrs.namedItem("type").nodeValue() ];
        curr = curr.trimmed();
        if ( curr == "INSERT INTO") {
		QPair<QString,QString> fieldspair;
                fieldspair = buildFields(e,ninput,keysforeignmap);
                if ( fieldspair.first.isEmpty() && fieldspair.second.isEmpty() ) {
                    return "";
                }
                if (!fieldspair.first.isEmpty()) {
                    fieldspair.first.chop(1);
                }
                if (!fieldspair.second.isEmpty()) {
                    fieldspair.second.chop(1);
                }

		curr = curr + " ";
		curr = curr + currnametable;
		curr = curr + " ";
		curr = curr + "("+fieldspair.first +") ";
		curr = curr + " VALUES ";	
		curr = curr + "("+fieldspair.second+"); ";
        } else if ( curr == "DELETE") {
                QList<QPair<QString,QString> > listfieldspair;
                listfieldspair = getFieldsValues(e,ninput,keysforeignmap);
                if ( listfieldspair.isEmpty() ) {
                    return "";
                }
                curr = curr + " FROM ";
                curr = curr + currnametable;
                curr = curr + "  ";
                Q_ASSERT(listfieldspair.count() > 0 );
                 curr = curr + " WHERE ";
                for(int i = 0; i < listfieldspair.count();i++) {
                    QString fieldkey = listfieldspair.at(i).first;
                    QString valuekey = listfieldspair.at(i).second;
                    curr = curr + fieldkey;
                    curr = curr + "=";
                    bool addquotes =  !(SafetYAWL::canTypeConvert(valuekey, QVariant::Bool)
                                    || SafetYAWL::canTypeConvert(valuekey, QVariant::Int));
                    curr = curr + (addquotes?"'":"");
                    curr = curr + valuekey;
                    curr = curr + (addquotes?"'":"");
                    curr = curr + " AND ";
                }
                curr.chop(5);
                qDebug("....................getFieldsValues...DELETE:\n\n%s\n\n",
                       qPrintable(curr));

	} else if ( curr == "UPDATE") {
		QList<QPair<QString,QString> > listfieldspair = getFieldsValues(e,ninput,keysforeignmap);
                if ( listfieldspair.isEmpty() ) {
                    return "";
                }
		curr = curr + " ";
		curr = curr + currnametable;
		curr = curr + " SET ";
		QString fieldkey, valuekey;
		Q_ASSERT(listfieldspair.count() > 0 );
        int i;
        bool addquotes;
        for(i=1; i < listfieldspair.count(); i++) {
            QPair<QString,QString> mypair = listfieldspair.at(i);
            if (mypair.first.trimmed().isEmpty()) {
                continue;
            }
            if (mypair.first.trimmed().startsWith("case when",Qt::CaseInsensitive)) { //FIX CHECK readonly fields
                SYD << tr("...startsWith CASE WHEN:|%1|").arg(mypair.first);
                continue;
            }
            curr = curr + mypair.first;
            curr = curr + "=";
            addquotes =  !(SafetYAWL::canTypeConvert(mypair.second, QVariant::Bool) || SafetYAWL::canTypeConvert(mypair.second, QVariant::Int));
            curr = curr + (addquotes?"'":"");
            curr = curr + mypair.second;
            curr = curr + (addquotes?"'":"");
            curr = curr + ",";
        }
        if (i == 1 ) {
            fieldkey = listfieldspair.at(0).first;
            valuekey = listfieldspair.at(0).second;

            QString keyupdate;
            keyupdate = keyupdate + fieldkey;
            keyupdate = keyupdate + "=";
            addquotes =  !(SafetYAWL::canTypeConvert(valuekey, QVariant::Bool) || SafetYAWL::canTypeConvert(valuekey, QVariant::Int));
            keyupdate = keyupdate + (addquotes?"'":"");
            keyupdate = keyupdate + valuekey;
            keyupdate = keyupdate + (addquotes?"'":"");
            curr = curr + " " + keyupdate + " WHERE " + keyupdate;
        }
        else {
            fieldkey = listfieldspair.at(0).first;
            valuekey = listfieldspair.at(0).second;
            curr.chop(1);
            curr = curr + " WHERE ";
            curr = curr + fieldkey;
            curr = curr + "=";
            addquotes =  !(SafetYAWL::canTypeConvert(valuekey, QVariant::Bool) || SafetYAWL::canTypeConvert(valuekey, QVariant::Int));
            curr = curr + (addquotes?"'":"");
            curr = curr + valuekey;
            curr = curr + (addquotes?"'":"");
        }
        }

	return curr;
}

QStringList SafetTextParser::processXml(bool doquery,bool dopermises) {
        QStringList results;
	Q_ASSERT( docok );  // Documento Abierto
	Q_CHECK_PTR( inputDomDocument );  // Documento de Entrada Abierto
	QDomNode ninput = inputDomDocument->firstChild();
	QString nameoperation = ninput.attributes().namedItem("parameter").nodeValue();
       SafetYAWL::streamlog.initAllStack();


        if ( dopermises ) {
            QMap<QString,QString> myhierarchy = MainWindow::loadPermisesHierarchy();
            if (!MainWindow::doPermiseExecOperation(nameoperation,myhierarchy)) {
                SYE << tr("Usuario no autorizado para realizar la operacion: %1")
                       .arg(nameoperation);
                return QStringList();
            }
        }
        operationname =  nameoperation; // Chequear cuandos se actualiza el nombre de la operación

	QDomNode n = findOperation( nameoperation );
        SYD << tr("...........SafetTextParser::processXml.....OPERATIONACTION...nameoperation:|%1|")
       .arg(nameoperation);

        if (n.isNull() ) {
           SYW
                    << tr("El archivo de configuración <b>NO</b> contiene <br/>"
                          "la operación: <b>%1</b>")
                    .arg(nameoperation);
            return results;
        }
        QString myaction = n.attributes().namedItem("dataaction").nodeValue();
        if (!myaction.isEmpty()) {
            SYD << tr("...........SafetTextParser::processXml.....DATAACTION: |%1|")
                   .arg(myaction);
            _currentdataaction = myaction;
        }

        myaction = n.attributes().namedItem("postaction").nodeValue();
        if (!myaction.isEmpty()) {
            SYD << tr("...........SafetTextParser::processXml.....POSTACTION: |%1|")
                   .arg(myaction);
            _currentpostaction = myaction;
        }


	QDomNodeList mylist = n.childNodes();
	QMap<QString,QString> keysforeignmap;
    for(int i=0; i < mylist.count(); i++) {
        Q_ASSERT (mylist.at(i).isElement());
        QDomElement myElement = mylist.at(i).toElement();

        if (myElement.tagName() != "command") {
            SYD << tr("....SafetTextParser::processXml.....NOTAG_COMMAND:|%1|")
                   .arg(myElement.tagName());
            continue;
        }
        // ** Importante donde se procesan los comandos
        //keysforeignmap.clear();
        setCurrentCommand(i);
        QString command = processCommand ( myElement,
                                           inputDomDocument->firstChild(),keysforeignmap);

        // ** Importante donde se procesan los comandos
        if (command.isEmpty() || SafetYAWL::errorStack().count() > 0 ) {
            return QStringList();
        }

        SYD
                << tr("  La Sentencia SQL a ejecutarse (1): \n \"%1\"").arg( command );

        results.append( replaceXmlCharSpecial(command,false));
        SYD
                << tr("  La Sentencia SQL a ejecutarse (2): \n \"%1\"").arg( command );
        if ( doquery ) {
            QSqlQuery query(SafetYAWL::currentDb ); // <-- puntero a db actual

            query.prepare(  command );
            bool executed = query.exec();
            if ( !executed )  {
                SYW
                        << tr("La Sentencia SQL :\n\"%1\"\nES INCORRECTA"
                              "--> Ejecute la sentencia en un cliente del gestor de BD"
                              ", y compruebe resultados.")
                           .arg(command);

//                SYE
//                        <<    tr("%2. \"%3\". \"%1\"")
//                              .arg(query.lastError().databaseText())
//                              .arg(query.lastError().number())
//                              .arg(query.lastError().type());

                SYE
                        <<    tr("%1")
                              .arg(query.lastError().databaseText());

                return QStringList();
            }

            if ( executed ) {
                SYA     << tr("*Consulta SQL:\n\"%1\"\n ejecutada satisfactoriamente...OK!").arg(command);

		SYD << tr(".........LASTINSERTID:|%1|").arg(query.lastInsertId().toInt());
SYD << tr("**processXml...GENCURRJSON...1");

		if (_currid == "-1" ) {
			QString mysql = command.trimmed();
			ParsedSqlToData  data = SafetTextParser::parseSql(mysql,true);
       
                	QString myseq = data.table + "_id_seq";
                
	                SYD << tr("**processXml...GENCURRJSON...sequence:|%1|")
                       .arg(myseq);
		
			QSqlQuery query( SafetYAWL::currentDb );
               		 QString command = QString("SELECT currval('%1')").arg(myseq);
      
        	  	 query.prepare(  command );
      	          bool executed = query.exec();
                	if (!executed ) {
        	            SYA << tr("No se calcula el ultimo ID. Problema con la secuencia \"%1\".").arg(myseq);
                	}
      
              	 bool isnext = query.next();
               
                
              		 QString myid = "-1";
            		   if (isnext ) {
                	        myid = query.value(0).toString();
              		 }
              		 SYD << tr("**processXml...GENCURRJSON...2.CURRID.myid:|%1|")
                      .arg(myid);
   	 		_currid = myid;
		}

                SYD << tr("**processXml...GENCURRJSON...2");
            }
        }
    }

    if ( doquery ) {
            SYD
                    << tr("Número de acciones a ejecutar: \"%1\"")
                    .arg(_actions.count());

            foreach(Action a, _actions) {
                 qDebug("....processXml...ejecutando: |%s|...|%s|...|%s|",qPrintable(a.type),
                        qPrintable(a.key),qPrintable(a.info));
                 SafetYAWL::streamlog << SafetLog::Debug
                         << tr("Ejecutando acción del tipo \"%1\" "
                               " para clave \"%2\" con valor \"%3\""
                               )
                                .arg(a.type)
                                .arg(a.key)
                                .arg(a.info);
                 doAction(a.type, a.key,a.info);
                 setCurValuekey(a.key);
            }
        }



        return results;
}


bool SafetTextParser::executeSQL(const QString& s) {

	return true;
}


ParsedSqlToData SafetTextParser::parseSql(const QString& s, bool parsetomap ) {


    ParsedSqlToData data;


     QRegExp rx;
     QString newsql = s;
     /**
       FIXME ¿Porque se reemplazada aqui"
       **/
     newsql.replace("'","");
     QString updatePattern = "UPDATE\\s+([a-zA-Z0-9_\\.\\(\\)][a-zA-Z0-9_,'\\.\\(\\)\\-]*)\\s+"
                             "SET\\s+"
                             "([a-zA-Z0-9_\\.\\(\\)\\[\\]'áéíóúñÑ\\|&][@a-zA-Z0-9_,;\\s'\\=\\.\\(\\)\\-\\[\\]áéíóúñÑ\\|\\*:#\\{\\}\\?/&]*\\s+)?WHERE\\s+"
                             "([a-zA-Z0-9_\\.\\(\\)][@a-zA-Z0-9_,;'\\=\\.\\(\\)\\-\\[\\]\\*]*)";

     QString insertPattern = "INSERT INTO\\s+([a-zA-Z0-9_][a-zA-Z0-9_\\.\\-]*)\\s+"
                             "\\(([a-zA-Z0-9_\\.\\(\\)\\[\\]][@a-zA-Z0-9_,'\\=\\.\\(\\)"
                             "\\-\\[\\]]*)\\)\\s+"
                             "VALUES\\s+\\(([a-zA-Z0-9_'\\./\\[\\]#\\|&][@a-zA-Z0-9_,'\\=\\.\\-/\\[\\]\\s#\\|\\*:#\\{\\}\\?\\(\\)&]*)"
                             "\\)\\s*";

     QString table = rx.cap(1);
     QString fields = rx.cap(2);
     QString values = rx.cap(3);
     QString keyfield;
     QString keyvalue;


     rx.setPattern(insertPattern);
     int pos = rx.indexIn(newsql);
     if ( pos == -1 ) {
         SYD
                              << tr("*La Sentencia \"%1\" no es una sentencia SQL de INSERCION (INSERT)")
                                 .arg(newsql);                  
         rx.setPattern(updatePattern);
         pos = rx.indexIn(newsql);

         if ( pos == -1 ) {
             SYW
                                  << tr("*La Sentencia generada \"%1\" no es una sentencia SQL válida de ACTUALIZACION (UPDATE)")
                                     .arg(newsql);
             data.null = true;
             return data;
         }
         else {
             table = rx.cap(1);
               QStringList allfields  = rx.cap(2).split(",",QString::SkipEmptyParts);

               SYD << tr(".........SafetTextParser::parseSql....rx.cap(3):|%1|")
                      .arg(rx.cap(3));
               keyfield = rx.cap(3).split("=").at(0).trimmed();
               keyvalue = rx.cap(3).split("=").at(1).trimmed();

               data.key = keyfield;
               data.keyvalue = keyvalue;
               SYD << tr(".........SafetTextParser::parseSql....data.key:|%1|")
                      .arg(data.key);
               SYD << tr(".........SafetTextParser::parseSql....data.keyvalue:|%1|")
                      .arg(data.keyvalue);

             foreach(QString f, allfields) {
                 QStringList myfield = f.split("=");

                 data.fields.append(myfield.at(0));
                 data.values.append(myfield.at(1));
             }

         }

     }
     else {
         table = rx.cap(1);
         fields = rx.cap(2);
         values = rx.cap(3);

         data.fields = fields.split(",",QString::SkipEmptyParts);
         data.values = values.split(",",QString::SkipEmptyParts);

     }




     data.table = table;
     if ( data.fields.count() > data.values.count()) {
       SYE
                 << tr("El numero de los nombres de los campos (%1) es mayor el numero de sus valores "
                       "(%2) correspondientes son diferentes")
                    .arg(data.fields.count())
                    .arg(data.values.count());
         return data;
     }
     if (data.key.isEmpty() ) {
        data.key = data.fields.at(0);
        QString newvalue = data.values.at(0).trimmed();
        SYD << tr("...........PROCESSING POSTACTION. (1)..newvalue:|%1|").arg(newvalue);
        // *** Para Postgresql
        if (newvalue.startsWith("nextval")) {
            QRegExp rx("\\s*nextval\\(([a-zA-Z_0-9'\\-]+)\\s*\\)\\s*");

            int pos = rx.indexIn(newvalue);
            if (pos == -1) {
                SYW << tr("Error (POSTACTION) al evaluar nextval en la expresión:\"%1\"")
                       .arg(newvalue);
                return data;
            }

            SYD << tr(".........EN EL CAMBIO...nextval POR currval");
            QString result = "";
            QString mysql = QString("SELECT last_value from %1").arg(rx.cap(1));

            SYD << tr("SafetTextParser::parseSQL...MYSQL:|%1|")
                   .arg(mysql);

            QSqlQuery query( SafetYAWL::currentDb );
            query.prepare(  mysql );

            SYD << tr(".........CURRVAL_APPEND_parseSQL::execBeforeFunction:|%1|")
                   .arg(mysql);
              bool executed = query.exec();
              if (!executed ) {
                   SYW << tr(" (email function) no se ejecutó correctamente "
                         "la sentencia SQL: \"%1\"")
                                           .arg(mysql);

               }
              bool isnext = query.next();
              if ( !isnext ) {
               SYW << tr(" (email function) No hay registros para ejecutar "
                      "la sentencia SQL: \"%1\"").arg(mysql);
              }
              else {

                result = query.record().value(0).toString();
              }


            SYD << tr("...........PROCESSING POSTACTION. (2)..result:|%1|").arg(result);
            data.keyvalue = result;
        }
        else {
            data.keyvalue = newvalue;
        }
     }
     data.null = false;
     if ( parsetomap ) {
         for(int i=0; i < data.fields.count(); i++) {
             data.map[ data.fields.at(i)] = data.values.at(i);
         }

     }
     return data;

}

bool SafetTextParser::checkIsField(const QString& f) {
    QString newstr = f;

    if ( newstr.endsWith(":")) {
        newstr.chop(1);
    }
//    qDebug("newstr:%s", qPrintable(newstr));
   QStringList mylist; // = getCurrentFields();
    if (mylist.count() == 0) {
        return true;
    }
    foreach (QString s, mylist) {
//        qDebug("...s:%s",qPrintable(s));
        if (s.startsWith(newstr)) {
            return true;
        }
    }
    return false;

}


