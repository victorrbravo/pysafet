/*
 * QDigiDocClient
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

#pragma once

#include "ui_SignatureDialog.h"
#include "SafetDocument.h"
//#include "DigiDoc.h"

/*
class SignatureWidget: public QLabel
{
	Q_OBJECT
        bool ocspsignvalid;

public:
        SignatureWidget( const DigiDocSignature &s, unsigned int signnum, bool extended, QWidget *parent = 0,
                         bool ovalid = false);

	bool isTest() const;
	bool isValid() const;
        bool ocspSignValid() const { return ocspsignvalid; }
        void setOCSPSignValid(bool v) { ocspsignvalid = v; }

Q_SIGNALS:
	void removeSignature( unsigned int num );

private Q_SLOTS:
	void link( const QString &url );

private:
	unsigned int num;
	DigiDocSignature s;
	bool test, valid;
};
*/

// ****************************************************************************
// Nueva clase SafetSignatureWidget para eliminar la dependencia de la 
// libreria libdigidocpp

class SafetSignatureWidget: public QLabel
{
	Q_OBJECT
        bool ocspsignvalid;

public:
        SafetSignatureWidget( SafetDocument &s, unsigned int signnum,
                              bool extended, QWidget *parent = 0, bool ovalid = false);

	bool isTest() const;
	bool isValid() const;
        bool ocspSignValid() const { return ocspsignvalid; }
        void setOCSPSignValid(bool v) { ocspsignvalid = v; }

Q_SIGNALS:
	void removeSignature( unsigned int num );

private Q_SLOTS:
	void link( const QString &url );

private:
	unsigned int num;
        SafetDocument s;
	bool test, valid;
};

// ****************************************************************************


/*
class SignatureDialog: public QDialog, private Ui::SignatureDialog
{
	Q_OBJECT

public:
	SignatureDialog( const DigiDocSignature &signature, QWidget *parent = 0 );

private Q_SLOTS:
	void showCertificate();

private:
	void addItem( QTreeWidget *view, const QString &variable, const QString &value );

	DigiDocSignature s;
};
*/

// ****************************************************************************
// Nueva clase SafetSignatureDialog para eliminar la dependencia de la
// libreria libdigidocpp

class SafetSignatureDialog: public QDialog, private Ui::SignatureDialog
{
        Q_OBJECT

public:
        SafetSignatureDialog( SafetDocument s, int signum, QWidget *parent = 0 );

private Q_SLOTS:
        void showCertificate();

private:
        void addItem( QTreeWidget *view, const QString &variable, const QString &value );

        //DigiDocSignature s;
        SafetDocument s;
        int signatureIndex;
};
// ****************************************************************************
