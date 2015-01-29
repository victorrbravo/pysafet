#include "docksbmenu.h"
#include "mainwindow.h"
#include "SignatureDialog.h"


DockSbMenu::DockSbMenu ( const QString & title, QWidget * parent, Qt::WindowFlags flags)
: QDockWidget(title,parent,flags) {
    //tbPrincipal = NULL;
    comboOperacion  = NULL;
    titlebar = NULL;
    currentTitle = title;
    viewSignaturesLayout = NULL;
    scrollAreaWidgetContents = NULL;
    resultFrame = NULL;
    panelsbMessages = NULL;
    signContentView = NULL;
    myarea = NULL;
    panelsbMenu = NULL;
    menusbWidget = NULL;
    init();
}

DockSbMenu::DockSbMenu ( QWidget* parent , Qt::WindowFlags flags )
:QDockWidget(parent,flags) {
    //tbPrincipal = NULL;
    comboOperacion  = NULL;
    scrollAreaWidgetContents = NULL;
    resultFrame = NULL;
    panelsbMessages = NULL;
    currentTitle = "";
    signContentView = NULL;
    init();
    myarea = NULL;
    panelsbMenu = NULL;
    menusbWidget = NULL;


}

void DockSbMenu::changeEvent ( QEvent * event ) {
    //qDebug("...QDockWidget::changeEvent(event)...%p",event);
    QDockWidget::changeEvent(event);
    if ( event == NULL ) {
        return;
    }
    if (eventslog.count() > 50
        &&  eventslog.at(eventslog.count()-1) != 88
        &&  eventslog.at(eventslog.count()-1) != 99 ) {
        eventslog.clear();
    }
    else {
        eventslog.append(event->type());
        if ( eventslog.count() > 2 ) {
            if ( (eventslog.at(eventslog.count()-2) == 88
                && eventslog.at(eventslog.count()-1) == 99) ||
                 (eventslog.at(eventslog.count()-2) == 99
                                 && eventslog.at(eventslog.count()-1) == 88)) {
                QStackedWidget *mystackwidget = NULL;
                if (MainWindow::mymainwindow ){

                    mystackwidget = MainWindow::mymainwindow->getStackedWidget();
                    if ( mystackwidget) {
                        int centralw = mystackwidget->geometry().width();
                        int centralh = mystackwidget->geometry().height();
                        //qDebug("resiseando...(1)...*");
                        int  newwidth = centralw/3+20;
                        int newheight = centralh/3+23;

                        QRect panelRect = geometry();



                        QPoint p;

                        p.setX( mystackwidget->geometry().bottomRight().x()
                                -newwidth);
                        panelRect.setSize(QSize(newwidth,newheight));

                        p.setY( mystackwidget->geometry().bottomRight().y()
                                -newheight);



                        panelRect.moveTopLeft(p);
                        setGeometry(panelRect);
                    }

                }

                eventslog.clear();
            }

        }
    }
    //qDebug("...QDockWidget::changeEvent(event)...type:%d",event->type());
    //qDebug("...QDockWidget::changeEvent(event)...windowstate:%d",windowState());

}

void DockSbMenu::resizeEvent ( QResizeEvent * event ) {

    QDockWidget::resizeEvent(event);
    if ( event == NULL ) {
        return;
    }
    if ( menusbWidget != NULL ) {

       QRect myrect = geometry();
       myarea->setGeometry(0,0,myrect.width()-14,myrect.height()-10);
       panelsbMenu->setGeometry(0,0,myrect.width()-20,myrect.height()-10);
    }
}

void DockSbMenu::init() {
    stylesbmenuNormal = QString::fromUtf8(
            "<style type=\"text/css\">\n"
            "body { background-color: #eff3f2 }\n"
            "A:link, A:visited {  color: #767676; text-decoration: none; font-weight:normal }\n"
            //"A:hover { color: red; font-weight:normal }\n"
            "A:hover {color: #112e64 }\n"
            "table {\n"
                    "border-width: thin thin thin thin;\n"
                    "border-spacing: 2px;\n"
                    "border-style: solid solid solid solid;\n"
                    "border-color: black black black black;\n"
                    "border-collapse: collapse;\n"
                    "background-color: rgb(255, 250, 250);\n"
            "}\n"
            "table th {\n"
                    "border-width: thin thin thin thin;\n"
                    "padding: 1px 1px 1px 1px;\n"
                    "border-style: dashed dashed dashed dashed;\n"
                    "border-color: gray gray gray gray;\n"
                    "background-color: ;\n"
                    "-moz-border-radius: 3px 3px 3px 3px;\n"
            "}\n"
            "table td {\n"
                    "border-width: thin thin thin thin;\n"
                    "padding: 1px 1px 1px 1px;\n"
                    "border-style: dashed dashed dashed dashed;\n"
                    "border-color: gray gray gray gray;\n"
                    "background-color: ;\n"
                    "-moz-border-radius: 3px 3px 3px 3px;\n"
            "}            \n"

            "</style> \n"
            );
    stylesbmenuResults = QString::fromUtf8(
            "<style type=\"text/css\">\n"
            "body { background-color: #eff3f2 }"
            "A:link, A:visited {  color: #00355F; text-decoration: none; font-weight:normal }\n"
            "A:hover { color: #8296a6; text-decoration: underline; font-weight:normal }\n"
            "table {\n"
                    "border-width: thin thin thin thin;\n"
                    "border-spacing: 2px;\n"
                    "border-style: solid solid solid solid;\n"
                    "border-color: black black black black;\n"
                    "border-collapse: collapse;\n"
                    "background-color: rgb(255, 250, 250);\n"
            "}\n"
            "table th {\n"
                    "border-width: thin thin thin thin;\n"
                    "padding: 1px 1px 1px 1px;\n"
                    "border-style: dashed dashed dashed dashed;\n"
                    "border-color: gray gray gray gray;\n"
                    "background-color: ;\n"
                    "-moz-border-radius: 3px 3px 3px 3px;\n"
            "}\n"
            "table td {\n"
                    "border-width: thin thin thin thin;\n"
                    "padding: 1px 1px 1px 1px;\n"
                    "border-style: dashed dashed dashed dashed;\n"
                    "border-color: gray gray gray gray;\n"
                    "background-color: ;\n"
                    "-moz-border-radius: 3px 3px 3px 3px;\n"
            "}            \n"
            "</style> \n"
            );

    stylebackground = QString::fromUtf8(
            "QWebView {"
            "background-image: url(:/fondoazul-small.png); } \n"
            );

    if ( currentTitle == tr("Acciones") ) {  // Panel para Acciones
        menusbWidget = new QStackedWidget(this);
        myarea  = new QScrollArea(this);
        panelsbMenu = new QWebView(this);
        setWidget(menusbWidget);
        menusbWidget->addWidget(myarea);
        panelsbMenu->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

        myarea->setWidget(panelsbMenu);
        connect(panelsbMenu,SIGNAL(linkClicked(QUrl)),MainWindow::mymainwindow,
                SLOT(linkClickedSbMenu(QUrl)));

        //setFloating(true);

    }   else if (currentTitle == tr("Resultado")) {  // Panel para Resultado
        myarea  = new QScrollArea(this);

        setWidget(myarea);
        scrollAreaWidgetContents = new QWidget();
        resultFrame = new QFrame(scrollAreaWidgetContents);
        resultFrame->setFrameShape(QFrame::StyledPanel);
        resultFrame->setFrameShadow(QFrame::Raised);

        QVBoxLayout *horizontalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        panelsbMenu = new QWebView(resultFrame);        
        panelsbMenu->setMaximumHeight(60);
        panelsbMenu->setMinimumHeight(40);
        panelsbMessages = new QWebView(resultFrame);
        panelsbMessages->setMinimumHeight(58);
        panelsbMessages->setMaximumHeight(75);

        panelsbMenu->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        if ( viewSignaturesLayout) {
            delete viewSignaturesLayout;
        }
        viewSignaturesLayout = new QVBoxLayout(resultFrame);
        viewSignaturesLayout->insertWidget(0,panelsbMenu);
        viewSignaturesLayout->insertWidget(0,panelsbMessages);
        horizontalLayout->addWidget(resultFrame);

        myarea->setWidget(scrollAreaWidgetContents);

        //connect(panelsbMessages,SIGNAL(linkClicked(QUrl)),MainWindow::mymainwindow,
        //        SLOT(linkClickedSbResult(QUrl)));

        // la siguiente conexion se encarga de establecer la ruta donde se encuentra el contenedor
        // para abrir el navegador de archivos y construir un correo electronico con el .ddoc
        connect(panelsbMenu,SIGNAL(linkClicked(QUrl)),MainWindow::mymainwindow,
                SLOT(linkClickedSbResult(QUrl)));

    }


    //myarea->setStyleSheet(stylebackground);

}


QString DockSbMenu::evalJS(const QString &js) const {
    Q_CHECK_PTR( panelsbMenu );
    QWebFrame *frame = panelsbMenu->page()->mainFrame();
    return frame->evaluateJavaScript(js).toString();
}




void DockSbMenu::addGroups(const QStringList& list,TextEdit* edit) {
// ***** Nuevo
    if ( panelsbMenu == NULL ) return;
    panelsbMenu->setHtml("");


}

void DockSbMenu::enterEvent(QEvent *event) {

    if (panelsbMessages != NULL ) {
        MainWindow::_panelinfotimer.stop();
    }


}

void DockSbMenu::leaveEvent ( QEvent *event) {

    if (panelsbMessages != NULL ) {
        MainWindow::_panelinfotimer.start(MainWindow::_seconstohide);
    }
}


void DockSbMenu::renderMessages(bool showPanelsbMenu) {
    cleanWidgets();
    if ( !headhtml.isEmpty()) {
        if ( !showPanelsbMenu ) {
            panelsbMessages->hide();
            panelsbMenu->setMaximumHeight(QWIDGETSIZE_MAX);
        }
        else {
            panelsbMessages->show();
            panelsbMenu->setMaximumHeight(75);
        }
        panelsbMenu->setHtml(headhtml);
    }
}


void DockSbMenu::addOptionsDigidoc(SafetDocument doc, QStringList& list) {
    qDebug("... DockSbMenu::addOptionsDigidoc(SafetDocument doc, QStringList& list) ... 1");
    QString htmlmenu;

    if ( currentTitle == tr("Resultado")) {
        htmlmenu = stylesbmenuResults + "\n";
        QRect rect = geometry();
        rect.setWidth(rect.width()-15);
        int h = rect.height();
        if ( h < 378 ) {
            h = 378;
            //if ( doc ) {
                //int countsign = digiDocument->signatures().count();
                int countsign =  doc.numberOfSignaturesOnOpenXAdESContainer();
                h = (countsign+1)*190;
            //}

        }
        rect.setHeight(h);
        scrollAreaWidgetContents->setGeometry(rect);
    }
    else {
        htmlmenu = stylesbmenuNormal + "\n";
    }
    if ( !headhtml.isEmpty()) {
        panelsbMessages->setHtml(headhtml);
        panelsbMessages->show();
    }

    Q_FOREACH( SafetSignatureWidget *w, findChildren<SafetSignatureWidget*>() )
                            w->deleteLater();
    Q_FOREACH( QLabel *w, findChildren<QLabel*>() )
                            w->deleteLater();

    htmlmenu += ""; // Agregar información antes de generar el menu

    //if ( digiDocument && !digiDocument->fileName().isEmpty()) {
    if ( !doc.getDataFileName().isEmpty() ) {
        // ** Colocando la lista de archivos digidoc

        if  (signContentView) {
            delete signContentView;
        }
        signContentView = new TreeWidget(scrollAreaWidgetContents);
        signContentView->setMaximumHeight(100);
        //signContentView->setColumnHidden( 3, !digiDocument->signatures().isEmpty() );
        bool hide = true;
        if (doc.numberOfSignaturesOnOpenXAdESContainer() < 1)
            hide = false;
        signContentView->setColumnHidden( 3, true );

        signContentView->setFrameShape(QFrame::NoFrame);
        signContentView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        signContentView->setDragEnabled(true);
        signContentView->setAlternatingRowColors(true);
        signContentView->setRootIsDecorated(false);
        signContentView->setHeaderHidden(true);
        signContentView->setColumnCount(4);

        //signContentView->setContent( digiDocument->documents() );
        signContentView->setContent( doc );

        // ** Colocando la lista de archivos digidoc
        QLabel *filesLabel = new QLabel(scrollAreaWidgetContents);
        filesLabel->setMaximumHeight(25);
        //filesLabel->setText(trUtf8("Número de archivos que contiene el sobre de firma(<b>%1</b>)").arg(digiDocument->documents().count()));
        filesLabel->setText(trUtf8("Número de archivos que contiene el sobre de firma(<b>%1</b>)").arg(doc.numberOfDataFileOnOpenXAdESContainer()));
        //signatureLabel->setAutoFillBackground(true);
        filesLabel->setAlignment(Qt::AlignHCenter);
        filesLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(222, 222, 223);\n"
                                                    "font: 10pt \"DejaVu Sans\";\n"
                                                    "color: rgb(0, 0, 0);"));

        QLabel *signatureLabel = new QLabel(scrollAreaWidgetContents);
        signatureLabel->setMaximumHeight(25);
        //signatureLabel->setText(trUtf8("Número de firmas que contiene el sobre de firma (<b>%1</b>)").arg(digiDocument->signatures().count()));
        signatureLabel->setText(trUtf8("Número de firmas que contiene el sobre de firma (<b>%1</b>)").arg(doc.numberOfSignaturesOnOpenXAdESContainer()));
        //signatureLabel->setAutoFillBackground(true);
        signatureLabel->setAlignment(Qt::AlignHCenter);
        signatureLabel->setStyleSheet(QString::fromUtf8("background-color: rgb(222, 222, 223);\n"
                                                        "font: 10pt \"DejaVu Sans\";\n"
                                                        "color: rgb(0, 0, 0);"));

        viewSignaturesLayout->addWidget( filesLabel );
        viewSignaturesLayout->addWidget(signContentView);
        viewSignaturesLayout->addWidget( signatureLabel );
        htmlmenu+= tr("<p align=center><b>[</b> ");


        //********************* c para verificación OCSP

        //SafetDocument doc;
        doc.initializeLibdigidoc();


        QString libdigidocConfigFile = SafetYAWL::getConfFile().getValue("Libdigidoc", "libdigidoc.configfilepath").toString();

        libdigidocConfigFile = QDir::homePath() + "/" + Safet::datadir + "/" + "digidoc.conf";

                               +
        doc.initDigidocConfigStore(libdigidocConfigFile);
/*
        int resultOCSP = doc.readDigiDocFile(digiDocument->fileName());
        if ( !resultOCSP) {
            SafetYAWL::streamlog << SafetLog::Error << tr("No fue posible cargar "
                                                          "el archivo digidoc \"%1\"")
                                                          .arg(digiDocument->fileName());
            return;
        }
*/

        //********************* Abriendo para verificación OCSP
        QList<bool> ocspsigns = doc.verifyOCSPSign();
        bool curocspvalid;

        qDebug(".****.........ocspsigns.count():%d", ocspsigns.count());
        foreach(QString s, list ) {
            //qDebug("valor de s: ", qPrintable(s));
            if ( s.startsWith(tr("Ver firmas del documento")) && viewSignaturesLayout ) {
                //if ( digiDocument == NULL ) continue;
                //QList<DigiDocSignature> signatures = digiDocument->signatures();
                int i = 0;

                //if ( ocspsigns.count() != signatures.count()) {
                if ( ocspsigns.count() != doc.numberOfSignaturesOnOpenXAdESContainer()) {
                     SafetYAWL::streamlog << SafetLog::Error
                             << trUtf8("Existe una incoherencia entre las firmas de verificación local CRL (%1) "
                                   "y las firmas de verificación OCSP (%2) (remota)").arg(doc.numberOfSignaturesOnOpenXAdESContainer())
                                   .arg(ocspsigns.count());
                }
                for ( int j = 0; j < doc.numberOfSignaturesOnOpenXAdESContainer(); j++){
                    if ( i >= ocspsigns.count()) {
                        curocspvalid = false;
                    }
                    else {
                        curocspvalid = ocspsigns.at(i);
                        qDebug("****..........curocspvalid valid:%d", curocspvalid);
                    }
                    qDebug("... antes de new SafetSignatureWidget");
                    SafetSignatureWidget *signature = new SafetSignatureWidget( doc, i, true, scrollAreaWidgetContents,curocspvalid);

                    viewSignaturesLayout->addWidget( signature );
                    ++i;
                }

                /*
                Q_FOREACH( const DigiDocSignature &c, signatures )
                {

                    if ( i >= ocspsigns.count()) {
                        curocspvalid = false;
                    }
                    else {
                        curocspvalid = ocspsigns.at(i);
                        qDebug("****..........curocspvalid valid:%d", curocspvalid);
                    }
                    SignatureWidget *signature = new SignatureWidget( c, i, true, scrollAreaWidgetContents,curocspvalid);

                    viewSignaturesLayout->addWidget( signature );
                    ++i;
                }
                */

            }
            else {
                htmlmenu+=QString("<a href=\"operacion:::%1\" title=\"%1\">%1</a>&nbsp;&nbsp;&nbsp;").arg(s);;
            }
        }
        htmlmenu+=tr("<b>]</b></p>");

        //htmlmenu+=tr("<p>  *** NO HUBO ERRORES  *** </p>");
        //********************* Cerrando para verificación OCSP

        doc.closeLibdigidoc();
        //********************* Cerrando para verificación OCSP

    }
    panelsbMenu->setHtml(htmlmenu);
    //qDebug("----------");
    //qDebug(qPrintable(htmlmenu));
    //qDebug("----------");
    qDebug("... DockSbMenu::addOptionsDigidoc(SafetDocument doc, QStringList& list) ... 2");

}

void DockSbMenu::addOptions(const QStringList& list) {
// ***** Nuevo

    if ( panelsbMenu == NULL ) return;

    QString htmlmenu = stylesbmenuNormal + "\n";
    for( int i=0; i < list.count(); i++) {
         QStringList infolist = list.at(i).split(SafetYAWL::LISTSEPARATORCHARACTER);

         QString templateDirImage = QString("file://%1");
         QString imagespath = QDir::homePath()+ "/" + Safet::datadir+"/"+Safet::imagesdir;
         QString defaultImage =templateDirImage.arg(imagespath + "/" + "default.png");
//         SafetYAWL::streamlog
//                 << SafetLog::Debug
//                 << trUtf8("Se va imagen por defecto para el menú: \"%1\"")
//                 .arg(defaultImage);


         int countinfos = infolist.count();

         if ( countinfos < 1) {
             SafetYAWL::streamlog
                     << SafetLog::Error
                     << tr("Un \"campo\" no esta correctamente escrito: \"%1\"").arg(list.at(i));
             return;
         }

        QString curstyle,desc,fileicon;
        QString s = infolist.at(0); // Titulo

        if ( countinfos > 1 ) {
            desc = infolist.at(1);
        }

        if ( countinfos > 2 ) {
            fileicon = infolist.at(2);
            if ( !fileicon.isEmpty()) {
                defaultImage = templateDirImage.arg(imagespath + "/" + fileicon);
            }
        }

        if ( infolist.count() > 3 && infolist.at(3)== "#ISTITLE#") {
            curstyle = "td.curtitle"
                               "{ background-color: #99CCCC;"
                               "border: 1px #000000 solid;"
                               "font-family: Verdana;"
                               "font-weight: bold;"
                               "text-align: center;"
                               "font-size: 12px;"
                               "color: #404040; }";
        }

        if ( !curstyle.isEmpty()) {
             htmlmenu  = htmlmenu+QString("<span title=\"%2\"><table width=\"100%\" style=\""+ curstyle
                                                  + "\" border=\"0\"><tr><td align=\"center\" class=\"curtitle\">"
                                                  "<img align=\"center\" border=\"0\" src=\""
                                                  + defaultImage +"\" />%1</td></tr></table></span>").
                                                  arg(s).arg(infolist.at(1));
        }
        else {

            if ( countinfos == 1 ) {
                htmlmenu  = htmlmenu+QString("<a href=\"operacion:::%1\" "
                                             "title=\"%1\"><img align=\"center\" border=\"0\" src=\""
                                             + defaultImage +"\"/>     %1</a><br/>").arg(s);
            }
            else {
                htmlmenu  = htmlmenu+QString(" <a href=\"operacion:::%1\" title=\"%2\"><img align=\"center\" border=\"0\" "
                                         "src=\""+ defaultImage +"\"/> %1</a><br/>")
                                    .arg(s).arg(infolist.at(1));
            }


        }
    }
    panelsbMenu->setHtml(htmlmenu);


}


void DockSbMenu::addOptions(TextEdit* edit,  bool afterdisabled) {

    if ( panelsbMenu == NULL ) return;

    QStringList list = MainWindow::sbmenulist;
    QString htmlmenu = stylesbmenuNormal+ "";
    for( int i=0; i < list.count(); i++) {
         QStringList infolist = list.at(i).split(":::",QString::SkipEmptyParts);
         Q_ASSERT ( infolist.count() > 0 );
         QString s = infolist.at(0);
         if ( infolist.count() > 1  ) {
             qDebug("(2)...opcion...DockSbMenu::addOptions:...",qPrintable(s));
             htmlmenu  = htmlmenu+QString("<a href=\"operacion:::%1\" title=\"%2\">%1</a><br/>").arg(s).arg(infolist.at(1));
         } else {
             qDebug("(1)...opcion...DockSbMenu::addOptions:...",qPrintable(s));
            htmlmenu  = htmlmenu+QString("<a href=\"operacion:::%1\" title=\"%1\">%1</a><br/>").arg(s);
         }
    }

    panelsbMenu->setHtml(htmlmenu);


}

void DockSbMenu::cleanWidgets() {
    qDebug("......DockSbMenu::cleanWidgets()...........");
    Q_FOREACH( SafetSignatureWidget *w, findChildren<SafetSignatureWidget*>() )
                            w->deleteLater();
    Q_FOREACH( QLabel *w, findChildren<QLabel*>() )
                            w->deleteLater();

    if (signContentView != NULL ) {
        delete signContentView;
        signContentView = NULL;
    }
}


QRect DockSbMenu::panelGeometry() const {
    if (panelsbMenu == NULL) return QRect();
    return panelsbMenu->geometry();

}
void DockSbMenu::setPanelGeometry(const QRect& r) {
    if (panelsbMenu == NULL) return;
    panelsbMenu->setGeometry(r);

}

void DockSbMenu::changeMask(DockSbMenu::MaskType t) {
    if ( t == DockSbMenu::Results ) {
            titlebar = new BlueTitleBar(this);
            setTitleBarWidget(titlebar);
            connect(this, SIGNAL(topLevelChanged(bool)), titlebar, SLOT(updateMask()));
//            connect(this, SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)), titlebar, SLOT(updateMask()));

    }
    else if (t == DockSbMenu::Normal ) {

          setTitleBarWidget(NULL);
          if ( titlebar) delete titlebar;
          titlebar = NULL;
    }
}

QSize BlueTitleBar::minimumSizeHint() const
{
    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != 0);
    QSize result(leftPm.width() + rightPm.width(), centerPm.height());
    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar)
        result.transpose();
    return result;
}

BlueTitleBar::BlueTitleBar(QWidget *parent)
    : QWidget(parent)
{
    leftPm = QPixmap(":/titlebarLeft.png");
    centerPm = QPixmap(":/titlebarCenter.png");
    rightPm = QPixmap(":/titlebarRight.png");
}

void BlueTitleBar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QRect rect = this->rect();

    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != 0);

    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
        QSize s = rect.size();
        s.transpose();
        rect.setSize(s);

        painter.translate(rect.left(), rect.top() + rect.width());
        painter.rotate(-90);
        painter.translate(-rect.left(), -rect.top());
    }

    painter.drawPixmap(rect.topLeft(), leftPm);
    painter.drawPixmap(rect.topRight() - QPoint(rightPm.width() - 1, 0), rightPm);
    QBrush brush(centerPm);
    painter.fillRect(rect.left() + leftPm.width(), rect.top(),
                        rect.width() - leftPm.width() - rightPm.width(),
                        centerPm.height(), centerPm);
}

void BlueTitleBar::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    QRect rect = this->rect();

    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != 0);

    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
        QPoint p = pos;
        pos.setX(rect.left() + rect.bottom() - p.y());
        pos.setY(rect.top() + p.x() - rect.left());

        QSize s = rect.size();
        s.transpose();
        rect.setSize(s);
    }

    const int buttonRight = 7;
    const int buttonWidth = 20;
    int right = rect.right() - pos.x();
    int button = (right - buttonRight)/buttonWidth;
    switch (button) {
        case 0:
            event->accept();
            dw->close();
            break;
        case 1:
            event->accept();
            //dw->setFloating(!dw->isFloating());
            break;
        case 2: {
            event->accept();
            QDockWidget::DockWidgetFeatures features = dw->features();
            if (features & QDockWidget::DockWidgetVerticalTitleBar)
                features &= ~QDockWidget::DockWidgetVerticalTitleBar;
            else
                features |= QDockWidget::DockWidgetVerticalTitleBar;
            dw->setFeatures(features);
            break;
        }
        default:
            event->ignore();
            break;
    }
}

void BlueTitleBar::updateMask()
{
    QDockWidget *dw = qobject_cast<QDockWidget*>(parent());
    Q_ASSERT(dw != 0);

    QRect rect = dw->rect();
    QPixmap bitmap(dw->size());

    {
        QPainter painter(&bitmap);

        ///initialize to transparent
        painter.fillRect(rect, Qt::color0);

        QRect contents = rect;
        contents.setTopLeft(geometry().bottomLeft());
        contents.setRight(geometry().right());
        contents.setBottom(contents.bottom()-y());
        painter.fillRect(contents, Qt::color1);



        //let's pait the titlebar

        QRect titleRect = this->geometry();

        if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
            QSize s = rect.size();
            s.transpose();
            rect.setSize(s);

            QSize s2 = size();
            s2.transpose();
            titleRect.setSize(s2);

            painter.translate(rect.left(), rect.top() + rect.width());
            painter.rotate(-90);
            painter.translate(-rect.left(), -rect.top());
        }

        contents.setTopLeft(titleRect.bottomLeft());
        contents.setRight(titleRect.right());
        contents.setBottom(rect.bottom()-y());

        QRect rect = titleRect;


        painter.drawPixmap(rect.topLeft(), leftPm.mask());
        painter.fillRect(rect.left() + leftPm.width(), rect.top(),
            rect.width() - leftPm.width() - rightPm.width(),
            centerPm.height(), Qt::color1);
        painter.drawPixmap(rect.topRight() - QPoint(rightPm.width() - 1, 0), rightPm.mask());

        painter.fillRect(contents, Qt::color1);
    }

    dw->setMask(bitmap);
}
