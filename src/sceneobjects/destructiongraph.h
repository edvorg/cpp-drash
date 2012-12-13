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
