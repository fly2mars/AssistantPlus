/*
suString.h
Written by Matthew Fisher

suString class and related conversions.
*/

#pragma once

//
// Intellisense for custom types
//
//http://thejefffiles.com/blog/autoexpdat-your-key-to-better-debugging-part-1/

class suString
{
public:
    struct LexicographicComparison
    {
        bool operator()(const suString &L, const suString &R) const
        {
            return strcmp(L.CString(), R.CString()) < 0;
        }
        static bool Compare(const suString &L, const suString &R)
        {
            return strcmp(L.CString(), R.CString()) < 0;
        }
    };

    suString()
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
    }

    ~suString()
    {
        if(_Data != NULL)
        {
            delete[] _Data;
        }
    }

    suString(const suString &S)
    {
        if(S._Data == NULL)
        {
            _Data = NULL;
            _Capacity = 0;
            _Length = 0;
        }
        else
        {
            _Length = S._Length;
            const UINT NewCapacity = _Length + 1;
            _Capacity = NewCapacity;
            _Data = new char[NewCapacity];
            memcpy(_Data, S._Data, NewCapacity);
        }
    }
    explicit suString(const suUnicodeString &S);
    explicit suString(UINT Value)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = Value;
    }
    explicit suString(UINT64 Value)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = Value;
    }
    explicit suString(DWORD Value)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = int(Value);
    }
    explicit suString(int Value)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = Value;
    }
    explicit suString(float Value)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = Value;
    }
    explicit suString(double Value)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = float(Value);
    }
    suString(const char *Text)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = Text;
    }
    explicit suString(char Character)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = Character;
    }
    explicit suString(bool B)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        if(B)
        {
            *this = "true";
        }
        else
        {
            *this = "false";
        }
    }

    //
    // Memory
    //
    __forceinline void FreeMemory()
    {
        if(_Data != NULL)
        {
            delete[] _Data;
            _Data = NULL;
        }
        _Length = 0;
        _Capacity = 0;
    }

    __forceinline void Allocate(UINT Capacity)
    {
        if(_Data != NULL)
        {
            delete[] _Data;
        }
        _Data = new char[Capacity];
        _Data[0] = '\0';
        _Length = 0;
        _Capacity = Capacity;
    }

    __forceinline void ReSize(UINT NewLength)
    {
        const UINT NewCapacity = NewLength + 1;
        _Length = Math::Min(_Length, NewLength);
        char *NewData = new char[NewCapacity];
        if(_Length > 0)
        {
            memcpy(NewData, _Data, _Length);
        }
        NewData[_Length] = '\0';
        if(_Data != NULL)
        {
            delete[] _Data;
        }
        _Data = NewData;
        _Capacity = NewCapacity;
    }

    //
    // Accessors
    //
    __forceinline char* CString()
    {
        if(_Data != NULL)
        {
            return _Data;
        }
        else
        {
            return (char *)&(_Data);
        }
    }

    __forceinline const char* CString() const
    {
        if(_Data != NULL)
        {
            return _Data;
        }
        else
        {
            return (char *)&(_Data);
        }
    }

    __forceinline UINT Length() const
    {
        return _Length;
    }

    __forceinline char Last() const
    {
#ifdef VECTOR_DEBUG
        if(_Length == 0)
        {
            SignalError("Last called on zero-length suString");
        }
#endif
        return _Data[_Length - 1];
    }

    __forceinline char& operator [] (UINT k)
    {
#ifdef VECTOR_DEBUG
        if(k >= _Length)
        {
            SignalError("Out-of-bounds suString access");
        }
#endif
        return _Data[k];
    }

    __forceinline char& operator [] (int k) 
    {
#ifdef VECTOR_DEBUG
        if(k < 0 || k >= int(_Length))
        {
            SignalError("Out-of-bounds suString access");
        }
#endif
        return _Data[k];
    }

    __forceinline const char& operator [] (UINT k) const
    {
#ifdef VECTOR_DEBUG
        if(k >= _Length)
        {
            SignalError("Out-of-bounds suString access");
        }
#endif
        return _Data[k];
    }

    __forceinline const char& operator [] (int k) const
    {
#ifdef VECTOR_DEBUG
        if(k < 0 || k >= int(_Length))
        {
            SignalError("Out-of-bounds suString access");
        }
#endif
        return _Data[k];
    }

    //
    // Conversions
    //
    int ConvertToInteger() const
    {
        return atoi(CString());
    }

    UINT ConvertToUnsignedInteger() const
    {
        return UINT(_atoi64(CString()));
    }

    LONGLONG ConvertToLongInteger() const
    {
        return _atoi64(CString());
    }

    ULONGLONG ConvertToLongUnsignedInteger() const
    {
        return ULONGLONG(_atoi64(CString()));
    }

    double ConvertToDouble() const
    {
        return atof(CString());
    }

    bool ConvertToBoolean() const;

    float ConvertToFloat() const
    {
        return float(atof(CString()));
    }

    /*Vec4f ConvertToVec4f() const
    {
        Vec4f Result;
        sscanf_s(_Data, "%f %f %f %f", &Result.x, &Result.y, &Result.z, &Result.w);
        return Result;
    }*/

    //
    // Query
    //
    bool ExactMatchAtOffset(const suString &Find, UINT Offset) const;
    bool Contains(const suString &Find) const;
    bool IsNumeric() const;
    bool IsSuffix(const suString &EndCanidate) const;
    bool IsPrefix(const suString &StartCanidate) const;
    void Partition(char Seperator, suVector<suString> &Output, bool PushEmptysuStrings = false) const;
    suVector<suString> Partition(char Seperator) const;
    void PartitionAboutIndex(UINT Index, suString &Left, suString &Right) const;
    void Partition(const suString &Seperator, suVector<suString> &Output) const;
    int FindFirstIndex(char Seperator) const;
    int FindLastIndex(char Seperator) const;
    suString FindAndReplace(const suString &Find, const suString &Replace) const;
    suString MakeLowercase() const;
	suString MakeUppercase() const;
    suString RemoveSuffix(const suString &EndCandidate) const;
    suString RemovePrefix(const suString &StartCandidate) const;
	suString FindExtension() const;   //find file extension, added by Yao
	suString SubString(int iStart, int nLen);
	UINT Hash() const;

	

    
    //
    // Assignment
    //
    suString& operator = (const char *suString);
    suString& operator = (char Character);
    suString& operator = (float Value);
    suString& operator = (int Value);
    suString& operator = (UINT Value);
    suString& operator = (UINT64 Value);
    suString& operator = (const suString &S);

    //
    // Modifiers
    //
    suString& operator += (const suString &S);
    __forceinline void operator += (char C)
    {
        PushEnd(C);
    }

    __forceinline void PushEnd(char C)
    {
        if(_Length + 1 >= _Capacity)
        {
            ReSize(_Capacity * 2 + 3);
        }
        _Data[_Length] = C;
        _Length++;
        _Data[_Length] = '\0';
    }

    __forceinline void PopEnd()
    {
#ifdef VECTOR_DEBUG
        if(_Length == 0)
        {
            SignalError("Pop called on empty suString");
        }
#endif
        _Length--;
        _Data[_Length] = '\0';
    }

    void PopFront()
    {
#ifdef VECTOR_DEBUG
        if(_Length == 0)
        {
            SignalError("Pop called on empty suString");
        }
#endif
        _Length--;
        for(UINT Index = 0; Index < _Length; Index++)
        {
            _Data[Index] = _Data[Index + 1];
        }
        _Data[_Length] = '\0';
    }

    //
    // Formatting
    //
    static suString UnsignedIntAsHex(UINT Value);
    static suString ZeroPad(const suString &S, UINT ZeroPadding);

private:
    void ResizeToCStringLength();
    friend suString operator + (const suString &L, const suString &R);
    friend bool operator == (const suString &L, const suString &R);
    friend bool operator == (const char *L, const suString &R);

    //suVector<char> _Data;
    char *_Data;
    UINT _Capacity;
    UINT _Length;
};

//
// suString Comparison
//
__forceinline bool operator == (const suString &L, const suString &R)
{
    const UINT LLen = L._Length;
    const UINT RLen = R._Length;
    if(LLen != RLen)
    {
        return false;
    }
    const char *LData = L._Data;
    const char *RData = R._Data;
    for(UINT i = 0; i < LLen; i++)
    {
        if(LData[i] != RData[i])
        {
            return false;
        }
    }
    return true;
}

__forceinline bool operator == (const char *L, const suString &R)
{
    const UINT LLen = UINT(strlen(L));
    const UINT RLen = R._Length;
    if(LLen != RLen)
    {
        return false;
    }
    const char *RData = R._Data;
    for(UINT i = 0; i < RLen; i++)
    {
        if(L[i] != RData[i])
        {
            return false;
        }
    }
    return true;
}

__forceinline bool operator == (const suString &R, const char *L)
{
    return (L == R);
}

__forceinline bool operator != (const suString &L, const suString &R)
{
    return !(L == R);
}
__forceinline bool operator != (const char *L, const suString &R)
{
    return !(L == R);
}
__forceinline bool operator != (const suString &R, const char *L)
{
    return !(L == R);
}

//
// suString Operations
//
suString operator + (const suString &L, const suString &R);
//suString operator + (const char *L, const suString &R);
//suString operator + (const suString &R, const char *L);
std::ostream& operator << (std::ostream &os, const suString &S);
