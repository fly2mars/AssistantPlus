#include <Engines.h>
#include "option.h"
#include <list>
#include <iostream>
#include <assert.h>

#define  OPTION_FILE "Set\\Config.xml"
//#pragma comment(lib, "ws2_32.lib")

COption::COption(void)
{	
	m_bInitialized = FALSE;
}

COption::~COption(void)
{
}

BOOL   COption::GetAsCommand(char **pBuffer, DWORD *nBufferLength)
{
	return TRUE;
}

BOOL COption::ParseOptionsCommand(unsigned char *pData, DWORD dwDataLength, BOOL bFromLocal)
{
	return TRUE;
}


std::string  COption::GetOption(std::wstring name)
{
	std::wstring para = suResourceMgr::gOnly().fetchStringByNote(name);
	if (para.empty())
	{
		throw suException("GetOption, option name doesn't exist!");
	}
	return SU::ws2s(para);
	
}
int COption::GetOptionVal(std::wstring name)
{
	std::wstring para = suResourceMgr::gOnly().fetchStringByNote(name);
	if (para.empty())
	{
		throw suException("GetOptionVal, option name doesn't exist!");
	}

	return SU::WstringToInt(para);
}


void COption::Init()
{	
}

bool COption::IsNumeric(LPCTSTR str)
{
	if (!str)
		return false;
	LPCTSTR p=str;
	while(*p)
	{
		if (*p<'0' || *p>'9')
		{
			return false;
		}
		p++;
	}
	return true;
}

void COption::LoadSetting()
{
	std::string configPath = SU::GetModuleDir() + OPTION_FILE;
	if (!Utility::FileExists(suString(configPath.c_str())) )
	{
		throw std::bad_exception(/*"Config file is not found!"*/);
	}

	suResourceMgr::gOnly().importStrings(configPath);
}


void COption::dump()
{
#ifdef _DEBUG
	suResourceMgr::gOnly().exportStrings("r:\\test.xml");
#endif
}