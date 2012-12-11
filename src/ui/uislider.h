#ifndef DRASH_UISLIDER_H
#define DRASH_UISLIDER_H

#include "uiwidget.h"

namespace drash
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

} // namespace drash

#endif // DRASH_UISLIDER_H
