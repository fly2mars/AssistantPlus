#include "config.h"
#include "option.h"
#include <list>

#define  OPTION_FILE "Set/Config.xml"
//#pragma comment(lib, "ws2_32.lib")

COption::COption(void)
{
	m_bInitialized = FALSE;
}

COption::~COption(void)
{
}




std::string  COption::GetOption(std::string name)
{
	return options_[name];

}
int COption::GetOptionVal(std::string name)
{
	return SU::StringToInt(options_[name]);
}


void COption::Init()
{
}


void COption::LoadSetting()
{
	options_.clear();
	std::string configPath = SU::GetModuleDir() + OPTION_FILE;
	//std::string configPath = std::string("E:/git/AssistantPlus/bin/") + OPTION_FILE;
	if (!Utility::FileExists(suString(configPath.c_str())))
	{
		throw std::bad_exception(/*"Config file is not found!"*/);
	}
	tinyxml2::XMLDocument conf;
	conf.LoadFile(configPath.c_str());

	if (conf.ErrorID()) throw suException("conf parse error!");

	tinyxml2::XMLElement * pRoot = conf.FirstChildElement("setting");
	if (pRoot == nullptr) throw suException("setting parse error!");


	tinyxml2::XMLElement * pElement = pRoot->FirstChildElement("add");
	if (pElement == nullptr)  throw suException("No add key is found.");

	const char * pKey = nullptr;
	const char * pValue = nullptr;
	while (pElement != nullptr) {
		pKey = pElement->Attribute("key");
		if (pKey != nullptr) {
			pValue = pElement->Attribute("value");
			if (pValue != nullptr) {
				options_[pKey] = std::string(pValue);
			}
		}
		pElement = pElement->NextSiblingElement("add");
	}

	dump();
	//suResourceMgr::gOnly().importStrings(configPath);
}


void COption::dump()
{
	std::cout << "--------------options---------------" << std::endl;
	for (std::map<std::string, std::string>::iterator it = options_.begin(); it != options_.end(); it++)
	{
		std::cout << it->first.c_str() << " = " << options_[it->first.c_str()].c_str() << std::endl;
	}
	std::cout << std::endl;
}