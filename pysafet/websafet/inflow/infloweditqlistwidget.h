#ifndef INFLOWEDITQLISTWIDGET_H
#define INFLOWEDITQLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QToolButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPair>
#include "cmdwidget.h"
#include "ui_editqlistwidget.h"

class   InflowEditQListWidget : public CmdWidget
{
    Q_OBJECT

    QGridLayout *mainLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *addButton;
    QToolButton *delButton;
    QToolButton *editButton;
    QListWidget *list;
    QWidget *_parent;

public:
    InflowEditQListWidget(QWidget *parent = 0);
    ~InflowEditQListWidget();
    void buildWidget();
    void setEditList(const QStringList& l);
    QStringList editList(bool withpass = true) const;


private:
    QString  genPasswordMask(const QString& s) const;
private slots:
    void delItem();
    void addItem();
    void editItem();
    // Funciones de Chequeo de Butón
    void areItemsEnabled();


};

namespace Inflow {
class ListDialog : public QDialog {

        Q_OBJECT
        QGridLayout *gridLayout;
        QLabel *labelTypeInfo;
        QVBoxLayout *verticalLayout;
        QLabel *labelInfo;
        QLineEdit *editInfo;
        QCheckBox *checkInfo;
        QDialogButtonBox *buttonBox;
        QButtonGroup *groupButton;
        QList<QPair<QString,QString> > predefinedValues;
        public:
           ListDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0 );
           void buildDialog();
           void setInfo(const QString &title = QString("Agregar"), const QString &info = QString("Ingrese frase:"),
                        const QString&helpmessage = QString(""));
           QString editinfo() const { if (editInfo) return editInfo->text(); else QString(""); }
           void setEditinfo(QListWidgetItem* item);
           bool isPassword() const { if (checkInfo) return (checkInfo->checkState()==Qt::Checked)?true:false; }
           void setPredefinedValues(QList<QPair<QString,QString> >& values) {predefinedValues = values;}
        private:

        public slots:
            void predefineButtonClicked(int id );
        private slots:
           void setEditInfoType(bool state);
        protected:
           virtual void showEvent ( QShowEvent * event );


    };
}

#endif // INFLOWEDITQLISTWIDGET_H
