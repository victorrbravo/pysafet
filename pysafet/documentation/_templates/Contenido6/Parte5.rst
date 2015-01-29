
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
Ver fichas usando flujos de trabajo
,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

En los pasos anteriores obtenemos los datos de dos **acciones** **vRegistrado** y **vPedido**, la cual utilizamos como ejemplo como se muestra en la siguiente imágenes:

 .. note:: **vRegistrado**

     .. _figura41:
     .. figure:: ../../_static/json.png
          :align: center
          :height: 200px
          :width: 600px

          **Figura 28: vRegistrado**

    **La siguiente** :ref:`figura41` **indica lo siguiente:**

    - Nos mostrara el nombre de las columnas.

    - Nos mostrara los datos de cada columna.

    - Nos mostrara un buscador **Search**.

    - Nos mostrara **Show** que nos indicara 
      cuantos datos quiere que se muestren en la tabla.


 .. note:: **vPedido**

     .. _figura44:
     .. figure:: ../../_static/json1.png
          :align: center
          :height: 200px
          :width: 750px

          **Figura 29: vPedido**

    **La siguiente** :ref:`figura44` **indica lo siguiente:**

    - Nos mostrara el nombre de las columnas, más el atributo **cantidad**.

    - Nos mostrara los datos de cada columna.

    - Nos mostrara un buscador **Search**.

    - Nos mostrara **Show** que nos indicara 
      cuantos datos quiere que se muestren en la tabla.



**A continuación vamos a demostrar, como obtener fichas de esa tabla de datos:**

++++++++++++++++++++++++++++++
Ficha (vRegistrado) (vPedido)
++++++++++++++++++++++++++++++

Los paso para obtener una ficha que vamos a seguir mas adelante, nos sirven para las demás acciones menos una, es decir, no solo para la acción **vRegistrado** sino tambien para las acciones **(vDisponible,vPor_llegar,vPor_agotarse,vAgotado)**. Para la acción **vPedido** los pason son diferentes ya que ah esta acción se agrega el el atributo **cantidad**, porque para mostrar los datos depende del valor cantidad.

Las demas acciones tienen, el mismo esquemas de mostrar los datos en fichas como se muestra en la siguiente imagen :ref:`figura45`:

.. note:: La imagen nos muestra el nombre de la ficha la cual sirve para las acciones **(vDisponible,vPor_llegar,vPor_agotarse,vAgotado)**, la cual tienen los mismos atributos que las demás acciones.


     .. _figura45:
     .. figure:: ../../_static/ficha1.png
          :align: center
          :height: 150px
          :width: 400px

          **Figura 30: Ficha vRegistrado**



   La imagen muestra el nombre de la ficha y sus atributos: :ref:`figura50`


     .. _figura50:
     .. figure:: ../../_static/ficha2.png
          :align: center
          :height: 200px
          :width: 400px

          **Figura 32: Ficha vPedido**

----------------
1° PRIMER PASO
----------------


- Nos vamos directorio que hemos creado anteriormente que se llama **JsonInterfaz/** 

 .. code-block:: bash

	$ cd $HOME/JsonInterfaz


----------------
2° SEGUNDO PASO
----------------

- Ejecutamos el Scritp que insertamos en el archivo que creamos anteriormente **.py(ScriptJson.py)**, con la varible **(vRegistrado)** o con la la variable **vPedido** o la que guste. 

 .. code-block:: python


	JsonInterfaz$ python ScriptJson.py


----------------
3° TERCER PASO
----------------

- Crearemos un archivo **.css** por ejemplo **(style.css)**, en el directorio **JsonInterfaz/**:

 .. code-block:: bash

	JsonInterfaz$ touch style.css

- Abrimos el archivo que creamos **.css(style.css)**, insertamos el siguiente código:

 .. code-block:: css

	body {
		background-repeat:no-repeat;
		background-position:top center;
		background-color:#657077;
		margin:40px;
	}

	#tabbed_box_1 {
		margin: 0px auto 0px auto;
		width:300px;
	}
	.tabbed_box h4 {
		font-family:Arial, Helvetica, sans-serif;
		font-size:23px;
		color:#ffffff;
		letter-spacing:-1px;
		margin-bottom:10px;
	}
	.tabbed_box h4 small {
		color:#e3e9ec;
		font-weight:normal;
		font-size:9px;
		font-family:Verdana, Arial, Helvetica, sans-serif;
		text-transform:uppercase;
		position:relative;
		top:-4px;
		left:6px;
		letter-spacing:0px;
	}
	.tabbed_area {
		border:1px solid #494e52;
		background-color:#636d76;
		padding:8px;	
	}

	ul.tabs {
		margin:0px; padding:0px;
		margin-top:5px;
		margin-bottom:6px;
	}
	ul.tabs li {
		list-style:none;
		display:inline;
	}
	ul.tabs li a {
		background-color:#464c54;
		color:#ffebb5;
		padding:8px 14px 8px 14px;
		text-decoration:none;
		font-size:9px;
		font-family:Verdana, Arial, Helvetica, sans-serif;
		font-weight:bold;
		text-transform:uppercase;
		border:1px solid #464c54;
		background-repeat:repeat-x; 
		background-position:bottom;
	}
	ul.tabs li a:hover {
		background-color:#2f343a;
		border-color:#2f343a;
	}
	ul.tabs li a.active {
		background-color:#ffffff;
		color:#282e32;
		border:1px solid #464c54; 
		border-bottom: 1px solid #ffffff;
		background-repeat:repeat-x;
		background-position:top;	
	}
	.content {
		background-color:#ffffff;
		padding:10px;
		border:1px solid #464c54; 	
		font-family:Arial, Helvetica, sans-serif;
		background-repeat:repeat-x;	 
		background-position:bottom;	
	}
	#content_2, #content_3 { display:none; }

	.content ul {
		margin:0px;
		padding:0px 20px 0px 20px;
	}
	.content ul li {
		list-style:none;
		border-bottom:1px solid #d6dde0;
		padding-top:15px;
		padding-bottom:15px;
		font-size:13px;
	}
	.content ul li:last-child {
		border-bottom:none;
	}
	.content ul li a {
		text-decoration:none;
		color:#3e4346;
	}
	.content ul li a small {
		color:#8b959c;
		font-size:9px;
		text-transform:uppercase;
		font-family:Verdana, Arial, Helvetica, sans-serif;
		position:relative;
		left:4px;
		top:0px;
	}
	.content ul li a:hover {
		color:#a59c83;
	}
	.content ul li a:hover small {
		color:#baae8e;
	}


----------------
4° CUARTO PASO
----------------



- Crearemos un archivo **.html** por ejemplo **(Fichas.html)**, en el directorio **JsonInterfaz/**:

 .. code-block:: bash

	JsonInterfaz$ touch FichavRegistrado.html


- Abrimos el archivo que creamos **.html(Fichas.html)**, insertamos el siguiente código:

 .. code-block:: html

	<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
	     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

	<html xmlns="http://www.w3.org/1999/xhtml"
	                 xml:lang="en" lang="en">
	<head>

	<meta http-equiv="Content-Type" content="text/html; 
	                              charset=ISO-8859-1">
	<title>Fichas</title>
	<link rel="stylesheet" type="text/css" media="screen">


	<script src="fichas/dataInventory.js"> </script>

	<script>


	function Fichas(){

	var valor = columnInventory.length;
	var valor1 = dataInventory.length-1;
	var j = 0, flecha = false;

	if (columnInventory[j].sTitle == "cantidad"){
	  flecha = true; 

	  for (var i = 0; i < valor; i++ ){

	   if( flecha == true && i == 2){ 

	    document.write("<li> <font color = 'blue'>"
	       + columnInventory[j].sTitle + ": </font>"
	       + dataInventory[valor1][j]+ "</li>");

	    document.write("<li> <font color = 'blue'>"
	     + columnInventory[i+1].sTitle + ": </font>"
	     + dataInventory[valor1][i+1]+ "</li>");
	   }
	  else{
	    document.write("<li> <font color = 'blue'>" 
	           + columnInventory[i+1].sTitle + ": </font>" 
	           + dataInventory[valor1][i+1]+ "</li>");
	  }
	 }

	}
	else{
	  for (var i = 0; i < valor; i++ ){
	   document.write("<li> <font color = 'blue'>"
	      + columnInventory[i].sTitle + ": </font>"
	      + dataInventory[j][i]+ "</li>");
	  }
	}

	}
	</script>
	</head>
	<body>

	<div id="tabbed_box_1" class="tabbed_box">
	<h4>Ficha de la tabla 
	<script> document.write(Variable[0]); 
	</script></h4>

	<div class="tabbed_area">
	<ul class="tabs">
	<li><a class="ta ctive">Ficha</a></li>

	</ul>

	<div id="content_1" class="content">
	<ul>
	<script> Fichas();</script>
	</ul> 

	</div>
	</div>
	</div>

	</body>
	</html>

 .. note:: **Este código sirve para todas la variables, se utilizo estilo css y se muestran los datos del archivo dataInventory.js utilizando código javascript.**

--------------------
5° QUINTO PASO
--------------------


- Abrimos el archivo **FichavRegistrado.html** con cualquier navegador web y se nos mostrara de la siguiente forma: :ref:`figura47`

 .. note:: 

  - Esta imagen nos muestra los datos de una fila de la tabla **vRegistrado**, por ejemplo los datos de la primera fila en forma de una ficha:

     .. _figura47:
     .. figure:: ../../_static/fichavRegistrado.png
          :align: center
          :height: 300px
          :width: 600px

          **Figura 31: Ficha de un registro**

    **La siguiente** :ref:`figura47` ** indica lo siguiente:**

    - Nos mostrara el **id** y su valor

    - Nos mostrara el **Nombre** y su valor.

    - Nos mostrara el **Status** y su valor.



  - Esta imagen nos muestra los datos de una fila de la tabla **vPedido**, por ejemplo los datos de la primera fila en forma de una ficha:

     .. _figura48:
     .. figure:: ../../_static/fichavPedido.png
          :align: center
          :height: 300px
          :width: 600px

          **Figura 31: Ficha de un pedido**

    **La siguiente** :ref:`figura48` ** indica lo siguiente:**

    - Nos mostrara el **id** y su valor

    - Nos mostrara el **Nombre** y su valor.

    - Nos mostrara el **Cantidad** y su valor.

    - Nos mostrara el **Status** y su valor.


- Aquí esta los 2 ejemplos de las fichas le damos **clik** algunas de ellas:

 - `FichavRegistrado.html`_
 - `FichavPedido.html`_



.. _FichavPedido.html: FichasPedido.html
.. _FichavRegistrado.html: FichasRegistrado.html






















