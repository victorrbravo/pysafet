# -*- coding: utf-8 -*-

import Safet
import os
myhome = os.getenv("HOME")
mymedia = myhome + "/github/website/static/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)

myinflow.setMediaPath(mymedia )
myinflow.setHostURL(myurl) 


#myconsult = u"operacion:Generar_gráfico_con_autofiltro Cargar_archivo_flujo:/home/vbravo/.safet/flowfiles/tendencias_af.xml Autofiltro:por_sub"
myconsult = u"operacion:Generar_gráfico_coloreado Cargar_archivo_flujo:/home/vbravo/.safet/flowfiles/eventos_publicaciones.xml"


result = myinflow.login("vbravo","marinone") 

if not result:
	print "Authentication failed"
	exit()

result = myinflow.toInputConsole(myconsult)

if not result:
	print "Consult failed error: %s"  % (myinflow.currentError())
	exit()

print u"%s" % (myinflow.currentJSON())
