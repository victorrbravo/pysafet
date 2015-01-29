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
#include "SafetParameter.h"
#include "SafetYAWL.h"

SafetParameter::SafetParameter():
        SafetXmlObject() {

}




SafetParameter::~SafetParameter() {

}

void SafetParameter::setChangefor(const QString& c) {
    if (c.isEmpty()){
        SYE << tr("Se introdujo un campo vac�o: \"%1\" para el par�metro")
               .arg(c);
        return;
    }
    _changefor = c;
    _changeforlist = c.split(",",QString::SkipEmptyParts);


}


