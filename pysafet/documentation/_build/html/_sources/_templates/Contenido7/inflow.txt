================================
:index:`Instalación de Inflow`
================================

 **Pre-requisitos:**
    
	 Conocimiento básico de instalación de paquetes,  y actividades mínimas de administración.

 **Paquetes necesarios:**

  a) Librería Trolltech Qt >= 4.4  **(libqt4-dev)**.
  b) Librería de Conexión a Datos Qt PSQL **(libqt4-sql-psql)**.
  c) Librería de Conexión a Datos Qt SQLITE **(libqt4-sql-sqlite)**.
  d) Librería XML2 **(libxml2-dev)**.
  e) Librería de archivo tareados **(libtar-dev)**.  
  f) Librería Graphviz (**libgraphiz-dev**, para plugins de visualización de gráfico).
  g) Libreria **libtar-dev** (Utilidad **"tar"** para respaldo de archivos).
  h) Compilador **g++ y make**.

  ::

	Desde la shell de Unix, puede instalar todo el conjunto  de  paquetes necesarios 
	[descritos desde el paso "a" hasta el paso "h"], a través del siguiente comando:

	 # aptitude install g++ libqt4-dev libqt4-sql-psql libxml2-dev  libqt4-sql-sqlite libtar-dev libgraphviz-dev g++ make

  **Observen la siguiente imagen:**

  .. image::
		  ../../_static/inflow1.png
		  :height: 300px
		  :width: 1300px

  :Figura 25: Paquetes de inflow.
	

 **Primer paso:**
	
  ::
	
	Crear un directorio, en el cual almacenará los archivos fuentes.

       $ mkdir inflow

 **Segundo paso:**
	
  ::
	
	Descargar los fuentes (git ...)

	$ git clone https://bitbucket.org/victorrbravo/inflow.git	
	
  **Observen la siguiente imagen:**
	
  .. image::
		  ../../_static/inflow.png
		  :height: 200px
		  :width: 1200px

  :Figura 26: inflow.
    

 **Tercer paso:**
	
  ::	

	 Ir al directorio descomprimido cd inflow/
		$ cd inflow
		inflow$ 

 **Cuarto paso:**
	
  ::	

    Ejecutar "qmake" o "qmake-qt4" para generar el archivo de compilación (Makefile)

	    $ qmake-qt4

  **Observen la siguiente imagen:**
	
  .. image::
		  ../../_static/inflow2.png
		  :height: 200px
		  :width: 1200px

  :Figura 27: qmake-qt4.
    

 **Quinto paso:**
	
  ::	

    Si en la pantalla no se muestran errores, realizar la compilación con "make".

         $ make

  **Observen la siguiente imagen:**
	
  .. image::
		  ../../_static/inflow3.png
		  :height: 400px
		  :width: 1000px

  :Figura 28: make.
    

 **Sexto paso:**
	
  ::	

	Realizar la instalación en los directorios del sistema (es necesario tener permisos de administrador o "root")

     $ su  
         Escriba la contraseña de administrador y asegúrese que se está en el directorio safet-0.1.1.alpha)

 **Séptimo paso:**
	
  ::	

    Instalación como administrador

  	# make install
      	¡Si no se presenta ningún mensaje de error SAFET está instalado correctamente en su sistema!

  **Observen la siguiente imagen:**
	
  .. image::
		  ../../_static/inflow4.png
		  :height: 400px
		  :width: 1000px

  :Figura 29: make install.




	







