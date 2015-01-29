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


#include "SafetDbXmlRepository.h"
#include "SafetYAWL.h"

using namespace DbXml;

SafetDbXmlRepository::SafetDbXmlRepository(const QString& home){

	//Q_ASSERT_X(!home.isEmpty(), "SafetDbXmlRepository::SafetDbXmlRepository()", qPrintable(tr("home is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!home.isEmpty() , 
	//	tr("No existe o no se especifico la ruta del directorio para el ambiente dbxml")) );
	SafetYAWL::evalAndCheckExit(!home.isEmpty(), tr("No existe o no se especifico la ruta del directorio para el ambiente dbxml"));

	u_int32_t env_flags = DB_CREATE     |  // If the environment does not
        	                               // exist, create it.
                	      DB_INIT_LOCK  |  // Initialize the locking subsystem
                	      DB_INIT_LOG   |  // Initialize the logging subsystem
	                      DB_INIT_MPOOL |  // Initialize the cache
	                      DB_INIT_TXN;     // Initialize transactions


	myEnv = new DbEnv(0);
        myManager = NULL;
        envHome = home;

	try {	
		qDebug("qPrintable(home): %s", qPrintable(home));
		myEnv->open(qPrintable(home), env_flags, 0);
		qDebug("antes de new XmlManager");
        	myManager = new XmlManager(myEnv, DBXML_ADOPT_DBENV); 
						// The manager and 
                                               // environment is closed 
                                               // when this object is 
                                               // destroyed.
		qDebug("qPrintable(home): %s", qPrintable(home));
		qDebug("INICIALIZANDO: %d", myManager);
    	} catch(DbException &e) {
        	std::cerr << "Error opening database environment: "
                	  << qPrintable(envHome) << std::endl;
        	std::cerr << e.what() << std::endl;
		SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - DbException: %1").arg(e.what());
    	} catch (XmlException &xe) {
                // Error handling goes here
        	std::cerr << "Error opening database environment: "
                	  << qPrintable(envHome)
                  	<< " or opening XmlManager." << std::endl;
        	std::cerr << xe.what() << std::endl;
		SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(xe.what());
    	}



}


SafetDbXmlRepository::~SafetDbXmlRepository(){


	try {
        	if (myManager != NULL) {
            		delete myManager;
        	}
    	} catch(DbException &e) {
        	std::cerr << "Error closing database environment: "
                	<< qPrintable(envHome) << std::endl;
        	std::cerr << e.what() << std::endl;
		SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - DbException: %1").arg(e.what());
    	} catch(XmlException &xe) {
        	std::cerr << "Error closing database environment: "
	                << qPrintable(envHome) << std::endl;
	        std::cerr << xe.what() << std::endl;
		SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(xe.what());
    	}


}


XmlManager * SafetDbXmlRepository::getXmlManager(){
	return myManager;
}


bool SafetDbXmlRepository::createContainer(const QString& containerName, const QString& containerType){

//<<<<<<< .mine
	//Q_ASSERT_X(!containerName.isEmpty(), "SafetDbXmlRepository::createContainer()", qPrintable(tr("containerName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerName.isEmpty() , tr("No existe o no se especifico el nombre para el contenedor dbxml")) );
	SafetYAWL::evalAndCheckExit(!containerName.isEmpty() , tr("No existe o no se especifico el nombre para el contenedor dbxml"));

	//Q_ASSERT_X(!containerType.isEmpty(), "SafetDbXmlRepository::createContainer()", qPrintable(tr("containerType is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerType.isEmpty() , tr("No existe o no se especifico el tipo de contenedor dbxml")) );
	SafetYAWL::evalAndCheckExit(!containerType.isEmpty() , tr("No existe o no se especifico el tipo de contenedor dbxml"));
	
	if (containerType == "XmlContainer::WholedocContainer")

 	qDebug("create container");
	{
		try{
			myManager->setDefaultContainerType(XmlContainer::WholedocContainer);
			qDebug("\tsetDefaultContainerType: WholedocContainer ...ok");
		}
		catch(XmlException &xe){
			std::cerr << "Error setting default container type: " << qPrintable(containerName) << std::endl;
	        	std::cerr << xe.what() << std::endl;
			SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(xe.what());
		}
	}
	
	Q_CHECK_PTR( myManager );
	if (containerType == "XmlContainer::NodeContainer")
	{
		qDebug("(2) %d", myManager);
		try{
			myManager->setDefaultContainerType(XmlContainer::NodeContainer);
			qDebug("\tsetDefaultContainerType: NodeContainer ...ok");
		}
		catch(XmlException &e){
			std::cerr << "Error setting default container type: " << qPrintable(containerName) << std::endl;
	        	std::cerr << e.what() << std::endl;
			SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(e.what());
		}
	}
	
	try{
		qDebug("\tANTES DE myManager->createContainer");
		container = myManager->createContainer(qPrintable(containerName));
	}
	catch(XmlException &xe){
		std::cerr << "Error creating container: " << qPrintable(containerName) << std::endl;
	       	std::cerr << xe.what() << std::endl;
		SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(xe.what());
		return false;
	}
	return true;
}

bool SafetDbXmlRepository::openContainer(const QString& containerName){
	//Q_ASSERT_X(!containerName.isEmpty(), "SafetDbXmlRepository::openContainer()", qPrintable(tr("containerName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerName.isEmpty() , tr("No existe o no se especifico el nombre del contenedor dbxml a abrir")) );
	SafetYAWL::evalAndCheckExit(!containerName.isEmpty() , tr("No existe o no se especifico el nombre del contenedor dbxml a abrir"));
	
	try{
		container = myManager->openContainer(qPrintable(containerName));
	}
	catch(XmlException &xe){
		std::cerr << "Error opening container: " << qPrintable(containerName) << std::endl;
	        std::cerr << xe.what() << std::endl;
		SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(xe.what());
		return false;
	}
	container.addAlias("mycontainer");
	return true;
}


void SafetDbXmlRepository::deleteContainer(const QString& containerName){
	//Q_ASSERT_X(!containerName.isEmpty(), "SafetDbXmlRepository::deleteContainer()", qPrintable(tr("containerName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerName.isEmpty() , tr("No existe o no se especifico el nombre del contenedor dbxml a borrar")) );
	SafetYAWL::evalAndCheckExit(!containerName.isEmpty() , tr("No existe o no se especifico el nombre del contenedor dbxml a borrar"));

	try{
		myManager->removeContainer(qPrintable(containerName));
	}
	catch(XmlException &xe){
		std::cerr << "Error deleting container: " << qPrintable(containerName) << std::endl;
	        std::cerr << xe.what() << std::endl;
		SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(xe.what());
	}
}

void SafetDbXmlRepository::renameContainer(const QString& containerName, const QString& newContainerName){
	//Q_ASSERT_X(!containerName.isEmpty(), "SafetDbXmlRepository::renameContainer()", qPrintable(tr("containerName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerName.isEmpty() , 
	//	tr("No existe o no se especifico el nombre del contenedor dbxml a renombrar")) );
	SafetYAWL::evalAndCheckExit(!containerName.isEmpty(),tr("No existe o no se especifico el nombre del contenedor dbxml a renombrar"));


	//Q_ASSERT_X(!newContainerName.isEmpty(), "SafetDbXmlRepository::renameContainer()", qPrintable(tr("newContainerName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!newContainerName.isEmpty() , tr("No existe o no se especifico el nuevo nombre del contenedor dbxml")) );
	SafetYAWL::evalAndCheckExit(!newContainerName.isEmpty(), tr("No existe o no se especifico el nuevo nombre del contenedor dbxml"));

	try{
		myManager->renameContainer(qPrintable(containerName), qPrintable(newContainerName));
	}
	catch(XmlException &xe){
		std::cerr << "Error renaming container: " << qPrintable(containerName) << std::endl;
	        std::cerr << xe.what() << std::endl;
		SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(xe.what());
	}
}



XmlUpdateContext SafetDbXmlRepository::createContextUpdate(){
	return myManager->createUpdateContext();
}


void SafetDbXmlRepository::addXmlDocumentToContainer(const QString& physicalName, const QString& docName){
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!physicalName.isEmpty() , tr("No existe o no se especifico el nombre del contenedor XML")) );
	SafetYAWL::evalAndCheckExit(!physicalName.isEmpty() , tr("No existe o no se especifico el nombre del contenedor XML"));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() , tr("No existe o no se especifico el nombre del documento dentrol del contenedor XML. Revise nombre de variables o claves")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(), 
		tr("No existe o no se especifico el nombre del documento dentrol del contenedor XML. Revise nombre de variables o claves"));

	// Need an update context for the put.
	//XmlUpdateContext theContext = myManager->createUpdateContext();	
	try {
        	// Get the input stream.
        	XmlInputStream *theStream = myManager->createLocalFileInputStream(qPrintable(physicalName));

        	// Put the document
        	container.putDocument(qPrintable(docName),      // The document's name
                	                 theStream,   // The actual document. 
                        	         theContext,  // The update context 
                                	              // (required).
	                                 0);          // Put flags.
    	} catch (XmlException &e) {
	        // Error handling goes here. You may want to check
	        // for XmlException::UNIQUE_ERROR, which is raised
	       // if a document with that name already exists in
	        // the container. If this exception is thrown, 
	        // try the put again with a different name, or
	        // use XmlModify to update the document.
		
		if ( e.getExceptionCode() == XmlException::UNIQUE_ERROR )
		{
			SafetYAWL::streamlog <<  SafetLog::Warning << tr("Libdbxml - XmlException: ya existe un documento en el contenedor con el nombre: %1").arg(docName);
		}else{
			SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(e.what());
		}
	}

}


QString SafetDbXmlRepository::getXmlDocument(const QString& docName){
	//Q_ASSERT_X(!docName.isEmpty(), "SafetDbXmlRepository::getXmlDocument()", qPrintable(tr("docName is Empty")));
	QString str;
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() , 
	//	tr("No se especifico el nombre del documento a extraer del contenedor XML")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(),tr("No se especifico el nombre del documento a extraer del contenedor XML"));


	document = container.getDocument(qPrintable(docName));
	return str;
}



QString SafetDbXmlRepository::writeXmlFileFromDb(const QString& docName){
	//Q_ASSERT_X(!docName.isEmpty(), "SafetDbXmlRepository::getXmlDocument()", qPrintable(tr("docName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() , 
	//	tr("No se especifico el nombre del documento XML a escribir a partir de la base de datos")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(),tr("No se especifico el nombre del documento XML a escribir a partir de la base de datos"));

	getXmlDocument(docName);
	XmlData content = document.getContent();

	QString fileName = "/tmp/";
	fileName = fileName+docName;
	fileName = fileName+".ddoc";

	QFile file(fileName);
	bool result = file.open(QIODevice::WriteOnly | QIODevice::Text);
	Q_ASSERT_X(result == true, "SafetDbXmlRepository::writeXmlFileFromDb()", qPrintable(tr("Could not open a tmp file")));
	
	QTextStream out(&file);

	//u_int32_t XmlData::get_size() const
	uint size = (uint) content.get_size();
//	qDebug("size: %d", size);

        out.setCodec("ISO 8859-1");
        //out.setCodec("UTF-8");
	//out << (const char *) content.get_data();
	QString x = (const char *) content.get_data();
	x = x.left(size);
	out << x;

	return fileName;
}


bool SafetDbXmlRepository::searchDocumentInContainer(const QString& docName){
	//Q_ASSERT_X(!docName.isEmpty(), "SafetDbXmlRepository::searchDocumentInContainer()", qPrintable(tr("docName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() , 
	//	tr("No se especifico el nombre del documento XML a buscar en el contenedor")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(),tr("No se especifico el nombre del documento XML a buscar en el contenedor"));
	
	// creacion de un alias del contenedor para que no ocurran problemas al momento de construir
	// un contexto de consulta
	container.addAlias("mycontainer");

	// consulta: se debe especificar la ruta desde donde comenzar la busqueda!!!!
	// devuelve todos los documentos cuyo nodo raiz es SignedDoc
	QString myQuery = "collection('mycontainer')/SignedDoc";

	// obtener el contexto para la consulta de documentos
	XmlQueryContext managerContext = getXmlManager()->createQueryContext();

	// crear un espacio de nombres asociado al contexto para la consulta de documentos
	// FIXME el espacio de nombres que se esta utilizando aqui es el que se define para 
	// la etiqueta SignedDoc de libdigidoc
	managerContext.setNamespace("", "http://www.sk.ee/DigiDoc/v1.3.0#");

	// preparar la consulta de documentos
	XmlQueryExpression managerQuery = getXmlManager()->prepare(qPrintable(myQuery), managerContext);

	// ejecutar la consulta para busqueda de documentos
	XmlResults results = managerQuery.execute(managerContext, 0);

//	qDebug("SafetDbXmlRepository::searchDocumentInContainer: se encontraron: %d documento(s) para la consulta", results.size());
// 	if(results.size()>0)
// 	{
// 		qDebug("SafetDbXmlRepository::searchDocumentInContainer: results.hasNext() !!!");
// 		
// 		
// 	}
// 	else
// 	{
// 		qDebug("SafetDbXmlRepository::searchDocumentInContainer NO results.hasNext()");
// 	}

	QString docString("");
	XmlValue value;
	
	while(results.next(value)){
		XmlDocument theDoc = value.asDocument();
		docString = theDoc.getName().c_str();

//		qDebug("docName: %s", qPrintable(docName));
//		qDebug("docString: %s", qPrintable(docString));
		
		if (docString == docName)
		{
//			qDebug("NOMBRE DEL DOCUMENTO ENCONTRADO: %s", qPrintable(docString));

			return true;
		}
	}  // fin del while
	return false;

}



int SafetDbXmlRepository::getSignatureStatus(const QString& docName,const QString& CN){
	//Q_ASSERT_X(!docName.isEmpty(), "SafetDbXmlRepository::getSignatureStatus()", qPrintable(tr("docName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() , 
	//	tr("No se especifico el nombre del documento XML para obtener el estatus de firma")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(),tr("No se especifico el nombre del documento XML para obtener el estatus de firma"));

	//Q_ASSERT_X(!CN.isEmpty(), "SafetDbXmlRepository::getSignatureStatus()", qPrintable(tr("CN is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!CN.isEmpty() , 
	//	tr("No se especifico el CN del firmante del documento XML para obtener el estatus de firma")) );
	SafetYAWL::evalAndCheckExit(!CN.isEmpty(),tr("No se especifico el CN del firmante del documento XML para obtener el estatus de firma"));


	// busqueda del documento en el contenedor
	bool exist = searchDocumentInContainer(docName);
	if (!exist)
		return SAFET_DOCUMENT_NOT_FOUND;
	
	// escribir el contenedor en un archivo temporal /tmp/
	QString tmpFile = writeXmlFileFromDb(docName);
	
	// buscar firma del documento docName con el CN dado
	SafetDocument doc;
	
	doc.createOpenXAdESContainer();
	
	// ojo se debe definir si la inicializacion de la libreria se va a hacer aqui o
	// antes de llamar a la funcion SafetDbXmlRepository::getSignatureStatus
	//doc.initializeLibdigidoc();
	//doc.initDigidocConfigStore("/usr/local/libdigidocModificada/etc/digidoc.conf");

	doc.readOpenXAdESContainerFile(tmpFile);

	int index = doc.getSignerIndex(CN);
	
    	if (index == -1)
    	{
		// el documento encontrado NO esta firmado por CN
//		qDebug("NO SE ENCONTRO NINGUNA FIRMA CON CN: %s en el contenedor digidoc", qPrintable(CN));
		return SAFET_DOCUMENT_NOT_SIGNED; // ojo -> este estado no se debe retornar
	}
	else
	{
		// el documento encontrado SI esta firmado por el CN
//		qDebug("SI SE ENCONTRO UNA FIRMA CON CN: %s en el contenedor digidoc", qPrintable(CN));
		//return SAFET_DOCUMENT_SIGNED;

		// ahora se debe verificar la firma del documento
    		if (doc.verifySignMadeWithSmartCard(tmpFile, index))
    		{
//			qDebug("la verificacion fue correcta, no hubo errores");
			return SAFET_DOCUMENT_SIGNATURE_VALID;
    		}
    		else
    		{
//			qDebug("la verificacion fue INCORRECTA, HUBO errores");
			return SAFET_DOCUMENT_SIGNATURE_INVALID;
    		}

	}

	return 0;
}


bool SafetDbXmlRepository::deleteXmlDocument(const QString& docName) {
	//Q_ASSERT_X(!docName.isEmpty(), "SafetDbXmlRepository::deleteXmlDocument()", qPrintable(tr("docName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() , 
	//	tr("No se especifico el nombre del documento XML a eliminar del contenedor")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(),tr("No se especifico el nombre del documento XML a eliminar del contenedor")) );

	bool search = searchDocumentInContainer(docName);
	if (search)
	{
		XmlUpdateContext theContext = myManager->createUpdateContext();
		try {
	        	container.deleteDocument(qPrintable(docName), theContext);

    		} catch (XmlException &e) {
		        // Error handling goes here. You may want to check
	        	// for XmlException::UNIQUE_ERROR, which is raised
		       // if a document with that name already exists in
		        // the container. If this exception is thrown, 
		        // try the put again with a different name, or
		        // use XmlModify to update the document.
			SafetYAWL::streamlog <<  SafetLog::Error << tr("Libdbxml - XmlException: %1").arg(e.what());
		}
		return true;
	}
	return false;
}






