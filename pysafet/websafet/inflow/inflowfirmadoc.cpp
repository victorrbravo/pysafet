#include "inflowfirmadoc.h"
#include "mainwindow.h"
#include <QProcess>


InflowFirmaDoc::InflowFirmaDoc(QObject *parent) 
{
    jscriptload = false; // No cargar el script
    _listprincipaltitle = "";
    _listprincipalcount = 0;



    SYD << tr("......****InflowFirmaDoc...WebView (1)...");

    //webDocument = new QWebView();

	
}

InflowFirmaDoc::~InflowFirmaDoc() {
}

void InflowFirmaDoc::doPrint() {
    qDebug("...InflowFirmaDoc::doprint....");

}

void InflowFirmaDoc::savepdf() {

//    QString myfile = QString("/var/www/media/testgen.pdf");

    SYD << tr("...InflowFirmaDoc::savepdf...Imprimiendo..... newfile:|%1|").arg(newHtmlFile());
//    SYD << tr("...InflowFirmaDoc::savepdf...Imprimiendo..... myfile:|%1|").arg(myfile);

//    QString mycommand = "/home/vbravo/wkhtmltopdf/wkhtmltopdf-amd64";
//    QProcess gtopdf;

//    gtopdf.start(mycommand, QStringList() << newHtmlFile() << myfile);

//    gtopdf.waitForStarted();

//    if ( !gtopdf.waitForFinished(23500) ) {

//		SYD  << tr("No se completo la conversion de HTML a PDF");
//		return;
//	}
	
    SYD << tr(".......InflowFirmaDoc...savepdf...testgen.pdf impreso  correctamente!  ");

}

void InflowFirmaDoc::viewpdf() {



    QString  myfile = SafetYAWL::getTempNameFiles(1).at(0)+".pdf";


}

void InflowFirmaDoc::loadSignersInfo(const QList<SafetWorkflow::InfoSigner>& l) {
   
}

void InflowFirmaDoc::generateJScriptreports(const QString& documents,
                                            const QList<QSqlField>& fields,
                                            int ncounts,
                                            const QString& entitytosign,
                                            QList<SafetWorkflow::InfoSigner>& signers
                                            )
{

    jscriptload = true;
    currentDocuments = documents;
//    qDebug("....currentDocuments...generateJScriptreports: %s", qPrintable(currentDocuments));
//    qDebug("..fields.count()...: %d", fields.count());
    currentFields = fields;
    _listprincipalcount = ncounts;
    _listprincipaltitle = entitytosign;
    _entityToSign = entitytosign;

    QString currentreg, totalregs;
    for(int i=0; i < signers.count(); i++) {
        currentreg = "{";
        currentreg += "commonName:";
        currentreg += "\""+ signers.at(i).commonName+"\",";
        currentreg += "issuer:";
        currentreg += "\""+ signers.at(i).issuer+"\",";
        currentreg += "date:";
        currentreg += "\""+ signers.at(i).date+"\",";
        currentreg += "hour:";
        currentreg += "\""+ signers.at(i).hour+"\",";
        currentreg += "location:";
        currentreg += "\""+ signers.at(i).location+"\",";
        currentreg += "role:";
        currentreg += "\""+ signers.at(i).role+"\",";
        currentreg += "serial:";
        currentreg += "\""+ signers.at(i).serial+"\",";
        currentreg += "typevalid:";
        currentreg += "\""+ signers.at(i).typevalid+"\",";
//        currentreg += "hexsign:";
//        currentreg += "\""+ QString(signers.at(i).hexsign.toHex().data())+"\",";
//        currentreg += "digest:";
//        currentreg += "\""+ QString(signers.at(i).digest.toHex().data())+"\",";

        currentreg += "isvalid:";
        if ( signers.at(i).isvalid ) {
            currentreg +=  "true ";
        }
        else {
            currentreg +=  "false ";
        }
        currentreg += "},";
        totalregs = totalregs + currentreg;
    }
    totalregs.chop(1);
    _currentSigners = totalregs;
}



void InflowFirmaDoc::executeJSCodeAfterLoad(bool ok ) {
     currentDocuments.replace("\n","");
     // ** Reemplazar caracteres especiales, hacen falta pruebas aqui
     QString data;
    data = QString("safettitle = '%1';\n").arg(_listprincipaltitle);
    data += QString("safetlistcount = %1;\n").arg(_listprincipalcount);


    QString dataAfile =  "var  safetafile = \"\";\n";

    foreach(QString afile, availableFiles()) {
        dataAfile = QString("var safetafile = \"%1\";\n")
                .arg(afile);
    }

    data += dataAfile;
    jscriptcolumns = "safetcolumns = [";
    int i = 0;
    foreach(QSqlField f, currentFields ) {

        QString scriptlen = SafetYAWL::getScriptLen(f);
         jscriptcolumns
                 += QString("{ key: \"%1\",label:\"%1\",width:%2,"
                            "resizeable:true,sortable:true},\n")
                    .arg(f.name())
                    .arg(scriptlen);

         i++;
    }
    if ( i > 0 ) {
        jscriptcolumns.chop(2);
    }
    jscriptcolumns += "];";
    QString code;
    code = data;    
    code += "\nsafetsigners = [";
    code += _currentSigners;
    code += "];\n";
    code += " ";
    code += jscriptcolumns;
    code += "\n\n";

    code += "safetlist = [";
    code += currentDocuments;
    code += "];\n";


    _allcode = code;	

}


    // Calcular la longitud del campo para la tabla generada en Listar_Datos

QString InflowFirmaDoc::evalJS(const QString &js) {
     qDebug("***.....evalJS...QWebFrame *frame = m_ui->webDocument->page()->mainFrame();");
    return QString();
}


void InflowFirmaDoc::loadReportTemplate(const QString& bdocname, const QString& newname) {

    QString html;

        QString fullnamePage;
        fullnamePage = SafetYAWL::getConf()["Reports/documenttosign.template"];




       QFile infile(fullnamePage);
       if ( !infile.open(QIODevice::ReadOnly) ) {

               SYE << tr("No es posible leer el archivo de plantilla (Firma): \"%1\" ")
                      .arg(fullnamePage);
           return;
       }



	SYD << tr("..........InflowFirmaDoc::loadReportTemplate....fullnamePage:|%1|").arg(fullnamePage);

    SYD << tr("..........InflowFirmaDoc::loadReportTemplate....ALLCODE _allcode:|%1|")
           .arg(_allcode);

    SYD << tr("..........InflowFirmaDoc::loadReportTemplate....CURRENTDOCUMENTS currentDocuments:|%1|")
           .arg(currentDocuments);

    QTextStream in(&infile);
       in.setCodec("ISO 8859-1");

       QString tpath;

       tpath =    SafetYAWL::getConf()["GeneralOptions/url.media"].trimmed() + "/" + "templates/";
       SYD << tr("..........InflowFirmaDoc::loadReportTemplate....TEMPLATEPATH...reading");




       SYD << tr("..........InflowFirmaDoc::loadReportTemplate....TEMPLATEPATH tpath:|%1|")
              .arg(tpath);

        while(!in.atEnd()) {
           QString line = in.readLine();
//           if (line.indexOf("<SAFET/>") != -1 && !isreplacing) {
//                line.replace("<SAFET/>",json);
//           }


           if ( line.indexOf("<TEMPLATEPATH/>")!= -1 )  {
               line.replace("<TEMPLATEPATH/>",tpath);
           }
           if (line.indexOf("<SAFETDOCS/>") != -1 ) {
                line.replace("<SAFETDOCS/>",currentDocuments);
           }
           else if (line.indexOf("<SAFETFILENAME/>") != -1 ) {
               line.replace("<SAFETFILENAME/>",bdocname);
           }
           else if (line.indexOf("<SAFETCODE/>") != -1 ) {
                line.replace("<SAFETCODE/>",_allcode);
		SYD << tr("...........InflowfirmaDoc..replacing <SAFETCODE/> by \n|%1|").arg(_allcode);
           }
           else  if (line.indexOf("<SAFETSIGNERS/>") != -1 ) {
                line.replace("<SAFETSIGNERS/>",_currentSigners);
           }
 
           html += line;
           html += "\n";

       }

       infile.close();

//       _newhtmlfile = SafetYAWL::getTempNameFiles(1).at(0) + ".html";

//       _newhtmlfile = "/var/www/media/"+ _newhtmlfile.section("/",-1);

       if (newname.isEmpty()) {
        _newhtmlfile = QString("/var/www/media/")+ QString("testgen.html");
       }
       else {
           _newhtmlfile = QString("/var/www/media/")+ newname;
       }
       //_newhtmlfile = "../media/testgen.html";
       SYD << tr(".......InflowFirmaDoc...newfile:...|%1|").arg(_newhtmlfile);



       QFile outfile(_newhtmlfile);
       if ( !outfile.open(QIODevice::WriteOnly | QIODevice::Truncate) ) {

               SYE << tr("No es posible escribir el archivo de plantilla (Firma): \"%1\" ")
                      .arg(fullnamePage);
         
       }

       SYD << tr(".......InflowFirmaDoc...newfile:...WRITING:|%1|").arg(_newhtmlfile);

	QTextStream out(&outfile);
       out.setCodec("ISO 8859-1");
	out << html;

	outfile.close();

	
}
