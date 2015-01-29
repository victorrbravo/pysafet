#include "SafetCipherFile.h"
#include "SafetYAWL.h"
#include <QStringList>
#include <openssl/rc4.h>
#include <stdio.h>
#include <string.h>


int SafetCipherFile::buffer_len = 256;

SafetCipherFile::SafetCipherFile(QObject *parent) :
    QFile(parent),
    _infile(this) {


    _currentline = 0;
  //  _infile = NULL;
    l = 0;

}

SafetCipherFile::SafetCipherFile(const QString& name )
    : QFile(name),
    _infile(this) {

//    buffer  = new char[SafetCipherFile::buffer_len ];
//    buffer_out  = new char[SafetCipherFile::buffer_len ];
    _currentline = 0;
//    _infile = NULL;
    l = 0;

}

SafetCipherFile::~SafetCipherFile() {
//    if (buffer ) {
//        delete buffer;
//    }
//    if (buffer_out) {
//        delete buffer_out;
//    }
//    if (_infile) {
//        delete _infile;
//    }

}

void SafetCipherFile::setPassword(const QString& p) {
    RC4_set_key(&_key,p.length(),(const unsigned char*) qPrintable(p));
}

QString SafetCipherFile::readCipherLine() {
    QString result;
    if (_currentline  < _bufferlines.count()) {
             result = _bufferlines.at(_currentline);
             _currentline++;
             return result;
    }

    return QString();

}

void SafetCipherFile::readAllCipher() {







    _currentline = 0;
    _bufferlines.clear();
    QString bufferline;
    while(!_infile.atEnd()) {
        l = _infile.readRawData((char*)buffer,buffer_len);
//        qDebug("l:%d",l);
        if ( l == -1 ) {
            break;
        }
        memset(buffer_out,0,buffer_len);

        RC4(&_key,l,(unsigned char*)buffer,(unsigned char*)buffer_out);
//        printf("(1)-------> l: %d\n",l);
//        printf("|");
//        for(int i=0; i < l;i++) {
//            printf("%c",buffer_out[i]);
//        }
//        printf("|");
//        printf("\n");
//        printf("(2)-------> l: %d\n",l);
//        //memset(buffer_out,0,buffer_len);
//        printf("version int (1)-------> l: %d\n",l);
//        for(int i=0; i < l;i++) {
//            printf("|%d|",buffer_out[i]);
//        }
//        printf("\n");
//        printf("version int (2)-------> l: %i\n",l);
//
        //        if (l < buffer_len  ) {
//            buffer_out[l] = 0;
//        }

        QString newdata;
        for(int i = 0; i < l; i++) {
            newdata = newdata + buffer_out[i];
        }
        bufferline = bufferline+ newdata;
    }

    _bufferlines = bufferline.split("\n");
//    qDebug("_bufferlines.count():%d",_bufferlines.count());
//    for(int i=0; i < _bufferlines.count();i++) {
//        _bufferlines[i].trimmed();
//       // qDebug("...**_bufferlines[%d]=|%s|",i,qPrintable(_bufferlines[i]));
//    }



    qDebug("...saliendo...");
    return;
}

bool SafetCipherFile::cipherInFile(const QString& filename) {
    QFile myfile(filename);
    if ( !myfile.open(QIODevice::WriteOnly)) {
        SafetYAWL::streamlog
                << SafetLog::Error
                << tr("No se pudo abrir el archivo \"%1\" para escritura")
                .arg(filename);
        return false;
    }
    QTextStream in(this);
    QDataStream out(&myfile);
    in.setCodec("ISO 8859-1");
    while(!in.atEnd()) {
        QString line = in.readLine();
        line += "\n";

        //qDebug("cifrando:%s",qPrintable(line));
        l = line.length();
        RC4(&_key,l,(unsigned char*)line.toLatin1().data(),buffer_out);
        int byteswritten = out.writeRawData((const char*)buffer_out,l);

    }

    myfile.close();

    return true;


}
