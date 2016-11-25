/*
Vector.h
Written by Matthew Fisher

Template vector library.  Extends functionality of std::vector
*/

#pragma once

template <class type> class Vector
{
public:
    Vector();
    explicit Vector(UINT _Size);
    Vector(const Vector<type> &V);

    ~Vector();
    

    //
    // Memory
    //
    void FreeMemory();

    //
    // Windows has claimed the nice name "ZeroMemory" for its own uses
    //
    void ZeroMem();

    Vector<type>& operator = (const Vector<type> &V);

    void ReSize(UINT Size);
    void Reserve(UINT Size);
    void Allocate(UINT Size);

    //
    // Accessors
    //
    __forceinline type& operator [] (UINT k)
    {
#ifdef VECTOR_DEBUG
        Assert(k < int(Data.size()), "Out-of-bounds vector access");
#endif
        return Data[k];
    }
    __forceinline type& operator [] (int k) 
    {
#ifdef VECTOR_DEBUG
        Assert(k >= 0 && k < int(Data.size()), "Out-of-bounds vector access");
#endif
        return Data[k];
    }
    __forceinline const type& operator [] (UINT k) const
    {
#ifdef VECTOR_DEBUG
        Assert(k < int(Data.size()), "Out-of-bounds vector access");
#endif
        return Data[k];
    }
    __forceinline const type& operator [] (int k) const
    {
#ifdef VECTOR_DEBUG
        Assert(k >= 0 && k < int(Data.size()), "Out-of-bounds vector access");
#endif
        return Data[k];
    }
    __forceinline UINT Length() const
    {
        return UINT(Data.size());
    }
    __forceinline type* CArray()
    {
#ifdef VECTOR_DEBUG
        Assert(Data.size() != 0, "CArray called on zero-length vector");
#endif
        return &(Data[0]);
    }
    __forceinline const type* CArray() const
    {
#ifdef VECTOR_DEBUG
        Assert(Data.size() != 0, "CArray called on zero-length vector");
#endif
        return &(Data[0]);
    }
    __forceinline type& RandomElement()
    {
        Assert(Length() > 0, "RandomElement called with no elements.");
        return (*this)[rand() % Length()];
    }
    __forceinline const type& RandomElement() const
    {
        Assert(Length() > 0, "RandomElement called with no elements.");
        return (*this)[rand() % Length()];
    }
    __forceinline type& Last()
    {
        Assert(Length() > 0, "Last called with no elements.");
        return (*this)[Length() - 1];
    }
    __forceinline const type& Last() const
    {
        Assert(Length() > 0, "Last called with no elements.");
        return (*this)[Length() - 1];
    }
    __forceinline type& First()
    {
        Assert(Length() > 0, "First called with no elements.");
        return (*this)[0];
    }
    __forceinline const type& First() const
    {
        Assert(Length() > 0, "Last called with no elements.");
        return (*this)[0];
    }

    //
    // Modifiers
    //
    void Append(const Vector<type> &V);
    void PushEnd(const type &t);
    void PushEnd();
    void PopEnd();
    void PopFront();
    void RemoveSlow(UINT Index);
    void RemoveSwap(UINT Index);
    void Randomize();
    void Sort();
    void Scale(const type &t);
    template<class orderingType> void Sort(orderingType Function)
    {
        std::sort(CArray(), CArray() + Length(), Function);
    }
    void Clear(const type &T);

    //
    // Query
    //
    type Sum() const;
    const type& MaxValue() const;
    UINT MaxIndex() const;
    const type& MinValue() const;
    UINT MinIndex() const;
    bool Contains(const type &t);
    UINT Hash32() const;
    UINT64 Hash64() const;

protected:
    vector<type> Data;
};

#include "VectorSTLImpl.cpp"
