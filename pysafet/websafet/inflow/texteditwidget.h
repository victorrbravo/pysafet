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
#ifndef TEXTEDITWIDGET_H
#define TEXTEDITWIDGET_H

//#include <QTextEdit>
#include "cmdwidget.h"

class TextEditWidget : public CmdWidget {
 Q_OBJECT

    bool _iswiki;
    void buildWikiWidget();
public:
    TextEditWidget(const QString& t, QObject *parent = 0, bool istextparent = true);
    ~TextEditWidget() {}
    virtual void buildWidget();

    bool isWiki() const { return _iswiki;}

    virtual QString text() const { return QString(""); }
     virtual void setFocus ( Qt::FocusReason reason );

     QString toPlainText() const;
     QString toHtml() const;
     static QString htmlToWiki(const QString& s);
     static QString wikiToHtml(const QString& s);

     virtual QString html();
 public slots:
     virtual void setText(const QString &newText);
     void setBold();
     void setItalic();
     void setUnder();

     virtual void insertAndClose();
 private slots:
 private:

//    QTextEdit* textedit;
//    QToolButton *boldbutton;
//    QToolButton *italicbutton;
//    QToolButton *underbutton;
//    QAction *actionTextBold;
//    QAction *actionTextItalic;
//    QAction *actionTextUnder;


};

#endif // TEXTEDITWIDGET_H
