%Module Safet 0

%Import QtCore/QtCoremod.sip

class SafetYAWL 
{
%TypeHeaderCode
#include "/home/vbravo/trac/seguridad/safet/libsafet/trunk/src/SafetYAWL.h"
%End

public:
    SafetYAWL();
    SafetYAWL(const SafetYAWL&);
    
    void openXML(const QString& s);
    void convertXMLtoObjects();
    
};