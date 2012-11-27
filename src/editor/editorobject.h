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

#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <vector>


#include "../scenewidget.h"
#include "../misc/cvec2.h"

// TODO: temp Testing build object
class EditorObject : public SceneWidget
{
    Q_OBJECT

public:
    explicit EditorObject(QWidget * _parent = NULL);

    virtual ~EditorObject();

    virtual void mousePressEvent( QMouseEvent * _event ) override;

    void StartBuildObject();

    bool EndBuild();

    const std::vector<drash::CVec2> &GetVector()const;

    void ChangeMode();
signals:
    void CreateNewObject();

private:
    enum WorkMode{Edit, Create, Simple};
    bool mStartBuild;
    WorkMode mMode;
//    bool mEditObject;
    std::vector<drash::CVec2> mVertexs;
//private slots:
//    void ClearScene();
};

#endif // EDITORWIDGET_H
