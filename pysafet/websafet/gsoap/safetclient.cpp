#include "soapH.h" // obtain the generated stub 
#include "safet.nsmap"

int main(int argc, char **argv) 
{ 
   struct soap soap;// = soap_new();
   bool result;
   //double a, b, result;

   //a = strtod(argv[2], NULL);
   //b = strtod(argv[3], NULL);

   //char str[256]; //= argv[1];
   //strcpy (str,argv[1]);
   //printf("valor de str: %s \n", str);

   //char * result;
   char * str = "/tmp/safet.xml";
   char * str2 = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\
<libsafetdb>\
<Table>\
<nombre_orgarnizacion>Sistema Nacional de Certificacion Electronica</nombre_orgarnizacion>\
<pais>248</pais>\
<estado>9</estado>\
<localidad>Caracas</localidad>\
<sector>Gobierno</sector>\
<direccion>Av Universidad. Esquina El chorro. Torre MCT. Piso 8</direccion>\
<telefono_1>0212-5688026</telefono_1>\
<correo_e>suscerte@suscerte.gob.ve</correo_e>\
</Table>\
<Table>\
<nombre_orgarnizacion>Fundacion Instituto de Ingenieria</nombre_orgarnizacion>\
<pais>248</pais>\
<estado>14</estado>\
<localidad>Caracas</localidad>\
<sector>Gobierno</sector>\
<direccion>Sartenejas</direccion>\
<telefono_1>5042222</telefono_1>\
<correo_e>pki@fii.org</correo_e>\
</Table>\
</libsafetdb>";

   //if ( soap_call_ns__hola(soap_new(), "http://localhost/cgi-bin/holamundo", "", str, result) == SOAP_OK)
   if ( soap_call_ns__guardarXml(soap_new(), NULL, NULL, str2, "nombreDocumento", result) == SOAP_OK)
   {
	if (result)
		printf("result = verdadero\n");
	else
		printf("result = false\n");
   }
   else
   {
	//printf("\nERROR != SOAP_OK\n");
	soap_print_fault(&soap, stderr);
    	exit(1);
   }
  soap_destroy(&soap);
  soap_end(&soap);
  soap_done(&soap);
  return 0;

}

