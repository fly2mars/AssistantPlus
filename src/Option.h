#pragma once
/*! \class COption
*   \brief 保存服务器配置信息，从配置文件中存取设置
*   \Change load/save info with tinyXML2  2017/08/04
*   \Change 用vector<t_OptionsCacheNew>保存新的配置信息d
*   \Change 使用suResourceMgr管理配置信息 2013-5-4
*/
//#include "OptionTypes.h"
#include <string>
#include <map>
class COption
{
public:
	COption(void);
	~COption(void);

	//search option by name
	std::string  GetOption(std::string name);
	int          GetOptionVal(std::string name);
	void         LoadSetting();

	void      dump();
	void      Init();

	bool      m_bInitialized;
private:
	std::map<std::string, std::string> options_;
};

