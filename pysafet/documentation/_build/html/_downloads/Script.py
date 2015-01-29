# -*- coding: utf-8 -*-

import Safet
import os

myhome = os.getenv("HOME")
mymedia = myhome + "/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)

myinflow.setMediaPath(mymedia)
myinflow.setHostURL(myurl)

result = myinflow.login("admin","admin")


# C(Insertar)
# myconsult = u"operacion:agregar_producto  Nombre: ComplejoB"


# D(Borrar o eliminar)
# myconsult = u"operacion:borrar_producto id:5"

# U(Actualizar)
myconsult = u"operacion:modificar_producto id:3 Nombre:Amoxicilina" 


# +(Flujo de trabajo)
# myconsult = u"operacion:Actualizar_producto id:3 Estado_producto:Pedido"

if result:
	result = myinflow.toInputForm(myconsult)
else:
	print "\n ---Usuario autenticado---\n"
	exit()


if result:
	print "\n --Se Actualizo correctamente el producto---\n"
else:
	print "\n No se Actualizo el producto....!!!\n"


if not result:
	print "\nConsulta failed error: %s\n" % (myinflow.currentError())
	exit()

print u"  %s\n" % (myinflow.currentJSON())

