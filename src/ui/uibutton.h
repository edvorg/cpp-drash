#ifndef DRASH_UIBUTTON_H
#define DRASH_UIBUTTON_H

#include "uiwidget.h"

namespace drash
{

class CUIButton : public CUIWidget
{
public:
    CUIButton() = default;
    CUIButton(const CUIButton &) = delete;
    CUIButton(CUIButton &&) = delete;
    CUIButton &operator =(const CUIButton &) = delete;
    CUIButton &operator =(CUIButton &&) = delete;

    virtual void Connect(CUISystem *_system) override;

    virtual void SetClickHandler(const std::function<void ()> &_handler) final;

protected:
private:
    std::function<void ()> mClickHandler = [] () {};
    bool mPressed = false;
    float mTime = 0;
};

} // namespace drash

#endif // DRASH_UIBUTTON_H
