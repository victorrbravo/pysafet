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
#include "suggestwidget.h"
//#include "googlesuggest.h"

#define GSEARCH_URL "http://www.google.com/search?q=%1"

SuggestWidget::SuggestWidget(const QString& t, QObject *parent, bool istextparent )
    : CmdWidget(t, parent, istextparent) {



}

SuggestWidget::~SuggestWidget() {

}


void SuggestWidget::setText(const QString &newText) {

}


bool SuggestWidget::isValid(QString& value) {
    return CmdWidget::isValid(value);
}

void SuggestWidget::buildWidget() {
    CmdWidget::buildWidget();
  //  completer = new GSuggestCompletion(lineedit);

//    adjustSize();
//    resize(400, height());
//    lineedit->setFocus();

}

void SuggestWidget::setFocus ( Qt::FocusReason reason ) {
    CmdWidget::setFocus( reason );

     
     
}
void SuggestWidget::insertAndClose() {
    CmdWidget::insertAndClose();
}


QString SuggestWidget::text() const {
    return CmdWidget::text();
}               
