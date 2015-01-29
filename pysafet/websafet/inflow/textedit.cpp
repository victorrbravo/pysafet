/****************************************************************************
**
** Copyright (C) 2006-2006 Trolltech ASA. All rights reserved.
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
/*
* SAFET Sistema Automatizado para la Firma Electrónica y Estampado de Tiempo
* Copyright (C) 2008 Víctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
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
#include "textedit.h"
#include "qcmdcompleter.h"
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QToolButton>
#include <QFile>
#include "SafetYAWL.h"
#include "cmdwidget.h"
#include "mainwindow.h"
#include "texteditwidget.h"

TextEdit::TextEdit(QWidget *parent)
: QTextEdit(parent), c(0), l(0)
{
  //  setPlainText(tr("This TextEdit provides autocompletions for words that have more than"
  //                  " 3 characters. You can trigger autocompletion using ") + 
  //                   QKeySequence("Ctrl+E").toString(QKeySequence::NativeText));



     //QMap<QString,QVariant> myconf;

     //myconf["namewidget"] = "ComboSvnWidget";
     //myconf["path"] = "/home/vbravo/trac/seguridad/safet/libsafet/trunk/tools";
     //listConfs().append( myconf );

     setFocus();
    flaginsert = false;
    //setStyleSheet(QString::fromUtf8("background-color: rgb(223, 223, 223);"));
//    connect(this, SIGNAL(textChanged()), this, SLOT(doTextChange()) );

}

void TextEdit::doTextChange() {

}

TextEdit::~TextEdit()
{
     qDebug("...~TextEdit...");
}

void TextEdit::setCompleter(QCompleter *completer)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    connect(completer, SIGNAL(activated(const QString&)),
                     this, SLOT(insertCompletion(const QString&)));

}

void TextEdit::unsetCompleter(QCompleter *completer) {

    QObject::disconnect(completer, SIGNAL(activated(const QString&)),
                     this, SLOT(insertCompletion(const QString&)));

}

QCompleter *TextEdit::completer() const
{
    return c;
}

DomModel* TextEdit::dommodel() {
    if (completer() == NULL) {
        SafetYAWL::streamlog
                << SafetLog::Debug
                << tr("Objeto completer es NULO (NULL");
        return NULL;
    }

        DomModel* mymodel = qobject_cast<DomModel*>(completer()->model());
        return mymodel;
}

void TextEdit::insertCompletion(const QString& completion)
{

    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();

    QString s = completion.right(extra);
    if (s.endsWith("*") ) {  	
    	s.chop(1);	
    }
    DomModel* mymodel = dommodel();


    if ( !s.contains("operacion:") ) {
         s = mymodel->prefix()+ s + mymodel->suffix();

     }

    
    tc.insertText(s+" ");
    QRect cr = cursorRect();
//marcador
    setTextCursor(tc);
    showFromWidget(cr, s );

    updateDockSbMenu();


}

QString TextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::BlockUnderCursor);
    return tc.selectedText();
}

void TextEdit::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore(); 
            return; // let the completer do default behavior
       default:
           break;
       }
    }
    DomModel *dommodel = ((QCmdCompleter*) c)->domModel();
    Q_CHECK_PTR( dommodel );
    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if ( isShortcut ) {
//        qDebug("...isShortcut....(1)....");
    }
    bool isShortcutLine = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_L); // CTRL+L
    if ( isShortcutLine ) {
         QRect cr = cursorRect();
        showFromWidget( cr );
        return; 
    }
    if (!c || !isShortcut) {// dont process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);
        return;
   }

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                      || eow.contains(e->text().right(1)))) {
        c->popup()->hide();	
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);   
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());

    QStringList list;
    bool listcommand = searchPrvsToken(list);
    if ( !listcommand ) {
        dommodel->getCommands();

    }
    else {
        QStringList infolist = list.at( list.count()-1 ).split(SafetYAWL::LISTSEPARATORCHARACTER,QString::SkipEmptyParts);
        Q_ASSERT( infolist.count() > 0 );

        fields = dommodel->getFields(infolist.at(0) );
        for(int i = 0; i < list.count()-1; i++ ) {
            QString field = list.at(i).trimmed();
            QString pattern = QString("%1\\*?(\\:\\:\\:.+)?").arg(field);
            QRegExp rx(pattern);
            int pos = fields.indexOf(rx);
            if ( pos != -1 && !dommodel->isRepetibleField(field)) {
                fields.removeAt(pos);
            }
        }
        dommodel->setStringList( fields );
    }	


    c->complete(cr); // popup it up!



}

bool TextEdit::checkIsField(const QString& f) {
    QString newstr = f;

    if ( newstr.endsWith(":")) {
        newstr.chop(1);
    }
//    qDebug("newstr:%s", qPrintable(newstr));
    QStringList mylist = getCurrentFields();
    if (mylist.count() == 0) {
        return false;
    }
    foreach (QString s, mylist) {
//        qDebug("...s:%s",qPrintable(s));
        if (s.startsWith(newstr)) {
            return true;
        }
    }
    return false;

}


QStringList TextEdit::getCurrentFields() {
    QStringList result, list;

    bool listcommand = searchPrvsToken(list);
    if ( !listcommand ) {
        return result;
    }

    QStringList infolist = list.at( list.count()-1 ).split(SafetYAWL::LISTSEPARATORCHARACTER,
                                                           QString::SkipEmptyParts);
    if ( infolist.count() > 0 ) {
        result = dommodel()->getFields(infolist.at(0) );

    }

    return result;

}

void TextEdit::modifyCurrentField() {
    if ( l && l->isVisible()) {
        if( MainWindow::mymainwindow != NULL ) {
            MainWindow::mymainwindow->statusBar()
                    ->showMessage("Cierre el widget abierto antes de editar otro campo!",3000);
        }
        return;
    }



    moveCursor(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);

    QSet<QString> blankcharacter;
    blankcharacter << " " << "\t" << "\r" << "\n";
    QString c;
    bool blankcharacterpresent = false, namefieldpresent = false;
    int length = toPlainText().length();
    int curpos = textCursor().position();
    while ( (curpos+1) < length ) {
        moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

        c =toPlainText().mid(curpos,1);
        if  (blankcharacter.contains(c)) {
            blankcharacterpresent = true;
        }

        if ( c ==  QString(":")) {
            if ( !blankcharacterpresent  && !namefieldpresent ) {
                namefieldpresent = true;
                moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
                curpos = textCursor().position();
                continue;
            }
            moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
            c =toPlainText().mid(textCursor().position(),1);
            if ( !blankcharacter.contains(c)) {
                moveCursor(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            }
            QString  preword = toPlainText().mid(textCursor().position(),12);

            moveCursor(QTextCursor::PreviousWord, QTextCursor::MoveAnchor);
            preword = toPlainText().mid(textCursor().position(),12);

            moveCursor(QTextCursor::PreviousWord, QTextCursor::MoveAnchor);
            preword = toPlainText().mid(textCursor().position(),12);

            moveCursor(QTextCursor::PreviousWord, QTextCursor::MoveAnchor);
            preword = toPlainText().mid(textCursor().position(),12);
            qDebug("\n4...textCursor().preword():|%s|",
                   qPrintable(preword));

//            curpos = textCursor().position();
            break;
        }
        curpos = textCursor().position();

    }
    QString preword;

    preword = toPlainText().mid(textCursor().position(),12);
    qDebug("\n5before...textCursor().preword():|%s|",
           qPrintable(preword));

#if (QT_VERSION < QT_VERSION_CHECK(4, 5, 0))
    //moveCursor(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);
    QString cbefore;
    curpos = textCursor().position();
    int curr = curpos;
    while((curpos+1) < length) {
        moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
        cbefore = toPlainText().mid(curr,curpos-curr+1);
        c =toPlainText().mid(curpos,1);
        curpos = textCursor().position();
        if ( c == ":" ) {
            moveCursor(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
            curpos = textCursor().position();
            while (curpos > 0 ) {
                c =toPlainText().mid(curpos,1);
                if (blankcharacter.contains(c)) {
                    qDebug("......check...............c: |%s|", qPrintable(c));
                    if (c == "\r" || c == "\n") {
                           while (curpos > 0 ) {
                               c =toPlainText().mid(curpos,1);
                               if (!blankcharacter.contains(c)) {
                                   break;
                               }
                               moveCursor(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
                               curpos = textCursor().position();


                           }

                    }
                    else {
                        while (curpos > 0 ) {
                            c =toPlainText().mid(curpos,1);
                            if (!blankcharacter.contains(c)) {
                                break;
                            }
                            moveCursor(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
                            curpos = textCursor().position();


                        }

//                        moveCursor(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
//                        moveCursor(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
                    }
                    break;
                }
                moveCursor(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
                curpos = textCursor().position();
            }


            break;
        }
    }
#else
        moveCursor(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);
#endif


    preword = toPlainText().mid(textCursor().position(),12);
    qDebug("\n5...textCursor().preword():|%s|",
           qPrintable(preword));

    // ***  Eliminar los espacios en blanco al final del campo
    while((curpos+1) < length) {
        moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
        c =toPlainText().mid(curpos,1);
        curpos = textCursor().position();
        if (!blankcharacter.contains(c)) {
            break;
        }
    }
    preword = toPlainText().mid(textCursor().position(),12);
    qDebug("\n.....6...textCursor().preword():|%s|",
           qPrintable(preword));

    // ***  Eliminar los espacios en blanco al final del campo
    curpos = textCursor().position();

    while ( (curpos-1) >  0 ) {
        moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        c =toPlainText().mid(textCursor().position(),1);
        if  ( c == ":") {
            qDebug("\n**1...textCursor().selectedText():|%s|",
                   qPrintable(textCursor().selectedText()));

#if (QT_VERSION > QT_VERSION_CHECK(4, 4, 3))
            moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
            moveCursor(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
#else
            qDebug("**...version <=4.4.3");
            moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
            moveCursor(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
#endif
            break;
        }
        curpos = textCursor().position();
    }

    // borrar y actualizar;
    //textCursor().removeSelectedText();

    QString namefield, valuefield;
    QStringList fields = textCursor().selectedText().split(":");
    qDebug("\n...textCursor().selectedText():|%s|",
           qPrintable(textCursor().selectedText()));
    namefield = fields.at(0);

    if (fields.count() > 1 ) {
        valuefield = fields.at(1);
    }

    insertPlainText(namefield+": ");
    insertPlainText("\n");
    moveCursor(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);

    // ** Mostrar valor en el widget
    // ************************************
    DomModel *mydommodel = dommodel();
    if ( mydommodel == NULL ) {
        return;
    }
    QString command = searchCommand();


    l = mydommodel->selWidgetFromField(command, namefield, this);

    if ( l == NULL ) {
        SafetYAWL::streamlog
                << SafetLog::Warning
                << tr("El Widget para la etiqueta \"%1\" no pudo ser mostrado")
                .arg(namefield);
        return;
    }


    l->setPrevCursor(textCursor());
    l->setPrevValue(valuefield);
    if (QString(l->metaObject()->className()) == "ListWidget") {
        l->setGeometry(cursorRect().topLeft().x(), cursorRect().topLeft().y()-cursorRect().height()/2,
                       l->getGeoParams().width(), l->getGeoParams().height()+100);

    }
    else {
    l->setGeometry(cursorRect().topLeft().x(), cursorRect().topLeft().y()-cursorRect().height()/2,
                   l->getGeoParams().width(), l->getGeoParams().height());
    }


    if (!l->isVisible()) {
        l->setPrevCursor(textCursor());
        l->setText(valuefield.trimmed());
        l->show();
        l->setFocus(Qt::PopupFocusReason);
    }

}

void TextEdit::delCurrentField() {

    if ( l && l->isVisible()) {
        MainWindow::mymainwindow->statusBar()->showMessage("Cierre el widget abierto antes de editar otro campo!",3000);
        return;
    }
    moveCursor(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);

    QSet<QString> blankcharacter;
    blankcharacter << " " << "\t" << "\r\n" << "\n";
    QString c;
    bool blankcharacterpresent = false, namefieldpresent = false;
    int length = toPlainText().length();
    int curpos = textCursor().position();
    while ( (curpos+1) < length ) {
        moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);

        c =toPlainText().mid(curpos,1);
        if  (blankcharacter.contains(c)) {
            blankcharacterpresent = true;
        }

        if ( c ==  QString(":")) {
            if ( !blankcharacterpresent  && !namefieldpresent ) {
                namefieldpresent = true;
                moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
                curpos = textCursor().position();
                continue;
            }
            moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);            
            c =toPlainText().mid(textCursor().position(),1);
            if ( !blankcharacter.contains(c)) {
                moveCursor(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
            }
            QString  preword = toPlainText().mid(textCursor().position(),12);
            moveCursor(QTextCursor::PreviousWord, QTextCursor::MoveAnchor);
            preword = toPlainText().mid(textCursor().position(),12);
            moveCursor(QTextCursor::PreviousWord, QTextCursor::MoveAnchor);
            preword = toPlainText().mid(textCursor().position(),12);
            moveCursor(QTextCursor::PreviousWord, QTextCursor::MoveAnchor);
            preword = toPlainText().mid(textCursor().position(),12);

            break;
        }
        curpos = textCursor().position();

    }
    moveCursor(QTextCursor::EndOfWord, QTextCursor::MoveAnchor);

    // ***  Eliminar los espacios en blanco al final del campo
    while((curpos+1) < length) {
        moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
        c =toPlainText().mid(curpos,1);
        curpos = textCursor().position();
        if (!blankcharacter.contains(c)) {
            break;
        }
    }
    // ***  Eliminar los espacios en blanco al final del campo
    curpos = textCursor().position();
    while ( (curpos-1) >  0 ) {
        moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        c =toPlainText().mid(textCursor().position(),1);
        if  ( c == ":") {
            moveCursor(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
            moveCursor(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
            break;
        }
        curpos = textCursor().position();
    }

    // borrar y actualizar;
    textCursor().removeSelectedText();

    insertPlainText("\n");
    moveCursor(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor);
    updateDockSbMenu();
}


void TextEdit::updateDockSbMenu(bool getfocus) {
    if (c == NULL) {
        return;
    }

    DomModel *dommodel = ((QCmdCompleter*) c)->domModel();
    if (dommodel == NULL ) return;
    QStringList list, listsbmenu;
    if ( MainWindow::docksbMenu == NULL ) return;
    bool listcommand = searchPrvsToken(list);
    if ( !listcommand ) {
        listsbmenu = dommodel->getCommands(true);
        MainWindow::docksbMenu->addOptions(listsbmenu);
    }
    else {


        listsbmenu = dommodel->getFields(list.at(list.count() -1 ),true );

        for(int i = 0; i < list.count()-1; i++ ) {
            QString field = list.at(i).trimmed();
            QString pattern = QString("%1\\*?(\\:\\:\\:.+)?").arg(field);

            QRegExp rx(pattern);

            int pos = listsbmenu.indexOf(rx);
            if (pos  != -1 && !dommodel->isRepetibleField(field)) {
                  listsbmenu.removeAt(pos);
            }

        }
        MainWindow::docksbMenu->addOptions(listsbmenu);
     }

    if ( getfocus ) {
        setFocus();
        moveCursor(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
    }

}


void TextEdit::execSbMenuAction(const QString& title) {

    insertCompletion(title);
    if ( title.startsWith("operacion:") ) {
        setFocus();
    }


}

void TextEdit::insertTextRegToModify(const QString& fieldname, const QString& key) {

      DomModel *dommodel = ((QCmdCompleter*) c)->domModel();
      if ( dommodel == NULL ) {
          return;
      }
      QStringList list;
        //bool iscommand = searchPrvsToken(list);
        searchPrvsToken(list);
        if ( list.count() > 0 && list.at(list.count()-1).startsWith("modificar") ) {
            qDebug("...getUpdateString....(1)");
            QString result = dommodel->getUpdateString(list.at(list.count()-1),fieldname,key,list);
            qDebug("...getUpdateString....(2)");
            insertPlainText(result);
		updateDockSbMenu();
        }

}


CmdWidget* TextEdit::currentWidget() {

    return l;
}

void TextEdit::cancelAndClose() {
    if (l == NULL ) {
        return;
    }
    QTextCursor mycursor = textCursor();
    setTextCursor(l->prevCursor());
    l->cancelAndClose();
    l = NULL;
    setFocus();
    setTextCursor(mycursor);

}

void TextEdit::insertAndClose() {
      qDebug("...TextEdit::insertAndClose()...(*1)...");
      if (l == NULL ) {
          return;
      }
      QTextCursor mycursor = textCursor();
      QString updatetitle = l->caption();
      QString updatecaption = l->text();
      qDebug("**...TextEdit:updatecaption: %s", qPrintable(updatecaption));
      setTextCursor(l->prevCursor());
      qDebug("...l->metaObject()->className():%s",l->metaObject()->className());
      if (QString("TextEditWidget") == l->metaObject()->className()) {
          TextEditWidget *mytextw = qobject_cast<TextEditWidget*>(l);
          if (mytextw == NULL  ) {
                 return;
          }
          if ( mytextw->isWiki()) {
              qDebug("....html:\n%s\n",qPrintable(mytextw->toHtml()));
              insertPlainText(TextEditWidget::htmlToWiki(mytextw->toHtml()));
          }
          else {
              insertPlainText(mytextw->toPlainText());              
          }

      }
      else {
          if (!l->isPassword()) {
              insertPlainText(updatecaption.trimmed());
          }
          else {
              QString text = updatecaption.trimmed();
              QByteArray textsha1 = QCryptographicHash::hash(QByteArray(qPrintable(text)),
                                                             QCryptographicHash::Sha1);
              insertPlainText(textsha1.toHex().data());
          }
      }


      setFocus();
      insertPlainText("\n");
      insertTextRegToModify(updatetitle, updatecaption);
      if (!l->input().isEmpty()) {
          QString myinput = l->input();
          if (myinput.startsWith("cn::")) {
              QStringList mylist = myinput.split("::");
              if (mylist.count() == 2  ) {
                  MainWindow::_currentCommonName = myinput+"::"+updatecaption;
              }
          }
      }
      l->close();
      //delete l;
      l = NULL;
  

      setFocus();
      setTextCursor(mycursor);
}



void TextEdit::showFromWidget(const QRect& r, const QString& caption) {

    qDebug("...showFromWidget....(1)...caption: |%s|",qPrintable(caption));

    if (l && l->isVisible()) { 
        setFocus();
        QString updatetitle = l->caption();
        QString updatecaption = l->text();
        l->hide();
        if (!l->isPassword()) {
            insertPlainText(l->text().trimmed());
        }
        else {
            QString text = l->text().trimmed();
            QByteArray textsha1 = QCryptographicHash::hash(QByteArray(qPrintable(text)),
                                                           QCryptographicHash::Sha1);
            insertPlainText(textsha1.toHex().data());
        }


        //insertPlainText("\n");
        insertTextRegToModify(updatetitle, updatecaption);
        if (l != NULL ) {
            l->close();
            l = NULL;
        }
     
        return;
    }


    DomModel *dommodel = ((QCmdCompleter*) c)->domModel();
    if ( dommodel == NULL ) {
        return;
    }

    QString command = searchCommand();
    QString newcaption = caption;
    qDebug("...showFromWidget....(4)...newcaption: |%s|",qPrintable(newcaption));
    if ( dommodel->prefix().length() > 0  && newcaption.endsWith(dommodel->suffix()) )  {
        newcaption.remove(0,1);
    }
    if ( dommodel->suffix().length() > 0  && newcaption.endsWith(dommodel->suffix()) )  {
        newcaption.chop(1);
    }

    qDebug("...showFromWidget....(5)...newcaption: |%s|",qPrintable(newcaption));

    l = dommodel->selWidgetFromField(command, newcaption, this);

    if ( l == NULL ) {
        SafetYAWL::streamlog << SafetLog::Warning << tr("El Widget para la etiqueta \"%1\" no pudo ser mostrado").arg(caption);
        return;
    }

    if (QString(l->metaObject()->className()) == "ListWidget") {
        l->setGeometry(cursorRect().topLeft().x(), cursorRect().topLeft().y()-cursorRect().height()/2,
                       l->getGeoParams().width(), l->getGeoParams().height()+100);

    }
    else {
    l->setGeometry(cursorRect().topLeft().x(), cursorRect().topLeft().y()-cursorRect().height()/2,
                   l->getGeoParams().width(), l->getGeoParams().height());
    }

//    Chequear

//    l->setPrevValue(newcaption);

    if (!l->isVisible()) {
        l->setPrevCursor(textCursor());
        l->show();
        l->setFocus(Qt::PopupFocusReason);
        updateDockSbMenu();
    }


}

QString TextEdit::hashToTextEdit(const QByteArray& h) {
    QString result;

    return result;
}

QByteArray TextEdit::texteditToHash(const QString& t) {
    QByteArray result;

    return result;
}

QMap<QString,QVariant> TextEdit::findConf(const QString& n) {

     QMap<QString,QVariant> mapnull;
     for ( int i = 0; i < listConfs().count(); i++) {
               if ( listConfs().at(i)["namewidget"]  == n ) {
                    return listConfs().at(i);
               }
     }
     return mapnull;
}


bool TextEdit::searchPrvsToken(QStringList& list) {
            bool result = false;

	    QTextCursor tc = textCursor();
	    int pos = tc.position();    
            int len = toPlainText().length();
            DomModel *dommodel = ((QCmdCompleter*) c)->domModel();
            Q_CHECK_PTR( dommodel );
            QRegExp rx(tr("\\b([\\-A-Za-z_0-9áéíóúñÑ]+)").toLatin1()+dommodel->suffix()+tr("\\s*([a-zA-Z_0-9\\-áéíóúñÑ]+)?").toLatin1());
            if ( dommodel->commandName().length() == 0 ) result = true;
	    pos = len;
	    while ( true ) {	
		int index = rx.lastIndexIn(toPlainText(), -((len-pos)+1) );
		if ( index == -1 ) break;
		pos = index;
                QString item;
                if ( !result && rx.cap(1).compare(dommodel->commandName(),Qt::CaseInsensitive) == 0 ) {
                     item = rx.cap(2);
                     result = true;
                }
                else {
                    item = rx.cap(1);
                }
                list.append( item );
	    }	   	
            if ( dommodel->commandName().length() == 0 ) {
                 list.append("safet");
                 result = true;
            }

            return result;
}


/*!
    \fn TextEdit::searchCommand()
 */
QString TextEdit::searchCommand() {
	    QString result ="";
	    DomModel *dommodel = ((QCmdCompleter*) c)->domModel();
	    Q_CHECK_PTR( dommodel );		
	    QTextCursor tc = textCursor();
	    int pos = tc.position();    
            int len = toPlainText().length();


	    pos = len; //ajuste 
            QRegExp rx("\\boperacion:\\s*([A-Za-z_0-9\\-áéíóúñÑ]+)");
	    while ( true ) {	
		int index = rx.lastIndexIn(toPlainText(), -((len-pos)+1) );
		if ( index == -1 ) break;
		pos = index;
                result = rx.cap(0);
                //result.chop(1);
                if ( dommodel->commands().contains( result ) == true ) {
                     result = rx.cap(1);
                     break;
                }
             }
            if (result.startsWith("operacion:") ) {
                result = result.mid(QString("operacion:").length());
            }
	    return result;
}

