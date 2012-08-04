#include "cbullet.h"

namespace drash{

CBullet::CBullet():
    CSolidBody()
{

}


void CBullet::BeginContact(const drash::CContact &_contact)
{

}

CBulletParams::CBulletParams():
    CSolidBodyParams()
{
}

}// namespace drash
