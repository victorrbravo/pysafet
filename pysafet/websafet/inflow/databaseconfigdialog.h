#ifndef DATABASECONFIGDIALOG_H
#define DATABASECONFIGDIALOG_H

#include <QDialog>


namespace Ui
{
    class DatabaseConfigDialog;
}


class DatabaseConfigDialog : public QDialog
{
    Q_OBJECT

    bool initialStage;

public:

    DatabaseConfigDialog(QWidget *parent = 0);
    ~DatabaseConfigDialog();

    void setInitialStage(bool b);
    bool getInitialStage();

    // valida que los campos no esten vacios
    bool validateFields();

    // prueba la conexion con los parametros establecidos
    bool testDataSources();

    // prueba la conexion con los nuevos parametros establecidos
    // en la interfaz grafica
    bool testNewDataSources();

    // QStringList con los valores de configuracion
    QStringList configValues();

    void setConfigValues(const QString& host,
                         const QString& database,
                         const QString& port,
                         const QString& user,
                         const QString& pass = QString(""),
                         const QString& source = QString("database1"));

    // asigna el valor de la bandera configFieldEmpty en caso de que
    // algun campo de la configuracion este vacio
    void setConfigFieldEmpty(bool value);

    // retorn el valor de la bandera configFieldEmpty
    bool getConfigFieldEmpty();

    // asigna el valor de la bandera cancelFlag en caso de que
    // se presiona el boton cancelar del dialogo
    void setCancelFlag(bool value);

    // retorn el valor de la bandera cancelFlag
    bool getCancelFlag();

    // para limpiar los campos del archivo auth.conf que contienen el nombre
    // de la base de datos, el usuario y la contrasena
    void cleanConfigurationFile();

    // para parsear el archivo SQL con la base de datos local de pruebas
    QStringList parseSQLFile(const QString& s);

    // crea la base de datos local de pruebas a partir del script SQL
    bool createDB();

public slots:
    void doAccept();
    void doCancel();

    void checkBoxLocal(int state);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DatabaseConfigDialog *ui;

    bool configFieldEmpty;

    bool cancelFlag;


};

#endif // DATABASECONFIGDIALOG_H
