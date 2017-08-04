#include <Windows.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <atlbase.h>  

#if _MSC_VER >= 1300    // for VC 7.0
// from ATL 7.0 sources
  #ifndef _delayimp_h
  extern "C" IMAGE_DOS_HEADER __ImageBase;
  #endif
#endif


namespace SU{

	_tString formatString(const char* format, ...)
	{
		char buf[1024];
		va_list arglist;
		va_start(arglist, format);
#ifdef _UNICODE
		_vsnwprintf(buf, 1024, format, arglist);
#else
	   _vsnprintf(buf, 1024, format, arglist);
#endif
		
		va_end(arglist);
		return _tString(buf);
	}


	_tString HexToString(
		const BYTE *pBuffer,
		size_t iBytes)
	{
		_tString result;

		for (size_t i = 0; i < iBytes; i++)
		{
			BYTE c ;

			BYTE b = pBuffer[i] >> 4;

			if (9 >= b)
			{
				c = b + '0';
			}
			else
			{
				c = (b - 10) + 'A';
			}

			result += (TCHAR)c;

			b = pBuffer[i] & 0x0f;

			if (9 >= b)
			{
				c = b + '0';
			}
			else
			{
				c = (b - 10) + 'A';
			}

			result += (TCHAR)c;
		}

		return result;
	}



	_tString GetCurrentDirectory()
	{
		DWORD size = ::GetCurrentDirectory(0, 0);

		auto_ptr<TCHAR> spBuf(new TCHAR[size]);

		if (0 == ::GetCurrentDirectory(size, spBuf.get()))
		{
			suException e("");
			throw suFuncException("GetCurrentDirectory()", "Failed to get current directory");
		}

		return _tString(spBuf.get());
	}
	

	_tString GetDateStamp()
	{
		SYSTEMTIME systime;
		GetSystemTime(&systime);

		static TCHAR buffer[9];

		_stprintf(buffer, _T("%04d-%02d-%02d"),
			systime.wYear,
			systime.wMonth,
			systime.wDay
			);

		return buffer;
	}
	_tString GetCurTime()
	{
		SYSTEMTIME systime;
		GetSystemTime(&systime);

		static TCHAR buffer[7];

		_stprintf(buffer, _T("%02d-%02d-%02d"),
			systime.wHour+8,
			systime.wMinute,
			systime.wSecond
			);

		return buffer;
	}
	_tString GetTimeStamp()
	{
		//YYYYMMDDHHMMSS
		SYSTEMTIME systime;
		GetSystemTime(&systime);
		static TCHAR buffer[15];
		_stprintf(buffer, _T("%04d%02d%02d%02d%02d%02d"),
			systime.wYear,
			systime.wMonth,
			systime.wDay,
			systime.wHour+8,
			systime.wMinute,
			systime.wSecond
			);
		return buffer;
	}

	_tString ToHex(BYTE c)
	{
		TCHAR hex[3];

		const int val = c;

		_stprintf(hex, _T("%02X"), val);

		return hex;
	}
	_tString GetComputerName()
	{
		static bool gotName = false;

		static _tString name = _T("UNAVAILABLE");

		if (!gotName)
		{
			TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1] ;
			DWORD computerNameLen = MAX_COMPUTERNAME_LENGTH ;

			if (::GetComputerName(computerName, &computerNameLen))
			{
				name = computerName;
			}

			gotName = true;
		}

		return name;
	}
	

	HMODULE GetCurrentModule()
	{
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)
		// Here's a trick that will get you the handle of the module
		// you're running in without any a-priori knowledge:
		// http://www.dotnet247.com/247reference/msgs/13/65259.aspx

		MEMORY_BASIC_INFORMATION mbi;
		static int dummy;
		VirtualQuery(&dummy, &mbi, sizeof(mbi));

		return reinterpret_cast<HMODULE>(mbi.AllocationBase);

#else    // VC 7.0
		// from ATL 7.0 sources

		return reinterpret_cast<HMODULE>(&__ImageBase);
#endif
	}

	_tString GetModuleDir(HINSTANCE hModule /* = NULL */)
	{
		TCHAR szFull[_MAX_PATH];

		TCHAR szDrive[_MAX_DRIVE];

		TCHAR szDir[_MAX_DIR];

		if (hModule == NULL)
		{
			hModule = GetCurrentModule();
		}

		if (0 == ::GetModuleFileName(hModule, szFull, sizeof(szFull) / sizeof(TCHAR)))
		{
			throw suFuncException("GetModuleFileName()", "Failed to get Module directory");
		}

		_tsplitpath(szFull, szDrive, szDir, NULL, NULL);

		_tcscpy(szFull, szDrive);

		_tcscat(szFull, szDir);

		return _tString(szFull);
	}

	_tString GetModuleFileName(
		HINSTANCE hModule /* = NULL */)
	{		
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szName[_MAX_PATH];

		static bool gotName = false;

		static _tString name = _T("UNAVAILABLE");

		if (hModule == NULL)
		{
			hModule = GetCurrentModule();
		}

		if (!gotName)
		{
			TCHAR moduleFileName[MAX_PATH + 1] ;
			DWORD moduleFileNameLen = MAX_PATH ;			

			if (::GetModuleFileName(hModule, moduleFileName, moduleFileNameLen))
			{				
				_tsplitpath(moduleFileName, szDrive, szDir, szName, NULL);
				name = szName;
			}

			gotName = true;
		}

		return name;
	}

	_tString GetUserName()
	{
		static bool gotName = false;

		static _tString name = _T("UNAVAILABLE");

		if (!gotName)
		{
			TCHAR userName[256] ;
			DWORD userNameLen = 255;

			if (::GetUserName(userName, &userNameLen))
			{
				name = userName;
			}

			gotName = true;
		}

		return name;
	}

	_tString StripLeading(
		const _tString &source,
		const char toStrip)
	{
		const TCHAR *pSrc = source.c_str();

		while (pSrc && *pSrc == toStrip)
		{
			++pSrc;
		}

		return pSrc;
	}

	_tString StripTrailing(
		const _tString &source,
		const char toStrip)
	{
		size_t i = source.length();
		const _TCHAR *pSrc = source.c_str() + i;

		--pSrc;

		while (i && *pSrc == toStrip)
		{
			--pSrc;
			--i;
		}

		return source.substr(0, i);
	}

#pragma comment(lib, "Version.lib")

	_tString GetFileVersion()
	{
		_tString version;

		const _tString moduleFileName = GetModuleFileName(NULL);

		LPTSTR pModuleFileName = const_cast<LPTSTR>(moduleFileName.c_str());

		DWORD zero = 0;

		DWORD verSize = ::GetFileVersionInfoSize(pModuleFileName, &zero);

		if (verSize != 0)
		{
			auto_ptr<BYTE> spBuffer(new BYTE[verSize]);

			if (::GetFileVersionInfo(pModuleFileName, 0, verSize, spBuffer.get()))
			{
				LPTSTR pVersion = 0;
				UINT verLen = 0;

				if (::VerQueryValue(spBuffer.get(),
					const_cast<LPTSTR>(_T("\\StringFileInfo\\080904b0\\ProductVersion")),
					(void**)&pVersion,
					&verLen))
				{
					version = pVersion;
				}
			}
		}

		return version;
	}

	double WstringToDouble( const std::wstring ws )
	{
		 std::wistringstream wstrm(ws);
		 double number;
		 wstrm >> number;
		 return number;
	}

	int WstringToInt( const std::wstring ws )
	{
		std::wistringstream wstrm(ws);
		int number;
		wstrm >> number;
		return number;
	}
	int StringToInt( const std::string s )
	{
		std::istringstream strm(s);
		int number;
		strm >> number;
		return number;
	}


	std::wstring s2ws(const std::string& s)
	{		
		int slength = (int)s.length() + 1;
		int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
		std::wstring r(len-1, L'\0');   //s.c_str() add '\0' automatically
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
		return r;
	}

	std::string ws2s(const std::wstring& s)
	{	
		int slength = (int)s.length() + 1;
		int len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0); 
		std::string r(len-1, '\0');
		WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0); 
		return r.substr(0,slength-1);
	}



	std::string GBK2UTF(std::string gbkStr)
	{
		int len;
		wchar_t *strUnicode;
		char    *strUTF8;
		if (gbkStr.size() == 0)
		{
			return "";
		}
		len = MultiByteToWideChar(936, 0,gbkStr.c_str(), -1, NULL,0);
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
		len = MultiByteToWideChar(936, 0, gbkStr.c_str(), -1, strUnicode, len);
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
	}

	void Output( const _tString &message )
	{
#ifdef _DEBUG		

#ifdef _UNICODE
		std::wcout << ToString(GetCurrentThreadId()) << _T(": ") << message << endl;
#else
		std::cout << ToString(GetCurrentThreadId()) << _T(": ") << message << endl;
#endif

		const _tString	msg = ToString(GetCurrentThreadId()) + _T(": ") + message + _T("\n");

		OutputDebugString(msg.c_str());

		
#else
		//lint -e{715} symbol 'message' not referenced
#endif //_DEBUG
	}

}//end namespace SU