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

#include "SignatureDialog.h"
#include <QDir>
#include "SafetYAWL.h"
#include "CertificateWidget.h"


//#include <digidocpp/Document.h>

#include <QDateTime>
#include <QSslKey>
#include <QTextStream>
/*
SignatureWidget::SignatureWidget( const DigiDocSignature &signature, unsigned int signnum, bool extended, QWidget *parent,
                                  bool ovalid )
:	QLabel( parent )
,	num( signnum )
,	s( signature )
,	test( false )
,	valid( false )
{

    ocspsignvalid = ovalid;
    qDebug(".....SignatureWidget::SignatureWidget.....(1)");
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
	setWordWrap( true );
	const SslCertificate cert = s.cert();
	QString content;
	QTextStream st( &content );
        //setStyleSheet("{\n"
        //    "background-color: transparent;\n"
        //    "padding: 5px;\n"
        //    "}");
	if( cert.isTempel() )
                st << "<img src=\":/ico_stamp_blue_16.png\">";
	else
                st << "<img src=\":/ico_person_blue_16.png\">";

	test = cert.isTest();
	st << "<b>" << cert.toString( cert.isTempel() ? "CN" : "GN SN" ) << "</b>";

	if( extended )
	{
                st << "<br /> <b>" << tr("Emitido a:") << "</b> "
                        << cert.subjectInfoUtf8( QSslCertificate::CommonName );
                st << "<br /><b>" << tr("Emitido por:") << "</b> " << cert.issuerInfo( QSslCertificate::CommonName );
		st << "<br />" << s.location() << "<br />";
                st << tr("Fecha de Firma:") << " "
			<< SslCertificate::formatDate( s.dateTime(), "dd. MMMM yyyy" ) << " "
                        << tr("hora:") << " "
			<< s.dateTime().toString( "hh:mm" );

                qDebug("fecha de firma:");
                qDebug(qPrintable(SslCertificate::formatDate( s.dateTime(), "dd. MMMM yyyy" )));
                qDebug("hora de firma:");
                qDebug(qPrintable(s.dateTime().toString( "hh:mm" )));
	}
	else
	{
		QString tooltip;
		QTextStream t( &tooltip );
		QString location = s.location();
		if( !location.isEmpty() )
			t << location << "<br />";
		QString role = s.role();
		if( !role.isEmpty() )
			t << role << "<br />";
                t << tr("Firmado en") << " "
			<< SslCertificate::formatDate( s.dateTime(), "dd. MMMM yyyy" ) << " "
                        << tr("Hora") << " "
			<< s.dateTime().toString( "hh:mm" );
		setToolTip( tooltip );

                qDebug("fecha de firma:");
                qDebug(qPrintable(SslCertificate::formatDate( s.dateTime(), "dd. MMMM yyyy" )));
                qDebug("hora de firma:");
                qDebug(qPrintable(s.dateTime().toString( "hh:mm" )));
	}

        st << "<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\"><tr>";
//        st << "<td>" << tr("La Firma Local es") << " ";
//	if( (s.isValid()) )
//                st << "<font color=\"green\">" << tr("Válida ") << "</font>";
//	else
//                st << "<font color=\"red\">" << tr("NO válida") << "</font>";
//       st << "</td><td></td>";
        st << "<td align=\"left\">" << tr("La Firma Electrónica es: ") << " ";
        if( (ocspSignValid()) ) {
                st << "<font color=\"green\">" << tr("\"Válida\" ")
            << "</font>";

         st << "<img src=\":/seal.png\">";
    }
        else {
                st << "<font color=\"red\">" << tr("\"NO válida\"") << "</font>";
            }
        st << "</td>";
        st << "<td align=\"right\">";
        st << "<a href=\"details\">" << tr("Detalles") << "</a>";
        st << "</td></tr>";
//	st << "<td align=\"right\">";
//	st << "<a href=\"remove\">" << tr("Eliminar") << "</a></td>";
        st << "</table>";

	setText( content );

	connect( this, SIGNAL(linkActivated(QString)), SLOT(link(QString)) );
        qDebug(".....SignatureWidget::SignatureWidget.....(2)");
}

bool SignatureWidget::isTest() const { return test; }
bool SignatureWidget::isValid() const { return valid; }

void SignatureWidget::link( const QString &url )
{
        if( url == tr("details") ) {
                qDebug("............details.................(1)");
		SignatureDialog( s, qobject_cast<QWidget*>(parent()) ).exec();
                qDebug("............details.................(2)");
        }
        else if( url == tr("remove") ) {
                    Q_EMIT removeSignature( num );
        }
}
*/

// ****************************************************************************
// Implementacion de nueva clase SafetSignatureWidget para eliminar la
// dependencia de la libreria libdigidocpp

SafetSignatureWidget::SafetSignatureWidget( SafetDocument &doc,
                                            unsigned int signnum, bool extended,
                                            QWidget *parent, bool ovalid )
:	QLabel( parent )
,	num( signnum )
,	s( doc )
,	test( false )
,	valid( false )
{
    ocspsignvalid = ovalid;
    qDebug(".....SafetSignatureWidget::SafetSignatureWidget.....(1)");
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
        setWordWrap( true );

        //const SslCertificate cert = s.getSslSignerCertData((int) signnum);

        QString content;
        QTextStream st( &content );
        //setStyleSheet("{\n"
        //    "background-color: transparent;\n"
        //    "padding: 5px;\n"
        //    "}");
        //if( cert.isTempel() )
        //        st << "<img src=\":/ico_stamp_blue_16.png\">";
        //else
                st << "<img src=\":/ico_person_blue_16.png\">";

        //test = cert.isTest();
        //st << "<b>" << cert.toString( cert.isTempel() ? "CN" : "GN SN" ) << "</b>";

        if( extended )
        {
                st << "<br /> <b>" << tr("Emitido a:") << "</b> "
                        // << cert.subjectInfoUtf8( QSslCertificate::CommonName );
                        << doc.getCN((int) signnum);
                //st << "<br /><b>" << tr("Emitido por:") << "</b> " << cert.issuerInfo( QSslCertificate::CommonName );
                st << "<br /><b>" << tr("Emitido por:") << "</b> " << doc.getSignerCertificateIssuerName((int) signnum);
                st << "<br />" << doc.signatureLocation((int) signnum) << "<br />";
                st << tr("Fecha de Firma:") << " "
                        //<< SslCertificate::formatDate( doc.getSingingTime((int) signnum), "dd. MMMM yyyy" ) << " "
                        << doc.getSingingTimeOnlyDate((int) signnum) << " "
                        << tr("hora:") << " "
                        << doc.getSingingTimeOnlyHour((int) signnum);
        }
        else
        {
                QString tooltip;
                QTextStream t( &tooltip );
                QString location = doc.signatureLocation((int) signnum);
                if( !location.isEmpty() )
                        t << location << "<br />";
                // por ahora se esta tomando el rol con indice 0
                QString role = doc.getRole((int) signnum, 0);
                if( !role.isEmpty() )
                        t << role << "<br />";
                t << tr("Firmado en") << " "
                        << doc.getSingingTimeOnlyDate((int) signnum) << " "
                        << tr("Hora") << " "
                        << doc.getSingingTimeOnlyHour((int) signnum);
                setToolTip( tooltip );
        }

        st << "<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\"><tr>";
//        st << "<td>" << tr("La Firma Local es") << " ";
//	if( (s.isValid()) )
//                st << "<font color=\"green\">" << tr("Válida ") << "</font>";
//	else
//                st << "<font color=\"red\">" << tr("NO válida") << "</font>";
//       st << "</td><td></td>";
        st << "<td align=\"left\">" << tr("La Firma Electrónica es: ") << " ";
        if( (ocspSignValid()) ) {
                st << "<font color=\"green\">" << tr("\"Válida\" ")
            << "</font>";

         st << "<img src=\":/seal.png\">";
    }
        else {
                st << "<font color=\"red\">" << tr("\"NO válida\"") << "</font>";
            }
        st << "</td>";
        st << "<td align=\"right\">";
        st << "<a href=\"details\">" << tr("Detalles") << "</a>";
        st << "</td></tr>";
//	st << "<td align=\"right\">";
//	st << "<a href=\"remove\">" << tr("Eliminar") << "</a></td>";
        st << "</table>";

        setText( content );


        connect( this, SIGNAL(linkActivated(QString)), SLOT(link(QString)) );
        qDebug(".....SafetSignatureWidget::SafetSignatureWidget.....(2)");

}

bool SafetSignatureWidget::isTest() const { return test; }
bool SafetSignatureWidget::isValid() const { return valid; }

void SafetSignatureWidget::link( const QString &url )
{
        if( url == tr("details") ) {
                qDebug("............details.................(1)");
                // a continuacion se despliega el dialogo SafetSignatureDialog para mostrar
                // los elementos de la firma
                //SafetSignatureDialog( s, qobject_cast<QWidget*>(parent()) ).exec();
                SafetSignatureDialog( s, (int) num, qobject_cast<QWidget*>(parent()) ).exec();
                qDebug("............details.................(2)");
        }
        else if( url == tr("remove") ) {
                    Q_EMIT removeSignature( num );
        }
}


// ****************************************************************************


/*
SignatureDialog::SignatureDialog( const DigiDocSignature &signature, QWidget *parent )
:	QDialog( parent )
,	s( signature )
{
	setupUi( this );

	tabWidget->removeTab( 2 );

	const SslCertificate c = s.cert();
        //QString titleText = c.toString( c.isTempel() ? "Número Serial CN " : "GN SN Número Serial" );
        //title->setText( titleText );
        //setWindowTitle( titleText );
        title->setText( tr("Detalles de la firma Electronica") );
        setWindowTitle(tr("Detalles de la firma Electronica"));
        error->setText("");
//	if( s.isValid() )
//                error->setText( tr("Firma inválida") );
//	else
//                error->setText( tr("Firma inválida (%1)")
//                        .arg( s.lastError().isEmpty() ? tr("Error desconocido") : s.lastError() ) );


	const QStringList l = s.locations();
	signerCity->setText( l.value( 0 ) );
	signerState->setText( l.value( 1 ) );
	signerZip->setText( l.value( 2 ) );
	signerCountry->setText( l.value( 3 ) );

	QStringList roles = s.roles();
	signerRole->setText( roles.value(0) );
	signerResolution->setText( roles.value(1) );

	// Certificate info
	QTreeWidget *t = signatureView;
        addItem( t, tr("Fecha de firma"), s.dateTime().toString( "dd.MM.yyyy hh:mm:ss" ) );
        addItem( t, tr("Tipo de firma"), c.publicKey().algorithm() == QSsl::Rsa ? "RSA" : "DSA" );
        addItem( t, tr("Formato de firma"), s.mediaType() );
        addItem( t, tr("Número de archivos"), QString::number( s.parent()->documents().size() ) );
        addItem( t, tr("Número de serial del certificado"), c.serialNumber() );
        addItem( t, tr("Certificado válido desde"), SslCertificate::toLocalTime( c.effectiveDate() ).toString( "dd.MM.yyyy" ) );
        addItem( t, tr("Certificado válido hasta"), SslCertificate::toLocalTime( c.expiryDate() ).toString( "dd.MM.yyyy" ) );
        addItem( t, tr("Emitido a"), c.issuerInfo( QSslCertificate::CommonName ) );
	t->resizeColumnToContents( 0 );

	// OCSP info
	t = ocspView;
        addItem( t, tr("Tipo de firma"), "" );
        addItem( t, tr("Tipo"), s.digestMethod() );
        addItem( t, tr("Número de Serial del certificado"), "" );
        addItem( t, tr("Hora"), "" );
	addItem( t, tr("SHA1 hash"), "" );
        addItem( t, tr("Nombre sujeto"), "" );
	t->resizeColumnToContents( 0 );
}

void SignatureDialog::addItem( QTreeWidget *view, const QString &variable, const QString &value )
{
	QTreeWidgetItem *i = new QTreeWidgetItem( view );
	i->setText( 0, variable );
	i->setText( 1, value );
	view->addTopLevelItem( i );
}

void SignatureDialog::showCertificate()
{ CertificateDialog( s.cert(), this ).exec(); }

*/


// ****************************************************************************
// Implementacion de nueva clase SafetSignatureDialog para eliminar la
// dependencia de la libreria libdigidocpp

SafetSignatureDialog::SafetSignatureDialog( SafetDocument doc, int signum, QWidget *parent )
:	QDialog( parent )
,	s( doc )
,       signatureIndex( signum )
{
        setupUi( this );
    qDebug("... SafetSignatureDialog::SafetSignatureDialog( SafetDocument doc, int signum, QWidget *parent ) ... 1");
        tabWidget->removeTab( 2 );

        //const SslCertificate c = s.cert();
        //const SslCertificate c = doc.getSslSignerCertData(signum);

        //QString titleText = c.toString( c.isTempel() ? "Número Serial CN " : "GN SN Número Serial" );
        //title->setText( titleText );
        //setWindowTitle( titleText );
        title->setText( tr("Detalles de la firma Electronica") );
        setWindowTitle(tr("Detalles de la firma Electronica"));
        error->setText("");
//	if( s.isValid() )
//                error->setText( tr("Firma inválida") );
//	else
//                error->setText( tr("Firma inválida (%1)")
//                        .arg( s.lastError().isEmpty() ? tr("Error desconocido") : s.lastError() ) );


        //const QStringList l = s.locations();
        const QStringList l = s.signatureLocations(signum);

        signerCity->setText( l.value( 0 ) );
        signerState->setText( l.value( 1 ) );
        signerZip->setText( l.value( 2 ) );
        signerCountry->setText( l.value( 3 ) );

        //QStringList roles = s.roles();
        //QStringList roles = s.signature;

        //signerRole->setText( roles.value(0) );
        signerRole->setText( s.getRole(signum, 0) );

        //signerResolution->setText( roles.value(1) );
        //signerResolution->setText( s.getRole(signum, 1) );

        // Certificate info
        QTreeWidget *t = signatureView;
        //addItem( t, tr("Fecha de firma"), s.dateTime().toString( "dd.MM.yyyy hh:mm:ss" ) );
        addItem( t, tr("Fecha de firma"), s.getSingingTimeOnlyDate(signum)+" "+s.getSingingTimeOnlyHour(signum));

        //addItem( t, tr("Tipo de firma"), c.publicKey().algorithm() == QSsl::Rsa ? "RSA" : "DSA" );
        //addItem( t, tr("Tipo de firma"), s.getSignatureType(signum) );

        //addItem( t, tr("Formato de firma"), s.mediaType() );
        addItem( t, tr("Formato de firma"), s.getSignatureFormat(signum) );

        //addItem( t, tr("Número de archivos"), QString::number( s.parent()->documents().size() ) );
        addItem( t, tr("Número de archivos"), QString::number( s.numberOfDataFileOnOpenXAdESContainer() ) );

        //addItem( t, tr("Número de serial del certificado"), c.serialNumber() );
        addItem( t, tr("Número de serial del certificado"), s.getSignerCertificateSerial(signum) );

        //addItem( t, tr("Certificado válido desde"), SslCertificate::toLocalTime( c.effectiveDate() ).toString( "dd.MM.yyyy" ) );
        addItem( t, tr("Certificado válido desde"), s.getValidAt(0) );

        //addItem( t, tr("Certificado válido hasta"), SslCertificate::toLocalTime( c.expiryDate() ).toString( "dd.MM.yyyy" ) );
        addItem( t, tr("Certificado válido hasta"), s.getValidUntil(signum) );

        //addItem( t, tr("Emitido a"), c.issuerInfo( QSslCertificate::CommonName ) );
        addItem( t, tr("Emitido a"), s.getCN(signum) );

        t->resizeColumnToContents( 0 );

        // OCSP info
        t = ocspView;
        addItem( t, tr("Tipo de firma"), "" );
        addItem( t, tr("Tipo"), "" /*s.digestMethod()*/ );
        addItem( t, tr("Número de Serial del certificado"), "" );
        addItem( t, tr("Hora"), "" );
        addItem( t, tr("SHA1 hash"), "" );
        addItem( t, tr("Nombre sujeto"), "" );
        t->resizeColumnToContents( 0 );
    qDebug("... SafetSignatureDialog::SafetSignatureDialog( SafetDocument doc, int signum, QWidget *parent ) ... 2");
}

void SafetSignatureDialog::addItem( QTreeWidget *view, const QString &variable, const QString &value )
{
        QTreeWidgetItem *i = new QTreeWidgetItem( view );
        i->setText( 0, variable );
        i->setText( 1, value );
        view->addTopLevelItem( i );
}

void SafetSignatureDialog::showCertificate()
//{ CertificateDialog( s.cert(), this ).exec(); }
{
    //SafetDocument document(s);
    //SslCertificate sslCertificate = document.getSslSignerCertData(0);
    CertificateDialog( s, this, signatureIndex ).exec();
    //CertificateDialog( sslCertificate, this ).exec();
}

// ****************************************************************************
