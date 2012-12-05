// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

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
