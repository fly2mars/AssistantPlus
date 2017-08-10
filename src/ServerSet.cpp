#include "config.h"
#include "ServerSet.h"

#include <iostream>


using namespace std;
////////////////////////////////////////////////////////////////
//AppServer Function Defination Here
///////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
//! CServerSet Class Body
///////////////////////////////////////////////////////////////
CServerSet* CServerSet::gOnly_ = NULL;

CServerSet& CServerSet::gOnly ()
{
	if (gOnly_ == NULL)
	{
		gOnly_ = new CServerSet();
	
	}
	return *gOnly_;
}
void CServerSet::gClear()
{
	modulesMapList_.clear();	
}
CServerSet::CServerSet()
{
	bPlugInLoad = FALSE;

	//pStatus = new suEStatus();	
	pStatus = new suAgentMemory();
}
CServerSet::~CServerSet()
{
	gClear();
}
/*Get option by name
 */
std::string  CServerSet::GetOption(std::string key)
{
	std::string keyName;
	try
	{
		keyName = m_Options.GetOption(key);
	}
	catch (...){
		;
	}
	return keyName;
}

void CServerSet::ShutdownsServer()
{
	
}

void CServerSet::PauseResumeServer()
{
	
}

int CServerSet::LoadSetting()
{
	m_Options.LoadSetting();	

	//set path
	m_strCurDir = SU::GetModuleDir();
	m_webRootDir = m_Options.GetOption("Web Root");
	m_appRootDir = m_Options.GetOption("App Root");

	try{
		strBindIp_ = m_Options.GetOption("ServerIP");
	}
	catch (...)
	{
		std::vector<std::string> _ipArr;
		GetComputerIP(_ipArr);
		if (!_ipArr.empty())
		{
			strBindIp_ = _ipArr[0];			
		}
		else{
			strBindIp_ = "127.0.0.1";
		}		
	}
	//std::cout << "Bind IP: " << strBindIp_ << std::endl;

	return 1;
}

void   CServerSet::SetLogFileName(const std::string &name)
{

}

//New Added Dll Functions
suService*   CServerSet::LoadModule(string sModuleFilePath)
{
	void* serviceModule = dlopen(sModuleFilePath.c_str(), RTLD_NOW | RTLD_GLOBAL);

	if (!serviceModule) throw suException(std::string(dlerror() ) );
	//createModule is a pointer to the create function in the module
	suService* (*createModule)() = (suService* (*)())dlsym(serviceModule, "create");
	if (!createModule)   throw suException(std::string(dlerror()));
	return createModule();
}
//New Added Dll Functions
suService*   CServerSet::LoadModuleEx(string sModuleFilePath)
{
	void* serviceModule = dlopen(sModuleFilePath.c_str(), RTLD_NOW | RTLD_GLOBAL);

	if (!serviceModule) throw std::bad_exception(/*dlerror()*/);
	//createModule is a pointer to the create function in the module
	suService* (*createModule)(suStatus*) = (suService* (*)(suStatus*))dlsym(serviceModule, "createEx");
	if (!createModule)  throw std::bad_exception(/*dlerror()*/);
	return createModule(pStatus);
}
void  CServerSet::ReadModules(string sModulesPath)
{
	//Using Utility::GetFilesFromDir to make compatible with linux.
	std::string  strPath = m_strCurDir + m_appRootDir + "/";      
	suVector<suString> dll_path_list;
	//todo: write linux version
#ifdef WIN32
	Utility::GetFilesFromDir(strPath.c_str(), dll_path_list, "*.dll");
#elif __linux

	Utility::GetFilesFromDir(strPath.c_str(), dll_path_list, "*.so");
#endif
	if (!dll_path_list.Length()){
		Console::WriteLine("No dll in the path");
		return;
	}	
	else {
		modulesMapList_.clear();
		Console::WriteLine("--------------Modules-------------");
		try
		{
			for (int i = 0; i < dll_path_list.Length(); i++) {
				suString filename = dll_path_list[i];          
				suString ext = filename.FindExtension();         
#ifdef WIN32
				suString service_name = filename.SubString(0, filename.Length() - ext.Length()).MakeUppercase();
#elif __linux		
				//remove "lib" in "libNull.so"  -> "Null"
				suString service_name = filename.SubString(3, filename.Length() - ext.Length() - 3).MakeUppercase();
#endif
				_tString path = strPath +  _tString(filename.CString());
				
				modulesMapList_[service_name.CString()] = LoadModuleEx(path);
				std::cout << "initialize "  << path << std::endl;
				modulesMapList_[service_name.CString()]->initialize();
			}
			
		}
		catch (std::bad_exception e)
		{
			cout << e.what() << endl;
		}
	}
	
	
	

}
suService*   CServerSet::GetModule  (string sServiceName)
{
	for(int i=0;i<(int)sServiceName.size();i++)
	{
		sServiceName[i] = (char)(toupper(sServiceName[i]));
	}
	if (modulesMapList_.count(sServiceName) == 1)
	{
		return modulesMapList_[sServiceName];
	}else
	{
		return NULL;
	}
}

std::string CServerSet::GetComputerIP(std::vector<std::string> &ipArr)
{
#ifdef WIN32
	char ac[80];
	ipArr.clear();

	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		return "";
	}

	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << endl;
		return "";
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		cerr << "Yow! Bad host lookup." << endl;
		return "";
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		ipArr.push_back(inet_ntoa(addr));
		cout << "Address " << i << ": " << inet_ntoa(addr) << endl;
	}
	WSACleanup();

	return ipArr[0];
#elif __linux
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
#endif
}

/* 系统变量
 * SYS_IP
 * CAGENT_IP
 * ACTION_CONTROLER    
 */

int CServerSet::init()
{
	//connect agent memory
	if (!pStatus->init())  return 0;

	//Update system memory
	//Update system IP
	std::vector<std::string>  ipList;
	GetComputerIP(ipList);
	std::string strIps;
	for (int i = 0; i < ipList.size(); i++)
	{
		strIps += ipList[i] + " ";
	}
	pStatus->setGlobalVar("SYS_IP", strIps);

	//Update CAgent IP
	pStatus->setGlobalVar("CAGENT_IP", GetOption("CAgent Address") );

	return 1;
}

void CServerSet::GetServices(std::vector<std::string> &nameLists)
{
	nameLists.clear();
	for (auto it = modulesMapList_.cbegin(); it != modulesMapList_.cend(); ++it)
		nameLists.push_back((*it).first);	
}
