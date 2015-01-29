#ifndef INFLOWFIRMADOC_H
#define INFLOWFIRMADOC_H

#include "combowidget.h"
#include "SafetWorkflow.h"

class ComboWidget;

class InflowFirmaDoc : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(InflowFirmaDoc)
    bool jscriptload;
    QString currentDocuments;
    QString _currentSigners;
    QString _entityToSign;
    QString jscriptcolumns;
    QString _listprincipaltitle;
    QString _allcode;
    QString _newhtmlfile;

    QList<QString> _availablefiles;

    int _listprincipalcount;
    QList<QSqlField> currentFields;
    QString evalJS(const QString &js);

public:
    explicit InflowFirmaDoc(QObject *parent = 0);
    virtual ~InflowFirmaDoc(); 


    QList<QString> availableFiles()  const { return _availablefiles; }

    void setAvailableFiles(const QList<QString>& list) { _availablefiles = list; }

    QString currentSigners() const  { return _currentSigners; }

    QString newHtmlFile() const { return _newhtmlfile; }

    void generateJScriptreports(const QString& documents,
                                                const QList<QSqlField>& fields,
                                                int ncounts,
                                                const QString& entitytosign,
                                                QList<SafetWorkflow::InfoSigner>& signers
                                );

    void loadSignersInfo(const QList<SafetWorkflow::InfoSigner>& l);
    void loadReportTemplate(const QString& bdocname = "" , const QString& newname = "");
    friend class ComboWidget;

    void savepdf();

    void executeJSCodeAfterLoad(bool ok );
private slots:
    void doPrint();
    void viewpdf();
};

#endif // INFLOWFIRMADOC_H
