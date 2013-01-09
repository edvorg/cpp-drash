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

#include "physobserver.h"

#include "sceneobject.h"
#include "figure.h"

namespace drash
{

CPhysObserver::CPhysObserver()
{
}

bool CPhysObserver::ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB)
{
    if (fixtureA->GetUserData() == nullptr ||
		fixtureB->GetUserData() == nullptr ||
		fixtureA->GetBody()->GetUserData() == nullptr ||
		fixtureB->GetBody()->GetUserData() == nullptr)
    {
        return false;
    }

    CSceneObject *o1 = reinterpret_cast<CSceneObject*>(fixtureA->GetBody()->GetUserData());
    CSceneObject *o2 = reinterpret_cast<CSceneObject*>(fixtureB->GetBody()->GetUserData());

    CFigure *f1 = reinterpret_cast<CFigure*>( fixtureA->GetUserData() );
    CFigure *f2 = reinterpret_cast<CFigure*>( fixtureB->GetUserData() );

    float z1 = f1->GetZ() + o1->GetPosZ();
    float z2 = f2->GetZ() + o2->GetPosZ();

    return drash::math::Abs(z1 - z2) < (f1->GetDepth() * 0.5 + f2->GetDepth() * 0.5);
}

void CPhysObserver::BeginContact(b2Contact * _contact)
{
    CFigure *f1 = reinterpret_cast<CFigure*>(_contact->GetFixtureA()->GetUserData());
    CFigure *f2 = reinterpret_cast<CFigure*>(_contact->GetFixtureB()->GetUserData());

    if ( f1 == nullptr || f2 == nullptr )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of figures. Skipping" );
        return;
    }

    if ( f1 == f2 || f1->GetSceneObject() == f2->GetSceneObject() )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "it's seems that pair of figures is part of same object" );
        return;
    }

    f1->GetSceneObject()->OnContactBegin(f1, f2);
    f2->GetSceneObject()->OnContactBegin(f2, f1);
}

void CPhysObserver::PreSolve(b2Contact *_contact, const b2Manifold *)
{
    CFigure *f1 = reinterpret_cast<CFigure*>(_contact->GetFixtureA()->GetUserData());
    CFigure *f2 = reinterpret_cast<CFigure*>(_contact->GetFixtureB()->GetUserData());

    if ( f1 == nullptr || f2 == nullptr )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of figures. Skipping" );
        return;
    }

    if ( f1 == f2 || f1->GetSceneObject() == f2->GetSceneObject() )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "it's seems that pair of figures is part of same object" );
        return;
    }

    f1->GetSceneObject()->OnContactPreSolve(f1, f2);
    f2->GetSceneObject()->OnContactPreSolve(f2, f1);
}

void CPhysObserver::EndContact(b2Contact *_contact)
{
    CFigure *f1 = reinterpret_cast<CFigure*>(_contact->GetFixtureA()->GetUserData());
    CFigure *f2 = reinterpret_cast<CFigure*>(_contact->GetFixtureB()->GetUserData());

    if ( f1 == nullptr || f2 == nullptr )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of figures. Skipping" );
        return;
    }

    if ( f1 == f2 || f1->GetSceneObject() == f2->GetSceneObject() )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "it's seems that pair of figures is part of same object" );
        return;
    }

    f1->GetSceneObject()->OnContactEnd(f1, f2);
    f2->GetSceneObject()->OnContactEnd(f2, f1);
}

} // namespace drash
