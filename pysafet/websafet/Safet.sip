%Module Safet 

%Import QtCore/QtCoremod.sip
%Import QtXml/QtXmlmod.sip
%Import QtSql/QtSqlmod.sip
%Import QtGui/QtGuimod.sip
%Import QtNetwork/QtNetworkmod.sip
%Import QtWebKit/QtWebKitmod.sip
%Import QtSvg/QtSvgmod.sip

class MainWindow : QObject
{
%TypeHeaderCode
#include "/home/vbravo/websafet/inflow/mainwindow.h"
%End

public:

    MainWindow(const QString& path = QLatin1String(""));
    QStringList autoComplete(const QString &path);
    bool login(const QString& name, const QString& pass);   
    bool logout(); 
    QString toInputForm(const QString& action);
    bool toInputConsole(const QString& action);
    bool toInputUsers(const QString& action);
    QString currentJSON();
    QString currentError();
    void setMediaPath(const QString& m);
    QString mediaPath();
    QString inputPath();
    void setInputPath(const QString& p);
    QString menuCommands();
    QString menuForm(const QString &o);	 
    QString hostURL();
    void setHostURL(const QString& u);
    void setHostMediaPath(const QString& m);
    QString hostMediaPath();
    QString generateFormHead(const QString &o);
    QString generateFormFooter(const QString &o);
    QString generateModifyHTML(const QString &operation, const QString &fieldname, const QString &key,const QString& secondkey, const QStringList& form);
    QString getFlowParameters(const QString& flowfilename); 
    void setConffileValues(const QMap<QString,QString>& values);
    void setParsValues(const QMap<QString, QString> &values);
    QMap<QString,QString> loadEditActions();
    QString addInfoGraphDateText();
    QString currentGraphTitle();
    QString loadReportTemplate(const QString& json, const QString &filename = QString(""),const QString& nameoperation = QString("Listar_datos"));
    void setTemplatePath(const QString& t);
    QString templatePath();
    void registerLogin(const QString &user);
    void registerLogout(const QString &user);
    void log(const QString& message);
    QStringList lastInfoGraph();
    bool doSaveGraph(const QStringList& mypars);
    QString addNodeToXMLWorkflow(const QString &fname,
                                 const QString& beforenode = QString(),                                  
                                 const QString &nodename = QString("newnode"),
                                 bool isparallel = false,
                                 const QString &options = QString(),
                                 const QString &query = QString(),
                                 const QString &nodetitle = QString(),
                                 const QString &documentsource = QString()
                                  );
    QString delNodeToXMLWorkflow(const QString &fname, const QString &nodename);
  QString changeConnXMLWorkflow(const QString &fname, 
                                   const QString &currnode, 
                                   const QString &nextnode, 
                                   const QString &newnode,
                                   const QString& newoptions = QString(),
                                    const QString&  newquery  = QString());

    bool createBdoc(const QString &content);
    QString checkUserRegister(const QString &fullname, const QString &account,const  QString &email, const QString &passone,const  QString &passtwo);
	QStringList getInfoOfUser(const QString &user);
    bool sendCheckEmail(const QString &user,const QString &plink);
    
    QString currentTable();
    void doLoadConfiguration(const QString &fileName);
    void doSaveConfiguration(const QString& fileName);

};

class SafetXmlObject : QObject 
{
%TypeHeaderCode
#include "/home/vbravo/websafet/src/SafetXmlObject.h"
%End

public:
    SafetXmlObject();
    bool syncAttributes(const QDomElement&);
        
};

class SafetDocument : SafetXmlObject
{
%TypeHeaderCode
#include "/home/vbravo/websafet/src/SafetDocument.h"
%End

public:
    SafetDocument();
	static QStringList getCertFileList(const QString& ext = ".pem");
	int numberOfDataFileOnOpenXAdESContainer();
	int numberOfSignaturesOnOpenXAdESContainer();
	QString getDataFileName(int index = 0);
	long getDataFileLength(int index = 0);
	QString getDataFileMimeType(int index = 0);
	int numberOfDataFileAttributes(int index = 0);
	void connectToDataBase(QString connectionString);
	QStringList getDataBaseTableNames();
	void createXMLFileFromSQL(QString sqlSentence, QString outputFileName );
	//QList<QString> getFields(QSqlQuery &query);
	//QList<QVariant::Type> getInfos(QSqlQuery &query);
	QString getXmlQuery(QSqlQuery &query, const QString& info = "");
	//QString getJsonQuery(QSqlQuery &query, QList<QSqlField>& fields, const QString& info = "");
	int addSignatureToExistingDigidocFile(QString keyFile, QString passwd, QString certFile, QString inFile, QString outFile);
	int signWithPrivateKeyOnFile(const QString&  keyFile,
                                     const QString& passwd,
                                     const QString& certFile,
                                     const QString& inFile,
                                     const QString& outFile,
                                     const QString& manifest,
                                     const QString& city,
                                     const QString& state,
                                     const QString& zip,
                                     const QString& country,
                                     const QString& notaryUrl,
                                     const QString& proxyHost,
                                     const QString& proxyPort);
	void initPKCS11Lib(QString libName);
	int initDigidocConfigStore(QString configFile);
	bool verifySignMadeWithSmartCard(QString fileName);
	bool verifySignMadeWithSmartCard(QString fileName, int signatureIndex);
	enum SignatureOperator  { AND, OR, ALL };
	bool verifySignMadeWithSmartCard(const QString& fileName,
                                         const QStringList& listOfSigners,
                                         bool &isneg,
                                         SignatureOperator op = SafetDocument::AND );
	void testSmartCardDriver(int slot);
	QString getCN(int signatureIndex);
	int getSignerIndex(QString cn);
	QStringList getCommonNameOfSigners();
	int getNumberOfPrivateKeys(int slot, QString passwd, QString libName);
	QString writeFileToDisk(QString string, QString name);
	QString returnFileToString(const QString& pathFileName);
	int decryptDocument(QString inFile, QString outputFile, QString pin);
	QString decryptString(QString inFile, QString pin);
	QStringList getTempNameFiles(int n);
	QString getCommonNameFromCertificate(QString certPath);
	QStringList getAttribute(int index = 0);
	QString getCountryOfSignature(int index = 0);
	QString getStateOfSignature(int index = 0);
	QString getCityOfSignature(int index = 0);
	QString getPostalCodeOfSignature(int index = 0);
	QStringList signatureLocations(int index = 0) const;
	QString signatureLocation(int index = 0) const;
	int getCountOfRoles(int index = 0);
	QString getRole(int index = 0, int roleIndex = 0);
	QString getSingingTime(int index = 0);
	QString getSingingTimeOnlyDate(int index = 0);
	QString getSingingTimeOnlyHour(int index = 0);
	QString getSignatureType(int index = 0);
	QString getSignatureFormat(int index = 0);
	QString getSignerCertificateIssuerName(int index = 0);
	QString getSignerCertificateSerial(int index = 0);
	QString getValidAt(int index = 0);
	QString getValidUntil(int index = 0);
	QString getPathOfSafetDocument();
	void setPathOfSafetDocument(QString path);// { m_filename = path; }
	QStringList enhancedKeyUsage(int index = 0);
	QString toUtf16( const QString &in ) const;
	QString toUtf8( const QString &in ) const;
	QString decode( const QString &in ) const;
	QString subjectInfoUtf8( int index, int subject ) const;
	QByteArray versionNumber(int index = 0);
	QByteArray serialNumber(int index = 0);
	QByteArray toHex( const QByteArray &in, QChar separator );
	QString getSubjectDN(int index = 0);
	QString getIssuerDN(int index = 0);
	QStringList policies(int index = 0);
	QByteArray authorityKeyIdentifier(int index = 0);
	QByteArray subjectKeyIdentifier(int index = 0);
	QString getPublicKeyAlgorithm(int index = 0);
	QString getPublicKeyLength(int index = 0);
	void writeX509ToFile(QString path, bool PEM, int index = 0);

};

class SafetVariable : SafetXmlObject
{
%TypeHeaderCode
#include "/home/vbravo/websafet/src/SafetVariable.h"
%End

public:
	SafetVariable();

	virtual void addChild(SafetXmlObject*);
	QString id();
	void setId(const QString&);
	QString tokenlink();
	void setTokenlink(const QString&);
	QString scope();
	void setScope(const QString&);
	QString type();
	void setType(const QString&);
	QString config();
	void setConfig(const QString&);
	QString source();
	void setSource(const QString&);
	QString documentsource();
	void setDocumentsource(const QString&);
	QString description();
	void setDescription(const QString&);
	QString foo();
	int getDocumentCount();
	QList<SafetDocument*>& getDocuments();
	QString getXMLDocument(const QString &value, long fieldno, QString& documentid  ) const;
	QString createXMLFileFromQuery(const QSqlQuery& query, const QString& outputFileName) const;

};

class SafetYAWL : SafetXmlObject
{
%TypeHeaderCode
#include "/home/vbravo/websafet/src/SafetYAWL.h"
%End

public:
    SafetYAWL(const QString& path = QLatin1String(""));

    void openXML(const QString&);
    void convertXMLtoObjects();
    SafetWorkflow* getWorkflow();
    void openDataSources();
    void setCurrentPin(const QString& p);
    QString currentPin() const;
    bool signDocumentsFromData(const ParsedSqlToData&,
                                   QString& nametowrite,
                                   QStringList& list,
                                   SafetDocument &doc,
                                   bool withsmartcard = true);
	bool verifyDocumentsFromData(const ParsedSqlToData& data, SafetDocument& doc);
    static void loadAllConfFiles(int option = 3);
    void loadPlugins(const QString& plugname);
private:
    SafetYAWL(const SafetYAWL&);
	
};

struct ParsedSqlToData {

%TypeHeaderCode
#include "/home/vbravo/websafet/src/SafetYAWL.h"
%End

	bool null;
	QString table;
	QString key;
	QString keyvalue;
	QStringList fields;
	QStringList values;
	QMap<QString,QString> map;
	ParsedSqlToData();
	bool isNull();
};

class SafetWorkflow : SafetXmlObject
{
%TypeHeaderCode
#include "/home/vbravo/websafet/src/SafetWorkflow.h"
%End

public:
    enum OutputFormat { XML, JSON, SVG };
    SafetWorkflow();
    QString listTasks(bool inc = false, const QString& c = QString("\n") );

//    SafetDocument getDocuments(const QString& idvariable, OutputFormat of = XML,
//                         const QString& info ="");


//    SafetDocument getDocuments(const QString& idvariable, QList<QSqlField>& fields,int &howmanydocuments,
//                         OutputFormat of = XML,
//                         const QString& info ="");



private:
    SafetWorkflow(const SafetWorkflow&);
  
    
        
};
