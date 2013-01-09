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

#ifndef DRASH_UISLIDER_H
#define DRASH_UISLIDER_H

#include "uiwidget.h"

namespace drash
{

namespace ui
{

class CUISlider : public CUIWidget
{
public:
    CUISlider() = default;
    CUISlider(const CUISlider &) = delete;
    CUISlider(CUISlider &&) = delete;
    CUISlider &operator =(const CUISlider &) = delete;
    CUISlider &operator =(CUISlider &&) = delete;

    virtual void Connect(CUISystem *_system) override;

    void SetValueHandler(const std::function<void (float _value)> &_handler);

    void SetValue(float _value);
    float GetValue() const;
    void SetPercent(float _percent);
    float GetPercent() const;
    void SetSliderWidth(unsigned int _width);
    unsigned int GetSliderWidth() const;
    void SetMin(float _min);
    float GetMin() const;
    void SetMax(float _max);
    float GetMax() const;

protected:
private:
    bool mPressed = false;
    bool mCatched = false;
    float mMin = 0;
    float mMax = 100;
    float mValue = 0;
    unsigned int mSliderWidth = 20;
    std::function<void (float _value)> mValueHandler = [] (float) {};
};

} // namepsace ui

} // namespace drash

#endif // DRASH_UISLIDER_H
