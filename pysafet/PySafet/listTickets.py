
import Safet
import os
myhome = os.getenv("HOME")
mymedia = myhome + "/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)

myinflow.setMediaPath(mymedia )
myinflow.setHostURL(myurl) 

myconsult = u"operacion:Listar_datos Cargar_archivo_flujo: %s/.safet/flowfiles/cartelerafirmada.xml Variable: vTicketSigned " % (myhome)


result = myinflow.login("fulano","fulano") 

if not result:
	print "Authentication failed"
	exit()

result = myinflow.toInputConsole(myconsult)

if not result:
	print "Consult failed error: %s"  % (myinflow.currentError())
	exit()

print u"%s" % (myinflow.currentJSON())
