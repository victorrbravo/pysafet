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

#include "SafetDirXmlRepository.h"


SafetDirXmlRepository::SafetDirXmlRepository()
 : SafetXmlRepository()
{
	
	//obtener la ruta del directorio asociado al repositorio
        QString path = SafetYAWL::getConfFile().getValue("XmlRepository", "xmlrepository.path").toString();
//	qDebug(qPrintable(path));

	//setEnvHome(path);
	internalContainerName = "";

	QDir dir(path);


	SafetYAWL::streamlog <<  SafetLog::Debug << QString::number(dir.exists());

	
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(dir.exists(),
	//	tr("No existe el directorio asociado al repositorio. Verifique el archivo de configuracion safet.conf.")) );
	SafetYAWL::evalAndCheckExit(dir.exists(), tr("No existe el directorio: \"%1\" asociado al repositorio. Verifique el archivo de configuracion safet.conf.").arg(path));

        SafetYAWL::streamlog <<  SafetLog::Debug
                << "Ambiente del repositorio configurado exitosamente!";
}


SafetDirXmlRepository::~SafetDirXmlRepository()
{
}


bool SafetDirXmlRepository::createContainer(const QString& containerName, const QString& containerType) {
	//SafetYAWL::streamlog <<  SafetLog::Debug << containerName ;
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerName.isEmpty() , tr("No se especifico el nombre del contenedor a crear.")) );
	SafetYAWL::evalAndCheckExit(!containerName.isEmpty(),tr("No se especifico el nombre del contenedor a crear."));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerType.isEmpty() , tr("No se especifico el tipo del contenedor a crear.")) );
	SafetYAWL::evalAndCheckExit(!containerType.isEmpty() , tr("No se especifico el tipo del contenedor a crear."));


//	QString containerPath = getEnvHome()+"/"+containerName;
	QString containerPath = containerName;
//	qDebug("containerPath: %s", qPrintable(containerPath));
	QDir container(containerPath);
	//SafetYAWL::streamlog <<  SafetLog::Debug << "containerPath: " << containerPath << "\n";

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!container.exists(),tr("El contenedor ya existe en el repositorio.")) );
	SafetYAWL::evalAndCheckExit(!container.exists(), tr("El contenedor ya existe en el repositorio."));

	if (container.mkdir(containerPath))
	{
		SafetYAWL::streamlog <<  SafetLog::Action << "Contenedor creado exitosamente en el repositorio." ;
		internalContainerName = containerName;
		return true;
	}
	else
	{
		SafetYAWL::streamlog <<  SafetLog::Warning << "Contenedor ya existe en el repositorio; no se puede crear." ;
		//SafetYAWL::streamlog <<  SafetLog::Debug << containerName ;
		//SafetYAWL::streamlog <<  SafetLog::Debug << containerPath ;
		return false;
	}

}


bool SafetDirXmlRepository::openContainer(const QString& containerName) {
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerName.isEmpty() , tr("No se especifico el nombre del contenedor a abrir.")) );
        if ( containerName.isEmpty() ) {
            SafetYAWL::streamlog
                    << SafetLog::Error
                    << tr("No se especifico el nombre del contenedor a abrir.");
            return false;
        }

	QString containerPath = containerName;

	QDir container(containerPath);

	if (container.exists())
	{
                //SafetYAWL::streamlog <<  SafetLog::Action << tr("Contenedor abierto exitosamente.") ;
		internalContainerName = containerPath;
                _isopencontainer = true;
		return true;
	}
	else
	{
            SafetYAWL::streamlog <<  SafetLog::Debug
                    << tr("No se pudo abrir el contenedor con nombre: %1").arg(containerPath) ;
		return false;
	}
	
}


void SafetDirXmlRepository::deleteContainer(const QString& containerName) {
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerName.isEmpty() , tr("No se especifico el nombre del contenedor a borrar.")) );
	SafetYAWL::evalAndCheckExit(!containerName.isEmpty() , tr("No se especifico el nombre del contenedor a borrar."));

//	QString containerPath = getEnvHome()+"/"+containerName;
	//QString containerPath = containerName;
	QString containerPath = containerName.section('/', -1); 
	QDir container(containerPath);
	container.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	if (container.exists())
	{
		//obtener el numero de archivos existentes dentro del contenedor
		QFileInfoList list = container.entryInfoList();

		for (int i=0; i<list.size(); i++)
		{
			QFileInfo fileInfo = list.at(i);
			container.remove(fileInfo.fileName());

		}
		if (container.rmdir(containerPath))
		{
			SafetYAWL::streamlog <<  SafetLog::Action << "Contenedor borrado exitosamente." ;
			internalContainerName = "";
		}
		else
		{
			SafetYAWL::streamlog <<  SafetLog::Action << "No se pudo borrar el contenedor porque no esta vacio." ;
		}
	}
	else
	{
		SafetYAWL::streamlog <<  SafetLog::Warning << "No se pudo borrar el contenedor porque no existe." ;
		
	}
	return;
}

void SafetDirXmlRepository::renameContainer(const QString& containerName, const QString& newContainerName) {
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerName.isEmpty() , tr("No se especifico el nombre del contenedor a renombrar.")) );
	SafetYAWL::evalAndCheckExit(!containerName.isEmpty() , tr("No se especifico el nombre del contenedor a renombrar."));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!containerName.isEmpty() , tr("No se especifico el nuevo nombre del contenedor.")) );
	SafetYAWL::evalAndCheckExit(!containerName.isEmpty() , tr("No se especifico el nuevo nombre del contenedor."));
	
//	QString containerPath = getEnvHome()+"/"+containerName;
	QString containerPath = containerName;
//	QString containerPathRenamed = getEnvHome()+"/"+newContainerName;
	QString containerPathRenamed = newContainerName;
	QDir container(containerPath);
	container.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	
	if (container.exists())
	{
		if (container.rename(containerPath, containerPathRenamed))
		{
			SafetYAWL::streamlog <<  SafetLog::Action << tr("Se renombro el contenedor exitosamente!.") ;
			internalContainerName = newContainerName;
		}
		else
		{
			SafetYAWL::streamlog <<  SafetLog::Warning << tr("No se pudo renombrar el contenedor.") ;
		}
	}
	else
	{
		SafetYAWL::streamlog <<  SafetLog::Warning << tr("No se pudo renombrar el contenedor porque no existe!.") ;
	}
	return;
}



void SafetDirXmlRepository::addXmlDocumentToContainer(const QString& physicalName, const QString& docName) {
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!physicalName.isEmpty() ,
	//	tr("No se especifico la ruta del archivo a agregar al contenedor.")) );
	SafetYAWL::evalAndCheckExit(!physicalName.isEmpty(), tr("No se especifico la ruta del archivo a agregar al contenedor."));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() ,
	//	tr("No se especifico el nombre que tendra el archivo XML dentro del contenedor.")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(),tr("No se especifico el nombre que tendra el archivo XML dentro del contenedor."));
	
//	QString containerPath = getEnvHome()+"/"+internalContainerName;
	QString containerPath = internalContainerName;
//	qDebug("containerPath: %s", qPrintable(containerPath));
	QDir container(containerPath);
	container.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	
	if (container.exists())
	{
	/*	//buscar si ya existe el archivo dentro del contenedor
		QFileInfoList list = container.entryInfoList();
		for (int i=0; i<list.size(); i++)
		{
			QFileInfo fileInfo = list.at(i);
         		std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()));
         		std::cout << std::endl;
			if (fileInfo.fileName() == docName)
			{
				SafetYAWL::streamlog <<  SafetLog::Error << "No se puede insertar archivo en el contenedor. Ya existe un archivo XML con el mismo nombre!." ;
				return;
			}
		}
	*/
		//agregar el archivo al contenedor
		QString docNamePath = containerPath+"/"+docName;
		qDebug(qPrintable(docNamePath));
		if (QFile::exists(physicalName))
		{
			
			if ( QFile::exists(docNamePath) ) {
				bool isremove = QFile::remove(docNamePath);
				if (!isremove ) 
				SafetYAWL::streamlog <<  SafetLog::Warning << tr("Fallo la eliminacion del archivo: \"%1\" del contenedor: %2").arg(docNamePath).arg(containerPath);
				SafetYAWL::streamlog <<  SafetLog::Warning << "El archivo a insertar ya existe en el contenedor, se sobreescribira." ;
			}
			if (QFile::copy(physicalName, docNamePath))
			{
				SafetYAWL::streamlog <<  SafetLog::Action << "Se agrego el archivo XML en el contenedor exitosamente!." ;
			}
			else
			{
				SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo agregar el archivo XML en el contenedor. Ya existe otro con el mismo nombre!." ;
			}
		}
		else
		{
			SafetYAWL::streamlog <<  SafetLog::Error << "No existe el archivo a agregar en el contenedor!." ;
		}
	}
	else
	{
		SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo insertar el archivo XML; el contenedor no existe!." ;
	}
	return;
}



QString SafetDirXmlRepository::getXmlDocument(const QString& docName) {
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() ,
	//	tr("No se especifico el nombre del archivo XML a retornar como cadena.")) );
    if ( docName.isEmpty()) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("No se especifico el nombre del archivo XML a retornar como cadena.");
        return QString("");
    }


	QString containerPath = internalContainerName;
	QDir container(containerPath);
	container.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	
	if (container.exists())
	{

		QString docNamePath = containerPath+"/"+docName;
		if (!QFile::exists(docNamePath))
		{
                        SafetYAWL::streamlog <<  SafetLog::Debug
                                << tr("No existe el archivo XML en el contenedor: \"%1\"").arg(docNamePath) ;
			return "";
		}
		QFile file(docNamePath);
     		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
                        SafetYAWL::streamlog <<  SafetLog::Debug
                                << tr("No se tiene acceso de lectura para el archivo XML: \"%1\"").arg(docNamePath) ;
         		return "";
		}

     		QTextStream in(&file);
		QString line("");
     		while (!in.atEnd()) 
		{
         		line.append(in.readLine());
 			//qDebug(qPrintable(line));
     		}
		qDebug(qPrintable(line));
		SafetYAWL::streamlog <<  SafetLog::Debug << "Se retorno la cadena asociada al archivo XML exitosamente!." ;
		return line;
	}
	else
	{
		SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo retornar la cadena asociada al archivo XML; el contenedor no existe!." ;
	}
	return "";

}


QString SafetDirXmlRepository::writeXmlFileFromDb(const QString& docName) {
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() ,
	//	tr("No se especifico el nombre del documento XML a escribir a partir de la base de datos")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(), tr("No se especifico el nombre del documento XML a escribir a partir de la base de datos"));

	QString fileName = "/tmp/";
	fileName.append(docName);
        if (!docName.endsWith(".ddoc")) {
            fileName.append(".ddoc");
        }
	

//	QString containerPath = getEnvHome()+"/"+internalContainerName;
	QString containerPath = internalContainerName;
	QDir container(containerPath);
	container.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	
	if (container.exists())
	{
		QString docNamePath = containerPath+"/"+docName;
		
		if (!QFile::exists(docNamePath))
		{
                        SafetYAWL::streamlog <<  SafetLog::Warning <<
                                tr("No existe el archivo XML en el contenedor: \"%1\"").arg(docNamePath) ;
			return "";
		}
		if (QFile::exists(fileName))
		{
			QFile::remove(fileName);
		}
		if (QFile::copy(docNamePath, fileName))
		{
                        SafetYAWL::streamlog <<  SafetLog::Action
                                << tr("Se escribio el archivo XML \"%1\" en el directorio temporal exitosamente!.").arg(fileName) ;
			return fileName;
		}
		else
		{
			SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo escribir el archivo XML en el directorio temporal. Ya existe otro con el mismo nombre!." ;
			return "";
		}
	}
	else
	{
		SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo retornar la cadena asociada al archivo XML; el contenedor no existe!." ;
		return "";
	}
}


bool SafetDirXmlRepository::searchDocumentInContainer(const QString& docName){
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() ,
	//	tr("No se especifico el nombre del documento XML a buscar en el contenedor")) );



	QString containerPath = internalContainerName;

    SYD << tr(".........SafetDirXmlRepository::searchDocumentInContainer...containerPath:|%1|")
           .arg(containerPath);
    SYD << tr(".........SafetDirXmlRepository::searchDocumentInContainer...docName:|%1|")
           .arg(docName);

	QDir container(containerPath);
	container.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        if (_firstsearch ) {
            if ( _documentsmap.contains(docName) ) {
                return true;
            }
            return false;
        }
	
        if (!container.exists()) {
            return false;
        }

        _firstsearch = true;


        QFileInfoList list = container.entryInfoList();
        for(int i = 0; i < list.size(); i++) {
            _documentsmap[ list.at(i).fileName() ] = true;
        }

        if (!_documentsmap.contains(docName)) {
            return false;
	}
        SYD << tr(".........SafetDirXmlRepository::searchDocumentInContainer...RETURNING TRUE");

        return true;

}



int SafetDirXmlRepository::getSignatureStatus(const QString& docName, const QString& CN) {

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() ,
	//	tr("No se especifico el nombre del documento XML para obtener el estatus de firma")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(), tr("No se especifico el nombre del documento XML para obtener el estatus de firma"));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!CN.isEmpty() ,
	//	tr("No se especifico el CN del firmante del documento XML para obtener el estatus de firma")) );
	SafetYAWL::evalAndCheckExit(!CN.isEmpty(), tr("No se especifico el CN del firmante del documento XML para obtener el estatus de firma"));


	// busqueda del documento en el contenedor
	bool exist = searchDocumentInContainer(docName);
	if (!exist)
	{
		SafetYAWL::streamlog <<  SafetLog::Error << "No se encontro el archivo XML en el contenedor para conocer el estado de la firma." ;
		qDebug("fallo searchDocumentInContainer(docName)");
		return SAFET_DOCUMENT_NOT_FOUND;
	}

	// escribir el contenedor en un archivo temporal /tmp/
	QString tmpFile = writeXmlFileFromDb(docName);
	if (tmpFile.isEmpty())
	{
		SafetYAWL::streamlog <<  SafetLog::Error << "No se pudo completar el proceso para conocer el estado de la firma, error interno." ;
		qDebug("writeXmlFileFromDb(docName)");
		return SAFET_DOCUMENT_NOT_FOUND;
	}

	// buscar firma del documento docName con el CN dado
	SafetDocument doc;
	doc.createOpenXAdESContainer();
	doc.readOpenXAdESContainerFile(tmpFile);

	int index = doc.getSignerIndex(CN);

	if (index == -1)
    	{
		SafetYAWL::streamlog <<  SafetLog::Error << "No se encontro ninguna firma con el CN (nombre comun)." ;
		// el documento encontrado NO esta firmado por CN
		qDebug("NO SE ENCONTRO NINGUNA FIRMA CON CN: %s en el contenedor digidoc", qPrintable(CN));
		return SAFET_DOCUMENT_NOT_SIGNED; // ojo -> este estado no se debe retornar
	}
	else
	{
		SafetYAWL::streamlog <<  SafetLog::Debug << "Se encontro una firma que coincide con el CN (nombre comun) dado." ;
		// el documento encontrado SI esta firmado por el CN
		qDebug("SI SE ENCONTRO UNA FIRMA CON CN: %s en el contenedor digidoc", qPrintable(CN));
		//return SAFET_DOCUMENT_SIGNED;

		// ahora se debe verificar la firma del documento
    		if (doc.verifySignMadeWithSmartCard(tmpFile, index))
    		{
			SafetYAWL::streamlog <<  SafetLog::Action << "La verificacion de la firma con el CN (nombre comun) dado es valida." ;
			qDebug("la verificacion fue correcta, no hubo errores");
			return SAFET_DOCUMENT_SIGNATURE_VALID;
    		}
    		else
    		{
			SafetYAWL::streamlog <<  SafetLog::Error << "La verificacion de la firma con el CN (nombre comun) dado NO es valida." ;
			qDebug("la verificacion fue INCORRECTA, HUBO errores");
			return SAFET_DOCUMENT_SIGNATURE_INVALID;
    		}
	}
	return 0;
}


bool SafetDirXmlRepository::deleteXmlDocument(const QString& docName)  {
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!docName.isEmpty() ,
	//	tr("No se especifico el nombre del documento XML a eliminar del contenedor")) );
	SafetYAWL::evalAndCheckExit(!docName.isEmpty(), tr("No se especifico el nombre del documento XML a eliminar del contenedor"));


//	QString containerPath = getEnvHome()+"/"+internalContainerName;
	QString containerPath = internalContainerName;
	QDir container(containerPath);
	container.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	
	if (container.exists())
	{
		QString docNamePath = containerPath+"/"+docName;
		qDebug(qPrintable(docNamePath));
		if (!QFile::exists(docNamePath))
		{
                    SafetYAWL::streamlog <<  SafetLog::Debug << "No existe el archivo XML a borrar en el contenedor!." ;
			return false;
		}
		else
		{
			if (QFile::remove(docNamePath))
			{
				SafetYAWL::streamlog <<  SafetLog::Action << "Se borro el archivo XML del contenedor exitosamente." ;
				return true;
			}
			else
			{
				SafetYAWL::streamlog <<  SafetLog::Error << "No se borro el archivo XML del contenedor." ;
				return false;
			}
		}
	}
	else
	{
		SafetYAWL::streamlog <<  SafetLog::Error << "No se encontro el archivo XML a borrar en el contenedor." ;
		return false;
	}


	return true;
}
