#ifndef CEDITORAPP_H
#define CEDITORAPP_H

#include "../app/app.h"

namespace drash {


enum State {
    BuildState,
    MoveState,
    Simple
};

class CObjectEditorApp : public CApp
{
public:
    virtual ~CObjectEditorApp() override {}

    virtual bool Init() override;
    virtual void Step(double _dt) override;
    inline virtual void Render() override;

    void StartBuild();

    inline bool IsStartBuild()const;

    bool BuildFigure(const std::string &_objectName);

    void AddNewObjectToTemplate(const std::string &_name);

    void ShowObject(const std::string &_name);

    inline void SetCurrentTemplateName(const std::string & _name);

    inline void SetTreeRefreshHandler(const std::function<void ()> &_han);
private:

    std::vector<drash::CVec2> mVertexs;

    void SetProcessor();

    bool ValidateFigure();

    CSceneObject *mCurrentObject = nullptr;

    State mState = Simple;

    void RemoveCurrentObject();

    std::string mCurrentTemplateName = "";

    std::function<void ()> mTreeRefreshHandler = [] () {};
};

inline bool CObjectEditorApp::IsStartBuild()const {
    return mState == BuildState;
}

inline void CObjectEditorApp::SetCurrentTemplateName(const std::string &_name){
    mCurrentTemplateName = _name;
}

inline void CObjectEditorApp::SetTreeRefreshHandler(const std::function<void ()> &_han) {
    mTreeRefreshHandler = _han;
}

}// namespace drash
#endif // CEDITORAPP_H
