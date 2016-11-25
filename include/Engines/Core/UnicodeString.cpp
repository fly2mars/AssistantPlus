/*
suUnicodeString.cpp
Written by Matthew Fisher

Implementation of suUnicodeString class.
*/

void suUnicodeString::ResizeToCStringLength()
{
    for(UINT i = 0; i < _Capacity; i++)
    {
        if(_Data[i] == UnicodeNullTerminator)
        {
            _Length = i;
            ReSize(_Length + 1);
            return;
        }
    }
    SignalError("No null terminator found in suUnicodeString::ResizeToCStringLength");
}

suUnicodeString& suUnicodeString::operator = (UnicodeCharacter Character)
{
    Allocate(2);
    _Length = 1;
    _Data[0] = Character;
    _Data[1] = UnicodeNullTerminator;
    return *this;
}

suUnicodeString& suUnicodeString::operator = (const UnicodeCharacter *S)
{
    UINT NewLength = UINT(lstrlenW(S));
    Allocate(NewLength + 1);
    memcpy(_Data, S, (NewLength + 1) * sizeof(UnicodeCharacter));
    _Length = NewLength;
    return *this;
}

suUnicodeString& suUnicodeString::operator = (const suUnicodeString &S)
{
    if(S._Data == NULL)
    {
        FreeMemory();
    }
    else if(&S != this)
    {
        Allocate(S._Length + 1);
        _Length = S._Length;
        memcpy(_Data, S._Data, _Capacity * sizeof(UnicodeCharacter));
    }
    return *this;
}

suUnicodeString& suUnicodeString::operator += (const suUnicodeString &S)
{
    ReSize(_Length + S._Length + 1);
    memcpy(_Data + _Length, S._Data, (S._Length + 1) * sizeof(UnicodeCharacter));
    return *this;
}

void suUnicodeString::Partition(UnicodeCharacter Seperator, suVector<suUnicodeString> &Output) const
{
    Output.FreeMemory();
    suUnicodeString CurEntry;
    const UINT Length = _Length;
    for(UINT Index = 0; Index < Length; Index++)
    {
        if(_Data[Index] == Seperator)
        {
            if(CurEntry._Length > 0)
            {
                Output.PushEnd(CurEntry);
                CurEntry._Length = 0;
                CurEntry._Data[0] = UnicodeNullTerminator;
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

suVector<suUnicodeString> suUnicodeString::Partition(UnicodeCharacter Seperator) const
{
    suVector<suUnicodeString> Result;
    Partition(Seperator, Result);
    return Result;
}

int suUnicodeString::FindFirstIndex(UnicodeCharacter Seperator) const
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

int suUnicodeString::FindLastIndex(UnicodeCharacter Seperator) const
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

void suUnicodeString::PartitionAboutIndex(UINT Index, suUnicodeString &Left, suUnicodeString &Right) const
{
    UINT L = Length();
    Assert(Index < L, "Index invalid in suUnicodeString::PartitionAboutIndex");
    Left.ReSize(Index);
    Right.ReSize(L - Index - 1);
    for(UINT LeftIndex = 0; LeftIndex < Index; LeftIndex++)
    {
        Left[LeftIndex] = _Data[LeftIndex];
    }
    for(UINT RightIndex = 0; RightIndex < Right.Length(); RightIndex++)
    {
        Right[RightIndex] = _Data[Index + 1 + RightIndex];
    }
}

void suUnicodeString::Partition(const suUnicodeString &Seperator, suVector<suUnicodeString> &Output) const
{
    Assert(Seperator.Length() >= 1, "empty seperator");
    Output.FreeMemory();
    suUnicodeString CurEntry;
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
                CurEntry._Data[0] = UnicodeNullTerminator;
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

bool suUnicodeString::ExactMatchAtOffset(const suUnicodeString &Find, UINT Offset) const
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

bool suUnicodeString::Contains(const suUnicodeString &Find) const
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

suUnicodeString suUnicodeString::FindAndReplace(const suUnicodeString &Find, const suUnicodeString &Replace) const
{
    suUnicodeString Result;
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

bool suUnicodeString::IsSuffix(const suUnicodeString &EndCanidate) const
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

suUnicodeString suUnicodeString::RemoveSuffix(const suUnicodeString &EndCandidate) const
{
    Assert(IsSuffix(EndCandidate), "Removing invalid suffix");
    suUnicodeString Result = *this;
    for(UINT i = 0; i < EndCandidate._Length; i++)
    {
        Result.PopEnd();
    }
    return Result;
}

bool suUnicodeString::IsPrefix(const suUnicodeString &StartCanidate) const
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

UINT suUnicodeString::Hash() const
{
    return Utility::Hash32((const BYTE *)_Data, _Length * sizeof(UnicodeCharacter));
}

//
// Overloaded operators
//

suUnicodeString operator + (const suUnicodeString &L, const suUnicodeString &R)
{
    suUnicodeString Result;
    const UINT TotalLength = L._Length + R._Length;
    Result.Allocate(TotalLength + 1);
    Result._Length = TotalLength;
    Result._Data[TotalLength] = UnicodeNullTerminator;
    if(L._Length > 0)
    {
        memcpy(Result._Data, L._Data, L._Length * sizeof(UnicodeCharacter));
    }
    if(R._Length > 0)
    {
        memcpy(Result._Data + L._Length, R._Data, R._Length * sizeof(UnicodeCharacter));
    }
    return Result;
}

ostream& operator << (ostream &os, const suUnicodeString &S)
{
    /*if(S.Length() > 0)
    {
        os.write(S.CString(), S.Length());
    }*/
    SignalError("Not implemented");
    return os;
}

bool operator == (const suUnicodeString &L, const suUnicodeString &R)
{
    UINT LLen = L.Length();
    UINT RLen = R.Length();
    if(LLen != RLen)
    {
        return false;
    }
    for(UINT i = 0; i < LLen; i++)
    {
        if(L[i] != R[i])
        {
            return false;
        }
    }
    return true;
}

bool operator == (const UnicodeCharacter *L, const suUnicodeString &R)
{
    UINT LLen = UINT(lstrlenW(L));
    UINT RLen = R.Length();
    if(LLen != RLen)
    {
        return false;
    }
    for(UINT i = 0; i < LLen; i++)
    {
        if(L[i] != R[i])
        {
            return false;
        }
    }
    return true;
}

bool operator == (const suUnicodeString &R, const UnicodeCharacter *L)
{
    return (L == R);
}
