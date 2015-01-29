#ifndef SENDSIGNDOCUMENTDIALOG_H
#define SENDSIGNDOCUMENTDIALOG_H

#include <QDialog>

namespace Ui {
    class SendSignDocumentDialog;
}

class SendSignDocumentDialog : public QDialog {
    Q_OBJECT
public:
    SendSignDocumentDialog(QWidget *parent = 0);
    ~SendSignDocumentDialog();

protected:
    void changeEvent(QEvent *e);

    bool searchInListNameFile(const QString& n);
public slots:
    void getContainerFiles();
    void showFileInfo();
    void  delFiles();
    void selectFiles();
    void sendFiles();
public:
    static void compresstar(const QStringList& files, const QString& filename);

private:
    Ui::SendSignDocumentDialog *ui;
};

#endif // SENDSIGNDOCUMENTDIALOG_H
