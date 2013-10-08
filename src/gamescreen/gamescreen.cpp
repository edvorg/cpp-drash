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

#include "gamescreen.h"

namespace drash {

CGameScreen::CGameScreen(CRoot &_parent):
    CScreen(_parent)
{
    InitScene();
}

CGameScreen::~CGameScreen()
{
    mScene.Release();
}

void CGameScreen::Step(double _dt)
{
    CScreen::Step(_dt);
    mScene.Step(_dt);
}

void CGameScreen::Render()
{
    CScreen::Render();
}

bool CGameScreen::InitScene()
{
    CSceneParams params;
    params.mGravity = -9.8;

    return mScene.Init(params);
}


} // namespace drash
