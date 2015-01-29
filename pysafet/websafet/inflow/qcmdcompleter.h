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
//
// C++ Interface: qcmdcompleter
//
// Description: 
//
//
// Author: VÃ­ctor R. Bravo,,, <victor@victor-laptop>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QCMDCOMPLETER_H
#define QCMDCOMPLETER_H

#include <QCompleter>
#include "dommodel.h"
/**
	@author VÃ­ctor R. Bravo,,, <victor@victor-laptop>
*/
class QCmdCompleter : public QCompleter
{
Q_OBJECT
public:
    QCmdCompleter(QObject *parent = 0);

    ~QCmdCompleter();
    void setDomModel(DomModel* model);
    void reset();
    DomModel* domModel() const { return mymodel;}
    
private:
	DomModel *mymodel;
};

#endif
