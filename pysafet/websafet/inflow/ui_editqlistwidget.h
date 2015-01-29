/********************************************************************************
** Form generated from reading ui file 'editqlistwidget.ui'
**
** Created: Thu Aug 13 10:22:58 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EDITQLISTWIDGET_H
#define UI_EDITQLISTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *addButton;
    QToolButton *delButton;
    QToolButton *editButton;
    QListWidget *list;

    void setupUi(QWidget *Form)
    {
    if (Form->objectName().isEmpty())
        Form->setObjectName(QString::fromUtf8("Form"));
    Form->resize(318, 214);
    gridLayout_2 = new QGridLayout(Form);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    addButton = new QToolButton(Form);
    addButton->setObjectName(QString::fromUtf8("addButton"));
    addButton->setStyleSheet(QString::fromUtf8("QToolButton { \n"
"     font: bold 10px;\n"
"	 color: green;\n"
"     border: 1px solid #8f8f91;\n"
"     border-radius: 4px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }"));

    horizontalLayout->addWidget(addButton);

    delButton = new QToolButton(Form);
    delButton->setObjectName(QString::fromUtf8("delButton"));
    delButton->setStyleSheet(QString::fromUtf8("QToolButton { \n"
"     font: bold 10px;\n"
"	 color: red;\n"
"     border: 1px solid #8f8f91;\n"
"     border-radius: 4px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }"));

    horizontalLayout->addWidget(delButton);

    editButton = new QToolButton(Form);
    editButton->setObjectName(QString::fromUtf8("editButton"));
    editButton->setStyleSheet(QString::fromUtf8("QToolButton { \n"
"     font: bold 10px;\n"
"	 color: green;\n"
"     border: 1px solid #8f8f91;\n"
"     border-radius: 4px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }"));

    horizontalLayout->addWidget(editButton);


    gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

    list = new QListWidget(Form);
    list->setObjectName(QString::fromUtf8("list"));

    gridLayout->addWidget(list, 0, 0, 1, 1);


    gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


    retranslateUi(Form);

    QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
    Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
    addButton->setText(QApplication::translate("Form", "+", 0, QApplication::UnicodeUTF8));
    delButton->setText(QApplication::translate("Form", "-", 0, QApplication::UnicodeUTF8));
    editButton->setText(QApplication::translate("Form", "\341\270\231", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITQLISTWIDGET_H
