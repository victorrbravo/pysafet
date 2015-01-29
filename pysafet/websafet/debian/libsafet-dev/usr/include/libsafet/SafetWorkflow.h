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

#ifndef SAFETWORKFLOW_H_
#define SAFETWORKFLOW_H_


#include "SafetXmlObject.h"

#include "SafetTask.h"
#include "SafetCondition.h"
#include "SafetVariable.h"
#include "SafetToken.h"
#include "SafetLog.h"
#include "SafetStats.h"
#include "SafetSQLParser.h"
#include "SafetDirXmlRepository.h"
#include "SafetParameter.h"

#ifdef SAFET_DIGIDOCPP
#include "log.h"
#include "BDoc.h"
#include "Conf.h"
#include "Document.h"
#include "crypto/cert/X509Cert.h"
#include "crypto/cert/X509CertStore.h"

#include "crypto/cert/DirectoryX509CertStore.h"

#include "bdocdocument.h"
#endif

#include <stack>


class SafetSQLParser;




/**
* \brief Clase SafetWorkflow
* 
* Esta Clase corresponde al objeto  flujo de trabajo. El objeto flujo de trabajo (workflow)
* contiene las tareas, condiciones y variables que definen el flujo, también poseen métodos
* para obtener y modificar los atributos tales como el identificador (id) y la descripción (desc)
*  
* Los objetos Workflow son contenidos dentro de el objeto SafetYAWL. 
* 
* Deriva de la Clase SafetXmlObject, lo que quiere decir que es sincronizado con un elemento
* XML definido en documento de definición de datos (DTD)
* 
* 
*/
class SafetWorkflow : public SafetXmlObject {
	Q_OBJECT
	Q_PROPERTY(QString id READ id WRITE setId)
	Q_PROPERTY(QString desc READ desc WRITE setDesc)


        friend class SafetStats;

        QStringList _extrainforols;
        QStringList _extrainfotss;






public:

        struct Miles {
            int id;
            QString prevtask;
            QString nametask;
            QString rol;
            QString note;
            QDateTime ts;
            int secondswait;
            QString humanwait;
            QString humandate;
            QString porc;
            QString porcremaining;
            QDateTime plannedts;

        };


        struct ExtraInfoSearchKey {
            QString key;
            QMap<QString,QString> extrainfo;
        };

	class PathHistory {
		QString _nodename;
		QQueue<QString> _history;
	public:
		PathHistory() {_nodename = "";}
		PathHistory(const QString& s) {_nodename = s;}
		QQueue<QString>& history() { return _history;}
		QString nodename() const { return _nodename;}		
		void setNodename(const QString& s)  { _nodename = s;}
	};
        enum ResultType { Complete, Partial };
    enum ItemSearched { SearchTask, SearchOption };
    enum OutputFormat { XML, JSON, SVG,JSONARRAY };
        enum TypeStats { Text, Graph };
        enum NextStates { OnlyNext, NextAndPrev, Previous, Neighbour, AllNotCompleted,  All, CurrentState };

        /**
         * \brief Devuelve  una objeto consulta (QSqlQuery) con los registros correspondientes a
         * los documentos calculados para la vari        SafetStats mystats;able v, correspondiente a una
         * determinada tarea o actividad
         * \param v Variable a ser calculada para una determinada tarea o actividad
         */

        QSqlQuery getSQLDocuments(const SafetVariable& v);
        /*
          * \brief Retorna la lista de variables de todas las tareas contenidas en el flujo de trabajo
          * \return Lista de punteros a Variables de las tareas  contenidas en el flujo de trabajo
          */
        QList<SafetVariable*>& getVariableList() { return variablelist;}
        QList<SafetTask*>& getTasklist() { return tasklist;}
        QList<SafetCondition*>& getConditionlist() { return conditionlist;}
        QList<SafetParameter*>& getParameterlist() { return parameterlist;}



        int numberOfTokens() const {  return ntokens; }

        /**
          * \brief Retorna el mapa de informacion de los nodos
          *  \return Map (QMAP) de informacion de los nodos por id
          */
        QMap<QString,QString>& infoModeMap() { return infonodemap; }

//        QString porc() const { return _porc; }
//        void setPorc(const QString& p) { _porc = p; }

        /*
          * \brief Coloca el numero total de fichas
          *
          */
        void setNumberOfTokens(int n) { ntokens = n; }

        QStringList& getExtrainfoRols() {return _extrainforols;}
        QStringList& getExtrainfoTSs() {return _extrainfotss;}
        ExtraInfoSearchKey& searchKey() { return _searchkey; }


        /**
         \brief Coloca los parametros {#1} en el orden que se encuentran en la lista p
         \param p Lista de valores de los parametros
         \return Devuelve true si hubo cambio de parametros, false en caso contrario.

         */
        bool putParameters(const QMap<QString,QString>& p);

        /**
          \brief Si existe un argumento del tipo {#1},{#2},... lo reemplaza por el elemento
                correspondiente en la posicion de la lista l
          \param strin Cadena de entrada
          \param Lista de valores para reemplazar
          \return Cadena reemplazada con el valor del argumento
          */
        static QString replaceArg(const QString& strin, const QMap<QString,QString>& l);
        /**
          \brief Cambia los valores "cualquiera". "todos", por expresiones SQL como "like..:"
          \param l lista de Parametros
          \return Nueva lista con los parametros en SQL
          */
        QMap<QString,QString> changeHumanizeParameters(const QMap<QString,QString>& l);

        /**
          \brief Quita los parentesis (CASO SQLite)

          */
        static QString removeOutParenthesis(const QString &s);


        /**
          \brief (Estatica) Colorea los estados con informacion extra (extrainfo)
          \param codigo del grafo en l�neas Nodo:...
          \return Nuevo codigo
          */
        static QString paintGraph(const QString& code);

        /**
          \brief (Estatica) Calcula una formula dada con los parametros (ExtraInfo.formula...) para un grafo
          \param codigo del grafo en l�neas Nodo:...
          \return Nuevo codigo
          */

           QString calculateGraphFormula(const QString &code,ResultType rtype =  SafetWorkflow::Complete);

        /**
          \brief Pinta un nodo para las estadisticas
          \param codigo del grafo en l�neas Nodo:...
          \return Nuevo codigo
          */
        static QString paintStats(const QString& code);


        /**
          \brief Genera una cadena en formato JSON dado un conjunto de hitos
          \param miles Lista de hitos ordenados por fecha (orden cronologico)
          \param Parametro de entrada (campos que se agregan en el documento JSON)
          \return cadena JSON
          */

        static QString getJSONMiles(const QList<SafetWorkflow::Miles>& miles,QList<QSqlField>& myfields);
private:

        ExtraInfoSearchKey _searchkey;



        SafetXmlRepository *_xmlRepository;
	QString _id, _desc;
	QList<SafetTask*> tasklist; // Lista de Tareas
	QList<SafetCondition*> conditionlist; // Lista de Condiciones
        QList<SafetParameter*> parameterlist;

	QList<SafetVariable*> variablelist; // Lista de variables  	
	QMap<QString,SafetNode*> nodemap; // Lista de todos los nodos del flujo: tareas y condciones
	QMap<QString,bool> visitnodemap;  // Nodos Visitados
	QMap<QString,int> SHAPEMAP;
        QMap<QString,QString> infonodemap; // Informacion (estadisticas de los nodos)
	QMap<QString,QString> RESOURCESFILES; // Mapa con las rutas de los archivos de recursos
	QSet<QString>  lastkeyset; // Conjunto para realizar el filtro
	QString  currentfilterkey; // guarda las claves para el mapa de claves "drop" (eliminadas)
//	QList< QMap<QString,QSet<QString> > > dropkeys;
	QMap<QString,QSet<QString> > dropkeys;
	QQueue<QString>  tokenlinkqueue; // Lista para realizar el filtro del enlace al token
	QQueue<QString> tokenlinkqueueInPath; // Lista para realizar el filtro del enlace al token
	QString _tablesource; // Tabla (origen) de la cual se está realizando la consulta
	QList<QString> whereset; // Conjunto de cláusulas WHERE  de los operadores tipo SPLIT
	QSet<QString> joinWhereset; // Conjunto de cláusulas WHERE  de los operadores tipo JOIN
	QStringList checkfieldslist; // Lista para cuando se especifica un conjunto de opciones en "options"
        QSet<QString> variablesset;// Conjunto de nombres variables para chequear que no se repitan nombres de variables
        QSet<QString> autofilterset;// Conjunto de nombres de autofiltros para chequear que no se repitan nombres de variables
        QSet<QString> recursivefilterset;// Conjunto de nombres de filtros recursivos para chequear que no se repitan nombres de variables
	bool isStartingKeyFilter; // Variable que indica que se inicio el filtro de claves
	QMap<QString,QSet<QString> > keysoptions; // mapa para lista de claves
        SafetSQLParser parser; // Objeto para descomponer y extraer elementos de texto escrito en un formato dado
        QSet<QString> optionsset;	// Conjunto del opciones  definir su uso
	QStack<QString> opeStack, joinOpeStack; // Pila de operadores AND, OR, XOR, etc. 
        QString addGraphvizNode(const QString& s, char* filetype = "png", const QString& info = "");
	int shapecount; // Contador de figuras
	int oriminy,orimaxy, oriminxx, orimaxx; // Valores originales del archivo SVG
        int ntokens;
        QString _libdigidocConfigFile; // Ruta del archivo de configuracion


        /**
          * \brief chequea que todos los destinos de las tareas existan
          * \return true si todos los destinos existen, false si uno o m�s destinos (source) existen
          */
        bool  checkSourcesTask();


	/*
	 * \brief
	 * Reinicia los contenedores, elimina todos los elementos del contenedor
	 */
	void resetContainers();

	/*
	 * \brief
	 * Funcion estática que retorna una lista de nombres de archivos temporales,
	 * Se garantiza que los nombres de los archivos son diferentes y se encuentran en el 
	 * directorio de archivos temporales del sistema operativo.
	 */
	static QStringList  getTempNameFiles(int n);
	/* 
	 * \brief Genera una cadena para de filtro (WHERE) para ser utilizada en la setencia SQL que se
	 * genera internamente 
	 */

        /**
          * \brief Ajusta la fecha dada en el formato "format" a la zona de horaria
                  especificada en el parametro del archivo de configuracion safet.conf
                  (GeneralOptions.timezone = Zone). Ver http://en.wikipedia.org/wiki/List_of_tz_zones_by_name
             \param d Fecha tipo cadena
             \param formatin Formato de la fecha de entrada
             \param formatin Formato de la fecha de salida
             \return Fecha en formato cadena trasladada a la zona horaria
          */
        static QString adjustTimeZone(const QString& d,
                                      const QString& formatin = "dd-MM-yyyy hh:mm:ss",
                                      const QString& formatout = "dd/MM/yyyy hh:mmap");
    


        /**
          \brief Utilizado para generar grafico de flujo de trabajo
          */
        QString pushAllChild(SafetNode *node,std::stack<QString>& localstack, QMap<QString,QString>& results,
                             const QString& info,
                             QMap<int,QString>& orderresults,
                             uint& countresults,
                             bool norender);
	/* 
	 * \brief Genera una cadena para de filtro (WHERE) para ser utilizada en la setencia SQL que se
	 * genera internamente 
	 */

        /*
          *\brief Devuelve el conjunto de valores pr�ximos dado un keyvalue actual
          *\param keyvalue Valor colocado en filtro
          *\return Lista de valores
          */
        QStringList searchNextValues(const QString& keyvalue);

	QString pushLeftChild(SafetNode *node,QStack<SafetWorkflow::PathHistory>& localstack,
			QQueue <QString>& queuepath);
	/* 
	 * \brief Genera una cadena para de filtro (WHERE) para ser utilizada en la setencia SQL que se
	 * genera internamente 
	 */

	void resetVisitNodeMap();
	/* 
	 * \brief Genera una Sentencia SQL  que contiene el filtro de las claves (enlaces a las fichas del flujo de trabajo)
	 * 
	 */


	QString generateKeyFilterString(bool brackets = true) const;
	/* 
	 * \brief Genera una Sentencia SQL  que contiene vínculos a tablas específicas con la finalidad de generar el enlace al tokenlink
	 * 
	 */
    QString generateJoinString(int numberPath = 0, const QString& nametable = "") const;
	/* 
	 * \brief Genera una cadena para de filtro (WHERE) para ser utilizada en la setencia SQL que se
	 * genera internamente 
	 */
        QString generateWhereFilter(const QString& f, SafetNode* node,int curroption, int ipath = 0);

	/* 
	 * \brief Genera el nuevo conjunto de claves (filtrado)
	 * \param s Cadena sql a ser ejecutada 
	 * \param set conjunto a ser modificado, donde se colocan las nuevas claves
	 * \param found opción que indica si se revisa el conjunto de opciones
	 * \param node Nodo evaluado 
	 */	
	void generateNewKeySet(const QString& s, QSet<QString> &set, bool found, SafetNode* node,
                        int curroption, const QQueue<QString > &path, bool checkSign = false, bool checkTableToken = false, const QString& l = "", int ipath = 0);
	
	/* 
	 * \brief Dado el operador de flujo (AND,OR,etc..) lo traduce en un su correspondiente
	 * operador para lenguaje SQL (UNION, INTERSECT, ...)
	 * \param s Cadena sql a ser ejecutada 
	 */	
    QString getSQLOperator(const QString& o) const;
	/* 
	 * \brief Procesa una opearacion SQL según los operadores AND, OR, ... lo traduce en un su correspondiente
	 * operador para lenguaje SQL (UNION, INTERSECT, ...)   
	 * \param myqueue Cola de operaciones SQL 
	 * \param ope
	 */ 
    QString processSQLOperation(const QString& sql, QStack<QString> &stack, const QString& ope) const;

	/* 
	 * \brief Evalua una expresion SQL convirtiendo los operadores YAWL (AND, OR, XOR,etc.) en operadores del
	 * lenguaje SQL
	 * \param sql Cadena sql a colocar como prefijo
	 * \param pila a evaluar
	 */ 	
	QString evalSQLKeyFilterStack(const QString& sql, QStack<QString> &e);
	
	/* 
	 * \brief Construye una expresión SQL dado la expresion (sql) y el filtro (s) 
	 * \param sql Cadena sql a colocar como prefijo
	 * \param s Filtro parar armar el SQL
	 */ 	

    QString addSQLPrefix(const QString& sql, const QString& s) const ;
	
	
    //QString  getStackExpression(const SafetVariable& v, QStack<QString> &splitresults, QStack<QString> &joinresults);

    QString  getStackExpression(const SafetVariable& v, QStack<QString> &splitresults);

	/**
	\brief Genera una una expresion para evaluación de una cláusula where
	\param f campo de la expresión
	\param o valor de la variable de la expresión
	\param haswhere indica si se lo incluye o no la palabra reservada SQL <WHERE>  a la expresión que se genera
	*/
	QString generateWhereExpression(const QString& f, const QString& o, bool haswhere = false);

	/**
	\brief dibuja un grafico dot (graphviz) del flujo de trabajo
	\param dot Cadena de caracteres que contiene el grafico en lenguaje dot
	\param filetype tipo de archivo a a generar, por omisión es de tipo png
	\return nombre del archivo donde se escriba la imagen generada
	*/
	QString drawDotGraph(const QString& dotfile, char* filetype = "png" );

	/*
        * \brief agrega paramétros para ejecución de JavaScript cuando la imagen es un mapa de imágenes HTML
        * \param map Mapa de imágenes en formato HTML
        * \return Mapa modificado con los parámetros JavaScript que se le agregaron
	*/
        QString addJScriptToMap( const QString& map );

        /*
         * \brief Genera una cadena con la informacion de un nodo del flujo de trabajo en un formato establecido
         *
         */
        QString printNodeInformation(SafetNode *node, const QString& nextnode, const QString& oldnextnode,
                                     const QString& info,
                                     bool norender);
public:


	
	/*
	 * \brief Constructor Vacío de la Clase SafetWorkflow
	 */ 
	SafetWorkflow();
	/*
	 * \brief Destructor de la Clase SafetWorkflow
	 */ 

	virtual  ~SafetWorkflow();
	
	void setId(QString i) { _id = i; }
    QString id() const { return _id; }

    void setDesc(QString d) { _desc = d; }    
	QString desc() const { return _desc; }

    virtual void addChild(SafetXmlObject* o);

    void addTask(SafetTask* t);
    /**
     * \brief Obtiene la lista de tareas contenido en el flujo de trabajo
     */

    QList<SafetTask*>& getTasks();


    /**
     * \brief Almacena la lista de tareas en una cadena, separadas por el caracter de retorno de carro "\n"
     * \param inc Indica si se deben incluir las nodos "Condiciones
     * \param c Indica la cadena para separar la lista de tareas y/o condiciones, por defecto se coloca el retorno de carro "\n"
     * \return Returna una cadena que contiene la lista de tareas
     */
    QString listTasks(bool inc = false, const QString& c = QString("\n") );

    /**
     * \brief Obtiene la lista de condiciones contenido en el flujo de trabajo
     */

    QList<SafetCondition*>& getConditions();

    /**
     * \brief Obtiene el mapa <nombre de nodo> -> <Puntero a objeto Nodo>  del flujo de trabajo
     * se utiliza para tener un acceso directo a todos los nodos del grafo del flujo de trabajo
     */

    QMap<QString,SafetNode*>& getNodes();

    /**
     * \brief Calcula el número de fichas (tokens) correspondiente a la tarea o actividad
     * que indica el parámetro idtask  
     * idtask Identificador de la tarea a calcular el número de fichas
	*/
    int numberOfTokens(const QString& idtask);

    /**
     * \brief Busca un objeto SafeTask correspondiente con el identificador (idtask)
     * idtask Identificador de la tarea a calcular el número de fichas
 	*/
    SafetTask* searchTask(const QString& idtask);


    enum HumanizeDateType { None, SinceWhenTime, WaitTime };
    /**
      \brief Humanizar la fecha en formato "Hace X dias, X semanas..."
      */

    static QString humanizeDate(int &pdays, const QString& d, const QString& formatin,
                                QDateTime now = QDateTime::currentDateTime(),
                                SafetWorkflow::HumanizeDateType typetime = SafetWorkflow::None, bool humanize = true);

    static QString humanizeDate(int seconds);

     /*
     * \brief Busca un objeto SafeCondition correspondiente con el identificador (idnode)
     * \param idnode Identificador del nodo (Tarea o Condicion) a calcular el numero de fichas
     */
     SafetCondition* searchCondition(const QString& idcon);


     /* \brief Convierte un archivo en formato SVG a formato JSON (JavaScript Object Notation)
     * 
     * \param s Archivo en formato SVG
     */
    QString svgToJSON(const QString& s);

    /**
     * \brief Escala numeros a la escala utilizada en JSON
     * 
     * \param s cadena donde se escalaran los números
     * \param minx Valor mi�nimo de Horizontal
     * \param maxx Valor maximo de Horizontal
     * \param miny Valor mi�nimo de Vertical
     * \param maxy Valor maximo de Vertical
     */
    QString scaleToJSON(const QString& s, int minx, int maxx, int miny, int maxy);

    /**
     * \brief A�ade el estilo (Color, forma, pincel, etc) dada un estilo SVG
     * 
     * \param s Cadena de estilo en formato svg/html
     */
     QString addShapeStyleToJSON( const QString& s );
    /**
     * \brief Agrega una Figura a la cadena JSON, dada un elemento de figura SVG (XML)
     * 
     * \param n Elemento correspondiente a la Figura SVG
     * \return Cadena JSON generada
     */
     QString convertSvgShapeToJSON(const QDomElement& n);

    /**
     * \brief Calcula el número de fichas (tokens) correspondiente a la variable
     * que indica el parámetro v  
     * v Variable a ser calculado el nùmero de fichas
	*/
    int numberOfTokens(const SafetVariable& v);

    /**
     * \brief Obtiene la definición de la ficha (token), Puntero al objeto SafetToken  
     * Existe solo una definición de ficha por objeto de flujo de trabajo
    */
    SafetToken* getToken();
    QList<QQueue<QString> > generatePaths(const QString& idtask, SafetWorkflow::ItemSearched t = SafetWorkflow::SearchTask);
    
    /**
     * \brief Devuelve un contenedor (pila) con todas las claves correspondientes
     * a la tarea que se indica con el idtask   
     * \param idtask identificador de la tarea de la cuál se calculará el conjunto de claves 
     * \param splitresults  Contenedor de resultados para el operador de salida (SPLIT)
     * \param splitresults  Contenedor de resultados para el operador de entrada (JOIN)
    */
    //void generateKeyFilterStack(const QString& idtask, QStack<QString>& splitresults, QStack<QString>& joinresults);
    void generateKeyFilterStack(const QString& idtask, QStack<QString>& splitresults);

    /**
      *  \brief Construye un filtro para el camino dado por "path", las demás parámetros son auxiliares
      *
      */
    int walkPath(QQueue<QString>& path, QStack<QString>& reversepath, QStack<QString>& prevnodes, QString&  splitresult, QString& joinresult,SafetPort* splitPort, SafetPort* joinPort, const QString& idtask = QString(""), int ipath = 0);

    /**
     * \brief Devuelve un puntero al objeto condición Inicial
    */
    SafetCondition* getStartCondition();
	/**
	* \brief Genera el filtro para los puertos evaluados en cada tarea
	*/
    QString generateKeyFilterExpression(QString& s, SafetNode* node, int curroption, const QQueue<QString > &path, bool isjoin = false, int ipath = 0);

    /**
     * \brief Chequear Operador Or Interno: expresiones que empiezan con contienen el símbolo ";"
     * \return retorna -1 si no encontro claúsulas or internas, sino retorna la posicion de la clausula
    */
    int checkInternalOrOptions(const QString& onew, QSet<QString>& optionsset, QString& lastk);
    /**
     * \brief Chequear el filtro tipo Firma en la generacion de conjuntos de claves de fichas (generatenewSet)
    */
    void checkForSignFilter(const QString&s, SafetConnection* conn, const QString& l, bool checkSign, QSet<QString> &set);

        /**
        * \brief Genera la sentencia SQL del filtro para los puertos evaluados en cada tarea
        */
    QString generateSQLPortKeyFilterExpression(SafetPort* port, int curroption, int npath = -1);

        /**
        * \brief  procesa el conjunto de filtro de una tarea/condicion especifica (generatenewSet)
        */
    void proccessQueryFilter(QString pattern,
                             QStringList& querys, QList< QStringList >& optionlists,
                             QSet<QString> &set, bool found, SafetNode* node,
                             int curroption, const QQueue<QString>& path,int ipath);


    /**
    * \brief  Chequea el conjunto actual con los otras conexiones
    */

    void checkPatternWithOthers(QString pattern, QStringList& querys,
                                            QList< QStringList >& optionlists, QSet<QString> &set,
                                            bool found, SafetNode* node,
                                            int curroption, const QQueue<QString>& path,int ipath);


    /**
      * \brief Calcula el numero de ruta a seguir dode un nodo (node) en particular al siguiente node (nextnode), se utiliza
      * para el cálculo de filtros
      **/
    int calculateCurroption(SafetNode::PortType type, SafetNode *node, SafetNode *nextnode);
    /**
      * \brief Si existe un patron diferente a "none" ejecuta la operacion con los diferentes caminos (paths) para puertos tipo salida "split"
      *
      */
    bool checkPatternWithOthersPath(const QString& value, const QString& pattern, const QStringList& querys, const QList< QStringList >& optionlists);
    /**
      * \brief Vacia las claves del mapa de claves eliminadas (dropkeys) que coincida con la cadena colocada como filtro)
      *
      */
    void clearDropkeys(const QString& filters);

    /**
     * \brief Genera la expresión SQL para dar enlace a la expresion a la cadena SQL que se pasa por parámetro
     * \param s expresion SQL  a la que se calculará su enlace
     *  \param final indica si es la expresión SQL que se le calcula la variable
    */  
    QString processTokenlink(const QString& s, bool final = false, int npath = -1);
    QString generateSQLToken(const QStringList &list, bool final = false, int npath = -1);


    /**
     * \brief Devuelve el puerto (Port) de entrada (JOIN) o salida (SPLIT) según se especifique 
     * \param type Tipo de puerto  a devolver : JOIN para devolver el puerto de entrada , y SPLIT para devolver el puerto de salida
     * \param node Nodo sobre la que opera el método
     * Devuelve el nombre del archivo grafico generado
    */
     static SafetPort* getOperationPort(SafetNode::PortType type, SafetNode* node);

    /**
     * \brief Devuelve el nodo anterior o siguiente  en la evaluación de un determinado camino
     * \param type Tipo de nodo a devolver : JOIN para devolver el nodo previo, y SPLIT para devolver el nodo anterior
     * \param path Camino de nodos (Cola) que faltan por evaluar
     * \param revesepath Camino de nodos (Pila) ya evaluados 		
     * de la biblioteca graphviz
     * Devuelve el nombre del archivo grafico generado
    */
    SafetNode* getOperationNode(SafetNode::PortType type, const QQueue<QString>& path, const QStack<QString>& reversepath );



    /**
     * \brief Genera un imagen de gráfico actual del flujo de trabajo utilizando la sintaxis 
     * de la biblioteca graphviz
     * Devuelve el nombre del archivo grafico generado
    */
    QString generateGraph(char* filetype, QString& json, const QString& info = "");

    /*
      * \brief Genera en forma textual (texto) la entrada para renderizar el grafico de flujo de trabajo
      */
    QString generateCodeGraph(char* filetype= "png", const QString& info = "", bool norender = false);
    
    /*
      * \brief Genera un archivo gráfico del flujo de trabajo (propio)
      * \param code Código para generar el gráfico
      * \param ext Tipo de extensión del archivo gráfico (png, bmp, gif,etc.)
      * \return nombre del archivo gráfico
      */
    QString renderGraph(const QString& code, const char* ext = "png");
    /**
     * \brief Devuelve un conjuto de claves que se encuentran en la tarea
     * o actividad indicada por el parámetro idtask
     * \param idtask Identificador de la tarea para calcular el número de claves 
     * 
    */

    /**
      \brief Incluye informacion a los codigos por tarea generado por la funcion generateGraph() para conocer rol y estampilla de tiempo de un token en cada tarea
      \param Mapa de tareas y codigos para ser procesos para generar los graficos, se modifica al llamar la funcion, ya que se le anexa a cada tarea informacion sobre rol y la estampilla de tiempo

      */
    void proccessExtraInfo( QMap<QString,QString>& codes,const QString& info,
                            QMap<int,QString>& orders);

    QSet<QString> getKeys(const QString& idtask);
    
    /**
     * \brief Devuelve un lista de los campos del documento     
     * \param idtask Identificador de la tarea
     * \return Lista de cadenas de los nombres de los campos correspondiente a la tarea que se pasa por par&aacute;metro
    */
     QList<QString> getFieldsOfDocument(const QString& idtask);

   /**
     * \brief Devuelve un lista de información sobre los campos de un determinado documento
     * \param idtask Identificador de la tarea 
     * \return Lista de informaciones (Tipo de campo, validación etc, correspondiente a cada campo del documento)
    */
     QList<QVariant::Type> getInfosOfDocument(const QString& idtask);
 
   /**
     * \brief Devuelve un conjuto de documentos (Objetos SafetDocument)  que se encuentran  
     * en la tarea o actividad indicada por el parámetro idtask
     * \param howmanydocuments cuantos documentos se encontraron
     * \param idvariable Identificador de una de las variable para calcular el número de claves 
     * \return Documento XML de los documentos correspondiente a las tareas
    */
    SafetDocument getDocuments(const QString& idvariable, QList<QSqlField>& fields,int &howmanydocuments,
                         OutputFormat of = XML,
                         const QString& info ="");

    SafetDocument getDocuments(const QString& idvariable, OutputFormat of = XML,
                         const QString& info ="");


    /**
     * \brief Devuelve un objeto de Base de datos que representa un conjuto de documentos 
     * en la tarea o actividad indicada por el parámetro idtask
     * \param idtask Identificador de la tarea para calcular el número de claves 
     * \return Objeto de Base de Datos (QSqlQuery)con el conjunto de documentos 
    */
    QSqlQuery getQueryFromTask(const QString& idtask);


    /**
     * \brief Genera un gráfico correspondiente con la clave dada 
     * \param s Clave a buscar
     * \param filetype Tipo de gráfico a generar
     * \return Nombre del archivo de gráfico o cadena declarativa del gráfico (Si es JSON o SVG el tipo de gráfico)
    */
    QString generateGraphKey(const QString& s, char* filetype= "png");


    /**
     * \brief Busca una variable en todas las tareas del Workflow 
     * \param n Nombre (ID) de la variable a buscar
     * \return Puntero a variable encontrado, o devuelve NULL sino encuentra ninguna variable con el nombre ID
    */	
    SafetVariable*  searchVariable(const QString& n);	

      /**
     * \brief Chequea si un documento dado por el registro actual de (query) se encuentra en el repositorio firmado por (signer)
     * \param query Objeto Consulta
     * \param signatories Nombre(s) comun(es)  del(los) firmante (s) separados por "," (signer)
     * \return Verdadero (True) si el documento se encuentra, Falso (False) si el documento no se encuentra
    */	
    bool checkDocuments(QSqlQuery& query, const QString& signatories, QSet<QString> &set,
                        bool operatorOption);



      /**
     * \brief Firma un documento 
	*/
    QString signDocument(const SafetVariable* v, const QString& key, QString &derror);
	
    /**
      \brief Obtiene la lista de firmantes de un documento generado por los datos de entrada
      @see signDocument
      */

    struct InfoSigner {
        QString commonName;
        QString issuer;
        QString date;
        QString hour;
        QString location;
        QByteArray hexsign;
        QByteArray digest;
        QString role;
        QString serial;
        bool isvalid;
        QString typevalid;

        InfoSigner() {
            isvalid = false;
        }
    };

    QList<SafetWorkflow::InfoSigner> getSignersDocument(const SafetVariable* v, const QString& key, QString &derror);

    void setTablesource(const QString& s) { _tablesource = s; }
    QString tablesource() const { return _tablesource; } 	

    SafetToken* getToken() const { return ptoken;}

	/**
	 * \brief Chequea si existen filtro del tipo "drop" (elementos eliminados en "tareas" o "condiciones" anteriores
	 * \param  s Cadena para generar el filtro actual
	 * \param node Nodo del filtro actual 
	 */
	QString checkDropFilter(const QString& s, SafetNode* node, const QQueue<QString> &path, int ipath = 0);
	/**
	 * \brief Imprime por la salida por defecto todos los caminos para una tarea dada
	 * \param  idtask Id de la tarea a imprimir los caminos desde el inicio hasta la tarea marcada por (idtask)
	 * \param info Información sobre la clave a buscar
	 */
	QString printPaths(const QString& idtask, const QString& info = "");
        
    /**
        * \brief Accede al conjunto de todas las  variables
        * \return retorna el conjunto de todas las variables
   */
        QSet<QString>& variablesId() { return variablesset; }


        QSet<QString>& autofiltersId() { return autofilterset; }

        QSet<QString>& recursivefiltersId() { return recursivefilterset; }

      /**
     * \brief Genera las clausulas Where dado una inicial (s) y utiliza el conjunto de where (whereset)
	*/
    QString generateWhereClauses(const QString& s, int ipath = 0);
      /**
     * \brief Chequea si se repite nombres de cambios en la clausulas Where
	*/
     QStringList UniteWhereset();

    static bool checkFilterNULL(const QString& f);

    static bool verifyNegation(const QStringList& list);

    bool delWhereClause(const QString& wc, bool onlynamefield = false);

    /**
      * \brief Lista los proximos estados posibles dado un id
      * \param id clave a encontrar en el flujo
      *  \return Lista de posibles estados a lo que se puede ir dada la clave
    */
    QStringList listNextStates(const QString& id, SafetWorkflow::NextStates st = SafetWorkflow::OnlyNext, bool incback = false);

    /**
      \brief Retorna una lista de la informaci�n de todos los campos atributos "numericinfos"
      de los nodos
      \param id  Identificador de la tarea, si se coloca vac�o retorna todos los "numericinfo" encontrados

      */
    QList<double> numericInfos(const QString& id = QString(""));
    /**
      \brief Retorna una lista de la informaci�n de todos los campos atributos "textualinfos"
      de los nodos
      \param id  Identificador de la tarea, si se coloca vac�o retorna todos los "textualinfo" encontrados
      \param fieldtype 0 textualinfo 1 roleinfo
      */
    QStringList textualInfos(const QString& id = QString(""), int fieldtype = 0);

    /**
      \brief Calcula los operadores JOIN cambiando los operadores tipo SPLIT por los correspondientes
      tipo SPLIT
      */
    void redefineSplitOperators(QStack<QString>& splitresults);
    
    /**
      * \brief Guardar Estados siguientes y previos (en estructuras _nexts y _previouses)
      */
    void saveStates(const QString &currstate, const QStringList& nextstates);
    int getQuerySize(QSqlQuery &qry);



    /**
      \brief Devuelve un mapa con los nombre de la tarea y su respectivo rol
      \return Mapa tarea -> rol
      */
    QStringList getVariablesByRole(const QString& r);
    bool doSubFlow(const QString& flowname, const QString& passname);
    bool isBackNode(SafetNode *node, const QString &nodename);
private:
   QMap<QString,QStringList> _nexts;
   QMap<QString,QStringList> _previouses;

   SafetToken* ptoken;
   SafetWorkflow(const SafetWorkflow&);

   /**
       * @brief checkDisconnectedNodes elimina los nodos desconectados del inicio de una especificaci�n de grafo
       * @param nodes Mapa de nodos (Por referencia, se modifica en el m�todo)
       * @param nodestart nodo de inicio
       * @param nuevo nodo final
       * @return  Variable por referencia
       */

         void checkDisconnectedNodes(QMap<QString, QString>& nodes, const QString& nodestart, const QString& nodeend,const QString& info,
                               QMap<int,QString>& orders);

         void shrinkNodes(QMap<QString, QString> &nodes);
};

#endif /*SAFETWORKFLOW_H_*/

