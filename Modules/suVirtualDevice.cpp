#include "config.h"
#include "suVirtualDevice.h"

suVirtualDevice::suVirtualDevice()
{

}

void suVirtualDevice::updateWSDL()
{

	//User DLL name to replace the name of service
	std::string serviceName = "suAgent";
	//wsdl 1.0
	wsdl_ = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"		
		"<wsdl:definitions xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\" xmlns:tm=\"http://microsoft.com/wsdl/mime/textMatching/\" xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:mime=\"http://schemas.xmlsoap.org/wsdl/mime/\" xmlns:tns=\"http://tempuri.org/\" xmlns:s=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap12=\"http://schemas.xmlsoap.org/wsdl/soap12/\" xmlns:http=\"http://schemas.xmlsoap.org/wsdl/http/\" targetNamespace=\"http://tempuri.org/\" xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl/\">"
		"<wsdl:types>"
		"<s:schema elementFormDefault=\"qualified\" targetNamespace=\"http://tempuri.org/\">"
		"<s:element name=\"run\">"
		"<s:complexType>"
		"<s:sequence>"
		"<s:element minOccurs=\"0\" maxOccurs=\"1\" name=\"cmd\" type=\"s:string\" />"
		"</s:sequence>"
		"</s:complexType>"
		"</s:element>"
		"<s:element name=\"runResponse\">"
		"<s:complexType>"
		"<s:sequence>"
		"<s:element minOccurs=\"0\" maxOccurs=\"1\" name=\"runResult\" type=\"s:string\" />"
		"</s:sequence>"
		"</s:complexType>"
		"</s:element>"
		"</s:schema>"
		"</wsdl:types>"
		"<wsdl:message name=\"runSoapIn\">"
		"<wsdl:part name=\"parameters\" element=\"tns:run\"/>"
		"</wsdl:message>"
		"<wsdl:message name=\"runSoapOut\">"
		"<wsdl:part name=\"parameters\" element=\"tns:runResponse\"/>"
		"</wsdl:message>"
		"<wsdl:portType name=\"nullSoap\">"
		"<wsdl:operation name=\"run\">"
		"<wsdl:input message=\"tns:runSoapIn\"/>"
		"<wsdl:output message=\"tns:runSoapOut\"/>"
		"</wsdl:operation>"
		"</wsdl:portType>"
		"<wsdl:binding name=\"nullSoap\" type=\"tns:nullSoap\">"
		"<soap:binding transport=\"http://schemas.xmlsoap.org/soap/http\"/>"
		"<wsdl:operation name=\"run\">"
		"<soap:operation soapAction=\"http://tempuri.org/run\" style=\"document\"/>"
		"<wsdl:input>"
		"<soap:body use=\"literal\"/>"
		"</wsdl:input>"
		"<wsdl:output>"
		"<soap:body use=\"literal\"/>"
		"</wsdl:output>"
		"</wsdl:operation>"
		"</wsdl:binding>"
		"<wsdl:binding name=\"nullSoap12\" type=\"tns:nullSoap\">"
		"<soap12:binding transport=\"http://schemas.xmlsoap.org/soap/http\"/>"
		"<wsdl:operation name=\"run\">"
		"<soap12:operation soapAction=\"http://tempuri.org/run\" style=\"document\"/>"
		"<wsdl:input>"
		"<soap12:body use=\"literal\"/>"
		"</wsdl:input>"
		"<wsdl:output>"
		"<soap12:body use=\"literal\"/>"
		"</wsdl:output>"
		"</wsdl:operation>"
		"</wsdl:binding>"
		"<wsdl:service name=\"null\">"
		"<wsdl:port name=\"nullSoap\" binding=\"tns:nullSoap\">"
		//"<soap:address location=\"http://$IP$:$PORT$/$SERVICE_NAME$\" />"
		"<soap:address location=\"http://$IP$:$PORT$/ws_$SERVICE_NAME$\" />"
		"</wsdl:port>"
		"<wsdl:port name=\"nullSoap12\" binding=\"tns:nullSoap12\">"
		"<soap12:address location=\"http://$IP$:$PORT$/ws_$SERVICE_NAME$\"/>"
		"</wsdl:port>"
		"</wsdl:service>"
		"</wsdl:definitions>";

	//?wsdl 2.0

	name_ = SU::GetModuleFileName();

	std::string strName = name_;

	//Soap1
	int nStart = wsdl_.find("$SERVICE_NAME$");
	if (nStart != string::npos)
	{
		wsdl_.replace(nStart, 14, strName.c_str());
	}

	//Soap12
	nStart = wsdl_.find("$SERVICE_NAME$");
	if (nStart != string::npos)
	{
		wsdl_.replace(nStart, 14, strName.c_str());
	}

}
