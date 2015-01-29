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

#include "CertificateWidget.h"

#include "ui_CertificateWidget.h"
//#include "SslCertificate.h"

#include <QDateTime>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QSslKey>

#include "openssl/pem.h"

class CertificateDialogPrivate: public Ui::CertificateDialog
{
public:
        //QSslCertificate cert;
        SafetDocument d;
        X509 * internalCert;
};

CertificateDialog::CertificateDialog( QWidget *parent )
:	QDialog( parent )
,	d( new CertificateDialogPrivate )
{
	d->setupUi( this );
	d->tabWidget->removeTab( 2 );
}
/*
CertificateDialog::CertificateDialog( const QSslCertificate &cert, QWidget *parent )
:	QDialog( parent )
,	d( new CertificateDialogPrivate )
{
	d->setupUi( this );
	setCertificate( cert );
	d->tabWidget->removeTab( 2 );
}
*/

CertificateDialog::CertificateDialog( SafetDocument doc, QWidget *parent, int index )
:	QDialog( parent )
,	d( new CertificateDialogPrivate )
{
        d->setupUi( this );

        setSafetDocument(doc, index);

        d->tabWidget->removeTab( 2 );
}


CertificateDialog::~CertificateDialog() { delete d; }

void CertificateDialog::addItem( const QString &variable, const QString &value, const QVariant &valueext )
{
	QTreeWidgetItem *t = new QTreeWidgetItem( d->parameters );
	t->setText( 0, variable );
	t->setText( 1, value );
	t->setData( 1, Qt::UserRole, valueext );
	d->parameters->addTopLevelItem( t );
}

void CertificateDialog::on_parameters_itemSelectionChanged()
{
	if ( !d->parameters->selectionModel()->hasSelection() || !d->parameters->selectedItems().size() )
		return;
	if( !d->parameters->selectedItems().value(0)->data( 1, Qt::UserRole ).toString().isEmpty() )
		d->parameterContent->setPlainText( d->parameters->selectedItems().value(0)->data( 1, Qt::UserRole ).toString() );
	else
		d->parameterContent->setPlainText( d->parameters->selectedItems().value(0)->text( 1 ) );
}

void CertificateDialog::save()
{
	QString file = QFileDialog::getSaveFileName( this,
                tr("Guardar certificado"),
                QString( "%1%2%3.pem" )
			.arg( QDesktopServices::storageLocation( QDesktopServices::DocumentsLocation ) )
			.arg( QDir::separator() )
                        .arg( "" ),
                tr("Certificados (*.cer *.crt *.pem)") );
	if( file.isEmpty() )
		return;

        //X509 * cert = getSignerCertData(index);
        FILE *fp;
        char *_a = "a", *_w="w", *p = _w;
        //if (append) p=_a;
        fp = fopen(qPrintable(file) , p);
        if (fp != NULL) {
            if (d->internalCert){
                //if (PEM)
                if ((QFileInfo( file ).suffix().toLower() == "pem") || (QFileInfo( file ).suffix().toLower() == "crt"))
                    PEM_write_X509(fp, d->internalCert);
                else
                    i2d_X509_fp(fp, d->internalCert);
                openssl_error("");
            }
        }
        //else fopen_error(file);
        else
            QMessageBox::warning( this, tr("Guardar certificado"), tr("Error al guardar archivo") );
        fclose(fp);

/*
	QFile f( file );
	if( f.open( QIODevice::WriteOnly ) )
	{
		f.write( QFileInfo( file ).suffix().toLower() == "pem" ? d->cert.toPem() : d->cert.toDer() );
		f.close();
	}
	else
                QMessageBox::warning( this, tr("Guardar certificado"), tr("Error al guardar archivo") );
*/
}

/*
void CertificateDialog::setCertificate( const QSslCertificate &cert )
{
	d->cert = cert;
	SslCertificate c = cert;
	QString i;
	QTextStream s( &i );
        s << "<b>" << tr("Información del certificado") << "</b><br />";
	s << "<hr>";
        s << "<b>" << tr("Este certificado tiene los siguientes usos:") << "</b>";
	s << "<ul>";
	Q_FOREACH( const QString &ext, c.enhancedKeyUsage() )
		s << "<li>" << ext << "</li>";
	s << "</ul>";
	s << "<br /><br /><br /><br />";
        //s << tr("* Refiérase a las políticas de la Autoridad de Certificación para más detalles.") << "<br />";
	s << "<hr>";
	s << "<p style='margin-left: 30px;'>";
        s << "<b>" << tr("Emitido a:") << "</b> " << c.subjectInfoUtf8( QSslCertificate::CommonName );
	s << "<br /><br /><br />";
        s << "<b>" << tr("Emitido por:") << "</b> " << c.issuerInfo( QSslCertificate::CommonName );
	s << "<br /><br /><br />";
        s << "<b>" << tr("Válida desde") << "</b> " << SslCertificate::toLocalTime( c.effectiveDate() ).toString( "dd.MM.yyyy" ) << " ";
        s << "<b>" << tr("a") << "</b> "<< SslCertificate::toLocalTime( c.expiryDate() ).toString( "dd.MM.yyyy" );
	s << "</p>";
	d->info->setHtml( i );

        addItem( tr("Versión"), "V" + c.versionNumber() );
        addItem( tr("Número de Serial"), QString( "%1 (0x%2)" )
		.arg( c.serialNumber().constData() )
		.arg( QString::number( c.serialNumber().toInt(), 16 ) ) );
        addItem( tr("Algoritmo de firma"), "sha1RSA" );

	QStringList text, textExt;
	QList<QByteArray> subjects;
	subjects << "CN" << "OU" << "O" << "C";
	Q_FOREACH( const QByteArray &subject, subjects )
	{
		const QString &data = c.issuerInfo( subject );
		if( data.isEmpty() )
			continue;
		text << data;
		textExt << QString( "%1 = %2" ).arg( subject.constData() ).arg( data );
	}
        addItem( tr("Emisor"), text.join( ", " ), textExt.join( "\n" ) );
        addItem( tr("Válido desde"), SslCertificate::toLocalTime( c.effectiveDate() ).toString( "dd.MM.yyyy hh:mm:ss" ) );
        addItem( tr("Válido a"), SslCertificate::toLocalTime( c.expiryDate() ).toString( "dd.MM.yyyy hh:mm:ss" ) );

	subjects.clear();
	text.clear();
	textExt.clear();
	subjects << "serialNumber" << "GN" << "SN" << "CN" << "OU" << "O" << "C";
	Q_FOREACH( const QByteArray &subject, subjects )
	{
		const QString &data = c.subjectInfoUtf8( subject );
		if( data.isEmpty() )
			continue;
		text << data;
		textExt << QString( "%1 = %2" ).arg( subject.constData() ).arg( data );
	}
        addItem( tr("Sujeto"), text.join( ", " ), textExt.join( "\n" ) );
        addItem( tr("Clave pública"), QString("%1 (%2)")
			.arg( c.publicKey().algorithm() == QSsl::Rsa ? "RSA" : "DSA" )
			.arg( c.publicKey().length() ),
		c.toHex( c.publicKey().toDer() ) );

	QStringList enhancedKeyUsage = c.enhancedKeyUsage();
	if( !enhancedKeyUsage.isEmpty() )
		addItem( tr("Enhanched key usage"), enhancedKeyUsage.join( ", " ), enhancedKeyUsage.join( "\n" ) );
	QStringList policies = c.policies();
	if( !policies.isEmpty() )
		addItem( tr("Certificate policies"), policies.join( ", " ) );
	addItem( tr("Authority key identifier"), c.toHex( c.authorityKeyIdentifier() ) );
	addItem( tr("Subject key identifier"), c.toHex( c.subjectKeyIdentifier() ) );
	QStringList keyUsage = c.keyUsage().values();
	if( !keyUsage.isEmpty() )
		addItem( tr("Key usage"), keyUsage.join( ", " ), keyUsage.join( "\n" ) );
}
*/

void CertificateDialog::setSafetDocument( SafetDocument &doc, int index )
{
    qDebug("... CertificateDialog::setSafetDocument( SafetDocument doc ) ... 1");
        QString i;
        QTextStream s( &i );
        s << "<b>" << tr("Información del certificado") << "</b><br />";
        s << "<hr>";
        s << "<b>" << tr("Este certificado tiene los siguientes usos:") << "</b>";
        s << "<ul>";
        Q_FOREACH( const QString &ext, doc.enhancedKeyUsage(index) )
                s << "<li>" << ext << "</li>";
        s << "</ul>";
        s << "<br /><br /><br /><br />";
        //s << tr("* Refiérase a las políticas de la Autoridad de Certificación para más detalles.") << "<br />";
        s << "<hr>";
        s << "<p style='margin-left: 30px;'>";
        s << "<b>" << tr("Emitido a:") << "</b> " << doc.getCN(index);
        s << "<br /><br /><br />";
        //s << "<b>" << tr("Emitido por:") << "</b> " << c.issuerInfo( QSslCertificate::CommonName );
        s << "<b>" << tr("Emitido por:") << "</b> " << doc.getSignerCertificateIssuerName(index);
        s << "<br /><br /><br />";
        //s << "<b>" << tr("Válida desde") << "</b> " << SslCertificate::toLocalTime( c.effectiveDate() ).toString( "dd.MM.yyyy" ) << " ";
        s << "<b>" << tr("Válida desde") << "</b> " << doc.getValidAt(index) << " ";
        //s << "<b>" << tr("a") << "</b> "<< SslCertificate::toLocalTime( c.expiryDate() ).toString( "dd.MM.yyyy" );
        s << "<b>" << tr("a") << "</b> "<< doc.getValidUntil(index);
        s << "</p>";
        d->info->setHtml( i );

        addItem( tr("Versión"), "V" + doc.versionNumber(index) );
        addItem( tr("Número de Serial"), QString( "%1 (0x%2)" )
                .arg( doc.serialNumber().constData() )
                .arg( QString::number( doc.serialNumber().toInt(), 16 ) ) );
        addItem( tr("Algoritmo de firma"), "sha1RSA" );

        QString issuerDN = doc.getIssuerDN(index);
        QStringList issuerList = issuerDN.split("/");
        addItem( tr("Emisor"), doc.getIssuerDN(index), issuerList.join("\n") );

        addItem( tr("Válido desde"), doc.getValidAt(index) );
        addItem( tr("Válido a"), doc.getValidUntil(index) );

        QString subjectDN = doc.getSubjectDN(index);
        QStringList subjectList = subjectDN.split("/");
        addItem( tr("Sujeto"), doc.getSubjectDN(index), subjectList.join("\n") );

        // aqui se debe colocar la clave publica
        addItem( tr("Clave pública"), QString("%1 (%2)")
        //                .arg( c.publicKey().algorithm() == QSsl::Rsa ? "RSA" : "DSA" )
                 .arg( doc.getPublicKeyAlgorithm(index) )
        //         .arg( c.publicKey().length() ));
                 .arg( doc.getPublicKeyLength(index) ));
        //       c.toHex( c.publicKey().toDer() ) );

        QStringList enhancedKeyUsage = doc.enhancedKeyUsage(index);
        if( !enhancedKeyUsage.isEmpty() )
                addItem( tr("Uso de clave extendido"), enhancedKeyUsage.join( ", " ), enhancedKeyUsage.join( "\n" ) );

        QStringList policies = doc.policies(index);
        if( !policies.isEmpty() )
                addItem( tr("Politicas de certificado"), policies.join( ", " ) );

        QStringList keyUsage = doc.keyUsage().values();
        if( !keyUsage.isEmpty() )
            addItem( tr("Uso de clave"), keyUsage.join( ", " ), keyUsage.join( "\n" ) );


        d->internalCert = new X509();
        d->internalCert = doc.getSignerCertData(index);
    qDebug("... CertificateDialog::setSafetDocument( SafetDocument doc ) ... 2");
}

void CertificateDialog::openssl_error(const QString txt) {
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
}
