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
#include <QSqlField>
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "SslCertificate.h"
#endif

#include <QSslKey>

#include "SafetDocument.h"
#include "SafetYAWL.h"

#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
X509_STORE *SafetDocument::store = NULL;
X509_LOOKUP *SafetDocument::lookup = NULL;
X509_STORE_CTX *SafetDocument::verify_ctx = NULL;
#endif // OpenSSL

QString SafetDocument::storepath = "";
QMap<int,QString> SafetDocument::digidocErrorStringsSpanish = QMap<int,QString>();


// definicion de un slot invalido para gestion de tarjetas inteligentes
#define INVALID_SLOTIID       1000
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
static CK_FUNCTION_LIST_PTR ckFunc = 0;
#endif 

static QByteArray ASN_STRING_to_QByteArray( ASN1_OCTET_STRING *str )
{ return QByteArray( (const char *)ASN1_STRING_data(str), ASN1_STRING_length(str) ); }
#endif // OpenSSL

SafetDocument::SafetDocument() {
	
	_key = "";
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	pSigDoc = NULL;
        pDataFile = NULL;
        pSigInfo = NULL;
#endif // DIGIDOC
	contentTypeArray[0] = "DETATCHED";
	contentTypeArray[1] = "EMBEDDED";
	contentTypeArray[2] = "EMBEDDED_BASE64";


}


SafetDocument::~SafetDocument() {
/*
    if (pSigDoc) {
        delete pSigDoc;
    }

    if (pDataFile) {
        delete pDataFile;
    }
    if (pSigInfo) {
        delete pSigInfo;
    }
    if (SafetDocument::store) {
        delete SafetDocument::store;
    }
    if (SafetDocument::lookup) {
        delete SafetDocument::lookup;
    }
    if (SafetDocument::verify_ctx) {
        delete SafetDocument::verify_ctx;
    }
*/
}


QString SafetDocument::key() const {
	return _key;
}

void SafetDocument::setKey(QString k){
	 _key = k;
}

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
SignedDoc * SafetDocument::signedDoc(){
	return pSigDoc;
}

void SafetDocument::setSignedDoc(SignedDoc * sigDoc){
	pSigDoc = sigDoc;
}
#endif // DIGIDOC

void SafetDocument::configureDigidocErrorString() {
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    digidocErrorStringsSpanish[ERR_DOC_DIGEST] = trUtf8("Fallas en la integridad "
                                                    "del documento Firmado (ddoc)."
                                                 "Posiblemente fue modificado "
                                                 "el documento Firmado (ddoc).");
    digidocErrorStringsSpanish[ERR_FILE_READ] = trUtf8("No se puede abrir el archivo para LECTURA"
                                                 );

    digidocErrorStringsSpanish[ERR_FILE_WRITE] = trUtf8("No se puede abrir el archivo para ESCRITURA"
                                                 );

    digidocErrorStringsSpanish[ERR_WRONG_URL_OR_PROXY] = trUtf8("No es posible realizar una conexión con el"
                                                            " Servidor OCSP o con su Servidor Proxy"
                                                 );
    digidocErrorStringsSpanish[ERR_PKCS_WRONG_SLOT] = trUtf8("Falla con el driver PKCS#11 (Tarjeta Inteligente) "
                                                            "Revise la conexión e instalación del driver (manejador)"
                                                 );
    digidocErrorStringsSpanish[ERR_PKCS_LOGIN] = trUtf8("No está insertada la tarjeta, el PIN es erróneo o está bloqueado"
                                                 );
    digidocErrorStringsSpanish[ERR_UNSUPPORTED_DIGEST] = trUtf8("Reseña (hash) no soportado.");
    digidocErrorStringsSpanish[ERR_DIGEST_LEN] = trUtf8("Longitud de la reseña (hash) incorrecta.");
    digidocErrorStringsSpanish[ERR_BUF_LEN] = trUtf8("Longitud del buffer de memoria insuficiente.");
    digidocErrorStringsSpanish[ERR_SIGNATURE_LEN] = trUtf8("Longitud de firma incorrecta.");
    digidocErrorStringsSpanish[ERR_PRIVKEY_READ] = trUtf8("No se puede leer la clave privada");
    digidocErrorStringsSpanish[ERR_PUBKEY_READ] = trUtf8("No se puede leer la clave pública");
    digidocErrorStringsSpanish[ERR_CERT_READ] = trUtf8("No se puede leer el certificado");
    digidocErrorStringsSpanish[ERR_SIGNEDINFO_CREATE] = trUtf8("No se puede crear el objeto SignedInfo en archivo .ddoc");
    digidocErrorStringsSpanish[ERR_SIGNEDINFO_DATA] = trUtf8("No se puede crear el objeto SignedInfo en archivo .ddoc");
    digidocErrorStringsSpanish[ERR_SIGNEDINFO_FINAL] = trUtf8("No se puede crear el objeto SignedInfo en archivo .ddoc");
    digidocErrorStringsSpanish[ERR_UNSUPPORTED_FORMAT] = trUtf8("El formato del documento firmado no es correcto.");
    digidocErrorStringsSpanish[ERR_BAD_INDEX] = trUtf8("Indice incorrecto.");
    digidocErrorStringsSpanish[ERR_TIMESTAMP_DECODE] = trUtf8("No se puede decodificar la estampilla de tiempo.");
    digidocErrorStringsSpanish[ERR_DIGIDOC_PARSE] = trUtf8("No se puede analizar sintácticamente el documento.");
    digidocErrorStringsSpanish[ERR_UNSUPPORTED_SIGNATURE] = trUtf8("El tipo de firma no está soportado");
    digidocErrorStringsSpanish[ERR_CERT_STORE_READ] = trUtf8("No se puede leer el certificado desde el contenedor de certificados");
    digidocErrorStringsSpanish[ERR_SIGPROP_DIGEST] = trUtf8("Reseña (hash) de las propiedades de la firma incorrecta.");
    digidocErrorStringsSpanish[ERR_COMPARE] = trUtf8("Firma incorrecta.");
    digidocErrorStringsSpanish[ERR_DOC_DIGEST] = trUtf8("Reseña (hash) del documento incorrecta.");
    digidocErrorStringsSpanish[ERR_MIME_DIGEST] = trUtf8("Reseña (hash) del tipo mime del documento incorrecta.");
    digidocErrorStringsSpanish[ERR_SIGNATURE] = trUtf8("Firma incorrecta.");
    digidocErrorStringsSpanish[ERR_CERT_INVALID] = trUtf8("Certificado inválido.");
    digidocErrorStringsSpanish[ERR_OCSP_UNSUCCESSFUL] = trUtf8("Solicitud OCSP sin éxito.");
    digidocErrorStringsSpanish[ERR_OCSP_UNKNOWN_TYPE] = trUtf8("Tipo de OCSP desconocido.");
    digidocErrorStringsSpanish[ERR_OCSP_NO_BASIC_RESP] = trUtf8("No hay respuesta básica del OCSP.");
    digidocErrorStringsSpanish[ERR_OCSP_WRONG_VERSION] = trUtf8("Versión de OCSP incorrecta.");
    digidocErrorStringsSpanish[ERR_OCSP_WRONG_RESPID] = trUtf8("ID de respuesta OCSP incorrecto.");
    digidocErrorStringsSpanish[ERR_OCSP_ONE_RESPONSE] = trUtf8("Cuenta de respuesta OCSP incorrecta.");
    digidocErrorStringsSpanish[ERR_OCSP_RESP_STATUS] = trUtf8("Estatus de respuesta OCSP incorrecta.");
    digidocErrorStringsSpanish[ERR_OCSP_NO_SINGLE_EXT] = trUtf8("No hay respuesta única de OCSP.");
    digidocErrorStringsSpanish[ERR_OCSP_NO_NONCE] = trUtf8("No hay extensión NONCE de OCSP.");
    digidocErrorStringsSpanish[ERR_NOTARY_NO_SIGNATURE] = trUtf8("No hay firma en la notarización.");
    digidocErrorStringsSpanish[ERR_NOTARY_SIG_MATCH] = trUtf8("Firma de notarizació no coincide.");
    digidocErrorStringsSpanish[ERR_SIGNERS_CERT_NOT_TRUSTED] = trUtf8("Certificado firmante no confiable, No hay certificado de AC emisor.");
    digidocErrorStringsSpanish[ERR_WRONG_CERT] = trUtf8("Certificado incorrecto.");
    //digidocErrorStringsSpanish[ERR_WRONG_CERT] = trUtf8("Excepción de puntero NULO.");
    digidocErrorStringsSpanish[ERR_NULL_CERT_POINTER] = trUtf8("El puntero al certificado es NULO.");
    digidocErrorStringsSpanish[ERR_NULL_SER_NUM_POINTER] = trUtf8("El puntero al número de certificado es NULO.");
    digidocErrorStringsSpanish[ERR_NULL_KEY_POINTER] = trUtf8("El puntero a la clave es NULO.");
    digidocErrorStringsSpanish[ERR_EMPTY_STRING] = trUtf8("Cadena vacía.");
    digidocErrorStringsSpanish[ERR_BAD_DATAFILE_INDEX] = trUtf8("El índice de DataFile está fuera de rango.");
    digidocErrorStringsSpanish[ERR_BAD_DATAFILE_COUNT] = trUtf8("La cuenta de DataFiles no está sincronizada.");
    digidocErrorStringsSpanish[ERR_BAD_ATTR_COUNT] = trUtf8("El contador de atributos no está sincronizado.");
    digidocErrorStringsSpanish[ERR_BAD_ATTR_INDEX] = trUtf8("El índice del atributo está fuera de rango.");
    digidocErrorStringsSpanish[ERR_BAD_SIG_INDEX] = trUtf8("El índice de la firma está fuera de rango.");
    digidocErrorStringsSpanish[ERR_BAD_SIG_COUNT] = trUtf8("El número de firmas no está sincronizada.");
    digidocErrorStringsSpanish[ERR_BAD_ROLE_INDEX] = trUtf8("Indice del rol está fuera del rango.");
    digidocErrorStringsSpanish[ERR_BAD_DOCINFO_COUNT] = trUtf8("El número de info del documento no está sincronizado.");
    digidocErrorStringsSpanish[ERR_BAD_DOCINFO_INDEX] = trUtf8("Indice de info del documento está fuera del rango.");
    digidocErrorStringsSpanish[ERR_BAD_NOTARY_INDEX] = trUtf8("Indice de notarización fuera de rango.");
    digidocErrorStringsSpanish[ERR_BAD_NOTARY_ID] = trUtf8("ID de notarización incorrecto.");
    digidocErrorStringsSpanish[ERR_BAD_NOTARY_COUNT] = trUtf8("El número de notarización no está sincronizado.");
    digidocErrorStringsSpanish[ERR_BAD_NOTARY_COUNT] = trUtf8("No se puede realizar la creación de la reseña (hash) X509.");
    digidocErrorStringsSpanish[ERR_CERT_LENGTH] = trUtf8("Longitud de certificado incorrecta.");
    digidocErrorStringsSpanish[ERR_PKCS_LIB_LOAD] = trUtf8("No se puede cargar la librería PKCS#11.");
    digidocErrorStringsSpanish[ERR_PKCS_SLOT_LIST] = trUtf8("No se puede obtener una lista de slots PKCS#11.");
    digidocErrorStringsSpanish[ERR_PKCS_PK] = trUtf8("No se puede realizar la búsqueda de la clave privada en la tarjeta");
    digidocErrorStringsSpanish[ERR_PKCS_CERT_LOC] = trUtf8("No se puede realizar la lectura de certificado desde la tarjeta.");
    digidocErrorStringsSpanish[ERR_PKCS_CERT_DECODE] = trUtf8("No se puede realizar la decodificación de certificado.");
    digidocErrorStringsSpanish[ERR_PKCS_SIGN_DATA] = trUtf8("No se puede realizar la firma de datos con la tarjeta inteligente.");
    digidocErrorStringsSpanish[ERR_PKCS_CARD_READ] = trUtf8("No se puede realizar la lectura de la tarjeta.");
    digidocErrorStringsSpanish[ERR_CSP_NO_CARD_DATA] = trUtf8("No se puede detectar una tarjeta inteligente o no puede ser leida.");
    digidocErrorStringsSpanish[ERR_CSP_OPEN_STORE] = trUtf8("No se puede abrir el repositorio del sistema.");
    digidocErrorStringsSpanish[ERR_CSP_CERT_FOUND] = trUtf8("No se puede encontrar certificado desde el repositorio, probablemente el certificado no está registrado.");
    digidocErrorStringsSpanish[ERR_CSP_SIGN] = trUtf8("No se puede realizar la firma con CSP.");
    digidocErrorStringsSpanish[ERR_CSP_NO_HASH_START] = trUtf8("No se puede iniciar la construcción de la reseña (hash) CSP.");
    digidocErrorStringsSpanish[ERR_CSP_NO_HASH] = trUtf8("No se pudo realizar la reseña (hash) CSP.");
    digidocErrorStringsSpanish[ERR_CSP_NO_HASH_RESULT] = trUtf8("No se puede leer el resultado de la reseña (hash) CSP.");
    digidocErrorStringsSpanish[ERR_CSP_OPEN_KEY] = trUtf8("No se puede abrir la clave.");
    digidocErrorStringsSpanish[ERR_CSP_READ_KEY] = trUtf8("No se puede leer la clave.");
    digidocErrorStringsSpanish[ERR_OCSP_SIGN_NOT_SUPPORTED] = trUtf8("El método de firma de OCSP solicitado no está soportado.");
    digidocErrorStringsSpanish[ERR_OCSP_SIGN_CSP_NAME] = trUtf8("No se puede agregar el nombre del firmante a la solicitud.");
    digidocErrorStringsSpanish[ERR_CSP_CERT_DECODE] = trUtf8("No se puede decodificar el certificado.");
    digidocErrorStringsSpanish[ERR_OCSP_SIGN_PKCS_NAME] = trUtf8("No se puede agregar el nombre del firmante a la solicitud");
    digidocErrorStringsSpanish[ERR_OCSP_SIGN_OSLL_CERT] = trUtf8("No se puede agregar el certificado a la solicitud OCSP.");
    digidocErrorStringsSpanish[ERR_OCSP_SIGN] = trUtf8("No se puede firmar la solicitud OCSP.");
    digidocErrorStringsSpanish[ERR_CERT_ISSUER] = trUtf8("Certificado no emitido por esta AC, o firma de certificado incorrecta.");
    digidocErrorStringsSpanish[ERR_OCSP_PKCS12_CONTAINER] = trUtf8("No se puede abrir el contenedor PKCS#12.");
    digidocErrorStringsSpanish[ERR_MODIFY_SIGNED_DOC] = trUtf8("No se puede modificar el documento firmado. Remueva la firma primero.");
    digidocErrorStringsSpanish[ERR_NOTARY_EXISTS] = trUtf8("No se puede remover la firma si la notarización existe.");
    digidocErrorStringsSpanish[ERR_UNSUPPORTED_CERT_SEARCH] = trUtf8("El método de búsqueda de certificado no está soportado.");
    digidocErrorStringsSpanish[ERR_INCORRECT_CERT_SEARCH] = trUtf8("El patrón de búsqueda de certificado está incorrecto.");
    digidocErrorStringsSpanish[ERR_BAD_OCSP_RESPONSE_DIGEST] = trUtf8("La reseña de la firma de la notarización está incorrecta.");
    digidocErrorStringsSpanish[ERR_LAST_ESTID_CACHED] = trUtf8("Certificado incorrecto en cache. Por favor intente de nuevo.");
    digidocErrorStringsSpanish[ERR_BAD_DATAFILE_XML] = trUtf8("El contenido XML no puede incluir la primera línea XML.");
    digidocErrorStringsSpanish[ERR_UNSUPPORTED_VERSION] = trUtf8("Versión SK-XML no soportada. Por favor actualizarla.");
    digidocErrorStringsSpanish[ERR_UNSUPPORTED_CHARSET] = trUtf8("Juego de caracteres no está soportado.");
    digidocErrorStringsSpanish[ERR_PKCS12_EXPIRED] = trUtf8("El certificado PKCS#12 está expirado. Obtenga uno nuevo.");
    digidocErrorStringsSpanish[ERR_CSP_USER_CANCEL] = trUtf8("El usuario canceló la selección de certificado.");
    digidocErrorStringsSpanish[ERR_CSP_NODEFKEY_CONTAINER] = trUtf8("No se puede encontrar el contenedor de claves por defecto.");
    digidocErrorStringsSpanish[ERR_CONNECTION_FAILURE] = trUtf8("Error de conexión.");
    digidocErrorStringsSpanish[ERR_NULL_PARAM] = trUtf8("Parámetro obligatorio está en NULO.");
    digidocErrorStringsSpanish[ERR_BAD_ALLOC] = trUtf8("Error de asignación de memoria.");
    digidocErrorStringsSpanish[ERR_CONF_FILE] = trUtf8("Error al abrir el archivo de configuración.");
    digidocErrorStringsSpanish[ERR_CONF_LINE] = trUtf8("Error en archivo de configuración.");
    digidocErrorStringsSpanish[ERR_OCSP_CERT_REVOKED] = trUtf8("El certificado ha sido revocado.");
    digidocErrorStringsSpanish[ERR_OCSP_CERT_UNKNOWN] = trUtf8("El estatus del certificado es desconocido. Soportado por esta AC?.");
    digidocErrorStringsSpanish[ERR_OCSP_PKCS12_NO_FILE] = trUtf8("Archivo de token PKCS#12 no definido.");
    digidocErrorStringsSpanish[ERR_OCSP_PKCS12_NO_PASSWD] = trUtf8("Contraseña del archivo de token PKCS#12 no definido.");
    digidocErrorStringsSpanish[ERR_BAD_DATAFILE_CONTENT_TYPE] = trUtf8("El tipo de contenido del DataFile es inválido.");
    digidocErrorStringsSpanish[ERR_OCSP_WRONG_URL] = trUtf8("La URL del OCSP no es correcta.");
    digidocErrorStringsSpanish[ERR_OCSP_MALFORMED] = trUtf8("La consulta OCSP no está bien formada.");
    digidocErrorStringsSpanish[ERR_OCSP_INTERNALERR] = trUtf8("Error interno en la consulta OCSP.");
    digidocErrorStringsSpanish[ERR_OCSP_TRYLATER] = trUtf8("Intente de nuevo, el OCSP responder está ocupado.");
    digidocErrorStringsSpanish[ERR_OCSP_SIGREQUIRED] = trUtf8("Se debe firmar la consulta OCSP.");
    digidocErrorStringsSpanish[ERR_OCSP_UNAUTHORIZED] = trUtf8("Consulta OCSP no autorizada.");
    digidocErrorStringsSpanish[ERR_UNKNOWN_CA] = trUtf8("Emisor de certificado desconocido.");
    digidocErrorStringsSpanish[ERR_DENC_ENC_METHOD] = trUtf8("El método de cifrado es inválido.");
    digidocErrorStringsSpanish[ERR_DENC_ENC_XMLNS] = trUtf8("El espacio de nombres XML es inválido.");
    digidocErrorStringsSpanish[ERR_DENC_BAD_PROP_IDX] = trUtf8("El índice ERR_DENC_BAD_PROP_IDX es inválido.");
    digidocErrorStringsSpanish[ERR_DENC_BAD_KEY_IDX] = trUtf8("El índice ERR_DENC_BAD_KEY_IDX es inválido");
    digidocErrorStringsSpanish[ERR_DENC_KEY_STATUS] = trUtf8("La clave de transporte no está lista.");
    digidocErrorStringsSpanish[ERR_DENC_DATA_STATUS] = trUtf8("El estatus de los datos está inválido para esta operación.");
    digidocErrorStringsSpanish[ERR_DENC_DATA_STATUS] = trUtf8("No se pudo descifrar los datos.");
    digidocErrorStringsSpanish[ERR_CHARSET_CONVERT] = trUtf8("No se puede convertir el juego de caracteres.");
    digidocErrorStringsSpanish[ERR_COMPRESS] = trUtf8("No se pueden comprimir los datos.");
    digidocErrorStringsSpanish[ERR_DECOMPRESS] = trUtf8("No se pueden descomprimir los datos.");
    digidocErrorStringsSpanish[ERR_OCSP_CERT_NOTFOUND] = trUtf8("No se ha encontrado el certificado del OCSP responder.");
    digidocErrorStringsSpanish[ERR_INVALID_CONTENT] = trUtf8("Caracteres inválidos en el manifiesto o dirección.");
    digidocErrorStringsSpanish[ERR_DENC_NO_KEY_FOUND] = trUtf8("No hay clave de transporte para esta tarjeta inteligente.");
    digidocErrorStringsSpanish[ERR_OCSP_RESP_NOT_TRUSTED] = trUtf8("El OCSP no es confiable, No hay ningún certificado asociado a él en el repositorio de certificados locales.");
    digidocErrorStringsSpanish[ERR_PRIV_CERT_NOT_FOUND] = trUtf8("Certificado no encontrado.");
    digidocErrorStringsSpanish[ERR_NO_OCSP] = trUtf8("La firma no incluye una confirmación OCSP.");
    digidocErrorStringsSpanish[ERR_OCSP_WRONG_SIGNATURE] = trUtf8("La firma del OCSP es incorrecta.");
    digidocErrorStringsSpanish[ERR_BAD_PARAM] = trUtf8("Parámetro inválido.");
    digidocErrorStringsSpanish[ERR_GENERIC_SOAP_ERR] = trUtf8("Error SOAP genérico.");
    digidocErrorStringsSpanish[ERR_TS_TIMESTAMPINFO_TYPE] = trUtf8("Tipo de estampilla de tiempo inválida.");
    digidocErrorStringsSpanish[ERR_TS_BAD_INCLUDEINFO_IDX] = trUtf8("El índice Include es inválido");
    digidocErrorStringsSpanish[ERR_TS_BAD_TIMESTAMPINFO_IDX] = trUtf8("El índice TimestampInfo es inválido.");
    digidocErrorStringsSpanish[ERR_TS_CREATE_TS_REQ] = trUtf8("Error creando solicitud de estampillado de tiempo.");
    digidocErrorStringsSpanish[ERR_CREATE_NONCE] = trUtf8("Error creando NONCE.");
    digidocErrorStringsSpanish[ERR_TXT2OID] = trUtf8("Error convirtiendo texto a OID.");
    digidocErrorStringsSpanish[ERR_HTTP_ERR] = trUtf8("El código de respuesta HTTP es inválido.");
    digidocErrorStringsSpanish[ERR_BAD_CERTID_IDX] = trUtf8("El índice Include es inválido");
    digidocErrorStringsSpanish[ERR_BAD_CERTVALUE_IDX] = trUtf8("El índice Include es inválido");
    digidocErrorStringsSpanish[ERR_TS_VERIFY] = trUtf8("Error de verificación de estampillado de tiempo.");
    digidocErrorStringsSpanish[ERR_TS_REQUEST] = trUtf8("Error obteniendo estampilla de tiempo.");
    digidocErrorStringsSpanish[ERR_TSA_NOT_TRUSTED] = trUtf8("Autoridad de Estampillado de Tiempo (TSA) no es confiable, no hay certificado en el repositorio de certificados locales.");
    digidocErrorStringsSpanish[ERR_ORPHONED_SIGNATURE] = trUtf8("Firma incompleta o huérfana.");
    digidocErrorStringsSpanish[ERR_WPKI_UNKNOWN_USER] = trUtf8("Usuario WPKI desconocido.");
    digidocErrorStringsSpanish[ERR_WPKI_INVALID_PHONE_NO] = trUtf8("El número de teléfono del usuario no es válido.");
    digidocErrorStringsSpanish[ERR_WPKI_UNTRUSTED_SRVICE] = trUtf8("Servicio WPKI no es confiable.");
    digidocErrorStringsSpanish[ERR_WPKI_UNTRUSTED_USER] = trUtf8("El servicio demanda autenticación del cliente.");
    digidocErrorStringsSpanish[ERR_WPKI_UNUSABLE_PHONE] = trUtf8("Error firmando con el teléfono celular del cliente, tipo de teléfono no usable?.");
    digidocErrorStringsSpanish[ERR_WPKI_TIMEOUT] = trUtf8("Expiró el tiempo durante la firma móvil.");
    digidocErrorStringsSpanish[ERR_WPKI_CANCELLED] = trUtf8("El usuario canceló la firma móvil.");
    digidocErrorStringsSpanish[ERR_WPKI_MID_NOT_READY] = trUtf8("MID no está listo.");
    digidocErrorStringsSpanish[ERR_WPKI_PHONE_NOT_REACHABLE] = trUtf8("El teléfono del usuario no está al alcance.");
    digidocErrorStringsSpanish[ERR_WPKI_SENDING_ERROR] = trUtf8("Error enviando solicitud de firma al teléfono celular del usuario.");
    digidocErrorStringsSpanish[ERR_WPKI_SIM_ERROR] = trUtf8("Error de tarjeta SIM.");
    digidocErrorStringsSpanish[ERR_WPKI_SERVICE_ERR] = trUtf8("Error interno del servicio de firma móvil.");
#endif // DIGIDOC

}

QString SafetDocument::digidocStringError(int err) {
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    if ( digidocErrorStringsSpanish.contains(err)) {
        return digidocErrorStringsSpanish[err];
    }
#endif // DIGIDOC
    return QString("");


}

void SafetDocument::openssl_error(const QString txt) {
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
        QString errtxt = "";
        QString error = "";

        while (int i = ERR_get_error() ) {
                errtxt = ERR_error_string(i ,NULL);
                fprintf(stderr, "OpenSSL error: %s\n", ERR_error_string(i ,NULL) );
                error += errtxt + "\n";
        }
        if (!error.isEmpty()) {
                if (!txt.isEmpty())
                        error = txt + "\n" + error;                
        }
#endif // OpenSSL
}

QString SafetDocument::opensslStringError() {
QString errtxt = "";
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
//        QString errtxt = "";

        while (int i = ERR_get_error() ) {
                errtxt += ERR_error_string(i ,NULL);
                errtxt += "\n";
        }
#endif
        return errtxt;
}


bool SafetDocument::ign_openssl_error()
{
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
        // ignore openssl errors
        QString errtxt;
        while (int i = ERR_get_error() ) {
           errtxt = ERR_error_string(i ,NULL);
           //fprintf(stderr,"IGNORED: %s\n", CCHAR(errtxt));
        }
        return !errtxt.isEmpty();
#endif
}

void SafetDocument::fopen_error(const QString fname) {
      SYE << tr("Error al abrir el archivo: \"%1\"").arg(fname);
}

QStringList SafetDocument::getCertFileList(const QString& ext) {
        QStringList result;
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL

        QString curext = ext;
        if (curext.isEmpty() ) curext = ".pem";
        const char* p2;


        storepath = SafetYAWL::getConf()["Libdigidoc/x509storepath"];
        if ( storepath.isEmpty()) {
            storepath = QDir::homePath()+ "/" + Safet::datadir+"/"+Safet::certdir;
            qDebug("...storepath: %s",qPrintable(storepath));

        }

        p2 = qPrintable(storepath);
        if ( p2 == NULL ) {
            SafetYAWL::streamlog << SafetLog::Warning << tr("no se especificó la variable del repositorio X509 correctamente");
            return result;
        }

        //QString capath = QString("%1/%2").arg(p2).arg("accenditel.pem");

        QDir dir(p2);
        if ( !dir.exists() ) {
            SafetYAWL::streamlog << SafetLog::Warning << tr("El repositorio de certificados x509 no existe.");
            return result;
        }
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);


        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);
                if ( fileInfo.fileName().endsWith(curext) ) {
                        result.append(fileInfo.absoluteFilePath());
                    //qDebug("...fileInfo.absoluteFilePath(): %s", qPrintable(fileInfo.absoluteFilePath()));
                }
        }
#endif // OpenSSL
        return result;
}

#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
X509* SafetDocument::loadX509FromFile(const QString& filename) {

        FILE *fp = fopen(filename.toLatin1(),"r");
        X509 *cert = NULL;
        if (!fp) {
                fopen_error(filename);
                return NULL;
        }
        cert = PEM_read_X509(fp, NULL, NULL, NULL);
        if (!cert) {
                ign_openssl_error();
                rewind(fp);
                cert = d2i_X509_fp(fp, NULL);
        }
        fclose(fp);
        openssl_error(filename);
        return cert;
}


int
verify_callback (int ok, X509_STORE_CTX * stor)
{
  if (!ok)
    fprintf (stderr, "Error: %s\n",
             X509_verify_cert_error_string (stor->error));
  return ok;
}

void
handle_error (const char *file, int lineno, const char *msg)
{
  fprintf (stderr, "** %s:%i %s\n", file, lineno, msg);
  ERR_print_errors_fp (stderr);
  exit (-1);
}

#define int_error(msg) handle_error(__FILE__, __LINE__, msg)
#endif // OpenSSL

void SafetDocument::setupLocalX509Store() {
#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
    OpenSSL_add_all_algorithms ();
    ERR_load_crypto_strings ();
    int ret;
//    QString CA_FILE = "/home/vbravo/.safet/certs/accenditel.pem";
//    QString CA_DIR = "/home/vbravo/.safet/certs";
//    QString CRL_FILE = "/home/vbravo/.safet/certs/crl_accenditel.crl";
    if ( SafetDocument::store != NULL ) {
        delete SafetDocument::store;
    }
    if ( SafetDocument::lookup != NULL ) {
        delete SafetDocument::lookup;
    }

    SafetYAWL::streamlog << SafetLog::Action
            << tr("Inicializando lectura del repositorio X509...ok!");

    if (!(SafetDocument::store = X509_STORE_new ()))
      int_error ("Error creating X509_STORE_CTX object");
    X509_STORE_set_verify_cb_func (SafetDocument::store, verify_callback);
    /* load the CA certificates and CRLs */
     qDebug("SAFETDOCUMENT...entrando....(1)");
     QStringList locations = getCertFileList(".pem");
     qDebug("SAFETDOCUMENT...entrando....(1)...locations.count(): %d",locations.count());
     foreach(QString l, locations) {
         SafetYAWL::streamlog
                 << SafetLog::Debug
                 << tr("Colocando ruta (location) para los certificados (pem): \"%1\"")
                 .arg(l);

        if ((ret = X509_STORE_load_locations (SafetDocument::store, qPrintable(l), NULL)) != 1) {
            SafetYAWL::streamlog << SafetLog::Error << tr("Error al cargar repositorios de certificados de AC: "
                                                       "\"%1\"").arg(l);
            SafetYAWL::streamlog
                    << SafetLog::Error

                    << tr("Error loading the CA file or directory ret: \"%1\"").arg(ret);
            return;
        }
        else {
            SafetYAWL::streamlog << SafetLog::Debug << tr("Cargando satisfactoriamente "
                                                          "el certificado: \"%1\"").arg(l);

        }
    }

     qDebug("...entrando....(2)...ret:%d",ret);
    if (X509_STORE_set_default_paths (SafetDocument::store) != 1) {
        qDebug("Error loading the system-wide CA certificates");
    }
    if (!(SafetDocument::lookup = X509_STORE_add_lookup (SafetDocument::store, X509_LOOKUP_file ()))) {
        qDebug("Error creating X509_LOOKUP object");
    }

    locations = getCertFileList(".crl");
    foreach(QString l, locations) {
        SafetYAWL::streamlog
                << SafetLog::Debug
                << tr("Colocando ruta (location) para los certificados (crl): \"%1\"")
                .arg(l);

        if ((ret = X509_load_crl_file (SafetDocument::lookup, qPrintable(l), X509_FILETYPE_PEM)) != 1) {
            SafetYAWL::streamlog << SafetLog::Error << tr("Error al cargar archivo de CRL: "
                                                          "\"%1\"").arg(l);
            return;
        }
        else {
            SafetYAWL::streamlog << SafetLog::Debug << tr("Cargando satisfactoriamente la CRL: \"%1\""
                                                       "\"%1\"").arg(l);
        }
    }
//
    /* set the flags of the store so that CRLs are consulted */
        X509_STORE_set_flags (SafetDocument::store, X509_V_FLAG_CRL_CHECK |
                              X509_V_FLAG_CRL_CHECK_ALL);
#endif // OpenSSL

}

QBitArray SafetDocument::verifyOCSPSignBit()  {

    QBitArray result;
    QList<bool> mylist = verifyOCSPSign();
    int i = 0;
    result.resize(mylist.count());
    foreach(bool b, mylist) {
        result.setBit(i,b);
        i++;
    }

    return result;
}

QList<bool> SafetDocument::verifyOCSPSign()  {
    QList<bool> resultlist;
#ifdef SAFET_DIGIDOC // Firma Electronica Digidoc
    SignedDoc *pSigDoc = signedDoc();
    if (!pSigDoc) {
        SafetYAWL::streamlog << SafetLog::Warning << tr("No existe un documento válido de firma");
        return resultlist;
    }

    int d = getCountOfSignatures(pSigDoc);

    if ( d <=0) {
        SafetYAWL::streamlog << SafetLog::Warning << tr("El documento no contiene firmas electrónicas");
         return resultlist;
     }

    DigiDocMemBuf mbuf;
    mbuf.pMem = 0;
    mbuf.nLen = 0;

     for(int s = 0; s < d; s++) {
         pSigInfo = getSignature(pSigDoc, s);
         if (pSigInfo == NULL ) {
                          SafetYAWL::streamlog << SafetLog::Warning <<
                                  tr("Firma \%1\" es NULA").arg(s);
                         resultlist.append(false);
                         continue;
         }
         X509* currentSigner = ddocSigInfo_GetSignersCert(pSigInfo);
         if ( currentSigner == NULL ) {
             SafetYAWL::streamlog << SafetLog::Error <<
                     tr("No existe un Certificado de Firmante para la firma \"%1\".")
                     .arg(s);
             resultlist.append(false);
             continue;
         }
         int err = ddocCertGetSubjectCN(currentSigner, &mbuf);
         if ( err != ERR_OK) {
             qDebug(".......**ddocCertGetSubjectCN........err: %d", err);
             QString stringerror = SafetDocument::digidocStringError(err);
             if (stringerror.isEmpty() ) {
                 stringerror = getErrorString(err);
             }

             SafetYAWL::streamlog << SafetLog::Warning <<
                     tr("No existe un Nombre de Sujeto Válido  para la firma \"%1\""
                        ".Error Digidoc: \"%2\"")
                     .arg(s)
                     .arg(stringerror);
             resultlist.append(false);
             continue;

         }


         err = verifySignatureAndNotary(pSigDoc, pSigInfo, qPrintable(m_filename)/*p_szInFile*/);
         qDebug("......SafetDocument::vSAFET_OPENSSLerifyOCSPSign() .verificando "
                "firma error Nro \"%d\" ERR_OK=%d ... err: %d",s,ERR_OK,err);
         if (err != ERR_OK && /* chequear */
             err != ERR_WRONG_CERT  && err != ERR_NULL_PARAM /* chequear */ ) {
             qDebug(".......^*****ddocCertGetSubjectCN........err: %d", err);
             QString stringerror = SafetDocument::digidocStringError(err);
             if (stringerror.isEmpty() ) {
                 stringerror = getErrorString(err);
             }

             SafetYAWL::streamlog << SafetLog::Error <<
                     trUtf8("Ocurrió un Error para "
                        "la firma \"%2\" "
                        "para el sujeto \"%3\": \"%1\" (%4)")
                     .arg(stringerror)
                     .arg(s+1)
                     .arg((const char*)mbuf.pMem)
                     .arg(err);
             resultlist.append(false);

         }

         else {
             //qDebug("....err == ERR_WRONG_CERT  || err == ERR_NULL_PARAM....");
             if ( err == ERR_WRONG_CERT  || err == ERR_NULL_PARAM) {
                 SafetYAWL::streamlog <<
                         SafetLog::Warning
                         << tr("Se está verificando un archivo firmado con un clave en software."
                               "Se agrega información de la biblioteca"
                               "digidoc: \"%1\"")
                         .arg(getErrorString(err));
             }
             NotaryInfo* pNot;
             const DigiDocMemBuf* pMBuf = NULL;
             char buf1[255];

             SafetYAWL::streamlog << SafetLog::Action
                     << tr("Verificada Informacion  Nro \"%1\""
                        " para el Sujeto \"%2\"")
                     .arg(s+1)
                     .arg((const char*)mbuf.pMem);
             // confirmation
             if(pSigDoc && pSigInfo) {
                 pNot = getNotaryWithSigId(pSigDoc, pSigInfo->szId);
                 if(pNot) {
                     pMBuf = ddocNotInfo_GetResponderId(pNot);
                     buf1[0] = 0;
                     int l1 = sizeof(buf1);
                     if(pNot->nRespIdType == RESPID_NAME_TYPE) {
                         strncpy(buf1, (const char*)pMBuf->pMem, l1);                         
                     }
                     if(pNot->nRespIdType == RESPID_KEY_TYPE) {
                         encode((const byte*)pMBuf->pMem, pMBuf->nLen, (byte*)buf1, &l1);
                     }

                 }
                  if(pNot && pMBuf) {
                      resultlist.append(true);
                      SafetYAWL::streamlog << SafetLog::Action
                              << trUtf8("Satisfactoria validación OCSP Nro \"%1\""
                                 " para el Sujeto \"%2\"")
                              .arg(s+1)
                              .arg((const char*)mbuf.pMem);

                  }
                  else {
                      SafetYAWL::streamlog << SafetLog::Error
                              << tr("Fallo validacion OCSP Nro \"%1\""
                                 " para el Sujeto \"%2\"")
                              .arg(s+1)
                              .arg((const char*)mbuf.pMem);
                      resultlist.append(false);
                  }

              } else {
                  SafetYAWL::streamlog << SafetLog::Error
                          << tr("Fallo (lectura archivo ) validacion OCSP Nro \"%1\""
                             " para el Subjeto \"%2\"")
                          .arg(s+1)
                          .arg((const char*)mbuf.pMem);
                  resultlist.append(false);

              }


         }
       }

    ddocMemBuf_free(&mbuf);
#endif // DIGIDOC
    return resultlist;
}

#ifdef SAFET_OPENSSL // CriptoSistema OpenSSL
bool SafetDocument::verifyWithLocalX509Store(X509 *cert){


    if ( SafetDocument::verify_ctx != NULL ) delete SafetDocument::verify_ctx;
   /* create a verification context and initialize it */
     if (!(SafetDocument::verify_ctx = X509_STORE_CTX_new ()))
       int_error ("Error creating X509_STORE_CTX object");
   /* X509_STORE_CTX_init did not return an error condition
   in prior versions */
     if (X509_STORE_CTX_init (SafetDocument::verify_ctx, SafetDocument::store, cert, NULL) != 1) {
         SafetYAWL::streamlog << SafetLog::Error << tr("Error inicializando un contexto de verificación");
         return false;
   }

    SslCertificate c = SslCertificate::fromX509( (Qt::HANDLE)cert );

    int ret;
   if ((ret = X509_verify_cert (SafetDocument::verify_ctx) ) != 1 ) {
       SafetYAWL::streamlog << SafetLog::Warning << tr("Error OpenSSL:\"\%2\". Certificado "
                                                     " NO fue verificado "
                                                     "para el Sujeto: \"%1\"").arg(c.subjectInfoUtf8( QSslCertificate::CommonName ))
                                                 .arg(X509_verify_cert_error_string(verify_ctx->error));

       return false;
   }
     else {
         SafetYAWL::streamlog << SafetLog::Action << tr("Certificado verificado correctamente "
                                                        "para Sujeto: \"%1\"").arg(c.subjectInfoUtf8( QSslCertificate::CommonName ));
     }
     return true;
}


bool SafetDocument::verifySigner(X509 *cert, X509 *signer) {
  if (cert == signer)
                return true;
  if ((cert == NULL )||( signer == NULL))
                return false;
  X509_NAME *subject =  X509_get_subject_name(signer);
  X509_NAME *issuer = X509_get_issuer_name(cert);
  SafetDocument::openssl_error();
  if (X509_NAME_cmp(subject, issuer)) {
      return false;
  }
  int i = X509_verify(cert, X509_get_pubkey(signer));
  ign_openssl_error();
  if (i>0) {      
      return true;
  }
  return false;
}
#endif // OpenSSL

bool SafetDocument::readDigiDocFile(const QString& filename) {
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    int err = ERR_OK;
    int nMaxDfLen;

    if ( !QFile::exists(filename) ) {
        SafetYAWL::streamlog << SafetLog::Error << tr("El archivo digidoc: \"%1\" no puede leerse").arg(filename);
        return false;
    }

    DEncEncryptedData* pEncData = 0;
    DEncEncryptedData** ppEncData = &pEncData;
    nMaxDfLen = ConfigItem_lookup_int("DATAFILE_MAX_CACHE", 0);

    SignedDoc **ppDoc = &pSigDoc;
    if(filename.endsWith(".cdoc")) {
        err = dencSaxReadEncryptedData(ppEncData, qPrintable(filename));
    } else {
            err = ddocSaxReadSignedDocFromFile(ppDoc, qPrintable(filename), 0, nMaxDfLen);
    }

    if ( err != ERR_OK) return false;

    m_filename = filename;
#endif // DIGIDOC
    return true;
}


#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
DataFile * SafetDocument::dataFile(int numfile){
	//return pDataFile;
	// se utiliza la funcion getDataFile de libdigidoc, los argumentos son:
	// el pSigDoc y el identificador del archivo; en este caso se usa 0 porque
	// estamos suponiendo que el contenedor tendra solo un archivo. en caso de();
	// no encontrar archivo dentro del contenedor retorna NULL
        return getDataFile(pSigDoc,numfile);
}

void SafetDocument::setDataFile(DataFile * pDFile){
	pDataFile = pDFile;

}

QList<DataFile *> SafetDocument::dataFilesList(){
    QList<DataFile *> list;
    int number = numberOfDataFileOnOpenXAdESContainer();
    if ( number > 0 ){
        for (int i = 0; i < number; i++){
            list.append(dataFile(number));
        }
    }
    return list;
}



SignatureInfo* SafetDocument::SigInfo(int index){
	
    SignedDoc * pSigDoc = 0;
    pSigDoc = signedDoc();
    Q_CHECK_PTR(pSigDoc);
    return getSignature(pSigDoc, index);
}

void SafetDocument::setSigInfo(SignatureInfo * sigInfo){
	pSigInfo = sigInfo;
}

QList<SignatureInfo *> SafetDocument::signaturesList(){
    QList<SignatureInfo *> list;
    int number = numberOfSignaturesOnOpenXAdESContainer();

    SignatureInfo* pSigInfo = 0;
    SignedDoc * pSigDoc = 0;
    pSigDoc = signedDoc();
    Q_CHECK_PTR(pSigDoc);

    if ( number > 0 ){
        for (int i = 0; i < number; i++){
            pSigInfo = getSignature(pSigDoc, i);
            Q_CHECK_PTR(pSigInfo);
            list.append(pSigInfo);
        }
    }
    return list;
}
#endif // DIGIDOC

void SafetDocument::initializeLibdigidoc(){
	// inicializar la libreria libdigidoc
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	initDigiDocLib();
#endif // DIGIDOC

}

void SafetDocument::closeLibdigidoc(){
	// finaliza o cierra la libreria libdigidoc
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	finalizeDigiDocLib();
#endif
}


int SafetDocument::createOpenXAdESContainer(){

        int err = 0;

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	//initializeLibdigidoc();
	
	// inicializar la estructura de SignedDoc
        err = ERR_OK;
	err = SignedDoc_new(&pSigDoc, DIGIDOC_XML_1_1_NAME, DIGIDOC_XML_1_3_VER);
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::createContainer()", qPrintable(QObject::tr("SignedDoc_new != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, 
	//	tr("SafetDocument::createContainer(): Libdigidoc: SignedDoc_new != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("SafetDocument::createContainer(): Libdigidoc: SignedDoc_new != ERR_OK - %1").arg(getErrorString(err)));


	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Creado Documento Ccontenedor OpenXAdES correctamente...OK!");
#endif // DIGIDOC
	return err;
}

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
int SafetDocument::addFileToOpenXAdESContainer(SignedDoc * pSigDoc, QString fileName, ContentType cType, QString mime){
		
	/*
	int DataFile_new(DataFile **newDataFile, SignedDoc* pSigDoc,
	const char* id, const char* filename,
	const char* contentType,
	const char* mime, long size,
	const byte* digest, int digLen,
	const char* digType, const char* szCharset,
	const char* szFileNameCharset)
	*/
	int err = ERR_OK;
	Q_CHECK_PTR(pSigDoc);
	//Q_ASSERT_X(!fileName.isEmpty(), "SafetDocument::addFileToOpenXAdESContainer()", qPrintable(tr("ContainerFileName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!fileName.isEmpty(), 
	//		tr("La ruta absoluta al archivo que se desea agregar al contenedor OpenXAdES no es correcta. Por favor verifique que no este vacia.")) );
	SafetYAWL::evalAndCheckExit(!fileName.isEmpty(), 
		tr("La ruta absoluta al archivo que se desea agregar al contenedor OpenXAdES no es correcta. Por favor verifique que no este vacia."));

	//Q_ASSERT_X(!mime.isEmpty(), "SafetDocument::addFileToOpenXAdESContainer()", qPrintable(tr("mime is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!mime.isEmpty(), 
	//	tr("El tipo MIME asociado al archivo que se desea agregar al contenedor OpenXAdES no es correcto. Por favor verifique que no este vacio.") ));
	SafetYAWL::evalAndCheckExit(!mime.isEmpty(), 
		tr("El tipo MIME asociado al archivo que se desea agregar al contenedor OpenXAdES no es correcto. Por favor verifique que no este vacio."));
	
	// agregar el archivo al contenedor
	//       DataFile_new(&pDataFile, pSigDoc, NULL, infile, CONTENT_EMBEDDED_BASE64, mime, 0, NULL, 0, NULL, CHARSET_UTF_8);
	err = DataFile_new(&pDataFile, pSigDoc, NULL, qPrintable(fileName), contentTypeArray[cType]/*array[cType]*/, qPrintable(mime), 0, NULL, 0, NULL, CHARSET_UTF_8);
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addFileToOpenXAdESContainer()", qPrintable(QObject::tr("DataFile_new != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("Libdigidoc: DataFile_new != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("Libdigidoc: DataFile_new != ERR_OK - %1").arg(getErrorString(err)));

	// calcula el tamano y el hash del archivo que se agrega al contenedor OpenXAdES
	// por ahora libdigidoc calula el hash del archivo con SHA1 
	err = calculateDataFileSizeAndDigest(pSigDoc, pDataFile->szId, qPrintable(fileName), DIGEST_SHA1);
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addFileToOpenXAdESContainer()", qPrintable(QObject::tr("calculateDataFileSizeAndDigest != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, 
	//	tr("Libdigidoc: calculateDataFileSizeAndDigest != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("Libdigidoc: calculateDataFileSizeAndDigest != ERR_OK - %1").arg(getErrorString(err)));

	// prueba para asignar el valor del puntero pDataFile de SafetDocument luego de insertar el archivo en digidoc
	setDataFile(pDataFile);

        SafetYAWL::streamlog
                <<  SafetLog::Debug
                << tr("Agregado archivo a contenededor correctamente...OK!");
	return err;
}



int SafetDocument::addAttributeToFile(DataFile * pDataFile, QString name, QString value){
	Q_CHECK_PTR(pDataFile);

	//Q_ASSERT_X(!name.isEmpty(), "SafetDocument::addAttributeToFile()", qPrintable(tr("name is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!name.isEmpty(), tr("No se especifico el nombre del atributo que se desea agregar al archivo")) );
	SafetYAWL::evalAndCheckExit(!name.isEmpty(), tr("No se especifico el nombre del atributo que se desea agregar al archivo"));

	//Q_ASSERT_X(!value.isEmpty(), "SafetDocument::addAttributeToFile()", qPrintable(tr("value is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!value.isEmpty(), tr("No se especifico el valor del atributo que se desea agregar al archivo")) );
	SafetYAWL::evalAndCheckExit(!value.isEmpty(), tr("No se especifico el valor del atributo que se desea agregar al archivo"));
	
	int err = ERR_OK;
	
	// ejemplo: 
	// addDataFileAttribute(pDataFile, "ISBN", "000012345235623465");
	err = addDataFileAttribute(pDataFile, qPrintable(name), qPrintable(value));
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addAttributeToFile()", qPrintable(tr("addDataFileAttribute != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("addDataFileAttribute != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("addDataFileAttribute != ERR_OK - %1").arg(getErrorString(err)));

        SafetYAWL::streamlog <<  SafetLog::Debug << tr("Agregado a \"%1\" atributo \"%2\" a archivo correctamente...OK!").arg(name).arg(value);
	return err;
	
}

int SafetDocument::addAttributeToFile(DataFile * pDataFile, QString fileName, QString name, QString value){
	Q_CHECK_PTR(pDataFile);

	//Q_ASSERT_X(!fileName.isEmpty(), "SafetDocument::addAttributeToFile()", qPrintable(tr("fileName is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!fileName.isEmpty(), 
	//	tr("No se especifico el nombre del archivo original al que se desea agregar atributo")) );
	SafetYAWL::evalAndCheckExit(!fileName.isEmpty(), tr("No se especifico el nombre del archivo original al que se desea agregar atributo"));


	//Q_ASSERT_X(!name.isEmpty(), "SafetDocument::addAttributeToFile()", qPrintable(tr("name is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!name.isEmpty(), tr("No se especifico el nombre del atributo que se desea agregar al archivo")) );
	SafetYAWL::evalAndCheckExit(!name.isEmpty(), tr("No se especifico el nombre del atributo que se desea agregar al archivo"));


	//Q_ASSERT_X(!value.isEmpty(), "SafetDocument::addAttributeToFile()", qPrintable(tr("value is Empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!value.isEmpty(), tr("No se especifico el valor del atributo que se desea agregar al archivo")) );
	SafetYAWL::evalAndCheckExit(!value.isEmpty(), tr("No se especifico el valor del atributo que se desea agregar al archivo"));

	
	int err = ERR_OK;
	
	// ejemplo: 
	// addDataFileAttribute(pDataFile, "ISBN", "000012345235623465");
	err = addDataFileAttribute(pDataFile, qPrintable(name), qPrintable(value));
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addAttributeToFile()", qPrintable(tr("addDataFileAttribute != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("addDataFileAttribute != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("addDataFileAttribute != ERR_OK - %1").arg(getErrorString(err)));
	
	// martes 270508 recalcular el hash del archivo porque se agrego un atributo
	// calcula el tamano y el hash del archivo que se agrega al contenedor OpenXAdES 
	// por ahora libdigidoc calula el hash del archivo con SHA1 
	err = calculateDataFileSizeAndDigest(pSigDoc, pDataFile->szId, qPrintable(fileName), DIGEST_SHA1);
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addAttributeToFile()", qPrintable(QObject::tr("calculateDataFileSizeAndDigest != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("calculateDataFileSizeAndDigest != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("calculateDataFileSizeAndDigest != ERR_OK - %1").arg(getErrorString(err)));
	
	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Agregado atributo a archivo correctamente...OK!");
	return err;
}
#endif // DIGIDOC



int SafetDocument::writeOpenXAdESContainerToFile(const QString& inputFileName,
                                                 const QString& outputFileName){
        int err = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SafetYAWL::evalAndCheckExit(!outputFileName.isEmpty(),
		tr("No se especifico el el nombre del archivo en formato OpenXAdES (.ddoc) que almacenara el contenedor."));

        err = ERR_OK;
	if ( inputFileName.length() > 0 ) {
//*verversionDigiDoc
            err = createSignedDoc(signedDoc(), qPrintable(inputFileName), qPrintable(outputFileName));
	}
	else {
            err = createSignedDoc(signedDoc(), NULL, qPrintable(outputFileName));
	}
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("No fue posible inicializar el archivo contenedor de digidoc, error de la libreria digidoc: %1").arg(getErrorString(err)));

	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Escrito contenedor OpenXAdES en el sistema de archivos...Nombre del archivo: %1").arg(outputFileName);
#endif // DIGIDOC
	return err;
}

int SafetDocument::writeDataFileOfContainerToFile(int index, QString digidocPath,
                                                  QString outputPath){

    int err = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    SafetYAWL::evalAndCheckExit(!digidocPath.isEmpty(),
            tr("No se especifico la ruta del contenendor para extraer el archivo."));

    SafetYAWL::evalAndCheckExit(!outputPath.isEmpty(),
            tr("No se especifico el nombre del archivo a extraer del contenedor."));

    err = ERR_OK;
    // Reads in signed XML document and extracts the desired data file
    // pSigDoc - signed document object if exists. Can be NULL
    // szFileName - digidoc filename
    // szDataFileName - name of the file where to store embedded data.
    // szDocId - DataFile Id atribute value
    // szCharset - convert DataFile content to charset
    QString p_szInFile = digidocPath;
    QString szDataFileName = outputPath;
    QString szDocId = "D";
    szDocId.append(QString::number(index));
    QString szCharset = CHARSET_UTF_8;

    err = ddocExtractDataFile(signedDoc(), qPrintable(p_szInFile),
                              qPrintable(szDataFileName), qPrintable(szDocId),
                              qPrintable(szCharset));
    qDebug(getErrorString(err));

    SafetYAWL::evalAndCheckExit(err == ERR_OK,
                                tr("No fue posible extraer el archivo: %1 del contenedor. Error: %2").arg(szDataFileName).arg(getErrorString(err)));

    SafetYAWL::streamlog <<  SafetLog::Debug <<
            tr("Escrito el archivo: %1 en el sistema de archivos").arg(szDataFileName);
#endif // DIGIDOC
    return err;
}

int SafetDocument::writeDataFileOfContainerToFile(int index, QString outputPath){

    int err = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    SafetYAWL::evalAndCheckExit(!outputPath.isEmpty(),
            tr("No se especifico la ruta destino del archivo a extraer."));

    err = ERR_OK;
    // Reads in signed XML document and extracts the desired data file
    // pSigDoc - signed document object if exists. Can be NULL
    // szFileName - digidoc filename
    // szDataFileName - name of the file where to store embedded data.
    // szDocId - DataFile Id atribute value
    // szCharset - convert DataFile content to charset
    QString p_szInFile = getPathOfSafetDocument();
    QString szDataFileName = outputPath;
    QString szDocId = "D";
    szDocId.append(QString::number(index));
    QString szCharset = CHARSET_UTF_8;

    err = ddocExtractDataFile(signedDoc(), qPrintable(p_szInFile),
                              qPrintable(szDataFileName), qPrintable(szDocId),
                              qPrintable(szCharset));
    qDebug(getErrorString(err));

    SafetYAWL::evalAndCheckExit(err == ERR_OK,
                                tr("No fue posible extraer el archivo: %1 del contenedor. Error: %2").arg(szDataFileName).arg(getErrorString(err)));

    SafetYAWL::streamlog <<  SafetLog::Debug <<
            tr("Escrito el archivo: %1 en el sistema de archivos").arg(szDataFileName);
#endif // DIGIDOC
    return err;
}

QString SafetDocument::writeDataFileOfContainerToTmp(int index, QString digidocPath){
    qDebug("... SafetDocument::writeDataFileOfContainerToTmp ... 1");
    QString pathFile;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    QStringList list = SafetYAWL::getTempNameFiles(1);
    QString pre = list.at(0);

    QDir tmpDir = QDir::tempPath();
    bool created = tmpDir.mkdir(pre);
    tmpDir.cd(pre);
    SafetYAWL::evalAndCheckExit(created,
                                tr("No se pudo crear el directorio temporal para operacion de extraccion de archivo de contenedor."));

    QString aux = getDataFileName(index);

    QStringList list1 = aux.split("/");
    aux = list1.last();
    qDebug("valor de aux");
    qDebug(qPrintable(aux));

    pathFile = tmpDir.absolutePath() + "/" + aux; // agregar el nombre del DataFile
    qDebug("valor de pathFile");
    qDebug(qPrintable(pathFile));

    writeDataFileOfContainerToFile(index, digidocPath, pathFile);

    qDebug("... SafetDocument::writeDataFileOfContainerToTmp ... 2");
#endif
    return pathFile;
}

int SafetDocument::readOpenXAdESContainerFile(const QString& fileName){
	

    int err = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    if (fileName.isEmpty() ) {
        return false;
    }


	
        err = ERR_OK;
	err  = ddocSaxReadSignedDocFromFile(&pSigDoc, qPrintable(fileName), 0, 0);
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::readOpenXAdESContainerFile()", 
	//		qPrintable(tr("ddocSaxReadSignedDocFromFile != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("ddocSaxReadSignedDocFromFile != ERR_OK - %1").arg(getErrorString(err)) ));
        if (err != ERR_OK ) {
            SafetYAWL::streamlog
                    << SafetLog::Error
                    <<
            tr("ddocSaxReadSignedDocFromFile != ERR_OK - \"%1\"").arg(getErrorString(err));
        }

	setSignedDoc(pSigDoc);
	//setSigInfo(getSignature (pSigDoc,))
	setDataFile(getDataFile(pSigDoc,0));

#endif // SAFET_DIGIDOC
	return err;
}


void SafetDocument::freeMemoryOfOpenXAdESContainer(){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignedDoc_free(pSigDoc);
#endif // DigiDoc
}

int SafetDocument::numberOfDataFileOnOpenXAdESContainer(){
    int result = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
        result =  getCountOfDataFiles(pSigDoc);
#endif
        return result;
	
}

int SafetDocument::numberOfSignaturesOnOpenXAdESContainer(){
        int result = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
        result =  getCountOfSignatures(pSigDoc);
#endif // DIGIDOC
        return result;
}

QString SafetDocument::getDataFileName(int index){
        QString result;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
        result = dataFile(index)->szFileName;
#endif
        return result;
	
}

long SafetDocument::getDataFileLength(int index){
        long result = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
       result = dataFile(index)->nSize;
#endif
        return result;
}

QString SafetDocument::getDataFileMimeType(int index){
    QString result;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
        result = dataFile(index)->szMimeType;
#endif
        return result;
}

int SafetDocument::numberOfDataFileAttributes(int index){
        int result = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
        result = getCountOfDataFileAttributes(dataFile(index));
#endif
        return result;
}


#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
SignatureInfo * SafetDocument::getOpenXAdESContainerSignature(int id){
	return getSignature(signedDoc(), id);
	
}
#endif

// funciones de prueba para generar XML a partir de un SQL

void SafetDocument::connectToDataBase(QString connectionString){
	/*
	QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
	db.setHostName("localhost");
	db.setDatabaseName("libsafetdb");
	db.setUserName("antonio");
	db.setPassword("antonio");
	bool ok = db.open();
	Q_ASSERT_X(ok, "SafetDocument::connectToDataBase()", "db.open() FALLO!");
	*/
	connectionString ="";
}

QStringList SafetDocument::getDataBaseTableNames(){
	QStringList tablesNames;
	return tablesNames;
}


QList<QString> SafetDocument::getFields(QSqlQuery &query) {	
	QList<QString> mylist;
	QSqlRecord rec;
	rec = query.record();
	
	query.next();	    
   	for (int i= 0; i< rec.count(); i++){
	 	mylist.push_back(rec.fieldName(i));
	}
	return mylist;
}

QList<QVariant::Type> SafetDocument::getInfos(QSqlQuery &query) {	
	QList<QVariant::Type> mylist;
	QSqlRecord rec;
	rec = query.record();
	
	query.next();	    
   	for (int i= 0; i< rec.count(); i++) {
	 	mylist.push_back(rec.field(i).type());
	}
	return mylist;
}

int SafetDocument::countForKey(QSqlQuery &query, const QString& key, SafetWorkflow *wf,
                               bool norender) {
    int result = 0;
  //  qDebug("...key: ... |%s|", qPrintable(key));
    if (key.length() == 0 )  {
            return 0;
     }


    while (query.next()) {
         if ( key.compare( query.value(0).toString().trimmed(), Qt::CaseInsensitive ) == 0 ) {
              result++;
         }

    }
   // qDebug("...countForKey...result: ... |%d|", result);
    return result;
}
void SafetDocument::generateExtraInfo(const QSqlQuery& query, SafetWorkflow *wf) {
    //** tiene extra info
//    if (!SafetYAWL::_hasextrainfo) {
//        return;
//    }
    if ( wf == NULL ) {
        SafetYAWL::streamlog
                << SafetLog::Debug
                << tr("ERROR, el flujo de trabajo actual no está disponible "
                "para generar información extra ");
                return;
    }


    bool ok;
    for(int i = 0; wf->getTasklist().count(); i++) {

        SafetYAWL::ExtraInfoTask myextra;
        SafetTask *mytask = qobject_cast<SafetTask*>(wf->getTasklist().at(i));
        if (mytask == NULL ) {
//            qDebug("....mytask...");
//            qDebug("....generateExtraInfo mytask: NULL");
            return;
        }
        if (mytask->getVariables().count() == 0 ) {
//            qDebug("**TAREA no tiene variables....");
            continue;
        }
        if (SafetYAWL::_extrainfomap.contains(mytask->id()) )  {
//            qDebug("...generateExtraInfo...continue...");
            return;
        }

        SafetVariable* myvar = mytask->getVariables().at(0);

        if (myvar == NULL ) {
            qDebug("...mytask...");
            qDebug("....generateExtraInfo myvar is NULL");
            return;
        }

        QString myrolvalue, myrolfield, mytsfield;
        int mytsvalue;
        if ( !myvar->rolfield().isEmpty()) {
            myrolfield = myvar->rolfield();

            myextra.rolfied = myrolfield;
            if ( query.record().contains(myrolfield)) {
                    myrolvalue = query.record().value(myrolfield).toString();

                    myextra.rolvalue = myrolvalue.trimmed();
            }
        }
        if ( !myvar->timestampfield().isEmpty()) {
            mytsfield = myvar->timestampfield();

            myextra.timestampfield = mytsfield.trimmed();
            if ( query.record().contains(mytsfield.trimmed())) {
                    mytsvalue = query.record().value(mytsfield.trimmed()).toInt(&ok);
                    qDebug("............................................mytsvalue = |%d|",
                           mytsvalue);
                    myextra.timestampvalue = mytsvalue;
            }
        }
        qDebug("....mytask->id():%s", qPrintable(mytask->id()));
        qDebug();
        qDebug("agregando..._extrainfomap...(1).");
        SafetYAWL::_extrainfomap[mytask->id()] = myextra;
        qDebug("agregando..._extrainfomap...(2).");


    }
    qDebug("...saliendo...generateExtraInfo....");

}

QString SafetDocument::getXmlQuery(QSqlQuery &query,int &dcount, const QString& info) {
	QString str;
	QTextStream out(&str);
        out.flush();
	QSqlRecord rec;
	rec = query.record();
	out << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << "\n";
	out << "<libsafetdb>" << "\n";
	
	QString cadena("");
	while (query.next()) {
	    
	    	//qDebug("REGISTRO: %s", qPrintable(x.setNum(j+1)));
	    	//qDebug("<: %s", qPrintable(x.setNum(j+1)), ">");
		//    qDebug("\t<Table>");
//                qDebug("...getXmlQuery....info: |%s| ...query.value: |%s|",qPrintable(info), query.value(0).toString().trimmed());
	   	if ( info.length() > 0 && rec.count() > 0 ) {
			if ( info.compare( query.value(0).toString().trimmed(), Qt::CaseInsensitive ) != 0 ) {
				continue;
			}
		}	
		out << "<record>" << "\n" ;
	   	for (int i= 0; i< rec.count(); i++){
	   		//qDebug(qPrintable(rec.fieldName(i)));
	   		//qDebug(qPrintable(query.value(i).toString()));	
	   		cadena.append("<");
	   		cadena.append(rec.fieldName(i));
	   		cadena.append(">");
	   		cadena.append(query.value(i).toString().trimmed());
	   		cadena.append("</");
	   		cadena.append(rec.fieldName(i));
	   		cadena.append(">");
	   		  	
	   		//qDebug(qPrintable(cadena));
	   		  	
	   		out << cadena << "\n" ;  	
	   		cadena.clear();
	   	}
	    //qDebug("</Table>");
	    
	    out << "</record>" << "\n" ;	
	}
	out << "</libsafetdb>" << "\n";
        out.flush();
        _documents = str;
	return str;
}



QString SafetDocument::getJsonQuery(QSqlQuery &query, QList<QSqlField>& fields, int &dcount, const QString& info) {
	QString str;
	QTextStream out(&str);
	QSqlRecord rec;
	rec = query.record();
	
        for( int i = 0; i < rec.count(); i++) {
           fields.append(rec.field(i) );
         }

	QString cadena("");

    QString mydirmedia = SafetYAWL::getConf()["GeneralOptions/dir.media"];

    dcount = 0;
	while (query.next()) {
	             

        dcount++;
	   	if ( info.length() > 0 && rec.count() > 0 ) {
			if ( info.compare( query.value(0).toString().trimmed(), Qt::CaseInsensitive ) != 0 ) {
				continue;
			}
		}	   		
        out << "{ ";
        for (int i= 0; i< rec.count(); i++) {
            cadena.append(" ");
            cadena.append(" \""+rec.fieldName(i)+"\" ");
            cadena.append(": \"");
            if ( rec.field(i).type() == QVariant::Bool ) {
                if (query.value(i).toString().trimmed() == "true" )
                    cadena.append(tr("Si"));
                else
                    cadena.append(tr("No"));
            }
            else if ((rec.field(i).type() == QVariant::Int
                      || rec.field(i).type() == QVariant::UInt
                      || rec.field(i).type() == QVariant::LongLong
                      || rec.field(i).type() == QVariant::ULongLong)
                     &&
                     (rec.field(i).name().indexOf(tr("fecha")) != -1
                      || rec.field(i).name().indexOf(tr("time")) != -1) ) {

                int mytimet = query.value(i).toInt();
                QString myvalue = QDateTime::fromTime_t(mytimet)
                        .toString(Safet::DateFormat);
                cadena.append(myvalue);

            }
            else {
                QString value = query.value(i).toString().trimmed();
                QString valuefile = mydirmedia+ "/" + value.section("/",-1);

                if (!value.isEmpty() && QFile::exists(valuefile)) {
                    _availablefiles.append(valuefile);
                }

                cadena.append(value);
            }
            cadena.append("\", ");
            if ( i+1 == rec.count() ) cadena.chop(2);
            out << cadena ;
            cadena.clear();
        }

		out << "},\n";
//                if (str.length() > SafetYAWL::MAX_JSON) {
//                    break;
//                }
	    
	}
        str.chop(2);
    _documents = str;
	return str;
}


QString SafetDocument::getJsonArrayQuery(QSqlQuery &query, QList<QSqlField>& fields, int &dcount,
                                         const QString& info) {
    QString str;
    QTextStream out(&str);
    QSqlRecord rec;
    rec = query.record();

        for( int i = 0; i < rec.count(); i++) {
           fields.append(rec.field(i) );
         }

    QString cadena(" ");


    QString mydirmedia = SafetYAWL::getConf()["GeneralOptions/dir.media"];
        SYD << tr(".....:SafetDocument::getJsonArrayQuery.........GETJSON...");
        dcount = 0;

    while (query.next()) {


        dcount++;
        if ( info.length() > 0 && rec.count() > 0 ) {
            if ( info.compare( query.value(0).toString().trimmed(), Qt::CaseInsensitive ) != 0 ) {
                continue;
            }
        }
        out << "[ ";
        for (int i= 0; i< rec.count(); i++) {
            cadena.append(" ");
            cadena.append(" \"");
            if ( rec.field(i).type() == QVariant::Bool ) {
                if (query.value(i).toString().trimmed() == "true" )
                    cadena.append(tr("Si"));
                else
                    cadena.append(tr("No"));
            }
            else if ((rec.field(i).type() == QVariant::Int
                      || rec.field(i).type() == QVariant::UInt
                      || rec.field(i).type() == QVariant::LongLong
                      || rec.field(i).type() == QVariant::ULongLong)
                     &&
                     (rec.field(i).name().indexOf(tr("fecha")) != -1
                      || rec.field(i).name().indexOf(tr("time")) != -1) ) {

                int mytimet = query.value(i).toInt();
                QString myvalue = QDateTime::fromTime_t(mytimet)
                        .toString(Safet::DateFormat);
                cadena.append(myvalue);

            }
            else {
                QString value = query.value(i).toString().trimmed();
                SYD << tr(".....:SafetDocument::getJsonArrayQuery.........GETJSON...value:|%1|")
                       .arg(value);

                QString valuefile = mydirmedia+ "/" + value.section("/",-1);

                SYD << tr(".....:SafetDocument::getJsonArrayQuery.........valuefile:|%1|")
                       .arg(valuefile);
                if (!value.isEmpty() && QFile::exists(valuefile)) {
                    _availablefiles.append(valuefile);
                }

                cadena.append(value);
            }
            cadena.append("\", ");
            if ( i+1 == rec.count() ) cadena.chop(2);
            out << cadena ;
            cadena.clear();
        }

        out << "],\n";

    }
        str.chop(2);
    _documents = str;
    return str;
}


void SafetDocument::createXMLFileFromSQL(QString sqlSentence, QString outputFileName) {
	
	//Q_ASSERT_X(!sqlSentence.isEmpty(), 
	//				"SafetDocument::createXMLFileFromSQL()", 
	//				qPrintable(QObject::tr("sqlSentence vacio")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!sqlSentence.isEmpty(), 
	//	tr("No se especifico la sentencia SQL que se desea realizar y guardar en un contendor OpenXAdES.")) );
	SafetYAWL::evalAndCheckExit(!sqlSentence.isEmpty(),tr("No se especifico la sentencia SQL que se desea realizar y guardar en un contendor OpenXAdES."));
	
	//Q_ASSERT_X(!outputFileName.isEmpty(), 
	//		"SafetDocument::createXMLFileFromSQL()", 
	//		qPrintable(QObject::tr("outputFileName vacio")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!outputFileName.isEmpty(), 
	//	tr("No se especifico la ruta del archivo XML de salida generado a partir de la sentencia SQL.")) );
	SafetYAWL::evalAndCheckExit(!outputFileName.isEmpty(),tr("No se especifico la ruta del archivo XML de salida generado a partir de la sentencia SQL."));


	if (outputFileName.isEmpty()){
//		qDebug("cadena outputFileName vacia");
		return;
    }
	
	QFile file(outputFileName);
	
/*	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
	    	qDebug("el archivo no se puede abrir para escritura");
	    	return;
	    }
*/	
	bool open = file.open(QIODevice::WriteOnly | QIODevice::Text); 
	//Q_ASSERT_X(open, 
	//		"SafetDocument::createXMLFileFromSQL()",
	//		qPrintable(QObject::tr("No se pudo crear el archivo XML correspondiente a la sentencia SQL.")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(open, 
	//	tr("No se pudo crear el archivo XML correspondiente a la sentencia SQL") ));
	SafetYAWL::evalAndCheckExit(open, tr("No se pudo crear el archivo XML correspondiente a la sentencia SQL"));
	
	QTextStream out(&file);
	
//	QSqlQuery query(sqlSentence);  
	QSqlQuery query(sqlSentence, SafetYAWL::currentDb);
//	qDebug("  creado QSqlQuery query");
	
	QSqlRecord rec;
	rec = query.record();
	
	out << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << "\n";
	out << "<libsafetdb>" << "\n";
	
	QString cadena("");
	while (query.next()) {
	    
	    	//qDebug("REGISTRO: %s", qPrintable(x.setNum(j+1)));
	    	//qDebug("<: %s", qPrintable(x.setNum(j+1)), ">");
//		    qDebug("\t<Table>");
	   		
		out << "<Table>" << "\n" ;
	   	for (int i= 0; i< rec.count(); i++){
	   		//qDebug(qPrintable(rec.fieldName(i)));
	   		//qDebug(qPrintable(query.value(i).toString()));	
	   		cadena.append("<");
	   		cadena.append(rec.fieldName(i));
	   		cadena.append(">");
	   		cadena.append(query.value(i).toString());
	   		cadena.append("</");
	   		cadena.append(rec.fieldName(i));
	   		cadena.append(">");
	   		  	
//	   		qDebug(qPrintable(cadena));
	   		  	
	   		out << cadena << "\n" ;  	
	   		cadena.clear();
	   	}
//	    qDebug("</Table>");
	    
	    out << "</Table>" << "\n" ;	
	}
	out << "</libsafetdb>" << "\n";
	
}
               




int SafetDocument::addSignatureToExistingDigidocFile(QString keyFile, QString passwd,
                                                     QString certFile, QString inFile,
                                                     QString outFile){
        int err = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
//	qDebug("entro en la funcion addSignatureToExistingDigidocFile\n");
	//Q_ASSERT_X(!keyFile.isEmpty(), "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(QObject::tr("keyFile vacio")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!keyFile.isEmpty(), 
	//	tr("No se especifico la ruta al archivo que contiene la clave para firmar.")) );
	SafetYAWL::evalAndCheckExit(!keyFile.isEmpty(),tr("No se especifico la ruta al archivo que contiene la clave para firmar."));

	//Q_ASSERT_X(!passwd.isEmpty(), "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(QObject::tr("passwd vacio")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!passwd.isEmpty(), 
	//	tr("No se especifico la contrasena que protege el archivo de la clave para firmar.")) );
	SafetYAWL::evalAndCheckExit(!passwd.isEmpty(),tr("No se especifico la contrasena que protege el archivo de la clave para firmar."));

	//Q_ASSERT_X(!certFile.isEmpty(), "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(QObject::tr("certFile vacio")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!certFile.isEmpty(), 
	//	tr("No se especifico la ruta al archivo que contiene el certificado firmante.")) );
	SafetYAWL::evalAndCheckExit(!certFile.isEmpty(),tr("No se especifico la ruta al archivo que contiene el certificado firmante."));

	//Q_ASSERT_X(!inFile.isEmpty(), "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(QObject::tr("inFile vacio")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!inFile.isEmpty(), 
	//	tr("No se especifico la ruta al archivo que se desea firmar.")) );
	SafetYAWL::evalAndCheckExit(!inFile.isEmpty(),tr("No se especifico la ruta al archivo que se desea firmar."));

	//Q_ASSERT_X(!outFile.isEmpty(), "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(QObject::tr("outFile vacio")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!outFile.isEmpty(), 
	//	tr("No se especifico la ruta del archivo de salida firmado.")) );
	SafetYAWL::evalAndCheckExit(!outFile.isEmpty(),tr("No se especifico la ruta del archivo de salida firmado."));
	
        err = ERR_OK;
	err = ddocSaxReadSignedDocFromFile(&pSigDoc, qPrintable(inFile), 0, 0);
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(tr("ddocSaxReadSignedDocFromFile != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("ddocSaxReadSignedDocFromFile != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("ddocSaxReadSignedDocFromFile != ERR_OK - %1").arg(getErrorString(err)));

//	qDebug("ddocSaxReadSignedDocFromFile(&pSigDoc, qPrintable(inFile), 0, 0)");
	
	// add new signature with default id
	err = SignatureInfo_new(&pSigInfo, pSigDoc, NULL);
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(tr("SignatureInfo_new != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("SignatureInfo_new != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("SignatureInfo_new != ERR_OK - %1").arg(getErrorString(err)));
//	qDebug("SignatureInfo_new(&pSigInfo, pSigDoc, NULL)");
	
	// automatically calculate doc-info elements for this signature
	err = addAllDocInfos(pSigDoc, pSigInfo);
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(tr("addAllDocInfos != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("addAllDocInfos != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("addAllDocInfos != ERR_OK - %1").arg(getErrorString(err)));
//	qDebug("addAllDocInfos(pSigDoc, pSigInfo)");
	
	// add signature production place
	err = setSignatureProductionPlace(pSigInfo, "Merida", "Merida", "5101","Venezuela");
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(tr("setSignatureProductionPlace != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("setSignatureProductionPlace != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("setSignatureProductionPlace != ERR_OK - %1").arg(getErrorString(err)));
//	qDebug("setSignatureProductionPlace(pSigInfo, Merida, Merida, 5101,Venezuela)");
	
	// add user roles
	err = addSignerRole(pSigInfo, 0, "VIP", -1, 0);
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(tr("addSignerRole != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("addSignerRole != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("addSignerRole != ERR_OK - %1").arg(getErrorString(err)));
//	qDebug("addSignerRole(pSigInfo, 0, VIP, -1, 0)");
	
	
//	qDebug("JUSTO ANTES DE calculateSigInfoSignature");
	//rc = calculateSigInfoSignature(pSigDoc,pSigInfo,int nSignType, const char* keyfile, const char* passwd, const char* certfile);
	err = calculateSigInfoSignature(pSigDoc,pSigInfo,SIGNATURE_RSA, qPrintable(keyFile), qPrintable(passwd), qPrintable(certFile));
	
	//rc = calculateSignatureWithEstID(pSigDoc, pSigInfo, 0, "r96F29bE");
//	qDebug(getErrorString(err));

	
	//printf("\nError en calculateSigInfoSignature: %s\n", getErrorString(rc)) ;
	
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(tr("calculateSigInfoSignature != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("calculateSigInfoSignature != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("calculateSigInfoSignature != ERR_OK - %1").arg(getErrorString(err)));
//	qDebug("calculateSigInfoSignature(pSigDoc,pSigInfo,SIGNATURE_RSA, qPrintable(keyFile), qPrintable(passwd), qPrintable(certFile));");
	
	
	
        err = createSignedDoc(pSigDoc, qPrintable(inFile), qPrintable(outFile));
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(tr("createSignedDoc != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("createSignedDoc != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("createSignedDoc != ERR_OK - %1").arg(getErrorString(err)));

//	qDebug("createSignedDoc(pSigDoc, qPrintable(inFile), qPrintable(outFile));");
	
	//return rc;
//	qDebug("termino la funcion addSignatureToExistingDigidocFile");
	
	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Agregada firma a contenedor OpenXAdES correctamente...OK!");
#endif // DIGIDOC
	return err;
} 

bool SafetDocument::checkPrivateKeyPassword(const QString& keyFile,
                                            const QString& passwd) {

    QFile file(keyFile);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
          return false;

      QSslKey mykey(&file,QSsl::Rsa,QSsl::Pem,QSsl::PrivateKey,passwd.toLatin1());
      if ( mykey.isNull()) {
          qDebug("mykey.isNull()");
          return false;
      }
      return true;



}

int SafetDocument::signWithPrivateKeyOnFile(const QString& keyFile, const QString& passwd,
                                            const QString& certFile, const QString& inFile,
                                            const QString& outFile,
                                            const QString& manifest,
                                            const QString& city,
                                            const QString& state,
                                            const QString& zip,
                                            const QString& country,
                                            const QString& notaryUrl,
                                            const QString& proxyHost,
                                            const QString& proxyPort){
	

    int err = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    if (!checkPrivateKeyPassword(keyFile,passwd)) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << trUtf8("La contraseña suministrada para el archivo \"%1\" "
                      "es incorrecta ")
                .arg(keyFile);
        return 0;
    }

	SafetYAWL::evalAndCheckExit(!passwd.isEmpty(),tr("No se especifico la contrasena que protege el archivo de la clave para firmar."));

	SafetYAWL::evalAndCheckExit(!certFile.isEmpty(),tr("No se especifico la ruta al archivo que contiene el certificado firmante."));


        if ( outFile.isEmpty() ) {
            SafetYAWL::streamlog
                    << SafetLog::Error <<
                    tr("No se especificó la ruta del archivo de salida firmado.");
            return 0;
        }


	SafetYAWL::evalAndCheckExit(!notaryUrl.isEmpty(),tr("No se especifico la URL del servidor para notarizar la firma (ej: http://proxyHost:proxyPort)"));

	SafetYAWL::evalAndCheckExit(!proxyHost.isEmpty(),tr("No se especifico el nombre del servidor para notarizar la firma."));

	SafetYAWL::evalAndCheckExit(!proxyPort.isEmpty(),tr("No se especifico el puerto del servidor para notarizar la firma."));

        err = ERR_OK;

        SignedDoc* mydoc = signedDoc();
        if ( !inFile.isEmpty() ) {
            err = ddocSaxReadSignedDocFromFile(&mydoc, qPrintable(inFile), 0, 0);
            qDebug("..........(*4)...inFile: %s",qPrintable(inFile));
        }
	// add new signature with default id
        SignatureInfo **ppSigInfo;
        err = SignatureInfo_new(&pSigInfo, pSigDoc, NULL);
        ppSigInfo = &pSigInfo;

	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::addSignatureToExistingDigidocFile()",qPrintable(tr("SignatureInfo_new != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("SignatureInfo_new != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("SignatureInfo_new != ERR_OK - %1").arg(getErrorString(err)));
//	qDebug("SignatureInfo_new(&pSigInfo, pSigDoc, NULL)");

        qDebug("............*ppSigInfo:%p ... pSigInfo: %p",*ppSigInfo,pSigInfo);

        err = addAllDocInfos(signedDoc(), *ppSigInfo);
	
        err = setSignatureProductionPlace(*ppSigInfo, qPrintable(city), qPrintable(state), qPrintable(zip), qPrintable(country));

	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("setSignatureProductionPlace != ERR_OK - %1").arg(getErrorString(err)));
	
        err = addSignerRole(*ppSigInfo, 0, qPrintable(manifest), -1, 0);

	
        qDebug("...certFile: %s", qPrintable(certFile));
        err = calculateSigInfoSignature(pSigDoc,*ppSigInfo,SIGNATURE_RSA, qPrintable(keyFile),
                                        qPrintable(passwd), qPrintable(certFile));


        qDebug("...calculateSigInfoSignature...err: %d", err);

       if( err != ERR_OK) {
                SafetYAWL::streamlog
                        << SafetLog::Error
                        <<
                    trUtf8("Ocurrió una falla al calcular la firma electrónica \"%1\"")
                    .arg(getErrorString(err));
                return 0;
            }
        qDebug("......confirmando...");
        err = getConfirmation(pSigDoc, *ppSigInfo, NULL, NULL, NULL, NULL, (char*) qPrintable(notaryUrl), (char*) qPrintable(proxyHost), (char*)qPrintable(proxyPort));
        qDebug("......confirmando...err: %d",err);
        if( err != ERR_OK) {
                 SafetYAWL::streamlog
                         << SafetLog::Error
                         <<
                     trUtf8("Ocurrió una falla confirmando con el OCSP \"%1\"")
                     .arg(getErrorString(err));
                 return 0;
             }

        SafetYAWL::streamlog
                << SafetLog::Action
                << trUtf8("Confirmación realizada correctamente...en \"%1\"...ok!")
                .arg(notaryUrl);

        qDebug("......notarizando...");
        err = notarizeSignature(pSigDoc, *ppSigInfo);

        if( err != ERR_OK) {
                 SafetYAWL::streamlog
                         << SafetLog::Error
                         <<
                     trUtf8("Ocurrió una falla notarizando con el OCSP \"%1\"")
                     .arg(getErrorString(err));
                 return 0;
             }

        SafetYAWL::streamlog
                << SafetLog::Action
                << trUtf8("Notarización realizada correctamente...ok!");

        qDebug("......despues de notarizar...");

#endif // DIGIDOC
	return err;
}



void SafetDocument::initPKCS11Lib(QString libName){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	//Q_ASSERT_X(!libName.isEmpty(), "SafetDocument::SafetDocument::initPKCS11Lib()",qPrintable(QObject::tr("libName vacio")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!libName.isEmpty(), tr("No se especifico la ruta a la libreria que implementa PKCS#11.")) );
	SafetYAWL::evalAndCheckExit(!libName.isEmpty(), tr("No se especifico la ruta a la libreria que implementa PKCS#11."));

//	qDebug("\nSafetDocument::initPKCS11Lib ... ");
//	qDebug(qPrintable(libName));
	initPKCS11Library(qPrintable(libName));
//	qDebug("despues de  initPKCS11Library");
	
	SafetYAWL::streamlog <<  SafetLog::Action << tr("Inicializada libreria PKCS11 correctamente...OK!");
#endif // DIGIDOC
}



int SafetDocument::initDigidocConfigStore(QString configFile)
{
    int err  = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	//Q_ASSERT_X(!configFile.isEmpty(), "SafetDocument::SafetDocument::initDigidocConfigStore()",qPrintable(QObject::tr("configFile vacio")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!configFile.isEmpty(), tr("La ruta al archivo de configuracion de la libreria Libdigidoc no se especifico correctamente.")) );
	SafetYAWL::evalAndCheckExit(!configFile.isEmpty(), tr("La ruta al archivo de configuracion de la libreria Libdigidoc no se especifico correctamente."));

        err = ERR_OK;
	err = initConfigStore(qPrintable(configFile));
	//Q_ASSERT_X(err == ERR_OK, "SafetDocument::initDigidocConfigStore()",qPrintable(tr("initConfigStore != ERR_OK")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("Libdigidoc: initConfigStore != ERR_OK - %1").arg(getErrorString(err)) ));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("Libdigidoc: initConfigStore != ERR_OK - %1").arg(getErrorString(err)));

	SafetYAWL::streamlog <<  SafetLog::Action << tr("Carga de configuracion de libreria Libdigidoc correctamente...OK!");
#endif // DIGIDOC
	return err;
}


#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
int SafetDocument::signWithSmartCard(SignedDoc* pSigDoc, QString pin, QString manifest, QString city, QString state, QString zip, QString country)
{

	Q_CHECK_PTR(pSigDoc);

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!pin.isEmpty(), tr("No se especifico el PIN de la tarjeta inteligente.")) );
	SafetYAWL::evalAndCheckExit(!pin.isEmpty(), tr("No se especifico el PIN de la tarjeta inteligente."));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!manifest.isEmpty(), tr("No se especifico el rol del firmante del documento.")) );
	SafetYAWL::evalAndCheckExit(!manifest.isEmpty(), tr("No se especifico el rol del firmante del documento."));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!city.isEmpty(), tr("No se especifico la ciudad en la que se produce la firma del documento.")) );
	SafetYAWL::evalAndCheckExit(!city.isEmpty(), tr("No se especifico la ciudad en la que se produce la firma del documento."));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!state.isEmpty(), tr("No se especifico el estado en el que se produce la firma del documento.")) );
	SafetYAWL::evalAndCheckExit(!state.isEmpty(), tr("No se especifico el estado en el que se produce la firma del documento."));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!zip.isEmpty(), 
	//	tr("No se especifico codigo postal de la ciudad en la que se produce la firma del documento.")) );
	SafetYAWL::evalAndCheckExit(!zip.isEmpty(),tr("No se especifico codigo postal de la ciudad en la que se produce la firma del documento."));

	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!country.isEmpty(), tr("No se especifico el pais en el que se produce la firma del documento.")) );
	SafetYAWL::evalAndCheckExit(!country.isEmpty(), tr("No se especifico el pais en el que se produce la firma del documento."));
	
	


	int err = ERR_OK;
	SignatureInfo* pSigInfo = NULL;
		
        err = signDocument(pSigDoc, &pSigInfo, qPrintable(pin), qPrintable(manifest),
                           qPrintable(city), qPrintable(state), qPrintable(zip),
                           qPrintable(country));
        if ( err != ERR_OK ) {
               return err;
          }

        SafetYAWL::streamlog
                <<  SafetLog::Action
                << tr("Aplicada firma electronica con tarjeta "
                      "inteligente a contendedor OpenXAdES correctamente...OK!");
	return err;
	
	
}
#endif // DIGIDOC


bool SafetDocument::verifySignMadeWithSmartCard(QString fileName)
{

 #ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC

 if ( !QFile::exists(fileName) ) {
    SafetYAWL::streamlog << SafetLog::Error <<  tr("No se especifico la ruta del archivo firmado que se desea verificar.");
}


  int err = ERR_OK, s, d, l, m;
  SignatureInfo* pSigInfo = 0;
  SignedDoc * pSigDoc = 0;
  NotaryInfo* pNot;
  DigiDocMemBuf mbuf;

  int g_cgiMode = 0;   // 1=output in CGI mode, 0=normal e.g. human readable mode
  char* g_szOutputSeparator = 0;


  mbuf.pMem = 0;
  mbuf.nLen = 0;

  pSigDoc = signedDoc(); //nuevo
  Q_CHECK_PTR(pSigDoc);


  //pSigInfo = SigInfo(); // nuevo
  //Q_CHECK_PTR(pSigInfo);

 qDebug(".........SafetDocument::verifySignMadeWithSmartCard ...(1)");
  d = getCountOfSignatures(pSigDoc);
  if ( d <=0) return false;
  qDebug(".........valor de getCountOfSignatures: %d",d);

  for(s = 0; s < d; s++) {
    pSigInfo = getSignature(pSigDoc, s);
    Q_CHECK_PTR(pSigInfo);

    X509* currentSigner = ddocSigInfo_GetSignersCert(pSigInfo);
    if ( currentSigner == NULL ) continue;
    err = ddocCertGetSubjectCN(currentSigner, &mbuf);
    //Q_ASSERT_X(err == ERR_OK, "SafetDocument::verifySignMadeWithSmartCard()",qPrintable(tr("ddocCertGetSubjectCN != ERR_OK")));
    //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err)) ));
    SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err)));
    if (err != ERR_OK)
    {
	SafetYAWL::streamlog <<  SafetLog::Error << tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err));
  	return false;
    }
    int validocsp = ConfigItem_lookup_bool("NOTARIZE_VIA_OCSP", 1);
    qDebug("....SafetDocument::verifySignMadeWithSmartCard...validocsp....: %d", validocsp);
    if ( validocsp /* valid_offline (vbravo) */ ) {
        err = verifySignatureAndNotary(pSigDoc, pSigInfo, qPrintable(fileName)/*p_szInFile*/);
        if (err != ERR_OK) {

            SafetYAWL::streamlog << SafetLog::Error <<
                    tr("Ocurrio el siguiente error verificando o notarizando la firma: \"%1\"").arg(getErrorString(err));
            return false;
        }
    }
    else {
        QStringList certfiles = getCertFileList();
        if ( certfiles.count() == 0 ) {
            return false;
        }

        foreach(QString certfile, certfiles) {
            //qDebug("....*********************SafetDocument::verifySignMadeWithSmartCard...certfile: %s", qPrintable(certfile));
            X509* certSigner = SafetDocument::loadX509FromFile(certfile);
            QString issuerName = ddocSigInfo_GetSignersCert_IssuerName(pSigInfo);
            qDebug("..........verificando el certificado............./usr/local/share/libdigidoc/certs/accenditel.pem:%s",qPrintable(issuerName));
            bool resultverify = SafetDocument::verifySigner(currentSigner,certSigner);
            //qDebug(".................*************......................certSigner: %p...................",certSigner);
            if ( resultverify) {
                qDebug("resultado verificado....OK!");
                SafetYAWL::streamlog << SafetLog::Action <<
                        tr("Se validó la firma del archivo con el certificado \"%1\"").arg(certfile);
                return true;
            }
        }
        const char* p2;
        p2 = ConfigItem_lookup("CA_CERT_PATH");
        Q_CHECK_PTR(p2);
        SafetYAWL::streamlog << SafetLog::Error
                << tr("No se pudo constatar la veracidad de una(s) la firma(s) con el repositorio de certificados local "
                      "ubicado en \"%1\"").arg(p2);

    }
    //qDebug("SE SUPONE QUE PASO verifySignatureAndNotary");

    ddocMemBuf_free(&mbuf);

    return false;
    // print signers roles / manifests
    //m = getCountOfSignerRoles(pSigInfo, 0);


} // fin del for





  // signers certificate
  //qDebug("antes de salir de verify");

  SafetYAWL::streamlog <<  SafetLog::Action << tr("Verificada la firma electronica de contendedor OpenXAdES correctamente...OK!");
#endif // DIGIDOC
  return true;
//}

// --------fin de prueba de verificacion ---------------------

}


bool SafetDocument::verifySignMadeWithSmartCard(QString fileName, int signatureIndex)
{
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC

//  qDebug("SafetDocument::verifySignMadeWithSmartCard ...");
  //Q_ASSERT_X(!fileName.isEmpty(), "SafetDocument::verifySignMadeWithSmartCard()", qPrintable(tr("fileName is Empty")));
  //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!fileName.isEmpty(), tr("No se especifico la ruta del archivo firmado que se desea verificar.")) );
  SafetYAWL::evalAndCheckExit(!fileName.isEmpty(), tr("No se especifico la ruta del archivo firmado que se desea verificar."));

  //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval((signatureIndex>=0), tr("No se especifico el indice de la firma que se desea verificar.")) );
  SafetYAWL::evalAndCheckExit((signatureIndex>=0), tr("No se especifico el indice de la firma que se desea verificar."));

// -------- prueba de verificacion, codigo copiado del cdigidoc

//--------------------------------------------------
// Verifys sigantures and notaries
//--------------------------------------------------
//int cmdVerify(SignedDoc* pSigDoc)
//{
  int err = ERR_OK, d, l, m;
  SignatureInfo* pSigInfo = 0;
  SignedDoc * pSigDoc = 0;
  NotaryInfo* pNot;
  DigiDocMemBuf mbuf;

  int g_cgiMode = 0;   // 1=output in CGI mode, 0=normal e.g. human readable mode
  char* g_szOutputSeparator = 0;

  mbuf.pMem = 0;
  mbuf.nLen = 0;

  pSigDoc = signedDoc(); //nuevo
  Q_CHECK_PTR(pSigDoc);

  //pSigInfo = SigInfo(); // nuevo
  //Q_CHECK_PTR(pSigInfo);


  d = getCountOfSignatures(pSigDoc);
//  qDebug("valor de getCountOfSignatures: %d",d);

  if (signatureIndex > (d-1)){
//	qDebug("No existe firma con indice:  %d en el contenedor digidoc", signatureIndex);
	SafetYAWL::streamlog <<  SafetLog::Warning << tr("No existe firma con indice: %1 en el contenedor digidoc").arg(signatureIndex);
	return false;
  }

  //qDebug("antes del comenzar la validacion de la firma con indice:  %d", signatureIndex);
//  for(s = 0; s < d; s++) {
    pSigInfo = getSignature(pSigDoc, signatureIndex);
    //Q_CHECK_PTR(pSigInfo);
    //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval((pSigInfo != NULL), tr("Error en verificacion de firma - Estructura de de datos de firma invalida!.") ));
    SafetYAWL::evalAndCheckExit((pSigInfo != NULL), tr("Error en verificacion de firma - Estructura de de datos de firma invalida!."));

    err = ddocCertGetSubjectCN(ddocSigInfo_GetSignersCert(pSigInfo), &mbuf);
    //Q_ASSERT_X(err == ERR_OK, "SafetDocument::verifySignMadeWithSmartCard()",qPrintable(tr("ddocCertGetSubjectCN != ERR_OK")));
    //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err)) ));
    SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err)));
    if (err != ERR_OK)
    {
        SafetYAWL::streamlog <<  SafetLog::Warning << tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err));
  	return false;
    }

    err = verifySignatureAndNotary(pSigDoc, pSigInfo, qPrintable(fileName)/*p_szInFile*/);
    //Q_ASSERT_X(err == ERR_OK, "SafetDocument::verifySignMadeWithSmartCard()",qPrintable(tr("verifySignatureAndNotary != ERR_OK")));
    SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("Fallo la verificacion o notarizacion del documento, error de la libreria DIGIDOC: %1").arg(getErrorString(err)));
    if (err != ERR_OK)
    {
	SafetYAWL::streamlog <<  SafetLog::Error << tr("verifySignatureAndNotary != ERR_OK - %1").arg(getErrorString(err));
  	return false;
    }
    SafetYAWL::streamlog <<  SafetLog::Debug << tr("**************** PASO verifySignatureAndNotary on err: %1").arg(getErrorString(err));

    //qDebug("SE SUPONE QUE PASO verifySignatureAndNotary");
    if(g_cgiMode) {
      fprintf(stdout, "\nSignature%s%s%s%s%s%d%s%s",
	      g_szOutputSeparator, 
	      pSigInfo->szId, 
	      g_szOutputSeparator,
	      (const char*)mbuf.pMem, 
	      g_szOutputSeparator,
	      err, 
	      g_szOutputSeparator,
	      getErrorString(err));
    } else {
//-      fprintf(stdout, "\nSignature: %s - %s - %s", pSigInfo->szId, 
//-	      (const char*)mbuf.pMem, ((!err) ? "OK" : "ERROR"));
    }
    ddocMemBuf_free(&mbuf);
    // print signers roles / manifests
    m = getCountOfSignerRoles(pSigInfo, 0);
    for(l = 0; l < m; l++) {
      if(g_cgiMode)
        fprintf(stdout, "\nClaimedRole%s%s", g_szOutputSeparator, getSignerRole(pSigInfo, 0, l));
//-      else
//-        fprintf(stdout, "\n\tClaimedRole: %s", getSignerRole(pSigInfo, 0, l));
    }
    if(pSigInfo->sigProdPlace.szCity || pSigInfo->sigProdPlace.szStateOrProvince ||
       pSigInfo->sigProdPlace.szPostalCode || pSigInfo->sigProdPlace.szCountryName)
      if(g_cgiMode)
        fprintf(stdout, "\nSignatureProductionPlace%s%s%s%s%s%s%s%s", g_szOutputSeparator,
		(pSigInfo->sigProdPlace.szCountryName ? pSigInfo->sigProdPlace.szCountryName : ""), 
		g_szOutputSeparator,
		(pSigInfo->sigProdPlace.szStateOrProvince ? pSigInfo->sigProdPlace.szStateOrProvince : ""), 
		g_szOutputSeparator,
		(pSigInfo->sigProdPlace.szCity ? pSigInfo->sigProdPlace.szCity : ""), 
		g_szOutputSeparator,
		(pSigInfo->sigProdPlace.szPostalCode ? pSigInfo->sigProdPlace.szPostalCode : ""));
//-      else
//-        fprintf(stdout, "\n\tnSignatureProductionPlace - Country: %s, State: %s, City: %s, Postal code: %s",
//-		(pSigInfo->sigProdPlace.szCountryName ? pSigInfo->sigProdPlace.szCountryName : ""),
//-		(pSigInfo->sigProdPlace.szStateOrProvince ? pSigInfo->sigProdPlace.szStateOrProvince : ""),
//-		(pSigInfo->sigProdPlace.szCity ? pSigInfo->sigProdPlace.szCity : ""),
//-		(pSigInfo->sigProdPlace.szPostalCode ? pSigInfo->sigProdPlace.szPostalCode : ""));
// } // fin del for

  // signers certificate
  if(pSigInfo && pSigInfo->pX509Cert)
//-	printCertificateInfo(pSigDoc, (X509*)pSigInfo->pX509Cert);
  // confirmation
  if(pSigDoc && pSigInfo) {
    pNot = getNotaryWithSigId(pSigDoc, pSigInfo->szId);
    if(pNot) {
      if(g_cgiMode)
	fprintf(stdout, "\nRevocationValues%s%s%s%s",  
		g_szOutputSeparator, 
		pNot->szNotType,
		g_szOutputSeparator, 
		pNot->timeProduced);
//-      else
//-	fprintf(stdout, "\n\tRevocationValues responder: %s produced-at: %s",  
//-		pNot->szNotType, pNot->timeProduced);
      // certificate
//-      printCertificateInfo(pSigDoc, (X509*)pNot->pX509Cert);
    }
  }
  //qDebug("antes de salir de verify");

  SafetYAWL::streamlog <<  SafetLog::Action << tr("Verificada firma electronica de contendedor OpenXAdES correctamente...OK!");
#endif // DIGIDOC
  return true;

}


bool SafetDocument::verifySignMadeWithSmartCard(const QString& fileName,
                                                const QStringList& listOfSigners,
                                                bool &isneg,
                                                SignatureOperator op)
{
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
//  qDebug("SafetDocument::verifySignMadeWithSmartCard ...");
  //Q_ASSERT_X(!fileName.isEmpty(), "SafetDocument::verifySignMadeWithSmartCard()", qPrintable(tr("fileName is Empty")));
  //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!fileName.isEmpty(), tr("No se especifico la ruta del archivo firmado que se desea verificar.")) );
    isneg = false;
    if ( fileName.isEmpty() ){
        SafetYAWL::streamlog
                << SafetLog::Error
                <<
                tr("No se especifico la ruta del archivo firmado que se desea verificar.");
        return false;
    }

    if (listOfSigners.isEmpty()) {
        SafetYAWL::streamlog
                << SafetLog::Error
                <<
                tr("No se especifico la lista de firmantes a verificar en el archivo:"
                   " \"%1\"").arg(listOfSigners.join(", "));
        return false;
    }



  int err = ERR_OK, d;
  //int m;
  SignatureInfo* pSigInfo = 0;
  SignedDoc * pSigDoc = 0;
  //NotaryInfo* pNot;
  DigiDocMemBuf mbuf;


  mbuf.pMem = 0;
  mbuf.nLen = 0;

  pSigDoc = signedDoc(); //nuevo
  Q_CHECK_PTR(pSigDoc);

  d = getCountOfSignatures(pSigDoc);

  int numOfSignersToVerify = listOfSigners.size();
  
//	SafetYAWL::streamlog << SafetLog::Action << tr("Numero de firmantes leidos: %1").arg(numOfSignersToVerify);
//	SafetYAWL::streamlog << SafetLog::Action << tr("Numero de firmantes en el documento: %1").arg(d);

  // si el numero de firmantes a verificar es mayor que el numero de firmantes que ya posee el
  // archivo, se retorna -1 
  if ( op == SafetDocument::AND ) {
      if (numOfSignersToVerify  !=  d  ) { // Es Diferente el numero de firmantes
          return false;
      }
  }
  int signatureIndex = -1;
  int validocsp = ConfigItem_lookup_bool("NOTARIZE_VIA_OCSP", 1);
  bool checkoperatoror  = false;
  int i;
  if (op == SafetDocument::ALL) {
      return d > 0;
  }

  for (i=0; i<numOfSignersToVerify; i++ ) {

      isneg = false;
      QString cn = listOfSigners.at(i).toLocal8Bit().constData();
      //SafetYAWL::streamlog <<  SafetLog::Debug << tr("Nombre Común (CN) del Firmante: \"%1\"").arg(cn);
      if (cn.startsWith("!")) {
//          cn = cn.mid(1);
//          qDebug();
//          qDebug();
//          qDebug();
//          qDebug("          ....cn:%s",qPrintable(cn));

          isneg = true;
      }
      signatureIndex = getSignerIndex(cn);
      if (signatureIndex == -1) {
          //SafetYAWL::streamlog <<  SafetLog::Warning << tr("No se encontro una firma con el CN de firmante: \"%1\"").arg(cn);

          if ( op == SafetDocument::AND) {
              qDebug("..operador AND");
              qDebug();
              qDebug();
              qDebug();

              if ( !isneg ) {
                  return false;
              }

          }
          else if ( op == SafetDocument::OR ) {
              qDebug("..operador OR");
              qDebug();
              qDebug();
              qDebug();

              if (isneg ) {
                  checkoperatoror = true;
                  break;
              }

          }
          qDebug("..sin operador");
          qDebug();
          qDebug();
          qDebug();

          continue;
      }

      pSigInfo = getSignature(pSigDoc, signatureIndex);
      Q_CHECK_PTR(pSigInfo);
      if ( pSigInfo == NULL  ) {
          SafetYAWL::streamlog
                  << SafetLog::Error <<
                  tr("Error en verificacion de firma - Estructura de de datos de firma invalida!.");
          return false;
      }


      err = ddocCertGetSubjectCN(ddocSigInfo_GetSignersCert(pSigInfo), &mbuf);
      if (err != ERR_OK) {
          SafetYAWL::streamlog <<  SafetLog::Error
                  << tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err));
          return false;
      }

      if ( validocsp  /* valid_offline (vbravo) */  ) {
          err = verifySignatureAndNotary(pSigDoc, pSigInfo,
                                         qPrintable(fileName)/*p_szInFile*/);
          if (err != ERR_OK) {
              if ( op == SafetDocument::AND) {
                  if (!isneg) {
                      return false;
                  }
              }
              else if ( op == SafetDocument::OR ) {
                  if (isneg ) {
                      checkoperatoror = true;
                      break;
                  }
              }


          }
          else {
              if ( op == SafetDocument::AND) {
                  if (isneg) {
                      ddocMemBuf_free(&mbuf);
                      return false;
                  }
              }
          }

          ddocMemBuf_free(&mbuf);

          if ( op == SafetDocument::OR ) {
              if (!isneg) {
                  checkoperatoror = true;
                  break;
              }
          }
      }
  }

  if (op == SafetDocument::OR ) {
      return checkoperatoror;
  }
#endif // DIGIDOC
  return true;

}






#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
void SafetDocument::printCertificateInfo(SignedDoc* pSigDoc, X509* pCert)
{
  int err, p;
  char buf1[500];
  PolicyIdentifier* pPolicies;
  DigiDocMemBuf mbuf;
  int nPols;

  int g_cgiMode = 0;   // 1=output in CGI mode, 0=normal e.g. human readable mode
  char* g_szOutputSeparator = 0;

  mbuf.pMem = 0;
  mbuf.nLen = 0;
  // serial number
  err = ReadCertSerialNumber(buf1, sizeof(buf1), pCert);
//  if(g_cgiMode)
//    fprintf(stdout, "\nX509Certificate%s%s",  g_szOutputSeparator, buf1);
//  else
//    fprintf(stdout, "\nX509Certificate nr: %s",  buf1);
  // issuer
  err = ddocCertGetIssuerDN(pCert, &mbuf);
//  if(g_cgiMode)
//    fprintf(stdout, "%s%s",  g_szOutputSeparator, (char*)mbuf.pMem);
//  else
//    fprintf(stdout, "\n\tIssuer: %s", (char*)mbuf.pMem);
  ddocMemBuf_free(&mbuf);
  // subject
  err = ddocCertGetSubjectDN(pCert, &mbuf);
//  if(g_cgiMode)
//    fprintf(stdout, "%s%s",  g_szOutputSeparator, (char*)mbuf.pMem);
//  else
//    fprintf(stdout, "\n\tSubject: %s", (char*)mbuf.pMem);
  ddocMemBuf_free(&mbuf);
  // ValidFrom
  memset(buf1, 0, sizeof(buf1));
  err = getCertNotBefore(pSigDoc, pCert, buf1,sizeof(buf1));

  if(g_cgiMode)
    fprintf(stdout, "%s%s",  g_szOutputSeparator, buf1);
  else
    fprintf(stdout, "\n\tNotBefore: %s",  buf1);
  // ValidTo
  memset(buf1, 0, sizeof(buf1));
     err = getCertNotAfter(pSigDoc, pCert, buf1, sizeof(buf1));
  if(g_cgiMode)
    fprintf(stdout, "%s%s",  g_szOutputSeparator, buf1);
  else
    fprintf(stdout, "\n\tNotAfter: %s",  buf1);
  // policy URL
  err = readCertPolicies(pCert, &pPolicies, &nPols);
  if(nPols) {
    for(p = 0; p < nPols; p++) {
      if(g_cgiMode)
	fprintf(stdout, "\nSignaturePolicy%s%s%s%s%s%s",  
		g_szOutputSeparator, pPolicies[p].szOID,
		g_szOutputSeparator, pPolicies[p].szCPS,
		g_szOutputSeparator, pPolicies[p].szUserNotice);
      else
	fprintf(stdout, "\n\tnSignaturePolicy oid: %s cps: %s desc: %s",  pPolicies[p].szOID,
		pPolicies[p].szCPS, pPolicies[p].szUserNotice);
    } // for p < nPols
  } 
  PolicyIdentifiers_free(pPolicies, nPols);
  SafetYAWL::streamlog <<  SafetLog::Debug << tr("Impreso informacion de certificado");
}
#endif // DIGIDOC


void SafetDocument::testSmartCardDriver(int slot)
{
/*
  int err = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
  err = ERR_OK;
  LIBHANDLE pLibrary = 0;

  CK_SLOT_ID slotids[20], slId = 0;

  CK_BYTE signature[200];

  int l1;
//qDebug("SafetDocument::testSmartCardDriver");
  // try active driver driver first
  sprintf((char*)signature, "DIGIDOC_DRIVER_%d_FILE", 
	  ConfigItem_lookup_int("DIGIDOC_DEFAULT_DRIVER", 1));


  for(l1 = 0; l1 < 20; l1++) 
    slotids[l1] = INVALID_SLOTIID; // initialize
  err = loadAndTestDriver(ConfigItem_lookup((const char*)signature),
			  &pLibrary, (CK_SLOT_ID*)slotids, 20, (CK_ULONG)slot);
//qDebug("2 SafetDocument::testSmartCardDriver");
  //ddocDebug(3, "calculateSignatureWithEstID", "Driver handle: %d err = %d slot: %d\n", pLibrary, err, slot);
  //qDebug("Driver handle: %d, error = %d slot: %d\n", pLibrary, err, slot);

  //Q_ASSERT_X(err == ERR_OK, "SafetDocument::testSmartCardDriver()",qPrintable(tr("loadAndTestDriver != ERR_OK")));
  //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("loadAndTestDriver != ERR_OK - %1").arg(getErrorString(err)) ));
  if (err != ERR_OK) {

      SafetYAWL::streamlog <<
              SafetLog::Error
              << tr("Falla cargando el controlador, mensaje: \"%1\"").arg(getErrorString(err));
      return;
  }

  //RETURN_IF_NOT(err == ERR_OK, err);


  // inittialize
  slId = INVALID_SLOTIID; // not found yet
  // find suitable slotid
  for(l1 = 0; l1 < 20; l1++) {
    // ddocDebug(3, "calculateSignatureWithEstID", "Slot idx: %d = %d\n", l1, slotids[l1]);
    //qDebug("slot idx: %d = %d\n", l1, slotids[l1]);
    if(slotids[l1] != INVALID_SLOTIID && l1 == slot) {
      slId = slotids[l1];
      // ddocDebug(3, "calculateSignatureWithEstID", "Select idx: %d slot: %d\n", l1, slId);
      //qDebug("select idx: %d slot: %d\n", (int) l1, (int) slId);
    }
  }
  if (slId == INVALID_SLOTIID)
  {
//	qDebug("NO SE ENCONTRO UN SLOT VALIDO");
	SafetYAWL::streamlog <<  SafetLog::Warning << tr("No se encontro un slot valido para un dispositivo criptografico.");
  }
  else
  {
//	qDebug("SE ENCONTRO UN SLOT VALIDO EN: %d", (int) slId);

  }

  if (pLibrary)
  {
        closePKCS11Library(pLibrary, 0);
	SafetYAWL::streamlog <<  SafetLog::Action << tr("Probada la carga del controlador del dispositivo criptografico");
  }

  pLibrary = NULL;
#endif // DIGIDOC
*/ 
}




QString SafetDocument::getCN(int signatureIndex)
{
    QString cn;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
  // esta funcion deberia retornar la cadena con el CN del certificado firmante

//  qDebug("SafetDocument::getCN ...");

  int err = ERR_OK, d;
  SignatureInfo* pSigInfo = 0;
  SignedDoc * pSigDoc = 0;
  DigiDocMemBuf mbuf;

  mbuf.pMem = 0;
  mbuf.nLen = 0;

  pSigDoc = signedDoc(); //nuevo
  Q_CHECK_PTR(pSigDoc);

  //pSigInfo = SigInfo(); // nuevo
  //Q_CHECK_PTR(pSigInfo);

  d = getCountOfSignatures(pSigDoc);
  //qDebug("valor de getCountOfSignatures: %d",d);

  if (signatureIndex > (d-1)){
//	qDebug("No existe firma con indice:  %d", signatureIndex);
	return "";
  }
  pSigInfo = getSignature(pSigDoc, signatureIndex);
  Q_CHECK_PTR(pSigInfo);

  err = ddocCertGetSubjectCN(ddocSigInfo_GetSignersCert(pSigInfo), &mbuf);
  //Q_ASSERT_X(err == ERR_OK, "SafetDocument::getCN()",qPrintable(tr("ddocCertGetSubjectCN != ERR_OK")));
  //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(err == ERR_OK, tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err)) ));
  SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err)));

  //fprintf(stdout, "\nSignature: %s - %s - %s", pSigInfo->szId, (const char*)mbuf.pMem, ((!err) ? "OK" : "ERROR"));

  cn = (const char*)mbuf.pMem;
  ddocMemBuf_free(&mbuf);

  SafetYAWL::streamlog <<  SafetLog::Debug << tr("Obtenido CN del certificado firmante");
#endif // DIGIDOC
  return cn;

}


int SafetDocument::getSignerIndex(QString cn){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    if (cn.isEmpty() ) {
        SafetYAWL::streamlog
                << SafetLog::Error
                <<
        tr("No se especifico el CN del firmante para determinar el indice de firma.");
    }

  int err = ERR_OK, s, d;
  SignatureInfo* pSigInfo = 0;
  SignedDoc * pSigDoc = 0;
  DigiDocMemBuf mbuf;

  mbuf.pMem = 0;
  mbuf.nLen = 0;

  pSigDoc = signedDoc(); //nuevo
  Q_CHECK_PTR(pSigDoc);

  d = getCountOfSignatures(pSigDoc);
  //qDebug("valor de getCountOfSignatures: %d",d);

  for(s = 0; s < d; s++) {
    pSigInfo = getSignature(pSigDoc, s);
    Q_CHECK_PTR(pSigInfo);
    if (pSigInfo == NULL ) {
        SafetYAWL::streamlog << SafetLog::Warning <<
                tr("Para el índice \"%1\" no se encuentra la firma (No se devolvio una firma válida").arg(cn);
       continue;
    }
    err = ddocCertGetSubjectCN(ddocSigInfo_GetSignersCert(pSigInfo), &mbuf);
//    qDebug("\n\t\tSubject: |%s|", (char*)mbuf.pMem);
//    qDebug("\n\t\tcn: |%s|", qPrintable(cn));
    if (err != ERR_OK) {
        SafetYAWL::streamlog << SafetLog::Warning <<
                tr("Se produjo el siguiente error Digidoc:  \"%1\"").arg(getErrorString(err));
        SafetYAWL::streamlog << SafetLog::Debug <<
                tr("Se produjo el siguiente error Digidoc ejecutando ddocCertGetSubjectCN:  \"%1\"").arg(getErrorString(err));
    }

    //fprintf(stdout, "\nSignature: %s - %s - %s", pSigInfo->szId, (const char*)mbuf.pMem, ((!err) ? "OK" : "ERROR"));

    QString cnFound = (const char*)mbuf.pMem;

    if (cnFound == cn)
    {
  //      qDebug("EL INDICE DE LA FIRMA CON CN %s es: %d", qPrintable(cn), s);
        //SafetYAWL::streamlog <<  SafetLog::Debug << tr("El indice de la firma con CN: \"%1\" es \"%2\"").arg(cn).arg(s);
	return s;
    }
    ddocMemBuf_free(&mbuf);

  } // fin del for

//  qDebug("SafetDocument::getSignerIndex(): no se encontro firma con CN %s", qPrintable(cn));
  // SafetYAWL::streamlog <<  SafetLog::Warning << tr("no se encontro firma con CN %1").arg(cn);
#endif // DIGIDOC
  return -1;

}



QStringList SafetDocument::getCommonNameOfSigners(){
    QStringList signerList;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	int signingNumber = numberOfSignaturesOnOpenXAdESContainer();
	//qDebug("Numero de firmantes: %d", signingNumber);



	for (int i=0; i<signingNumber; i++)
	{
		signerList << getCN(i);
	}
#endif // DIGIDOC
	return signerList;

}


int SafetDocument::getNumberOfPrivateKeys(int slot, QString passwd, QString libName){
//	qDebug("SafetDocument::getNumberOfPrivateKeys ...");
    int err = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
        err = ERR_OK;
	LIBHANDLE pLibrary = 0;
  	CK_RV rv;
  	CK_SLOT_ID slotids[20], slId = 0;
  	CK_SESSION_HANDLE hSession = 0;
  	char keyId[20][20];
  	CK_ULONG keyIdLen[20];
  	CK_BYTE signature[200];
  	int l1;

	// try active driver driver first
  	snprintf((char*)signature, sizeof(signature), "DIGIDOC_DRIVER_%d_FILE", 
		ConfigItem_lookup_int("DIGIDOC_DEFAULT_DRIVER", 1));
  	for(l1 = 0; l1 < 20; l1++) 
    		slotids[l1] = INVALID_SLOTIID; // initialize
//  	err = loadAndTestDriver(ConfigItem_lookup((const char*)signature),
//			  &pLibrary, (CK_SLOT_ID*)slotids, 20, (CK_ULONG)slot);
  	ddocDebug(3, "calculateSignatureWithEstID", "Driver handle: %d err = %d slot: %d", 
		    pLibrary, err, slot);
  	RETURN_IF_NOT(err == ERR_OK, err);
  	// inittialize
  	slId = INVALID_SLOTIID; // not found yet


  	for(l1 = 0; l1 < 20; l1++) {
    		if(slotids[l1] != INVALID_SLOTIID)
      		ddocDebug(3, "calculateSignatureWithEstID", 
			"Slot idx: %d = %d", l1, slotids[l1]);
    		if(slotids[l1] != INVALID_SLOTIID && l1 == slot) {
      			slId = slotids[l1];
      			ddocDebug(3, "calculateSignatureWithEstID", 
				"Select idx: %d slot: %d", l1, slId);
    		}
  	}

	initPKCS11LibrarySafet(qPrintable(libName));

	// open session
	if(slId != INVALID_SLOTIID) {
  		rv = (*ckFunc->C_OpenSession)(slId, CKF_SERIAL_SESSION,0,0,&hSession);
  		if(rv == CKR_OK && qPrintable(passwd)) { // don't login if pin is null. Session can be used also to read certs.
    			/* Kommentaar:  	Ainult CKU_USER toetatud. */
    			rv = (*ckFunc->C_Login)(hSession, CKU_USER, (unsigned char*)qPrintable(passwd), strlen(qPrintable(passwd)));
    			if(rv != CKR_OK)
      				hSession = CK_INVALID_HANDLE; // mark session bad!
  		}
		else
		{
			//qDebug("no se pudo abrir una sesion");
			return -1;
		}
		
		//hSession = OpenSession(slId, passwd);
		ddocDebug(3, "calculateSignatureWithEstID","Open sess for slot: %d sess = %uld\n", slId, hSession);
		if (hSession == CK_INVALID_HANDLE) { err = ERR_PKCS_LOGIN; SET_LAST_ERROR(err); return err; }
		ddocDebug(3, "calculateSignatureWithEstID", "OpenSession ok, hSession = %d\n", (int)hSession);

		// get private key
		for(l1 = 0; l1 < 20; l1++) {
			memset(keyId[l1], 0, 20);
      			keyIdLen[l1] = 0;
    		}

		//err = LocatePrivateKey(hSession, keyId, keyIdLen, hKeys);

		CK_OBJECT_HANDLE Objects[10];
    		//CK_RV rv;
    		CK_ULONG ulObjectCount = sizeof(Objects)/sizeof(CK_OBJECT_HANDLE);
    		// CK_BBOOL IsTrue = TRUE;

		// Set up a template to search for all Private Key tokens 
		// Given the session context, that is associated with
		// one slot we will find only one object
		CK_OBJECT_CLASS ObjClass = CKO_PRIVATE_KEY;
		
		CK_ATTRIBUTE Template1[] = {
	      		{ CKA_CLASS,            &ObjClass,  sizeof(ObjClass)    }
    		};
	    	CK_ULONG ulCount = sizeof(Template1) / sizeof(CK_ATTRIBUTE);

    		/*** Initiate the object search
		C_FindObjectsInit initializes a search for token and session objects that match a template.
		hSession is the sessions handle; pTemplate points to a search template that specifies the
    		attribute values to match; ulCount is the number of attributes in the search template. The
    		matching criterion is an exact byte-for-byte match with all attributes in the template. 
    		To find all objects, set ulCount to 0.
    		*/
    		ddocDebug(3, "LocatePrivateKey", "LocatePrivateKey");
    		rv = (*ckFunc->C_FindObjectsInit)(hSession,Template1,ulCount);
    		if(rv==CKR_OK) {
      			// Get list of object handles
      			rv = (*ckFunc->C_FindObjects)(hSession,Objects,ulObjectCount, &ulObjectCount);
      			ddocDebug(3, "LocatePrivateKey", "Find: %d count: %ld", rv, ulObjectCount);
      			if(rv!=CKR_OK) {
//				qDebug("No se encontraron objetos claves privadas");
				return -1;
      			}
    		}
    		// Remember to call C_FindObjectsFinal to terminate the search
    		rv = (*ckFunc->C_FindObjectsFinal)(hSession);
		
		return ulObjectCount; // ojo aqui debe ir el valor del numero de claves privadas de la tarjeta
	}
	else // slot invalido
	{
//		qDebug("slot invalido");
		return -1;
	}
#endif // DIGIDOC
            return err;

}


#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
LIBHANDLE SafetDocument::initPKCS11LibrarySafet(const char* libName) 
{	
  LIBHANDLE pLibrary = 0;
  CK_C_GetFunctionList pC_GetFunctionList;
  CK_RV rv;

  // load PKCS#11 driver
  ddocDebug(3, "initPKCS11Library", "Loading driver: %s\n", libName);
#ifdef WIN32
  pLibrary = LoadLibrary((LPCSTR)libName);
#else
  pLibrary = dlopen(libName, RTLD_NOW);
#endif
  if(pLibrary != NULL) {
    // printf("Resolve PKCS#11 function index!\n");
    // Get function pointer to C_GetFunctionList
#ifdef WIN32
    pC_GetFunctionList = (CK_C_GetFunctionList)GetProcAddress(/*(HINSTANCE__*)*/pLibrary, "C_GetFunctionList");
#else
    pC_GetFunctionList = (CK_C_GetFunctionList)dlsym(pLibrary, "C_GetFunctionList");
#endif
    if(pC_GetFunctionList != NULL) {
      ddocDebug(3, "initPKCS11Library", "Getting PKCS#11 func!\n");
      // Get function pointers to all PKCS #11 functions
      rv = (*pC_GetFunctionList)(&ckFunc);
      if(rv == CKR_OK) {
	ddocDebug(3, "initPKCS11Library", "Initializing PKCS#11 library:");
	// Initalize Cryptoki
	rv = (*ckFunc->C_Initialize)(0);
	ddocDebug(3, "initPKCS11Library", "Initlialized: %d", (int)rv);
	if(rv != CKR_OK) {
	  ddocDebug(2, "initPKCS11Library", "Error initializing library!\n");
	  pLibrary = NULL;  // error initializing the library 
	}
      } else {
	ddocDebug(2, "initPKCS11Library", "Error getting PKCS#11 func!");
	pLibrary = NULL;  // error getting function pointers
      }
    } else {
      ddocDebug(2, "initPKCS11Library", "Error resolving PKCS#11 function index!");
      pLibrary = NULL;  // error getting function list function
    }
  } else {
#ifdef WIN32
    ddocDebug(2, "initPKCS11Library", "Error loading driver : %s", libName);
#else
    ddocDebug(2, "initPKCS11Library", "Error loading driver : %s", dlerror());
#endif
  }
  return pLibrary;
}
#endif

QString SafetDocument::writeFileToDisk(QString string, QString name)
{
	//Q_ASSERT_X(!string.isEmpty(), "SafetDocument::writeFileToDisk()",qPrintable(QObject::tr("string empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!string.isEmpty(), tr("No se especifico la cadena que se desea escribir en el archivo.")) );
	SafetYAWL::evalAndCheckExit(!string.isEmpty(), tr("No se especifico la cadena que se desea escribir en el archivo."));

	//Q_ASSERT_X(!name.isEmpty(), "SafetDocument::writeFileToDisk()",qPrintable(QObject::tr("name empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!name.isEmpty(), tr("No se especifico la ruta del archivo que se desea escribir")) );
	SafetYAWL::evalAndCheckExit(!name.isEmpty(), tr("No se especifico la ruta del archivo que se desea escribir"));

	QString pathFileName = "/tmp/";
	pathFileName.append(name);

	QFile file(pathFileName);
	
	bool open = file.open(QIODevice::WriteOnly | QIODevice::Text);
	//Q_ASSERT_X(open,
	//		"SafetDocument::writeFileToDisk()",
	//		qPrintable(QObject::tr("No se pudo crear el archivo a firmar en el directorio temporal.")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(open,tr("No se pudo crear el archivo a firmar en el directorio temporal") ));
	SafetYAWL::evalAndCheckExit(open,tr("No se pudo crear el archivo %1 a firmar en el directorio temporal").arg(pathFileName));

	QStringList list = string.split("***");

	QTextStream out(&file);
	for (int i = 0; i < list.size()-1; ++i) // <- se coloco -1 
	{
		out << list.at(i);  //.toLocal8Bit().constData();
		out << endl;
	}
	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Escrito archivo en el %1").arg(name);
	return pathFileName;
}


QString SafetDocument::returnFileToString(const QString& pathFileName)
{
	//Q_ASSERT_X(!pathFileName.isEmpty(), "SafetDocument::returnFileToString()",qPrintable(QObject::tr("pathFileName empty")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!pathFileName.isEmpty(), 
	//	tr("No se especifico la ruta del archivo que se desea retornar en la cadena.")) );
	SafetYAWL::evalAndCheckExit(!pathFileName.isEmpty(),tr("No se especifico la ruta del archivo que se desea retornar en la cadena."));

	QFile file(pathFileName);
	
        bool open = file.open(QIODevice::ReadOnly | QIODevice::Text);
	//Q_ASSERT_X(open,
	//		"SafetDocument::returnFileToString()",
	//		qPrintable(QObject::tr("No se pudo crear el archivo a leer del directorio temporal.")));
	//(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(open,tr("No se pudo crear el archivo a leer del directorio temporal") ));
        SafetYAWL::evalAndCheckExit(open,tr("No se pudo abrir el archivo: \"%1\"").arg(pathFileName));


	QString stringToReturn("");
	QTextStream in(&file);
     	while (!in.atEnd()) {
        	stringToReturn.append(in.readLine());
		stringToReturn.append("***");
        }

	return stringToReturn;

}

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
int SafetDocument::encryptString(DEncEncryptedData** ppEncData, QString stringToEncrypt, QString recipient, 
	QString keyName, QString certFile, QString outputFile, QString mimeType)
{
	Q_CHECK_PTR(pSigDoc);
	Q_CHECK_PTR(ppEncData);

	SafetYAWL::evalAndCheckExit(!stringToEncrypt.isEmpty(), tr("No se especifico la cadena que se desea cifrar."));
	SafetYAWL::evalAndCheckExit(!recipient.isEmpty(), tr("No se especifico el nombre comun del receptor del documento a cifrar."));
	SafetYAWL::evalAndCheckExit(!keyName.isEmpty(), tr("No se especifico el nombre de la clave simetrica para cifrar el documento."));
	SafetYAWL::evalAndCheckExit(!certFile.isEmpty(), tr("No se especifico la ruta absoluta al archivo que contiene el certificado del receptor."));
	SafetYAWL::evalAndCheckExit(!outputFile.isEmpty(), tr("No se especifico la ruta absoluta al archivo que contiene el archivo cifrado."));
	SafetYAWL::evalAndCheckExit(!mimeType.isEmpty(), tr("No se especifico el tipo MIME del archivo a cifrar."));
	
	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Proceso de cifrado de cadena iniciado ...!");
	qDebug("Proceso de cifrado de cadena iniciado ...!");

	int err = ERR_OK, i;
	DEncEncryptedKey* pEncKey = 0;
  	X509* pCert = 0;
	char* carriedkeyname = NULL;
	char* id = NULL;
	//char* mime = NULL;

	if(!(*ppEncData))
	{
		err = dencEncryptedData_new(ppEncData, DENC_XMLNS_XMLENC, DENC_ENC_METHOD_AES128, 0, 0, 0);
		SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencEncryptedData_new != ERR_OK - %1").arg(getErrorString(err)));
	}
	
	err = dencMetaInfo_SetLibVersion(*ppEncData);
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencMetaInfo_SetLibVersion != ERR_OK - %1").arg(getErrorString(err)));
	
	err = dencMetaInfo_SetFormatVersion(*ppEncData);
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencMetaInfo_SetFormatVersion != ERR_OK - %1").arg(getErrorString(err)));
	
        SafetYAWL::streamlog
                << SafetLog::Debug
                << tr("Leyendo certificado: %1").arg(certFile);
        err = ReadCertificate(&pCert, qPrintable(certFile));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("ReadCertificate != ERR_OK - %1").arg(getErrorString(err)));
	
	err = dencEncryptedKey_new(*ppEncData, &pEncKey, pCert,DENC_ENC_METHOD_RSA1_5,
		     id, qPrintable(recipient), qPrintable(keyName), carriedkeyname);
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencEncryptedKey_new != ERR_OK - %1").arg(getErrorString(err)));
	
	QStringList list = getTempNameFiles(1);
	
	QFile inFile(list.at(0));
	bool open = inFile.open(QIODevice::WriteOnly | QIODevice::Text);
	SafetYAWL::evalAndCheckExit(open,tr("No se pudo crear el archivo temporal para operacion de cifrado."));
		
	QTextStream out(&inFile);
	out << stringToEncrypt;
	out << endl;
	SafetYAWL::streamlog <<  SafetLog::Debug << tr("escrito el archivo contenido del archivo temporal a cifrar.");

	// encrypt the file
  	err = dencEncryptFile(*ppEncData, qPrintable(list.at(0)), qPrintable(outputFile), qPrintable(mimeType));
  	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencEncryptFile != ERR_OK - %1").arg(getErrorString(err)));
  	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Archivo cifrado escrito en: %1 ").arg(outputFile);
	SafetYAWL::evalAndCheckExit(inFile.remove(), tr("El archivo temporal de cifrado no se pudo borrar."));	
	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Proceso de cifrado de documento finalizado.");

  	return err;

}


int SafetDocument::encryptDocument(DEncEncryptedData** ppEncData, QString documentToEncrypt, QString recipient, 
	QString keyName, QString certFile, QString outputFile, QString mimeType)
{
	Q_CHECK_PTR(pSigDoc);
	Q_CHECK_PTR(ppEncData);

	SafetYAWL::evalAndCheckExit(!documentToEncrypt.isEmpty(), tr("No se especifico la cadena que se desea cifrar."));
	SafetYAWL::evalAndCheckExit(!recipient.isEmpty(), tr("No se especifico el nombre comun del receptor del documento a cifrar."));
	SafetYAWL::evalAndCheckExit(!keyName.isEmpty(), tr("No se especifico el nombre de la clave simetrica para cifrar el documento."));
	SafetYAWL::evalAndCheckExit(!certFile.isEmpty(), tr("No se especifico la ruta absoluta al archivo que contiene el certificado del receptor."));
	SafetYAWL::evalAndCheckExit(!outputFile.isEmpty(), tr("No se especifico la ruta absoluta al archivo que contiene el archivo cifrado."));
	SafetYAWL::evalAndCheckExit(!mimeType.isEmpty(), tr("No se especifico el tipo MIME del archivo a cifrar."));

	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Proceso de cifrado de documento iniciado ...!");

	int err = ERR_OK, i;
	DEncEncryptedKey* pEncKey = 0;
  	X509* pCert = 0;
	char* carriedkeyname = NULL;
	char* id = NULL;
	//char* mime = NULL;

	if(!(*ppEncData))
	{
		err = dencEncryptedData_new(ppEncData, DENC_XMLNS_XMLENC, DENC_ENC_METHOD_AES128, 0, 0, 0);
		SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencEncryptedData_new != ERR_OK - %1").arg(getErrorString(err)));
	}
	
	err = dencMetaInfo_SetLibVersion(*ppEncData);
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencMetaInfo_SetLibVersion != ERR_OK - %1").arg(getErrorString(err)));
	
	err = dencMetaInfo_SetFormatVersion(*ppEncData);
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencMetaInfo_SetFormatVersion != ERR_OK - %1").arg(getErrorString(err)));
	
	err = ReadCertificate(&pCert, qPrintable(certFile));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("ReadCertificate != ERR_OK - %1").arg(getErrorString(err)));
	
	err = dencEncryptedKey_new(*ppEncData, &pEncKey, pCert,DENC_ENC_METHOD_RSA1_5,
		     id, qPrintable(recipient), qPrintable(keyName), carriedkeyname);
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencEncryptedKey_new != ERR_OK - %1").arg(getErrorString(err)));
	
	// encrypt the file
  	err = dencEncryptFile(*ppEncData, qPrintable(documentToEncrypt), qPrintable(outputFile), qPrintable(mimeType));
  	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencEncryptFile != ERR_OK - %1").arg(getErrorString(err)));
  	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Archivo cifrado escrito en: %1 ").arg(outputFile);
	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Proceso de cifrado de documento finalizado.");
	
  	return err;
}
#endif // DIGIDOC

int SafetDocument::decryptDocument(QString inFile, QString outputFile, QString pin)
{
    int err = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SafetYAWL::evalAndCheckExit(!inFile.isEmpty(), tr("No se especifico la ruta absoluta del documento que se desea descifrar."));
	SafetYAWL::evalAndCheckExit(!outputFile.isEmpty(), tr("No se especifico la ruta absoluta del documento descifrado."));
	SafetYAWL::evalAndCheckExit(!pin.isEmpty(), tr("No se especifico el pin de la tarjeta inteligente para descifrar el documento."));

	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Proceso de descifrado de documento iniciado ...!");
	
        err = ERR_OK;
        int i;
	
	//err = dencSaxReadDecryptFile(qPrintable(inFile), qPrintable(outputFile), qPrintable(pin));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencSaxReadDecryptFile != ERR_OK - %1").arg(getErrorString(err)));
	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Archivo descifrado escrito en: %1:").arg(outputFile);
	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Proceso de descifrado de documento finalizado.");
#endif // DIGIDOC

	return err;

}


QString SafetDocument::decryptString(QString inFile, QString pin)
{
        QString line("");
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SafetYAWL::evalAndCheckExit(!inFile.isEmpty(), tr("No se especifico la ruta absoluta del documento que se desea descifrar."));
	SafetYAWL::evalAndCheckExit(!pin.isEmpty(), tr("No se especifico el pin de la tarjeta inteligente para descifrar el documento."));

	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Proceso de descifrado de cadena iniciado ...!");

	int err = ERR_OK, i;
	
	QStringList list = getTempNameFiles(1);
	QString outputFile = list.at(0);

	//err = dencSaxReadDecryptFile(qPrintable(inFile), qPrintable(outputFile), qPrintable(pin));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("dencSaxReadDecryptFile != ERR_OK - %1").arg(getErrorString(err)));

	QFile file(outputFile);
	bool open = file.open(QIODevice::ReadOnly | QIODevice::Text);
	SafetYAWL::evalAndCheckExit(open,tr("No se pudo abrir el archivo descifrado para extrarer la cadena."));


     	QTextStream in(&file);
     	//while (!in.atEnd()) {
        	line = in.readLine(); // <---- aqui hay q ver si se esta leyendo correctamente
		//qDebug("cadena leida:");
		//qDebug(qPrintable(line));
     	//}
	SafetYAWL::evalAndCheckExit(file.remove(), tr("El archivo descifrado no se pudo borrar."));
	//qDebug(qPrintable(line));

	SafetYAWL::streamlog <<  SafetLog::Debug << tr("Proceso de descifrado de cadena finalizado.");
#endif  // DIGIDOC

	return line;
}





QStringList  SafetDocument::getTempNameFiles(int n) {
    QStringList list;
    if (n == 0 ) return list;
    Q_ASSERT(n > 0 );
    
    QList<QTemporaryFile*> filelist;
    
    for (int i= 0; i < n; i++) {
    	filelist.push_back(new QTemporaryFile());
    	Q_CHECK_PTR( filelist.at(i) );
    	bool result = filelist.at(i)->open();
    	Q_ASSERT_X(result, "getTempNameFile", qPrintable(tr("No fue posible abrir el archivo temporal")));
    	list.push_back(filelist.at(i)->fileName());
    }

    Q_ASSERT(n == filelist.count()); // Deben ser igual el numero de la lista de archivos
    for (int i= 0; i < n; i++) {
    	filelist.at(i)->close();
    	delete filelist.at(i); 
    }
    return list;
}


QString SafetDocument::getCommonNameFromCertificate(QString certPath){
        QString cn;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SafetYAWL::evalAndCheckExit(!certPath.isEmpty(), tr("No se especifico la ruta absoluta del certificado digital para extraer el CN."));

	int err = ERR_OK;
	X509* pCert = NULL;
	DigiDocMemBuf mbuf;

	mbuf.pMem = 0;
  	mbuf.nLen = 0;

	err = ReadCertificate(&pCert, qPrintable(certPath));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("ReadCertificate != ERR_OK - %1").arg(getErrorString(err)));

	err = ddocCertGetSubjectCN(pCert, &mbuf);
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("ddocCertGetSubjectCN != ERR_OK - %1").arg(getErrorString(err)));

	cn = (const char*)mbuf.pMem;

	ddocMemBuf_free(&mbuf);
	if(pCert) {
		X509_free(pCert);
	  	pCert = 0;
	}
#endif // DIGIDOC
	return cn;
}


QString SafetDocument::searchCommonNameInCertificates(QString commonName,
                                                      QString x509repository)
{
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SafetYAWL::evalAndCheckExit(!commonName.isEmpty(), tr("No se especifico el nombre comun del certificado digital a buscar en el repositorio."));
	SafetYAWL::evalAndCheckExit(!x509repository.isEmpty(), tr("No se especifico la ruta absoluta del repositorio de certificados x509."));

	QDir dir(x509repository);
	SafetYAWL::evalAndCheckExit(dir.exists(), tr("El repositorio de certificados x509 no existe."));
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);

	QString certNameToReturn, tmpCert;
	x509repository.append("/");

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
        	QFileInfo fileInfo = list.at(i);
 //        	qDebug("dentro del for");
//		qDebug(qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName())));
		
		tmpCert = x509repository;
		tmpCert.append(fileInfo.fileName());
		if ( commonName.compare(getCommonNameFromCertificate(tmpCert),Qt::CaseInsensitive) == 0 )
		{
//			qDebug("el certificado buscado se encuentra en:");
//			qDebug(qPrintable(tmpCert));
//			qDebug("*************");
			return tmpCert;
		}
		tmpCert.clear();
     	}
#endif // DIGIDOC
	return "";
}


QStringList SafetDocument::getAttribute(int index){
        QStringList list;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	int i = numberOfDataFileAttributes();
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < i)) , tr("El indice del atributo buscado no existe."));

	DataFile * pDataFile = dataFile();
	char * a;
	char * b;
	getDataFileAttribute(pDataFile, index, &a, &b);

	list << (QString) a << (QString) b;
#endif // DIGIDOC
	return list;
}

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
X509 * SafetDocument::getSignerCertData(int index){

    SignatureInfo* pSigInfo = 0;
    SignedDoc * pSigDoc = 0;
    pSigDoc = signedDoc();
    Q_CHECK_PTR(pSigDoc);
    int numberOfSignatures = getCountOfSignatures(pSigDoc);
    SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) ,
            tr("No hay informacion del certificado firmante; el archivo no posee firmas."));
    pSigInfo = getSignature(pSigDoc, index);
    Q_CHECK_PTR(pSigInfo);
    return (X509 *) getSignCertData(pSigInfo);
}


SslCertificate SafetDocument::getSslSignerCertData(int index){

    SignatureInfo* pSigInfo = 0;
    SignedDoc * pSigDoc = 0;
    pSigDoc = signedDoc();
    Q_CHECK_PTR(pSigDoc);
    int numberOfSignatures = getCountOfSignatures(pSigDoc);
    SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) ,
            tr("No hay informacion del certificado firmante; el archivo no posee firmas."));
    pSigInfo = getSignature(pSigDoc, index);
    Q_CHECK_PTR(pSigInfo);
    Q_ASSERT_X(pSigInfo != NULL, "SafetDocument::getSslSignerCertData", "pSigInfo es NULO");
    //return (X509 *) getSignCertData(pSigInfo);

    QSslCertificate c;
    qDebug("...SafetDocument::getSslSignerCertData(int index)");
    X509 *x509 = new X509();
    x509 = (X509*) getSignCertData(pSigInfo); // getSignerCertData(index);
    qDebug("   ... paso getSignerCertData(index)");
    Q_CHECK_PTR(x509);
    Q_ASSERT_X(x509 != NULL, "SafetDocument::getSslSignerCertData", "x509 es NULO");
    if (x509 == NULL)
        qDebug("x509 NULL");
    else
        qDebug("x509 NO NULL");
qDebug("antes de qDebug()");
    qDebug((const char *) x509->sig_alg->algorithm->data);
    qDebug((const char*) x509->cert_info->issuer->bytes->data);
qDebug("desues de qDebug()");
    //c = SslCertificate::fromX509( (Qt::HANDLE)x509 );

    unsigned char *cert = NULL;
qDebug("antes de i2d_X509( (X509*)x509, &cert )");
    int len = i2d_X509( (X509*)x509, &cert );
qDebug("despues de i2d_X509( (X509*)x509, &cert )");
    QByteArray der;
    if( len >= 0 )
            der = QByteArray( (char*)cert, len );
    OPENSSL_free( cert );
    //return QSslCertificate( der, QSsl::Der );

    qDebug("   ... paso SslCertificate::fromX509( (Qt::HANDLE)x509 )");
    X509_free( x509 );
/*
    X509 *x509nuevo = new X509();
    Q_ASSERT_X(pSigInfo != NULL, "SafetDocument::getSslSignerCertData", "pSigInfo2 es NULO");
    x509nuevo = (X509*) getSignCertData(pSigInfo); // getSignerCertData(index);
    qDebug("   ... paso getSignerCertData(index)");
    Q_CHECK_PTR(x509nuevo);
    Q_ASSERT_X(x509nuevo != NULL, "SafetDocument::getSslSignerCertData", "x509 es NULO");
    if (x509nuevo == NULL)
        qDebug("x509nuevo NULL");
    else
        qDebug("x509nuevo NO NULL");
qDebug("antes de qDebug()");
    qDebug((const char *) x509nuevo->sig_alg->algorithm->data);
    qDebug((const char*) x509nuevo->cert_info->issuer->bytes->data);
qDebug("desues de qDebug()");
    //c = SslCertificate::fromX509( (Qt::HANDLE)x509 );
    X509_free( x509nuevo );
*/
    return QSslCertificate( der, QSsl::Der );//c;
}
#endif  // DIGIDOC

QString SafetDocument::getCountryOfSignature(int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del pais donde se produce la firma; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , tr("El indice de la firma buscada para extraer el pais no existe."));

	pSigInfo = getSignature(pSigDoc, index);
    	Q_CHECK_PTR(pSigInfo);
	if (!pSigInfo->sigProdPlace.szCountryName)
	{
		SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido el pais donde se produjo la firma del archivo.");
		return "";
	}
	else
	{
		QString country = pSigInfo->sigProdPlace.szCountryName;
		return country;
	}
#endif // DIGIDOC
        return QString("");
}


QString SafetDocument::getStateOfSignature(int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del estado donde se produce la firma; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , tr("El indice de la firma buscada para extraer el estado no existe."));

	pSigInfo = getSignature(pSigDoc, index);
    	Q_CHECK_PTR(pSigInfo);
	if (!pSigInfo->sigProdPlace.szStateOrProvince)
	{
		SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido el estado donde se produjo la firma del archivo.");
		return "";
	}
	else
	{
		QString state = pSigInfo->sigProdPlace.szStateOrProvince;
		return state;
	}
#endif // DIGIDOC
       return QString("");
}


QString SafetDocument::getCityOfSignature(int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion de la ciudad donde se produce la firma; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , tr("El indice de la firma buscada para extraer la ciudad no existe."));

	pSigInfo = getSignature(pSigDoc, index);
    	Q_CHECK_PTR(pSigInfo);
	if (!pSigInfo->sigProdPlace.szCity)
	{
		SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definida la ciudad donde se produjo la firma del archivo.");
		return "";
	}
	else
	{
		QString state = pSigInfo->sigProdPlace.szCity;
		return state;
	}
#endif // DIGIDOC
        return QString("");
}


QString SafetDocument::getPostalCodeOfSignature(int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del codigo postal de la ciudad donde se produce la firma; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer el codigo postal de la ciudad no existe."));

	pSigInfo = getSignature(pSigDoc, index);
    	Q_CHECK_PTR(pSigInfo);
	if (!pSigInfo->sigProdPlace.szPostalCode)
	{
		SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido el codigo postal de la ciudad donde se produjo la firma del archivo.");
		return "";
	}
	else
	{
		QString state = pSigInfo->sigProdPlace.szPostalCode;
		return state;
	}
#endif // DIGIDOC
        return QString("");
}

QStringList SafetDocument::signatureLocations(int index) const{
    // crear la lista de cadenas
    QStringList l;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    SignatureInfo* pSigInfo = 0;
    //SignedDoc * pSigDoc = 0;
    //pSigDoc = signedDoc();
    //Q_CHECK_PTR(pSigDoc2);

    int numberOfSignatures = getCountOfSignatures(pSigDoc);

    SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) ,
            tr("No hay informacion del lugar donde se hizo la firma; el archivo no posee firmas."));

    SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) ,
            tr("El indice de la firma buscada para extraer la informacion no existe."));

    pSigInfo = getSignature(pSigDoc, index);
    Q_CHECK_PTR(pSigInfo);

    if (!pSigInfo->sigProdPlace.szCity)
    {
            SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definida la ciudad donde se produjo la firma del archivo.");
            l << QString::fromUtf8("");
    }
    else
    {
            QString city = pSigInfo->sigProdPlace.szCity;
            l << QString::fromUtf8(qPrintable(city)).trimmed();
    }
    if (!pSigInfo->sigProdPlace.szStateOrProvince)
    {
            SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido el estado donde se produjo la firma del archivo.");
            l << QString::fromUtf8("");
    }
    else
    {
            QString state = pSigInfo->sigProdPlace.szStateOrProvince;
            l << QString::fromUtf8(qPrintable(state)).trimmed();
    }
    if (!pSigInfo->sigProdPlace.szPostalCode)
    {
            SafetYAWL::streamlog <<  SafetLog::Debug <<
                    tr("No esta definido el codigo postal de la ciudad donde se produjo la firma del archivo.");
            l << QString::fromUtf8("");
    }
    else
    {
            QString postal = pSigInfo->sigProdPlace.szPostalCode;
            l << QString::fromUtf8(qPrintable(postal)).trimmed();
    }
    if (!pSigInfo->sigProdPlace.szCountryName)
    {
            SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido el pais donde se produjo la firma del archivo.");
            l << QString::fromUtf8("");
    }
    else
    {
            QString country = pSigInfo->sigProdPlace.szCountryName;
            l << QString::fromUtf8(qPrintable(country)).trimmed();
    }
#endif // DIGIDOC
    return l;
}

QString SafetDocument::signatureLocation(int index) const{
    QStringList l = signatureLocations(index);
    l.removeAll( "" );
    return l.join( ", " );
}

int SafetDocument::getCountOfRoles(int index){
      int number = 0;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del numero de roles de la firma; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer el numero de roles no existe."));

	pSigInfo = getSignature(pSigDoc, index);
    	Q_CHECK_PTR(pSigInfo);
        number = getCountOfSignerRoles(pSigInfo, 0);
#endif // DIGIDOC
	return number;

}


QString SafetDocument::getRole(int index, int roleIndex){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del rol del firmante; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer el rol del firmante no existe."));

	pSigInfo = getSignature(pSigDoc, index);
    	Q_CHECK_PTR(pSigInfo);

	int numberOfRoles = getCountOfSignerRoles(pSigInfo, index);

	if ( numberOfRoles == 0)
		return "";

	SafetYAWL::evalAndCheckExit( ((roleIndex >= 0) && (roleIndex < numberOfRoles)) , 
		tr("El indice del rol firmante no existe."));

	QString role = getSignerRole(pSigInfo, 0, roleIndex);
	if (role.isEmpty())
	{
		SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido el rol del firmante del archivo.");
		return "";
	}
	else
	{
		return role;
	}
#endif // DIGIDOC
        return QString("");
}

QString SafetDocument::getSingingTime(int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
        pSigDoc = signedDoc();
        Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion de fecha y hora de la firma; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer la fecha y hora no existe."));

	pSigInfo = getSignature(pSigDoc, index);
    	Q_CHECK_PTR(pSigInfo);
	if (!pSigInfo->szTimeStamp)
	{
		SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido la fecha y hora de firma del archivo.");
		return "";
	}
	else
	{
		QString state = pSigInfo->szTimeStamp;
		return state;
	}
#endif // DIGIDOC
    return QString("");
}

QString SafetDocument::getSingingTimeOnlyDate(int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
        SignatureInfo* pSigInfo = 0;
        SignedDoc * pSigDoc = 0;

        pSigDoc = signedDoc();
        Q_CHECK_PTR(pSigDoc);

        int numberOfSignatures = getCountOfSignatures(pSigDoc);
        SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) ,
                tr("No hay informacion de fecha y hora de la firma; el archivo no posee firmas."));

        SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) ,
                tr("El indice de la firma buscada para extraer la fecha y hora no existe."));

        pSigInfo = getSignature(pSigDoc, index);
        Q_CHECK_PTR(pSigInfo);
        if (!pSigInfo->szTimeStamp)
        {
                SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido la fecha y hora de firma del archivo.");
                return "";
        }
        else
        {
                QString date = pSigInfo->szTimeStamp;
                // aqui se debe extraer SOLO la fecha
                QStringList list = date.split("T");
                QStringList sublist = list.at(0).split("-");
                QString reorderedDate;
                reorderedDate.append(sublist.at(2));
                reorderedDate.append("-");
                reorderedDate.append(sublist.at(1));
                reorderedDate.append("-");
                reorderedDate.append(sublist.at(0));
                return reorderedDate;
        }
#endif // DIGIDOC
        return QString("");
}

QString SafetDocument::getSingingTimeOnlyHour(int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
        SignatureInfo* pSigInfo = 0;
        SignedDoc * pSigDoc = 0;

        pSigDoc = signedDoc();
        Q_CHECK_PTR(pSigDoc);

        int numberOfSignatures = getCountOfSignatures(pSigDoc);
        SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) ,
                tr("No hay informacion de fecha y hora de la firma; el archivo no posee firmas."));

        SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) ,
                tr("El indice de la firma buscada para extraer la fecha y hora no existe."));

        pSigInfo = getSignature(pSigDoc, index);
        Q_CHECK_PTR(pSigInfo);
        if (!pSigInfo->szTimeStamp)
        {
                SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido la fecha y hora de firma del archivo.");
                return "";
        }
        else
        {
                QString date = pSigInfo->szTimeStamp;
                // aqui se debe extraer SOLO la hora
                QStringList list = date.split("T");
                return list.at(1);
        }
#endif
    return QString("");
}


QString SafetDocument::getSignatureType(int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del tipo de firma; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer el tipo de firma no existe."));

	pSigInfo = getSignature(pSigDoc, index);
    	Q_CHECK_PTR(pSigInfo);

	if (!pSigInfo->pSigValue->szType)
	{
		SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido el tipo de firma del archivo.");
		return "";
	}
	else
	{
		QString signatureType = pSigInfo->pSigValue->szType;
		return signatureType;
	}
#endif // DIGIDOC
        return QString("");
}


QString SafetDocument::getSignatureFormat(int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del formato de firma; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer el formato de firma no existe."));

	pSigInfo = getSignature(pSigDoc, index);
    	Q_CHECK_PTR(pSigInfo);

	if ( (!pSigDoc->szFormat) || (!pSigDoc->szFormatVer) )
	{
		SafetYAWL::streamlog <<  SafetLog::Debug << tr("No esta definido el formato de firma del archivo.");
		return "";
	}
	else
	{
		QString signatureFormat("");
		signatureFormat.append(pSigDoc->szFormat);
		signatureFormat.append("/");
		signatureFormat.append(pSigDoc->szFormatVer);
		return signatureFormat;
	}
#endif
        return QString("");
}


QString SafetDocument::getSignerCertificateIssuerName(int index){
    QString issuerName;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del nombre del emisor del certificado firmante; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer el nombre del emisor del certificado firmante no existe."));

	pSigInfo = getSignature(pSigDoc, index);
	Q_CHECK_PTR(pSigInfo);

        issuerName = ddocSigInfo_GetSignersCert_IssuerName(pSigInfo);
	QStringList list = issuerName.split("/");
	for (int i = 0; i < list.size(); ++i)
	{
		if (list.at(i).contains("CN="))
		{
			issuerName = list.at(i);
			issuerName.remove("CN=");
		}
	}
#endif
	return issuerName;
}


QString SafetDocument::getSignerCertificateSerial(int index){
    QString issuerSerial;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del serial del certificado firmante; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer el serial del certificado firmante no existe."));

	pSigInfo = getSignature(pSigDoc, index);
	Q_CHECK_PTR(pSigInfo);

        issuerSerial = ddocSigInfo_GetSignersCert_IssuerSerial(pSigInfo);
#endif // DIGIDOC
	return issuerSerial;
}


QString SafetDocument::getValidAt(int index){
    QString notBefore;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);

	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion del inicio del periodo de validez del certificado firmante; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer el inicio del periodo de validez del certificado firmante no existe."));

	pSigInfo = getSignature(pSigDoc, index);
	Q_CHECK_PTR(pSigInfo);

        X509* currentSigner = ddocSigInfo_GetSignersCert(pSigInfo);
        Q_CHECK_PTR(currentSigner);

        char buf1[500];
	//int getCertNotBefore(const SignedDoc* pSigDoc, X509* cert, char* timestamp, int len)
        int err = getCertNotBefore(pSigDoc, currentSigner, buf1, sizeof(buf1));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("getCertNotBefore != ERR_OK - %1").arg(getErrorString(err)));

        notBefore = buf1;
#endif // DIGIDOC
        return notBefore;
}


QString SafetDocument::getValidUntil(int index){
    QString notAfter;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
	SignatureInfo* pSigInfo = 0;
  	SignedDoc * pSigDoc = 0;
	
	pSigDoc = signedDoc();
  	Q_CHECK_PTR(pSigDoc);
	
	int numberOfSignatures = getCountOfSignatures(pSigDoc);
	SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) , 
		tr("No hay informacion de culminacion del periodo de validez del certificado firmante; el archivo no posee firmas."));
	
	SafetYAWL::evalAndCheckExit( ((index >= 0) && (index < numberOfSignatures)) , 
		tr("El indice de la firma buscada para extraer la culminacion del periodo de validez del certificado firmante no existe."));

	pSigInfo = getSignature(pSigDoc, index);
	Q_CHECK_PTR(pSigInfo);

	X509* currentSigner = ddocSigInfo_GetSignersCert(pSigInfo);
	Q_CHECK_PTR(currentSigner);

	char buf1[500];
	//int getCertNotBefore(const SignedDoc* pSigDoc, X509* cert, char* timestamp, int len)
	int err = getCertNotAfter(pSigDoc, currentSigner, buf1, sizeof(buf1));
	SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("getCertNotAfter != ERR_OK - %1").arg(getErrorString(err)));

        notAfter = buf1;
#endif // DIGIDOC
	return notAfter;
}

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
X509 * SafetDocument::getOCSPResponderCertificate(int index){

    SignatureInfo* pSigInfo = 0;
    SignedDoc * pSigDoc = 0;
    pSigDoc = signedDoc();
    Q_CHECK_PTR(pSigDoc);
    int numberOfSignatures = getCountOfSignatures(pSigDoc);
    SafetYAWL::evalAndCheckExit( ( numberOfSignatures > 0) ,
            tr("No hay informacion del certificado de OCSP; el archivo no posee firmas."));
    pSigInfo = getSignature(pSigDoc, index);
    Q_CHECK_PTR(pSigInfo);
    return ddocSigInfo_GetOCSPRespondersCert(pSigInfo);
}
#endif // DIGIDOC

QString SafetDocument::getPathOfSafetDocument(){
    return m_filename;
}


QStringList SafetDocument::enhancedKeyUsage(int index){
    QStringList list;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC

    X509 * x1 = getSignerCertData(index);
    qDebug("... SafetDocument::enhancedKeyUsage(int index) ... 1");
    EXTENDED_KEY_USAGE *usage = (EXTENDED_KEY_USAGE*) X509_get_ext_d2i( (X509*)x1, NID_ext_key_usage, NULL, NULL );
    if( !usage )
        return QStringList() << QObject::tr("All application policies");
        //qDebug("lista de cadenas vacias");


    for( int i = 0; i < sk_ASN1_OBJECT_num( usage ); ++i ){
        ASN1_OBJECT *obj = sk_ASN1_OBJECT_value( usage, i );
        switch( OBJ_obj2nid( obj ) )
        {
            case NID_client_auth:
                list << QObject::tr("Proves your identity to a remote computer"); break;
            case NID_email_protect:
                list << QObject::tr("Protects e-mail messages"); break;
            case NID_OCSP_sign:
                list << QObject::tr("OCSP signing"); break;
            default: break;
        }
    }
    sk_ASN1_OBJECT_pop_free( usage, ASN1_OBJECT_free );
    for (int i = 0; i < list.size(); ++i)
        qDebug(list.at(i).toLocal8Bit().constData());
    qDebug("... SafetDocument::enhancedKeyUsage(int index) ... 2");
#endif // DIGIDOC
    return list;
}

QString SafetDocument::toUtf16( const QString &in ) const
{
        QString res;
        bool firstByte = true;
        ushort data;
        int i = 0;
        while( i < in.size() )
        {
                if( in.mid( i, 2 ) == "\\x" )
                {
                        if( firstByte )
                                data = in.mid( i+2, 2 ).toUShort( 0, 16 );
                        else
                                res += QChar( (data<<8) + in.mid( i+2, 2 ).toUShort( 0, 16 ) );
                        i += 4;
                }
                else
                {
                        if( firstByte )
                                data = in[i].unicode();
                        else
                                res += QChar( (data<<8) + in[i].unicode() );
                        ++i;
                }
                firstByte = !firstByte;
        }
        return res;
}

QString SafetDocument::toUtf8( const QString &in ) const
{
        QString res;
        int i = 0;
        while( i < in.size() )
        {
                if( in.mid( i, 2 ) == "\\x" )
                {
                        const char data[2] = {
                                in.mid( i+2, 2 ).toUInt( 0, 16 ),
                                in.mid( i+6, 2 ).toUInt( 0, 16 ) };
                        res +=  QString::fromUtf8( data, 2 );
                        i += 8;
                }
                else
                {
                        res += in[i];
                        ++i;
                }
        }
        return res;
}



QString SafetDocument::decode( const QString &in ) const
{
    if( in.contains( "\\x" ) )
        return in.contains( "\\x00" ) ? toUtf16( in ) : toUtf8( in );
    else
        return in;
}

QString SafetDocument::subjectInfoUtf8( int index, /*SubjectInfo*/ int subject ) const
{
    // obtener

    QSslCertificate cert;
    if (subject == 0)
        return decode( cert.subjectInfo( QSslCertificate::Organization ) );
    if (subject == 1)
        return decode( cert.subjectInfo( QSslCertificate::CommonName ) );
    if (subject == 2)
        return decode( cert.subjectInfo( QSslCertificate::LocalityName ) );
    if (subject == 3)
        return decode( cert.subjectInfo( QSslCertificate::OrganizationalUnitName ) );
    if (subject == 4)
        return decode( cert.subjectInfo( QSslCertificate::CountryName ) );
    if (subject == 5)
        return decode( cert.subjectInfo( QSslCertificate::StateOrProvinceName ) );
}

QByteArray SafetDocument::versionNumber(int index)
{

    QByteArray result;

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    X509 * x509 = getSignerCertData(index);
    qDebug("... SafetDocument::versionNumber(int index) ");
        result = QByteArray::number( qlonglong(ASN1_INTEGER_get( ((X509*)x509)->cert_info->version )) + 1 );
#endif // DIGIDOC

    return result;
}

QByteArray SafetDocument::serialNumber(int index)
{
       QByteArray result;

#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    X509 * x509 = getSignerCertData(index);
    qDebug("... SafetDocument::serialNumber()(int index) ");
    result = QByteArray::number( qlonglong(ASN1_INTEGER_get( ((X509*)x509)->cert_info->serialNumber )) );
#endif // DIGIDOC
    return result;
}

QByteArray SafetDocument::toHex( const QByteArray &in, QChar separator )
{
        QByteArray ret = in.toHex().toUpper();
        for( int i = 2; i < ret.size(); i += 3 )
                ret.insert( i, separator );
        return ret;
}

QString SafetDocument::getSubjectDN(int index){
    QString result;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    qDebug("... SafetDocument::getSubjectDN(int index) ... 1");
    X509 * x509 = getSignerCertData(index);
    int err = ERR_OK;
    DigiDocMemBuf pMemBuf;
    pMemBuf.pMem = 0;
    pMemBuf.nLen = 0;

    err = ddocCertGetSubjectDN(x509, &pMemBuf);
    SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("SafetDocument::getSubjectDN - %1").arg(getErrorString(err)));
    result = (const char*)pMemBuf.pMem;
#endif // DIGIDOC
    return result;


}

QString SafetDocument::getIssuerDN(int index){
    QString result;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    qDebug("... SafetDocument::getSubjectDN(int index) ... 1");
    X509 * x509 = getSignerCertData(index);
    int err = ERR_OK;
    DigiDocMemBuf pMemBuf;
    pMemBuf.pMem = 0;
    pMemBuf.nLen = 0;

    err = ddocCertGetIssuerDN(x509, &pMemBuf);
    SafetYAWL::evalAndCheckExit(err == ERR_OK, tr("SafetDocument::getSubjectDN - %1").arg(getErrorString(err)));
    result = (const char*)pMemBuf.pMem;
#endif // DIGIDOC

    return result;
}

QStringList SafetDocument::policies(int index){
    QStringList list;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    X509 * x1 = getSignerCertData(index);
    qDebug("... SafetDocument::policies(int index) ... 1");

    CERTIFICATEPOLICIES *cp = (CERTIFICATEPOLICIES*) X509_get_ext_d2i( (X509*)x1, NID_certificate_policies, NULL, NULL );
    if( !cp )
        return QStringList();


    for( int i = 0; i < sk_POLICYINFO_num( cp ); ++i )
    {
        POLICYINFO *pi = sk_POLICYINFO_value( cp, i );
        char buf[50];
        memset( buf, 0, 50 );
        int len = OBJ_obj2txt( buf, 50, pi->policyid, 1 );
        if( len != NID_undef )
            list << buf;
    }
    sk_POLICYINFO_pop_free( cp, POLICYINFO_free );
    qDebug("... SafetDocument::policies(int index) ... 2");
#endif // DIGIDOC
    return list;
}

QByteArray SafetDocument::authorityKeyIdentifier(int index)
{
    QByteArray out;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    X509 * x1 = getSignerCertData(index);
    qDebug("... SafetDocument::policies(int index) ... 1");
    AUTHORITY_KEYID *id = (AUTHORITY_KEYID *) X509_get_ext_d2i( (X509*)x1, NID_authority_key_identifier, NULL, NULL );


    if( id && id->keyid )
        out = ASN_STRING_to_QByteArray( id->keyid );
    AUTHORITY_KEYID_free( id );
#endif // DIGIDOC
    return out;
}

QByteArray SafetDocument::subjectKeyIdentifier(int index)
{
    QByteArray out;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    X509 * x1 = getSignerCertData(index);
    qDebug("... SafetDocument::policies(int index) ... 1");
    ASN1_OCTET_STRING *id = (ASN1_OCTET_STRING *) X509_get_ext_d2i( (X509*)x1, NID_subject_key_identifier, NULL, NULL );

    if( !id )
        return QByteArray();
    out = ASN_STRING_to_QByteArray( id );
    ASN1_OCTET_STRING_free( id );
#endif
    return out;
}

QHash<int,QString> SafetDocument::keyUsage(int index)
{
    QHash<int,QString> list;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    X509 * x1 = getSignerCertData(index);
    qDebug("... SafetDocument::keyUsage() ... 1");

    ASN1_BIT_STRING *keyusage = (ASN1_BIT_STRING*) X509_get_ext_d2i( (X509*)x1, NID_key_usage, NULL, NULL );
    if( !keyusage )
        return QHash<int,QString>();


    for( int n = 0; n < 9; ++n )
    {
        if( ASN1_BIT_STRING_get_bit( keyusage, n ) )
        {
            switch( n )
            {
                case DigitalSignature: list[n] = QObject::tr("Digital signature"); break;
                case NonRepudiation: list[n] = QObject::tr("Non repudiation"); break;
                case KeyEncipherment: list[n] = QObject::tr("Key encipherment"); break;
                case DataEncipherment: list[n] = QObject::tr("Data encipherment"); break;
                case KeyAgreement: list[n] = QObject::tr("Key agreement"); break;
                case KeyCertificateSign: list[n] = QObject::tr("Key certificate sign"); break;
                case CRLSign: list[n] = QObject::tr("CRL sign"); break;
                case EncipherOnly: list[n] = QObject::tr("Encipher only"); break;
                case DecipherOnly: list[n] = QObject::tr("Decipher only"); break;
                default: break;
            }
        }
    }
    ASN1_BIT_STRING_free( keyusage );
#endif // DIGIDOC
    return list;
}

QString SafetDocument::getPublicKeyAlgorithm(int index){
    QString algorithm;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    X509 * x1 = getSignerCertData(index);
    qDebug("... SafetDocument::getPublicKey() ... 1");
    //EVP_PKEY evp = X509_get_pubkey((X509*)x1);

    EVP_PKEY *key = (EVP_PKEY*) X509_get_pubkey((X509*)x1);
    //unsigned char *data = NULL;
    //int len = 0;
    //QSsl::KeyAlgorithm alg;
    //QSsl::KeyType type;



    switch( EVP_PKEY_type( key->type ) )
    {
    case EVP_PKEY_RSA:
    {
            algorithm = "RSA";
            /*
            RSA *rsa = EVP_PKEY_get1_RSA( key );
            alg = QSsl::Rsa;
            type = rsa->d ? QSsl::PrivateKey : QSsl::PublicKey;
            len = rsa->d ? i2d_RSAPrivateKey( rsa, &data ) : i2d_RSAPublicKey( rsa, &data );
            RSA_free( rsa );
            */
            break;
    }
    case EVP_PKEY_DSA:
    {
            algorithm = "DSA";
            /*
            DSA *dsa = EVP_PKEY_get1_DSA( key );
            alg = QSsl::Dsa;
            type = dsa->priv_key ? QSsl::PrivateKey : QSsl::PublicKey;
            len = dsa->priv_key ? i2d_DSAPrivateKey( dsa, &data ) : i2d_DSAPublicKey( dsa, &data );
            DSA_free( dsa );
            */
            break;
    }
    default: break;
    }
#endif // DIGIDOC

    return algorithm;

}

QString SafetDocument::getPublicKeyLength(int index){
    QString x;
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    X509 * x1 = getSignerCertData(index);
   
    EVP_PKEY *key = (EVP_PKEY*) X509_get_pubkey((X509*)x1);
    int len= 0;
    QString length = "0";
    unsigned char *data = NULL;


    char st[64];
    sprintf(st,"%i bit", EVP_PKEY_size(key) * 8 );
    openssl_error();
    x = st;
#endif // DIGIDOC
    return x;
}

void SafetDocument::writeX509ToFile(QString path, bool PEM, int index){
#ifdef SAFET_DIGIDOC // Firma electronica DIGIDOC
    X509 * cert = getSignerCertData(index);
    FILE *fp;
    char *_a = "a", *_w="w", *p = _w;
    //if (append) p=_a;
    fp = fopen(qPrintable(path) , p);
    if (fp != NULL) {
        if (cert){
            if (PEM)
                PEM_write_X509(fp, cert);
            else
                i2d_X509_fp(fp, cert);
            openssl_error();
        }
    }
    else fopen_error(path);
    fclose(fp);
#endif // DIGIDOC
}






