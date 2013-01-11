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

#ifndef DRASH_TEST_TEST8_H
#define DRASH_TEST_TEST8_H

#include "../app/app.h"
#include "../ui/uibutton.h"

namespace drash
{

namespace test
{

class CTest8 : public CApp
{
public:
    CTest8() = default;

private:
    bool Init() override;
    bool InitUI();
    bool InitLevels();
    bool InitCamera();
    bool InitLights();

    ui::CUIButton mButton1;
    ui::CUIButton mButton2;
    CLevel *mLevel1 = nullptr;
    CLevel *mLevel2 = nullptr;
    greng::CPointLight mLight1;
};

} // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST8_H
