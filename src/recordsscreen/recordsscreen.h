/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

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

#ifndef DRASH_RECORDSSCREEN_H
#define DRASH_RECORDSSCREEN_H

#include "../root/screen.h"

namespace drash {

class CRecordsScreen : public CScreen
{
public:
    explicit CRecordsScreen(CRoot & _parent);

    virtual ~CRecordsScreen();

    virtual void Step(double _dt) override;
    virtual void Render() override;
};

} // namespace drash

#endif // DRASH_RECORDSSCREEN_H
