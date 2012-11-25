// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#pragma once
#ifndef CBULLET_H
#define CBULLET_H

#include "csceneobject.h"

namespace drash
{

class CBulletParams : public CSceneObjectParams
{
public:
    CBulletParams();

    CVec2 mTarget;
};

class CBullet : public CSceneObject
{
public:
    friend class CScene;

    typedef CBulletParams ParamsT;

protected:
    CBullet();

    bool Init( const ParamsT &_params );

    virtual void OnContactBegin( const CContact &_contact );
    virtual void OnBoom( const CBoomParams &_boom );
};

}// namespace drash
#endif // CBULLET_H
