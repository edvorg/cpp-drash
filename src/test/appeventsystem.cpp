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
    if (mCombinationLock == true)
    {
        mCurrentNode->mProcessor.Pressing();
    }
}

void CAppEventSystem::PressEvent(const CAppEvent &_event)
{
    if (mCombinationLock == true)
    {
        return;
    }

    LOG_INFO("pressed "<<_event.ToString().c_str());
    mCurrentCombination.AddEvent(_event);

    bool right_way = false;

    for (auto i = mCurrentNode->mChilds.begin(); i != mCurrentNode->mChilds.end(); i++)
    {
        if (i->mCombination == mCurrentCombination)
        {
            i->mProcessor.BeginPressing();
            mCurrentNode = &*i;
            mCombinationLock = true;
            return;
        }
        else if (i->mCombination.ContainsEvent(_event))
        {
            right_way = true;
        }
    }

    if (!right_way)
    {
        mCurrentNode = &mTree;
        for (auto i = mCurrentNode->mChilds.begin(); i != mCurrentNode->mChilds.end(); i++)
        {
            if (i->mCombination == mCurrentCombination)
            {
                i->mProcessor.BeginPressing();
                mCurrentNode = &*i;
                mCombinationLock = true;
                return;
            }
        }
    }
}

void CAppEventSystem::ReleaseEvent(const CAppEvent &_event)
{
    LOG_INFO("released "<<_event.ToString().c_str());
    mCurrentCombination.RemoveEvent(_event);

    if (mCurrentCombination.GetEventsCount() == 0)
    {
        mCurrentNode->mProcessor.EndPressing();
        if (mCurrentNode->mChilds.size() == 0)
        {
            mCurrentNode = &mTree;
        }
        mCombinationLock = false;
    }
}

} // namespace drash
