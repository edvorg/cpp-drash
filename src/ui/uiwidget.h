#ifndef DRASH_UIWIDGET_H
#define DRASH_UIWIDGET_H

#include <functional>

namespace drash
{

class CUIControl;

class CUIWidget
{
public:
    friend class CUISystem;

    CUIWidget() = default;
    CUIWidget(const CUIWidget &) = delete;
    CUIWidget(CUIWidget &&) = delete;
    CUIWidget &operator =(const CUIWidget &) = delete;
    CUIWidget &operator =(CUIWidget &&) = delete;

    void SetPressHandler(const std::function<void ()> &_handler);
    void SetDrawHandler(const std::function<void ()> &_handler);

protected:
private:
    void SetControlHandlers();

    CUIControl *mControl = nullptr;
};

} // namespace drash

#endif // DRASH_UIWIDGET_H
