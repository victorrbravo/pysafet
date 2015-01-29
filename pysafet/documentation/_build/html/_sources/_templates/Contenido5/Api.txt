


============================================
:index:`Clase MainWindow(Safet.MainWindow)`
============================================



**Clase MainWindow:**


 **Constructor:**
  **MainWindow** (home-usurio)
	**home-usuario:** Directorio del usuario donde se encuentra la carpeta **.safet/**
		
	**Ejemplo**

	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome) #---> MainWindow
			

		

 **Listado de myinflow:**

 #. **setHostURL** (u) [Procedimiento]
	Coloca el url del servidor web actual.

	**Ejemplo:**
	
	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome)
	... 
	... myinflow.setHostURL(myurl) #---> setHostURL 


 #. **setMediaPath** (m) [Procedimiento]
	Coloca la ruta del directorio de los archivos gráficos.

	**Ejemplo:**
		
	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome)
	... 
	... myinflow.setMediaPath(mymedia)  #---> setMediaPath 


 #. **addInfoGraphDateText** () [Retorna cadenas (String)]
	Agregar la información de fechas al grafo.

 #. **addNodeToXMLWorkflow** (fname,beforenode = "",nodename = "newnode",isparallel = false,options"",query = "",nodetitle = "",documentsource = "") [Retorna cadenas (String)]
	Agregar un nodo al grafo. 


 #. **autoComplete** (path) [Retorna cadenas (String)]
	Retorna la lista de autocompletación.
	
 #. **changeConnXMLWorkflow** (fname,currnode,nextnode,newnode,newoptions = "",newquery ="" ) [Retorna cadenas (String)]
	Cambia una conexión entre nodo.

 #. **checkUserRegister** (fullname,account,email,passone,passtwo) [Retorna cadenas (String)]
	Realiza el registro de un nuevo usuario. 

 #. **createBdoc** (content) [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]
	Crea un documento "bdoc" vacío.


 #. **currentDATA** () [Retorna cadenas (String)]
	Devuelve los datos (DATA) actual.

 #. **currentError** () [Retorna cadenas (String)]
	Devuelve el error actual.

	**Ejemplo:**
	
	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome)
	... myinflow.setMediaPath(mymedia)
	... myinflow.setHostURL(myurl)
	...	
	... myconsult = u"operacion:Listar_datos Cargar_archivo_flujo:/home/cenditel/.safet/flowfiles/mensajes.xml Variable: vIniciado"
	...
	... result = myinflow.login("usuario","usuario")
	... 
	... if not result:
	...	print "Consult failed error: %s"  % (myinflow.currentError()) ###---->>currentError()
	...	exit()



 #. **currentGraphTitle** () [Retorna cadenas (String)]
	Devuelve  el titulo del último gráfo generado.

 #. **currentJSON** () [Retorna cadenas (String)]
	Devuelve la imformación en formato JSON de la última acción generada


	**Ejemplo:**
	
	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome)
	... myinflow.setMediaPath(mymedia)
	... myinflow.setHostURL(myurl)
	...
	... myconsult = u"operacion:Listar_datos Cargar_archivo_flujo:/home/cenditel/.safet/flowfiles/mensajes.xml Variable: vIniciado"
	... 
	... result = myinflow.login("usuario","usuario")
	... 
	... if not result:
	...	print "Consult failed error: %s"  % (myinflow.currentError()) 
	...	exit()
	...
	... print u"%s" % (myinflow.currentJSON()) ###---->>currentJSON()




 #. **currentTable** () [Retorna cadenas (String)]
	Obtiene los datos en forma de tabla del último gráfo generado.

 #. **delNodeToXMLWorkflow** (fname,nodename) [Retorna cadenas (String)]
	Elimina un nodo de un gráfo.

 #. **doLoadConfiguration** (fileName)[Procedimiento]
	Carga un archivo de proyecto.


	
	**Ejemplo**

	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome)
	...
	... myinflow.doLoadConfiguration("Proyecto.tar")  ###---->> doLoadConfiguration



 #. **doSaveConfiguration** (fileName)[Procedimiento]
	Guarda un archivo de proyecto (en el directorio **.safet/** del usuario actual).

	**Ejemplo**

	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome)
	... 
	... myinflow.doSaveConfiguration("usuario.tar") ####--------->> doSaveConfiguration



 #. **doSaveGraph** (mypars) [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]
	Guarda los parámetros de último gráfo generado.

 #. **generateFormFooter** (o) [Retorna cadenas (String)]
	Imprime el HTML del pie de pagina general para **websafet**.

 #. **generateFormHead** (o) [Retorna cadenas (String)]
	Imprime el HTML de la cabecera general para **websafet**.

 #. **generateModifyHTML** (operation,fieldname,key,secondkey,form) [Retorna cadenas (String)]
	Genera una consulta **"AJAX"** para los campos de combos de selección.

 #. **getFlowParameters** (flowfilename) [Retorna cadenas (String)]
	Obtiene una lista de parámetros dado un nombre de gráfo.

 #. **getInfoOfUser** (user) [Retorna cadenas (String)]
	Obtiene la información de un usuario.


 #. **hostMediaPath** () [Retorna cadenas (String)]
	Obtiene la ruta del directorio de archivos multimedia.

 #. **hostURL** () [Retorna cadenas (String)]
	Obtiene el url del host **(websafet)**.

 #. **inputPath** () [Retorna cadenas (String)]
	Obtiene el directorio (ruta) de las aplicaciónes actual **(.safet)**.

 #. **lastInfoGraph** () [Retorna cadenas (String)]
	Obtiene información sobre el último gráfo generado.

 #. **loadReportTemplate** (json,filename = "",nameoperation = "Listar_datos") [Retorna cadenas (String)]
	Carga una plantilla para generar una salida HTML **(websafet)**.

 #. **log** (message)[Procedimiento]
	Escribe en el "log" de safet  **<HOME>.safet/log/safet.log**.
 
 #. **login** (name,pass) [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]
	Inicia una sesión **PySafet**.


	**Ejemplo:**
	
	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome)
	... myinflow.setMediaPath(mymedia)
	... myinflow.setHostURL(myurl)
	... 
	... myconsult = u"operacion:Listar_datos Cargar_archivo_flujo:/home/cenditel/.safet/flowfiles/mensajes.xml Variable: vIniciado"
	...
	... result = myinflow.login("usuario","usuario") ###---> login


 #. **logout** () [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]
	Sale una de las sesión **PySafet**.

 #. **mediaPath** () [Retorna cadenas (String)]
 	Obtiene las rutas del directorio "media" utilizando para generar los gráfos **(.svg/.png)**.

 #. **menuCommands** () [Retorna cadenas (String)]
	Obtiene la lista de acciones en formato HTML.

 #. **menuForm** (o) [Retorna cadenas (String)]
	Obtiene un formulario escrito en HTML correspondiente a una acción en el archivo **<HOME>.safet/input/deftrac.xml**.

 #. **postAction** () [Retorna cadenas (String)]
	Obtiene la acción posterior a ejecutar en uan operación.

 #. **registerLogin** (user) [Procedimiento]
	Escribe en el registro de **PySafet** el inicio de una seción.

 #. **registerLogout** (user) [Procedimiento]
	Escribe en el registro de **PySafet** el inicio de la salida de una sesión.

 #. **sendCheckEmail** (user,plink) [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]
	Envía un correo de chequeo para el resgistro de un usuario.

 #. **setConffileValues** (values) [Procedimiento]
	Coloca los valores de configuración.

 #. **setHostMediaPath** (m) [Procedimiento]
	Coloca la ruta para los archivo de medias.


 #. **setTemplatePath** (t) [Procedimiento]
	Coloca el directorio de los archivos de plantilla (templates).

 #. **templatePath** () [Retorna cadenas (String)]
	Obtiene el directorio para los archivos de plantilla (templates).

 #. **toInputConsole** (action, withpermises = true) [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]
	Ejecuta una operación de consulta ****** en el archivo **defconsole.xml**.


	**Ejemplo:**

	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome)
	... myinflow.setMediaPath(mymedia)
	... myinflow.setHostURL(myurl)
	...
	... myconsult = u"operacion:Listar_datos Cargar_archivo_flujo:/home/cenditel/.safet/flowfiles/mensajes.xml Variable: vIniciado"
	...
	... result = myinflow.toInputConsole(myconsult) ###------>>> toInputConsole




 #. **toInputForm** (action, withpermises = true) [Retorna cadenas (String)]
	Ejecuta una operación de entrada de datos (Formulario) descrita en el archivo de **deftrac.xml**.

	
	**Ejemplo:**

	>>> import Safet
	... import os
	... myhome = os.getenv("HOME")
	... mymedia = myhome + "/tmp"
	... myurl = "http://localhost"
	... 
	... myinflow = Safet.MainWindow(myhome)
	... myinflow.setMediaPath(mymedia)
	... myinflow.setHostURL(myurl)
	...
	... myconsult = u"operacion:agregar_mensaje Titulo: hola mundo"
	...
	... result = myinflow.toInputForm(myconsult) ###------>>> toInputForm



	
 #. **toInputUsers** (action) [Retorna verdadero si se ejecuto la acción, en caso contrario retorna falso]
	Ejecuta una acción de gestión de usuarios descrita en el archivo **defusers.xml**.




