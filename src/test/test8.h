// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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
#include "../greng/pointlight.h"

namespace drash {

    namespace test {

        class Test8 : public App {
        public:
            Test8(greng::Greng& greng);
            ~Test8();

        private:
            virtual void Render() override;

            bool InitUI();
            bool InitLevels();
            bool InitCamera();
            bool InitLights();

            ui::UIButton button1;
            ui::UIButton button2;
            LevelDesc* level1 = nullptr;
            LevelDesc* level2 = nullptr;
            greng::PointLight light1;
        };

    } // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST8_H
