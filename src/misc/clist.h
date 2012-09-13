#pragma once
#ifndef CLIST_H
#define CLIST_H

namespace drash{

template <class T>
class CList
{
public:
    CList();
    T mData;
    CList* mNext;
};

}// namespace drash
#endif // CLIST_H
