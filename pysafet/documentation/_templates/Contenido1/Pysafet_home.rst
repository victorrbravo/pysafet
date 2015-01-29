========================================================
:index:`Paso para la creación del directorio .safet/`
========================================================


++++++++++++++++
1° PRIMER PASO
++++++++++++++++

 Descargamos nuestro MensajesVersionInicial con estención .tar 

        **DOWNLOAD:**

    	.. image::
		  ../../_static/download.png
		  
  		
    	:download:`ProductoVersionInicial.TAR <../Descargas/mensajesVersionInicial.tar>`


++++++++++++++++
2° SEGUNDO PASO
++++++++++++++++

- Creamos un directorio con cualquier nombre por ejemplo **Pysafet** en nuestro **<HOME>**

 .. code-block:: bash

	$ make $HOME/Pysafet


- Copiamos nuestro archivo **(.tar)** que descargamos a nuestra carpeta **<HOME>Pysafet/**. En este caso se descargo en la carpeta **"Descargas"**.

 .. code-block:: bash

	$ cp Descargas/ProductoVersionInicial.tar $HOME/Pysafet/

- Acedemos a la carpeta Pysafet

 .. code-block:: bash

	$ cd Pysafet/




++++++++++++++++
3° TERCER PASO
++++++++++++++++


- Crearemos un archivo **(.py)** por ejemplo **"MontarSafet.py"** dentro del directorio **<HOME>safe/**:

 .. code-block:: bash

	Pysafet$ touch MontarSafet.py


- Abrimos nuestro archivo **(MontarSafet.py)** y insertamos el siguiente Script de python:

 .. code-block:: python


	import Safet
	import os

	myhome = os.getenv("HOME")
	mymedia = myhome + "/tmp"
	myurl = "http://localhost"

	myinflow = Safet.MainWindow(myhome)

	myinflow.setMediaPath(mymedia)
	myinflow.setHostURL(myurl)

	myinflow.doLoadConfiguration("ProductoVersionInicial.tar")



- Ejecutamos el archivo **(MontarSafet.py)** desde al consola comando como usuario normal:


 .. code-block:: bash

	Pysafe$ python MontarSafet.py




-  Nos vamos al **<HOME>**  a verificar si monto el (.safet/) con el comando **ls**:

 .. code-block:: bash

	Pysafet$ ls $HOME/.safet/

	auth.conf digidoc.conf flowfiles images log     reports xmlrepository
	certs     dtd          graphs    input  mydb.db safet.conf



