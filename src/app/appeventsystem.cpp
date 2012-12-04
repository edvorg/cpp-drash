#include "appeventsystem.h"

namespace drash
{

CAppEventCombinationTree::CAppEventCombinationTree()
{
}

CAppEventSystem::CAppEventSystem()
{
}

void CAppEventSystem::SetProcessor(const char *_combinations, const CAppEventProcessor &_processor)
{
    using std::list;
    using std::string;
    using std::for_each;

    char str[128];
    strncpy(str, _combinations, 127);
    str[127] = 0;

    list<string> simple_combinations;
    list<CAppEventCombination> combinations;

    for (char *c = strtok(str, " "); c != nullptr; c = strtok(nullptr, " "))
    {
        simple_combinations.push_back(string(c));
    }

    std::for_each(simple_combinations.begin(), simple_combinations.end(), [&combinations] (string &_str)
    {
        char str2[128];
        strncpy(str2, _str.c_str(), 127);
        str2[127] = 0;

        CAppEventCombination comb;
        for (char *c2 = strtok(str2, "-"); c2 != nullptr; c2 = strtok(nullptr, "-"))
        {
            CAppEvent e;
            e.FromString(std::string(c2));
            comb.AddEvent(e);
        }

        combinations.push_back(comb);
    });

    CAppEventCombinationTree *node = &mTree;
    for (auto i = combinations.begin(); i != combinations.end(); i++)
    {
        bool found = false;

        for (auto j = node->mChilds.begin(); j != node->mChilds.end(); j++)
        {
            if (j->mCombination == *i)
            {
                found = true;
                node = &*j;
                break;
            }
        }

        if (found == false)
        {
            node->mChilds.push_back(CAppEventCombinationTree());
            node->mChilds.back().mCombination = *i;
            node = &node->mChilds.back();
        }
    };

    node->mProcessor = _processor;
}

void CAppEventSystem::Process()
{
    std::for_each(mCurrentCombinations.begin(), mCurrentCombinations.end(), [] (CAppEventCombinationTree *&t)
    {
        t->mProcessor.Pressing();
    });
}

void CAppEventSystem::PressEvent(const CAppEvent &_event)
{
    mCurrentState.AddEvent(_event);

    bool fallback = true;

    // now we find key combination in current combination tree root
    // if we pressed key, which is completely wrong, we set fallback flag to true

    for (auto i = mCurrentRoot->mChilds.begin(); i != mCurrentRoot->mChilds.end(); i++)
    {
        // mOperationLock is used for fast determine, that node already in mCurrentCombinations list
        if (i->mOperationLock == false && mCurrentState.ContainsCombination(i->mCombination))
        {
            this->mCurrentCombinations.push_back(&*i);
            i->mOperationLock = true;
            i->mProcessor.BeginPressing();

            fallback = false;
        }
        else if (fallback == true && i->mCombination.ContainsEvent(_event) == true)
        {
            fallback = false;
        }
    }

    // if pressed key is completely wrong and current tree node is not &mTree (root)
    // try to process this key second time, using &mTree as current tree node

    if (fallback == true && mCurrentRoot != &mTree)
    {
        mCurrentRoot = &mTree;

        for (auto i = mCurrentRoot->mChilds.begin(); i != mCurrentRoot->mChilds.end(); i++)
        {
            if (i->mOperationLock == false && mCurrentState.ContainsCombination(i->mCombination))
            {
                this->mCurrentCombinations.push_back(&*i);
                i->mOperationLock = true;
                i->mProcessor.BeginPressing();
            }
        }
    }
}

void CAppEventSystem::ReleaseEvent(const CAppEvent &_event)
{
    mCurrentState.RemoveEvent(_event);

    // find key combinations, thas is not intersect current events state

    for (auto i = mCurrentCombinations.begin(); i != mCurrentCombinations.end();)
    {
        if (mCurrentState.ContainsCombination((*i)->mCombination) == false)
        {
            // if this tree node's combination is last one to be removed, look for his childs

            if (mCurrentCombinations.size() == 1)
            {
                if ((*i)->mChilds.size() != 0)
                {
                    // if it has any - try to process next combinations from this tree node

                    mCurrentRoot = *i;
                }
                else
                {
                    // fall back to combinations tree root

                    mCurrentRoot = &mTree;
                }
            }

            // process end pressing end remove

            (*i)->mOperationLock = false;
            (*i)->mProcessor.EndPressing();
            i = mCurrentCombinations.erase(i);
            continue;
        }

        i++;
    }
}

} // namespace drash
