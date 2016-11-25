#include "ServerSet.h"
#include <tchar.h>
#include <io.h>
#include <direct.h>
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
	m_strCurDir = SU::GetModuleDir();
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
		keyName = m_Options.GetOption(SU::s2ws(key));
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

	//set web root
	m_webRootDir = m_Options.GetOption(L"Web Root");
	/*if (m_webRootDir.find("\\",0) != 0)
	{
		m_webRootDir = "\\" + m_webRootDir;
	}*/
	if (m_webRootDir.find("\\",1) != m_webRootDir.size()-1)
	{
		m_webRootDir = m_webRootDir + std::string("\\");
	}
	m_webRootDir = SU::GetModuleDir()+ m_webRootDir;

	try{
		strBindIp_ = m_Options.GetOption(L"ServerIP");
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
		//TODO: modify config.xml here
		//Bind 
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

	if (!serviceModule) throw std::bad_exception(/*dlerror()*/);
	//createModule is a pointer to the create function in the module
	suService* (*createModule)() = (suService* (*)())dlsym(serviceModule, "create");
	if (!createModule)  throw std::bad_exception(/*dlerror()*/);
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
	long handle;
	struct _finddata_t fs;    //!< file struct
	string strPath;

	strPath = m_strCurDir + string(APPDIR);

	_chdir(strPath.c_str());
	handle = _findfirst("*.dll",&fs);

	//if (handle == -1)
	//{
	//	gLogStream << "ReadModules:File find error";
	//	return;
	//}

	if (GetFileAttributes(fs.name) & FILE_ATTRIBUTE_DIRECTORY)
	{
		//is directory
		if (*fs.name != '.')
		{
			Console::WriteLine("No dll in the path");
			return;;
		}
	}else
	{
		modulesMapList_.clear();
		try
		{
			string keyStr = fs.name;
			keyStr = keyStr.substr(0,keyStr.size()-4);
			for(int i=0;i<(int)keyStr.size();i++)
			{
				keyStr[i] = (char)(toupper(keyStr[i]));
			}
			modulesMapList_[keyStr] = LoadModuleEx(fs.name);
			modulesMapList_[keyStr]->initialize();
		}
		catch (std::bad_exception e)
		{
			cout << e.what() << endl;			
		}
		
	}

	while (!_findnext(handle, &fs))
	{
		if (GetFileAttributes(fs.name) & FILE_ATTRIBUTE_DIRECTORY)
		{
			//is directory
			if (*fs.name != '.')
			{
				return ;
			}
		}else
		{
			//Load Dll Info
			try
			{
				string keyStr = fs.name;
				keyStr = keyStr.substr(0,keyStr.size()-4);
				for(int i=0;i<(int)keyStr.size();i++)
				{
					keyStr[i] = (char)(toupper(keyStr[i]));
				}
				modulesMapList_[keyStr] = LoadModuleEx(fs.name);
				modulesMapList_[keyStr]->initialize();			
			}
			catch (std::bad_exception e)
			{
				cout << e.what() << endl;			
			}
		}
	}

	_findclose(handle);

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
