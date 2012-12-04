#ifndef CEDITORAPP_H
#define CEDITORAPP_H

#include "../app/app.h"

namespace drash {


class CObjectEditorApp : public CApp
{
public:
    virtual ~CObjectEditorApp() override {}

    virtual bool Init() override;
    virtual void Step(double _dt) override;
    inline virtual void Render() override;

    void StartBuild();

    inline bool IsStartBuild()const;

    bool BuildFigure(std::string _objectName);

    void AddNewObjectToTemplate(std::string _name);

    void ShowObject(std::string _name);
private:

    std::vector<drash::CVec2> mVertexs;

    void SetProcessor();


    bool ValidateFigure();

    bool mBuildStart;

    CSceneObject *mCurrentObject = nullptr;

    void RemoveCurrentObject();
};

inline bool CObjectEditorApp::IsStartBuild()const {
    return mBuildStart;
}


}// namespace drash
#endif // CEDITORAPP_H
