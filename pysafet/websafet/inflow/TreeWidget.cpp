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

#include "TreeWidget.h"


//#include <digidocpp/Document.h>

#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QKeyEvent>
#include <QMimeData>
#include <QUrl>

#include "SafetYAWL.h"

TreeWidget::TreeWidget( QWidget *parent )
:	QTreeWidget( parent )
{
	setColumnCount( 4 );
	header()->setStretchLastSection( false );
	header()->setResizeMode( 0, QHeaderView::Stretch );
	header()->setResizeMode( 1, QHeaderView::ResizeToContents );
	header()->setResizeMode( 2, QHeaderView::ResizeToContents );
	header()->setResizeMode( 3, QHeaderView::ResizeToContents );
	connect( this, SIGNAL(clicked(QModelIndex)), SLOT(clicked(QModelIndex)) );
	connect( this, SIGNAL(doubleClicked(QModelIndex)), SLOT(openFile(QModelIndex)) );
}

void TreeWidget::clicked( const QModelIndex &index )
{
	switch( index.column() )
	{
	case 2:
	{
		QString filepath = QFileDialog::getSaveFileName( this,
                        tr("Guardar Archivo"), QString( "%1/%2" )
				.arg( QDesktopServices::storageLocation( QDesktopServices::DocumentsLocation ) )
				.arg( model()->index( index.row(), 0 ).data().toString() ) );
		if( !filepath.isEmpty() )
                {
                    //Q_EMIT save( index.row(), filepath );
                    qDebug("TreeWidget::clicked: CADENA RUTA ");
                    qDebug(qPrintable(getRuta()));
                    if (QFile::copy(getRuta(), filepath))
                        SafetYAWL::streamlog <<  SafetLog::Debug <<
                                tr("Escrito el archivo: %1 en el sistema de archivos (%2)").arg(getRuta()).arg(filepath);
                    else{
                        SafetYAWL::streamlog <<  SafetLog::Debug <<
                                tr("No se pudo escribir el archivo: %1 en el sistema de archivos!!!").arg(getRuta());
                    }
                }
		break;
	}
	case 3: Q_EMIT remove( index.row() ); break;
	default: break;
	}
}

void TreeWidget::keyPressEvent( QKeyEvent *e )
{
	switch( e->key() )
	{
	case Qt::Key_Delete:
		Q_EMIT remove( currentIndex().row() );
		e->accept();
		break;
	case Qt::Key_Return:
		openFile( currentIndex() );
		e->accept();
		break;
	default: return QTreeWidget::keyPressEvent( e );
	}
}

QMimeData* TreeWidget::mimeData( const QList<QTreeWidgetItem*> items ) const
{
	QList<QUrl> list;
	Q_FOREACH( QTreeWidgetItem *item, items )
		list << url( indexFromItem( item ) );
	QMimeData *data = new QMimeData();
	data->setUrls( list );
	return data;
}

QStringList TreeWidget::mimeTypes() const
{ return QStringList() << "text/uri-list"; }

void TreeWidget::openFile( const QModelIndex &index )
{ QDesktopServices::openUrl( url( index ) ); }


QString TreeWidget::fileSize( quint64 bytes )
{
        const quint64 kb = 1024;
        const quint64 mb = 1024 * kb;
        const quint64 gb = 1024 * mb;
        const quint64 tb = 1024 * gb;
        if( bytes >= tb )
                return QString( "%1 TB" ).arg( qreal(bytes) / tb, 0, 'f', 3 );
        if( bytes >= gb )
                return QString( "%1 GB" ).arg( qreal(bytes) / gb, 0, 'f', 2 );
        if( bytes >= mb )
                return QString( "%1 MB" ).arg( qreal(bytes) / mb, 0, 'f', 1 );
        if( bytes >= kb )
                return QString( "%1 KB" ).arg( bytes / kb );
        return QString( "%1 B" ).arg( bytes );
}

/*
void TreeWidget::setContent( const QList<digidoc::Document> &docs )
{
	clear();
	Q_FOREACH( const digidoc::Document &file, docs )
	{
		QTreeWidgetItem *i = new QTreeWidgetItem( this );
		i->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled );

		QFileInfo info( QString::fromUtf8( file.getPath().data() ) );
		i->setText( 0, info.fileName() );
		i->setData( 0, Qt::ToolTipRole, info.fileName() );
		i->setData( 0, Qt::UserRole, info.absoluteFilePath() );

                i->setText( 1, TreeWidget::fileSize( info.size() ) );
		i->setData( 1, Qt::TextAlignmentRole, (int)Qt::AlignRight|Qt::AlignVCenter );
		i->setData( 1, Qt::ForegroundRole, Qt::gray );

                i->setData( 2, Qt::DecorationRole, QPixmap(":/ico_save.png") );
                i->setData( 2, Qt::ToolTipRole, tr("Guardar") );
		i->setData( 2, Qt::SizeHintRole, QSize( 20, 20 ) );

                i->setData( 3, Qt::DecorationRole, QPixmap(":/ico_delete.png") );
                i->setData( 3, Qt::ToolTipRole, tr("Eliminar") );
		i->setData( 3, Qt::SizeHintRole, QSize( 20, 20 ) );

		addTopLevelItem( i );
	}
}
*/

void TreeWidget::setContent( SafetDocument doc )
{
    clear();
    qDebug("... TreeWidget::setContent( SafetDocument doc ) ... 1");
    for ( int i = 0; i < doc.numberOfDataFileOnOpenXAdESContainer(); i++){
        QTreeWidgetItem *j = new QTreeWidgetItem( this );
        j->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled );

        // extraer el archivo (DataFile) y escribirlo en un directorio temporal
        // /tmp/qt_temp.TXXXXX
        QString writtenFile = doc.writeDataFileOfContainerToTmp(i, doc.getPathOfSafetDocument());
        QFileInfo info( QString::fromUtf8( qPrintable(writtenFile) ) );

        ruta = writtenFile;

        j->setText( 0, info.fileName() );
        j->setData( 0, Qt::ToolTipRole, info.fileName() );
        j->setData( 0, Qt::UserRole, info.absoluteFilePath() );

        j->setText( 1, TreeWidget::fileSize( info.size() ) );
        j->setData( 1, Qt::TextAlignmentRole, (int)Qt::AlignRight|Qt::AlignVCenter );
        j->setData( 1, Qt::ForegroundRole, Qt::gray );

        j->setData( 2, Qt::DecorationRole, QPixmap(":/ico_save.png") );
        j->setData( 2, Qt::ToolTipRole, tr("Guardar") );
        j->setData( 2, Qt::SizeHintRole, QSize( 20, 20 ) );

        j->setData( 3, Qt::DecorationRole, QPixmap(":/ico_delete.png") );
        j->setData( 3, Qt::ToolTipRole, tr("Eliminar") );
        j->setData( 3, Qt::SizeHintRole, QSize( 20, 20 ) );

        addTopLevelItem( j );
    }

/*
        clear();
        Q_FOREACH( const digidoc::Document &file, docs )
        {
                QTreeWidgetItem *i = new QTreeWidgetItem( this );
                i->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled );

                QFileInfo info( QString::fromUtf8( file.getPath().data() ) );
                i->setText( 0, info.fileName() );
                i->setData( 0, Qt::ToolTipRole, info.fileName() );
                i->setData( 0, Qt::UserRole, info.absoluteFilePath() );

                i->setText( 1, TreeWidget::fileSize( info.size() ) );
                i->setData( 1, Qt::TextAlignmentRole, (int)Qt::AlignRight|Qt::AlignVCenter );
                i->setData( 1, Qt::ForegroundRole, Qt::gray );

                i->setData( 2, Qt::DecorationRole, QPixmap(":/ico_save.png") );
                i->setData( 2, Qt::ToolTipRole, tr("Guardar") );
                i->setData( 2, Qt::SizeHintRole, QSize( 20, 20 ) );

                i->setData( 3, Qt::DecorationRole, QPixmap(":/ico_delete.png") );
                i->setData( 3, Qt::ToolTipRole, tr("Eliminar") );
                i->setData( 3, Qt::SizeHintRole, QSize( 20, 20 ) );

                addTopLevelItem( i );
        }
*/
    qDebug("... TreeWidget::setContent( SafetDocument doc ) ... 2");
}


Qt::DropActions TreeWidget::supportedDropActions() const
{ return Qt::CopyAction; }

QUrl TreeWidget::url( const QModelIndex &item ) const
{ return QUrl::fromLocalFile( item.model()->index( item.row(), 0 ).data( Qt::UserRole ).toString() ); }
