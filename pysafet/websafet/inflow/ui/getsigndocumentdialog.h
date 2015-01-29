#ifndef GETSIGNDOCUMENTDIALOG_H
#define GETSIGNDOCUMENTDIALOG_H

#include <QDialog>
#include <QListWidget>


class GetSignDocumentDIalog : public QObject {
    Q_OBJECT
public:
    GetSignDocumentDIalog(QWidget *parent = 0);
    ~GetSignDocumentDIalog();
    static QStringList uncompresstar(const QString& filename);

public slots:
    void selectFiles();
    void delFiles();
    void includeFiles();
    bool enableIncludeFiles();
    bool searchInListNameFile(const QString& n);
    void showFileInfo();

};

#endif // GETSIGNDOCUMENTDIALOG_H
