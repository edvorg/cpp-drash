#ifndef CBULLET_H
#define CBULLET_H
#include "csolidbody.h"
#include "../cscene.h"
namespace drash{

class CBulletParams:public CSolidBodyParams{
public:
    CBulletParams();
};

class CBullet : public CSolidBody
{
public:
    friend class CScene;

    CBullet();

    virtual void BeginContact(const CContact &_contact);
};

}// namespace drash
#endif // CBULLET_H
