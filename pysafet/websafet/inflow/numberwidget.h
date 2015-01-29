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
#ifndef NUMBERWIDGET_H
#define NUMBERWIDGET_H

// #include <QSpinBox>
// #include <QDoubleSpinBox>
#include "cmdwidget.h"

class NumberWidget : public CmdWidget {
 Q_OBJECT

    QString _format;

public:
    NumberWidget(const QString& t, QObject *parent = 0, bool istextparent = true);
    ~NumberWidget() {}
    virtual void buildWidget();

    virtual QString text() const;
     virtual void setFocus ( Qt::FocusReason reason );
//     virtual QRect getGeoParams() const;
//     QStringList& options() { return _options;}
    virtual bool isValid(QString& value);
    virtual QString html();
    virtual QString htmlForKey(const QString &k, const QString &otherk = "");
public slots:
     virtual void setText(const QString &newText);
 private:



};

#endif // NUMBERWIDGET_H
