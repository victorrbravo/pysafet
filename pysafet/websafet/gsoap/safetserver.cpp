// Contents of file "safetserver.cpp":
#include "soapH.h"
#include "safet.nsmap"

#include "SafetYAWL.h"
#include "SafetLog.h"

#include "SafetDocument.h"
#include "SafetDirXmlRepository.h"


class Foo 
{

public:
	Foo(){}
	
	static void evalEventOnExit(SafetLog::Level l);

};

void Foo::evalEventOnExit(SafetLog::Level l) {
    	if ( l == SafetLog::ExitApp ) {
		SafetYAWL::streamlog << "** Error: Ocurrio un error.No se pudo completar la accion.";
		SafetYAWL::streamlog << " * Importante: Revise el registro de eventos (log)" << endl;
		::exit(0);
	}
}



int main(int argc, char **argv)
{
   soap_serve(soap_new()); // call the incoming remote method request dispatcher 
}

// Implementation of the "add" remote method:
// in: cadena que representa el contenido del documento firmado
// name: nombre o identificador del documento firmado
// result: booleano donde se retorna el resultado de la funcion
int ns__guardarXml(struct soap * soap, xsd__string in, xsd__string name, xsd__boolean &result)
{
	//SafetConsoleApp safetapp(argc,argv);
	//safetapp.executeParsed();

	SafetYAWL yawl;

	SafetYAWL::setEvalExit( Foo::evalEventOnExit ); // <----- se cae aqui


        SafetYAWL::streamlog <<  SafetLog::Action << "Servicio Web iniciado correctamente...OK!";
        SafetDirXmlRepository repository;

        QString home = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.path").toString();
        QString homefile = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.name").toString();

        SafetDocument doc;

        if (repository.openContainer(home + "/" + homefile))
        {
                SafetYAWL::streamlog <<  SafetLog::Debug << "ns__guardarXml: Contenedor abierto exitosamente";

                // QString string, QString name
                SafetYAWL::streamlog <<  SafetLog::Debug << in;
                doc.writeFileToDisk(in, "file");
                repository.addXmlDocumentToContainer("/tmp/file", name);
                QFile::remove("/tmp/file"); // borrar el archivo temporal utilizado
                SafetYAWL::streamlog <<  SafetLog::Debug << "Agregado documento exitosamente";
                result = true;
        }
        else
        {
                // el contenedor no existe por lo tanto se debe crear
                if (repository.createContainer(home + "/" + homefile, "XmlContainer::WholedocContainer"))
                {
                        SafetYAWL::streamlog <<  SafetLog::Debug << "Contenedor creado exitosamente";
                        doc.writeFileToDisk(in, "file");
                        repository.addXmlDocumentToContainer("/tmp/file", name);
                        QFile::remove("/tmp/file"); // borrar el archivo temporal utilizado
                        SafetYAWL::streamlog <<  SafetLog::Debug << "despues de addXmlDocumentToContainer";
                        result = true;
                }
                else
                {
                        SafetYAWL::streamlog <<  SafetLog::Debug << "No se pudo crear el contenedor para almacenar el documento";
                        result = false;
                }
        }
        SafetYAWL::streamlog <<  SafetLog::Debug << "Servicio Web guardarXml terminado.";

	return SOAP_OK;
}

// *****************************************************************************************************************************

int ns__verifyDocumentSignatures(struct soap * soap, xsd__string in, xsd__boolean &result)
{

	SafetYAWL yawl;
	SafetYAWL::setEvalExit( Foo::evalEventOnExit );
	SafetYAWL::streamlog <<  SafetLog::Action << "Servicio Web de verificacion de firmas iniciado correctamente...OK!";

	SafetDirXmlRepository repository;
	QString home = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.path").toString();
	QString homefile = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.container.name").toString();
	QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfile").toString();

	if (repository.openContainer(home + "/" + homefile))
	{
		//SafetYAWL::streamlog <<  SafetLog::Debug << "ns__verifyDocumentSignatures: Contenedor abierto exitosamente";
		
		// QString string, QString name
		SafetYAWL::streamlog <<  SafetLog::Debug << home + "/" + homefile + "/" +in;
		SafetDocument doc;
		doc.initializeLibdigidoc();
    		doc.createOpenXAdESContainer();
    		doc.initDigidocConfigStore(libdigidocConfigFile);
		
		int res = doc.readOpenXAdESContainerFile(home + "/" + homefile + "/" +in);
		if (res != ERR_OK)
		{
			result = false;
			SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo leer el documento firmado dentro del contenedor .";
			return SOAP_OK;
		}
		
		if (doc.verifySignMadeWithSmartCard(home + "/" + homefile + "/" +in))
    		{
			doc.closeLibdigidoc();
			result = true;
			SafetYAWL::streamlog <<  SafetLog::Debug << "Verificacion de firmas del documento correcta!.";
			SafetYAWL::streamlog <<  SafetLog::Debug << "Servicio Web verifyDocumentSignatures terminado correctamente.";
			return SOAP_OK;
    		}
    		else
    		{
			SafetYAWL::streamlog <<  SafetLog::Debug << "Verificacion de firmas del documento INCORRECTA!.";
			doc.closeLibdigidoc();
			result = false;
			return SOAP_OK;
    		}
	}
	else // no se pudo abrir el contenedor que tiene el documento a verificar
	{
		result = false;
		SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo abrir el contenedor que tiene el documento a verificar.";
		return SOAP_OK;
	}
	//SafetYAWL::streamlog <<  SafetLog::Debug << "Servicio Web verifyDocumentSignatures terminado correctamente.";
	//return SOAP_OK;
	SafetYAWL::streamlog <<  SafetLog::Debug << "*-*-*-*-*- NO DEBERIA VER ESTA LINEA";

return SOAP_OK;
}

// *****************************************************************************************************************************


int ns__verifyDocumentSignatureWithId(struct soap * soap, xsd__string in, xsd__int indexOfSignature, xsd__boolean &result)
{

	SafetYAWL yawl;
	SafetYAWL::setEvalExit( Foo::evalEventOnExit );
	SafetYAWL::streamlog <<  SafetLog::Action << "Servicio Web de verificacion de firmas iniciado correctamente...OK!";

	SafetDirXmlRepository repository;
	QString home = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.path").toString();
	QString homefile = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.container.name").toString();
	QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfile").toString();

	if (repository.openContainer(home + "/" + homefile))
	{
		//SafetYAWL::streamlog <<  SafetLog::Debug << "ns__verifyDocumentSignatures: Contenedor abierto exitosamente";
		
		// QString string, QString name
		SafetYAWL::streamlog <<  SafetLog::Debug << home + "/" + homefile + "/" +in;
		SafetDocument doc;
		doc.initializeLibdigidoc();
    		doc.createOpenXAdESContainer();
    		doc.initDigidocConfigStore(libdigidocConfigFile);
		
		int res = doc.readOpenXAdESContainerFile(home + "/" + homefile + "/" +in);
		if (res != ERR_OK)
		{
			result = false;
			SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo leer el documento firmado dentro del contenedor .";
			return SOAP_OK;
		}
		
		if (doc.verifySignMadeWithSmartCard(home + "/" + homefile + "/" +in, indexOfSignature))
    		{
			doc.closeLibdigidoc();
			result = true;
			SafetYAWL::streamlog <<  SafetLog::Debug << "Verificacion de firmas del documento correcta!.";
			SafetYAWL::streamlog <<  SafetLog::Debug << "Servicio Web verifyDocumentSignatureWithId terminado correctamente.";
			return SOAP_OK;
    		}
    		else
    		{
			SafetYAWL::streamlog <<  SafetLog::Debug << "Verificacion de firmas del documento INCORRECTA!.";
			doc.closeLibdigidoc();
			result = false;
			return SOAP_OK;
    		}
	}
	else // no se pudo abrir el contenedor que tiene el documento a verificar
	{
		result = false;
		SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo abrir el contenedor que tiene el documento a verificar.";
		return SOAP_OK;
	}
	//SafetYAWL::streamlog <<  SafetLog::Debug << "Servicio Web verifyDocumentSignatures terminado correctamente.";
	//return SOAP_OK;
	SafetYAWL::streamlog <<  SafetLog::Debug << "*-*-*-*-*- NO DEBERIA VER ESTA LINEA";
return SOAP_OK;
}

// *****************************************************************************************************************************


int ns__verifyDocumentSignatureWithNames(struct soap* soap, xsd__string in, xsd__string signerNames, xsd__boolean &result)
{

	SafetYAWL yawl;
        // *********** Codigo para prueba
	SafetYAWL::setEvalExit( Foo::evalEventOnExit );
	QString home = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.path").toString();
	QString homefile = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.name").toString();
	QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfilepath").toString();

	SafetYAWL::streamlog <<  SafetLog::Action << QObject::tr("Servicio Web de verificacion para  firmas iniciado correctamente...ok!");
        SafetXmlRepository *xmlRepository = NULL;
        QString type = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.type").toString();
        if ( type == "dir" )  {
                xmlRepository = new SafetDirXmlRepository();
        }
#ifdef SAFET_DBXML   // Incluir la biblioteca dbxml para soporte de Base de Datos XML Nativa
        else if ( type == "dbxml" )  {
                xmlRepository = new SafetDbXmlRepository();

        }
#endif // Libreria DbXml
        Q_CHECK_PTR( xmlRepository );
        xmlRepository->openContainer(home + "/" + homefile);

        QString docName = QString(in) ;
        bool answer = false;
        answer = xmlRepository->searchDocumentInContainer(docName);
        QString archivoEscrito;

        if (answer) {
                archivoEscrito = xmlRepository->writeXmlFileFromDb(docName);
                SafetYAWL::streamlog <<  SafetLog::Debug << QObject::tr("Archivo digidoc escrito en disco: %1").arg(archivoEscrito);
        }
        else {
                return false;
        }
        SafetDocument doc;

        doc.initializeLibdigidoc();

        doc.createOpenXAdESContainer();

        // ************* prueba para abrir el archivo de configuracion de digidoc ******
        //QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfilepath").toString();
        doc.initDigidocConfigStore(libdigidocConfigFile);


        // ************* prueba para leer un contenedor desde un archivo en el sistema de archivos*************

        doc.readOpenXAdESContainerFile(archivoEscrito);
	QString mysigners = QString(signerNames);
        QStringList signers = mysigners.split(",");

        SafetYAWL::streamlog <<  SafetLog::Debug << QObject::tr("Iniciando verificacion de firmantes electronico...ok!");
        SafetYAWL::streamlog <<  SafetLog::Debug << QObject::tr("Firmantes: \"%1\"").arg(signers.join(","));
        SafetYAWL::streamlog <<  SafetLog::Debug << QObject::tr("Documento a verificar los firmantes dados: \"%1\"").arg(archivoEscrito);
        result = doc.verifySignMadeWithSmartCard(archivoEscrito, signers);
        SafetYAWL::streamlog <<  SafetLog::Action << QObject::tr("Resultado de la verificacion de firma: \"%1\"").arg(result?QObject::tr("..ok!"):QObject::tr("...fallo"));

        doc.closeLibdigidoc();
        return SOAP_OK;

        //******************** codigo Antonio
//	SafetDirXmlRepository repository;
//	QString home = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.path").toString();
//	QString homefile = SafetYAWL::getConfFile().getValue("Xml Repository", "xmlrepository.name").toString();
//	QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfilepath").toString();


//	if (repository.openContainer(home + "/" + homefile))
//	{
//		//SafetYAWL::streamlog <<  SafetLog::Debug << "ns__verifyDocumentSignatures: Contenedor abierto exitosamente";
//
//		// QString string, QString name
//		SafetYAWL::streamlog <<  SafetLog::Debug << QObject::tr("Nombre del archivo de firma: \"%1\"").arg(home + "/" + homefile + "/" +in);
//		SafetDocument doc;
//		doc.initializeLibdigidoc();
//    		doc.createOpenXAdESContainer();
//    		doc.initDigidocConfigStore(libdigidocConfigFile);
//
//		int res = doc.readOpenXAdESContainerFile(home + "/" + homefile + "/" +in);
//		if (res != ERR_OK)
//		{
//			result = false;
//			SafetYAWL::streamlog <<  SafetLog::Action << QObject::tr("No se pudo leer el documento firmado dentro del contenedor .");
//			return SOAP_OK;
//		}
//		QString signers = signerNames;
//		SafetYAWL::streamlog <<  SafetLog::Action << QObject::tr("Firmantes leidos: \"%1\"").arg(signerNames);
//
//		QStringList signerList = signers.split(":");
//		//SafetYAWL::streamlog <<  SafetLog::Debug << signerList.at(0).toLocal8Bit().constData();
//		//SafetYAWL::streamlog <<  SafetLog::Debug << signerList.at(1).toLocal8Bit().constData();
//		QString archivoEscrito = home + "/" + homefile + "/" +in;
//                SafetYAWL::streamlog <<  SafetLog::Debug << QObject::tr("Documento a verificar los firmantes (Servicio Web): \"%1\"").arg(archivoEscrito);
//		if (doc.verifySignMadeWithSmartCard(home + "/" + homefile + "/" +in, signerList))
//    		{
//			doc.closeLibdigidoc();
//			result = true;
//			SafetYAWL::streamlog <<  SafetLog::Action << QObject::tr("Verificacion de firmas del documento correcta...ok!.");
//			SafetYAWL::streamlog <<  SafetLog::Debug << QObject::tr("Servicio Web verifyDocumentSignatureWithNames terminado correctamente.");
//			return SOAP_OK;
//    		}
//    		else
//    		{
//			SafetYAWL::streamlog <<  SafetLog::Debug << QObject::tr("La verificacion de firmas del documento ubicado en \"%1\" NO fue exitosa").arg(home + "/"+ homefile + "/"+ in);
//			doc.closeLibdigidoc();
//			result = false;
//			return SOAP_OK;
//    		}
//	}
//	else // no se pudo abrir el contenedor que tiene el documento a verificar
//	{
//		result = false;
//		SafetYAWL::streamlog <<  SafetLog::Error << QObject::tr("No se pudo abrir el contenedor que tiene el documento a verificar.");
//		return SOAP_OK;
//	}
//	//SafetYAWL::streamlog <<  SafetLog::Debug << "Servicio Web verifyDocumentSignatures terminado correctamente.";
//	//return SOAP_OK;
//	SafetYAWL::streamlog <<  SafetLog::Debug << "*-*-*-*-*- NO DEBERIA VER ESTA LINEA";
return SOAP_OK;
}



// *****************************************************************************************************************************

// ns__authenticateUser es una funcion que se utiliza para realizar la autenticacion de usuarios ante el servidor de 
// documentos firmados. La idea es que un usuario se autentica si:
// i) se verifica la firma electronica de un documento validado por el servidor OCSP y
// ii) el nombre comun del firmante aparece en el archivo de lista de control de acceso de tickets con permiso en true
//
// A esta funcion se le pasa una cadena que representa un documento firmado segun el estandar XAdES.
//
// in: cadena que representa el contenido del documento firmado pasado como argumento
// result: booleano donde se retorna el resultado de la funcion: true si el usuario pasa el proceso de autenticacion
int ns__authenticateUser(struct soap * soap, xsd__string in, xsd__boolean &result)
{

	SafetYAWL yawl;
	SafetYAWL::setEvalExit( Foo::evalEventOnExit );
	SafetYAWL::streamlog <<  SafetLog::Action << "Servicio Web de autenticacion de usuario iniciado correctamente...OK!";

	SafetDocument doc;
	SafetYAWL::streamlog <<  SafetLog::Debug << in;
	doc.writeFileToDisk(in, ".file");

	// a continuacion se verifica la firma del documento para autenticar al usuario
		if (doc.verifySignMadeWithSmartCard("/tmp/.file"))
    		{
			SafetYAWL::streamlog <<  SafetLog::Action << "La verificacion de las firmas del documento es correcta. Autenticacion de usuario exitosa" ;
			//qDebug("la verificacion fue correcta, no hubo errores");

			// una vez verificada la firma se debe revisar la ACL de tickets para conocer el permiso del usuario
			QString strAclFile = SafetYAWL::getConfFile().getValue("ACL", "acl.file").toString();
			//qDebug("valor de aclfile:");
			//qDebug(qPrintable(strAclFile));
			
			doc.readOpenXAdESContainerFile("/tmp/.file");

			QString signerCN = doc.getCN(0);
			//qDebug("valor de signerCN:");
			//qDebug(qPrintable(signerCN));

			//QFile aclfile(strAclFile);
			QFile file(strAclFile);
     			if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         			result = false;

     			QTextStream in(&file);
     			while (!in.atEnd()) {
     				QString line = in.readLine();
				//qDebug(qPrintable(line));
				if (line.startsWith("#"))
					continue;
				if (line.contains(signerCN))
				{
					QString value = line.section("=", 1);
					if (value.contains("true", Qt::CaseInsensitive))
					{
						result = true;
						//qDebug(qPrintable(value));
						qDebug("Autenticacion de usuario exitosa.");
					}
					else
						result = false;
				}
     			}

		}
    		else
    		{
			SafetYAWL::streamlog <<  SafetLog::Error << "La verificacion de las firmas del documento NO es correcta. Falla la autenticacion de usuario" ;
			qDebug("la verificacion fue INCORRECTA, HUBO errores");
			//return false;
			result = false;
    		}
	QFile::remove("/tmp/.file");

	SafetYAWL::streamlog <<  SafetLog::Action << "Servicio Web de autenticacion de usuario finalizado!";
	//result = true;
	return SOAP_OK;

}


// *****************************************************************************************************************************

// ns__retrieveServerDbConfig es una funcion que se utiliza para obtener la cadena de configuración de la base de datos
// del servidor que mantiene la fuente de datos del flujo de trabajo
//
// 
//
// recipientCN: cadena que representa el nombre comun del solicitante
// configstring: cadena cifrada de acceso a la fuente de datos
int ns__retrieveServerDbConfig(struct soap * soap, xsd__string recipientCN, xsd__string &configstring)
{

 	SafetYAWL yawl;
 	SafetYAWL::setEvalExit( Foo::evalEventOnExit );
 	SafetYAWL::streamlog <<  SafetLog::Action << "Servicio Web de retrieveServerDbConfig iniciado correctamente...OK!";
	
	DEncEncryptedData* ppEncData = 0;
	SafetDocument docTest;
	docTest.initializeLibdigidoc();
	docTest.createOpenXAdESContainer();

	// obtener el certificado del usuario solicitante para cifrar la cadena
	QString certificateRepository = SafetYAWL::getConfFile().getValue("X509", "x509.repositoryname").toString();

	QString recipientCertificatePath = docTest.searchCommonNameInCertificates((QString) recipientCN, certificateRepository);
	
	SafetYAWL::evalAndCheckExit(!recipientCertificatePath.isEmpty(), QObject::tr("retrieveServerDbConfig: la ruta al certificado esta vacia."));

	// obtener la cadena de acceso a la base de datos
	QString dbUser = SafetYAWL::getConfFile().getValue("Database", "database.user.1").toString();
	QString passwd = SafetYAWL::getConfFile().getValue("Database", "database.password.1").toString();
	QString stringToEncrypt;

	stringToEncrypt.append(dbUser);
	stringToEncrypt.append(":");
	stringToEncrypt.append(passwd);
	
	//stringToEncrypt = "pronto se dara el viaje para brasil";

	// obtener un nombre de archivo temporal para almacenar los datos cifrados
	QStringList list = docTest.getTempNameFiles(1);
	QString tmpFile = list.at(0);
	
	// establecer el tipo mime del archivo cifrado
	QString mimeType = "text/xml";

	// cifrar la cadena de caracteres para acceso a la base de datos
	docTest.encryptString(&ppEncData, stringToEncrypt, recipientCN, "symmetricKey", recipientCertificatePath, tmpFile, mimeType);

	// convertir el archivo con los datos cifrados a una cadena
	QString encryptedString = docTest.returnFileToString(tmpFile); // <---cadena que se debe retornar
	SafetYAWL::evalAndCheckExit(!encryptedString.isEmpty(), QObject::tr("retrieveServerDbConfig: cadena de configuracion cifrada esta vacia."));
	int size = encryptedString.size();

	// eliminar el archivo con el contenido cifrado
	QFile fileToErase(tmpFile);
	SafetYAWL::evalAndCheckExit(fileToErase.remove(), QObject::tr("retrieveServerDbConfig: El archivo temporal cifrado no se pudo borrar."));

	// retornar la cadena cifrada 
	configstring = new char [size];
	strcpy(configstring, qPrintable(encryptedString));
	
	dbUser.clear();
	passwd.clear();
	stringToEncrypt.clear();

	qDebug("ns__retrieveServerDbConfig: Se realizo el cifrado de la cadena correctamente");

	return SOAP_OK;

}



