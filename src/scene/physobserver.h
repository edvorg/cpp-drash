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

#ifndef PHYSOBSERVER_H
#define PHYSOBSERVER_H

#include <Box2D/Box2D.h>

namespace drash
{

class CPhysObserver : public b2ContactListener, public b2ContactFilter
{
public:
    CPhysObserver();

    virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;
    virtual void BeginContact(b2Contact * _figure) override;
    virtual void PreSolve(b2Contact* _contact, const b2Manifold *) override;
    virtual void EndContact(b2Contact * _figure) override;
};

}

#endif // PHYSOBSERVER_H
