

.. Safet_para_Ubuntu:

==========================
:index:`Archivos fuentes`
==========================
	

:index:`Instalación de las dependencias`
-------------------------------------------

	El código fuente de la librería Libsafet se encuentra alojada en una plataforma de desarrollo colaborativo de software (forja) llamada Github, esta plataforma utiliza como sistema de control de versión el software Git. 


- :Para la compilación se requiere que se instale las siguientes dependencias:

 ::

	libgraphviz-dev
	libtar-dev
	g++
	libglib2.0-dev
	x11proto-xext-dev
	libqt4-dev
	libssl-dev
	make
	python-qt4-sql
	python-sip-dev
	python-qt4-dev
	libqt4-sql-sqlite



- :Para instalar las dependencias desde la línea de comando o terminal (puede hacerlo con synaptics):

 :: 

	# aptitude install libgraphviz-dev libtar-dev g++ libglib2.0-dev x11proto-xext-dev 
	                   libqt4-dev libssl-dev make python-qt4-sql python-sip-dev python-qt4-dev libqt4-sql-sqlite  


- :Para clonar el repositorio de Libsafet debemos instalar el control de versiones git:

 ::

	$ sudo aptitude install git


- :Luego ubicado en el directorio de trabajo (donde se va a descargar los fuentes) ejecuta el comando:

 ::

	$ git clone https://github.com/victorrbravo/pysafet.git pysafet


	Dentro del directorio de trabajo se crea el directorio pysafet donde se tiene el clone de la librería Libsafet



:index:`Pasos para la compilación`
------------------------------------


- :Compilación de la libraría Libsafet:

 ::

	$ cd pysafet/websafet
	$ make clean
	$ qmake-qt4
	$ make
	$ cd ../PySafet/
	$  python configure.py
	$ make 
	$ sudo make install
	$ cd ../websafet
	$ sudo make install

:index:`Verificación de la instalación`
----------------------------------------


- :Se procede arrancar un interprete de Python, para la misma debe hacer los siguientes paso:

 ::

	$ python
	cenditel@CENDITEL:~/proyecto/pysafet/websafet$ python 
	Python 2.7.3 (default, Jan  2 2013, 13:56:14) 
	[GCC 4.7.2] on linux2 
	Type "help", "copyright", "credits" or "license" for more information. 
	>>> 

- :Dentro prompt de python (>>>), se importa la librería de Safet ejecutando el siguiente comando:

  >>> import Safet
  >>>


:index:`Funciones`
-------------------
-
  
  >>> import Safet
  >>> dir(Safet) 
  ['MainWindow', 'ParsedSqlToData', 'SafetDocument', 'SafetVariable', 'SafetWorkflow', 
  'SafetXmlObject', 'SafetYAWL', '__doc__', '__file__', '__name__', '__package__']




 





