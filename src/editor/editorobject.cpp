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

#include "editorobject.h"

using namespace drash;

EditorObject::EditorObject(QWidget *_parent):
    SceneWidget(_parent),
    mStartBuild(false)
{
    mMode = Edit;
//    connect(this,SIGNAL(CreateNewObject()),
//            this,SLOT(RemoveObjects()));


}

EditorObject::~EditorObject()
{
}

void EditorObject::mousePressEvent(QMouseEvent *_event){
    //QWidget::mousePressEvent(_event);
    if (mStartBuild == true) {
        auto cam = mApp->GetDebugDrawSystem().GetActiveCam();
        CVec2 pos = this->WidgetSpaceToWorldSpace(CVec2(_event->x(),
                                                        _event->y()),
                                                        -cam->GetZ().Get());

        mVertexs.push_back(pos);
        LOG_INFO(pos);
    }
}

void EditorObject::StartBuildObject(){
    mMode = Create;
    mStartBuild = true;
    mVertexs.clear();
    emit this->CreateNewObject();
}

bool EditorObject::EndBuild(){
    if (mStartBuild == false ||
            mVertexs.empty()) {
        return false;
    }
    mStartBuild = false;

    mVertexs.clear();
    return true;
}

const std::vector<CVec2> &EditorObject::GetVector() const
{

    return mVertexs;

}

void EditorObject::ChangeMode()
{
    mMode = (mMode == Create)? Simple: Create;
}
