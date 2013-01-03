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

#ifndef DRASH_DESTRUCTIONGRAPH_H
#define DRASH_DESTRUCTIONGRAPH_H

#include <vector>

namespace drash
{

class CDestructionGraphNode
{
public:
    constexpr static const unsigned int mConnectionsCountLimit = 3;

    CDestructionGraphNode() = default;
    CDestructionGraphNode(const CDestructionGraphNode &) = delete;
    CDestructionGraphNode(CDestructionGraphNode &&) = delete;
    CDestructionGraphNode &operator =(const CDestructionGraphNode &) = delete;
    CDestructionGraphNode &operator =(CDestructionGraphNode &&) = delete;

    CDestructionGraphNode(unsigned int _value);

protected:
private:
    int mValue = -1;

    CDestructionGraphNode *mConnections[mConnectionsCountLimit];
    unsigned int mConnectionsCount = 0;
};

class CDestructionGraph final
{
public:
    CDestructionGraph() = default;
    CDestructionGraph(const CDestructionGraph &) = delete;
    CDestructionGraph(CDestructionGraph &&) = delete;
    CDestructionGraph &operator =(const CDestructionGraph &) = delete;
    CDestructionGraph &operator =(CDestructionGraph &&) = delete;

protected:
private:
};

} // namespace drash

#endif // DRASH_DESTRUCTIONGRAPH_H
