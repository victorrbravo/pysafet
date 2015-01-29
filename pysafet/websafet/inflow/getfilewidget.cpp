/*
* SAFET Sistema Automatizado para la Firma Electrónica y Estampado de Tiempo
* Copyright (C) 2008 VÃ­ctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
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

/****************************************************************************
**
** Copyright (C) 2004-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

//#include <QtGui>
//#include <QToolButton>
//#include <QMessageBox>
#include "SafetYAWL.h"
#include "getfilewidget.h"

GetFileWidget::GetFileWidget(const QString& t, QObject *parent, bool istextparent )
    : CmdWidget(t, parent,istextparent)
{

    _showfilename = true;

    SYD << tr("...GetFileWidget::GetFileWidget....(1)");

}

GetFileWidget::~GetFileWidget() {

}


void GetFileWidget::setText(const QString &newText) {
    _currtext = newText;
}


void GetFileWidget::showDialog() {
/*    QString fileName;

   QString types = tr("Todos (*);;Archivos de Texto (*.txt)");

   if ( conf().contains("filter") ) {
       types = conf()["filter"].toString();
   }

    QString curdir = QDir::homePath()+ "/" + Safet::datadir;
    QString dialogtype = conf()["options"].toString();
    if (dialogtype.startsWith("open") || dialogtype.isEmpty() ) {
        fileName = QFileDialog::getOpenFileName(this,
                                                tr("Seleccione un archivo"),
                                                curdir,
                                                types);

    }
    else if ( dialogtype.startsWith("save") ) {

             fileName = QFileDialog::getSaveFileName(this,
                                                     tr("Seleccione un directorio y un nombre de archivo"),
                                                     QDir::homePath(),
                                                     types);
    }
    else if (dialogtype.startsWith("dir") ) {
             fileName = QFileDialog::getSaveFileName(this,
                                                     tr("Seleccione un Directorio"),
                                                     QDir::homePath());
   }
    else {
        SafetYAWL::streamlog << SafetLog::Error << tr("No se definió correctamente el atributo \"%1\" "
                                                      "de un campo \"filename\" "
                                                      "(campos posibles open,save,dir").arg(dialogtype);
    }

    if (!fileName.isEmpty()) {
        filecombo->setEditText(fileName);

         filecombo->setFocus( Qt::PopupFocusReason );
    }
*/
}



bool GetFileWidget::isValid(QString& value) {
     _currtext = value;
     QString optionsstr = conf()["options"].toString();
     if ( optionsstr.startsWith("save")) {
         return CmdWidget::isValid(value);
     }
     else {

        if ( !QFile::exists(value)) {
            return false;
        }
    }
     SYD << tr("..GetFileWidget::isValid...setting...SafetYAWL::combovarglobal0");
     SafetYAWL::combovarglobal0 = value;
     SYD << tr("..GetFileWidget::isValid...setting..value:|%1|").arg(SafetYAWL::combovarglobal0);

    return true;
}

void GetFileWidget::buildWidget() {
    updateCombo();


}

void GetFileWidget::setFocus ( Qt::FocusReason reason ) {
//     QWidget::setFocus ( reason );
//     filecombo->setFocus( reason);
     
     
}
void GetFileWidget::insertAndClose() {    
/*    if ( _texteditparent ) {
        if ( filecombo) {
            QString value = filecombo->currentText().simplified();
            _texteditparent->insertPlainText(value.toLatin1());
            _texteditparent->insertPlainText("\n");
        }
    }

    close();*/
}

void GetFileWidget::updateCombo() {

    SYD << tr("...GetFileWidget::updateCombo()....(1)");
    QStringList myoptions = conf()["options"].toString().split(",",QString::SkipEmptyParts);

    _selected.clear();
    foreach(QString op, myoptions) {
        QString myfield = op.section("::",0,0).trimmed();
        SYD << tr("........GetFileWidget::updateCombo.......myfield:|%1|")
               .arg(myfield);
        if (myfield.compare("show",Qt::CaseSensitive) == 0) {
            QString myvalue = op.section("::",1,1).trimmed();
            if (myvalue.compare("id",Qt::CaseSensitive) == 0 ) {
                _showfilename = false;
            }
        }
        else if (myfield.compare("selected",Qt::CaseSensitive) == 0) {
            QString myvalue = op.section("::",1,1).trimmed();
            QStringList numbers = myvalue.split(";",QString::SkipEmptyParts);
            bool ok;

            SYD << tr("........GetFileWidget::updateCombo.......numbers.count():|%1|")
                   .arg(numbers.count());

            foreach(QString n, numbers) {
                int number = n.toInt(&ok);
                _selected.append(number);
                SYD << tr("........GetFileWidget::updateCombo.......GETFILEWIDGETAPPEND:|%1|")
                       .arg(number);


            }

        }
    }

    /* FIXME ver un algoritmo para mostrar los id de los graficos */

    //_showfilename = true;
    SYD << tr("........GetFileWidget::updateCombo......._selected.count():|%1|")
           .arg(_selected.count());


    QString mime = SafetYAWL::getConf()["Widgets/getfilewidget.*"];
        QStringList mimes  = mime.
                         split(SafetYAWL::LISTSEPARATORCHARACTER,QString::SkipEmptyParts);

        _options.clear();
        _wfids.clear();


        SYD << tr("...........GetFileWidget::updateCombo(out).............mimes.count():|%1|")
               .arg(mimes.count());

        for(int i=0; i < mimes.count(); i++) {


            if (!QFile::exists(mimes.at(i))) {
                continue;
            }
            if (_selected.count() > 0 ) {
                if (!_selected.contains(i)) {
                    continue;
                }
            }

            if ( _showfilename == false )  {
                SafetYAWL  myconfigurator (SafetYAWL::pathconf);
                myconfigurator.openXML(mimes.at(i));
                myconfigurator.convertXMLtoObjects();
                SafetWorkflow *mywf = myconfigurator.getWorkflow();
                if ( mywf ) {
                    _wfids.append(mywf->id());
                }
            }

            _options.append(mimes.at(i));
            SYD << tr("....GetFileWidget::updateCombo....file...i:|%2|...:|%1|")
                   .arg(mimes.at(i)).arg(i);
        }
        SYD << tr("...........****GetFileWidget::updateCombo(out).............");


}

QString GetFileWidget::html() {

    updateCombo();



    QString result = QString("<select name=\"%1\" id=\"%1\">\n")
            .arg(caption());

    result += QLatin1String("<option value=\"\"></option>");
    int i = 0;
    foreach(QString s, options()){
        if (s.trimmed().isEmpty()) {
            continue;
        }
        QString newitem;
        if ( _showfilename ) {
            newitem = QString("<option value=\"%1\">%1</option>\n")
                    .arg(s);
        }
        else {
            newitem = QString("<option value=\"%2\">%1</option>\n")
                    .arg(i<_wfids.count()?_wfids.at(i):s)
                    .arg(s);
        }

        i++;
        result += newitem;
    }


    result += QLatin1String("</select>");
    SYD << tr("...GetFileWidget::html()...:|%1|")
           .arg(result);
    return result;

}

QString GetFileWidget::text() const {
        QString result = "";
        result = _currtext;
        return result;
}               
