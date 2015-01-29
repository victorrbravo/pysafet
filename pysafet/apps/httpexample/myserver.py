#!/usr/bin/env python
# -*- coding: utf-8 -*-
import tornado.httpserver
import tornado.ioloop
import tornado.web

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("""
<form name="input" action="/loadsignature" method="post">
id: <input type="text" name="id"><br>
status: <input type="text" name="status">
resumen: <input type="text" name="summary">
<input type="submit" value="Enviar"/>
</form> 
""")

class SignatureHandler(tornado.web.RequestHandler):
    def post(self):
	 idfield = self.get_argument("id", None)
	 statusfield =  self.get_argument("status", None)
	 summaryfield =  self.get_argument("summary", None)
	 mymessage = idfield + statusfield + summaryfield 
         mypage = u"""
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>SAFET - Firma electronica</title>
<script src="https://gestion.cenditel.gob.ve/media/jquery-latest.js"></script>
<script src="https://gestion.cenditel.gob.ve/media/Concurrent.Thread.js"></script>
<link type="text/css" href="https://gestion.cenditel.gob.ve/media/css/ui-lightness/jquery-ui-1.8.8.custom.css" rel="stylesheet" />
<script src="https://gestion.cenditel.gob.ve/js/jquery.ui.custom.js"></script>
<script src="https://gestion.cenditel.gob.ve/media/jquery.jec-1.3.1.js"></script>
<script src="https://crypto-js.googlecode.com/svn/tags/3.1.2/build/rollups/sha1.js"></script>
<script type="text/javascript">

var hasfirst = false;
var namebdoc = "";

function plugin() {
   return document.getElementById('plugin');
}

function calculateDataToSign(message) {
	mydata = message + "<SAFETSEPARATOR/>" + getdocumentid(message);
	document.getElementById('textareadoc').value = mydata;

	console.log(mydata);
}

function hex2num (s_hex) {

var myvalue =  parseInt(s_hex,16);
if (myvalue > 128 ) {
	myvalue = 128 - ( myvalue - 128);
}
return myvalue;

}


function showData() {
    document.getElementById('errorMessage').innerHTML = '';
    document.getElementById('errorCode').innerHTML = '0';
    try {
        var cert = plugin().getCertificate();
        for (var i = 0; i < names.length; i++) {
          name = names[i];
          document.getElementById(name).innerHTML = cert[name];
        }
        document.getElementById('PEM').innerHTML = cert.certificateAsPEM;
        var hex = cert.certificateAsHex; 
        var s = "";
        if (hex) for (var i = 0; i < hex.length; i += 64) s += hex.substr(i, 64) + "\\n";
        document.getElementById('hex').innerHTML = s;
    }
    catch (e) {
        document.getElementById('errorMessage').innerHTML = plugin().errorMessage;
        document.getElementById('errorCode').innerHTML = plugin().errorCode;
    }
}



function showVersion() {
    document.getElementById('version').innerHTML = plugin().version;
}

function getdocumentid(message) {


    var hash = CryptoJS.SHA1(message).toString();

var tableid = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
var output = "";

for (i = 0; i < hash.length/2; i++) {
	var index = 0;
	var mynum = hex2num(hash.substring(i*2,i*2+2) );
	if ( (mynum % 62) > 0 ) {
		index  = mynum % 62; 
	 }
	 else {
		index =  (mynum  % 62)*-1;			
	}

	output = output + tableid[ index].toString();
}
	console.log("HASH (Salida) OUTPUT:|" + output +".bdoc|");
	return output + ".bdoc";
}


function createData() {
                var cert = plugin().getCertificate();
                var mypem   = cert.certificateAsPEM;

		try {
			console.log("createData");
			namebdoc = getdocumentid('<SAFETPAR1/>');
			 $.post("https://gestion.cenditel.gob.ve/bdoc/create/testdoc", {idaction:1,content:document.getElementById('textareadoc').value,id:12, path: namebdoc, pem: mypem }, function(data) { console.log("data"); document.getElementById('hash').value = data; hasfirst = true; } );
		} catch(e) {
			alert(e);
		}
}


function signFinal() {
	
	if ( !hasfirst) {
		alert("Debe ejecutar el botón (1) Verificar Integridad antes de ejecutar esta acción");
		return;
	}
 
    document.getElementById('errorMessage').innerHTML = '';
    document.getElementById('errorCode').innerHTML = '0';
    var hash = document.getElementById('hash').value;


    try {
      var id = plugin().getCertificate().id;
      var mysignature = plugin().sign(id, hash, { language: 'en' }); // , { language: 'et' }

      var s = "";
      $.post("https://gestion.cenditel.gob.ve/bdoc/finalize",{idaction:2,signature: mysignature}, function(data) {alert("hola"); alert(data); } );

      alert('¡Firma electrónica realizada exitosamente! archivo:' + namebdoc);
    }
    catch (e) {
	alert(plugin.errorMessage);
    }
}




</script>




</head>
<body onload="showVersion(); calculateDataToSign('<SAFETPAR2/>')" >
<input type="textarea" name="textareadoc"  id="textareadoc" value=""/>
<input type="button" onclick="createData()" value="(1) Verificar Integridad"/>
<input type="button" onclick="signFinal()" value="(2) Firmar Electrónicamente"/>
hash: <input type="text" readonly="" size="80" maxlength="128" id="hash" name="hash" value="n/a">
<object id="plugin" type="application/x-digidoc" width="1" height="1"></object><br/>
Plug-in version: <span id="version"></span>
<br/>
<br/>
<a href="#" onclick="javascript:showData()">show data</a> <br/>

<table id="data"></table>
<div>errorMessage: <span id="errorMessage"></span></div>
<div>errorCode: <span id="errorCode"></span></div>

<div>PEM:</div>
<pre id="PEM"></pre>

<div>DER in hex:</div>
<pre id="hex"></pre>
</body>
</html>

""" 
	 mypage = mypage.replace("<SAFETPAR1/>", mymessage)
	 mypage = mypage.replace("<SAFETPAR2/>", mymessage) 
	 self.write(mypage)	
 


application = tornado.web.Application([
    (r"/", MainHandler),
    (r"/loadsignature", SignatureHandler),
])

if __name__ == "__main__":
    http_server = tornado.httpserver.HTTPServer(application, ssl_options={
        "certfile": "server.crt",
        "keyfile": "server.key",
    })
    http_server.listen(8888)
    tornado.ioloop.IOLoop.instance().start()


