#-*- coding: utf-8 -*-
from PyQt4.QtCore import *
import Safet
import os
import sys

if len(sys.argv) < 3:
	print "La sintaxis es safetxml1.py <Tarea_Anterior> <TareaNueva>"
	sys.exit(0)

myinflow = Safet.MainWindow(u"..")
result = myinflow.login(u"vbravo",u"zpinquar")
#if not result:
#   return u"Falla en la autenticación"

#s = QString(os.getcwd()+"/flujoviaticosgeneral.xml")
s = QString("../.safet/flowfiles/flujoAppPlanificacionSoftware.xml")
print s
myinflow.addNodeToXMLWorkflow(s, sys.argv[1],sys.argv[2],False)

