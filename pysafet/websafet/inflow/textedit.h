/****************************************************************************
**
** Copyright (C) 2006-2006 Trolltech ASA. All rights reserved.
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
#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QDateTimeEdit>
#include "dommodel.h"

class QCompleter;
class QLineEdit;
class QDateTimeEdit;
class QCheckBox;
class QGroupBox;
class QWidget;
class CmdWidget;

class TextEdit : public QTextEdit
{
    Q_OBJECT


public:
    TextEdit(QWidget *parent = 0);
    ~TextEdit();

    void setCompleter(QCompleter *c);
    void unsetCompleter(QCompleter *completer);
    QCompleter *completer() const;
    QString searchCommand();
    QList< QMap<QString,QVariant> > _listconfs;
    QMap<QString,QVariant> findConf(const QString& n);
    QList< QMap<QString,QVariant> >& listConfs() { return _listconfs;}
    void updateDockSbMenu(bool getfocus = false);
    void insertTextRegToModify(const QString& fieldname, const QString& key);

    bool flaginsert;
    DomModel* dommodel();
    static QString hashToTextEdit(const QByteArray& h);
    static QByteArray texteditToHash(const QString& t);
    /**
      \brief Devuelve el widget actual que esta visible en el cuadro de edicion
      \return Widget actual
      */
    CmdWidget* currentWidget();
    /**
        \param Coloca el widget actual que se debe mostrar (Coloque NULL, para iniciar
        \param cur Widget actual
        */
    void setCurrentWidget(CmdWidget* cur) { l = cur;}
    QStringList getCurrentFields();
    bool checkIsField(const QString& f);

protected:
    void keyPressEvent(QKeyEvent *e);


private slots:

    void showFromWidget(const QRect& r, const QString& caption = "");
    void insertCompletion(const QString &completion);

public slots:

    void execSbMenuAction(const QString& title);
    void delCurrentField();
    void modifyCurrentField();
    void doTextChange();
    void cancelAndClose();
    void insertAndClose();

    /**
      * \brief Busca los campos y nombre de la operacion a realizar
               y los coloca en el campo list
        \param Parámetro de salida, incluye una lista con los "tokens" anteriores a donde
               se encuentra el cursor de texto, el ultimo elemento es el nombre de la operacion
        \return retorna verdadero (true) si existe un nombre de "operacion" en el texto, falso (false)
               en caso contrario
    */
    bool searchPrvsToken(QStringList& list);


private:
    QString textUnderCursor() const;
    QStringList fields;


private:
    QCompleter *c;
    CmdWidget *l;
};

#endif // TEXTEDIT_H

