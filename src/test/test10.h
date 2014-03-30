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

#ifndef DRASH_TEST_TEST10_H
#define DRASH_TEST_TEST10_H

#include "../app/app.h"
#include "../greng/pointlight.h"
#include "../greng/spotlight.h"

namespace drash {

    namespace test {

        class CTest10 : public CApp {
        public:
            CTest10(greng::CGrengSystemsSet& greng);

        private:
            virtual void Step(double _dt) override;
            virtual void Render() override;
            bool InitCamera();
            bool InitObjects();
            bool InitLights();
            bool InitProcessors();

            greng::CCamera* camera = nullptr;
            greng::CPointLight lightPoint1;
            greng::CSpotLight spotLight;
            CSceneObject* lamp1Object = nullptr;
            CSceneObject* wallTopObject = nullptr;
        };

    } // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST10_H
