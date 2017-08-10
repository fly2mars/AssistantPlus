#pragma once
/**
Common utility functions.
*/
#include <Engines/Utility/tstring.h>
#include <strstream>
#include <cstdarg>
#include <Engines/Core/Vector.h>


namespace SU {
	void Output(const _tString &message);
	std::wstring s2ws(const std::string& s);
	std::string ws2s(const std::wstring& s);

	//Using std::stoi.... to replace the following functions. 
	int WstringToInt(const std::wstring ws);
	double WstringToDouble(const std::wstring ws);
	int StringToInt(const std::string s);
	/**
	* Format a string object
	*/
	_tString formatString(const char* format, ...);
	//
	_tString HexToString(const BYTE *pBuffer, size_t iBytes);

	void StringToHex(const _tString &str, BYTE *pBuffer, size_t nBytes);

	//_tString GetLastErrorMessage(DWORD last_error);

	//https://stackoverflow.com/questions/1528298/get-path-of-executable
	///HMODULE  GetCurrentModule();                              //!<Get handle of current EXE/DLL 
	_tString GetModuleFileName();
	_tString GetModuleDir();            //!<获取执行程序目录
	_tString GetCurrentDirectory();      //!<获取运行时目录

	_tString GetDateStamp();
	_tString GetCurTime();
	_tString GetTimeStamp();             //!<返回YYYYMMDDHHMMSS


	_tString ToHex(BYTE c);

	_tString GetComputerName();
	_tString GetUserName();

	_tString GetFileVersion();

	_tString StripLeading(const _tString &source, const char toStrip);

	_tString StripTrailing(const _tString &source, const char toStrip);

	std::string GBK2UTF(std::string gbkStr);

	// Output vector to file  
	//
	template <class T> void SaveToASCIIFile(suVector<T> &v, const suString &Filename)
	{
		std::ofstream File(Filename.CString());
		PersistentAssert(!File.fail(), "Failed to open file");
		File << v._Length << std::endl;
		for (unsigned int Index = 0; Index < v._Length; Index++)
		{
			File << v._Data[Index] << std::endl;
		}
	}
	//
	// Read vector from file  
	//
	template <class T> void LoadFromASCIIFile(suVector<T> &v, const suString &Filename)
	{
		std::ifstream File(Filename.CString());
		PersistentAssert(!File.fail(), "Failed to open file");
		unsigned int Length;
		File >> Length;
		v.Allocate(Length);
		for (unsigned int Index = 0; Index < Length; Index++)
		{
			File >> v._Data[Index];
		}
	}

	

	/**
	* Converts a type to a _tString.
	* Convert a type to a string by streaming it. Requires that there's an ostream
	* inserter available for type T.
	*/
	template <class T>
	_tString ToString(T num)
	{
		_tString strNum = "";

		{
			std::strstream buf;

			buf << num << std::ends;

#ifdef _UNICODE
			std::string temp = buf.str();

			USES_CONVERSION;

			strNum = A2W(temp.c_str());
#else
			strNum = buf.str();
#endif
			buf.freeze(false);
		}

		return strNum;
	}

	template <class T>
	bool ToBool(const T &value)
	{
		return (0 != value);
	}

	inline bool BOOL_to_bool(const BOOL bResult)
	{
		// Convert a make believe BOOL into a real bool.
		// Removes warning C4800...

		return (TRUE == bResult);
	}




}//end name space SU