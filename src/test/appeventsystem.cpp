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
}

void CAppEventSystem::PressEvent(const CAppEvent &_event)
{
    mCurrentCombination.AddEvent(_event);
}

void CAppEventSystem::ReleaseEvent(const CAppEvent &_event)
{
    mCurrentCombination.RemoveEvent(_event);
}

} // namespace drash
