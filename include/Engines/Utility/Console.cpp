/*
Console.cpp
Written by Matthew Fisher

Functions for interacting with the console.

example
   use define to output info into file
   #define CONSOLE_TO_FILE   

*/
using namespace std;
namespace Console
{
#ifdef CONSOLE_TO_FILE   
	ofstream ConsoleFile("Console.txt");

    ofstream& File()
    {
        return ConsoleFile;
    }
#endif


    void AdvanceLine()
    {
        WriteLine("");
    }

    void WriteLine(const suString &S)
    {
        HANDLE StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD BytesWritten;
        suString FinalString = S + suString("\n");
        WriteConsole(StdHandle, FinalString.CString(), FinalString.Length(), &BytesWritten, NULL);

#ifdef CONSOLE_TO_FILE   
        ConsoleFile << S << endl;
        ConsoleFile.flush();
#endif

    }

    void OverwriteLine(const suString &S)
    {
        const UINT ConsoleWidth = 79;
        HANDLE StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO CursorInfo;
        GetConsoleScreenBufferInfo(StdHandle, &CursorInfo);
        CursorInfo.dwCursorPosition.X = 0;
        CursorInfo.dwCursorPosition.Y = Math::Max(CursorInfo.dwCursorPosition.Y - 1, 0);
        SetConsoleCursorPosition(StdHandle, CursorInfo.dwCursorPosition);
        DWORD BytesWritten;
        suString FinalString = S;
        while(FinalString.Length() < ConsoleWidth)
        {
            FinalString.PushEnd(' ');
        }
        FinalString.PushEnd('\n');
        WriteConsole(StdHandle, FinalString.CString(), FinalString.Length(), &BytesWritten, NULL);
#ifdef CONSOLE_TO_FILE   
        ConsoleFile << S << endl;
        ConsoleFile.flush();
#endif
    }

    void WriteString(const suString &S)
    {
        HANDLE StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD BytesWritten;
        WriteConsole(StdHandle, S.CString(), S.Length(), &BytesWritten, NULL);
#ifdef CONSOLE_TO_FILE   
        ConsoleFile << S;
        ConsoleFile.flush();
#endif
    }

	
}