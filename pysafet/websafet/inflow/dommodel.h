/****************************************************************************
**
** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef DOMMODEL_H
#define DOMMODEL_H

//#include <QStringListModel>
#include <QDomDocument>
#include <QModelIndex>
#include <QVariant>
#include <QStringList>
#include <QSet>
//#include <QTextEdit>
class DomItem;
class CmdWidget;


namespace Dommodel {
    const QStringList STRINGLISTEMPTY;
}

class DomModel : public QObject
{
    Q_OBJECT
//  QTextEdit *mytextedit;
  QString _stringtoparse;
  QMap<QString,QString> _currentactions;

public:
  enum TypeField { String, StringBox, DateTime, Number, FileName, ComboVar,
                   ComboAutofilter, ComboListTable, ComboListLiteral, ComboFlow, ComboRecursivefilter,
                   Suggest, ComboConffile, ListConffile, ComboColor,
                   ListTable,ListLiteral, ComboGraphs,ComboTask, ComboListBinaryRepo,Output,
                   Unknown };

    DomModel(const QString& fileName, QObject *parent = 0);
    ~DomModel();
    void setStringToParse(const QString& s) { _stringtoparse = s; }
    QString stringToParse() const { return _stringtoparse; }

    bool documentOk() { return docok; }
   // QVariant data(const QModelIndex &index, int role) const;
//    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QStringList getCommands(bool includedesc = false, bool ascending = true);
    QMap<QString,QString> currentActions() const { return _currentactions; }

    QStringList getFields(const QString& command, bool includedesc = false );
    QStringList  fieldsWithAttr(const QString& command, QString namekey = "changekey");
    TypeField typeField(const QString& command);
    static DomModel::TypeField  typeFieldFromString(const QString& command);

    CmdWidget* selWidgetFromField(const QString& command, const QString& field,
                                  QString keyvalue = QString(), const QStringList& form = Dommodel::STRINGLISTEMPTY);
    void setCommands(const QStringList& list) { commandslist = list; }
    QStringList commands() const { return commandslist; }
    void reset();
    void domModelFromFile(const QString& fileName);
    QString prefix() const { return _prefix; }
    QString suffix() const { return _suffix; }
    QString commandName() const { return _commandname; }
    QString searchPathFlow() const;
    QString searchAutofilterFlow() const;
    QString searchRecursivefilterFlow() const;

    /**
      \brief método interno para buscar clave en un buffer (texttosearch)
     @see searchVisibility
     */
    QString searchKeyValueOnTextEdit(const QString& keyname = QString("id"),
                                     const QString& texttosearch = QString("")) const;
    void readFile();
//    void setTextEdit(QTextEdit* te) { mytextedit = te;}
    /**
      * \brief Busca si un campo se muestra (visible) o no (no visible) según una expresion
      * \param node Nodo que se evaluará visibilidad
      * \param texttosearch Si es vacío se busca en el buffer de edición si no lo es se busca
      segun este texto
      * @see searchKeyValueOnTextEdit
      * \return true si el campo se muestra (visible), false en caso contrario
      */
    bool searchVisibility(const QDomNode& node, const QString& texttosearch = QString(""));
    /**
      \brief Data una nombre de campo clave (keyname) y su valor (keyvalue) busca en el repositorio
      de datos y obtiene una cadena con los campos y valores para posteriormente poder realizar una actualización
      (UPDATE)
      \param operationame Nombre de la operación para modificación
      \param keyname Nombre del campo clave a buscar (Se obtiene de cuadro de edición)
      \param keyvalue Valor del campo clave (Se obtiene de cuadro de edición)
      \param prevfields Lista de campos anteriores, estos campos no se incluyen en la cadena que se retorna
      \return Retorna la cadena para la actualización
      */
    QString getUpdateString(const QString& operationname, const QString& keyname, const QString& keyvalue, const QStringList& prevfields);
    /**
       \brief Realiza la operación en el repositorio de datos mediante consulta SQL para
        realizar  operaciones de Modificación
       \param sql Cadena Sql a ejecutar
       \param titles Cadena plantilla para generar cadena de retorno
       \param dtw Lista de nombres de campos que tienen formato fecha (aunque en la bd son enteros)
       \return retorna cadena con todos los títulos según plantilla
       */
    QString doUpdateSqlString(const QString& sql, const QString& titles, const QStringList& dtw);
    /**
        \brief Determina si un campo se puede repetir en la lista de entrada
        \param Campo verificar si es repetible (Colocar el nombre del campo completo con opción
               de Obligatario o no
        \return Retorna Verdadero si es repetible, de lo contrario retorna falso
        */
    bool isRepetibleField(const QString& f);

    /**
      * \brief Chequea si el campo "field" pertenece a un widget dinámico
      * \param field descriptor del widget dinámico
      * \return true si el descriptor corresponde a un widget dinámico, false de otra forma
      */

    bool isDynWidget(const QString& field);
    /**
      * \brief Utilitario convierte fecha de tipo time_t a fecha tipo D/M/Y
      * \param d fecha de tipo time_t
      * \return fecha de tipo DMY
      */
    static QString convertTimetToDMY(const QString &d);
    /**
      \brief Dado una cadena del tipo <campo_real>:<tabla>::<Nombre_presentado> en un combolisttable
             se extrae el nombre presentado
             \param options Generalmente campo "options" de un combolisttable
             \return Nombre que se presenta al usuario
      */
    QString extractNameField(const QString &options, const QString& table);
private:
    CmdWidget* checkDynWidget(const QString& s, const QString& field ="", QWidget *parent = 0);
    QString _prefix;
    QString _suffix;
    QString _commandname;
    QObject* _cpparent;
 //   QRegExpValidator* checkValidation(const QDomNamedNodeMap& attributeMap);
    QDomDocument *domDocument;
    QStringList commandslist;
    QSet<QString> repetiblesFields;
    bool docok;
};

#endif
