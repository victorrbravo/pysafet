========================================================
:index:`Paso para la creación del directorio .safet/`
========================================================


:command:`1.- Paso:`

     	::
	
		Descargamos nuestro MensajesVersionInicial con estención .tar 

        **DOWNLOAD:**

    	.. image::
		  ../../_static/download.png
		  
  		
    	:download:`MensajesVersionInicial.TAR <../Descargas/mensajesVersionInicial.tar>`





:command:`2.- Paso:`	    

 ::

	//Creamos un carpeta Pysafet en nuestro HOME
	$ make Pysafet

	.........................................................
	//Copiamos nuestro archivo (.tar) que descargamos a nuestra carpeta Pysafet. En este caso se descargo en la carpeta "Descargas". 
	$ cp Descargas/mensajesVersionInicial.tar Pysafet/

	.........................................................
	// Acedemos a la carpeta Pysafet
	$ cd Pysafet/

	........................................................
	// Crearemos un archivo (.py) que lo llamares (MontarSafet.py)
	Pysafet$ touch MontarSafet.py


:command:`3.- Paso:`

- :Abrimos nuestro archivo (MontarSafet.py) y copiamos y guardamos el siguinte codigo:

  >>> import Safet
  ... import os
  ...
  ... myhome = os.getenv("HOME")
  ... mymedia = myhome + "/tmp"
  ... myurl = "http://localhost"
  ...
  ... myinflow = Safet.MainWindow(myhome)
  ...
  ... myinflow.setMediaPath(mymedia)
  ... myinflow.setHostURL(myurl)
  ...
  ... myinflow.doLoadConfiguration("mensajesVersionInicial.tar")


- :Explicación del código:

  >>> #Importación la librería Safet y os
  ... import Safet
  ... import os
  ...
  ... myhome = os.getenv("HOME") #
  ... mymedia = myhome + "/tmp"
  ... myurl = "http://localhost"
  ...
  ... myinflow = Safet.MainWindow(myhome)
  ...
  ... myinflow.setMediaPath(mymedia)
  ... myinflow.setHostURL(myurl)
  ...
  ... myinflow.doLoadConfiguration("mensajesVersionInicial.tar")


:command:`4.- Paso:`

 ::

	// Se compila el archivo (Montar.py)
	Pysafe$ python Montar.py

	.........................................................
	
	// Nos vamos al "HOME"  a verificar si monto el (.safet/)
	Pysafet$ ls ../.safet/


	



	




	
