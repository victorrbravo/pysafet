/****************************************************************************
**
** Copyright (C) 2006-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
/*
* SAFET Sistema Automatizado para la Firma Electrï¿½nica y Estampado de Tiempo
* Copyright (C) 2008 Vï¿½ctor Bravo (vbravo@cenditel.gob.ve), Antonio Araujo (aaraujo@cenditel.gob.ve
*
* CENDITEL Fundacion Centro Nacional de Desarrollo e Investigaciï¿½n en TecnologÃ­as Libres
*
* Este programa es software libre; Usted puede usarlo bajo los tï¿½rminos de la licencia de
* software GPL versiï¿½n 2.0 de la Free Software Foundation.
*
* Este programa se distribuye con la esperanza de que sea ï¿½til, pero SI NINGUNA GARANTï¿½A;
* tampoco las implï¿½ï¿½citas garantï¿½ï¿½as de MERCANTILIDAD o ADECUACIÃ“N A UN PROPï¿½SITO PARTICULAR.
* Consulte la licencia GPL para mï¿½s detalles. Usted debe recibir una copia de la GPL junto
* con este programa; si no, escriba a la Free Software Foundation Inc. 51 Franklin Street,
* 5ï¿½ Piso, Boston, MA 02110-1301, USA.
*
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// #include <QMainWindow>

#include <QTextStream>
#include <QSettings>
#include <QGraphicsSvgItem>
#include <QMutex>
#include <QWaitCondition>
//#include <QApplication>
//#include <QTcpServer>
//#include <QStackedWidget>

#include "SafetYAWL.h"

//#include "graphicsworkflow.h"
//#include "graphicssvgitemlive.h"
#include "threadconsole.h"
#include "threadinfoserver.h"
//#include "SafetConsoleApp.h"
//#include "principalframe.h"
#include "SafetTextParser.h"
//#include "DigiDoc.h"
#include "SafetDocument.h"
//#include "QProgressIndicator.h"
//#include "inflowthread.h"
#include "threadconsole.h"



class QAbstractItemModel;
class QComboBox;
class QCmdCompleter;
class QLabel;
class QLineEdit;
class QProgressBar;
class QDomDocument;
class DomModel;
class ThreadConsole;
//class GraphicsWorkflow;
class GraphicsSvgItemLive;
class InflowThread;
//class ThreadConsole;


const QString SAFETCONSOLEFILE = "defconsole.xml";
const QString  SAFETCONFIGFILE = "defconfigure.xml";
const QString  SAFETUSERSFILE = "defusers.xml";
const QString  SAFETMANAGEMENTSIGNFILE = "defmanagementsignfile.xml";

namespace Safet {
    const QString LOADFLOWFILE_NAMEFIELD = QObject::tr("Cargar_archivo_flujo");
    const QStringList STRINGLISTEMPTY;
const QString AUTOFILTERFLOW_NAMEFIELD = QObject::tr("Autofiltro");
const QString RECURSIVEFILTERFLOW_NAMEFIELD = QObject::tr("Filtro_recursivo");
const QString GRAPHSFILENAME = "graphs";
const QString PLANNEDGRAPHSFILENAME = "plannedgraphs";

const QString COLONMARK = "##SAFETCOLON##";
const QString AMPMARK_ = "__SAFETAMP__";
const QString COLONMARK_ = "__SAFETCOLON__";
const QString COMMAMARK = "##SAFETCOMMA##";
const QString BRACKETOPENMARK = "##SAFETBRACKETOPEN##";
const QString BRACKETCLOSEMARK = "##SAFETBRACKETCLOSE##";
const QString MPIPEMARK = "##SAFETPIPE##";

const QString ALL = "Safet::all";

const int NOKEY = 10;
const QString EMAIL_PATTERN =
                   QString("[_A-Za-z0-9-\\+]+(\\.[_A-Za-z0-9-]+)*@")
                   + "[A-Za-z0-9-]+(\\.[A-Za-z0-9]+)*(\\.[A-Za-z]{2,})";
const QString TEMPLATEFLOWFILE = ""
        "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
        "<!--\n"
        "Documento  : tickets.xml\n"
        "Creado     : 16/10/08 09:27 AM\n"
        "Autor      : nombre_autor\n"
        "Descripcion: Archivo generado por plantilla de la Libreria SAFET\n"
        "-->        \n"
        "<!--<!DOCTYPE yawl SYSTEM \"http://trac-safet.cenditel.gob.ve/dtd/yawlworkflow.dtd\">-->\n"
        "<!DOCTYPE yawl SYSTEM \"file:///home/vbravo/.safet/dtd/yawlworkflow.dtd\">\n"
        "<yawl  version=\"0.01\">\n"
        "<workflow id=\"%2\">\n"
        "        <token key=\"id\"  keysource=\"actividad_por_accion\">\n"
        "        </token>\n"
        "<parameter type=\"combolistliteral\" options=\"TB::La dirección del gráfico es arriba-abajo::TB,LR::La dirección del gráfico es izquierda-derecha::LR\" mandatory=\"no\"\n"
        "                       title=\"Direccion_Grafico_de_Flujo\" configurekey=\"Plugins.Graphviz/plugins.graphviz.graph.rankdir\">\n"
        "                </parameter>\n"
        "<parameter type=\"combolistliteral\" options=\"svg::Gráfico SVG::svg,png::Gráfico PNG::png\" mandatory=\"no\"\n"
        "                       title=\"Formato_del_Grafico\" configurekey=\"Plugins.Graphviz/plugins.graphviz.graphtype\">\n"
        "                </parameter>\n"
        "<parameter type=\"combolistliteral\" options=\"on::Incluir::on,off::No Incluir::off\" mandatory=\"no\"\n"
        "                       title=\"Incluir_Fecha_Hora\" configurekey=\"Graphs/graphs.infotext.include\">\n"
        "                </parameter>\n"
        "<parameter type=\"string\" options=\"\" mandatory=\"no\"\n"
        "                       title=\"Titulo\" configurekey=\"GeneralOptions/generaloptions.currentflowtitle\">\n"
        "                </parameter>\n"
        "<parameter type=\"combocolor\" mandatory=\"no\" title=\"Color Para Estadistica\"\n"
         "                       options=\"antiquewhite::Color Blanco Antiguo::antiquewhite,green::Color Verde::green,coral::Color Rojo Coral::coral,red::Color Rojo::red,yellow::Color Amarillo::yellow,skyblue::Color Azul Cielo::skyblue,ghostwhite::Color Blanco Fantasma::ghostwhite,Blanco::Color Blanco::white,lightgray::Color Gris Claro::lightgray,blue::Color Azul::blue,orange::Color Naranja::orange,gray::Color Gris::gray,black::Color Negro::black,white::Color Blanco::white\"\n"
        "configurekey=\"Plugins.Graphviz/plugins.graphviz.stats.colorgradient\" defaultvalue=\"Amarillo\">\n"
        "                </parameter>\n"
        "        <condition id=\"inicial\" type=\"start\">\n"
        "                <port side=\"forward\" type=\"split\"   >\n"
        "                         <connection source=\"%1\" query=\"select tipo from actividad_por_accion\"  options=\"%1\" >\n"
        "                        </connection>\n"
        "                </port>\n"
        "        </condition>\n"
        "        <task id=\"%1\" report=\"no\">\n"
        "                <port side=\"forward\" type=\"split\" >\n"
        "                        <connection source=\"final\" query=\"true\" options=\"\" >\n"
        "                        </connection>\n"
        "                </port>\n"
        "                <variable id=\"v%1\" scope=\"task\"  type=\"sql\" config=\"1\"   tokenlink=\"\" documentsource=\"select nombre,status from actividad_por_accion\" >\n"
        "                </variable>\n"
        "        </task>\n"

        "<condition id=\"final\">\n"
        "                <port side=\"forward\" type=\"split\">\n"
        "                        <connection source=\"\">\n"
        "                        </connection>\n"
        "                </port>\n"
        "        </condition>\n"
        "</workflow>\n"
        "        <configuration id=\"1\" connectstring=\"psql:seguridad.cenditel.gob.ve:ticket:vbravo:vbravo_\">\n"
        "        </configuration>\n"
        "</yawl>\n";

const QString TEMPLATELEGEND = ""
        "  <g\n"
        "     id=\"layer1\">\n"
        "    <rect\n"
        "       width=\"250.03214\"\n"
        "       height=\"165.03214\"\n"
        "       x=\"-258.01605\"\n"
        "       y=\"1.3461177\"\n"
        "       id=\"rect4073\"\n"
        "       style=\"fill:#ffffff;fill-opacity:1;stroke:#000000;stroke-width:0.36787015;stroke-linecap:round;stroke-linejoin:miter;stroke-miterlimit:4;stroke-opacity:1;stroke-dasharray:none\" />\n"
        "    <g\n"
        "       transform=\"translate(-260,-1)\"\n"
        "       id=\"g4014\">\n"
        "      <rect\n"
        "         width=\"32.564808\"\n"
        "         height=\"25.06481\"\n"
        "         x=\"14.379484\"\n"
        "         y=\"17.260754\"\n"
        "         id=\"rect4995\"\n"
        "         style=\"fill:#ff0000;stroke:#000000;stroke-width:3.75\" />\n"
        "      <text\n"
        "         x=\"59.411888\"\n"
        "         y=\"34.793159\"\n"
        "         id=\"text4997\"\n"
        "         xml:space=\"preserve\"\n"
        "         style=\"font-size:20px;font-style:normal;font-weight:normal;fill:#000000;fill-opacity:1;stroke:none;font-family:Bitstream Vera Sans\"><tspan\n"
        "           x=\"59.411888\"\n"
        "           y=\"34.793159\"\n"
        "           id=\"tspan5001\">Tarea con atraso</tspan></text>\n"
        "      <g\n"
        "         transform=\"matrix(1.25,0,0,1.25,-63.16174,-32.682296)\"\n"
        "         id=\"g5072\">\n"
        "        <rect\n"
        "           width=\"26.051847\"\n"
        "           height=\"20.051847\"\n"
        "           x=\"61.254543\"\n"
        "           y=\"65.974075\"\n"
        "           id=\"rect4995-6\"\n"
        "           style=\"fill:#ffff00;stroke:#000000;stroke-width:3\" />\n"
        "        <text\n"
        "           x=\"97.280472\"\n"
        "           y=\"80\"\n"
        "           id=\"text4997-5\"\n"
        "           xml:space=\"preserve\"\n"
        "           style=\"font-size:16px;font-style:normal;font-weight:normal;fill:#000000;fill-opacity:1;stroke:none;font-family:Bitstream Vera Sans\"><tspan\n"
        "             x=\"97.280472\"\n"
        "             y=\"80\"\n"
        "             id=\"tspan5001-8\">Tarea proxima</tspan></text>\n"
        "      </g>\n"
        "      <g\n"
        "         transform=\"matrix(1.25,0,0,1.25,-62.38259,-31.45837)\"\n"
        "         id=\"g20612\">\n"
        "        <rect\n"
        "           width=\"26.051847\"\n"
        "           height=\"20.051847\"\n"
        "           x=\"60.988918\"\n"
        "           y=\"91.974075\"\n"
        "           id=\"rect4995-6-9\"\n"
        "           style=\"fill:#ffffff;stroke:#000000;stroke-width:3\" />\n"
        "        <text\n"
        "           x=\"97.014847\"\n"
        "           y=\"106\"\n"
        "           id=\"text4997-5-6\"\n"
        "           xml:space=\"preserve\"\n"
        "           style=\"font-size:16px;font-style:normal;font-weight:normal;fill:#000000;fill-opacity:1;stroke:none;font-family:Bitstream Vera Sans\"><tspan\n"
        "             x=\"97.014847\"\n"
        "             y=\"106\"\n"
        "             id=\"tspan5001-8-0\">Tarea lejana</tspan></text>\n"
        "      </g>\n"
        "      <g\n"
        "         transform=\"matrix(1.25,0,0,1.25,-58.81405,7.112185)\"\n"
        "         id=\"g20612-2\">\n"
        "        <line\n"
        "           x1=\"57\"\n"
        "           y1=\"115\"\n"
        "           x2=\"87\"\n"
        "           y2=\"87\"\n"
        "           style=\"stroke:#000000;stroke-width:3\"\n"
        "           id=\"line20723\" />\n"
        "        <text\n"
        "           x=\"97.014847\"\n"
        "           y=\"106\"\n"
        "           id=\"text4997-5-6-5\"\n"
        "           xml:space=\"preserve\"\n"
        "           style=\"font-size:16px;font-style:normal;font-weight:normal;fill:#000000;fill-opacity:1;stroke:none;font-family:Bitstream Vera Sans\"><tspan\n"
        "             x=\"97.014847\"\n"
        "             y=\"106\"\n"
        "             id=\"tspan5001-8-0-3\">Tarea ejecutada</tspan></text>\n"
        "      </g>\n"
        "    </g>\n"
        "  </g>\n"

        ;
}



class MainWindow : public QObject
{

    Q_OBJECT



    QString _currenterror;

//    GraphicsWorkflow *gviewoutput;

    QString _dirInput; // Directorio de archivos imÃ¡genes, iconos, archivos utilitarios

    // *** Valores de formato para listado y grÃ¡fico de flujo
    QString _listprincipaltitle;    
    int _listprincipalcount;
    QString _listprincipalvariable;
    QString _listprincipalkey;

    // *** Valores de formato para listado y grÃ¡fico de flujo
    QString  _pathdoc; // Archivo de entrada para generar la consulta
    QString gopt;
    int verbose_flag;
    bool parsed;

    QMap<int,QString> commands;
    //QMap<QString,QString> _permiseshierarchy; // jerarquia de permisos

    QStringList filecompleters;
    QStringList _plugs;

    GraphicsSvgItemLive* _itemredraw;

    QString _currenttable;
    friend class ThreadConsole;

    bool initApp; // para iniciar la aplicacion
    bool _issmartmenuvisible;

     bool runningConsole;



    // Cadena que mantiene la ruta completa de un documento SafetDocument
    // cargado con SafetDocument::readDigidocFile
    QString pathOfSafetDocument;




    // indicador de progreso
    //QProgressIndicator* progressIndicator;


    /** variable para thread
    *
    */
    //static ThreadConsole myThreadConsole;


    QMap<QString,QStringList> users;



    QString _currentjson;
    QString _currentdata;

    QString _inputpath;
    QString _hostmediapath;
    QString _hosturl;
     uint _wfnumpars;


public:

    // Estructura utilitaria para listar documentos por clave



    // indicador de progreso
//    QProgressIndicator* progressIndicator;

    // Enumeraciones


    // Datos estaticos
    static SafetYAWL *configurator;
    static MainWindow *mymainwindow;
    static QStringList sbmenulist;

    static bool _issigning;
    static QString _currentCommonName;

    static QString showString;
    QString _templatepath;


    QString _currconfpath; // Ruta de acceso para el directorio .safet p.e. /home/vbravo
    QString _mediapath;




    static QMap<QString,QPair<QString,QString> > _graphs;

    // Primer campo: nombre de la operacion
    // Primer elemento del QStringList corresponde a los usuarios para esa operaciÃ³n
    // Segundo elemento del QStringList corresponde a las acciones que pueden ejecutar los usuarios
    // Tercer elemento del QStringList corresponde al SHA1 (firma de la operacion)

    static QMap<QString,QStringList> permises; // Estructura para guardar los permisos

    static QString currentaccount; // cuenta actual de usuario
    static QString currentrole; // rol de la cuenta actual de usuario
    static QString currentrealname;
    // Funciones
    /**
      \brief ejecuta un funcion de safet segun los parametros que se encuentran en memoria
      (como consola)
     \return retorna true si se ejecutÃ³ correctamente, false en caso contrario
     */
    bool executeParsed();
    /**
      \brief Retornando JSON Actual
      */
    QString currentJSON();
    /**
      \brief Retornando los datos actuales en formato JSON
      */

    QString currentDATA() const;
    QString currentADATA() const;

    int currentCOUNT() const { return _listprincipalcount; }



    QString currentTable();

    /* Carga una configuracion */

    void doLoadConfiguration(const QString &fileName);

    /* Guarda una configuracion */

    void doSaveConfiguration(const QString& fileName);
    /**

      Retorna la ruta completa del archivo de entrada que se está procesando
      */
    QString inputPath() { return _inputpath;}
    void setInputPath(const QString& p) { _inputpath = p;}

    QString hostURL();
    void setHostURL(const QString& u);

    bool parse(int &argc, char **argv);

    QString postAction();
    void refreshListView(const QString& doc);
    void buildModel(QMap<QString,QVariant>& mymap);

    QStringList& getFileCompleters() { return filecompleters;}
    /*
      \brief Funcion que revisa la pila de errores y devuelve Verdadero (true)
             si existen errores. TambiÃ©n muestra el panel de errores con los mensajes de errores
      \return Verdadero (true) si existen errores en la pila, Falso (false) de otra forma
      */
    bool queryForErrors();

public:
    MainWindow(const QString& path = QLatin1String(""));
    ~MainWindow();
    bool parseArgs(const QString& a);
    void setupToolbar();
    void setupTabWidget();

   QString currentError();
   void loadAuthorization(); 
   void loadInfo();
   
   bool login(const QString& name, const QString& pass);
   bool logout(); 

   void setMediaPath(const QString& m);
   QString mediaPath();

   void setTemplatePath(const QString& t) { _templatepath = t; }
   QString templatePath() { return _templatepath;}

   void setHostMediaPath(const QString& m);
   QString hostMediaPath();




    /**
     ** \brief Obtiene la plantilla de estilo principal
     * \return Plantilla de estilo en formato CSS
     */

    QString getPrincipalCSS() const;


    /**
     * @brief sendEmail Envia un email utilzando el sistema local
     * @param recipients
     * @param subject
     * @param message
     */

     void sendEmail(const QString &recipients, const QString &subject, const QString &message);
    /**
    * \brief Borra todos los archivos que se encuentran en el directorio d
      \param d Directorio donde se eliminarÃ¡n los archivos
    */
    static void doDelAllFilesOnDirectory(const QString& d);
    /**
    * \brief Funcion delegada (Callback) para gestionar un error
    */
    static void evalEventOnExit(SafetLog::Level);


    /**
     * @brief normalize Cambia los acentos
     * @param text cadena de texto con acentos
     * @return cadena de texto sin acentos
     */
    static QString normalize(const QString &text);

    /**
      * \brief Retorna el mapa de los grafos visibles (nombre, codigo para ser generado)
      */
    static QMap<QString,QPair<QString,QString> >& graphs() { return _graphs; }

    /**
     * \brief Evalua si la firma de una operacion es vÃ¡lida segun un determinado permiso
              (read,view,modify)
        \param op Firma de la operaciÃ³n
        \param permise Tipo de permiso (read,view,modify);

     */
     static bool doPermiseExecOperation(const QString& op, QMap<QString,QString>& phierarchy,
                                        const QString& permise = "read");

     /**
      * \brief Evalua si la firma de una operacion es vÃ¡lida segun un determinado permiso
               (read,view,modify)
         \param op Firma de la operaciÃ³n
         \param permise Tipo de permiso (diferentes tipos de lecturay);
         \return mapas de tipo de accion (claves) -> permiso para el usuario actual

      */
     static QMap<QString,bool> doPermiseExecOperationAction(const QString& op);

        /*
          *  \brief Coloca el conjunto de comandos para ser utilizado en la consola
          * \param cmds conjunto de comandos
          */
        void setCommands(QMap<int,QString>& cmds) { commands = cmds;}

       /*
         * \brief Coloca el item a redibujar
         * \brief item a redibujar
         */
        void setItemRedraw(GraphicsSvgItemLive* i = 0) { _itemredraw = i; }
    /**
    * \brief Funcion delegada (Callback) para gestionar la entrada
    */
    static QString evalEventOnInput(SafetYAWL::TypeInput,const QString&, bool&);

     // Metodos para la Consola
    void listTasks();
    void listDocuments(const QString& key);
    void listDocuments();
    void manageData();
    bool genGraph();
    void signDocument();
    void calStatistics();
    /**
        * \brief Muestra la ayuda del uso de la aplicacion de consola safet
        */
    void displayUsage();

    void version();

    /*
          * \brief Genera una Consola (Shell)
          * \param command nÃºmero de parÃ¡metro
          * \return true si el resultado fue exitoso, falso en caso contrario
          */
    bool  processCommand(int command);

    /**
       \brief Dada una sentencia SQL (sql) se procesa un archivo de texto ( por defecto se procesa safet.conf)
              modificando las diferentes opciones segï¿½n  la expresiï¿½n SQL
       \param sql Sentencia SQL (INSERT, UPDATE, etc..)
       \param filename ruta completa  del archivo de configuraciï¿½n, si no se especifica se toma por defecto safet.conf
       \param multiplefields Admite sustituciones de campo multiples terminados en ".*"
       \param numberreg NÃºmero de registro a cambiar
       */
    void proccessConfFile(const QString& sql, const QString& filename = QString("auth.conf"), bool multiplefields = false);
    void setCommands(const QMap<int,QString>& cmds) { commands = cmds; }
    void setModelCompleter(int opt);
    bool loadWidgetPlugins(const QString& f, bool add = false);
    void loadSettings();
    void writeSettings();
    QString evalJS(const QString &js);


    /**
      \brief Traduce el contenido de la pila "stack" a una cadena en formato HTML, en funcion
      de del tipo de mensaje denotado por "l"
      \param stack Pila de mensajes, al finalizar esta pila estara vacia
      \param l Tipo del mensaje (Error, Warning, Action,Debug)
      \return Cadena en formato HTML que contiene hora y texto de cada mensaje
      */
    QString renderMessageStack(QStack<QPair<QDateTime,QString> >& stack, SafetLog::Level l);

    /**
      * Verificaciones
      */
    void successVerification(QStringList list = QStringList(), const QString& msg = "");

    /** Funcion agregada para eliminar dependencia de digidocpp realiza la verificacion
    * de un contenedor. Se utiliza para desplegar el widget SafetSignatureWidget
    * \param list lista de cadena
    * \param msg mensaje
    * \param doc objeto SafetDocument asociado al contenedor
    */
    void successVerification(QStringList list, const QString& msg, SafetDocument doc);


    /**
      Copia los archivos iniciales para el directorio "home" / .safet
      **/
    void copyInitialFiles();



    /**
      \brief Cifrar o descifrar  un archivo indicado por pathname
      \param pathname ruta completa del archivo a cifrar
      \param operation 0 cifrar y 1 para descifrar
      */
    void doCipherFile(bool iscipher = false);


    /**
      \brief Comparar dos grÃ¡ficos (first,second) y generar un tercero con la diferencia de fichas
             entre los dos
      \param first Fuente del primer grÃ¡fico
      \param second Fuente del segundo grÃ¡fico
      \return Fuente del grÃ¡fico con la estadÃ­stica
      */
    QString generateGraphCompared(const QString& first, const QString& second, QStringList& taskmark);

    /**
      \brief Comparar dos grÃ¡ficos (first,second) y generar un tercero con la diferencia de fichas
             entre los dos
      \param first Fuente del primer grÃ¡fico
      \param second Fuente del segundo grÃ¡fico
      \return Fuente del grÃ¡fico con la estadÃ­stica
      */
      QString generateTableCompared(const QString &first,const  QString &second, QStringList &taskmark);

    /**
      \brief Dibuja el grÃ¡fico especificado por "code" en la pestana de flujos de trabajos (GrÃ¡fico)
      y le agrega la etiqueta de fecha "datetag"
      \param code CÃ³digo del grÃ¡fico a dibujar
      \param datetag Etiqueta de la fecha
      */
    void doRenderGraph(const QString& code, const QString& datetag);

    /**
      \brief Habilita/Deshabilita los botones enviar de las pestanas una vez que se conecta
      a la fuente de datos correctamente
      **/
    void setEnableCompletingButtons(bool b);




    /**
      \brief Carga la plantilla que se muestra en la pestaÃ±a de "Reportes". Buscar la  etiqueta <SAFET/>
      \param json texto a reemplizar
      \param filename ruta completa de la plantilla
      */
    QString loadReportTemplate(const QString& json, const QString &filename = QString(""),
                               const QString& nameoperation = QString("Listar_datos"));



    QStringList autoComplete(const QString &path);
    /**
      * \brief Obtener una cadena HTML, con el menú para acceder
        a las operaciones de Formulario
        */
    QString menuCommands();

    /**
      \brief Obtiene el formulario para la operación especifica como "o"
      */
    QString menuForm(const QString &o, bool forwidget = false, const QString& fieldlist = "" );
    /**
      \brief Obtiene algunos valores de un formulario HTML de la operacion "o" filtrado por
            clave keyvalue
      */
    QString formFieldsForKey(const QString &o, const QString& fieldname,
                             const QString &keyvalue, DomModel* mymodel, const QString& otherkey = "",
                             const QStringList& form = Safet::STRINGLISTEMPTY );
    /**
      \brief genera el texto HTML Cabecera correspondiente a la operación que señalada en "o"
      */
    QString generateFormHead(const QString &o);

    /**
      \brief genera el texto HTML de final de página correspondiente a la operación que señalada en "o"
      */
    QString generateFormFooter(const QString &o);
    /**
      \brief Generar una cadena en lenguaje HTML que contiene el registro encontrado según los parametros
      operation, fieldname y key
      Devuelve una cadena vacía si no consigue el valor
      */
    QString generateModifyHTML(const QString &operation, const QString &fieldname, const QString &key,
                               const QString& secondkey, const QStringList& form);
    /**
      * \brief Agrega el texto HTML para los parámetros según la operación "nameoperation"
      * \return texto HTML con cuadro  de diálogo basado en jquery.ui
      */
    QString addParametersDialog(const QString &nameoperation);
    /**
      \brief Devuelve valor interno (innerHTML, JQuery) para el cuadro diálogo de parámetros de SAFET

      */
    QString getFlowParameters(const QString &flowfilename);
    QString getWidget(SafetParameter *p); // Obtener el objeto HTML del widget según el parámetro
    /**
      * \brief Cargar las acciones o Consultas guardardas
      * \return Lista de acciones
      */
    QMap<QString,QString> loadEditActions();

    /**
      * \brief Genera un menú con los enlaces a los módulos principales
      */
    QString generateModulesMenu();

    /**
      * \brief Convierte una operacion del tipo operacion:nombre_n... a un título
               Sin caracteres "_" y sin la palabra "operacion:"
               */
    static QString convertOpToTitle(const QString &op);
    QString returnFormLink(const QString& nameoperation); // "Retornar a Consulta"

    /**
      \brief Registrar el ingreso de un usuario al sistema
      \param user nombre del usuario
      */
    void registerLogin(const QString &user);

    /**
      \brief Registrar la salida de un usuario al sistema
      \param user nombre del usuario
      */
    void registerLogout(const QString &user);

    /**
      * \brief Escribir un mensaje (message) en el registro de eventos
      */
    void log(const QString& message);
    /**
      ** \brief reemplaza marcas como ##SAFETCOLON## (Safet::COLONMARK)
      */
    static QString replaceMarks(const QString &s);

    /**
      * \brief
      * \return Retorna dos parámetros, el primero el código del grafo,
            el segundo la hora y fecha que se realizó el grafo
            */
    QStringList lastInfoGraph();
    /**
      * \brief  Borra un gráfico del archivo por defecto (graphs.gph)
      * \param n nombre del gráfico a borrar
      * \return true si el gráfico fue eliminado, false en caso contrario
      */
    bool deleteGraph(const QString &n, const QString& gfn = Safet::GRAPHSFILENAME);

    /**
      * \brief  Guarda lo que está contenido en en el mapa estático graphs()
      * \param gfn nombre del repositorio de grafos (sin extensión)
      * \return true si el gráfico fue eliminado, false en caso contrario
      */
     bool doSaveCompleteMapGraphs(const QString &gfn = Safet::GRAPHSFILENAME);
    /**
      \brief Crea un registro de grafo en el archivo de grafos planificados (~/.safet/graphs/plannedgraphs.gph)
      \param pathflow ruta completa del archivo de flujo de trabajo
      \param name Nombre del grafo
      \return true si la acción fue exitosa, false en caso contrario
      */
    bool doGeneratePlannedGraph(const QString &pathflow, const QString &name, const QString& idactivity = "-1");

    /**
      \brief Modifica la fecha de un codigo de grafo
      */
    QString doModifyDateGraph(const QString &nametask, const QDateTime &mydate, const QString &code);

    /**
      *  \brief Obtener un mapa con las tareas y fecha del grafo guardado con nombre "namegraph"
      */
    QMap<QString, QDateTime> doGetPlannedGraph(const QString &namegraph);
    /**
      Cargar jerarquia de permisos
      */

    static QMap<QString,QString> loadPermisesHierarchy();
    /**
      \brief chequea si el permiso tiene jerarquia
      */
    static bool checkPermiseInHierarchy(const QString &permise, const QMap<QString, QString> &myphierarchy, const QString &realtypes);
    /**
      \brief Funciones para la gestion de archivos de SafetWorkflow
            add - agregar estado
            del - borrar estado
            modify - modificar estado

      */
     QString addNodeToXMLWorkflow(const QString &fname,
                                 const QString& beforenode = QString(),                                  
                                 const QString &nodename = QString("newnode"),
                                 bool isparallel = false,
                                 const QString &options = QString(),
                                 const QString &query = QString(),
                                 const QString &nodetitle = QString(),
                                 const QString &documentsource = QString()
                                  );


     /**
       \brief
       \param fname Nombre del flujo del trabajo
       \param nodename Nombre del nodo a eliminar
       \return nombre del archivo modificado (flujo de trabajo) generalmente "fname"
       */
     QString delNodeToXMLWorkflow(const QString &fname, const QString &nodename);

     /**
       \brief
       \param fname Nombre del flujo de trabajo
       \param currnode Nodo a cambiar la conexión
       \param nextnode proximo Nodo la conexión a cambiar
       \param newnode Nuevo Nodo a la que se cambiará la conexión
       \param  nombre del archivo modificado (flujo de trabajo) generalmente "fname"
       */
     QString changeConnXMLWorkflow(const QString &fname,
                                   const QString &currnode,
                                   const QString &nextnode,
                                   const QString &newnode,
                                   const QString& newoptions = QString(),                                   
                                  const QString&  newquery  = QString());

     /**
       * @brief changeRoleXMLWorkflow
       * @param fname Nombre del archivo de flujo de trabajo
       * @param currnode Nodo Actual
       * @param newrol  Nuevo Role ("role")
       * @return
       */

      QString changeRoleXMLWorkflow(const QString &fname, const QString &currnode, const QString &newrol,
                                    const QString& ATTRNAME = "role");
     /**
       \brief extrae los parametros de una consulta (todos los parametros que tenga como prefijo
       la palabra "parameters."
       */
     QString extractParameters(const QString &action);

      /**
        \brief Obtiene todas las fechas de una planificación
        \param code Código del grafo
        \return mapa con el nombre de la tarea y la fecha correspondiente
        */

     QMap<QString, QDateTime> getDatesGraph(const QString &code);


     bool delXmlMenuGroupForm(const QString &newff);


     bool addXmlMenuGroupForm(const QString &newff);

     void addShapeToSVG(const QString &filename, const QStringList &list);



     QString checkUserRegister(const QString &fullname, const QString &account,const  QString &email, const QString &passone,const  QString &passtwo);

     QStringList getInfoOfUser(const QString &user);

     bool sendCheckEmail(const QString &user,const QString &plink);
     QString generateCaptcha();

//     QString porc() const { return _porc; }
//     void setPorc(const QString& p) { _porc = p; }


      QStringList uncompresstar(const QString &filename);
      void compresstar(const QStringList &files, const QString &filename);

      /**
       * \brief Construir y enviar el email segun los parámetros colocados en el archivo safet.conf
       * \param data datos del formulario de envio
       * \param cs datos de la action (dataaction)
       * \return vacío
       */

      void buildEmail(QMap<QString, QString>& data, const QString &cs, QString& idkey);


      /**
       * @brief parseForEmail Reemplazar como campos de una variable de una tarea de un flujo una plantilla para email
       * @param t plantilla con campos del tipo {#campo}
       * @return  plantilla con los campos modificados
       */
      QString parseForEmail(const QString& t, const QString& idkey, QMap<QString,QString>& data,
                            const QString& cs);

      /**
       * @brief checkAndExecEvents
       *        Chequea los eventos pendientes en el directorio events (los archivos deben tener nombres
       *        event0,event1
       * @return
       */
      bool checkAndExecEvents();
      /**
       * @brief numberOfDocuments
       *          Cuenta el número de documentos en un flujo
       * @param t
       * @param idkey
       * @return
       */
      int numberOfDocuments(const QString &t, const QString &idkey, QString& currvalue);

      /**
       * @brief genTicket
       * Genera un número ticket y lo coloca al usuario indicado en el campo tickets
       * @param user Usuario indicado
       * @return
       */
      QString genTicket(const QString& user);


      QStringList generateTips(const QString &cs, const QString& parcialscript);

public Q_SLOTS: // Slots para el manejo de los servicios de Escritorio (QDesktopServices)

        void browse( const QUrl &url );
        void mailTo( const QUrl &url );

            void toSend(bool sign = false);


    bool createBdoc(const QString &content);
    QString toInputForm(const QString& action, bool withpermises = true);
    bool toInputConsole(const QString& action, bool withpermises = true);
    bool toInputUsers(const QString& action);

    void setParsValues(const QMap<QString, QString> &values);
    void evalParsValues(SafetWorkflow *wf);
    /**
      *\brief Colocar valores de configuración intermedios (Conffile)
      *\param values Valores según formato <Seccion>/<Clave> que se pasa directo al mapa de configuracion

      */
    void setConffileValues(const QMap<QString,QString>& values);

    /**
        \brief Agregar Fecha y Hora según formato
        \return Retorna Cadena con la fecha y/o hora
        */
    QString addInfoGraphDateText();

    /**
      \brief Devuelve el título actual colocado en el archivo safet.conf
      \return Retorna texto colocado en (generaloptions.currentflowtitle)
      */
    QString currentGraphTitle();

    /**
      \brief Funciones para guardar, restaurar y comparar grafos
      \param mypars recibe tres parametros para guardar en el archivo,
      el primer es el nombre del grafo, el segundo es el código del grafo y
      el tercero es la etiqueta de fecha.
      \return true si se logró guardar el grafo, false en caso contrario
      */

    bool doSaveGraph(const QStringList& mypars);

    static void doRestoreGraph(const QString& gfn = Safet::GRAPHSFILENAME);
    QString doCompareGraphs(const QString& firstgraph, const QString& secondgraph,  QStringList& taskmark);

private slots:

    void timeHideResult();

    void about();

    void toLoadWeb();

    void toInputSign();
    void toInputConfigure(const QString& mytexto = QString("") );
    void toDelAllWorkflow();
    void toDelOneWorkflow();
    void toClearTextEdit();
//    void selInputCombo(int);
    void checkSelInputTab(int opt);
    void selInputTab(int);
    void doGeneralOptions();
    void doWidgetsOptions();
    void setToInputReports();
    void setToInputFlowGraph();
    void setToInputManagementSignDocument();
    void doGetSignDocument();
    void doSendSignDocument();
    void checkGoPrincipal();

    void addToHistoryList();
    void editToHistoryList();
    void delToHistoryList();
    void saveToHistoryList();


    void showSmartMenu();
    void showSuccessfulMessage(const QString& m);

    QString drawWorkflow(const QString& filename);


    void toChangeUser();

    // establece la ruta completa de un documento SafetDocument
    // cargado con SafetDocument::readDigidocFile
    void setPathOfSafetDocument(QString path) { pathOfSafetDocument = path; }

    // retorna la ruta completa de un documento SafetDocument
    // cargado con SafetDocument::readDigidocFile
    QString getPathOfSafetDocument() { return pathOfSafetDocument; }

    void doInsertInAuthConfFile(QRegExp& rx); // Insertar un grupo de campos en una secciÃ³n
    /**
     \brief Busca los campos que coinciden con la clave key y devuelve los
            nombres separados por ","
     \param key Clave en el archivo de autorizaciÃ³n (auth conf) de la
     \return Campos separados por el caracter ","
     */
    QString searchFieldsInAuthConf(const QString& key);

public slots:
    void linkClickedSbMenu(const QUrl& url);
    void linkClickedSbResult(const QUrl& url);
    void setEnabledToolBar(bool e = true); // Habilitar o Desahabilitar barra de herramientas
    void doExit();
    /**
      \brief Funcion para preguntar antes de salir
      */
    bool maybeSave();
    /*
      * \brief va a la pantalla principal
      */
    void goPrincipal();







    // slot para desplegar el dialogo para modificar la configuracion de la fuente de datos
    void manageDataSources();



protected:
    void saveWindowState();
    void restoreWindowState();




    QString outputText;
    QTextStream streamText;
    void sendStreamToOutput();
    void iterateMap(const QMap<QString,QVariant>& map);
    void createDockWindow();    
    void createDockShowResultWindow();
    void buildMenuOnPanelsbMenu(const QString& option);


    /**
       *\brief Genera la lista de archivos (en "myfiles") para un paquete tar
       *\param folder Nombre de ruta completa de la carpeta para generar el archivo tar
       *\param myfiles parÃ¡metro de salida con la lista de archivos agregada
       *\param exts Lista de extensiones para cargar los archivos (xml,png,html,etc...)
       *\return nombre del archivo .tar
       */
    QString generateListForTar(const QString& folder, QStringList& myfiles,
                               const QStringList& exts);



private:

    /**
      * \brief Evalua el mapa de configuraciones actual (_currconfvalues)
      */
    void evalConffileValues();
    /**
      * \brief Procesa un archivo tar (f) y descomprime los archivos en
      el directorio de configuraciÃ³n por defecto
      * \param f archivo con extensiÃ³n .tar a descomprimir
      * \param isprincipal si es el directorio principal
      * \return retorna verdadero si la operaciÃ³n fue exitosa, falso en caso contrario
      */
    bool processDirTar(const QString& f, bool isprincipal = false);
    bool searchInHistoryList(const QString& str);

    void createMenu();
    void configureStatusBar();
    QAbstractItemModel *modelFromFile(const QString& fileName);

    void generateJScriptreports(const QString& documents, const QList<QSqlField>& fields);
    void setupStackedWebviews(const QIcon& icon, const QString& name, const QString& desc = QString(""));

    //    QComboBox *comboMode;

//    PrincipalFrame *principalFrame;


    // Utilizado para reporte en web
//    QString _porc;
    QString jscriptcolumns;
    bool jscriptload;
    QString currentDocuments;
    QString currentADocuments;
    QList<QSqlField> currentFields;

    // para mostrar el Assistant

    // Objeto SafetDocument asociado a MainWindow
    //SafetDocument safetDocument;

    QMap<QString,QString> _currconfvalues;
    QMap<QString,QString> _currparsvalues;

    /**
     * \brief Reemplaza el macro {#safetdocumentid:<flujo>:<variable>} con el id generado del documento
     * \param t plantilla
     */

    bool replaceDocumentid(QString &t, const QString& idkey);
    void executeWithoutJSON();
    void executeWithJSON();
    QString doListUsers(const QString &selectrol);
    QString searchScript(const QString &namevar);
private slots:
    void executeJSCodeAfterLoad( /*bool ok */ );



};


#endif // MAINWINDOW_H
