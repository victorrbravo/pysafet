# -*- coding: utf-8 -*-

import Safet
import os
myhome = os.getenv("HOME")
mymedia = myhome + "/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)

myinflow.setMediaPath(mymedia )
myinflow.setHostURL(myurl) 

myconsult = u"operacion:Generar_gráfico_coloreado Cargar_archivo_flujo: %s/.safet/flowfiles/productosp.xml  parameters.Empieza_por:P" % (myhome)

result = myinflow.login("admin","admin") 

if not result:
	print "Authentication failed"
	exit()

result = myinflow.toInputConsole(myconsult)

if not result:
	print "Consult failed error: %s"  % (myinflow.currentError())
	exit()

print u"%s" % (myinflow.currentJSON())
