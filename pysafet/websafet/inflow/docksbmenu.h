#ifndef DOCKSBMENU_H
#define DOCKSBMENU_H

#include <QDockWidget>
#include <QWebView>
#include <QStackedWidget>
#include <QScrollArea>
#include <QComboBox>
#include <QWebFrame>
#include <QVBoxLayout>
#include "TreeWidget.h"
#include "textedit.h"
//#include "DigiDoc.h"
#include "SafetDocument.h"
//#include "iisTaskPanel"

class BlueTitleBar : public QWidget
{
    Q_OBJECT
public:
    BlueTitleBar(QWidget *parent = 0);

    QSize sizeHint() const { return minimumSizeHint(); }
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

public slots:
    void updateMask();

private:
    QPixmap leftPm, centerPm, rightPm;
};


class DockSbMenu : public QDockWidget
{
    Q_OBJECT
    QStackedWidget *menusbWidget;
    //iisTaskPanel *panelsbMenu;
    QWebView *panelsbMenu,*panelsbMessages; // Paneles para mostrar resultados
    TreeWidget *signContentView; // Para mostrar una lista de archivos  (p.e. digidoc)
    QScrollArea *myarea;
    QWidget *scrollAreaWidgetContents;// Widget para cuando se muestran resultados
    QFrame *resultFrame;
    QFrame *myconf;
    QString stylesbmenuNormal, stylesbmenuResults, stylebackground; // Estilos del menu
    BlueTitleBar *titlebar;
    QString currentTitle;
    QString headhtml; //  Cadena que incluya una mensaje de cabecera en formato HTML
    QTimer showtimer;

 //   iisTaskBox *tbPrincipal;
//    QStack<iisIconLabel*> labellist;
     QVBoxLayout *viewSignaturesLayout;

protected:
    void init();
    virtual void resizeEvent ( QResizeEvent * event );
    virtual void changeEvent ( QEvent * event );
    QList<int> eventslog;
public:

    enum MaskType { Normal, Results };
    enum ShowAction { Default, Show, Hide };

    QString cssStyle() const { return stylesbmenuNormal; }
    void  setCssStyle(const QString& s) { stylesbmenuNormal = s; }
    void cleanWidgets();


    DockSbMenu ( const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    DockSbMenu ( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    void setPanelGeometry(const QRect& r);
    QComboBox *comboOperacion;
    QRect panelGeometry() const;
    void changeMask(DockSbMenu::MaskType t);
    /**
      \brief Agregar un mensaje de cabecera para mostrar en la ventana
      \param h Mensaje en formato HTML
      */
    void addHeadHtml(const QString& h) { headhtml = h;}
    QWebView * getPanelsbMessages() { return panelsbMessages;}

    //void addOptionsDigidoc(DigiDoc* digiDocument,QStringList& list);
    // funcion agregada para eliminar dependencia de digidocpp
    void addOptionsDigidoc(SafetDocument doc,QStringList& list);
    void addOption(const QString &title, const QString &tooltip = QString("") , const QIcon& icon = QIcon());
    void addOptions(const QStringList& list);
    void addOptions(TextEdit* edit, bool afterdisabled);
    void addGroups(const QStringList& list,TextEdit* edit);
    void renderMessages(bool showPanelsbMenu = true);
    void clear();
    QString evalJS(const QString &js) const;
    QWebView* panel() { return panelsbMenu; }

private:
    virtual void enterEvent ( QEvent *event);
    virtual void leaveEvent ( QEvent * event );


};


#endif // DOCKSBMENU_H
