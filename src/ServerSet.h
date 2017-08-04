#pragma once
/* \class CServerSet
*  \brief 单例对象，负责管理Server配置和维护运行时状态
*  \date  2006-07-08
*/
/*
*2006-08-01
*为适合自动升级需求，改回原有插件管理的结构
*/

#include "config.h"
#include "Option.h"
#include "suServerStatus.h"

////////////////////////////////////////////////////////////////
//Define all interface here
///////////////////////////////////////////////////////////////
#define IDEGINEDIR "IDEgine/"
#define APPDIR     "APP/"
#define LOGDIR     "Log/"
#define CURVERSION 0x00010000


typedef suLock CServerLock;

////////////////////////////////////////////////////////////////
//Server Status Structure
///////////////////////////////////////////////////////////////
/*! \class CServerSet
*   \brief 保存Server运行信息
*
*/

class CServerSet
{
public:
	typedef map<string, suService*>   ModulesMap; //map between services' name and modules' interface

	//std::string GetCurrentDir();
	static CServerSet&  gOnly();
	void gClear();

	int    init();
	int    LoadSetting();


	//!服务器控制
	void ShutdownsServer();
	void PauseResumeServer();

	//!私有变量设定
	void    SetLog(BOOL isLog_);                  //!<是否写日志
	void    SetThreadNum(int n) { nThreadNum = n; }
	void    SetInBufSize(DWORD dSize) { nInBufSize_ = dSize; }
	void    SetOutBufSize(DWORD dSize) { nOutBufSize_ = dSize; }

	BOOL    IsLog() { return isLog_; }
	BOOL    IsLoad() { return bPlugInLoad; }
	void    SetLogFileName(const std::string &name);//!<设置log文件名

	std::string  GetComputerIP(std::vector<std::string> &ipArr);    //!<返回第一个IP地址，仅能用在网络监听没有初始化的时候，否则调用WSACleanup()将导致网络中断
	std::string  GetDir() { return m_strCurDir; }
	std::string  GetIP() { return strBindIp_; }
	std::string  GetWebRootDir() { return m_webRootDir; }
	std::string  GetOption(std::string key);
	const int    GetThreadNum() { return nThreadNum; }
	const DWORD  GetInBufSize() { return nInBufSize_; }
	const DWORD  GetOutBufSize() { return nOutBufSize_; }
	COption&     GetOption() { return m_Options; }


	//New Load dll functions
	suService*   LoadModule(string sModuleFilePath);
	suService*   LoadModuleEx(string sModuleFilePath);//load module with input pointer
	void         ReadModules(string sModulesPath);

	//This is a dangous function
	void         GetServices(std::vector<std::string> &nameLists);

	suService*   GetModule(string sServiceName);

public:

	suAgentMemory *pStatus;

private:
	static CServerSet* gOnly_;
	CServerSet();
	~CServerSet();

	std::string strBindIp_;
	std::string m_strCurDir;
	std::string m_webRootDir;
	std::string m_appRootDir;
	DWORD       m_version;


	BOOL           isLog_;
	int            nThreadNum;
	DWORD          nInBufSize_;     //!<接收缓冲区大小
	DWORD          nOutBufSize_;    //!<发送缓冲区大小

	BOOL           bPlugInLoad;     //!<是否装入plugin

	COption        m_Options;
	ModulesMap     modulesMapList_;     //!<存储装入的动态链接库函数接口与生成Services名的映射，Services名=动态模块文件名
};


