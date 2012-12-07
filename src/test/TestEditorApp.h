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
#ifndef TESTEDITORAPP_H
#define TESTEDITORAPP_H

#include "../app/app.h"

namespace drash
{

class CTestEditorApp : public CApp
{
public:
    virtual ~CTestEditorApp() override {}

    virtual bool Init() override;
    virtual void Render() override;

private:
    void SetProcessors();
    void CreateFigure();
    void CompleteFigure();
    void CreateTemplate();
    void DetachCurrentObject();

    double mTime = 0;

    std::vector<CVec2> mVertices;
    unsigned int mTemplateCounter = 0;
    CSceneObjectGeometry *mCurrentTemplate = nullptr;
    CSceneObject *mCurrentObject = nullptr;
};

} // namespace drash

#endif // TESTEDITORAPP_H
