# -*- encoding: utf-8 -*-
import Safet
import os
myhome = os.getenv("HOME")
mymedia = myhome + "/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)

myinflow.setMediaPath(mymedia )
myinflow.setHostURL(myurl) 

#myconsult = u"operacion:Listar_datos Cargar_archivo_flujo: %s/.safet/flowfiles/carteleratresf.xml Variable: vToDo " % (myhome)
myconsult = u"operacion:Generar_gráfico_con_autofiltro Cargar_archivo_flujo: /home/vbravo/.safet/flowfiles/carteleraplanificados.xml Autofiltro:por_tentativo parameters.ByPeriod:week"
#myconsult = u"operacion:Generar_gráfico_con_autofiltro Cargar_archivo_flujo: /home/vbravo/.safet/flowfiles/carteleratodos.xml Autofiltro:por_proyecto"
#myconsult = u"operacion:Generar_gráfico_coloreado Cargar_archivo_flujo: /home/vbravo/.safet/flowfiles/carteleratodos.xml parameters.ByProject:2"
#myconsult = u"operacion:Listar_datos_con_autofiltro  Cargar_archivo_flujo:/home/vbravo/.safet/flowfiles/cartelerafiltros.xml Autofiltro:por_finalizado Variable:vfinishdateWeek_5"
myconsult = u"operacion:Listar_datos_con_autofiltro  Cargar_archivo_flujo: /home/vbravo/.safet/flowfiles/cartelerafiltros.xml Autofiltro:por_finalizado  Variable:vTodas_las_tareas parameters.ByDate1:1419120000"

print myconsult

result = myinflow.login("vbravo","marinone") 

if not result:
	print "Authentication failed"
	exit()

result = myinflow.toInputConsole(myconsult)

if not result:
	print "Consult failed error: %s"  % (myinflow.currentError())
	exit()

print u"Result:\n%s" % (myinflow.currentJSON())
