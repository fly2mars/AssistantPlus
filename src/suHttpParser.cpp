#include "suhttpparser.h"
#include "ServerSet.h"
#include <iostream> 

#if __linux
#include <iconv.h>        //GBK UTF-8 conversion

class CodeConverter 
{
public:
	CodeConverter(const char *from_charset, const char *to_charset) {
		cd = iconv_open(to_charset, from_charset);
	}

	~CodeConverter() {
		iconv_close(cd);
	}

	// ×ª»»Êä³ö 
	int convert(char *inbuf, int inlen, char *outbuf, int outlen) {
		char **pin = &inbuf;
		char **pout = &outbuf;

		memset(outbuf, 0, outlen);
		return iconv(cd, pin, (size_t *)&inlen, pout, (size_t *)&outlen);
	}
private:
	iconv_t cd;
};
#endif 



suHttpParser::suHttpParser(void)
{
	m_MimeTypes["doc"]	= "application/msword";
	m_MimeTypes["bin"]	= "application/octet-stream";
	m_MimeTypes["dll"]	= "application/octet-stream";
	m_MimeTypes["exe"]	= "application/octet-stream";
	m_MimeTypes["pdf"]	= "application/pdf";
	m_MimeTypes["p7c"]	= "application/pkcs7-mime";
	m_MimeTypes["ai"]	= "application/postscript";
	m_MimeTypes["eps"]	= "application/postscript";
	m_MimeTypes["ps"]	= "application/postscript";
	m_MimeTypes["rtf"]	= "application/rtf";
	m_MimeTypes["fdf"]	= "application/vnd.fdf";
	m_MimeTypes["arj"]	= "application/x-arj";
	m_MimeTypes["gz"]	= "application/x-gzip";
	m_MimeTypes["rar"]  = "application/x-gzip";
	m_MimeTypes["class"]= "application/x-java-class";
	m_MimeTypes["js"]	= "application/x-javascript";
	m_MimeTypes["lzh"]	= "application/x-lzh";
	m_MimeTypes["lnk"]	= "application/x-ms-shortcut";
	m_MimeTypes["tar"]	= "application/x-tar";
	m_MimeTypes["hlp"]	= "application/x-winhelp";
	m_MimeTypes["cert"]	= "application/x-x509-ca-cert";
	m_MimeTypes["zip"]	= "application/zip";
	m_MimeTypes["cab"]	= "application/x-compressed";
	m_MimeTypes["arj"]	= "application/x-compressed";
	m_MimeTypes["aif"]	= "audio/aiff";
	m_MimeTypes["aifc"]	= "audio/aiff";
	m_MimeTypes["aiff"]	= "audio/aiff";
	m_MimeTypes["au"]	= "audio/basic";
	m_MimeTypes["snd"]	= "audio/basic";
	m_MimeTypes["mid"]	= "audio/midi";
	m_MimeTypes["rmi"]	= "audio/midi";
	m_MimeTypes["mp3"]	= "audio/mpeg";
	m_MimeTypes["vox"]	= "audio/voxware";
	m_MimeTypes["wav"]	= "audio/wav";
	m_MimeTypes["ra"]	= "audio/x-pn-realaudio";
	m_MimeTypes["ram"]	= "audio/x-pn-realaudio";
	m_MimeTypes["bmp"]	= "image/bmp";
	m_MimeTypes["gif"]	= "image/gif";
	m_MimeTypes["jpeg"]	= "image/jpeg";
	m_MimeTypes["jpg"]	= "image/jpeg";
	m_MimeTypes["png"]  = "image/png";
	m_MimeTypes["tif"]	= "image/tiff";
	m_MimeTypes["tiff"]	= "image/tiff";
	m_MimeTypes["xbm"]	= "image/xbm";
	m_MimeTypes["wrl"]	= "model/vrml";
	m_MimeTypes["htm"]	= "text/html";
	m_MimeTypes["html"]	= "text/html";
	m_MimeTypes["c"]	= "text/plain";
	m_MimeTypes["cpp"]	= "text/plain";
	m_MimeTypes["def"]	= "text/plain";
	m_MimeTypes["h"]	= "text/plain";
	m_MimeTypes["txt"]	= "text/plain";
	m_MimeTypes["xml"]  = "text/xml";
	m_MimeTypes["rtx"]	= "text/richtext";
	m_MimeTypes["rtf"]	= "text/richtext";
	m_MimeTypes["java"]	= "text/x-java-source";
	m_MimeTypes["css"]	= "text/css";
	m_MimeTypes["mpeg"]	= "video/mpeg";
	m_MimeTypes["mpg"]	= "video/mpeg";
	m_MimeTypes["mpe"]	= "video/mpeg";
	m_MimeTypes["avi"]	= "video/msvideo";
	m_MimeTypes["mov"]	= "video/quicktime";
	m_MimeTypes["qt"]		= "video/quicktime";
	m_MimeTypes["shtml"]	= "wwwserver/html-ssi";
	m_MimeTypes["asa"]	= "wwwserver/isapi";
	m_MimeTypes["asp"]	= "wwwserver/isapi";
	m_MimeTypes["cfm"]	= "wwwserver/isapi";
	m_MimeTypes["dbm"]	= "wwwserver/isapi";
	m_MimeTypes["isa"]	= "wwwserver/isapi";
	m_MimeTypes["plx"]	= "wwwserver/isapi";
	m_MimeTypes["url"]	= "wwwserver/isapi";
	m_MimeTypes["cgi"]	= "wwwserver/isapi";
	m_MimeTypes["php"]	= "wwwserver/isapi";
	m_MimeTypes["wcgi"]	= "wwwserver/isapi";

	m_HomeDir         = CServerSet::gOnly().GetWebRootDir();
	m_DefalutHomepage = string("index.html");
	
	m_requestFilename = "";
	m_getParameter    = "";
	m_soapAction      = "";
	m_postParameter   = "";

	contentType_      = "text/html";

	m_httpMethod      = UNKNOWN;
	
	serverName_ = "Soap App Server";
	bKeepAlive_ = TRUE;

}

suHttpParser::~suHttpParser(void)
{
}


bool suHttpParser::isValidGet(string szRequest)
{
	//
	// Determine Connection type
	//
	size_t n = szRequest.find("\nConnection: Keep-Alive", 0);
	if (n != string::npos)
		bKeepAlive_ = TRUE;

	n = szRequest.find("\nConnection: close", 0);
	if (n != string::npos)
		bKeepAlive_ = FALSE;

	// check HTTP GET head; Set m_requestFilename=request string follows GET
	n = szRequest.find("\r", 0);
	if (n == string::npos)
	{
		return FALSE;
	}
	std::string strFirstLine = szRequest.substr(0,n); 
	size_t n1 = strFirstLine.find(" ", 0);                    //find first space
	if (n1 == string::npos)
	{
		return FALSE;
	}
	size_t n2 = strFirstLine.find(" ", n1+1);                  //find second space
	if (n2 == string::npos)
	{
		return FALSE;
	}
	m_requestFilename = szRequest.substr(n1 + 1, n2 - n1 - 1);
	if(m_requestFilename == "/")
	{
		m_requestFilename = m_DefalutHomepage;
	}
	if (m_requestFilename.find("/",0) == 0)
	{
		m_requestFilename = m_requestFilename.substr(1,m_requestFilename.size()-1);
	}

	m_httpMethod = GET;
	
	return TRUE;

}
bool suHttpParser::getParameter()
{
	// if return TRUE: m_requestFilename = ServiceName; m_requestParameter=parameter follows m_requestFilename
	if (m_requestFilename == "")
	{
		return FALSE;
	}
	size_t n = m_requestFilename.find("?",0);
	if (n == string::npos)
	{
		return FALSE;
	}
	m_getParameter = m_requestFilename.substr(n+1,m_requestFilename.size()-n-1);
	m_requestFilename = m_requestFilename.substr(0,n);

	for (size_t i = 0; i < m_getParameter.size(); ++i)
	{
		m_getParameter[i] = (char)toupper(m_getParameter[i]);
	}	
	return TRUE;
}
bool suHttpParser::getSoapAction(string szRequest)
{
	//std::cout << szRequest.c_str() << std::endl;

	//Only cmd parameter got here cmd

	int n = szRequest.find("<cmd>");
	if (n == string::npos)
	{
		//SOAP PHP client
		int n1 = szRequest.find("<ns1:cmd>", n);
		if (n1 == string::npos)
		{
			return FALSE;
		}
		int n2 = szRequest.find("</ns1:cmd>", n1);
		if (n2 == string::npos)
		{
			return FALSE;
		}
		m_postParameter = szRequest.substr(n1 + 9, n2 - n1 - 9);
	}
	else{
		int n5 = szRequest.find("</cmd>", n);
		if (n5 == string::npos)
		{
			return FALSE;
		}
		m_postParameter = szRequest.substr(n + 5, n5 - n - 5);
	}
	
	//ÅÐ¶ÏÊÇ·ñutf-8±àÂë
	m_postParameter = UTF2GBK(m_postParameter);
	return TRUE;
}
bool suHttpParser::isValidPost  (string szRequest)
{
	//
	// Determine Connection type
	//
	size_t n = szRequest.find("\nConnection: Keep-Alive", 0);
	if (n != string::npos)
		bKeepAlive_ = TRUE;

	n = szRequest.find("\nConnection: close", 0);
	if (n != string::npos)
		bKeepAlive_ = FALSE;

	// check HTTP POST head Set m_requestFilename=request string follows POST
	n = szRequest.find("\r", 0);
	if (n == string::npos)
	{
		return FALSE;
	}
	std::string strFirstLine = szRequest.substr(0,n); 
	size_t n1 = strFirstLine.find(" ", 0);                    //find first space
	if (n1 == string::npos)
	{
		return FALSE;
	}
	size_t n2 = strFirstLine.find(" ", n1+1);                  //find second space
	if (n2 == string::npos)
	{
		return FALSE;
	}
	m_requestFilename = szRequest.substr(n1 + 1, n2 - n1 - 1);
	if(m_requestFilename == "/")
	{
		m_requestFilename = m_DefalutHomepage;
	}
	if (m_requestFilename.find("/",0) == 0)
	{
		m_requestFilename = m_requestFilename.substr(1,m_requestFilename.size()-1);
	}

	m_httpMethod = POST;

	
	return TRUE;
}

bool suHttpParser::genWsdlResponse(string szWsdl)
{
	string szMethod;
	string szFileName;	
	string szStatusCode("200 OK");	
	string szConnectionType("close");	
	string szDateTime;
	char pResponseHeader[2048];
	size_t length = 0;
	char *pBuf = NULL;

	contentType_ = "text/xml; charset=utf-8";

	char szDT[128];
	struct tm *newtime;
	time_t ltime;

	time(&ltime);
	newtime = gmtime(&ltime);
	strftime(szDT, 128,	"%a, %d %b %Y %H:%M:%S GMT", newtime);

	//replace IP:PORT
	std::string strIp = CServerSet::gOnly().GetIP();
	std::string strPort = CServerSet::gOnly().GetOption().GetOption("Serverport");
	//Soap 1
	replaceWithString(szWsdl, "$IP$", strIp.c_str());
	replaceWithString(szWsdl, "$PORT$", strPort.c_str());
	//Soap 12
	replaceWithString(szWsdl, "$IP$", strIp.c_str());
	replaceWithString(szWsdl, "$PORT$", strPort.c_str());

	//replaceWithString(szWsdl, "$IP$", strIp.c_str());
	//replaceWithString(szWsdl, "$PORT$", strPort.c_str());

	//build header & body of wsdl
	length = szWsdl.size();

	sprintf(pResponseHeader, "HTTP/1.1 %s\r\nDate: %s\r\nServer: %s\r\nAccept-Ranges: bytes\r\nContent-Length: %d\r\nConnection: %s\r\nContent-Type: %s\r\n\r\n",
		szStatusCode.c_str(), szDT, serverName_.c_str(), (int)length, bKeepAlive_ ? "Keep-Alive" : "close", contentType_.c_str());

	m_strResponse = string(pResponseHeader);
	m_strResponse += szWsdl;
	
	return TRUE;	
}

bool suHttpParser::genPostResonse(string strReturn)
{
	char pResponseHeader[2048];
	string strSoap = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
		"<soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"
		"<soap:Body>"
		"<runResponse xmlns=\"http://tempuri.org/\">"
		"<runResult>{VALUE}</runResult>"
		"</runResponse>"
		"</soap:Body>"
		"</soap:Envelope>";

	string strUtf8 = GBK2UTF(strReturn);
	strSoap.replace(strSoap.find("{VALUE}"),7,strUtf8);

	sprintf(pResponseHeader,"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/xml; charset=utf-8\r\n"
		"Content-Length: %d\r\n\r\n",
		strSoap.size());
	
	m_strResponse = string(pResponseHeader) + strSoap;

	return TRUE;


}

//×Ö·û´®²Ù×÷
string suHttpParser::replaceWithString(string& oriString, string removePart, string newPart)
{
	int nStart = oriString.find(removePart.c_str());
	if (nStart != string::npos)
	{
		oriString.replace(nStart, removePart.size(), newPart.c_str());
	}
	return oriString;
}

//±àÂë×ª»»

string suHttpParser::GBK2UTF(string gbkStr)
{
	if (gbkStr.empty())
	{
		return "";
	}

#ifdef WIN32
	int len;
	wchar_t *strUnicode;
	char    *strUTF8;

	len = MultiByteToWideChar(CP_GBK, 0,gbkStr.c_str(), -1, NULL,0);
	if (len <1)
	{
		return "";
	}
	//else ok
	strUnicode = (wchar_t *) malloc(sizeof(wchar_t) * len);
	if (!strUnicode)
	{
		return NULL;

	}
	//else ok
	len = MultiByteToWideChar(CP_GBK, 0, gbkStr.c_str(), -1, strUnicode, len);
	if (len<1)
	{
		free(strUnicode);
		return NULL;
	}
	//else ok

	len = WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	if (len<1)
	{
		free(strUnicode);
		return NULL;
	}
	//else ok
	strUTF8 = (char *) malloc(sizeof(char) * len);
	if (!strUTF8)
	{
		free(strUnicode);
		return NULL;
	}

	//else ok


	len = WideCharToMultiByte (CP_UTF8, 0, strUnicode, -1, strUTF8, len, NULL,NULL);
	free(strUnicode);
	if (len<1)
	{
		free(strUTF8);
		return NULL;
	}
	//else ok
	return strUTF8;
	
#elif __linux
	int nLen = gbkStr.size() * 2;
	char out[nLen];
	auto pIn = &*gbkStr.begin();
	CodeConverter cc = CodeConverter("GBK", "utf-8");
	cc.convert(pIn, gbkStr.size(), out, nLen);
	return out;
#endif
	
}
string suHttpParser::UTF2GBK(string utf8Str)
{
	if (utf8Str.size() == 0)
	{
		return "";
	}
#ifdef WIN32
	int len;
	wchar_t *strUnicode;
	char    *strGBK;
	
	len = MultiByteToWideChar(CP_UTF8, 0,utf8Str.c_str(), -1, NULL,0);
	if (len <1)
	{
		return "";
	}
	//else ok
	strUnicode = (wchar_t *) malloc(sizeof(wchar_t) * len);
	if (!strUnicode)
	{
		return NULL;

	}
	//else ok
	len = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, strUnicode, len);
	if (len<1)
	{
		free(strUnicode);
		return NULL;
	}
	//else ok

	len = WideCharToMultiByte(CP_GBK, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	if (len<1)
	{
		free(strUnicode);
		return NULL;
	}
	//else ok
	strGBK = (char *) malloc(sizeof(char) * len);
	if (!strGBK)
	{
		free(strUnicode);
		return NULL;
	}

	//else ok


	len = WideCharToMultiByte (CP_GBK, 0, strUnicode, -1, strGBK, len, NULL,NULL);
	free(strUnicode);
	if (len<1)
	{
		free(strGBK);
		return NULL;
	}

	//else ok
	return strGBK;
#elif __linux
	int nLen = utf8Str.size() * 2;
	char out[nLen];
	auto pIn = &*utf8Str.begin();  //https://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
	CodeConverter cc = CodeConverter("utf-8", "GBK");
	cc.convert(pIn, utf8Str.size(), out, nLen);
	return out;
#endif
}