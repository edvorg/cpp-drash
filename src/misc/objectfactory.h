// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#ifndef DRASH_OBJECTFACTORY_H
#define DRASH_OBJECTFACTORY_H

#include "../diag/logger.h"
#include <string>

namespace drash
{

template<class T>
class CObjectFactory final
{
public:
    class CFactoryProduct
    {
    public:
        friend class CObjectFactory;

    protected:
    private:
        long int mInternalId;
    };

    CObjectFactory() = delete;
    CObjectFactory(const CObjectFactory &) = delete;
    CObjectFactory(CObjectFactory &&) = delete;
    CObjectFactory &operator =(const CObjectFactory &) = delete;
    CObjectFactory &operator =(CObjectFactory &&) = delete;

    explicit CObjectFactory(unsigned int _size, const char *_product_name);
    ~CObjectFactory();

    T* CreateObject();
    bool DestroyObjectSafe(T *_obj);
    void DestroyObject(T *_obj);
    inline bool IsObject(T *_obj) const;
    inline T * const * GetObjects() const;
    inline unsigned int EnumObjects() const;

protected:
private:
    T **mObjects = nullptr;
    unsigned int mObjectsCount = 0;
    unsigned int mObjectsCountLimit = 0;
    std::string mProductName;
};

template<class T>
CObjectFactory<T>::CObjectFactory(unsigned int _size, const char *_product_name):
    mObjectsCountLimit(_size),
    mProductName(_product_name)
{
    if (mObjectsCountLimit == 0)
    {
        LOG_ERR("CObjectFactory<"<<
                mProductName.c_str()<<
                ">::CObjectFactory(): Empty factory is unusable");
        return;
    }

    mObjects = new T*[mObjectsCountLimit];
    for (unsigned int i = 0; i < mObjectsCount; i++)
    {
        mObjects[i] = nullptr;
    }
}

template<class T>
CObjectFactory<T>::~CObjectFactory()
{
    if (mObjectsCount != 0)
    {
        LOG_WARN("CObjectFactory<"<<
                 mProductName.c_str()<<
                 ">::~CObjectFactory(): Potential memory leak. You mush remove objects manually");

        for (unsigned int i = 0; i < mObjectsCount; i++)
        {
            DestroyObject(mObjects[i]);
            mObjects[i] = nullptr;
        }
    }

    delete [] mObjects;
    mObjects = nullptr;
}

template<class T>
T *CObjectFactory<T>::CreateObject()
{
    if (mObjectsCount >= mObjectsCountLimit)
    {
        LOG_ERR("CObjectFactory<"<<
                mProductName.c_str()<<
                ">::CreateObject(): Objects count exceedes it's limit ("<<mObjectsCountLimit<<')');
        return nullptr;
    }

    T *res = mObjects[mObjectsCount] = new T();
    res->mInternalId = mObjectsCount++;
    return res;
}

template<class T>
bool CObjectFactory<T>::DestroyObjectSafe(T *_obj)
{
    if (IsObject(_obj) == false)
    {
        LOG_ERR("CObjectFactory<"<<
                mProductName.c_str()<<
                ">::DestroyObject(): Something wrong with objects creation logic. Skipping");
        return false;
    }

    DestroyObject(_obj);

    return true;
}

template<class T>
void CObjectFactory<T>::DestroyObject(T *_obj)
{
    if (reinterpret_cast<CFactoryProduct*>(_obj)->mInternalId < --mObjectsCount)
    {
        mObjects[reinterpret_cast<CFactoryProduct*>(_obj)->mInternalId] = mObjects[mObjectsCount];
        reinterpret_cast<CFactoryProduct*>(mObjects[reinterpret_cast<CFactoryProduct*>(_obj)->mInternalId])->mInternalId = reinterpret_cast<CFactoryProduct*>(_obj)->mInternalId;
        mObjects[mObjectsCount] = nullptr;
    }
    else
    {
        mObjects[reinterpret_cast<CFactoryProduct*>(_obj)->mInternalId] = nullptr;
    }

    delete reinterpret_cast<CFactoryProduct*>(_obj);
}

template<class T>
inline bool CObjectFactory<T>::IsObject(T *_obj) const
{
    return _obj != nullptr && _obj->mInternalId < mObjectsCount && _obj == mObjects[_obj->mInternalId];
}

template<class T>
inline T * const * CObjectFactory<T>::GetObjects() const
{
    return mObjects;
}

template<class T>
inline unsigned int CObjectFactory<T>::EnumObjects() const
{
    return mObjectsCount;
}

} // namespace drash

#endif // DRASH_OBJECTFACTORY_H
