==========================================================
:index:`Listar los datos de la tabla de la base de datos`
==========================================================



 **Script para listar el producto**
 
 - **Operación:** Listar_datos 
 - **Cargar_archivo_flujo:** Url donde se ubica el archivo.
 - **Variable:** vRegistrado


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
 ... # Operación
 ... myconsult = u"operacion:Listar_datos Cargar_archivo_flujo:/home/cenditel/.safet/flowfiles/productos.xml Variable: vRegistrado"  
 ...
 ... result = myinflow.login("admin","admin")
 ... 
 ... if not result:
 ...	 print "Autentication Failed"
 ...     exit()
 ...	 
 ... result = myinflow.toInputConsole(myconsult)
 ...
 ... if not result:
 ... print "consulta failed error: %s" % (myinflow.currentError())
 ... exit()
 ...
 ... print u"%s" % (myinflow.currentJSON())


 
