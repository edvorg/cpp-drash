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

#ifndef GRENG_CAMERAMANAGER_H
#define GRENG_CAMERAMANAGER_H

#include "../misc/vec3.h"
#include "../misc/objectfactory.h"

namespace greng {

    using drash::Vec3f;
    class Camera;

    class CameraParams {
    public:
        bool ortho = false;
        float orthoWidth = 1.0f;
        float fov = M_PI / 4.0f;
        float depthOfView = 1000.0f;
        Vec3f pos = { 0, 0, 0 };
        Vec3f rotation = { 0, 0, 0 };
    };

    class CameraManager {
    public:
        static const unsigned int camerasCountLimit = 3;

        CameraManager();
        ~CameraManager();

        void Step(double _dt);

        Camera* CreateCamera(const CameraParams& _params);
        bool DestroyCamera(Camera* _camera);
        inline Camera* const* GetCameras() const;
        inline unsigned int EnumCameras() const;

        /// aspect ratio is window width in pixels divided on widow height
        void SetAspectRatio(float _ratio);

        /// aspect ratio is window width in pixels divided on widow height
        inline float GetAspectRatio() const;

    protected:
    private:
        drash::ObjectFactory<Camera> cameraFactory;

        float aspectRatio = 1;
    };

    inline Camera* const* CameraManager::GetCameras() const {
        return cameraFactory.GetObjects();
    }

    inline unsigned int CameraManager::EnumCameras() const {
        return cameraFactory.EnumObjects();
    }

    inline float CameraManager::GetAspectRatio() const { return aspectRatio; }

} // namespace greng

#endif // GRENG_CAMERAMANAGER_H
