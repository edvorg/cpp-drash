#ifndef DRASH_UIWIDGET_H
#define DRASH_UIWIDGET_H

#include <functional>

namespace drash
{

class CUIControl;
class CUISystem;

class CUIWidget
{
public:
    CUIWidget() = default;
    CUIWidget(const CUIWidget &) = delete;
    CUIWidget(CUIWidget &&) = delete;
    CUIWidget &operator =(const CUIWidget &) = delete;
    CUIWidget &operator =(CUIWidget &&) = delete;
    virtual ~CUIWidget();

    virtual void Connect(CUISystem *_system);
    virtual void Disconnect() final;

    virtual CUISystem *GetUISystem() const final;

    virtual void SetPressHandler(const std::function<void ()> &_handler) final;
    virtual void SetReleaseHandler(const std::function<void ()> &_handler) final;
    virtual void SetDrawHandler(const std::function<void ()> &_handler) final;

protected:
private:
    CUIControl *mUIControl = nullptr;
    CUISystem *mUISystem = nullptr;
};

} // namespace drash

#endif // DRASH_UIWIDGET_H
