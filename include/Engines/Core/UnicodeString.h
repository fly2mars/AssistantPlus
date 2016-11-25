/*
suUnicodeString.h
Written by Matthew Fisher

suString class and related conversions.
*/

#pragma once

//
// Intellisense for custom types
//
//http://thejefffiles.com/blog/autoexpdat-your-key-to-better-debugging-part-1/

typedef wchar_t UnicodeCharacter;

const UnicodeCharacter UnicodeNullTerminator = 0;
const UnicodeCharacter UnicodeHeader = 0xFEFF;
const UnicodeCharacter UnicodeNewline = 0x000D;
const UnicodeCharacter UnicodeSpace = 0x0020;
        

class suUnicodeString
{
public:
    
    struct LexicographicComparison
    {
        bool operator()(const suUnicodeString &L, const suUnicodeString &R) const
        {
            SignalError("Not implemented");
        }
    };

    suUnicodeString()
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
    }

    ~suUnicodeString()
    {
        if(_Data != NULL)
        {
            delete[] _Data;
        }
    }

    suUnicodeString(const suUnicodeString &S)
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
            _Data = new UnicodeCharacter[NewCapacity];
            memcpy(_Data, S._Data, NewCapacity * sizeof(UnicodeCharacter));
        }
    }
    suUnicodeString(const UnicodeCharacter *Text)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = Text;
    }
    explicit suUnicodeString(UnicodeCharacter C)
    {
        _Data = NULL;
        _Capacity = 0;
        _Length = 0;
        *this = C;
    }
    explicit suUnicodeString(const suVector<UnicodeCharacter> &S)
    {
        _Length = S.Length();
        _Capacity = _Length + 1;
        _Data = new UnicodeCharacter[_Capacity];
        memcpy(_Data, S.CArray(), _Length * sizeof(UnicodeCharacter));
        _Data[_Length] = UnicodeNullTerminator;
    }
    explicit suUnicodeString(const suString &S)
    {
        _Length = S.Length();
        _Capacity = _Length + 1;
        _Data = new UnicodeCharacter[_Capacity];
        for(UINT Index = 0; Index < _Length; Index++)
        {
            _Data[Index] = S[Index];
        }
        _Data[_Length] = UnicodeNullTerminator;
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
        _Data = new UnicodeCharacter[Capacity];
        _Data[0] = UnicodeNullTerminator;
        _Length = 0;
        _Capacity = Capacity;
    }

    __forceinline void ReSize(UINT NewLength)
    {
        const UINT NewCapacity = NewLength + 1;
        _Length = Math::Min(_Length, NewLength);
        UnicodeCharacter *NewData = new UnicodeCharacter[NewCapacity];
        if(_Length > 0)
        {
            memcpy(NewData, _Data, _Length * sizeof(UnicodeCharacter));
        }
        NewData[_Length] = UnicodeNullTerminator;
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
    __forceinline UnicodeCharacter* CString()
    {
        if(_Data != NULL)
        {
            return _Data;
        }
        else
        {
            return (UnicodeCharacter *)&(_Data);
        }
    }

    __forceinline const UnicodeCharacter* CString() const
    {
        if(_Data != NULL)
        {
            return _Data;
        }
        else
        {
            return (UnicodeCharacter *)&(_Data);
        }
    }

    __forceinline UINT Length() const
    {
        return _Length;
    }

    __forceinline UnicodeCharacter Last() const
    {
#ifdef VECTOR_DEBUG
        if(_Length == 0)
        {
            SignalError("Last called on zero-length string");
        }
#endif
        return _Data[_Length - 1];
    }

    __forceinline UnicodeCharacter& operator [] (UINT k)
    {
#ifdef VECTOR_DEBUG
        if(k >= _Length)
        {
            SignalError("Out-of-bounds string access");
        }
#endif
        return _Data[k];
    }

    __forceinline UnicodeCharacter& operator [] (int k) 
    {
#ifdef VECTOR_DEBUG
        if(k < 0 || k >= int(_Length))
        {
            SignalError("Out-of-bounds string access");
        }
#endif
        return _Data[k];
    }

    __forceinline const UnicodeCharacter& operator [] (UINT k) const
    {
#ifdef VECTOR_DEBUG
        if(k >= _Length)
        {
            SignalError("Out-of-bounds string access");
        }
#endif
        return _Data[k];
    }

    __forceinline const UnicodeCharacter& operator [] (int k) const
    {
#ifdef VECTOR_DEBUG
        if(k < 0 || k >= int(_Length))
        {
            SignalError("Out-of-bounds string access");
        }
#endif
        return _Data[k];
    }

    //
    // Assignment
    //
    suUnicodeString& operator = (UnicodeCharacter Character);
    suUnicodeString& operator = (const UnicodeCharacter *S);
    suUnicodeString& operator = (const suUnicodeString &S);

    //
    // Query
    //
    bool ExactMatchAtOffset(const suUnicodeString &Find, UINT Offset) const;
    bool Contains(const suUnicodeString &Find) const;
    bool IsNumeric() const;
    bool IsSuffix(const suUnicodeString &EndCanidate) const;
    bool IsPrefix(const suUnicodeString &StartCanidate) const;
    void Partition(UnicodeCharacter Seperator, suVector<suUnicodeString> &Output) const;
    suVector<suUnicodeString> Partition(UnicodeCharacter Seperator) const;
    void PartitionAboutIndex(UINT Index, suUnicodeString &Left, suUnicodeString &Right) const;
    void Partition(const suUnicodeString &Seperator, suVector<suUnicodeString> &Output) const;
    int FindFirstIndex(UnicodeCharacter Seperator) const;
    int FindLastIndex(UnicodeCharacter Seperator) const;
    suUnicodeString FindAndReplace(const suUnicodeString &Find, const suUnicodeString &Replace) const;
    suUnicodeString RemoveSuffix(const suUnicodeString &EndCandidate) const;
    UINT Hash() const;
    
    //
    // Modifiers
    //
    suUnicodeString& operator += (const suUnicodeString &S);
    __forceinline void operator += (UnicodeCharacter C)
    {
        PushEnd(C);
    }

    __forceinline void PushEnd(UnicodeCharacter C)
    {
        if(_Length + 1 >= _Capacity)
        {
            ReSize(_Capacity * 2 + 3);
        }
        _Data[_Length] = C;
        _Length++;
        _Data[_Length] = UnicodeNullTerminator;
    }

    __forceinline void PopEnd()
    {
#ifdef VECTOR_DEBUG
        if(_Length == 0)
        {
            SignalError("Pop called on empty string");
        }
#endif
        _Length--;
        _Data[_Length] = UnicodeNullTerminator;
    }

    void PopFront()
    {
#ifdef VECTOR_DEBUG
        if(_Length == 0)
        {
            SignalError("Pop called on empty string");
        }
#endif
        _Length--;
        for(UINT Index = 0; Index < _Length; Index++)
        {
            _Data[Index] = _Data[Index + 1];
        }
    }

private:
    void ResizeToCStringLength();
    friend suUnicodeString operator + (const suUnicodeString &L, const suUnicodeString &R);

    UnicodeCharacter *_Data;
    UINT _Capacity;
    UINT _Length;
};

//
// suString Comparison
//
bool operator == (const suUnicodeString &L, const suUnicodeString &R);
bool operator == (const UnicodeCharacter *L, const suUnicodeString &R);
bool operator == (const suUnicodeString &R, const UnicodeCharacter *L);
__forceinline bool operator != (const suUnicodeString &L, const suUnicodeString &R) {return !(L == R);}
__forceinline bool operator != (const UnicodeCharacter *L, const suUnicodeString &R) {return !(L == R);}
__forceinline bool operator != (const suUnicodeString &R, const UnicodeCharacter *L) {return !(L == R);}

//
// suString Operations
//
suUnicodeString operator + (const suUnicodeString &L, const suUnicodeString &R);
std::ostream& operator << (std::ostream &os, const suUnicodeString &S);

