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

#ifndef SAFETPKCS12_H
#define SAFETPKCS12_H

// #pragma once

#include <QSslCertificate>
#include <QSslKey>
#include <QStringList>

#include <openssl/err.h>
#include <openssl/x509v3.h>
#include <openssl/pkcs12.h>

/**
* \brief Clase SafetPKCS12
*
* Esta Clase corresponde a la abstraccion de un objeto PKCS12.
*
* Se desea proveer acceso a objetos PKCS12 y funciones para leer y extraer elementos
* del objeto.
*/

class SafetPKCS12Private;

class SafetPKCS12
{

public:

    enum ErrorType
    {
            InvalidPassword = 1,
            Unknown = -1,
    };


    /**
    * Constructor de la clase SafetPKCS12
    * \param device dispositivo desde donde leer el PKCS12 (archivo. p.ej: QFile)
    * \param pin corresponde a la contrasena que protege el PKCS12
    */
    SafetPKCS12( QIODevice *device, const QByteArray &pin );

    /**
    * Constructor de la clase SafetPKCS12
    * \param data datos asociados al PKCS12
    * \param pin corresponde a la contrasena que protege el PKCS12
    */
    SafetPKCS12( const QByteArray &data, const QByteArray &pin );

    /**
    * Destructor de la clase SafetPKCS12
    */
    ~SafetPKCS12();

    /**
    * Retorna un QSslCertificate asociado al certificado incluido en el PKCS12
    * \return QSslCertificado objeto asociado al certificado incluido en el PKCS12
    */
    QSslCertificate certificate() const;

    // Retorna el tipo de error: InvalidPassword o Unknown
    /**
    * Retorna el tipo de error: InvalidPassword o Unknown
    * \return tipo de error: InvalidPassword o Unknown
    */
    ErrorType error() const;

    /**
    * Retorna una cadena con error
    * \return QString con cadena de error
    */
    QString errorString() const;

    /**
    * Retorna si PKCS12 es nulo
    * \return verdadero si PKCS12 es nulo
    */
    bool isNull() const;

    /**
    * Retorna la clave incluida en el PKCS12
    * \return QSslKey clave incluida en el PKCS12
    */
    QSslKey key() const;

    /**
    * retorna objeto QSslCertificate a partir de X509*
    * \return QSslCertificate a partir de X509*
    */
    static QSslCertificate fromX509( X509 * x509 );

    /**
    * retorna objeto QSslKey a partir de EVP_PKEY*
    * \return QSslKey a partir de EVP_PKEY*
    */
    static QSslKey keyFromEVP( EVP_PKEY * evp );

    /**
    * Escribe el certificado del objeto PKCS12 en la ruta pasada como argumento
    * \param outputPath ruta absoluta donde se desea escribir el archivo
    * \param pem verdadero si se desea escribir el certificado en formato PEM, en
    * caso contrario es DER
    * \return verdadero en caso de haber escrito correctamente el certificado
    */
    bool writeCert(QString outputPath, bool pem = true);

    /**
    * Escribe la clave del objeto PKCS12 en la ruta pasada como argumento
    * \param outputPath ruta absoluta donde se desea escribir el archivo
    * \param pem verdadero si se desea escribir la clave en formato PEM, en
    * caso contrario es DER
    * \param pass contrasena de al menos cuatro (4) caracteres para proteger
    * la clave. Si no se pasa este parametro la clave no se cifra con clave simetrica
    * \return verdadero en caso de haber escrito correctamente la clave
    */
    bool writeKey(QString outputPath, QString pass, bool pem = true);

private:
    SafetPKCS12Private *d;

};



#endif // SAFETPKCS12_H
