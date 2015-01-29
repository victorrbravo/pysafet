==============================================================
:index:`Crear el flujo de trabajo utilizando un archivo XML`
==============================================================

	Visualizamos el archivo **productos.xml** que se encuentra en el directorio **<HOME>.safet/flowfiles/** consta de los siguiente:

 #. **Encabezado:**

	::
 

		<?xml version='1.0' encoding='UTF-8'?>
		<!DOCTYPE yawl SYSTEM 'file:///home/cenditel/.safet/dtd/yawlworkflow.dtd'>
		<!--
		Documento  : tickets.xml
		Creado     : 16/10/08 09:27 AM
		Autor      : nombre_autor
		Descripcion: Archivo generado por plantilla de la Libreria SAFET
		-->

		<!--<!DOCTYPE yawl SYSTEM "http://trac-safet.cenditel.gob.ve/dtd/yawlworkflow.dtd">-->
	

 #. **Condición inicial:**

	::
	
		# Etiqueta principal yawl 
		<yawl version="0.01">

		 # Nombre del flujo							
		 <workflow id="productos">

		  # ficha: Nombre de la tabla  y Nombre del campo	
		  <token keysource="productos" key="id"/>

		  # Condición inicial	
		  <condition type="start" id="inicial">

		   <port side="forward" type="split">
  
	            # Slq donde el inicio me aparecerá en el grafo apuntando a registrado
	             <connection query="select status from productos" options="Registrado" source="Registrado"/>
   
	            </port>

		 </condition>
	
 #. **Registrado:**

	::

	   # Condición de Registrado la cual puede pasar un caso
	   <task title="en inventario" id="Registrado">

	    # Registrado apunta a una opción
	     <port side="forward" type="split">

		   # Cambio de estado ahora Registado  apunta a Pedido
		   <connection query="select status from productos" options="Pedido" source="Pedido"/>

	      </port>

	     # Variable vRegistro donde me aparecerá un mensaje la hora en el cuaal se hizo ese registro		 
	     <variable config="1" documentsource="select id,mensaje,status from productos" type="sql" tokenlink="" id="vRegistrado"
                rolfield="(select rol from productos_registro where productoid=productos.id and regstatus='Registrado')
                as rol" scope="task" timestampfield="(select fecha from 
                productos_registro where productoid=productos.id and regstatus='Registrado') as fecha"/>

	   </task>

		  
		  
 #. **Pedido:**

	::

	  # Condición de Pedido la cual pueden pasar 2 casos
	  <task title="" id="Pedido" textualinfo="">

		# Producto pedido apunta a dos opciones
	  	 <port pattern="or" side="forward" type="split">
			
		# Producto pedido apunta a Disponible
		<connection query="select status from productos" options="Disponible" source="Disponible"/>
			
		# Producto pedido apunta a Por_llegar			
		<connection query="select status from productos" options="Por_llegar" source="Por_llegar"/>

	  	 </port>

		 # Variable vpedido donde nos aparecerá un mensaje el id , el status y fecha y hora de esa acción 
		 <variable config="1" documentsource="select id,mensaje,status from productos" type="sql" tokenlink="" id="vPedido"
                     rolfield="(select rol from&#xa;productos_registro where productoid=productos.id and regstatus='Pedido')
                     as rol" scope="task"timestampfield="(select fecha from productos_registro where productoid=productos.id 
                     and regstatus='Pedido') as fecha"/>
	
	  </task>



 #. **Por_llegar:**

	::

		# Producto Por_llegar la cual puede pasar un caso		
		<task title="" id="Por_llegar" textualinfo="">
		
		 # Por_llegar apunta a una opción
		 <port pattern="none" side="forward" type="split">
			
			# Producto Por_llegar apunta a 	Por_agotarse
			<connection query="select status from productos" options="Por_agotarse" source="Por_agotarse"/>
		   
		 </port>

		   # Variable vPor_llegar donde nos aparecerá un mensaje el id , el status y fecha y hora de esa acción 
		   <variable config="1" documentsource="select id,mensaje,status from productos" type="sql" tokenlink="" id="vPor_llegar"
                         rolfield="(select rol from productos_registro where productoid=productos.id and regstatus='Por_llegar')
                         as rol" scope="task" timestampfield="(select fecha from productos_registro where productoid=productos.id
                         and regstatus='Por_llegar') as fecha"/>

		</task>
		

 #. **Disponible:**

	
	::
	
	  # Producto Disponible la cual puede pasar un caso 
	  <task title="" id="Disponible" textualinfo="">

	     # Disponible apunta a una opción
	     <port side="forward" type="split">

		# Producto Disponible apunta a 	Por_agotarse		
		<connection query="select status from productos" options="Por_agotarse" source="Por_agotarse"/>

	     </port>

	     # Variable vDisponible donde nos aparecerá un mensaje el id , el status y fecha y hora de esa acción 
	     <variable config="1" documentsource="select id,mensaje,status from productos" type="sql" tokenlink="" id="vDisponible"
                      rolfield="(select rol from&#xa;productos_registro where productoid=productos.id and regstatus='Disponible') 
                      as&#xa;rol" scope="task" timestampfield="(select fecha from productos_registro where&#xa;productoid=productos.id 
                      and regstatus='Disponible') as fecha"/>
	
	  </task>



 #. **Por_agotarse:**


	::

	  # Producto Por_agotarse la cual puede pasar un caso
	  <task title="" id="Por_agotarse" textualinfo="">

	    # Disponible apunta a una opción	   
	    <port side="forward" type="split">

		 # Producto Por_agotarse apunta a Agotado
		 <connection query="select status from productos" options="Agotado" source="Agotado"/>

	    </port>

	    # Variable vPor_agotarse donde nos aparecerá un mensaje el id , el status y fecha y hora de esa acción	
	    <variable config="1" documentsource="select id,mensaje,status from productos" type="sql" tokenlink="" id="vPor_agotarse"
		rolfield="(select rol from&#xa;productos_registro where productoid=productos.id and regstatus='Por_agotarse') 
		as&#xa;rol" scope="task" timestampfield="(select fecha from productos_registro where&#xa;productoid=productos.id 
		and	regstatus='Por_agotarse') as fecha"/>

	   </task>
	


 #. **Agotado:**

	::

	  # Producto Agotado la cual puede pasar un caso	
	  <task title="" id="Agotado" textualinfo="">

	   # Agotado apunta a dos opción
	   <port side="forward" type="split">

		# Producto Agotado apunta a Pedido
		<connection query="select status from productos" options="Pedido" back="yes" source="Pedido"/>

		# Producto Agotado apunta a final
		<connection query="true" options="" source="final"/>

	   </port>

	   # Variable vAgotado donde nos aparecerá un mensaje el id , el status y fecha y hora de esa acción
	   <variable config="1" documentsource="select id,mensaje,status from productos" type="sql" tokenlink="" id="vAgotado"
		rolfield="(select rol from productos_registro where productoid=productos.id and	regstatus='Agotado')
		as rol" scope="task" timestampfield="(select fecha from productos_registro where productoid=productos.id 
		and regstatus='Agotado') as	fecha"/>

	   </task>



 #. **Condition Final:**

	::

	    # Condición final
	    <condition id="final">

	      <port side="forward" type="split">

	 	<connection source=""/>

	     </port>

	     </condition>

	   # Cerramos la etiqueta del flujo workflow
	  </workflow>

	   #
	   <configuration id="1" connectstring="psql:seguridad.cenditel.gob.ve:ticket:vbravo:vbravo_"/>

	  # Cerramos la principal etiqueta yawl
	  </yawl>
