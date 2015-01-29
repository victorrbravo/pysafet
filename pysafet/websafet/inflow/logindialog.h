#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMap>


namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

public slots:
    void doAccept();
    void doCancel();

protected:
    void checkInputFiles();
    QMap<QString,QStringList> users;
    void changeEvent(QEvent *e);
    void loadInfo();
    void loadAuthorization();
private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
