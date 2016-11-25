/** \file OptionTypes.h
* 定义所有影响服务器运行的
*变量类型.
*设置项目
*软件版本
*协议版本
*/


#if !defined(OPTION_TYPES_INCLUDED)
#define OPTION_TYPES_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Options.h : Header
//
#include <wtypes.h>
#include <tchar.h>

#define OPTION_SERVERPORT 1
#define OPTION_THREADNUM  2
#define OPTION_TIMEOUT    3
#define OPTION_NOTRANSFERTIMEOUT 4
#define OPTION_LOGINTIMEOUT 5
#define OPTION_ADMINNAME 6
#define OPTION_ADMINPASS 7
#define OPTION_ADMINIPBINDINGS  8
#define OPTION_ADMINIPADDRESSES 9
#define OPTION_ADMINPORT       10
#define OPTION_ENABLELOGGING 11
#define OPTION_LOGLIMITSIZE 12
#define OPTION_LOGTYPE      13
#define OPTION_LOGDELETETIME 14
#define OPTION_RECEIVEBUFFERSIZE 15
#define OPTION_SENDBUFFERSIZE    16
#define OPTION_CONNECTIONSTRING  17

#define OPTIONS_NUM 17

struct t_Option
{
	TCHAR name[30];
	int   nType;
	BOOL  bOnlyLocal; //If TRUE, setting can only be changed from local connections
};

const DWORD SERVER_VERSION   = 0x00080700;
const DWORD PROTOCOL_VERSION = 0x00010400;


static const t_Option m_Options[OPTIONS_NUM]={	
	    _T("Serverport"),				1,	FALSE,
		_T("Number of Threads"),		1,	FALSE,												
		_T("Timeout"),					1,	FALSE,
		_T("No Transfer Timeout"),		1,	FALSE,
		_T("Login Timeout"),			1,	FALSE,
		_T("Admin Name"),				1,	TRUE,
		_T("Admin Password"),			0,	TRUE,
		_T("Admin IP Bindings"),		0,	TRUE,
		_T("Admin IP Addresses"),		0,	TRUE,
		_T("Admin Port"),               1,  FALSE,
		_T("Enable logging"),			1,	FALSE,
		_T("Logsize limit"),			1,	FALSE,
		_T("Logfile type"),				1,	FALSE,
		_T("Logfile delete time"),		1,	FALSE, 												
		_T("Receive Buffer Size"),		1,	FALSE,	
		_T("Send Buffer Size"),         1,  FALSE,
		_T("Database Connection String"),0, TRUE,
};

#endif // OPTION_TYPES_INCLUDED