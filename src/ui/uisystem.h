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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#ifndef DRASH_UISYSTEM_H
#define DRASH_UISYSTEM_H

#include "../misc/vec2.h"
#include "../appeventsystem/eventsystem.h"

namespace greng
{

class CRenderer;

}

namespace drash
{

namespace ui
{

class CUIControl;

class CUISystem final : public CTouchListener
{
public:
    constexpr static const unsigned int mControlsCountLimit = 10;

    CUISystem() = default;
    CUISystem(const CUISystem &) = delete;
    CUISystem(CUISystem &&) = delete;
    CUISystem &operator =(const CUISystem &) = delete;
    CUISystem &operator =(CUISystem &&) = delete;
    virtual ~CUISystem();

    void Release();

    CUIControl *CreateControl();
    void DestroyControl(CUIControl *_control);

    void SetAspectRatio(float _ratio);
    void SetWidth(unsigned int _width);
    void SetHeight(unsigned int _height);

    bool ScreenSpaceToUISpace(const CVec2f &_from, int &_x, int &_y);
    bool UISpaceToScreenSpace(int _x, int _y, CVec2f &_v);

    void SetCursorPos(int _x, int _y);
    inline int GetCursorPosX() const;
    inline int GetCursorPosY() const;

    void BeginEvent();
    void EndEvent();

    void SetRenderer(greng::CRenderer *_renderer);
    inline greng::CRenderer *GetRenderer() const;

    void Step(double _dt);
    void DebugDraw() const;
    void Render();

    // implemets Listener's interface
    virtual void TouchPress(const CTouchEvent & _evt) override;
    virtual void TouchPressing(const CTouchEvent & _evt) override;
    virtual void TouchRelease(const CTouchEvent & _evt) override;

protected:
private:
    CUIControl *mControls[mControlsCountLimit];
    unsigned int mControlsCount = 0;

    float mAspectRatio = 1;
    unsigned int mWidth = 1024;
    unsigned int mHeight = 768;

    int mCursorX = 0;
    int mCursorY = 0;

    CUIControl *mPressedControl = nullptr;

    greng::CRenderer *mRenderer = nullptr;
};

inline int CUISystem::GetCursorPosX() const
{
    return mCursorX;
}

inline int CUISystem::GetCursorPosY() const
{
    return mCursorY;
}

inline void CUISystem::SetRenderer(greng::CRenderer *_renderer)
{
    mRenderer = _renderer;
}

inline greng::CRenderer *CUISystem::GetRenderer() const
{
    return mRenderer;
}

} // namepsace ui

} // namespace drash

#endif // DRASH_UISYSTEM_H
