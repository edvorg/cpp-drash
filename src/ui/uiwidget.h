#ifndef DRASH_UIWIDGET_H
#define DRASH_UIWIDGET_H

#include <functional>
#include "../misc/vec2.h"

namespace drash
{

namespace ui
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
    virtual void Disconnect();

    CUISystem *GetUISystem() const;

    void SetPressHandler(const std::function<void ()> &_handler);
    void SetReleaseHandler(const std::function<void ()> &_handler);
    void SetStepHandler(const std::function<void (double _dt)> &_handler);
    void SetDrawHandler(const std::function<void ()> &_handler);

    void SetPos(const CVec2i &_pos);
    void SetSize(const CVec2ui &_size);
    const CVec2i GetPos() const;
    const CVec2ui GetSize() const;

protected:
private:
    CUIControl *mUIControl = nullptr;
    CUISystem *mUISystem = nullptr;
};

} // namepsace ui

} // namespace drash

#endif // DRASH_UIWIDGET_H
