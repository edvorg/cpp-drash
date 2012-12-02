#ifndef CEDITORAPP_H
#define CEDITORAPP_H

#include "../app/app.h"

namespace drash {

enum StateEditObject{
    EditObject,
    BuildingObject,
    Simple,
};

class CEditorApp : public CApp
{
public:
    virtual ~CEditorApp() override {}

    virtual bool Init() override;
    virtual void Step(double _dt) override;
    inline virtual void Render() override;

    StateEditObject objectState = Simple;
};

}// namespace drash
#endif // CEDITORAPP_H
