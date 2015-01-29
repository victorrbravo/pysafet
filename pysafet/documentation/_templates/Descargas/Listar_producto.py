
import Safet
import os

myhome = os.getenv("HOME")
mymedia = myhome + "/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)
myinflow.setMediaPath(mymedia)
myinflow.setHostURL(myurl)



myconsult = u"operacion:Listar_datos Cargar_archivo_flujo:/home/cenditel/.safet/flowfiles/productos.xml Variable: vRegistrado"

result = myinflow.login("admin","admin")

if not result:
	print "Autentication Failed"
	exit()

result = myinflow.toInputConsole(myconsult)

if not result:
	print "consulta failed error: %s" % (myinflow.currentError())
	exit()

print u"%s" % (myinflow.currentJSON())
