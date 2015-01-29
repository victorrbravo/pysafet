
  
.. _Safet_para_Debian:


==============================================
:index:`Debian Wheezy 7.0 (64 bits/32bits)`
==============================================


- :Se Agrega el repositorio al archivo: `/etc/apt/sources.list`_
 
.. _/etc/apt/sources.list: http://linuxgnublog.org/como-anadir-repositorios-en-debian

 ::	

	deb http://tibisay.cenditel.gob.ve/repositorio wheezy main




- :Obtener la clave: `(GPG)`_

.. _(GPG): http://www.genbetadev.com/seguridad-informatica/manual-de-gpg-cifra-y-envia-datos-de-forma-segura 

:del repositorio Debían, utilizando la siguiente línea de comando:
 ::

	# wget http://tibisay.cenditel.gob.ve/repositorio/apt-seguridad.gpg.asc
	# apt-key add apt-seguridad.gpg.asc

	



- :Luego ejecutamos:

 :: 

	# aptitude update
	# aptitude install pysafet


.. image:: 
	      ../../_static/Pysafet.png
	      :width: 500px

:Figura 6: Instalación de pysafet.
	
:index:`Funciones`
--------------------

- :Importación: Dentro de la consola de python importamos safet para su uso
 
  >>> import Safet
  >>> dir(Safet) 
  ['MainWindow', 'ParsedSqlToData', 'SafetDocument', 'SafetVariable', 'SafetWorkflow', 
  'SafetXmlObject', 'SafetYAWL', '__doc__', '__file__', '__name__', '__package__']



- :Otra formar de mostrar las funciones:
  
  >>> for i in range(len(dir(Safet))):
  ...     dir(Safet)[i]
  ... 
  'MainWindow'
  'ParsedSqlToData'
  'SafetDocument'
  'SafetVariable'
  'SafetWorkflow'
  'SafetXmlObject'
  'SafetYAWL'
  '__doc__'
  '__file__'
  '__name__'
  '__package__'









