/*
* SAFET Sistema Automatizado para la Firma Electrnica y Estampado de Tiempo
* Copyright (C) 2008 Víctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigacin en Tecnologías Libres
*
* Este programa es software libre; Usted puede usarlo bajo los trminos de la licencia de
* software GPL versin 2.0 de la Free Software Foundation.
*
* Este programa se distribuye con la esperanza de que sea til, pero SI NINGUNA GARANTA;
* tampoco las implcitas garantas de MERCANTILIDAD o ADECUACIÓN A UN PROPSITO PARTICULAR.
* Consulte la licencia GPL para ms detalles. Usted debe recibir una copia de la GPL junto
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5 Piso, Boston, MA 02110-1301, USA.
*
*/

#ifndef COMBOVARWIDGET_H
#define COMBOVARWIDGET_H

#include "cmdwidget.h"
#include "SafetYAWL.h"

class ComboWidget : public CmdWidget
{
 Q_OBJECT

    bool _oneoption;
    QString _currentgraphfile;
    bool _filtercompleted;

public:
    enum Type { ListTable, VariableSafet, AutofilterSafet, ListLiteral, Flow, RecursivefilterSafet,
            ConffileSafet, ColorSafet, GraphsSafet, TaskSafet, ListBinaryRepo};
private:
    Type _type;
    QStringList _itemvaluelist,_itemrealvaluelist;
    QString hashtext;
    QString proccessWhereOption(const QString& w);
public:
     Type type() const { return _type; }
     void setType(ComboWidget::Type t) { _type = t; }
public:

    ComboWidget(const QString& s, ComboWidget::Type t = ComboWidget::VariableSafet,
                QObject *parent = 0, bool istextparent = true);
    ~ComboWidget();
    bool oneoption() { return _oneoption;}
    void setOneoption(bool b) { _oneoption = b; }

    virtual void setFocus ( Qt::FocusReason reason );
    virtual void buildWidget();
    void buildButtonWidget(const QStringList& l);
    virtual QRect getGeoParams() const;
    virtual bool isValid(QString& value);
    virtual QString html();
    virtual QString htmlForKey(const QString &k, const QString& otherk = "");


    void updateComboGraphsSafet(bool inwidget = true);
    void updateComboTaskSafet(bool inwidget = true);
    void updateComboListBinaryRepo(bool inwidget = true);

    // Funciones estaticas
    static QString replaceLiteral(const QString& r);
    // Utilizar
    /**
      \brief Ejecutar accion previa
      \param keys Claves a modificar
      \param op Consulta SQL
      \parma kv Clave a utilizar
      */
    static QPair<QStringList,bool> execBeforeFunction(const QStringList& keys, const QString& op,
                                          const QString& kv, const QString& otherkey);

public slots:

    virtual QString text() const;
    virtual void setText(const QString &newText);
    void updateCombo();
    void updateComboVariableSafet(bool inwidget = true);
    void updateComboAutofilterSafet(bool inwidget = true);
    void updateComboRecursivefilterSafet(bool inwidget = true);
    void updateComboListTable(bool inwidget = true);
    void updateComboListLiteral();
    void updateVarGlobal(const QString& value);
    void updateComboFlow(bool inwidget = true);
    void updateComboConffileSafet(bool inwidget = true);
    void updateComboColorSafet(bool inwidget = true);
    void viewdoc();
    void selColor();
    virtual void insertAndClose();
    QString getRealValue(const QString& s);
    QString getVarValue(const QString& s);
    QStringList& itemsValueList() { return _itemvaluelist;}
    QStringList& itemsRealValueList() { return _itemrealvaluelist;}
  //  QComboBox* combo() { return varbox; }
    QString findkeyvalue(const QString& k);
protected:
//    QComboBox* varbox;
//    QPushButton* signbutton;
//    QToolButton* viewdocbutton;
//    QToolButton* getcolorbutton;
    QStringList infos;
    QMap<QString,QString>  realvalues;
    QMap<QString,QString>  varvalues;
    QString _cursigners;
    void generateLiterals(const QString& s, bool inwidget = true);


};

#endif // COMBOVARWIDGET_H

