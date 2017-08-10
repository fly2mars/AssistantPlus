#pragma once
#include <string>
#include <map>

#define CP_GBK 936

/**\class suHttpParser: a simple HTTP parser
 *use example:
 *suHttpParser parser;
 *parser.is
 */
class suHttpParser
{
public:
	typedef std::map<std::string, std::string>		MIMETYPES;

	suHttpParser(void);
	~suHttpParser(void);
	
	MIMETYPES  m_MimeTypes;

	enum       {UNKNOWN,GET,POST};
	std::string     m_HomeDir;
	std::string     m_DefalutHomepage;
	std::string     m_requestFilename;
	std::string	   m_getParameter;
	std::string     m_soapAction;
	std::string     m_soapReturn;
	std::string     m_postParameter;
	std::string     m_strResponse;
	int        m_httpMethod;


public:
	//http method
	bool isValidGet   (std::string szRequest);   // check HTTP GET head; Set m_requestFilename=request std::string follows GET
	bool isValidPost  (std::string szRequest);   // c// check HTTP POST head Set m_requestFilename=request std::string follows POST
	bool getParameter ();                   // check GET parameters, if return TRUE: m_requestFilename = ServiceName; m_requestParameter=parameter follows m_requestFilename
	bool getSoapAction(std::string szRequest);
	
	bool genWsdlResponse     (std::string szWsdl);//gen wdsl xml response
	bool genPostResonse      (std::string strReturn); //gen web service response

private:
	std::string GBK2UTF(std::string gbkStr);
	std::string UTF2GBK(std::string utf8Str);
	std::string replaceWithString(std::string& oriString, std::string removePart, std::string newPart);
public:
	std::string serverName_;
	std::string contentType_;
	bool   bKeepAlive_;

};

//POST /null HTTP/1.1
//Accept: */*
//Accept-Language: zh-cn
//soapaction: http://tempuri.org/Run
//Content-Type: text/xml; charset=utf-8
//Accept-Encoding: gzip, deflate
//User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; QQDownload 6
//						 27; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.
//						 30729)
//Host: localhost:5001
//Content-Length: 300
//Connection: Keep-Alive
//Cache-Control: no-cache
//
//<?xml version="1.0" encoding="utf-8"?><soap:Envelope xmlns:xsi="http://www.w3.or
//g/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:so
//ap="http://schemas.xmlsoap.org/soap/envelope/"><soap:Body><Run xmlns="http://tem
//puri.org/"><cmd>Play</cmd></Run></soap:Body></soap:Envelope>

