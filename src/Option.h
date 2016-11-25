#pragma once
/*! \class COption
*   \brief 保存服务器配置信息，从配置文件中存取设置
*   \Change 用vector<t_OptionsCacheNew>保存新的配置信息d
*   \Change 使用suResourceMgr管理配置信息 2013-5-4
*/
//#include "OptionTypes.h"

class COption
{
public:
	COption(void);
	~COption(void);

	BOOL       GetAsCommand(char **pBuffer, DWORD *nBufferLength);
	BOOL        ParseOptionsCommand(unsigned char *pData, DWORD dwDataLength, BOOL bFromLocal = FALSE);

	//search option by name
	std::string  GetOption(std::wstring name);
	int          GetOptionVal(std::wstring name);
	void         LoadSetting();	

	void      dump();

//protected:
	static bool IsNumeric(LPCTSTR str);

	void  Init();

	BOOL           m_bInitialized;

};

