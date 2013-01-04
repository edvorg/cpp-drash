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

#ifndef DRASH_TEST_TEST5_H
#define DRASH_TEST_TEST5_H

#include "test3.h"

namespace drash
{

namespace test
{

class CTest5 : public CTest3
{
public:
    CTest5() = default;
    virtual bool Init() override;
    virtual void Render() override;

protected:
private:
    void SetupCam();
    void SetupMeshes();
    void SetupTextures();
    void SetupProcessors();

    greng::CMesh *mMesh1;
    greng::CMesh *mMesh2;
    greng::CMesh *mMesh3;
    CMatrix4f mMesh3ConstMatrix;
    greng::CTexture *mTex1;
    greng::CTexture *mTex2;
    greng::CTexture *mTex3;
    greng::CTexture *mTex4;
    greng::CTexture *mTex5;
};

} // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST5_H
