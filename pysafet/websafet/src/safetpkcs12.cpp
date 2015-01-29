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

/*
 * QEstEidCommon
 *
 * Copyright (C) 2009 Jargo Kõster <jargo@innovaatik.ee>
 * Copyright (C) 2009 Raul Metsma <raul@innovaatik.ee>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "safetpkcs12.h"
#include "SafetYAWL.h"

// ****************************************************************************
// Implementacion de la clase SafetPKCS12Private
class SafetPKCS12Private
{
public:
    SafetPKCS12Private(): error(SafetPKCS12::Unknown) {}
    void init( const QByteArray &data, const QByteArray &pin );
    void setLastError();

    QSslCertificate cert;
    QSslKey key;
    SafetPKCS12::ErrorType error;
    QString errorString;
};

void SafetPKCS12Private::init( const QByteArray &data, const QByteArray &pin )
{
    OpenSSL_add_all_algorithms();

    qDebug("... SafetPKCS12Private::init ... 1");
    BIO *bio = BIO_new_mem_buf( const_cast<char*>(data.constData()), data.size() );
    if( !bio ){
        return setLastError();
        /*
        qDebug("!bio");
        qDebug(qPrintable(errorString));
        SafetYAWL::streamlog << SafetLog::Error << QObject::tr("Error al inicializar objeto PKCS12: BIO no valida");
        return;
        */
    }

    PKCS12 *p12 = d2i_PKCS12_bio( bio, NULL );
    BIO_free( bio );
    if( !p12 ){
        return setLastError();
        /*
        qDebug("!p12");
        qDebug(qPrintable(errorString));
        SafetYAWL::streamlog <<  SafetLog::Debug << errorString;
        SafetYAWL::streamlog << SafetLog::Error << QObject::tr("Error al inicializar objeto PKCS12: PKCS12 no valida");
        return;
        */
    }

    X509 *c = NULL;
    EVP_PKEY *k = NULL;
    int ret = PKCS12_parse( p12, pin.constData(), &k, &c, NULL );
    PKCS12_free( p12 );
    if( !ret ){
        return setLastError();
        /*
        setLastError();
        qDebug("!ret");
        qDebug(qPrintable(errorString));
        SafetYAWL::streamlog << SafetLog::Error << QObject::tr("Error al inicializar objeto PKCS12: EVP_PKEY no valida");
        return;
        */
    }
    SafetYAWL::streamlog <<  SafetLog::Debug << QObject::tr("Analizada correctamente la estructura PKCS12");

    cert = SafetPKCS12::fromX509( c );
    key = SafetPKCS12::keyFromEVP( k );

    X509_free( c );
    EVP_PKEY_free( k );
    qDebug("... SafetPKCS12Private::init ... 2");
    SafetYAWL::streamlog << SafetLog::Action << QObject::tr("Inicializado objeto SafetPKCS12 correctamente!");
}

void SafetPKCS12Private::setLastError()
{
    unsigned long err = ERR_get_error();
    if( ERR_GET_LIB(err) == ERR_LIB_PKCS12 )
    {
        switch( ERR_GET_REASON(err) )
        {
            case PKCS12_R_MAC_VERIFY_FAILURE: error = SafetPKCS12::InvalidPassword; break;
            default: error = SafetPKCS12::Unknown; break;
        }
    }
    else
        error = SafetPKCS12::Unknown;
    errorString = ERR_error_string( err, NULL );
}

// ****************************************************************************
// Implementacion de la clase SafetPKCS12
SafetPKCS12::SafetPKCS12( QIODevice *device, const QByteArray &pin )
:	d(new SafetPKCS12Private)
{
    if( device )
        d->init( device->readAll(), pin );
}

SafetPKCS12::SafetPKCS12( const QByteArray &data, const QByteArray &pin )
:	d(new SafetPKCS12Private)
{
    d->init( data, pin );
}

SafetPKCS12::~SafetPKCS12() { delete d; }

QSslCertificate SafetPKCS12::certificate() const { return d->cert; }

SafetPKCS12::ErrorType SafetPKCS12::error() const { return d->error; }

QString SafetPKCS12::errorString() const { return d->errorString; }

bool SafetPKCS12::isNull() const { return d->cert.isNull() && d->key.isNull(); }

QSslKey SafetPKCS12::key() const { return d->key; }


QSslCertificate SafetPKCS12::fromX509( X509 * x509 )
{
    unsigned char *cert = NULL;
    int len = i2d_X509( (X509*)x509, &cert );
    QByteArray der;
    if( len >= 0 )
        der = QByteArray( (char*)cert, len );
    OPENSSL_free( cert );
    return QSslCertificate( der, QSsl::Der );
}

QSslKey SafetPKCS12::keyFromEVP( EVP_PKEY * evp )
{
    EVP_PKEY *key = (EVP_PKEY*)evp;
    unsigned char *data = NULL;
    int len = 0;
    QSsl::KeyAlgorithm alg;
    QSsl::KeyType type;

    switch( EVP_PKEY_type( key->type ) )
    {
        case EVP_PKEY_RSA:
        {
            RSA *rsa = EVP_PKEY_get1_RSA( key );
            alg = QSsl::Rsa;
            type = rsa->d ? QSsl::PrivateKey : QSsl::PublicKey;
            len = rsa->d ? i2d_RSAPrivateKey( rsa, &data ) : i2d_RSAPublicKey( rsa, &data );
            RSA_free( rsa );
            break;
        }
        case EVP_PKEY_DSA:
        {
            DSA *dsa = EVP_PKEY_get1_DSA( key );
            alg = QSsl::Dsa;
            type = dsa->priv_key ? QSsl::PrivateKey : QSsl::PublicKey;
            len = dsa->priv_key ? i2d_DSAPrivateKey( dsa, &data ) : i2d_DSAPublicKey( dsa, &data );
            DSA_free( dsa );
            break;
        }
        default: break;
    }

    QSslKey k;
    if( len > 0 )
        k = QSslKey( QByteArray( (char*)data, len ), alg, QSsl::Der, type );
    OPENSSL_free( data );

    return k;
}

bool SafetPKCS12::writeCert(QString outputPath, bool pem)
{
    if (outputPath.isEmpty())
        return false;

    QFile f( outputPath );
    if( f.open( QIODevice::WriteOnly ) )
    {
        if (pem)
        {
            //f.write( QFileInfo( file ).suffix().toLower() == "pem" ? d->cert.toPem() : d->cert.toDer() );
            f.write( d->cert.toPem());
        }
        else
        {
            f.write( d->cert.toDer());
        }
        f.close();
        return true;
    }
    else
    {
        SafetYAWL::streamlog <<  SafetLog::Error << QObject::tr("No se pudo escribir el certificado extraido del PKCS12");
        return false;
    }
}


bool SafetPKCS12::writeKey(QString outputPath, QString pass, bool pem)
{
    if (outputPath.isEmpty())
        return false;


    QFile f( outputPath );
    if( f.open( QIODevice::WriteOnly ) )
    {
        if (pem)
        {
            //f.write( QFileInfo( file ).suffix().toLower() == "pem" ? d->cert.toPem() : d->cert.toDer() );
            if (!pass.isEmpty())
            {
                QByteArray array(qPrintable(pass));
                f.write( d->key.toPem(array));
            }
            else
            {
                f.write( d->key.toPem());
            }
        }
        else
        {
            if (!pass.isEmpty())
            {
                QByteArray array(qPrintable(pass));
                f.write( d->key.toDer(array));
            }
            else
            {
                f.write( d->key.toDer());
            }
        }
        f.close();
        return true;
    }
    else
    {
        SafetYAWL::streamlog <<  SafetLog::Error << QObject::tr("No se pudo escribir la clave extraida del PKCS12");
        return false;
    }
}


