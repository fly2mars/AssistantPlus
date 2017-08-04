#include "../src/config.h"
#include <boost/filesystem/operations.hpp>

#include <boost/filesystem/path.hpp>

#include <iostream>
#include "../src/ServerSet.h"

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif
UTFUNC(suStringTest)
{
	std::cout << "- " << this->name().c_str() << " has started." << std::endl;
	suString filename;

	filename = "test.dll";
	std::cout << filename.FindExtension() << std::endl;
	suString ext = filename.FindExtension();
	suString service_name = filename.SubString(0, filename.Length() - ext.Length()).MakeUppercase();
	std::cout << service_name.CString() << std::endl;

	suVector<suString> fileList;
	Utility::GetFilesFromDir("E:/FangCloudSync/Books/AI", fileList, "*.pdf");
	
	for(int i=0; i<fileList.Length(); i++)
	  std::cout << fileList[i].CString() << std::endl;
}

char *pSelfName;
UTFUNC(getCurdir)
{
	std::cout << "- " << this->name().c_str() << " has started." << std::endl;
	//namespace fs = boost::filesystem;

	//fs::path full_path(fs::initial_path<fs::path>());

	//full_path = fs::system_complete(fs::path(pSelfName));

	//std::cout << full_path << std::endl;
	//std::cout << "root path: " << full_path.root_path() << std::endl;

	////Without file name
	//std::cout << "whithout filename: " << full_path.parent_path() << std::endl;
	////std::cout << fs::basename(full_path) << std::endl;
	std::cout << SU::GetModuleDir() << std::endl;

}

UTFUNC(testTinyXML2)
{
	std::cout << "- " << this->name().c_str() << " has started." << std::endl;
	
	COption m_Options;
	m_Options.LoadSetting();
	m_Options.dump();
	/*std::string configPath = "E:/git/AssistantPlus/bin/Set/Config.xml";
	configPath = "./utf8test.xml";
	tinyxml2::XMLDocument conf;
	conf.LoadFile(configPath.c_str());
	
	std::cout << " error id : " << conf.ErrorID() << std::endl;
	suString msg;

	std::map<std::string, std::string>  listKeys;

	tinyxml2::XMLElement * pRoot = conf.FirstChildElement("setting");
	if (pRoot == nullptr) throw suException("conf parse error");

   
	tinyxml2::XMLElement * pElement = pRoot->FirstChildElement("add");
	if (pElement == nullptr)  throw suException("No add key is found.");

	const char * pKey = nullptr;
	const char * pValue = nullptr;
	while (pElement != nullptr){
		pKey = pElement->Attribute("key");
		if (pKey != nullptr) {
			pValue = pElement->Attribute("value");
			if (pValue != nullptr) {
				listKeys[pKey] = std::string(pValue);
			}
		}
		pElement = pElement->NextSiblingElement("add");
	}
	
   
	for (std::map<std::string, std::string>::iterator it = listKeys.begin(); it != listKeys.end(); it++)
	{
		Console::WriteLine(it->first.c_str());
		Console::WriteLine(it->second.c_str());

	}
*/

	

	

}
#ifdef __linux
std::string GetPrimaryIp()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sock != -1);

	const char* kGoogleDnsIp = "8.8.8.8";
	uint16_t kDnsPort = 53;
	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
	serv.sin_port = htons(kDnsPort);

	int err = connect(sock, (const sockaddr*)&serv, sizeof(serv));
	assert(err != -1);

	sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (sockaddr*)&name, &namelen);
	assert(err != -1);

	int buflen = 16;
	char buffer[16];
	const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, buflen);
	assert(p);
	close(sock);
	return std::string(buffer);
}
UTFUNC(getIP)
{
	std::cout << "- " << this->name().c_str() << " has started." << std::endl;
	char ip[16];

	std::cout << "GetPrimaryIp: " << GetPrimaryIp() << std::endl;

	setenv("LANG", "C", 1);
	FILE * fp = popen("ifconfig", "r");
	if (fp) {
		char *p = NULL, *e; size_t n;
		while ((getline(&p, &n, fp) > 0) && p) {
			if (p = strstr(p, "inet ")) {
				p += 5;
				if (p = strchr(p, ':')) {
					++p;
					if (e = strchr(p, ' ')) {
						*e = '\0';
						printf("%s\n", p);
					}
				}
			}
		}
	}
	pclose(fp);
}
#endif
int main(int argc, char** argv)
{
	pSelfName = argv[0];
	bool state = suUnitTest::gOnly().run ();
	suUnitTest::gOnly().dumpResults (std::cout);

	return state;
}
