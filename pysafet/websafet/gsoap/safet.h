// Contents of file "safet.h":
//gsoap ns service name: safet
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded
//gsoap ns service port: http://localhost/cgi-bin/safet
//gsoap ns service namespace: urn:safet
typedef char * xsd__string;
typedef bool xsd__boolean;
typedef int xsd__int;

int ns__guardarXml(xsd__string in, xsd__string name, xsd__boolean &result);
int ns__verifyDocumentSignatures(xsd__string in, xsd__boolean &result);
int ns__verifyDocumentSignatureWithId(xsd__string in, xsd__int indexOfSignature, xsd__boolean &result);
int ns__verifyDocumentSignatureWithNames(xsd__string in, xsd__string signerNames, xsd__boolean &result);