#ifndef DRASH_APPEVENTSYSTEM_H
#define DRASH_APPEVENTSYSTEM_H

#include "appevent.h"
#include "appeventprocessor.h"
#include "appeventcombination.h"
#include <string>
#include <list>
#include <map>

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

    static bool ValidateModeName(const std::string &_name);
    bool SetMode(const std::string &_name);
    void SetProcessor(const char *_combinations, const CAppEventProcessor &_processor);

    void Process();

    void PressEvent(const CAppEvent &_event);
    void ReleaseEvent(const CAppEvent &_event);

protected:
private:
    /// contains all current events
    /// PressEvent invokation adds event to mCurrentState
    /// ReleaseEvent invokation removes event from mCurrentState
    CAppEventCombination mCurrentState;

    /// contains combinations being processed
    std::list<CAppEventCombinationTree*> mCurrentCombinations;


    /// name of mode currently in use. mode is just name for one combinations tree.
    /// the reason for using of modes is processor conflicts when we need to bind
    /// more than one CAppEventProcessor instances to the same combination
    /// but execute only one instance at time
    std::string mCurrentMode = "";

    /// combination trees for each mode
    std::map<std::string, CAppEventCombinationTree> mTrees;

    /// root of current mode tree
    CAppEventCombinationTree *mCurrentModeRoot = nullptr;

    /// start point for searching of combinations to process
    CAppEventCombinationTree *mCurrentNode = nullptr;

    bool mModeChanged = false;
};

} // namespace drash

#endif // DRASH_APPEVENTSYSTEM_H
