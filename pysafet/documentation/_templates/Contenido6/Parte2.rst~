
=====================================================
:index:`Operaciones CRUD+(flujo) utilizando PySafet` 
=====================================================


 Editamos el archivo **deftrac.xml** que se encuentra en el directorio **<HOME>.safet/input/**

 **1.- Escribimos el encabezado (información de Autor).**

  ::

	<?xml version="1.0" encoding="UTF-8" ?>
	<!--
	Documento  : deftrac.xml
	Creado     : Fulano de tal
	Autor      : Fulano de tal
	Descripcion: Archivo de Entrada para SAFET - Inflow
	-->


 - **Escribimos la ruta al validador XML(formato DTD)**

  ::

	
	<!DOCTYPE operations SYSTEM "file:///home/cenditel/.safet/dtd/yawlinput.dtd">


 - **Ahora podemos empezar a escribir las operaciones siguientes:**

  - C(Insertar)
  - R(Listar*)
  - U(Actualizar)
  - D(Borrar o eliminar)
  - +(Flujo de trabajo)


  ::

	<operations suffix=":" commandname="operacion">


	<operation  name="Productos"  desc="Agregar,Modificar,Eliminar,Listar" icon="firmadoc.png"> </operation>



 **2.- Operation Insertar "Productos" C(Insertar)**

 Las operaciones en PySafet consiste en una lista de comandos que se ejecutan secuencialmente.
	
 Los comandos se componen de campos **"Fields"** que corresponden a los diferentes  tipos de datos básicos y complejos.  Por ejemplo analicemos el código **XML** del archivo **deftrac.xml** (operación agregar pruducto). Se define los siguiente:

	**1.-** El tipo de comando es **"agregar".(type)**, los tipos posibles son **"agregar","actualizar" y "eliminar"**.

	**2.-** La tabla de la base de datos donde se realizará el comando **"command"** es **"productos"**.
	
	**3.-** Luego se especificar la lista de campos **"fields"**. Para este campo se especificaran dos campos, el nombre del producto **"Nombre"**, y el estado del producto que tomará el valor literal **"literal", "Registrado"**.

	**4.-** El segundo comando de la operación **"Agregar_producto"**  también es del tipo **"Agregar"** y ojo los campos que son necesarios para registrar el eventos **"Agregar_producto"** en la tabla **"productos_registro"**.

	**5.-** La palabra de PySafet **"_USERNAME"** se utiliza para obtener el usuario actual

	
 A continuación se mostrara el archivo **deftrac.xml**:
  ::

	<operation  name="agregar_producto"  desc="" icon="plus.png">
		
		<command id ="1" type="agregar" table="productos" >

			<fields>

				<field type="string" icon="resumen.png" mandatory="yes"	validation="" 
											title="Nombre" desc="">
				nombre
				</field>

				<field type="string" literal="Registrado" mandatory="yes" >
				status
				</field>
	
			</fields>

		</command>

		<command id ="1" type="agregar" table="productos_registro" >

	    	<fields>

		  <field type="datetime" mandatory="yes"  
					function="seq from sqlite_sequence where name='productos'"  input="no">
		      productoid
		      </field>

		      <field type="datetime" mandatory="yes"  function="datetime('now')" 
									format="time_t" input="no">
		       fecha
		      </field>

		      <field type="string" literal="_USERNAME" mandatory="yes" >
		      rol      
		      </field>
	  
		      <field type="string" literal="Registrado" mandatory="yes" >
		      regstatus
		      </field>

		   </fields>
		
		</command>

	</operation>


 

 - **Ejecutamos el Script para Insertar un producto**
	
	Seguidamente vamos a utilizar esta operación **"agregar_producto"** en un Script de python como se muestra a continuación:
	
  - **Operación:** agregar_producto 
  - **Nombre:** El nombre del Producto agregar

	>>>     # Importación de la librería Safet y os
	...  1) import Safet
	...  2) import os
	...  3)
	...  4) # Aqui octengo mi home,media y url
	...  5) myhome = os.getenv("HOME")
	...  6) mymedia = myhome + "/tmp"
	...  7) myurl = "http://localhost"
	...  8)
	...  9) # Constructor principal
	... 10) myinflow = Safet.MainWindow(myhome)
	... 11)
	... 12) #
	... 13) myinflow.setMediaPath(mymedia)
	... 14) myinflow.setHostURL(myurl)
	... 15)
	... 16) # Si no es un usuario registrado el metodo "login" retorna False
	... 17) result = myinflow.login("admin","admin")
	... 18)
	... 19) # Agregamos el poducto a ingresar por ejemplo "Champu Olorin",
	... 20) #                    "ComplejoB","Aspirina","Acetaminofén","Ibuprofeno".
	... 21) # 
	... 22) myconsult = u"operacion:agregar_producto Nombre: Champu Olorin"  ----->> Operaciones
	... 23) 
	... 24) if result:	  
	... 25)	  #
	... 26) 	  result = myinflow.toInputForm(myconsult)
	... 27) else:	  
	... 28)	  print "\n ---Usuario autenticado---\n"
	... 29)	  exit()
	... 30)
	... 31) if result:
	... 32)	 #
	... 33)	 print "\n  --Se inserto correctamente el producto---\n"
	... 34) else:
	... 35)	   #
	... 36)	  print "\n No se inserto el producto....!!!\n"
	... 37)
	... 38)  #
	... 39) if not result:
	... 40)  	 print "\nConsulta de (failed error): %s\n" % (myinflow.currentError())
	... 41)     exit()
	... 42)  
	... 43)
	... 44) print u"  %s\n" % (myinflow.currentJSON())
	... 45)




 Crearemos un archivo **".py"** con cualquier nombre y copiamos el **Script** y lo ejecutamos de la siguiente manera:
  ::

	$ python Script_Insertar_producto.py
	

 - **Observen la siguiente imagen:**

	.. image::
		  ../../_static/insertar1.png
		  :height: 200px
		  :width: 1200px

	:Figura 25: Insertar producto.

 - **Observen la base de datos en la siguiente imagen:**


     .. _figura1:
     .. figure:: ../../_static/lista_producto3.png
          :align: center
          :height: 480px
	  :width: 850px

          **Figura 26: Tabla productos**



   .. image::
		  ../../_static/lista_producto4.png
		  :align: center
		  :height: 480px
		  :width: 850px

   :Figura 27: Tabla productos_registro.




 **3.- Operation Eliminar "Productos" D(Borrar o eliminar)**


  ::

	
	<operation  name="borrar_producto"  desc="Elimina un ticket por id" icon="clear.png"> 

		<command id ="1" type="eliminar" table="productos">

			<fields>

				<field type="combolisttable" options="id:productos::id || ' - ' || nombre" 
									mandatory="yes" primarykey="yes" title="id" >
				id
				</field>		

			</fields>

		</command>

		<command id ="1" type="eliminar" table="productos_registro">

			<fields>

				<field type="string"  mandatory="yes" title="id" >
				productoid
				</field>		
		
			</fields>

		</command>

	</operation>


 - **Ejecutamos el Script para elimiar el producto**
	
	Seguidamente vamos a utilizar esta operación **"borrar_producto"** en un Script de python como se muestra a continuación:
	
  - **Operación:** borrar_producto 
  - **id:** Aqui colocaremos el valor de **id** del producto por ejemplo borraremos el producto **Ibuprofeno** y su **id** es **5**. Observe la :ref:`figura1` 


	>>>     # Importación de la librería Safet y os
	...  1) import Safet
	...  2) import os
	...  3)
	...  4) # Aqui octengo mi home,media y url
	...  5) myhome = os.getenv("HOME")
	...  6) mymedia = myhome + "/tmp"
	...  7) myurl = "http://localhost"
	...  8)
	...  9) # Constructor principal
	... 10) myinflow = Safet.MainWindow(myhome)
	... 11)
	... 12) #
	... 13) myinflow.setMediaPath(mymedia)
	... 14) myinflow.setHostURL(myurl)
	... 15)
	... 16) # Si no es un usuario registrado el metodo "login" retorna False
	... 17) result = myinflow.login("admin","admin")
	... 18)
	... 19) # Se cambio esta operación ahora es eliminar con el id del producto
	... 20) myconsult = u"operacion:borrar_producto id:5" ----->> Operaciones
	... 21)
	... 22) #
	... 23) if result:
	... 24)	  #
	... 25)	  result = myinflow.toInputForm(myconsult)
	... 26) else:
	... 27)	  #
	... 28)	  print "\n ---Usuario autenticado---\n"
	... 29)	  exit()
	... 30)
	... 31) if result:
	... 32)	 #
	... 33)	 print "\n  --Se borro correctamente el producto---\n"
	... 34) else:
	... 35)	   #
	... 36)	  print "\n No se borro el producto....!!!\n"
	... 37)
	... 38)  #
	... 39) if not result:
	... 40)  	 print "\nConsulta de (failed error): %s\n" % (myinflow.currentError())
	... 41)     exit()
	... 42)  
	... 43)
	... 44) print u"  %s\n" % (myinflow.currentJSON())
	... 45)



  ::

	$ python Eliminar_producto.py
	

 **Observen la siguiente imagen:**

	.. image::
		  ../../_static/Eliminar.png
		  :align: center
		  :height: 300px
		  :width: 800px

	:Figura 27: Script Eliminar producto.


 - **Observen en la db en la siguiente imagen:** Observe la :ref:`figura2`


     .. _figura2:
     .. figure:: ../../_static/Eliminar1.png
          :align: center
          :height: 480px
	  :width: 850px

          **Figura 26: Tabla productos**


 - **Observen en la db en la siguiente imagen:** Observe la :ref:`figura3`


     .. _figura3:
     .. figure:: ../../_static/Eliminar2.png
          :align: center
          :height: 480px
	  :width: 850px

          **Figura 27: Tabla productos_registro**




**4.- Operation Actualizar "Nombre del Producto" U(Actualizar)**

  ::

	<operation  name="modificar_producto"  desc="" icon="plus.png">

		<command id ="1" type="actualizar" table="productos" >
	
			<fields>

				<field type="combolisttable" options="id:productos::id || ' - ' || nombre" 
							mandatory="yes" primarykey="yes" title="id" order="desc">
				id
				</field>

				<field type="string" icon="resumen.png" mandatory="yes" validation="" 
									title="Nombre" desc="">
				nombre
				</field>

			</fields>
		
		</command>

	</operation>


 - **Ejecutamos el Script para Actualizar el nombre producto**
	
	Seguidamente vamos a utilizar esta operación **"Actualizar_producto"** en un Script de python como se muestra a continuación:
	
  - **Operación:** modificar_producto
  - **id:** Aqui colocaremos el valor de **id** del producto por ejemplo vamos a actualizar el producto **Aspirina** y su **id** es **3**. Observe la :ref:`figura1` 
  - **Nombre:** Aqui se coloca el nombre al que le vamos a modificar por ejemplo **Amoxacilina** por **Aspirina**.
 


	>>>     # Importación de la librería Safet y os
	...  1) import Safet
	...  2) import os
	...  3)
	...  4) # Aqui octengo mi home,media y url
	...  5) myhome = os.getenv("HOME")
	...  6) mymedia = myhome + "/tmp"
	...  7) myurl = "http://localhost"
	...  8)
	...  9) # Constructor principal
	... 10) myinflow = Safet.MainWindow(myhome)
	... 11)
	... 12) #
	... 13) myinflow.setMediaPath(mymedia)
	... 14) myinflow.setHostURL(myurl)
	... 15)
	... 16) # Si no es un usuario registrado el metodo "login" retorna False
	... 17) result = myinflow.login("admin","admin")
	... 18)
	... 19) # Se cambio esta operación para actualizar el nombre del producto 
	... 20) # 
	... 21) myconsult = u"operacion:modificar_producto id:3 Nombre:Amoxicilina"  ----->> Operaciones
	... 22) 
	... 23) if result:
	... 24)	  #
	... 25)	  result = myinflow.toInputForm(myconsult)
	... 26) else:
	... 27)	  #
	... 28)	  print "\n ---Usuario autenticado---\n"
	... 29)	  exit()
	... 30)
	... 31) if result:
	... 32)	 #
	... 33)	 print "\n  --Se actualizo el nombre del prodcuto correctamente---\n"
	... 34) else:
	... 35)	   #
	... 36)	  print "\n No actualizo el nombre del prodcuto....!!!\n"
	... 37)
	... 38)  #
	... 39) if not result:
	... 40)  	 print "\nConsulta de (failed error): %s\n" % (myinflow.currentError())
	... 41)     exit()
	... 42)  
	... 43)
	... 44) print u"  %s\n" % (myinflow.currentJSON())
	... 45)




  ::

	$ python Modificar_producto.py
	

 **Observen la siguiente imagen:**

	.. image::
		  ../../_static/Modificar.png
		  :align: center
		  :height: 300px
		  :width: 800px

	:Figura 29: Script Actualizar producto.


 - **Observen en la db en la siguiente imagen:** Observe la :ref:`figura6`


     .. _figura6:
     .. figure:: ../../_static/Modificar1.png
          :align: center
          :height: 480px
	  :width: 850px

          **Figura 26: Tabla productos**



 **5.- Operation Actualizar "Productos" +(Flujo de trabajo)**

  ::

	<operation  name="Actualizar_producto" desc="Pasa de estado un determinado ticket" icon="padlock.png"> 

		<command id ="1" type="actualizar" table="productos">

			<fields>

				<field type="combolisttable" options="id:productos::'('	|| id || ')' ||  nombre" 
								mandatory="yes" primarykey="yes" order="desc">
				id
				</field>		

				<field type="comboflow" mandatory="yes" options="next" 
					path="/home/cenditel/.safet/flowfiles/productos.xml" title="Status_producto">
				status
				</field>

			</fields>
		</command>

		<command id ="1" type="agregar" table="productos_registro" >

			<fields>

				<field type="datetime" mandatory="yes"  function="seq from sqlite_sequence where name='productos'"
															input="no">
				 productoid
				</field>

				 <field type="datetime" mandatory="yes"  function="datetime('now')" 
									format="time_t" input="no">
				  fecha
				 </field>

				 <field type="string" literal="_USERNAME" mandatory="yes" >
				  rol      
				 </field>

				 <field type="string" title="Status"  mandatory="yes" >
				 regstatus
				 </field>

		  </fields>

		</command>
	
	</operation>


 - **Ejecutamos el Script para Actualizar el Status del producto**
	
	Seguidamente vamos a utilizar esta operación **"Actualizar_producto"** en un Script de python como se muestra a continuación:
	
  - **Operación:** Actualizar_producto 
  - **id:** Aqui colocaremos el valor de **id** del producto por ejemplo vamos a actualizar el producto **Aspirina** y su **id** es **3**. Observe la :ref:`figura1` 
  - **Estado_producto:** Aqui se coloca el estado del producto es decir si es por llegar,Agotarse,Pedido,En Espera. En este caso esta **Registrado** vamos a modificarlo a **pedido**.
 


	>>>     # Importación de la librería Safet y os
	...  1) import Safet
	...  2) import os
	...  3)
	...  4) # Aqui octengo mi home,media y url
	...  5) myhome = os.getenv("HOME")
	...  6) mymedia = myhome + "/tmp"
	...  7) myurl = "http://localhost"
	...  8)
	...  9) # Constructor principal
	... 10) myinflow = Safet.MainWindow(myhome)
	... 11)
	... 12) #
	... 13) myinflow.setMediaPath(mymedia)
	... 14) myinflow.setHostURL(myurl)
	... 15)
	... 16) # Si no es un usuario registrado el metodo "login" retorna False
	... 17) result = myinflow.login("admin","admin")
	... 18)
	... 19) # Se cambio esta operación para actualizar producto 
	... 20) # 
	... 21) myconsult = u"operacion:Actualizar_producto id:3 Status_producto:Pedido"----->> Operaciones
	... 22) 
	... 23) if result:
	... 24)	  #
	... 25)	  result = myinflow.toInputForm(myconsult)
	... 26) else:
	... 27)	  #
	... 28)	  print "\n ---Usuario autenticado---\n"
	... 29)	  exit()
	... 30)
	... 31) if result:
	... 32)	 #
	... 33)	 print "\n  --Se Actualizo correctamente el Status del producto---\n"
	... 34) else:
	... 35)	   #
	... 36)	  print "\n No Actualizo el Status del producto....!!!\n"
	... 37)
	... 38)  #
	... 39) if not result:
	... 40)  	 print "\nConsulta de (failed error): %s\n" % (myinflow.currentError())
	... 41)     exit()
	... 42)  
	... 43)
	... 44) print u"  %s\n" % (myinflow.currentJSON())
	... 45)
	... 46)  
	... 47)
	... 48)  
	... 49)
	... 50)  



  ::

	$ python Actualizar_Status_producto.py
	

 **Observen la siguiente imagen:**

	.. image::
		  ../../_static/Actualizar.png
		  :align: center
		  :height: 300px
		  :width: 800px

	:Figura 29: Script Actualizar producto.


 - **Observen en la db en la siguiente imagen:** Observe la :ref:`figura4`


     .. _figura4:
     .. figure:: ../../_static/Actualizar1.png
          :align: center
          :height: 480px
	  :width: 850px

          **Figura 26: Tabla producto**


 - **Observen en la db en la siguiente imagen:** Observe la :ref:`figura5`


     .. _figura5:
     .. figure:: ../../_static/Actualizar2.png
          :align: center
          :height: 480px
	  :width: 850px

          **Figura 27: Tabla producto_registro**



 **Nota:**
   ::

	  Si tienen error aquí están los archivos
	
 - Archivo deftrac:
  
	**DOWNLOAD:**

    	.. image::
		  ../../_static/download.png
		  
  		
    	:download:`deftrac.xml <../Descargas/deftrac.xml.tar.gz>`
  
 - Archivo Script:

	**DOWNLOAD:**

    	.. image::
		  ../../_static/download.png
  		
    	:download:`Script.py <../Descargas/Script.py>`






