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

#pragma once
#ifndef TESTAPP3DRASHTEST_H
#define TESTAPP3DRASHTEST_H

#include "test1.h"

#include "../ui/uislider.h"
#include "../ui/uibutton.h"

namespace drash {

    namespace test {

        class Test3 : public Test1 {
        public:
            Test3(greng::GrengSystemsSet& greng);

        protected:
            virtual void Step(double _dt) override;
            virtual void Render() override;

        private:
            void SetProcessors();
            void InitObjects();
            SceneObject* moveObject = nullptr;

            // gravity in y axis
            ui::UISlider slider1;
            // gravity in x axis
            ui::UISlider slider2;

            greng::PointLight light1;
        };

    } // namespace test

} // namespace drash

#endif // TESTAPP3DRASHTEST_H
