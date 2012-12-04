#ifndef DRASH_APPEVENTSYSTEM_H
#define DRASH_APPEVENTSYSTEM_H

#include "appevent.h"
#include "appeventprocessor.h"
#include "appeventcombination.h"
#include <list>

namespace drash
{

class CAppEventCombinationTree
{
public:
    friend class CAppEventSystem;

    CAppEventCombinationTree();

protected:
private:
    CAppEventProcessor mProcessor;
    CAppEventCombination mCombination;
    std::list<CAppEventCombinationTree> mChilds;
    bool mOperationLock = false;
};

class CAppEventSystem
{
public:
    CAppEventSystem();

    void SetProcessor(const char *_combinations, const CAppEventProcessor &_processor);

    void Process();

    void PressEvent(const CAppEvent &_event);
    void ReleaseEvent(const CAppEvent &_event);

protected:
private:
    CAppEventCombination mCurrentState;
    CAppEventCombinationTree mTree;
    std::list<CAppEventCombinationTree*> mCurrentCombinations;
    CAppEventCombinationTree *mCurrentRoot = &mTree;
};

} // namespace drash

#endif // DRASH_APPEVENTSYSTEM_H
