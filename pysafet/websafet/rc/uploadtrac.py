#!/usr/bin/python
# -*- coding: utf-8  -*-
import sys
from mechanize import Browser
#from BeautifulSoup import BeautifulSoup
TRACURL = "http://seguridad.cenditel.gob.ve/rootve"

if len(sys.argv)  < 5 or len(sys.argv) > 6:
    print "error:Se debe especificar cuatro (4) o cinco (5) argumentos  <usuario> <contraseña> <ticket> <ruta_completa_archivo>  [<trac_url>]"
    sys.exit(0)

myname = sys.argv[1]
mypass = sys.argv[2]
myticket = sys.argv[3]
myfile = sys.argv[4]
if len(sys.argv) == 6:
	TRACURL = sys.argv[5]

try:
	print "%s/login" % (TRACURL) 
	br = Browser()
	br.open("%s/login" % (TRACURL))
except:
	print "error:Problemas con la librerias (Mechanize) de Python"
	sys.exit(1)

# follow second link with element text matching regular expressioni
for form in br.forms():
    br.form = form
br["user"]= myname
br["password"]= mypass
text1 = ""
try:
	response1 = br.submit()
	text1 = response1.read();
except:
	print "error: Ocurrio una falla enviando datos de autenticacion"
	sys.exit(1)

if (text1.find("Error") != -1 ):
	print "error:Autenticación  para ""%s"" fallo" % (myname)
	sys.exit(0)    

attachurl =  "%s/attachment/ticket/%s/?action=new&attachfilebutton=Attach+file" % (TRACURL,myticket)
br.open(attachurl)
try:
	for form in br.forms():
#	    print form
#	    print ""
	    br.form = form
#	br.form = br.forms().next()
	#br["attachment"] = "/home/vbravo/imagenesSafet/tickets.png"
#	help(br.form.add_file)
        f = open(myfile)
	br.form.add_file(f,None,myfile)	
except StopIteration:
	print "error:Un error ocurrio al tratar de subir el archivo"
	sys.exit(0)
try:
	response2 = br.submit()
	text2 = response2.read()
except:
	print "error:Ocurrio una falla enviando los datos para la subida del archivo"
	sys.exit(1)


if (text2.find("Error") != -1 ):
	print "error:La página no realizo la subida correctamente"
else:
	print "ok:Archivo ""%s"" se subió correctamente " % (myfile)
	

