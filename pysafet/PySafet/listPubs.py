# -*- encoding: utf-8 -*-
import Safet
import os
myhome = os.getenv("HOME")
mymedia = myhome + "/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)

myinflow.setMediaPath(mymedia )
myinflow.setHostURL(myurl) 

#myconsult = u"operacion:Listar_datos Cargar_archivo_flujo: /home/victor/.safet/flowfiles/tendencias_sub.xml Variable: vPorCategoria"
myconsult = u"operacion:Listar_datos_con_autofiltro Cargar_archivo_flujo: /home/victor/.safet/flowfiles/tendencias_af.xml Autofiltro:por_sub Variable:v6"

print myconsult

result = myinflow.login("vbravo","zpinquar") 

if not result:
	print "Authentication failed"
	exit()

result = myinflow.toInputConsole(myconsult)

if not result:
	print "Consult failed error: %s"  % (myinflow.currentError())
	exit()

print u"Result:\n%s" % (myinflow.currentJSON())
