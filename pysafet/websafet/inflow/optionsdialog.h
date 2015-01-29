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
#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include "ui_inflowopciones.h"


class OptionsDialog : public QDialog
                     , private Ui::OptionDialog
{

     Q_OBJECT
     QMap<QString,QMap<QString,QWidget*> > widgetmap;
public:
    OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();
    QListWidget* plugList() { return listWidget; }
    QWidget* getVariantWidget(const QVariant& v, const QString& message = "");
public slots:
    void doLoadWidget();
    void addWidget();
    void delWidget();
    void doSaveConfs();
    void doAccept();

    void addTab(const QString&);
private:
    void buildTab();

};

#endif // OPTIONSDIALOG_H
