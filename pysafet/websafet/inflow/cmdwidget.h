/*
* SAFET Sistema Automatizado para la Firma Electrónica y Estampado de Tiempo
* Copyright (C) 2008 VÃ­ctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigación en TecnologÃ­as Libres
*
* Este programa es software libre; Usted puede usarlo bajo los términos de la licencia de
* software GPL versión 2.0 de la Free Software Foundation.
*
* Este programa se distribuye con la esperanza de que sea útil, pero SI NINGUNA GARANTÍA;
* tampoco las implí­citas garantí­as de MERCANTILIDAD o ADECUACIÃ“N A UN PROPÓSITO PARTICULAR.
* Consulte la licencia GPL para más detalles. Usted debe recibir una copia de la GPL junto
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5º Piso, Boston, MA 02110-1301, USA.
*
*/

#ifndef CMDWIDGET_H
#define CMDWIDGET_H

//#include <QtGui>
//#include <QWidget>
//#include <QRegExpValidator>
//#include <QTextEdit>
#include <QObject>
#include <QStringList>
#include <QMap>
#include <QVariant>

/**
        @author Víctor R. Bravo,,, <vbravo@cenditel.gob.ve>
*/
class CmdWidget : public QObject
{
Q_OBJECT
protected:

    QString _caption;

    bool _ispassword;
    QString _typeinput;
    int texteditpos;
    QString _prevvalue;
//    QTextCursor _prevcursor;

    QString _input;
    bool _istextparent;
    QString _defaultvalue;

    QStringList _changefor; // Campos para qlos cambia los datos del widget

protected:
    QStringList _options;
    QString _usertooltip;
//    QTextEdit *_texteditparent;
    QMap<QString,QVariant> _conf;
//    QRegExpValidator *_validator;
     void *_validator;
 public:
     /*
       * \brief Creacion de un widget generico para ser leÃ­do en un formulario
       * \param wdefault Construir el widget original
       */



    CmdWidget(const QString& t, QObject *parent = 0, bool istextparent = true);
    //CmdWidget();
    void setCaption(const QString& c) { _caption = c;}
    QString caption() const { return _caption;}
    virtual void buildWidget();
    virtual void setFocus ( Qt::FocusReason);
    QString typeInput()  const;

    void setChangefor(const QString& c);
    QStringList changefor() { return _changefor;}

    void setDefaultValue(const QString& v) { _defaultvalue = v; }
    QString defaultValue() const { return _defaultvalue; }

    bool isTextParent() const { return _istextparent;}

    virtual QString userTooltip() const { return _usertooltip; }

    int  texeditPos() { return texteditpos;}
    ~CmdWidget();

    QString input() const { return _input;}
    void setInput(const QString& i) { _input = i;}
    void setPrevValue(const QString& v) { _prevvalue = v; }
    QString prevValue() { return _prevvalue;  }

    virtual bool isValid(QString& value);
    virtual QString text() const;


    virtual void setConf(QMap<QString, QVariant>& c) { _conf = c;}
    virtual QMap<QString, QVariant>& conf() {return _conf;}

    virtual QMap<QString, QVariant> conf() const {return _conf;}
//    virtual QRect getGeoParams() const;
    void setOptionsProperties(const QStringList ps);

    virtual QString html();
    virtual QString htmlForKey(const QString& k, const QString& otherk = "");
    virtual QStringList options() const;
public slots:
    virtual void setText(const QString &newText);
    virtual void insertAndClose();
    virtual void cancelAndClose();
    void doReturn() ;
    bool isPassword() const { return _ispassword;}
//    bool isLineedit() const { return lineedit != NULL; }
private slots:
    void changeLblValidator(const QString& text);	
protected:
// methods 
  	

// atributtes
//    QLineEdit *lineedit;
//    QToolButton *okbutton;
//    QToolButton *quitbutton;
//    QToolButton *checkbutton;
//    QHBoxLayout *mainLayout;
//    QLabel *label, *lblvalidator;
//    QWidget* principalWidget;
};

#endif
