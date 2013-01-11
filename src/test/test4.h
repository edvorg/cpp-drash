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

#ifndef DRASH_TEST_TEST4_H
#define DRASH_TEST_TEST4_H

#include "../app/app.h"
#include "../ui/uibutton.h"
#include "../ui/uislider.h"
#include "../misc/animator.h"
#include "../misc/moveablepoint.h"
#include "../greng/camera.h"

namespace drash
{

namespace test
{

class CTest4 : public CApp
{
public:
    CTest4() = default;

protected:
    virtual bool Init() override;
    virtual void Step(double _dt) override;
    virtual void Render() override;

private:
    ui::CUIButton mTestButton1;
    ui::CUIButton mTestButton2;
    ui::CUISlider mTestSlider1;

    float mValue = 77;
    CAnimator<float> mValueAnimator1 = mValue;
    CAnimator<float> mValueAnimator2 = mValue;

    CVec3f mTestPoint;
    CMoveablePoint mPoint;
};

} // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST4_H
