/*
suString.cpp
Written by Matthew Fisher

Implementation of suString class.
*/

suString::suString(const suUnicodeString &S)
{
    const UINT SLength = S.Length();
    _Length = SLength;
    _Capacity = _Length + 1;
    _Data = new char[_Length + 1];
    for(UINT Index = 0; Index < SLength; Index++)
    {
        if(S[Index] >= 128)
        {
            _Data[Index] = '#';
        }
        else
        {
            _Data[Index] = char(S[Index]);
        }
    }
    _Data[SLength] = '\0';
}

suString suString::UnsignedIntAsHex(UINT Value)
{
    suString S;
    S.Allocate(20);
    sprintf(S.CString(), "%x", Value);
    S.ResizeToCStringLength();
    return S;
}

suString suString::ZeroPad(const suString &S, UINT ZeroPadding)
{
    suString Prefix;
    for(UINT i = S.Length(); i < ZeroPadding; i++)
    {
        Prefix.PushEnd('0');
    }
    return Prefix + S;
}

bool suString::ConvertToBoolean() const
{
    if(*this == "true")
    {
        return true;
    }
    if(*this == "false")
    {
        return false;
    }
    SignalError("Invalid string to boolean conversion");
    return false;
}

void suString::ResizeToCStringLength()
{
    for(UINT i = 0; i < _Capacity; i++)
    {
        if(_Data[i] == '\0')
        {
            _Length = i;
            ReSize(_Length + 1);
            return;
        }
    }
    SignalError("No null terminator found in suString::ResizeToCStringLength");
}

suString& suString::operator = (float Value)
{
    ostringstream S(ostringstream::out);
    S << Value;
    *this = S.str().c_str();
    return *this;
}

suString& suString::operator = (char Character)
{
    Allocate(4);
    _Length = 1;
    _Data[0] = Character;
    _Data[1] = '\0';
    return *this;
}

suString& suString::operator = (int Value)
{
    ostringstream S(ostringstream::out);
    S << Value;
    *this = S.str().c_str();
    return *this;
}

suString& suString::operator = (UINT Value)
{
    ostringstream S(ostringstream::out);
    S << Value;
    *this = S.str().c_str();
    return *this;
}

suString& suString::operator = (UINT64 Value)
{
    ostringstream S(ostringstream::out);
    S << Value;
    *this = S.str().c_str();
    return *this;
}

suString& suString::operator = (const char *suString)
{
    UINT NewLength = UINT(strlen(suString));
    Allocate(NewLength + 1);
    memcpy(_Data, suString, NewLength + 1);
    _Length = NewLength;
    return *this;
}

suString& suString::operator = (const suString &S)
{
    if(S._Data == NULL)
    {
        FreeMemory();
    }
    else if(&S != this)
    {
        Allocate(S._Length + 1);
        _Length = S._Length;
        memcpy(_Data, S._Data, _Capacity);
    }
    return *this;
}

suString& suString::operator += (const suString &S)
{
    if(S._Length > 0)
    {
        const UINT StartLength = _Length;
        ReSize(StartLength + S._Length + 1);
        memcpy(_Data + StartLength, S._Data, S._Length + 1);
        _Length = StartLength + S._Length;
    }
    return *this;
}

suString suString::MakeLowercase() const
{
    suString Result(*this);
    for(UINT i = 0; i < Result._Length; i++)
    {
        Result[i] = tolower(Result[i]);
    }
    return Result;
}

void suString::Partition(char Seperator, suVector<suString> &Output, bool PushEmptyStrings) const
{
    Output.FreeMemory();
    suString CurEntry;
    const UINT Length = _Length;
    for(UINT Index = 0; Index < Length; Index++)
    {
        if(_Data[Index] == Seperator)
        {
            if(CurEntry._Length > 0 || PushEmptyStrings)
            {
                Output.PushEnd(CurEntry);
                CurEntry._Length = 0;
                CurEntry._Data[0] = '\0';
            }
        }
        else
        {
            CurEntry.PushEnd(_Data[Index]);
        }
    }
    if(CurEntry._Length > 0)
    {
        Output.PushEnd(CurEntry);
    }
}

suVector<suString> suString::Partition(char Seperator) const
{
    suVector<suString> Result;
    Partition(Seperator, Result);
    return Result;
}

int suString::FindFirstIndex(char Seperator) const
{
    UINT L = Length();
    for(UINT CurIndex = 0; CurIndex < L; CurIndex++)
    {
        if(_Data[CurIndex] == Seperator)
        {
            return CurIndex;
        }
    }
    return -1;
}

int suString::FindLastIndex(char Seperator) const
{
    UINT L = Length();
    for(int CurIndex = L - 1; CurIndex >= 0; CurIndex--)
    {
        if(_Data[CurIndex] == Seperator)
        {
            return CurIndex;
        }
    }
    return -1;
}

void suString::PartitionAboutIndex(UINT Index, suString &Left, suString &Right) const
{
    UINT L = Length();
    Assert(Index < L, "Index invalid in suString::PartitionAboutIndex");
    
    Left.ReSize(Index);
    for(UINT LeftIndex = 0; LeftIndex < Index; LeftIndex++)
    {
        Left._Data[LeftIndex] = _Data[LeftIndex];
    }
    Left._Data[Index] = '\0';
    Left._Length = Index;

    const UINT RLength = L - Index - 1;
    Right.ReSize(RLength);
    for(UINT RightIndex = 0; RightIndex < RLength; RightIndex++)
    {
        Right._Data[RightIndex] = _Data[Index + 1 + RightIndex];
    }
    Right._Data[RLength] = '\0';
    Right._Length = RLength;
}

void suString::Partition(const suString &Seperator, suVector<suString> &Output) const
{
    Assert(Seperator.Length() >= 1, "empty seperator");
    Output.FreeMemory();
    suString CurEntry;
    for(UINT CurIndex = 0; CurIndex < Length(); CurIndex++)
    {
        bool IsSeperator = true;
        for(UINT TestIndex = 0; TestIndex < Seperator.Length() && CurIndex + TestIndex < Length() && IsSeperator; TestIndex++)
        {
            if(_Data[CurIndex + TestIndex] != Seperator[TestIndex])
            {
                IsSeperator = false;
            }
        }

        if(IsSeperator)
        {
            if(CurEntry._Length > 0)
            {
                Output.PushEnd(CurEntry);
                CurEntry._Length = 0;
                CurEntry._Data[0] = '\0';
            }
            CurIndex += Seperator._Length - 1;
        }
        else
        {
            CurEntry.PushEnd(_Data[CurIndex]);
        }
    }
    if(CurEntry._Length > 0)
    {
        Output.PushEnd(CurEntry);
    }
}

bool suString::ExactMatchAtOffset(const suString &Find, UINT Offset) const
{
    UINT MatchLength = 0;
    for(UINT Index = 0; Index + Offset < _Length && Index < Find._Length; Index++)
    {
        if(_Data[Index + Offset] == Find[Index])
        {
            MatchLength++;
            if(MatchLength == Find._Length)
            {
                return true;
            }
        }
    }
    return false;
}

bool suString::Contains(const suString &Find) const
{
    const UINT Length = _Length;
    for(UINT Index = 0; Index < Length; Index++)
    {
        if(ExactMatchAtOffset(Find, Index))
        {
            return true;
        }
    }
    return false;
}

suString suString::FindAndReplace(const suString &Find, const suString &Replace) const
{
    suString Result;
    const UINT Length = _Length;
    for(UINT Index = 0; Index < Length; Index++)
    {
        if(ExactMatchAtOffset(Find, Index))
        {
            Result += Replace;
            Index += Find.Length() - 1;
        }
        else
        {
            Result += _Data[Index];
        }
    }
    return Result;
}

bool suString::IsNumeric() const
{
    return (_Length > 0 &&
           (ConvertToDouble() != 0.0 ||
            _Data[0] == '0' ||
            _Data[0] == '.' ||
            _Data[0] == '-'));
}

bool suString::IsSuffix(const suString &EndCanidate) const
{
    if(_Length < EndCanidate._Length)
    {
        return false;
    }
    for(UINT i = 0; i < EndCanidate._Length; i++)
    {
        if(_Data[_Length - EndCanidate._Length + i] != EndCanidate._Data[i])
        {
            return false;
        }
    }
    return true;
}

suString suString::RemoveSuffix(const suString &EndCandidate) const
{
    Assert(IsSuffix(EndCandidate), "Removing invalid suffix");
    suString Result = *this;
    for(UINT i = 0; i < EndCandidate._Length; i++)
    {
        Result.PopEnd();
    }
    return Result;
}

suString suString::RemovePrefix(const suString &StartCandidate) const
{
    Assert(IsPrefix(StartCandidate), "Removing invalid prefix");
    suString Result = *this;
    for(UINT i = 0; i < StartCandidate._Length; i++)
    {
        Result.PopFront();
    }
    return Result;
}

bool suString::IsPrefix(const suString &StartCanidate) const
{
    if(_Length < StartCanidate._Length)
    {
        return false;
    }
    for(UINT i = 0; i < StartCanidate._Length; i++)
    {
        if(_Data[i] != StartCanidate._Data[i])
        {
            return false;
        }
    }
    return true;
}

UINT suString::Hash() const
{
    return Utility::Hash32((const BYTE *)_Data, _Length);
}

//
// Overloaded operators
//

suString operator + (const suString &L, const suString &R)
{
    suString Result;
    const UINT TotalLength = L._Length + R._Length;
    Result.Allocate(TotalLength + 1);
    Result._Length = TotalLength;
    Result._Data[TotalLength] = '\0';
    if(L._Length > 0)
    {
        memcpy(Result._Data, L._Data, L._Length);
    }
    if(R._Length > 0)
    {
        memcpy(Result._Data + L._Length, R._Data, R._Length);
    }
    return Result;
}

/*suString operator + (const char *L, const suString &R)
{
    suString Ret = L;
    for(UINT i = 0; i < R.Length(); i++)
    {
        Ret.PushEnd(R[i]);
    }
    return Ret;
}

suString operator + (const suString &L, const char *R)
{
    UINT RLen = UINT(strlen(R));
    suString Ret = L;
    for(UINT i = 0; i < RLen; i++)
    {
        Ret.PushEnd(R[i]);
    }
    return Ret;
}*/

ostream& operator << (ostream &os, const suString &S)
{
    if(S.Length() > 0)
    {
        os.write(S.CString(), S.Length());
    }
    return os;
}
