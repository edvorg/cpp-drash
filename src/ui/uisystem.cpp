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

#include "uisystem.h"

#include "uicontrol.h"

namespace drash {

    namespace ui {

        CUISystem::CUISystem(greng::CRenderer& _renderer)
            : mRenderer(_renderer) {}

        CUISystem::~CUISystem() {
            for (unsigned int i = 0; i < mControlsCount; i++) {
                delete mControls[i];
                mControls[i] = nullptr;
            }
            mControlsCount = 0;
        }

        CUIControl* CUISystem::CreateControl() {
            if (mControlsCount == mControlsCountLimit) {
                return nullptr;
            }

            return mControls[mControlsCount++] = new CUIControl;
        }

        void CUISystem::DestroyControl(CUIControl* _control) {
            for (unsigned int i = 0; i < mControlsCount; i++) {
                if (mControls[i] == _control) {
                    _control->mDestroyHandler();
                    delete _control;
                    mControls[i] = nullptr;
                    mControlsCount--;
                    return;
                }
            }
        }

        void CUISystem::SetAspectRatio(float _ratio) {
            mAspectRatio = _ratio;
            SetWidth(mWidth);
        }

        void CUISystem::SetWidth(unsigned int _width) {
            mWidth = _width;
            mHeight = mWidth / mAspectRatio;
        }

        bool CUISystem::ScreenSpaceToUISpace(const CVec2f& _from, int& _x,
                                             int& _y) {
            _x = (_from.mX + 0.5f) * static_cast<float>(mWidth);
            _y = (_from.mY + 0.5f) * static_cast<float>(mHeight);
            return true;
        }

        bool CUISystem::UISpaceToScreenSpace(int _x, int _y, CVec2f& _v) {
            _v.mX =
                (static_cast<float>(_x) / static_cast<float>(mWidth)) - 0.5f;
            _v.mY =
                (static_cast<float>(_y) / static_cast<float>(mHeight)) - 0.5f;
            return true;
        }

        void CUISystem::SetCursorPos(int _x, int _y) {
            mCursorX = _x;
            mCursorY = _y;
        }

        void CUISystem::BeginEvent() {
            for (unsigned int i = 0; i < mControlsCount; i++) {
                int local_x = GetCursorPosX() - mControls[i]->GetPos().mX;
                int local_y = GetCursorPosY() - mControls[i]->GetPos().mY;

                if (0 <= local_x &&
                    local_x <= static_cast<int>(mControls[i]->GetSize().mX) &&
                    0 <= local_y &&
                    local_y <= static_cast<int>(mControls[i]->GetSize().mY)) {
                    mPressedControl = mControls[i];
                    mPressedControl->mPressHandler();
                    break;
                }
            }
        }

        void CUISystem::EndEvent() {
            if (mPressedControl != nullptr) {
                mPressedControl->mReleaseHandler();
                mPressedControl = nullptr;
            }
        }

        void CUISystem::Step(double _dt) {
            for (unsigned int i = 0; i < mControlsCount; i++) {
                mControls[i]->mStepHandler(_dt);
            }
        }

        void CUISystem::DebugDraw() const {
            for (unsigned int i = 0; i < mControlsCount; i++) {
                mControls[i]->mDrawHandler();
            }
        }

    } // namepsace ui

} // namespace drash
