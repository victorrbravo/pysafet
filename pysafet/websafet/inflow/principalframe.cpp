/*
* SAFET Sistema Automatizado para la Firma Electrnica y Estampado de Tiempo
* Copyright (C) 2008 Vctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigacin en Tecnologías Libres
*
* Este programa es software libre; Usted puede usarlo bajo los trminos de la licencia de
* software GPL versin 2.0 de la Free Software Foundation.
*
* Este programa se distribuye con la esperanza de que sea til, pero SI NINGUNA GARANTA;
* tampoco las implcitas garantas de MERCANTILIDAD o ADECUACIÓN A UN PROPSITO PARTICULAR.
* Consulte la licencia GPL para ms detalles. Usted debe recibir una copia de la GPL junto
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5 Piso, Boston, MA 02110-1301, USA.
*
*/

#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QItemSelectionModel>
#include <QGridLayout>
#include <QSpacerItem>
#include "principalframe.h"



PrincipalFrame::PrincipalFrame(QWidget *parent, Qt::WindowFlags f)
: QFrame(parent,f)
{
    setupUi(this);

}

PrincipalFrame::~PrincipalFrame() {
     //qDebug("...~OptionsDialog();...");
}

