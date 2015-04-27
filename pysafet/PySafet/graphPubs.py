# -*- coding: utf-8 -*-

import Safet
import os
myhome = os.getenv("HOME")
mymedia = myhome + "/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)

myinflow.setMediaPath(mymedia )
myinflow.setHostURL(myurl) 


myconsult = u"operacion:Generar_gráfico_con_autofiltro Cargar_archivo_flujo:/home/victor/.safet/flowfiles/tendencias_af.xml Autofiltro:por_sub"


result = myinflow.login("vbravo","zpinquar") 

if not result:
	print "Authentication failed"
	exit()

result = myinflow.toInputConsole(myconsult)

if not result:
	print "Consult failed error: %s"  % (myinflow.currentError())
	exit()

print u"%s" % (myinflow.currentJSON())
