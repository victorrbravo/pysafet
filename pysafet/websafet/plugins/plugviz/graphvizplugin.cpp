/*
* SAFET Sistema Automatizado para la Firma Electrónica y Estampado de Tiempo
* Copyright (C) 2008 Víctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigación en Tecnologías Libres
*
* Este programa es software libre; Usted puede usarlo bajo los términos de la licencia de
* software GPL versión 2.0 de la Free Software Foundation.
*
* Este programa se distribuye con la esperanza de que sea útil, pero SI NINGUNA GARANTÍA;
* tampoco las implícitas garantías de MERCANTILIDAD o ADECUACIÓN A UN PROPÓSITO PARTICULAR.
* Consulte la licencia GPL para más detalles. Usted debe recibir una copia de la GPL junto
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5º Piso, Boston, MA 02110-1301, USA.
*
*/
#include "graphvizplugin.h"
#include "SafetYAWL.h"
#include <QTextCodec>
#include <QColor>
#include <QStringList>
extern "C" {
    #include <locale.h>
}

graphvizPlugin::graphvizPlugin() {


 }


 QString graphvizPlugin::renderDocuments(const QString& docs, const QMap<QString,QString>& ) const  {

        QString str = docs;
        //qDebug("...rendering...graphviz...");
//        if (context.count() == 0 ) return docs;

//        foreach(QObject* myobj, context) {
//             if (myobj) {
//                    qDebug("class: %s", myobj->metaObject()->className());
//               }
//        }
        return str;
 }

QString graphvizPlugin::processColor(const QString& c) {

     if ( !c.startsWith("#") ) return c;

     Q_ASSERT_X(QRegExp("#[0-9A-Fa-f]{6}").indexIn( c ) >= 0, qPrintable(tr("graphviz_plugins")),
                qPrintable(tr("El parametro de color:\"%1\" no coincide con el patron: \"%2\" (H digito hexadecimal de 0 a F)").arg(c).arg("#HHHHHH"))); // Coincidencia con el Color



     return c;
     //return fred + " " + fgreen + " " + fblue;

}

int graphvizPlugin::checkExtrainfoShow(const QString& sshow) {
    int result = false;



    QStringList mylist = sshow.split(",",QString::SkipEmptyParts);

    //Porc,Tokens,Total,InfoText,InfoDate
    if (mylist.contains("Porc")) {
        result += Porc;
    }

    if (mylist.contains("Tokens")) {
        result += Tokens;
    }

    if (mylist.contains("Total")) {
        result += Total;
    }

    if (mylist.contains("InfoText")) {
        result += InfoText;
    }

    if (mylist.contains("InfoDate")) {
        result += InfoDate;
    }

    return result;
}

QString graphvizPlugin::checkSES(const QString& s, ExtraInfoShow ex,
                                              int  curconf) {
    QString  result;
    if ( ex & curconf )   {
        result = s;
    }

    return result;
}


 QString  graphvizPlugin::parseCodeGraph(const QString& code, const QMap<QString,QString>& context) const  {

      QString result;
      result += "digraph G {\n";
//      result += "size=\"5,5\";\n";
      QStringList mylist = code.split("\n",QString::SkipEmptyParts);
      //_typegraph = context[conf()+"graphtype"];
      QString label, taskshape ;
      double sizemax = context[conf()+"stats.sizemax"].toDouble();
      double sizemin = context[conf()+"stats.sizemin"].toDouble();
      result += "rankdir=\""+context[conf()+"graph.rankdir"] +"\";\n";
      result += "ranksep=\""+context[conf()+"graph.ranksep"] +"\";\n";
      result += "fontsize=\""+context[conf()+"graph.fontsize"] +"\";\n";

      QString sshow = context[conf()+"extrainfo.show"];

      int curshow = checkExtrainfoShow(sshow);

      int origcurshow = curshow;

      bool isextra = false;
      QString statsfontcolor;
      QString statsfillcolor;
      QString statsstyle;
      if (context.contains(conf()+"showstats")) {
          isextra = (context[conf()+"showstats"].compare("yes",Qt::CaseSensitive) == 0);
          statsfontcolor = context[conf()+"stats.fontcolor"];
          statsfillcolor = context[conf()+"stats.fillcolor"];
          statsstyle = context[conf()+"stats.style"];

    }
      bool showfinal  = false;

      qDebug("...graphviz::parseCodeGraph...showfinal:|%d|",showfinal);

      if (context.contains(conf()+"extrainfo.showfinal") ) {
        showfinal = context[conf()+"extrainfo.showfinal"]
                                                .compare("on",
                                                Qt::CaseSensitive) == 0;
        qDebug("...graphviz::parseCodeGraph...contains showfinal");
      }

      qDebug("...graphviz::parseCodeGraph...showfinal (2):|%d|",showfinal);
      bool showheretokens = context[conf()+"extrainfo.showwheretokens"]
                                                .compare("on",
                                                Qt::CaseSensitive) == 0;


      bool origshowheretokens = showheretokens;
      bool showtextual = false;
	   
      showtextual = ( context[conf()+"show.textualinfo"].compare("yes") == 0 );

      bool showrole = false;
	   
      showrole = ( context[conf()+"show.role"].compare("yes") == 0 );

      bool  usegradient = true;

      if (context.contains(conf()+"usegradient")) {
            usegradient = ( context[conf()+"usegradient"].compare("yes") == 0 );
      }

      foreach (QString sec, mylist) {
          QString extranode;
          QString notenode;
          QString nameextranode;
          QRegExp rx("Nodo:\\s*([a-zA-Z0-9\\+_\\.]+)\\s*,");
          int pos = rx.indexIn( sec );
          Q_ASSERT ( pos >= 0 );
          QString newnode = rx.cap(1);
          rx.setPattern("Patron:\\s*([a-zA-Z0-9_\\.]+)\\s*,");
          pos = rx.indexIn( sec );
          Q_ASSERT ( pos >= 0 );
          QString patron = rx.cap(1);
          label  = newnode;
          taskshape = context[conf()+"task.shape"];
          if ( patron.compare("none") != 0 ) {
              taskshape = "Mrecord";
          }
          rx.setPattern("Union:\\s*([a-zA-Z0-9_\\.]+)\\s*,");
          pos = rx.indexIn( sec );
          Q_ASSERT ( pos >= 0 );
          QString patternunion = rx.cap(1);
          if ( patternunion.compare("none" ) != 0 ) {
              taskshape = "Mrecord";
          }

//          QString titlePattern ="Titulo:\\s*([a-zA-Z\\s0-9;\\+_\\.][a-zA-Z\\s0-9;\\0351\\0341\\+\\-/:_\\s\\.]+[a-zA-Z\\s0-9;\\+_\\.])\\s*,?";
          QString titlePattern ="Titulo:\\s*([a-zA-Z\\s0-9;\\0351\\0341\\+\\-/:_\\s\\.]+)\\s*,?";
          rx.setPattern(titlePattern);
          pos = rx.indexIn( sec );
          Q_ASSERT ( pos >= 0 );
          QString desc = rx.cap(1);



          QString rolePattern ="Rol:\\s*([a-zA-Z\\s0-9;\\+_\\.][a-zA-Z\\s0-9;\\0351\\0341\\+\\-/:_\\s\\.]+[a-zA-Z\\s0-9;\\+_\\.])\\s*,";
          rx.setPattern(rolePattern);
          pos = rx.indexIn( sec );


          QString myrole;

          if ( showrole && pos >= 0 )  {
              myrole  = rx.cap(1);
          }

          QString notePattern ="Nota:\\s*([a-zA-Z\\s0-9;\\+_\\.][a-zA-Z\\s0-9;\\0351\\0341\\+\\-/:_\\s\\.]+[a-zA-Z\\s0-9;\\+_\\.])\\s*,";
          rx.setPattern(notePattern);
          pos = rx.indexIn( sec );


          QString mynote;

          if ( pos >= 0 )  {
              mynote  = rx.cap(1);
          }


          QString textualPattern ="TextualInfo:\\s*([a-zA-Z\\s0-9;%\\0351\\0341\\+\\-/:_\\s\\.]+)\\s*,";
          rx.setPattern(textualPattern);
          pos = rx.indexIn( sec );


          QString mytextual;

          if ( showtextual && pos >= 0 )  {
              mytextual  = rx.cap(1);
          }



          rx.setPattern("info\\.task\\.size:\\s*([a-zA-Z0-9_\\.]+)\\s*,?");
          pos = rx.indexIn( sec );
          //    qDebug("...sec: %s", qPrintable(sec));
          // qDebug("pos: %d, patron: %s size: %s", pos, qPrintable(rx.pattern()), qPrintable(sec));
          QString ssize,taskfontsize;
          if  ( pos > 0 ) {
              bool ok;

              double porc = rx.cap(1).toDouble(&ok);
              double vw = porc* (sizemax-sizemin)+sizemin;
              double vh = porc*(sizemax-sizemin)+sizemin;
              vh = vh/2; // Acortar el tamano vertical
              ssize = "," + QString(" width=%1,height=%2,style=\"filled,bold\"").arg(vw, 0, 'g', 2).arg(vh,0, 'g', 2);
              taskfontsize = QString(", fontsize=%1").arg(porc*((sizemax+36)-(sizemin+8))+(sizemin+8),0, 'f', 0);
              ssize += taskfontsize;


          }
          // Para el color
          rx.setPattern("info\\.task\\.color:\\s*([\\-a-zA-Z0-9_\\.]+)\\s*,?");
          pos = rx.indexIn( sec );

          QString scolor = " ,fillcolor=\""+ processColor(context[conf()+"task.fillcolor"]) +"\" ";

          qDebug(".....parseCodeGraph...newnode:|%s|",qPrintable(newnode));
          if  ( pos > 0 || ( newnode == "final" && showfinal) ) {
              bool ok;
              QString cgradient = context[conf()+"stats.colorgradient"];
              Q_ASSERT( cgradient.length() > 0 );

              if ( newnode == "final"  && showfinal) {
                qDebug(".....parseCodeGraph...YES newnode:|%s|",qPrintable(newnode));
                 showheretokens = false;
                 curshow = 0x08;
              }
              else {

                  curshow = origcurshow;
                  showheretokens = origshowheretokens;
              }

              QString ogradient = context[conf()+"stats.othergradient"];
              QColor othercolor (ogradient);
              QColor color ( cgradient );
              int h,s,v;

              double porc = rx.cap(1).toDouble(&ok);
              if ( porc ==  -100 ) {

              }

              else if ( porc < 0 ) {
                  othercolor.getHsv (&h,&s,&v);
                  porc = 1-(porc*-1.0);
                  // Usar gradiente **
                  if ( usegradient ) {
                      s = int(porc*255.0);
                      othercolor.setHsv(h,s,v);
                  }
                  scolor = "," + QString("fillcolor=\"#%1%2%3\"").arg(othercolor.red(),2,16)
                          .arg(othercolor.green(),2,16).arg(othercolor.blue(),2,16);
              }
              else {
                  color.getHsv (&h,&s,&v);
                  s = int(porc*255.0);
                  if ( usegradient ) {
                      color.setHsv(h,s,v);
                  }

                  scolor = "," + QString("fillcolor=\"#%1%2%3\"").arg(color.red(),2,16)
                          .arg(color.green(),2,16).arg(color.blue(),2,16);

              }



              if (isextra ) {

                  QString  total,tokens, rol, timestamp;
                  QStringList stats;

                  QString lastfield = sec.section(",",-1);
                  stats = lastfield.split("...",QString::SkipEmptyParts);

                  QString myinfo;
                  if (stats.count() < 2 ) {

                      if (porc > 0.0) {
                          myinfo = tr("La clave se encuentra aqu��");
                          tokens = "1";
                          total =  "1";
                      }
                      else {
                          tokens = "0";
                          total =  "1";
                      }
                      //isextra = false;
                  }
                  else if (stats.count() == 4 ) {
                      tokens = stats.at(0);
                      total =  stats.at(1);
                      rol = stats.at(2);
                      timestamp = stats.at(3);

                  }
                  else  {
                      tokens = stats.at(0);
                      total =  stats.at(1);

                  }

                  nameextranode = newnode + "_" +"stats";

                  bool ok;
                  int ntokens = tokens.toInt(&ok);
                  if (stats.count() == 4 && !rol.isEmpty() && !timestamp.isEmpty() ) {
                      if ( rol == tr("n/a")) {

                          if (timestamp == tr("n/a") ) {
                              // double diffporc = stats.at(2).toDouble(&ok);
                              if (ntokens > 0 ) {
                                  myinfo =  (checkSES(tr("Porcentaje: %1%<br/>"),Porc,curshow).arg(porc*100,0,'f',2)
                                             +checkSES(tr("Nro Documentos: %1<br/>"),Tokens,curshow).arg(tokens)
                                             +checkSES(tr("Total: %1"),Total,curshow).arg(total));



                                  //                                      .arg(diffporc*100,0,'f',2);
                              }
                              else  {
                                  if ( !showheretokens ) {
                                      myinfo =  (checkSES("Porcentaje: %1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                                 +checkSES("Nro Documentos: %1<br/>",Tokens,curshow).arg(tokens)
                                                 +checkSES("Total: %1",Total,curshow).arg(total));



                                  }
                              }

                          }
                          else {
                              // double diffporc = stats.at(2).toDouble(&ok);
                              if (ntokens > 0 ) {
                                  myinfo =  ( checkSES("Porcentaje: %1%<br/>",Porc,curshow)
                                              .arg(porc*100,0,'f',2)
                                              +checkSES("Nro Documentos: %1<br/>",Tokens,curshow).arg(tokens)
                                              +checkSES("Total: %1<br/>",Total,curshow).arg(tokens)
                                              +checkSES("%1",InfoDate,curshow)
                                              .arg(timestamp));




                              }
                              else {
                                  if ( !showheretokens ) {
                                      myinfo =  (checkSES("Porcentaje: %1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                                 +checkSES("Nro Documentos: %1<br/>",Tokens, curshow).arg(tokens)
                                                 +checkSES("Total: %1<br/>",Total,curshow).arg(total)
                                                 +checkSES("%1",InfoDate,curshow).arg(timestamp));






                                  }
                              }
                          }

                      }
                      else {
                          if (timestamp == tr("n/a") ) {
                              // double diffporc = stats.at(2).toDouble(&ok);
                              if ( ntokens > 0 ) {
                                  myinfo =  (checkSES("Porcentaje: %1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                             +checkSES("Nro Documentos: %1<br/>",Tokens,curshow).arg(tokens)
                                             +checkSES("Total: %1<br/>",Total,curshow).arg(total)
                                             +checkSES("%1",InfoText,curshow).arg(rol));




                              }
                              else  {
                                  if ( !showheretokens ) {
                                      myinfo =  (checkSES("Porcentaje: %1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                                 +checkSES("Nro Documentos: %1<br/>",Tokens,curshow).arg(tokens)
                                                 +checkSES("Total: %1<br/>",Total,curshow).arg(total)
                                                 +checkSES("%1",InfoText,curshow).arg(rol));





                                  }
                              }



                          }
                          else {
                              if (ntokens > 0 ) {
                                  myinfo =  (checkSES("Porcentaje: %1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                             +checkSES("Nro Documentos: %1<br/>",Tokens,curshow).arg(tokens)
                                             +checkSES("Total: %1<br/>",Total,curshow).arg(total)
                                             +checkSES("%1<br/>",InfoText,curshow).arg(rol)
                                             +checkSES("%1",InfoDate,curshow)
                                             .arg(timestamp));


                              }
                              else  {
                                  if ( !showheretokens ) {
                                      myinfo =  (checkSES("Porcentaje: %1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                                 +checkSES("Nro Documentos: %1<br/>",Tokens,curshow).arg(tokens)
                                                 +checkSES("Total: %1<br/>",Total,curshow).arg(total)
                                                 +checkSES("%1<br/>",InfoText,curshow).arg(rol)
                                                 +checkSES("%1",InfoDate, curshow)
                                                 .arg(timestamp));

                                  }
                              }

                          }
                      }

                  }
                  else if (stats.count() > 2 && !stats.at(2).trimmed().isEmpty()) {
                      //double diffporc = stats.at(2).toDouble(&ok);
                      if (ntokens > 0 ) {
                          myinfo = (checkSES("Porcentaje: %1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                    +checkSES("Nro Documentos: %1<br/>",Tokens,curshow).arg(tokens)
                                    +checkSES("Total: %1",Total,curshow).arg(total));

                      }
                      else  {
                          if ( !showheretokens ) {
                              myinfo =  (checkSES("Porcentaje: %1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                         +checkSES("Nro Documentos: %1<br/>",Tokens,curshow).arg(tokens)
                                         +checkSES("Total: %1",Total,curshow).arg(total));
                          }
                      }


                  }
                  else {

                      if (ntokens > 0 ) {
                          myinfo =  (checkSES("%1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                     +checkSES("Tareas: %1<br/>",Tokens,curshow).arg(tokens));
                                   //  +checkSES("Total: %1",Total,curshow).arg(total));

                      }
                      else  {
                          if ( !showheretokens ) {
                              myinfo =  (checkSES(" %1%<br/>",Porc,curshow).arg(porc*100,0,'f',2)
                                         +checkSES("Tareas: %1<br/>",Tokens,curshow).arg(tokens));
                                         //+checkSES("Total: %1",Total,curshow).arg(total));




                          }
                      }



                  }
                  if (lastfield.contains("...")) {
                      if (!myinfo.isEmpty()) {
                          if (myinfo.indexOf(tr("AQUI")) != -1 ) {
                              extranode = nameextranode +QString(" [style=\"dotted\" "
                                                                 " penwidth=\"4.0\"  "
                                                                 "color=\"black\" "
                                                                 " label=<%1>  "
                                                                 "shape=\"plaintext\" fontcolor=\"%2\" fillcolor=\"%3\"];\n")
                                      .arg( myinfo)
                                      .arg(statsfontcolor)
                                      .arg(statsfillcolor);
                              extranode += nameextranode + " -> " + newnode  + " [penwidth=\"4.0\" style=\"solid\"];\n";

                          }
                          else {
                              extranode = nameextranode +QString(" [style=\"%4\" "
                                                                 " label=<%1>"
                                                                 "shape=\"box\" fontcolor=\"%2\" fillcolor=\"%3\"];\n")
                                      .arg( myinfo)
                                      .arg(statsfontcolor)
                                      .arg(statsfillcolor)
                                      .arg(statsstyle);
                              extranode += nameextranode + " -> " + newnode  + " [style=\"dotted\"];\n";
                          }
                          //qDebug("...extranode. %s",qPrintable(extranode));
                      }
                  }

              } // if (extra) ...
              if (!mynote.isEmpty() ) {
                  notenode = newnode+"_note" +QString(" [style=\"filled\" "
                                                      " penwidth=\"1.0\"  "
                                                      "color=\"black\" "
                                                      " label=<%1>  "
                                                      "shape=\"note\" fontcolor=\"%2\" fillcolor=\"lightgray\"];\n")
                          .arg( mynote)
                          .arg(statsfontcolor);
                  notenode += newnode+"_note"  + " -> " + newnode  + " [penwidth=\"1.0\" style=\"solid\"];\n";



              }
          } // fillcolor ...
          // *** Patrones (JOIN / SPLIT )
          if ( patron.compare("none") == 0 ) {
              if (patternunion.compare("none") == 0 ) {
                  label = newnode + (desc.length()==0?"":"\\n"+desc) + (myrole.length()==0?"":"\\n*"+myrole+"*")+ (mytextual.length()==0?"":"\\n"+mytextual+"");
              } else {
                  label = "{ " + patternunion + " | " + newnode +"\\n"+ desc + (myrole.length()==0?"":"\\n*"+myrole+"*") + (mytextual.length()==0?"":"\\n"+mytextual+"") + "}";
              }
          }
          else {
              if (patternunion.compare("none") == 0 ) {
                  label = "{ " + newnode +(desc.length()==0?"":"\\n"+desc) + (myrole.length()==0?"":"\\n*"+myrole+"*") + (mytextual.length()==0?"":"\\n"+mytextual+"")  +  " | " + patron + "}";
              }
              else {
                  label = "{ " + patternunion + " | " + newnode +(desc.length()==0?"":"\\n"+desc) + (myrole.length()==0?"":"\\n*"+myrole+"*") + (mytextual.length()==0?"":"\\n"+mytextual+"") +  " | " + patron + "}";
              }
          }

          // *** Patrones (JOIN / SPLIT )
          rx.setPattern("Siguiente:\\s*([a-zA-Z\\s0-9;\\+_\\.]+)\\s*\\,");
          pos = rx.indexIn( sec );
          Q_ASSERT ( pos >= 0 );
          QString next = rx.cap(1);
          QStringList nexts =  next.split(";", QString::SkipEmptyParts);
          rx.setPattern("Tipo:\\s*([a-zA-Z\\s0-9;_\\.]+)\\s*\\,");
          pos = rx.indexIn( sec );
          Q_ASSERT ( pos >= 0 );
          QString typeNode = rx.cap(1);


          if (typeNode == "Task" ) {
              result += newnode + " [style="+context[conf()+"task.style"]+
                      ",image=\"cloud.ps\""+
                      ", fontname=\"Helvetica\""+
                      ", shape="+taskshape+
                      ", label=\""+label+"\""
                      +scolor+" ,color=\""+
                      processColor(context[conf()+"task.color"]) +"\""+ ssize +"];\n";
          } else if ( typeNode == "Condition" ) {
              result += newnode
                      + " [style="+context[conf()+"condition.style"]+
                      ", shape="+context[conf()+"condition.shape"]+
                      ",fillcolor=\""+
                      processColor(context[conf()+"condition.fillcolor"]) +
                      "\",color=\""+ processColor(context[conf()+"condition.color"])
                      +"\"];\n";
          }
          foreach( QString s, nexts ) {
              result += newnode + " -> " + s + ";\n";
          }
          if (isextra ) {
              result += extranode;
              extranode = "";
          }
          if (!mynote.isEmpty() ) {
              result += notenode;
              notenode = "";
          }

      }
      result.chop(1);
      //result += sec;
      result += "\n";
      result += "}";
      return result;
 }



QString graphvizPlugin::renderGraph(const QString&  code, const QString& info,
                                    const QMap<QString,QString>& context)  {
    if ( info.isEmpty() ) return QString(tr("ERROR informacion NO se suministro"));

    bool isonlycode = false;
    if ( context[conf()+"onlycode"] ==  "yes" ) {
        isonlycode = true;
    }

    QLocale::setDefault(QLocale::C);
    setlocale(LC_ALL, "C");
    QStringList list= getTempNameFiles(2);
    Q_ASSERT( list.count() == 2 );

    _typegraph = context[conf()+"graphtype"];
    GVC_t *gvc; graph_t *g; FILE *fpin, *fpout;

    QString infile, outfile;
//    infile = "/home/victorrbravo/django/media/archivos/"+list.at(0).section("/",-1);
//    outfile = "/home/victorrbravo/django/media/archivos/"+list.at(1).section("/",-1)+"."+_typegraph;

    QString keyinfile = conf()+"infile";
    QString keyoutfile = conf()+"outfile";

    if ( context.contains(keyinfile) ) {
        qDebug("keyinfile:|%s|",qPrintable(keyinfile));
        infile = context[keyinfile]+ "/"+ list.at(0).section("/",-1);
    }
    else {
        infile = "/tmp/"+list.at(0).section("/",-1);
    }
    if ( context.contains(keyinfile) ) {
        qDebug("keyoutfile:|%s|",qPrintable(keyoutfile));
           outfile = context[keyoutfile] + "/" + list.at(1).section("/",-1)+"."+_typegraph;
    }
    else {
        outfile = "/tmp/"+list.at(1).section("/",-1)+"."+_typegraph;
    }

    qDebug("infile:|%s|",qPrintable(infile));
    qDebug("outfile:|%s|",qPrintable(outfile));

    gvc = gvContext();
    Q_CHECK_PTR( gvc );
    fpin = fopen (qPrintable(infile),"w+");
    if (fpin == NULL) {
        return QString(tr("ERROR abriendo archivo de entrada: \"%1\"")
                       .arg(infile));

    }
    fwrite( qPrintable(code), sizeof(char), code.length(), fpin);
    fflush(fpin);
    fclose(fpin);


    fpin = fopen (qPrintable(infile),"r");
    Q_CHECK_PTR(fpin);
    g = agread(fpin);
    fclose(fpin);
    char filetype[10];
    //strncpy(filetype,qPrintable(info), strlen(filetype));

    qDebug("................Graphviz...renderGraph GV_FILE_PATH:|%s|",getenv("GV_FILE_PATH"));

   

    if ( isonlycode ) {
        QString myname = list.at(1)+".dot";
                QFile file(myname);
                if ( !file.open(QIODevice::WriteOnly))  {
                    return QString(tr("ERROR abriendo archivo de entrada: \"%1\"")
                                   .arg(myname));
                }

         QTextStream out(&file);

         out << code;
        file.close();
        return myname;
    }
    else {
        strncpy(filetype,qPrintable(_typegraph),3);
        filetype[3] = 0;
        char nameengine[] = "dot";
        gvLayout(gvc, g, (char*)nameengine);
        fpout = fopen (qPrintable(outfile),"w+");
        if (fpout == NULL) {
            return QString(tr("ERROR abriendo archivo de salida: \"%1\"")
                           .arg(outfile));

        }
        gvRender(gvc, g,  filetype, fpout);
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);

    }

    fclose(fpout);

    return list.at(1);
}

QStringList  graphvizPlugin::getTempNameFiles(int n) {
     QStringList list;
     if (n == 0 ) return list;
     Q_ASSERT(n > 0 );

     QList<QTemporaryFile*> filelist;

     for (int i= 0; i < n; i++) {
          filelist.push_back(new QTemporaryFile());
          Q_CHECK_PTR( filelist.at(i) );
          bool result = filelist.at(i)->open();
          if (!result) {
             //   qDebug("No fue posible abrir el archivo temporal");
            }
          list.push_back(filelist.at(i)->fileName());
     }

     Q_ASSERT(n == filelist.count()); // Deben ser igual el numero de la lista de archivos
     for (int i= 0; i < n; i++) {
          filelist.at(i)->close();
          delete filelist.at(i);
     }
     return list;
}


Q_EXPORT_PLUGIN2(pnp_graphviz, graphvizPlugin)
