
import Safet
import os
import json


def jsonToJquery(myoldarray):
	mynewarray  = []

	stringcolumn = ""
	currcolumns = []
	currkeys = []
	for reg in myoldarray:
		currkeys = reg.keys()
		myvalue = ""
		regarray = []
		for currkey in currkeys: 		
			myvalue  = reg[currkey]
			regarray.append(myvalue)
		mynewarray.append(regarray)


	for currkey in currkeys:
		currcolumn = { "sTitle" : currkey }
		currcolumns.append(currcolumn)

	return (mynewarray,currcolumns)


def writeJsonData(data,columns,filename):
	stringnewarray = u"%s" %  data
	stringcolumns = u"%s" % columns

	file = open(filename, "w")

	datatowrite  = u"dataInventory = %s\n\ncolumnInventory = %s" % ( stringnewarray, stringcolumns )

	datatowrite =  datatowrite.replace("u'","'")

	datatowrite = datatowrite + "\n"
	file.write(datatowrite)

	file.close()




myhome = os.getenv("HOME")
mymedia = myhome + "/tmp"
myurl = "http://localhost"

myinflow = Safet.MainWindow(myhome)

myinflow.setMediaPath(mymedia )
myinflow.setHostURL(myurl) 

myconsult = u"operacion:Listar_datos Cargar_archivo_flujo: %s/.safet/flowfiles/productos.xml Variable: vPedido " % (myhome)


result = myinflow.login("admin","admin") 

if not result:
	print "Authentication failed"
	exit()

result = myinflow.toInputConsole(myconsult)

if not result:
	print "Consult failed error: %s"  % (myinflow.currentError())
	exit()

mystringdata = u"%s" % myinflow.currentJSON();
mydata = json.loads(mystringdata)
print mydata["safetlist"]

(mynewarray,currcolumns) = jsonToJquery(mydata["safetlist"])

writeJsonData(mynewarray,currcolumns,"dataInventory.js")


