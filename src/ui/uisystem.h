#ifndef DRASH_UISYSTEM_H
#define DRASH_UISYSTEM_H

namespace drash
{

class CUIControl;
class CUIWidget;

class CUISystem final
{
public:
    constexpr static const unsigned int mControlsCountLimit = 10;

    CUISystem() = default;
    CUISystem(const CUISystem &) = delete;
    CUISystem(CUISystem &&) = delete;
    CUISystem &operator =(const CUISystem &) = delete;
    CUISystem &operator =(CUISystem &&) = delete;

    bool Init();
    void Release();

    bool ConnectWidget(CUIWidget &_widget);
    void DisconnectWidget(CUIWidget &_widget);

protected:
private:
    CUIControl *CreateControl();
    void DestroyControl(CUIControl *_control);

    CUIControl *mControls[mControlsCountLimit];
    unsigned int mControlsCount = 0;
};

} // namespace drash

#endif // DRASH_UISYSTEM_H
