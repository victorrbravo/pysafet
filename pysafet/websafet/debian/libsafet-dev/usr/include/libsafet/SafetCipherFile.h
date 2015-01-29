#ifndef SAFETCIPHERFILE_H
#define SAFETCIPHERFILE_H

#include <QFile>
 #include <QStringList>
#include <openssl/rc4.h>


class SafetCipherFile : public QFile
{
Q_OBJECT
    RC4_KEY _key;
    unsigned int l;
    unsigned char buffer[256 ];
    unsigned char buffer_out[256];
    QStringList _bufferlines;
    static  int buffer_len;
    int _currentline;
    QDataStream _infile;

public:
    explicit SafetCipherFile(QObject *parent = 0);
    SafetCipherFile(const QString & name );
    ~SafetCipherFile();

    void setPassword(const QString& p);
    QString readCipherLine();
    void readAllCipher();
    bool cipherInFile(const QString& filename);
signals:

public slots:

};

#endif // SAFETCIPHERFILE_H
