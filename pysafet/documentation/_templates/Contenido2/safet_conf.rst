

:command:`safet.conf` (Archivo de configuración inicial)
-----------------------------------------------------------

	En este archivo se especifican los parámetros generales para el funcionamiento de la librería PySafet.


 :command:`1.- [Workflow]:` (Sección de flujo de trabjo)
	 En esta sección se define el color del flujo y Sección para la interfaz Web.


 ::

	[Workflow] 

	# Establece el color por defecto 
	defaultcolor = white 		------> # Admite white,black,blue y formato RGB #000000

	# Establece el color activo 
	activecolor = white 

	# Establece el color pasivo 
	traccolor =  white 

 :command:`2.- [Operators]:` (Operadores de flujo de trabajo)

	En esta sección se establecen las variables de la imágenes de los operadores que son definido en un flujo de trabajo, entre las variables se encuentra:

 - :command:`split.xor =` Se activa un enlace saliente una vez terminada la tarea.
 - :command:`split.and =` Activa todos los vínculos salientes una vez terminada esta tarea
 - :command:`split.or =` Se utiliza para desencadenar algunos, pero no necesariamente todos los flujos salientes a otras tareas.
 - :command:`join.xor =` Esta tarea se activa cada vez que un nuevo enlace se ha activado.
 - :command:`join.and =` Esta tarea se activa cuando todos los vínculos se han activado.
 - :command:`join.or =` Esta tarea se activa cuando almeno un vinculo se ha activado.



 ::

	[Operators] 

	# Operador split.xor 
	split.xor = imgs/xor.png   -----> # Archivo de imagen en el directorio <HOME>.safet/imgs

	# Operador split.and 
	split.and = imgs/and.png 

	# Operador split.or 
	split.or = imgs/or.png 

	# Operador join.xor 
	join.xor = imgs/join-xor.png 

	# Operador join.and 
	join.and = imgs/join-and.png 

	# Operador join.or 
	join.or = imgs/join-or.png 

	# Ningun operador 
	none =  imgs/none.png 


 :command:`3.- [Log]:` (Registro de eventos)

	En esta sección se configura las diferentes opciones de la información sobre el registro de los eventos. Entre las opciones tenemos:

 - :command:`log.filepath =` Establece la ruta absoluta donde reside el archivo de registro ejemplo: <HOME>.safet/.log
 - :command:`log.debug =` Habilita la opción de registro de mensajes de depuración (on/off)
 - :command:`log.action =` Habilita la opción de registro de de mensajes de acciones (on/off)
 - :command:`log.warning =` Habilita la opción de registro de de mensajes de advertencia  (on/off)
 - :command:`log.error =` Habilita la opción de registro de de mensajes de error (on/off)
 - :command:`log.output =` Salida del registro de ejecución, las opciones son: default (file), file y screen para pantalla.



 ::

	[Log] 

	# Establece la ruta absoluta donde reside el archivo de registro 
	log.filepath = /home/pbuitrago/.safet/log 

	# Habilita la opción de registro de mensajes de depuración 
	log.debug = on 

	# Habilita la opción de registro de mensajes de acciones 
	log.action = on 

	# Habilita la opción de registro de mensajes de advertencia 
	log.warning = on 

	# Habilita la opción de registro de mensajes de error 
	log.error = on 

	# Salida del registro de ejecución, las opciones son: default (file), file y screen 
	log.output = file 



 :command:`4.- [XmlRepository]:` (Repositorio xml)

	Información sobre repositorio de documentos firmados electrónica mente bajo el formato **"bdoc"** (firma electrónica xml) asociados a flujos de trabajos.

 ::
	
	[XmlRepository]
	# En caso de utilizar un repositorio de documentos XML remoto se debe 
	# especificar la informacion de acceso al mismo a traves de un servicio web
	xmlrepository.remote.ip = 150.187.36.6
	xmlrepository.remote.urlwebservice = http://150.187.36.6/cgi-bin/safet

	# Establece el tipo de repositorio de documenfet_auth_conftos XML. Valores posibles:
	# dir: para repositorio basado en un directorio local
	# dbxml: para repositorio DBXML
	xmlrepository.type = dir

	# Establece la ruta absoluta al repositorio XML

	xmlrepository.path = <HOME>.safet/xmlrepository

	# Establece el nombre del repositorio XML
	xmlrepository.name = container1

 :command:`5.- [Input]:` (Entrada de datos)

	Opciones para la entrada/modificación de datos

 ::

	[Input]
	# Establece la ruta absoluta para el directorio en la entrada/modificacion de datos
	input.path = <HOME>.safet/input
	#input.file = deflista.xml
	input.file = deftrac.xml

	
	
 :command:`6.- [System]:` (Opciones generales para la aplicación Inflow)

	 Opciones generales referentes al funcionamiento del tipo de aplicación : Consola/Web/Gráfica.

 ::	

	[System]
	system.evalexit = on  -----> # Preguntar al salir de la aplicación "inflow"

	# Información referente a la base de datos o repositorio 
	# para el acceso de la librería

	

 :command:`7.- [Widgets]:` (objetos para entrada de datos)

 ::

	[Widgets]

	# Lector de archivo en el sistema
	# lista de archivos de acceso rápido 
	widgets.getfilewidget.* = <HOME>.safet/flowfiles/mensajes.xml


	widgets.texteditwidget.wiki.leftbold = '''		---> # Introducción para el widget "wiki"
	widgets.texteditwidget.wiki.rightbold = '''			""		""	  ""
	widgets.texteditwidget.wiki.leftitalic = ''			""		""	  ""
	widgets.texteditwidget.wiki.rightitalic = ''			""		""     	  ""					
	widgets.texteditwidget.wiki.leftunderline = __			""		""	  ""
	widgets.texteditwidget.wiki.rightunderline = __			"" 		""	  ""

 
 :command:`8.- [GeneralOptions]:` (Opciones generales)

	Esta sección incluye  parámetro de uso generar.

 ::

	[GeneralOptions]
	
	generaloptions.consoleactions.* = Gráfico Mensajes;operacion:Generar_gráfico_coloreado   ---------> # Consultas guardadas 
	Cargar_archivo_flujo: <HOME>.safet/flowfiles/mensajes.xml

	generaloptions.consoleactions.* = Grafico por clave;operacion:Generar_gráfico_para_clave 
	Cargar_archivo_flujo: <HOME>.safet/flowfiles/mensajes.xml Clave: 9	---------> # Consultas guardadas 

	# Titulo para el gráfico de flujo de trabajo

	generaloptions.currentflowtitle = Tareas Proyecto SAFET

	# Opciones On (Incluir en el gráfico, No incluir), Off si no se quiere incluir

	generaloptions.currentflowtitle.font = Dejavu Sans

	# Tamaño de la fuente para el texto de informacin en cada flujo de trabajo
	generaloptions.currentflowtitle.size = 18

	# Separación de la fuente para el texto de informacin en cada flujo de trabajo
	generaloptions.currentflowtitle.separation = 10

	# Directorio donde sealmacenan los archivos  de salida grafos(.png,svg) 
	generaloptions.dir.media = <HOME>PySafet

	generaloptions.currentflowtitle.include = off
	# Ver http://en.wikipedia.org/wiki/List_of_tz_zones_by_name
	# Zona horaria
	generaloptions.timezone = America/Caracas

	# mostrar información estadística en el grafo
	generaloptions.extrainfo.showonly = off
	generaloptions.extrainfo.showhumanize = on

	# Mostrar el dialogo para los parámetros para cada documento de flujo de trabajo

	# Se colocan las opciones generales del sistema SAFET (Lista de acciones guardadas, entre otros)



 :command:`9.- [Stats]:` ( Uso de las estadística para los grafos)

	Valores para el calculo de estadísticas

 ::

	[Stats]

	#Activar la recolección de estadísticas
	stats.actived = off 
	#Fecha de inicio de calculo de estadística, * significa que no hay fecha colocada
	stats.fromdate = *
	#Fecha de fin de calculo de estadística, * significa que no hay fecha colocada
	stats.todate = *



 :command:`10.- [Libdigidoc]:` (Parámetro para la firma electrónica)

	Establece variables de configuración para la librería Libdigidoc utilizada por Libsafet.	

 :: 

	[Libdigidoc]
	# Ruta del archivo de configuración digidoc para usarlo con protocolo OCSP
	libdigidoc.configfi:q
	lepath = <HOME>.safet/digidoc.conf

	# directorio donde se almacenan los certificados de validación 
	libdigidoc.x509storepath =  <HOME>.safet/certs

	# Tipo de validación de firma de Digidoc "ocsp" : via protocolo OCSP, "keystore": Repositorio
	# de claves del navegador Mozilla / Firefox
	#libdigidoc.validationtype = keystore

	# Tipo de archivos "MIME" permitidos 
	libdigidoc.mimestypes.* = application/vnd.sun.xml.writer sxw
	libdigidoc.mimestypes.* = application/vnd.sun.xml.writer.template stw
	libdigidoc.mimestypes.* = application/vnd.sun.xml.writer.global sxg
	libdigidoc.mimestypes.* = application/vnd.stardivision.writer sdw vor
	libdigidoc.mimestypes.* = application/vnd.stardivision.writer-global sgl
	libdigidoc.mimestypes.* = application/vnd.sun.xml.calc sxc
	libdigidoc.mimestypes.* = application/vnd.sun.xml.calc.template stc
	libdigidoc.mimestypes.* = application/vnd.stardivision.calc sdc
	libdigidoc.mimestypes.* = application/vnd.sun.xml.impress sxi
	libdigidoc.mimestypes.* = application/vnd.sun.xml.impress.template sti
	libdigidoc.mimestypes.* = application/vnd.stardivision.impress sdd sdp
	libdigidoc.mimestypes.* = application/vnd.sun.xml.draw sxd
	libdigidoc.mimestypes.* = application/vnd.sun.xml.draw.template std
	libdigidoc.mimestypes.* = application/vnd.stardivision.draw sda
	libdigidoc.mimestypes.* = application/vnd.sun.xml.math sxm
	libdigidoc.mimestypes.* = application/vnd.stardivision.math smf
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.text odt
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.text-template ott
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.text-web oth
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.text-master odm
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.graphics odg
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.graphics-template otg
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.presentation odp
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.presentation-template otp
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.spreadsheet ods
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.spreadsheet-template ots
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.chart odc
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.formula odf
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.database odb
	libdigidoc.mimestypes.* = application/vnd.oasis.opendocument.image odi
	libdigidoc.mimestypes.* = application/pdf pdf
	libdigidoc.mimestypes.* = application/xml xml
	libdigidoc.mimestypes.* = text/plain txt
	libdigidoc.mimestypes.* = text/css css
	libdigidoc.mimestypes.* = text/xml xml
	libdigidoc.mimestypes.* = text/html html 
	libdigidoc.mimestypes.* = application/x-gzip tgz
	libdigidoc.mimestypes.* = application/zip zip


 :command:`11.- [Autofilter]:` (Opciones para los Auto filtros)

	Sirve para realizar clasificaciones automáticas.

 ::


	[Autofilter]
	autofilter.datetime.period = 672


 :command:`12.- [DefaultValues]:` (Valores por defecto) 
	
	Opciones que se toman por defecto.

 ::

	[DefaultValues]

	defaultvalues.report = yes
	defaultvalues.digidoc.manifest = Investigador
	defaultvalues.digidoc.city = Mérida
	defaultvalues.digidoc.state = Mérida
	defaultvalues.digidoc.country = VE
	defaultvalues.digidoc.zip = 5101
	defaultvalues.panel.info.secondstohide = 4000

 :command:`13.- [Reports]:` (Reportes)

	Opcion que es para la generación de reportes.

 ::

 	[Reports]
	# Configuraciones para mostrar información en la aplicación Inflow

	# tipo de protocolo (file,http,ftp,entre otros)
	reports.protocol = file 

	# ruta para obtener la plantilla (HTML)
	reports.path = <HOME>.safet/reports

	# nombre de la plantilla HTML + AJAX
	reports.general.template = <HOME>.safet/reports/sf_plantillaLista01.html

	# nombre de la plantilla para documento a firmar
	reports.documenttosign.template = <HOME>.safet/reports/sf_plantillaFirma01.html

	# transformar fecha numerica a formato de fecha
	reports.options.datetransform = on
	reports.options.dateformat = dd/MM/yyyy


 :command:`14.- [Graphs]:` (Grafos)

	Opciones Para los gráficos de flujo de trabajo

 :: 

	[Graphs]

	# opciones para el texto de información en cada flujo de trabajo
	# opciones on/off
	graphs.infotext.include = on

	# Formato  para el texto de informacin en cada flujo de trabajo (el %date indica Fecha y %time Hora, %datetime Hora y Fecha)
	graphs.infotext.format = Generado en %datetime

	# Nombre de la fuente para el texto de información en cada flujo de trabajo
	graphs.infotext.font = Book Antiqua

	# Tamaño de la fuente para el texto de información en cada flujo de trabajo
	graphs.infotext.size = 14


	# Separación de la fuente para el texto de información en cada flujo de trabajo
	graphs.infotext.separation = 30


 :command:`15.- [Ftp]:` (Transferencia FTP)

	Opción para la trasferencia de archivos utilizando el protocolo `FTP`_.

.. _FTP: http://es.wikipedia.org/wiki/File_Transfer_Protocol

 ::

	[Ftp]

	ftp.default.server = victorbravo.info
	ftp.default.account = victorrbravo


 :command:`16.- [ExtraInfo]:` (Información estadísticas de los grafos)

	Parametros de la información extra de cada nodo

 ::

	[ExtraInfo]

	extrainfo.infotext.fields = owner,porcentage,completed
	extrainfo.infotext.separator = 

	extrainfo.infotext.completed = [*]

	# Mostrar campos coloreados (lo ya pasados)
	extrainfo.coloured = yes

	# Mostrar estadísticas al final del grafo (resumen)
	extrainfo.summarize = yes

	extrainfo.title = Porcentaje:

	extrainfo.formula = sumall

	extrainfo.pattern = #PAR0#CLA00-9#CLA1+#PAR1#SLA%


 :command:`17.- [Plugins]:` (Complementos o componentes)

	Opciones para las librerias adicionales Plugins.


 :: 

	[Plugins]
	# Ruta donde se encuentra las librerías adicionales (plugins)
	plugins.path = /usr/lib/libsafet


 :command:`18.- [Plugins.Graphviz]:` (Componentes Graphviz)
	
	Opciónes para la generación de grafos utilizando la biblioteca  `Graphviz`_.

.. _Graphviz: http://www.graphviz.org/

 ::
	
	[Plugins.Graphviz]

	# Formato de archivo de salida de grafo, valores posibles: svg/png/gif
	plugins.graphviz.graphtype = svg

	# Información a mostrar en cuadro de información extra Porc,Tokens,Total,InfoText,InfoDate
	plugins.graphviz.extrainfo.show = Porc,Tokens,Total,InfoText,InfoDate


	#Color activo de para ser utilizado en los gráficos 
	plugins.graphviz.task.fillcolor = #f1f1f1

	#Color activo de la lnea para ser utilizado en los gráficos 
	plugins.graphviz.task.color = black


	# Atributo utilizado en la estadística Opciones posibles (Color/Size/Line/Custom)
	plugins.graphviz.stats.attribute = Color

	# Tamaño máximo para la estadística de (Tamaño Máximo)
	plugins.graphviz.stats.sizemax = 2 

	# Tamaño mínimo  para la estadística de (Tamaño Mínimo)
	plugins.graphviz.stats.sizemin = 1

	# Color para dibujar estadística
	plugins.graphviz.stats.colorgradient = yellow

	# Color del texto para cuadro de estadística
	plugins.graphviz.stats.fontcolor = black

	# Color de fondo  para cuadro de estadística
	plugins.graphviz.stats.fillcolor = antiquewhite

	# Estilo de la línea del cuadro de estadística 
	plugins.graphviz.stats.style = dashed

	# Mostrar cuadro de estadística
	plugins.graphviz.showstats = yes


	#Dirección del grafo  TB (Arriba-Abajo) LR (Izquierda-Derecha)
	plugins.graphviz.graph.rankdir = TB

	# Tamao del fuente para todos los nodos
	plugins.graphviz.graph.fontsize = 12

	# Separador del rank  
	plugins.graphviz.graph.ranksep = 0.5 equally


	# Figura para la tarea (Task)
	plugins.graphviz.task.shape = box

	# Estilo de la Figura para la tarea (Task) filled,bold,dotted,empty
	plugins.graphviz.task.style = filled

	#Color activo de para ser utilizado en los gráficos 
	plugins.graphviz.condition.fillcolor = #FFFFFF 

	#Color activo de la línea para ser utilizado en los gráficos 
	plugins.graphviz.condition.color = black

	# Figura para la (Condition) (box, ellipse, circle, etc.)
	plugins.graphviz.condition.shape = ellipse

	# Estilo de la Figura para la tarea (Condition)
	plugins.graphviz.condition.style = filled

	# Mostrar la información extra solo donde existan tokens (fichas) 
	plugins.graphviz.extrainfo.showwheretokens = on

	#Mostrar la estadística en el nodo "FINAL", valores admitidos (on/off)
	plugins.graphviz.extrainfo.showfinal = on






