
#include <iostream>
#include <fstream>
#include <locale> 
#include <codecvt>
#include <unistd.h>


namespace SU {

	void Output(const _tString &message)
	{
#ifdef _DEBUG		

#ifdef _UNICODE
		std::wcout << ToString(GetCurrentThreadId()) << _T(": ") << message << endl;
#else
		std::cout << ToString(GetCurrentThreadId()) << _T(": ") << message << endl;
#endif

		//todo: use syslog on linux 
		//const _tString	msg = ToString(GetCurrentThreadId()) + _T(": ") + message + _T("\n");

		//OutputDebugString(msg.c_str());

#else
		//lint -e{715} symbol 'message' not referenced
#endif //_DEBUG
	}

	//https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
	//c++ 11
	std::wstring s2ws(const std::string& s)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(s);

	}
	std::string ws2s(const std::wstring& s)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(s);
	}

	int WstringToInt(const std::wstring ws)
	{
		return std::stoi(ws);
	}
	double WstringToDouble(const std::wstring ws)
	{
		return std::stod(ws);
	}
	int StringToInt(const std::string s)
	{
		return std::stoi(s);
	}

	/**
	* Format a string object
	*/
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
			BYTE c;

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
	_tString GetModuleFileName()
	{
		char arg1[20];
		char exepath[PATH_MAX + 1] = { 0 };

		sprintf(arg1, "/proc/%d/exe", getpid());
		readlink(arg1, exepath, 1024);
		suString fullPath = _tString(exepath).c_str();
		int idx = fullPath.FindLastIndex('/');
		if (idx != -1)
			return _tString(fullPath.SubString(idx + 1, fullPath.Length() - idx - 1).CString());

	}

	_tString GetModuleDir()
	{
		char arg1[20];
		char exepath[PATH_MAX + 1] = { 0 };

		sprintf(arg1, "/proc/%d/exe", getpid());
		readlink(arg1, exepath, 1024);

		suString fullPath = _tString(exepath).c_str();
		int idx = fullPath.FindLastIndex('/');

		if (idx != -1)
			return _tString(fullPath.SubString(0, idx+1).CString() );

		return _tString("/");
	}

	_tString GetCurrentDirectory()
	{
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			return _tString(cwd);
		else
			return _tString("");
	}
	

}//end namespace SU