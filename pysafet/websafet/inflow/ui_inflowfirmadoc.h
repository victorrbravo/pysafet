/********************************************************************************
** Form generated from reading UI file 'inflowfirmadoc.ui'
**
** Created: Tue Jun 26 15:53:42 2012
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFLOWFIRMADOC_H
#define UI_INFLOWFIRMADOC_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_InflowFirmaDoc
{
public:
    QGridLayout *gridLayout;
    QLabel *labelTitulo;
    QWebView *webDocument;
    QLabel *label;
    QListWidget *listSigners;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonVerPDF;
    QPushButton *buttonImprimir;
    QPushButton *buttonGuardar;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *InflowFirmaDoc)
    {
        if (InflowFirmaDoc->objectName().isEmpty())
            InflowFirmaDoc->setObjectName(QString::fromUtf8("InflowFirmaDoc"));
        InflowFirmaDoc->resize(640, 513);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/firmadoc.png"), QSize(), QIcon::Normal, QIcon::Off);
        InflowFirmaDoc->setWindowIcon(icon);
        gridLayout = new QGridLayout(InflowFirmaDoc);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelTitulo = new QLabel(InflowFirmaDoc);
        labelTitulo->setObjectName(QString::fromUtf8("labelTitulo"));
        labelTitulo->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labelTitulo, 0, 0, 1, 1);

        webDocument = new QWebView(InflowFirmaDoc);
        webDocument->setObjectName(QString::fromUtf8("webDocument"));
        webDocument->setUrl(QUrl("about:blank"));

        gridLayout->addWidget(webDocument, 1, 0, 1, 1);

        label = new QLabel(InflowFirmaDoc);
        label->setObjectName(QString::fromUtf8("label"));
        label->setTextFormat(Qt::RichText);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 2, 0, 1, 1);

        listSigners = new QListWidget(InflowFirmaDoc);
        listSigners->setObjectName(QString::fromUtf8("listSigners"));
        listSigners->setMaximumSize(QSize(16777215, 200));
        listSigners->setFrameShape(QFrame::WinPanel);
        listSigners->setFrameShadow(QFrame::Raised);
        listSigners->setModelColumn(0);

        gridLayout->addWidget(listSigners, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonVerPDF = new QPushButton(InflowFirmaDoc);
        buttonVerPDF->setObjectName(QString::fromUtf8("buttonVerPDF"));

        horizontalLayout->addWidget(buttonVerPDF);

        buttonImprimir = new QPushButton(InflowFirmaDoc);
        buttonImprimir->setObjectName(QString::fromUtf8("buttonImprimir"));

        horizontalLayout->addWidget(buttonImprimir);

        buttonGuardar = new QPushButton(InflowFirmaDoc);
        buttonGuardar->setObjectName(QString::fromUtf8("buttonGuardar"));

        horizontalLayout->addWidget(buttonGuardar);


        gridLayout->addLayout(horizontalLayout, 4, 0, 1, 1);

        buttonBox = new QDialogButtonBox(InflowFirmaDoc);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 5, 0, 1, 1);


        retranslateUi(InflowFirmaDoc);
        QObject::connect(buttonBox, SIGNAL(accepted()), InflowFirmaDoc, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), InflowFirmaDoc, SLOT(reject()));

        QMetaObject::connectSlotsByName(InflowFirmaDoc);
    } // setupUi

    void retranslateUi(QDialog *InflowFirmaDoc)
    {
        InflowFirmaDoc->setWindowTitle(QApplication::translate("InflowFirmaDoc", "Firma Electr\303\263nica de documentos", 0, QApplication::UnicodeUTF8));
        labelTitulo->setText(QApplication::translate("InflowFirmaDoc", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#00007f;\">\302\277Desea Firmar este Documento?</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("InflowFirmaDoc", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'DejaVu Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Personas que han firmado este documento:</span></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
        buttonVerPDF->setText(QApplication::translate("InflowFirmaDoc", "Ver &PDF", 0, QApplication::UnicodeUTF8));
        buttonImprimir->setText(QApplication::translate("InflowFirmaDoc", "&Imprimir", 0, QApplication::UnicodeUTF8));
        buttonGuardar->setText(QApplication::translate("InflowFirmaDoc", "&Guardar como...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InflowFirmaDoc: public Ui_InflowFirmaDoc {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFLOWFIRMADOC_H
