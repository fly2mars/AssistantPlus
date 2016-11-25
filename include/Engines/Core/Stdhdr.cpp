/*
Stdhdr.cpp
Written by Matthew Fisher

Collection of very useful functions/macros/constants.
See Stdhdr.h for a more thorough definition/explination of all constants/functions defined here.
*/

using namespace std;
namespace Utility
{
    void CopyStringToClipboard(const suString &S)
    {
        OpenClipboard(NULL);
        EmptyClipboard();

        HGLOBAL GlobalHandle;
        UINT BytesToCopy = S.Length() + 1;
        GlobalHandle = GlobalAlloc(GMEM_MOVEABLE, BytesToCopy);
        if(GlobalHandle != NULL)
        {
            BYTE *StringPointer = (BYTE*)GlobalLock(GlobalHandle);
            memcpy(StringPointer, S.CString(), BytesToCopy);
            GlobalUnlock(GlobalHandle);
            SetClipboardData(CF_TEXT, GlobalHandle);
        }
        CloseClipboard();
    }

    suString LoadStringFromClipboard()
    {
        suString Result;

        OpenClipboard(NULL);
        HGLOBAL GlobalHandle = GetClipboardData(CF_TEXT);
        if(GlobalHandle != NULL)
        {
            const char *StringPointer = (const char *)GlobalLock(GlobalHandle);
            if(StringPointer != NULL)
            {
                Result = StringPointer;
                GlobalUnlock(GlobalHandle);
            }
        }
        CloseClipboard();

        return Result;
    }

    void GetClipboardLines(suVector<suString> &Output)
    {
        Output.FreeMemory();

        OpenClipboard(NULL);
        HGLOBAL GlobalHandle = GetClipboardData(CF_TEXT);
        if(GlobalHandle != NULL)
        {
            const char *StringPointer = (const char *)GlobalLock(GlobalHandle);
            if(StringPointer != NULL)
            {
                suString Result = StringPointer;
                Result = Result.FindAndReplace(suString('\r'), suString());
                Result.Partition('\n', Output);
                GlobalUnlock(GlobalHandle);
            }
        }
        CloseClipboard();
    }

    bool FileExists(const suString &Filename)
    {
        std::ifstream File(Filename.CString());
        return (!File.fail());
    }

    suString GetNextLine(std::ifstream &File)
    {
        string CurLine;
        getline(File, CurLine);
        return suString(CurLine.c_str());
    }

    void GetFileLines(std::ifstream &File, suVector<suString> &Output)
    {
        //Output.FreeMemory();
        suVector<suString*> TempOutput;
        string CurLine;
        while(!File.fail())
        {
            getline(File, CurLine);
            if(!File.fail())
            {
                TempOutput.PushEnd(new suString(CurLine.c_str()));
            }
        }
        const UINT LineCount = TempOutput.Length();
        Output.Allocate(LineCount);
        for(UINT Index = 0; Index < LineCount; Index++)
        {
            Output[Index] = *(TempOutput[Index]);
            delete TempOutput[Index];
        }
    }

    void GetUnicodeFileLines(wifstream &File, suVector<suUnicodeString> &Output)
    {
        SignalError("wifstream seems to die on ASCII EOF character");
        Output.FreeMemory();
        wstring CurLine;
        while(!File.eof())
        {
            getline<UnicodeCharacter>(File, CurLine);
            if(!File.eof())
            {
                Output.PushEnd(suUnicodeString(CurLine.c_str()));
            }
        }
    }

    void GetFileLines(const suString &Filename, suVector<suString> &Output)
    {
        ifstream File(Filename.CString());
        PersistentAssert(!File.fail(), suString("Failed to open ") + Filename);
        GetFileLines(File, Output);
    }

    void GetUnicodeFileLines(const suString &Filename, suVector<suUnicodeString> &Output, UINT LineLimit)
    {
        suVector<BYTE> Data;
        Utility::GetFileData(Filename, Data);

        const UINT Length = Data.Length() / 2;
        const unsigned short *CArray = (unsigned short *)Data.CArray();

        UINT StringCount = 0;
        if(LineLimit == 0)
        {
            for(UINT Index = 0; Index < Length; Index++)
            {
                unsigned short CurCharacter = CArray[Index];
                if(CurCharacter == 0x000D || CurCharacter == 0x000A)
                {
                    if(Index < Length - 1 && (CArray[Index + 1] == 0x000D || CArray[Index + 1] == 0x000A))
                    {
                        Index++;
                    }
                    StringCount++;
                }
            }
			//Output.Allocate
            Output.Allocate(StringCount);
        }
        else
        {
            Output.Allocate(LineLimit);
        }

        UINT StringIndex = 0;
        suUnicodeString *CurString = &(Output[0]);

        bool Done = false;
        for(UINT Index = 0; Index < Length && !Done; Index++)
        {
            unsigned short CurCharacter = CArray[Index];
            if(CurCharacter == 0x000D || CurCharacter == 0x000A)
            {
                if(Index < Length - 1 && (CArray[Index + 1] == 0x000D || CArray[Index + 1] == 0x000A))
                {
                    Index++;
                }
                if(StringIndex == LineLimit - 1)
                {
                    Done = true;
                }
                else
                {
                    StringIndex++;
                    if(StringIndex != Output.Length())
                    {
                        CurString = &(Output[StringIndex]);
                    }
                }
            }
            else if(CurCharacter != 0xFEFF)
            {
                //Console::WriteString(suString::UnsignedIntAsHex(CurCharacter) + suString(" "));
                CurString->PushEnd(CurCharacter);
            }
        }
        if(LineLimit != 0)
        {
            Output.Allocate(StringIndex + 1);
        }
    }

    FILE* CheckedFOpen(const char *Filename, const char *Mode)
    {
        FILE *File = fopen(Filename, Mode);
        PersistentAssert(File != NULL && !ferror(File), suString("Failed to open file: ") + suString(Filename));
        return File;
    }

    UINT GetFileSize(const suString &Filename)
    {
        BOOL Success;
        WIN32_FILE_ATTRIBUTE_DATA Info;
        Success = GetFileAttributesEx(Filename.CString(), GetFileExInfoStandard, (void*)&Info);
        PersistentAssert(Success && Info.nFileSizeHigh == 0, suString("GetFileAttributesEx failed on ") + Filename);
        return Info.nFileSizeLow;
    }

    void GetFileData(const suString &Filename, suVector<BYTE> &Output)
    {
        FILE *InputFile = Utility::CheckedFOpen(Filename.CString(), "rb");
        UINT FileSize = Utility::GetFileSize(Filename);
        Output.Allocate(FileSize);
        Utility::CheckedFRead(Output.CArray(), sizeof(BYTE), FileSize, InputFile);
        fclose(InputFile);
    }

    void CopyFile(const suString &SourceFile, const suString &DestFile)
    {
        suVector<BYTE> Data;
        GetFileData(SourceFile, Data);
        FILE *File = Utility::CheckedFOpen(DestFile.CString(), "wb");
        Utility::CheckedFWrite(Data.CArray(), sizeof(BYTE), Data.Length(), File);
        fclose(File);
    }

    void MessageBox(const char *suString)
    {
        ::MessageBox(NULL, suString, "Data Report", MB_OK);
    }

    void MessageBox(const suString &S)
    {
        ::MessageBox(NULL, S.CString(), "Data Report", MB_OK);
    }

    //
    // This hash function taken from http://www.burtleburtle.net/bob/hash/doobs.html
    //

    __forceinline void HashMix(UINT &a, UINT &b, UINT &c)
    {
        a -= b; a -= c; a ^= (c>>13);
        b -= c; b -= a; b ^= (a<<8);
        c -= a; c -= b; c ^= (b>>13);
        a -= b; a -= c; a ^= (c>>12);
        b -= c; b -= a; b ^= (a<<16);
        c -= a; c -= b; c ^= (b>>5);
        a -= b; a -= c; a ^= (c>>3);
        b -= c; b -= a; b ^= (a<<10);
        c -= a; c -= b; c ^= (b>>15);
    }

    UINT Hash32(const BYTE *k, UINT Length)
    {
        UINT a, b, c, LocalLength;

        /* Set up the internal state */
        LocalLength = Length;
        a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
        c = 0x9b9773e9;

        /*---------------------------------------- handle most of the key */
        while (LocalLength >= 12)
        {
            a += (k[0] + ((UINT)k[1]<<8) + ((UINT)k[2]<<16) + ((UINT)k[3]<<24));
            b += (k[4] + ((UINT)k[5]<<8) + ((UINT)k[6]<<16) + ((UINT)k[7]<<24));
            c += (k[8] + ((UINT)k[9]<<8) + ((UINT)k[10]<<16)+ ((UINT)k[11]<<24));
            HashMix(a, b, c);
            k += 12;
            LocalLength -= 12;
        }

        /*------------------------------------- handle the last 11 bytes */
        c += Length;
        switch(LocalLength)              /* all the case statements fall through */
        {
            case 11: c += ((UINT)k[10]<<24);
            case 10: c += ((UINT)k[9]<<16);
            case 9 : c += ((UINT)k[8]<<8);
        /* the first byte of c is reserved for the length */
            case 8 : b += ((UINT)k[7]<<24);
            case 7 : b += ((UINT)k[6]<<16);
            case 6 : b += ((UINT)k[5]<<8);
        case 5 : b += k[4];
            case 4 : a += ((UINT)k[3]<<24);
            case 3 : a += ((UINT)k[2]<<16);
            case 2 : a += ((UINT)k[1]<<8);
        case 1 : a += k[0];
        /* case 0: nothing left to add */
        }
        HashMix(a, b, c);
        /*-------------------------------------------- report the result */
        return c;
    }

    UINT64 Hash64(const BYTE *k, UINT Length)
    {
        UINT a, b, c, LocalLength;

        /* Set up the internal state */
        LocalLength = Length;
        a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
        c = 0x9b9773e9;

        /*---------------------------------------- handle most of the key */
        while (LocalLength >= 12)
        {
            a += (k[0] + ((UINT)k[1]<<8) + ((UINT)k[2]<<16) + ((UINT)k[3]<<24));
            b += (k[4] + ((UINT)k[5]<<8) + ((UINT)k[6]<<16) + ((UINT)k[7]<<24));
            c += (k[8] + ((UINT)k[9]<<8) + ((UINT)k[10]<<16)+ ((UINT)k[11]<<24));
            HashMix(a, b, c);
            k += 12;
            LocalLength -= 12;
        }

        /*------------------------------------- handle the last 11 bytes */
        c += Length;
        switch(LocalLength)              /* all the case statements fall through */
        {
            case 11: c += ((UINT)k[10]<<24);
            case 10: c += ((UINT)k[9]<<16);
            case 9 : c += ((UINT)k[8]<<8);
        /* the first byte of c is reserved for the length */
            case 8 : b += ((UINT)k[7]<<24);
            case 7 : b += ((UINT)k[6]<<16);
            case 6 : b += ((UINT)k[5]<<8);
        case 5 : b += k[4];
            case 4 : a += ((UINT)k[3]<<24);
            case 3 : a += ((UINT)k[2]<<16);
            case 2 : a += ((UINT)k[1]<<8);
        case 1 : a += k[0];
        /* case 0: nothing left to add */
        }
        HashMix(a, b, c);
        /*-------------------------------------------- report the result */
        return UINT64(c) + UINT64(UINT64(a) << 32);
    }

    // Create a process with the given command line, and wait until it returns
    int RunCommand(const suString &ExecutablePath, const suString &CommandLine, bool Block)
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        ZeroMemory( &pi, sizeof(pi) );

        suString FullCommandLine = ExecutablePath.CString() + suString(" ") + CommandLine.CString();

        // Start the child process.
        if( !CreateProcess( NULL,   // No module name (use command line)
            FullCommandLine.CString(),  // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory
            &si,            // Pointer to STARTUPINFO structure
            &pi )           // Pointer to PROCESS_INFORMATION structure
        )
        {
            SignalError("CreateProcess failed");
            return -1;
        }

        if(Block)
        {
            // Wait until child process exits.
            WaitForSingleObject( pi.hProcess, INFINITE );
        }

        // Close process and thread handles.
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
        return 0;
    }
}

void Math::FindZeroNewton(float &s, UINT Iterations, float (*RealFunction) (float))
{
    //
    // Newton's method.  The derivative is approximated via stepping a distance DS away.
    //
    float FPrimeS, FS, DS = 0.0001f;

    for(UINT i = 0; i < Iterations; i++)
    {
        FS = RealFunction(s);            //compute the current point
        FPrimeS = (RealFunction(s + DS) - FS) / DS;    //compute the derivative
        if(FPrimeS == 0.0f) return;        //bail out if zero denominator
        s = s - FS / FPrimeS;            //Newton's step
    }
}

#if defined(_DEBUG)

void DebugOnlyAssert(bool Statement, const char *Description)
{
    if(!Statement)
    {
        SignalError(Description);
    }
}

void DebugOnlyAssert(bool Statement, const suString &Description)
{
    if(!Statement)
    {
        SignalError(Description.CString());
    }
}

#endif

#if defined(_DEBUG) || defined(ALWAYS_ASSERT)

void SignalError(const char *Description)
{
    MessageBox(NULL, Description, "Fatal Problem Encountered", MB_OK);
#ifdef _DEBUG
    __asm int 3;
#endif
    ExitProcess(1);
}

void SignalError(const suString &Description)
{
    MessageBox(NULL, Description.CString(), "Fatal Problem Encountered", MB_OK);
#ifdef _DEBUG
    __asm int 3;
#endif
    ExitProcess(1);
}

void Assert(bool Statement, const char *Description)
{
    if(!Statement)
    {
        MessageBox(NULL, Description, "Fatal Problem Encountered", MB_OK);
#ifdef _DEBUG
        __asm int 3;
#endif
        ExitProcess(1);
    }
}

void Assert(bool Statement, const suString &Description)
{
    if(!Statement)
    {
        MessageBox(NULL, Description.CString(), "Fatal Problem Encountered", MB_OK);
#ifdef _DEBUG
        __asm int 3;
#endif
        ExitProcess(1);
    }
}

#endif

void PersistentSignalError(const char *Description)
{
    MessageBox(NULL, Description, "Fatal Problem Encountered", MB_OK);
#ifdef _DEBUG
    __asm int 3;
#endif
    ExitProcess(1);
}

void PersistentSignalError(const suString &Description)
{
    MessageBox(NULL, Description.CString(), "Fatal Problem Encountered", MB_OK);
#ifdef _DEBUG
    __asm int 3;
#endif
    ExitProcess(1);
}

void PersistentAssert(bool Statement, const char *Description)
{
    if(!Statement)
    {
        MessageBox(NULL, Description, "Fatal Problem Encountered", MB_OK);
#ifdef _DEBUG
        __asm int 3;
#endif
        ExitProcess(1);
    }
}

void PersistentAssert(bool Statement, const suString &Description)
{
    if(!Statement)
    {
        MessageBox(NULL, Description.CString(), "Fatal Problem Encountered", MB_OK);
#ifdef _DEBUG
        __asm int 3;
#endif
        ExitProcess(1);
    }
}