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

#ifndef SAFETDOCUMENT_H_
#define SAFETDOCUMENT_H_

#include <QString>
#include <QStringList>
#include <QtGlobal>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QFile>
#include <QTemporaryFile>
#include <QList>
#include <QSslCertificate>
#include <QHash>
#ifdef SAFET_DIGIDOC // Archivos para firma electrónica DIGIDOC

#include <DigiDocDefs.h>
#include <DigiDocLib.h>
#include <DigiDocSAXParser.h>
#include <DigiDocPKCS11.h>
#include <DigiDocConfig.h>
#include <DigiDocCert.h>
#include <DigiDocConvert.h>
#include <DigiDocDebug.h>
#include <DigiDocEnc.h>
#include <DigiDocGen.h>
#include <DigiDocEncSAXParser.h>
#include <DigiDocOCSP.h>
#include <DigiDocDfExtract.h>

#endif // Archivos para firma electrónica DIGIDOC

#include "SafetXmlObject.h"
#include "SslCertificate.h"

#if SAFET_OPENSSL // CriptoSistema OPENSSL
#include <openssl/err.h>
#include <openssl/x509v3.h>
#include <openssl/pkcs12.h>
#endif // CriptoSistema OPENSSL


#ifdef SAFET_DBXML

#include "db_cxx.h"
#include "DbXml.hpp"

#endif

class SafetWorkflow;

#ifdef SAFET_OPENSSL  // CriptoSistema OpenSSL
static bool parsePKCS12Cert( const QByteArray &data, const QByteArray &pass, X509 **cert, EVP_PKEY **key )
{
        BIO *bio = BIO_new( BIO_s_mem() );
        if( !bio )
        { ERR_get_error(); return false; }

        BIO_write( bio, data.data(), data.size() );

        PKCS12 *p12 = d2i_PKCS12_bio( bio, NULL );
        BIO_free( bio );
        if( !p12 )
        { ERR_get_error(); return false; }

        int ret = PKCS12_parse( p12, pass.data(), key, cert, NULL );
        PKCS12_free( p12 );
        if(!ret)
                ERR_get_error();
        return ret;
}
#endif // CriptoSistema OpenSSL

/**
* \brief Clase SafetDocument
* 
* Esta Clase corresponde al objeto documento de un flujo de trabajo. Un documento estará contenido
* en un objeto SafetVariable. La clase SafetDocument permite la gestión de un objeto contenedor OpenXAdES
* para agregar un archivo al cual se le puede aplicar firma electrónica y estampillado de tiempo. 
* 
* La clase SafetDocument utiliza la libreria libdigidoc para la gestion de contenedores OpenXAdES. Cuando se 
* utilizan funciones de libdigidoc se retorna un valor ERR_OK para exito y otro valor en caso contrario. Es
* posible obtener una descripcion del error para las funciones de libdigidoc con:
* 
* char * w=getErrorString(rc);
* 
* De aqui se obtiene una cadena en el caso de que la funcion retorne con codigo de error (rc != ERR_OK). Las
* cadenas de error se encuentran en ingles.
*/

class SafetDocument : public SafetXmlObject {

private:

	/**
	* \brief Clave del documento.
	* 
	* Corresponde a un identificador unico del documento. 
	*/
	QString _key;
	


    QString _documents;
	/**
	* \brief Puntero a la estructura contenedor OpenXAdES del documento SafetDocument..
	* 
	* El contenedor OpenXAdES permite almacenar archivos (bajo la forma de: sentencia SQL, archivo generico 
	* y página web) dentro del objeto SafetDocument. El contenedor será la estructura utilizada para
	* almanecenar los datos a los que se les aplicará la firma electrónica o el estampillado de tiempo.   
	*/
#ifdef SAFET_DIGIDOC   //  Codigo para firma electronica DIGIDOC
	SignedDoc * pSigDoc;
	
	/**
    * \brief Puntero a la estructura archivo que se agregara al contenedor OpenXAdES del documento SafetDocument.
	* 
	* 
	*/
	DataFile * pDataFile;
	
	/**
	* \brief Puntero a la estructura SignatureInfo que se agregara al contenedor OpenXAdES del documento SafetDocument.
	* 
	* Esta estructura incluye informacion relevante a la firma electronica aplicada al contenedor.
	*/
	SignatureInfo* pSigInfo;

	/**
	* \brief  Documento XML correspondiente al objeto SafetDocument
	* 
	* Cuando el documento (SafetDocument) corresponde a un conjunto de registros de una repositorio relacional, 
	* se guarda su equivalente en un documento XML 
	*/
#endif //  Codigo para firma electronica DIGIDOC

	QString xmldocument;	

	/**
	* \brief  Objeto de consulta asociada al conjunto de documentos
	* 
	* Objeto Conjunto de registros relaciones que funcionar paraara mantener el estado o la posición en 
	* el conjunto de documentos
	*/
#ifdef SAFET_OPENSSL   //  Criptosistema OpenSSL
        static X509_STORE *store;
        static X509_LOOKUP *lookup;
        static X509_STORE_CTX *verify_ctx;
#endif //  Criptosistema OpenSSL
        static QString storepath; // Ruta del repositorio de Nombre CA

        /**
          * \brief Mapa de mensajes errores de Digidoc (Traducción al español)

          */
        static QMap<int,QString> digidocErrorStringsSpanish;

        /**
          * \brief Corresponde con la ruta completa del documento cargado con readDigidocFile
          */
        QString m_filename;



        /**
          * \brief Lista de archivos en los datos
          */
        QList<QString> _availablefiles;


public:
        /**
        * Enumerado que establece el uso de la clave asociada a un certificado firmante
        */
        enum KeyUsage
        {
                DigitalSignature = 0,
                NonRepudiation,
                KeyEncipherment,
                DataEncipherment,
                KeyAgreement,
                KeyCertificateSign,
                CRLSign,
                EncipherOnly,
                DecipherOnly,
        };
	
        /**
          * \brief Cuenta el numero de documentos que son iguales a una clave especifica (key)
          * \param query consulta a evaluar
          * \param key clave a evaluar (Comparar)
          * \param wf Flujo de trabajo actual
          * \param norender true Si se ejecuta la función para ser mostrado en un gráfico de flujo de trabajo
          * \return Numero de documentos relacionados con la clave ( 0 si no existen documentos)
          */


        int countForKey(QSqlQuery &query,const QString& key, SafetWorkflow *wf, bool norender);
	/**
	* \brief Enumerado que define las forma en la que un archivo se inserta en un contenedor OpenXAdES.
	* 
	* Las formas de insertar un archivo en un contenedor son:
	* 
	* SAFET_CONTENT_DETATCHED -> el archivo no se inserta en el contenedor; se inserta un enlace a su ubicación
	* en el sistema de archivo.
	* 
	* SAFET_CONTENT_EMBEDDED -> el archivo se inserta en el contenedor en su formato original.
	* 
	* SAFET_CONTENT_EMBEDDED_BASE64 -> el archivo se inserta codificado en formato BASE64
	*/
	enum ContentType {SAFET_CONTENT_DETATCHED, SAFET_CONTENT_EMBEDDED, SAFET_CONTENT_EMBEDDED_BASE64};
	
	/**
	* \brief Arreglo de cadenas que permite la conversion del tipo enumerado ContenType a la cadena que define 
	* la forma en la que un archivo se inserta en un contenedor OpenXAdES.
	*/
	const char * contentTypeArray[3];
	
	/**
	* Constructor de la clase.
	*/ 
	SafetDocument();
	
        /**
        * Constructor copia de la clase.
        */
        //SafetDocument(SafetDocument &doc);

	/**
	* Destructor de la clase.
	*/
	virtual ~SafetDocument();
		
	/**
	* Retorna el valor de la clave "key" unica del documento
	*/
	QString key() const;
	
	/**
	* Asigna el valor de la clave "key" unica del documento
	*/
	void setKey(QString k);


    QString documents() const  { return _documents; }
    void setDocuments(const QString& docs) { _documents = docs; }


    QList<QString> availableFiles()  const { return _availablefiles; }

    void setAvailableFiles(const QList<QString>& list) { _availablefiles = list; }
    void addAvailableFile(const QString& file) { _availablefiles.append(file); }
    void clearAvailableFiles() { _availablefiles.clear(); }

#ifdef SAFET_DIGIDOC   //  Codigo para firma electronica DIGIDOC
	/**
	* Retorna el puntero pSigDoc de SafetDocument
	*/
        SignedDoc* signedDoc();

	/**
	* Asigna el puntero pSigDoc de SafetDocument
	* \param sigDoc puntero a la estructura del contenedor digidoc
	*/
	void setSignedDoc(SignedDoc * sigDoc);

	/**
	* Retorna el puntero pDataFile de SafetDocument.
        * \param numfile Índice del archivo a acceder dentro del contenedor
	* En caso de no encontrar archivo dentro del contenedor retorna NULL.
	*/
        DataFile * dataFile(int numfile = 0);
	
	/**
	* Asigna el puntero pDataFile de SafetDocument
	* \param sigDoc puntero a la estructura pDataFile del contenedor digidoc
	*/
	void setDataFile(DataFile * pDFile);

        /**
        * Retorna un lista de DataFile con todos los documentos agregados al contenedor digidoc
        * \return Lista de DataFile internos en el contendor digidoc
        */
        QList<DataFile *> dataFilesList();

	/**
        * Retorna el puntero SignatureInfo (firma) con indice pasado como argumento
        * \param index indice de la firma a obtener
        * \return SignatureInfo* puntero a la firma recuperada
	*/
        SignatureInfo* SigInfo(int index = 0);
	
	/**
	* Asigna el puntero pSigInfo de SafetDocument
	* \param sigDoc puntero a la estructura SignatureInfo del contenedor digidoc
	*/
        void setSigInfo(SignatureInfo * sigInfo);

        /**
        * Retorna un lista de SignatureInfo con todos las firmas electronicas aplicadas
        * \return Lista de SignatureInfo internos en el contendor digidoc
        */
        QList<SignatureInfo *> signaturesList();
#endif	//  Codigo para firma electronica DIGIDOC
        /**
          \brief Funciones de manejo de errores de OpenSSL
          */
        static void openssl_error(const QString txt = "");

        static bool ign_openssl_error();

        /**
          \brief Configura los mensajes de error para la biblioteca Digidoc
          */
        static void configureDigidocErrorString();

        /**
          \brief Retorna la cadena en en español del respectivo número de error,
                en caso contrario no devuelve ningun valor
          */
        static QString digidocStringError(int err);

        /**
          \brief Devuelve los ultimos errores de OpenSSL en un QString
          \return un cadena (QString) con los errores separados por \n
          */

        static QString opensslStringError();
        /**
            \brief Funciones de error para manejo de archivos (fopen)
            */
        static void fopen_error(const QString fname);


        /**
          \brief Obtiene la lista de nombres de los certificados validos de la ruta
          definida en el directorio de certificados (Digidoc)
          \return Lista de ruta completa, incluye nombre de archivos de certificados (con extensión .pem ó .crt)
          **/
        static QStringList getCertFileList(const QString& ext = ".pem");

#ifdef SAFET_OPENSSL // Criptosistema OpenSSL
        /**
          \brief Carga de un archivo en disco a una estructura de Certificado X509
          \param filename Ruta y nombre del archivo en disco
          \return Un puntero NULL si la carga falló, de otra forma carga el archivo X509
          */
        static X509* loadX509FromFile(const QString& filename);

        /**
          * \brief verifica si el certificado "signer" está firmado por el certificado "cert"
          * \param cert Certificado al que se le verifica la firma
          * \param signer Firmante del certificado "cert"
          * \return Retorna verdadero si el certificado "signer" es el firmante del certificado "cert"
          */
        static bool verifySigner(X509 *cert, X509 *signer);





        /**
          \brief Verifica localmente si el certificado "cert" está firmado por uno
          de los certificados de el repositorio CA, y no está incluído en las listas de revocación locales
          \param cert Certificado a verificar firma y validez
          \return Verdadero si el certificado se encuentra activo, Falso en caso contrario
          */
        static bool verifyWithLocalX509Store(X509 *cert);

#endif // CriptoSistema OpenSSL

        /**
          * \brief Configura y carga en memoria el Repositorio de Certificados de CA, además de las CRL (Estática)
          */
        static void setupLocalX509Store();

        /**
          \brief Chequea si la contraseña del archivo indicado por <keyFile>,
                 es la correcta
          \param keyFile, ruta y nombre del archivo de clave en formato PEM
          \param passwd Contraseña a evaluar
          \return Verdadero si es la contraseñá correcta, en otro caso retorna falso
          */
        static bool checkPrivateKeyPassword(const QString& keyFile,
                                     const QString& passwd);


        /**
          * \brief Lee un un archivo digidoc desde la ubicación que se especifica en el parámetro fileName
          *  \param filename Ruta y nombre de archivo con extensión .ddoc
          * \return Retorna verdadero si fue exitosa la lectura
          */
        bool readDigiDocFile(const QString& filename);

        /**
          \brief Verifica remotamente (OCSP) si la firmas son válidas
          \return Una lista de valores lógicos (booleanos) en un Arreglo de Bits que corresponden con los números de firmas
          */
        QBitArray verifyOCSPSignBit();

        /**
          \brief Verifica remotamente (OCSP) si la firmas son válidas
          \return Una lista de valores lógicos (booleanos) que corresponden con los números de firmas
          */
        QList<bool> verifyOCSPSign();
	/**
	* Inicializar la libreria libdigidoc
	* 
	*/
	void initializeLibdigidoc();
	
	/**
	* Cerrar la libreria libdigidoc
	* 
	*/
	void closeLibdigidoc();
	
	/**
	* Crear la estructura de un Contenedor OpenXAdES
	* 
	*/
	int createOpenXAdESContainer();

#ifdef SAFET_DIGIDOC // Firma electrónica
	/**
	* \brief Agrega un archivo al contenedor OpenXAdES.
	* 
	* Un archivo se puede agregar a un contenedor OpenXAdES de varias maneras:
	* 
	* i) colocando en el contenedor sólo una referencia a la ubicación física del archivo en el sistema de 
	* archivos
	* 
	* ii) colocando el archivo dentro del contenedor OpenXAdES en su forma original
	* 
	* iii) colocando el archivo dentro del contenendor OpenXAdES pero convertido en BASE64
	* 
	* La manera en que se agrega el archivo al contenedor se especifica en el argumento cType.
	* 
	* \param pSigDoc puntero a la estructura de contenedor OpenXAdES del documento SafetDocument
	* \param fileName nombre del archivo a incluir en la estructura del contenedor
	* \param cType especifica la manera en que se incluye el archivo dentro de la estructura del contenedor.
	* Los posibles valores se encuentran definidos en el enumerado ContentType 
	* \param mime especifica el tipo mime del archivo que se agrega en la estructura del contenedor
	* 
	*/
	int addFileToOpenXAdESContainer(SignedDoc * pSigDoc, QString fileName, ContentType cType, QString mime);
	
	/**
	* Agrega un atributo adicional a un archivo dentro de un Contenedor OpenXAdES
	* 
	* \param pDataFile puntero al archivo que se agrega en el contenedor OpenXAdES
	* \param name cadena que especifica el nombre del atributo adicional
	* \param value cadena que especifica el valor del atributo adicional
	*/
	int addAttributeToFile(DataFile * pDataFile, QString name, QString value);
	
	/**
	* Agrega un atributo adicional a un archivo dentro de un Contenedor OpenXAdES
	* 
	* Esta funcion se diferencia de la anterior pues se le pasa como un argumento el archivo al que 
	* se le agrega el atributo; de esta manera se recalcula el hash del archivo.
	*
	* \param pDataFile puntero al archivo que se agrega en el contenedor OpenXAdES
	* \param fileName cadena que especifica el nombre del archivo original
	* \param name cadena que especifica el nombre del atributo adicional
	* \param value cadena que especifica el valor del atributo adicional
	*/
	int addAttributeToFile(DataFile * pDataFile, QString fileName, QString name, QString value);

#endif // DIGIDOC
	/**
	* Escribe y guarda el contenedor OpenXAdES en un archivo del sistema de archivos.
	* 
	* \param inputFileName cadena que especifica el nombre del archivo que se desea incluir en el contenedor
	* \param outputFileName cadena que especifica el nombre del archivo en formato OpenXAdES (.ddoc) que incluye
	* el contenedor.
	*/	
        int writeOpenXAdESContainerToFile(const QString& inputFileName,
                                          const QString& outputFileName);
	
        /**
        * Escribe el archivo (DataFile) incluido en el contenedor OpenXAdES en un archivo en el sistema de archivos.
        *
        * \param index indice del archivo que se desea extraer del contenedor
        * \param digidocPath ruta al archivo digidoc (.ddoc) que se desea extraer el archivo incluido
        * \param outputPath ruta donde se desea escribir el archivo en el sistema de archivos
        * \return ERR_OK en caso de que el archivo haya sido extraido del contenedor y escrito correctamente. En
        * en otro caso retorna el codigo de error
        */
        int writeDataFileOfContainerToFile(int index = 0, QString digidocPath = "",
                                           QString outputPath = "");

        /**
        * Escribe el archivo (DataFile) incluido en el contenedor OpenXAdES en un archivo en el sistema de archivos.
        *
        * \param index indice del archivo que se desea extraer del contenedor
        * \param outputPath ruta donde se desea escribir el archivo en el sistema de archivos
        * \return ERR_OK en caso de que el archivo haya sido extraido del contenedor y escrito correctamente. En
        * en otro caso retorna el codigo de error
        */
        int writeDataFileOfContainerToFile(int index = 0, QString outputPath = "");

        /**
        * Escribe el archivo (DataFile) incluido en el contenedor OpenXAdES en un directorio temporal.
        *
        * \param index indice del archivo que se desea extraer del contenedor
        * \param digidocPath ruta al archivo digidoc (.ddoc) que se desea extraer el archivo incluido
        * \return cadena que representa la ruta absoluta al directorio temporal con el archivo extraido
        */
        QString writeDataFileOfContainerToTmp(int index = 0, QString digidocPath = "");

	/**
	* \brief Lee un contenedor OpenXAdES desde en un archivo del sistema de archivos.
	* 
	* Este método lee un objeto contenedor OpenXAdES de un archivo almacenado en el sistema de archivos. El 
	* contenedor leido se almacena en el miembro pSigDoc.
	* 
	* \param fileName cadena que especifica la ruta y nombre del archivo que se pretende leer.
	* 
	*/
	int readOpenXAdESContainerFile(const QString& fileName);
	
	/**
	* Libera la memoria del pSigDoc cuando ya no se trabaja con el.
	*/
	void freeMemoryOfOpenXAdESContainer();
	
	/**
	* Retorna el numero de archivos que posee el contenedor OpenXAdES
	*/
	int numberOfDataFileOnOpenXAdESContainer();
	
	/**
	* Retorna el numero de firmas que posee el contenedor OpenXAdES
	*/
	int numberOfSignaturesOnOpenXAdESContainer();
	
	/**
	* Retorna el nombre del archivo que posee el contenedor OpenXAdES
        * \param index indice del documento
	*/
        QString getDataFileName(int index = 0);
	
	/**
	* Retorna la longitud del archivo que posee el contenedor OpenXAdES
        * \param index indice del documento
	*/
        long getDataFileLength(int index = 0);
	
	/**
	* Retorna el tipo MIME del archivo que posee el contenedor OpenXAdES
        * \param index indice del documento
	*/
        QString getDataFileMimeType(int index = 0);
	
	/**
        * Retorna el numero de atributos del archivo con indice index en el contenedor OpenXAdES
        * \return numero de atributos adicionales que posee el archivo con indice index en del contenedor
	*/
        int numberOfDataFileAttributes(int index = 0);


#ifdef SAFET_DIGIDOC // Firma Electronica DIGIDOC
	/**
	* Retorna informacion sobre la firma electronica con identificador pasado como argumento. La estructura 
	* SignatureInfo incluye informacion completa sobre la firma electronica asociada.
	* 
	* \param id representa el identificador de la firma. Un contenedor OpenXAdES puede incluir una o varias 
	* firmas. El identifoador de firmas comienza a enumerarse desde el valor 0.
	* \return puntero SignatureInfo con la estructura de informacion de firma
	*/
	SignatureInfo * getOpenXAdESContainerSignature(int id);
#endif // DIGIDOC
	
	/**
	* Funcion de prueba para realizar conexion a base de datos
	*/
	void connectToDataBase(QString connectionString);
	
	/**
	* funcion de prueba para obtener lista de tablas de la base de datos a las que se esta conectado
	*/	
	QStringList getDataBaseTableNames();
	
	
	/**
	* \brief Crea un archivo XML con el resultado de una sentencia SQL. 
	* 
	* El contenido del archivo XML incluye el resultado de una sentencia SQL. Esto corresponde a un caso 
	* en el que se requiere aplicar firma electronica o estampillado de tiempo a un tipo de archivo que no
	* es de texto ni de datos. Los tipos de archivos que se categorizan son: archivo (file), SQL (resultado 
	* de una sentencia SQL) y pagina web. 
	* 
	* \param sqlSentence cadena que representa la sentencia SQL que se desea realizar y guardar en un 
	* contendor OpenXAdES
	* \param outputFileName cadena que representa el nombre del archivo XML de salida en el sistema de 
	* archivos
	* 
	*/
	void createXMLFileFromSQL(QString sqlSentence, QString outputFileName );
	


	

	

	/**
	* \brief Dada una consulta, devuelve la lista de nombres de campos asociada al documento
	* \param query Conjunto de registros  de una base de datos relacional
	* \return Lista de cadenas de los campos asociados al documento
	*/
	QList<QString> getFields(QSqlQuery &query);


	/**
	* \brief Dada una consulta, devuelve la lista de informaciones sobre los camoi
	* \param query Conjunto de registros  de una base de datos relacional
	* \return Lista de tipos (Consulta QVariant::Type) de los campos);
	*/
	QList<QVariant::Type> getInfos(QSqlQuery &query);

	/**
	* \brief Actualiza el contenido del documento interno con los registros de la consulta dada (query)
	* 
	* Dada una conjunto de registros, se actualiza el objeto y se genera un documento XML
	* correspondiente a la consulta que se pasa por parámetro. Se modifica el parámetro query (QSqlQuery)
	* \param query Conjunto de registros  de una base de datos relacional
	* \return Formato en XML de la consulta (Cadena de caracteres)
	*/
    QString getXmlQuery(QSqlQuery &query, int &dcount, const QString& info = "");

	/**
	* \brief Actualiza el contenido del documento interno con los registros de la consulta dada (query)
	* 
	* Dada una conjunto de registros, se actualiza el objeto y se genera un documento JSON
	* correspondiente a la consulta que se pasa por parámetro. Se modifica el parámetro query (QSqlQuery)
	* \param query Conjunto de registros  de una base de datos relacional
	* \return Formato en JSON de la consulta (Cadena de caracteres)
	*/
        QString getJsonQuery(QSqlQuery &query, QList<QSqlField>& fields, int &dcount,
                             const QString& info = "");



        QString getJsonArrayQuery(QSqlQuery &query, QList<QSqlField> &fields, int &dcount, const QString &info = "");
	/**
	* Funcion para aplicar firma electronica a un documento digidoc (OpenXAdES) existente.
	* 
	* \param keyFile cadena que representa la ruta donde se encuentra el archivo con la clave privada del firmante
	* \param passwd contrasena asociada a la clave privada
	* \param certFile archivo que contiene el certificado del usuario firmante
	* \param inFile archivo en formato digidoc a firmar
	* \param outFile ruta del archivo firmado dentro del sistema de archivos
	* 
	*/
        int addSignatureToExistingDigidocFile(QString keyFile, QString passwd,
                                              QString certFile, QString inFile, QString outFile);

	/**
	* Funcion para firma un documento digidoc (OpenXAdES) utilizando una clave privada almancenada en un archivo.
	* 
	* \param keyFile cadena que representa la ruta donde se encuentra el archivo con la clave privada del firmante
	* \param passwd contrasena asociada a la clave privada
	* \param certFile archivo que contiene el certificado del usuario firmante
	* \param inFile archivo en formato digidoc a firmar
	* \param outFile ruta del archivo firmado dentro del sistema de archivos
	* \param manifest ... rol del firmante
	* \param city ciudad donde se realiza la firma
	* \param state estado donde se realiza la firma
	* \param zip codigo postal donde se realiza la firma
	* \param country pais donde se realiza la firma
	* \param notaryUrl url del servidor OCSP para realizar la notarizacion de la firma
	* \param proxyHost nombre del servidor OCSP
	* \param proxyPort numero de puerto del servidor OCSP
	* 
	*/
        int signWithPrivateKeyOnFile(const QString&  keyFile,
                                     const QString& passwd,
                                     const QString& certFile,
                                     const QString& inFile,
                                     const QString& outFile,
                                     const QString& manifest,
                                     const QString& city,
                                     const QString& state,
                                     const QString& zip,
                                     const QString& country,
                                     const QString& notaryUrl,
                                     const QString& proxyHost,
                                     const QString& proxyPort);

	/**
	* Funcion para inicializar la libreria pkcs11
	* 
	* \param libName cadena que representa la ruta donde se encuentra la libreria pkcs11
	* 
	*/
	void initPKCS11Lib(QString libName);
	
	
	/**
	* Funcion para especificar el archivo de configuracion de la libreria digidoc
	* 
	* \param configFile cadena que representa la ruta donde se encuentra el archivo de configuracion de digidoc
	* 
	* Esta funcion debe ser llamada antes de realizar operaciones de firma y verificacion de firma, pues es necesario leer
	* el archivo de configuracion de digididoc
	*/
	int initDigidocConfigStore(QString configFile);


#ifdef SAFET_DIGIDOC // Firma electrónica
	/**
	* Funcion para firmar el contenido de un envoltorio digidoc con la tarjeta inteligente
	* 
	* \param pSigDoc puntero al contenedor que mantiene el archivo a firmar
	* \param pin numero de identificacion personal que protege la tarjeta inteligente
	* \param manifest ... rol del firmante
	* \param city ciudad donde se realiza la firma
	* \param state estado donde se realiza la firma
	* \param zip codigo postal donde se realiza la firma
	* \param country pais donde se realiza la firma
	* 
	*/
	int signWithSmartCard(SignedDoc* pSigDoc, QString pin, QString manifest, QString city, QString state, QString zip, QString country);
#endif // DIGIDOC
	/**
	* Funcion para verificar firma de un envoltorio digidoc con la tarjeta inteligente
	* 
	* \param fileName ruta del archivo que contiene la firma ddoc
	* \return codigo de error. en caso de exito: ERR_OK = 0
	*/	
	bool verifySignMadeWithSmartCard(QString fileName);
	
	/**
	* Funcion para verificar la firma de un envoltorio digidoc con el identificador pasado como argumento 2
	* 
	* \param fileName ruta del archivo que contiene la firma ddoc
	* \param signatureIndex indice de la firma que se desea verificar
	* \return retorna en caso de exito: la firma se verifico correctamente
	*/	
	bool verifySignMadeWithSmartCard(QString fileName, int signatureIndex);
	

        enum SignatureOperator  { AND, OR, ALL };

	/**
	* Funcion para verificar las firmas de un envoltorio digidoc
	* 
	* Esta funcion se encarga de verificar TODAS las firmas de un documento digidoc. Si al menos una
	* firma no es valida la verificacion de las firmas es incorrecta.
	*
	* \param fileName ruta del archivo que contiene la firma ddoc
	* \param listOfSigners lista de cadenas con los nombre comun (CN) de cada firmante del documento
	* \return retorna en caso de que todas las firmas sean verificadas
	*/

        bool verifySignMadeWithSmartCard(const QString& fileName,
                                         const QStringList& listOfSigners,
                                         bool &isneg,
                                         SignatureOperator op = SafetDocument::AND );

#ifdef SAFET_DIGIDOC // Firma electrónica
	/**
	* Funcion para imprimir el contenido de certificados de un archivo digidoc
	* 
	* \param pSigDoc puntero al contenedor que mantiene el archivo firmado
	* \param pCert puntero al 
	*/
	void printCertificateInfo(SignedDoc* pSigDoc, X509* pCert);
#endif // DIGIDOC

	/**
	* Funcion para probar la carga del driver de la tarjeta inteligente
	* 
	* \param slot identificador del slot en el que se encuentra la tarjeta inteligente
	* 
	*/
	void testSmartCardDriver(int slot);


	/**
	* Funcion que retorna el nombre comun del certificado firmante de la firma n del documento
	* firmado
	* 
	* \param signatureIndex indice de la firma de la cual se desea el CN del firmante
	* 
	* \return cadena con el CN del firmante, en caso de que no exista se retorna una cadena vacia
	*/
	QString getCN(int signatureIndex);


	/**
	* Funcion que retorna indice de una firma dado el CN del certificado firmante.
	* 
	* En el caso de que un documento tenga mas de una firma con un mismo CN se retorna la primera
	* firma encontrada.
	*
	* \param cn cadena que representa el CN del certificado firmante
	* 
	* \return indice de la firma dentro de contenedor digidoc. en caso de que no 
	* exista una firma con el CN dado se retorna -1
	*/
	int getSignerIndex(QString cn);


	/**
	* Funcion que retorna una lista de cadenas con los CN (nombre comun) de cada firmante del documento.
	*
	* Para verificar si la lista de cadenas esta vacia se puede utilizar la funcion isEmpty() de QStringList,
	* que retorna verdadero en caso de que la lista este vacia.
	*
	* \return lista de cadenas con los CN de los firmantes, en caso de que no exista se retorna una cadena vacia.
	*/
	QStringList getCommonNameOfSigners();


	/**
	* Funcion que retorna el numero de claves privadas que posee una tarjeta inteligente
	*
	* \param slot numero de slot en el que se encuentra la tarjeta inteligente
	* \param passwd representa la cadena asociada al PIN de la tarjeta inteligente
	* \param libName cadena con la ruta de la implementacion de PKCS11 (p.ej: /usr/lib/opensc-pkcs11.so)
	*
	* \return numero de claves privadas que posee la tarjeta
	*/
	int getNumberOfPrivateKeys(int slot, QString passwd, QString libName);

#ifdef SAFET_DIGIDOC // Firma electrónica
	/**
	* Funcion para cargar la libreria PKCS11 
	*
	* \param libname cadena con la ruta de la implementacion de PKCS11 (p.ej: /usr/lib/opensc-pkcs11.so)
	*
	* \return numero de claves privadas que posee la tarjeta
	*/
	LIBHANDLE initPKCS11LibrarySafet(const char* libName);
#endif


	/**
	* Funcion para escribir el contenido de una cadena pasada como argumento en un archivo
	*
	* \param string cadena que se desea escribir en un archivo
	* \param name cadena que corresponde al nombre del archivo
	*
	* \return ruta del archivo generado en el sistema de archivos
	*/
	QString writeFileToDisk(QString string, QString name);

	/**
	* Funcion que retorna el contenido de un archivo en una cadena
	*
	* \param pathFileName ruta al archivo que se desea retornar
	*
	* \return cadena con el contenido del archivo
	*/
        QString returnFileToString(const QString& pathFileName);


#ifdef SAFET_DIGIDOC // Firma electrónica
	/**
	* Funcion para cifrar una cadena utilizando claves publicas/privadas.
	*
	* Para cifrar la cadena se utiliza una clave simetrica (clave de 
	* transporte) generada de forma aleatoria. Luego esta clave se cifra
	* con la clave publica almacenada en el certificado del destinatario.
	* Para descrifrar la clave de transporte se utiliza la clave privada
	* almacenada en una tarjeta inteligente. Luego se descifra la cadena
	* con la clave simetrica.
	*
	* \param pSigDoc puntero al contenedor que mantiene el archivo a cifrar.
	* \param ppEncData puntero a estructura que contiene los datos cifrados.
	* \param stringToCrypt cadena que se desea cifrar.
	* \param recipient cadena con el nombre comun (CN) del receptor del archivo.
	* \param keyName cadena con el nombre de la clave simetrica (clave de transporte)
	* con la cual se cifra el documento.
	* \param certFile ruta absoluta al archivo que contiene el certificado del 
	* receptor.
	* \param outputFile ruta absoluta al archivo que contiene el documento 
	* cifrado.
	* \param mimeType tipo mime del archivo que se va a cifrar.
	*
	* \return codigo de error, ERR_OK == correcto, en otro caso error.
	*/
	int encryptString(DEncEncryptedData** ppEncData, QString stringToEncrypt, QString recipient, 
		QString keyName, QString certFile, QString outputFile, QString mimeType);

	/**
	* Funcion para cifrar un documento utilizando claves publicas/privadas.
	*
	* Para cifrar el documento se utiliza una clave simetrica (clave de 
	* transporte) generada de forma aleatoria. Luego esta clave se cifra
	* con la clave publica almacenada en el certificado del destinatario.
	* Para descrifrar la clave de transporte se utiliza la clave privada
	* almacenada en una tarjeta inteligente. Luego se descifra el documento
	* con la clave simetrica.
	*
	* \param ppEncData puntero a estructura que contiene los datos cifrados.
	* \param documentToEncrypt documento que se desea cifrar.
	* \param recipient cadena con el nombre comun (CN) del receptor del archivo.
	* \param keyName cadena con el nombre de la clave simetrica (clave de transporte)
	* con la cual se cifra el documento.
	* \param certFile ruta absoluta al archivo que contiene el certificado del 
	* receptor.
	* \param outputFile ruta absoluta al archivo que contiene el documento 
	* cifrado.
	* \param mimeType tipo mime del archivo que se va a cifrar.
	*
	* \return codigo de error, ERR_OK == correcto, en otro caso error.
	*/
	int encryptDocument(DEncEncryptedData** ppEncData, QString documentToEncrypt, QString recipient, 
		QString keyName, QString certFile, QString outputFile, QString mimeType);

#endif // DIGIDOC
	/**
	* Funcion que descifra un documento
	*
	* \param inFile ruta absoluta al archivo que se desea cifrar.
	* \param outputFile ruta absouta al archivo descifrado.
	* \param pin numero de identificacion personal de la tarjeta inteligente.
	*
	* \return codigo de error, ERR_OK == correcto, en otro caso error.
	*/
	int decryptDocument(QString inFile, QString outputFile, QString pin);


	/**
	* Funcion que descifra una cadena cifrada almacenada en un archivo
	*
	* \param inFile ruta absoluta al archivo que contiene la cadena cifrada
	* \param pin numero de identificacion personal de la tarjeta inteligente.
	*
	* \return cadena descifrada
	*/
	QString decryptString(QString inFile, QString pin);



	/**
	* Funcion que retorna lista de nombres unicos para archivos temporales 
	*
	* \param n numero de nombres de archivos solicitados
	*
	* \return lista de cadenas con los nombres de archivos temporales
	*/
	QStringList getTempNameFiles(int n);


	/**
	* Funcion que retorna el nombre comun de un certificado digital
	*
	* \param n numero de nombres de archivos solicitados
	*
	* \return lista de cadenas con los nombres de archivos temporales
	*/
	QString getCommonNameFromCertificate(QString certPath);

	/**
	* Funcion que buscar en un conjunto de certificados dentro de un directorio 
	* uno cuyo nombre comun sea el pasado como argumento.
	* 
	* \param commonName nombre comun del usuario que se desea buscar en el conjunto 
	* de certificados
	*\param x509repository ruta absoluta al repositorio de archivos de certificados
	*
	* \return nombre del archivo que contiene el certificado con nombre comun buscado
	*/
	QString searchCommonNameInCertificates(QString commonName, QString x509repository);


	/**
	* Retorna el atributo con id pasado como argumento. El indice 0 es el valor por defecto
	* \param index indice del atributo a solicitar
	* \return retorna un QStrinList con el nombre y el valor del atributo con
	* con el indice index
	*/
	QStringList getAttribute(int index = 0);

#ifdef SAFET_DIGIDOC // Criptosistema OpenSSL
        /**
        * Retorna en un objeto X509 el certificado firmante asociado a la firma pasada como argumento
        * \param index indice de la firma para extraer el certificado firmante
        * \return retorna puntero a objeto X509 con el certificado firmante de la firma con indice pasado como argumento
        */
        X509 * getSignerCertData(int index = 0);

        /**
        * Retorna en un objeto SslCertificate el certificado firmante asociado a la firma
        * pasada como argumento
        * \param index indice de la firma para extraer el certificado firmante
        * \return retorna un objeto SslCertificate con el certificado firmante de la firma
        * con indice pasado como argumento
        */
        SslCertificate getSslSignerCertData(int index = 0);
#endif // DIGIDOC

	/**
	* Retorna una cadena con el pais en el que se produce la firma
	* \param index indice de la firma para extraer el nombre del pais
	* \return cadena con el pais en el que se produce la firma. Cadena vacia en caso de no estar establecido el pais
	*/
	QString getCountryOfSignature(int index = 0);

	/**
	* Retorna una cadena con el estado en el que se produce la firma
	* \param index indice de la firma para extraer el nombre del estado
	* \return cadena con el estado en el que se produce la firma. Cadena vacia en caso de no estar establecido el estado
	*/
	QString getStateOfSignature(int index = 0);

	/**
	* Retorna una cadena con la ciudad en la que se produce la firma
	* \param index indice de la firma para extraer el nombre de la ciudad
	* \return cadena con la ciudad en el que se produce la firma. Cadena vacia en caso de no estar establecida la ciudad
	*/
	QString getCityOfSignature(int index = 0);

	/**
	* Retorna una cadena con el codigo postal de la ciudad en la que se produce la firma
	* \param index indice de la firma para extraer el codigo postal de la ciudad
	* \return cadena con el codigo postal de la ciudad en el que se produce la firma. Cadena vacia en caso de no estar establecido el codigo
	*/
	QString getPostalCodeOfSignature(int index = 0);

        /**
        * Retorna una QStringList con los datos del lugar donde se realiza la firma
        * \param index indice de la firma para extraer la informacion
        * \return lista de cadenas con el lugar donde se hace la firma
        */
        QStringList signatureLocations(int index = 0) const;

        /**
        * Retorna una QString con los datos del lugar donde se realiza la firma,
        * los elementos de la cadena estan separados por coma
        * \param index indice de la firma para extraer la informacion
        * \return cadena con el lugar donde se hace la firma
        */
        QString signatureLocation(int index = 0) const;

	/**
	* Retorna el numero de roles asociados a una firma
	* \param index indice de la firma para buscar el numero de roles
	* \return numero de roles asociados a una firma
	*/
	int getCountOfRoles(int index = 0);

	/**
	* Retorna una cadena con el rol del usuario firmante de la firma con indice pasado como argumento
	* \param index indice de la firma para extraer el rol
	* \param roleIndex indice de del rol a extraer
	* \return el rol del firmante
	*/
	QString getRole(int index = 0, int roleIndex = 0);

	/**
	* Retorna una cadena con el la fecha en la que se realizo la firma identificado con el indice pasado como argumento
	* \param index indice de la firma para extraer la fecha de firma
	* \return fecha en que se realizo la firma del archivo
	*/
	QString getSingingTime(int index = 0);

        /**
        * Retorna una cadena con solo la fecha en la que se realizo la firma identificado con el indice pasado como argumento
        * NOTA: el formato de la cadena con la fecha es: DD-MM-AAAA
        * \param index indice de la firma para extraer la fecha de firma
        * \return fecha en que se realizo la firma del archivo
        */
        QString getSingingTimeOnlyDate(int index = 0);
	
        /**
        * Retorna una cadena con solo la hora en la que se realizo la firma identificado con el indice pasado como argumento.
        * NOTA: el valor retornado de la hora esta en formato UTC, por ejemplo 22:29:04Z
        * \param index indice de la firma para extraer la fecha de firma
        * \return fecha en que se realizo la firma del archivo
        */
        QString getSingingTimeOnlyHour(int index = 0);

	/**
	* Retorna una cadena con el tipo de firma realizada
	* \param index indice de la firma para extraer el tipo de firma
	* \return tipo de firma
	*/
	QString getSignatureType(int index = 0);

	/**
	* Retorna una cadena con el formato de la firma
	* \param index indice de la firma para extraer el formato
	* \return formato de la firma
	*/
	QString getSignatureFormat(int index = 0);

	/**
	* Retorna una cadena con el nombre de la AC que emitio el certificado firmante
	* \param index indice de la firma para extraer el nombre del certificado
	* \return nombre de la AC que emitio el certificado firmante
	*/
	QString getSignerCertificateIssuerName(int index = 0);

	/**
	* Retorna una cadena con el serial del certificado firmante
	* \param index indice de la firma para extraer el serial del certificado
	* \return serial del certificado firmante
	*/
	QString getSignerCertificateSerial(int index = 0);

	/**
	* Retorna una cadena con la fecha de inicio de periodo de validez de certificado (valid at)
	* \param index indice de la firma para extraer la fecha
	* \return fecha de inicio de periodo de validez de certificado 
	*/
	QString getValidAt(int index = 0);

	/**
	* Retorna una cadena con la fecha de culminacion de periodo de validez de certificado (valid at)
	* \param index indice de la firma para extraer la fecha
	* \return fecha de culminacion de periodo de validez de certificado 
	*/
	QString getValidUntil(int index = 0);

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
        /**
        * Retorna el certificado del OCSP responder asociado a la firma con indice pasado como argumento
        * \param index indice de la firma para ubicar el certificado de OCSP
        * \return certificado de OCSP (X509*) o NULL en caso de no encontrarlo
        */
        X509 * getOCSPResponderCertificate(int index = 0);
#endif  // DIGIDOC
        /**
        * Retorna la ruta completa del documento cargado con readDigidocFile
        * \return ruta completa del documento cargado con readDigidocFile
        */
        QString getPathOfSafetDocument();

        /**
        * Asigna la ruta del contenedor en m_filename
        */
        void setPathOfSafetDocument(QString path) { m_filename = path; }

        /**
        * Retorna una lista de cadenas con los usos extendidos de clave que posee un certificado de firma
        * \param index indice de la firma asociada al certificado
        */
        QStringList enhancedKeyUsage(int index = 0);

        /**
        * Retorna cadena en UTF16
        */
        QString toUtf16( const QString &in ) const;

        /**
        * Retorna cadena en UTF8
        */
        QString toUtf8( const QString &in ) const;

        /**
        * Decodifica una cadena UTF8, UTF16
        */
        QString decode( const QString &in ) const;

        /**
        * Retorna cadena UTF8 con la informacion del sujeto
        * subject 0 -> Organization
        * subject 1 -> CommonName
        * subject 2 -> LocalityName
        * subject 3 -> OrganizationalUnitName
        * subject 4 -> CountryName
        * subject 5 -> StateOrProvinceName
        */
        QString subjectInfoUtf8( int index, /*SubjectInfo*/ int subject ) const;

        /**
        * Retorna en QByteArray el numero de version del certificado firmante
        */
        QByteArray versionNumber(int index = 0);

        /**
        * Retorna en QByteArray el numero de serial del certificado firmante
        */
        QByteArray serialNumber(int index = 0);

        /**
        * Retorna en QByteArray el contenido en HEX
        */
        QByteArray toHex( const QByteArray &in, QChar separator );

        /**
        * Retorna el DN del sujeto del certificado firmante de la firma con indice index
        * \param index indice de la firma
        */
        QString getSubjectDN(int index = 0);

        /**
        * Retorna el DN del emisor del certificado firmante de la firma con indice index
        * \param index indice de la firma
        */
        QString getIssuerDN(int index = 0);

        /**
        * Retorna una lista de cadenas con politicas del certificado de firma
        * \param index indice de la firma asociada al certificado
        */
        QStringList policies(int index = 0);

        /**
        * Retorna en QByteArray el authorityKeyIdentifier del certificado firmante con indice index
        * \param index indice de la firma asociada al certificado
        */
        QByteArray authorityKeyIdentifier(int index = 0);

        /**
        * Retorna en QByteArray el subjectKeyIdentifier del certificado firmante con indice index
        * \param index indice de la firma asociada al certificado
        */
        QByteArray subjectKeyIdentifier(int index = 0);

        /**
        * Retorna los usos de la clave asociada al certificado firmante
        * \param index indice de la firma asociada al certificado
        */
        QHash<int,QString> keyUsage(int index = 0);

        /**
        * Retorna el algoritmo utilizado para la clave publica
        * \param index indice de la firma asociada al certificado
        */
        QString getPublicKeyAlgorithm(int index = 0);

        /**
        * Retorna la longitud de la clave publica
        * \param index indice de la firma asociada al certificado
        */
        QString getPublicKeyLength(int index = 0);

        /**
        * Escribe el certificado asociado a la estructura X509 de la firma con indice index
        * \param path ruta donde se desea escribir el archivo
        * \param PEM verdadero si se desea escribir el archivo en formato PEM, falso para DER
        * \param index indice de la firma asociada al certificado
        */
        void writeX509ToFile(QString path, bool PEM, int index = 0);

        /**
           \brief Genera la información textual que se le coloca a cada tarea (Rol y fecha de entrada)
           \param query Consulta de donde se extrae la información (el penúltimo campo es
           el rol y el ultimo campo es la estampilla de tiempo
           \param wf Flujo de trabajo actual
          */
        void generateExtraInfo(const QSqlQuery& query, SafetWorkflow *wf);



};

#endif /*SAFETDOCUMENT_H_*/


/**
* \mainpage Proyecto SAFET: Sistema Automatizado para la Firma Electronica y Estampado de Tiempo
*
* Este es la pagina inicial del proyecto.
*/

