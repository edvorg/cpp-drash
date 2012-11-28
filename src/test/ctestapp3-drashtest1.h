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
#ifndef CTESTAPP3DRASHTEST_H
#define CTESTAPP3DRASHTEST_H

#include "app.h"

namespace drash
{

class CTestApp3 : public CApp
{
public:
    virtual ~CTestApp3() override {}

    virtual bool Init() override;
    virtual void Step(double _dt) override;
    inline virtual void Render() override;

private:
    double mTime = 0;
    CSceneObject *mO1 = nullptr;
    CSceneObject *mO2 = nullptr;
};

} // namespace drash

#endif // CTESTAPP3DRASHTEST_H
