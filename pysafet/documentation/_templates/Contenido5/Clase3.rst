


==============================
:index:`Clase SafetDocument` 
==============================



**Clase SafetDocument** : SafetXmlObject
	Esta clase modela un documento de una variable en **PySafet** (el documento puede ser firmado electrónicamente).


	**Listado de myinflow:**


	#. **getCertFileList** (ext = ".pem") [Retorna cadenas (String)]
		Obtiene la ruta del certificado usado para firmar el documento. 


	#. **numberOfDataFileOnOpenXAdESContainer** () [Retorna un entero (int)]
		Número de archivos que contienen el documento firmado.	


	#. **numberOfSignaturesOnOpenXAdESContainer** () [Retorna un entero (int)]
		Número de firmas que contienen el documento firmado.

	#. **getDataFileName** (index = 0) [Retorna cadenas (String)]
		Nombre del archivo contenido de índice "index".

	#. **getDataFileLength** (index = 0) [Retorna un (long)]
		Tamaño del archivo contenodo de índice "index".

	#. **getDataFileMimeType** (index = 0) [Retorna cadenas (String)]
		Tipo de archivo contenido de índice "indice".	

	#. **numberOfDataFileAttributes** (index = 0) [Retorna un entero (int)]
		Atributo del archivo contenido de índice "indice".

	#. **getXmlQuery** (query,dcount,info = "") [Retorna cadenas (String)]
		Obtiene consulta **XML**.	

	#. **addSignatureToExistingDigidocFile** (keyFile,passwd,certFile,inFile,outFile) [Retorna un entero (int)]
		Agrega un firma al archivo.

	#. **signWithPrivateKeyOnFile** (keyFile,passwd,certFile,inFile,outFile,manifest,city,state,zip,country,notaryUrl,proxyHost,proxyPort)[Retorna un entero (int)]
		Firma electrónicamente un archivo con un certificado PCKS#LZ.

	#. **initPKCS11Lib** (libName) [Procedimiento]
		Inicia el manejador de la tarjeta inteligente.

	#. **initDigidocConfigStore** (configFile) [Retorna un entero (int)]
		Inicializa la configuración.

	#. **verifySignMadeWithSmartCard** (fileName) [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]
	#. **verifySignMadeWithSmartCard** (fileName,signatureIndex)  [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]
	#. **verifySignMadeWithSmartCard** (fileName,listOfSigners,isneg,op = SafetDocument::AND )  [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]

		Verifica una firma electrónica que fue realizada utilizando tarjeta inteligente.


	#. **testSmartCardDriver** (slot) [Procedimiento]
		Realiza una prueba a la conexión con la tarjeta inteligente.

	#. **getCN** (signatureIndex) [Retorna cadenas (String)]
		Retorna el nombre común de la firma **"signature Index" (commun name)** .
	
	#. **getSignerIndex** (cn) [Retorna un entero (int)]
		Retorna el índice de firma del correspondiente nombre camún (ch).

	#. **getCommonNameOfSigners** () [Retorna cadenas (String)]
		Obtiene una lista con el nombre camún de los firmantes del archivo.

	#. **getNumberOfPrivateKeys** (slot,passwd,libName) [Retorna un entero (int)]
		Obtiene el número de claves privadas utilizados para el archivo.	

	#. **writeFileToDisk** (string,name) [Retorna cadenas (String)]
		Escribe en otro archivo el documento.
	
	#. **returnFileToString** (pathFileName) [Retorna cadenas (String)]
		Convierte el archivo a una cadena.
	
	#. **decryptDocument** (inFile,outputFile,pin) [Retorna un entero (int)]
		Descifra (decencripta) el archivo.	

	#. **decryptString** (inFile,pin)[Retorna cadenas (String)]
		Descifra (decencripta) una cadena.

	#. **getTempNameFiles** (n) [Retorna cadenas (String)]
		Crea "n" archivos temporales.

	#. **getCommonNameFromCertificate** (certPath) [Retorna cadenas (String)]
		Obtiene el nombre camún (CN) de un archivo de certificación.	
	
	#. **getCountryOfSignature** (index = 0) [Retorna cadenas (String)]
		Obtiene el valor de país **"country"** especificado para la frima electrónica de índice **"Index"**.

	#. **getStateOfSignature** (index = 0) [Retorna cadenas (String)]
		Obtiene el valor del Estado **(STATE)** departamento o región especificando para la firma electrónica de índice "Indice".

	#. **getCityOfSignature** (index = 0) [Retorna cadenas (String)]
		Obtiene el valor de la Ciudad.	

	#. **getPostalCodeOfSignature** (index = 0) [Retorna cadenas (String)]
		Obtiene el valor de código Postal.
	
	#. **getCountOfRoles** (index = 0) [Retorna un entero (int)]
		Obtiene el número de roles (nominación de cargo del firmante).

	#. **getRole** (index = 0,roleIndex = 0) [Retorna cadenas (String)]


	#. **getSingingTime** (index = 0) [Retorna cadenas (String)]
	#. **getSingingTimeOnlyDate** (index = 0) [Retorna cadenas (String)]
	#. **getSingingTimeOnlyHour** (index = 0) [Retorna cadenas (String)]
	#. **getSignatureType** (index = 0) [Retorna cadenas (String)]
	#. **getSignatureFormat** (index = 0) [Retorna cadenas (String)]
	#. **getSignerCertificateIssuerName** (index = 0) [Retorna cadenas (String)]
	#. **getSignerCertificateSerial** (index = 0) [Retorna cadenas (String)]
	#. **getValidAt** (index = 0) [Retorna cadenas (String)]
	#. **getValidUntil** (index = 0) [Retorna cadenas (String)]
		Obtiene datos del certificado electrónico usado para la firma de índice "Indice".	
	
	#. **getPathOfSafetDocument** () [Retorna cadenas (String)]
		Obtiene la ruta donde se guarda los documentos firmados.

	#. **setPathOfSafetDocument** (path) [Procedimiento]


	#. **getSubjectDN** (index = 0) [Retorna cadenas (String)]

	#. **getIssuerDN** (index = 0) [Retorna cadenas (String)]

	#. **policies** (index = 0) [Retorna cadenas (String)]


	#. **getPublicKeyAlgorithm** (index = 0) [Retorna cadenas (String)]

	#. **getPublicKeyLength** (index = 0) [Retorna cadenas (String)]

	#. **writeX509ToFile** (path,PEM,index = 0) [Procedimiento]


	QByteArray versionNumber(int index = 0)

	QByteArray serialNumber(int index = 0);

	QByteArray toHex( const QByteArray &in, QChar separator );


	QByteArray authorityKeyIdentifier(int index = 0);

	QByteArray subjectKeyIdentifier(int index = 0);

	


