#ifndef DRASH_UIWIDGET_H
#define DRASH_UIWIDGET_H

namespace drash
{

class CUIWidget
{
public:
    CUIWidget() = default;
    CUIWidget(const CUIWidget &) = delete;
    CUIWidget(CUIWidget &&) = delete;
    CUIWidget &operator =(const CUIWidget &) = delete;
    CUIWidget &operator =(CUIWidget &&) = delete;
};

} // namespace drash

#endif // DRASH_UIWIDGET_H
