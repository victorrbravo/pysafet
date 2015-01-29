# -*- coding: utf-8 -*-
from PyQt4.QtCore import *
import Safet
import os
mystr = u"operacion:Listar_datos Cargar_archivo_flujo: ../.safet/flowfiles/flujobloggeneral.xml Variable: VariableFinalizado"
myform = QString(mystr)
myinflow = Safet.MainWindow("..")
result = myinflow.login("admin","zpinquar")
if result:
	isinserted = myinflow.toInputConsole(mystr)
	if isinserted:
		print "Consulta Realizada...OK"
		print "JSON:\n%s" % (myinflow.currentJSON())
		
	else:
		print "Consulta NO Realizada"
