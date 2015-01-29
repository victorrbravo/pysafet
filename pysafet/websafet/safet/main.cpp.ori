
#include <QCoreApplication>
#include <QMap>
#include <QFile>
#include "mainwindow.h"
#include <getopt.h>
#include <iostream>
using namespace std;

QMap<int,QString> commands;

bool parse(int &argc, char **argv) {
       int c;
       int verbose_flag = 0;
       bool parsed = true;
       while (1)
         {
             struct option long_options[] =
             {
               /* These options set a flag. */
               {"verbose", no_argument,       &verbose_flag , 1},
               {"brief",   no_argument,       &verbose_flag, 0},
                {"operation",    required_argument,       0, 'o'},
                {"variable",    required_argument,       0, 'v'},
                {"key",    required_argument,       0, 'k'},
                {"output",    required_argument,       0, 'O'},
                {"file",    required_argument,       0, 'f'},

               {0, 0, 0, 0}
             };
           /* getopt_long stores the option index here. */
           int option_index = 0;

           c = getopt_long (argc, argv, "o:v:f:k:O:",
                            long_options, &option_index);

           if (c == -1)
             break;
           switch (c)
             {
            case 'o':
               if (!commands.contains('o') ) {
                    commands['o'] = QString::fromUtf8(optarg);
               }
               break;
           case 'O':
              if (!commands.contains('O') ) {
                   commands['O'] = QString::fromUtf8(optarg);
              }
              break;

           case 'f':
              if (!commands.contains('f') ) {
                   commands['f'] =  QString::fromUtf8(optarg);
              }
              break;
           case 'v':
              if (!commands.contains('p') ) {
                   commands['p'] = QString("Variable:%1").arg(optarg);
              }
              break;
           case 'k':
              if (!commands.contains('p') ) {
                   commands['p'] = QString("Clave:%1").arg(optarg);
              }
              break;

           default:
                        qDebug("operacion:%c",c);
                        return false;
             }
         }


       if (!commands.contains('f')) {
           cout << "Debe especificar el parametro --file o -f " << endl;
           parsed = false;

       }
       if (!commands.contains('O')) {
           cout << "Debe especificar el parametro --output o -O " << endl;
           parsed = false;

       }

       if (!commands.contains('p')) {
           cout << "Debe especificar el parametro --variable o -v , o --key -k" << endl;
           parsed = false;

       }
       if (!commands.contains('o')) {
           cout << "Debe especificar el parametro nombre de la operacion --operation  o -o" << endl;
           parsed = false;

       }



        return parsed;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    if ( parse(argc,argv) == false ) {
//        foreach(int key, commands.keys()) {
//            QString value = commands[key];
//            cout << "key:" << (char)key << " value:" << qPrintable(value) << endl;
//        }

        cout << endl << "No se ejecuto la operacion" << endl << endl;
        return 1;
    }
//    foreach(int key, commands.keys()) {
//        QString value = commands[key];
//        cout << "key:" << (char)key << " value:" << qPrintable(value) << endl;
//    }

    QString newconsult;


    newconsult = QString("operacion:%1 Cargar_archivo_flujo:%2 %3")
            .arg(commands['o'])
            .arg(commands['f'])
            .arg(commands['p']);

    //qDebug("...newconsult:|%s|", qPrintable(newconsult));
    QString myhome = QDir::homePath();
    //qDebug("...myhome:|%s|", qPrintable(myhome));

    MainWindow mymain(QString("%1").arg(myhome));
    mymain.setMediaPath(QString("%1/django/media/archivos").arg(myhome));
    mymain.setHostMediaPath("http://localhost");
    mymain.registerLogin("vbravo");
    if (!mymain.login("vbravo","d91408cd") ) {
        qDebug("falla de autenticacion");
        return 1;
    }
//    cout << "Realizando la consulta:\n";
    bool result = mymain.toInputConsole(newconsult);

    if (!result ) {
        cout << "Ocurrio un error:\n";
        qPrintable(mymain.currentError());
        SYE << "Ocurrio un error";

        return 1;
    }
    QFile myoutput(commands['O']);
    qDebug("Escribiendo en : |%s|", qPrintable(commands['O']));

       if ( !myoutput.open(QIODevice::WriteOnly) ) {
          qDebug("No se pudo abrir el archivo:|%s|",qPrintable(commands['O']));
          return 1;
         }

       QTextStream out(&myoutput);
       out.setCodec("Utf8");
    out << mymain.currentJSON();

    myoutput.close();

    return 0;
}
