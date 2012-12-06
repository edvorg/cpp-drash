#include "appeventsystem.h"

namespace drash
{

CAppEventCombinationTree::CAppEventCombinationTree()
{
}

CAppEventSystem::CAppEventSystem()
{
}

bool CAppEventSystem::ValidateModeName(const std::string &_name)
{
    return std::find(_name.begin(), _name.end(), ' ') == _name.end();
}

bool CAppEventSystem::SetMode(const std::string &_name)
{
    if (ValidateModeName(_name) == false)
    {
        LOG_ERR("CAppEventSystem::SetMode(): invalid mode name \""<<_name.c_str()<<"\"");
        return false;
    }

    auto i = mTrees.find(_name);

    if (i == mTrees.end())
    {
        mTrees.insert(std::pair<std::string, CAppEventCombinationTree>(_name, CAppEventCombinationTree()));
        i = mTrees.find(_name);
    }
    else if (i->first == mCurrentMode)
    {
        return true;
    }

    mCurrentMode = _name;
    mCurrentNode = mCurrentModeRoot =&i->second;

    for (auto i = mCurrentCombinations.begin(); i != mCurrentCombinations.end(); i++)
    {
        (*i)->mOperationLock = false;
    }
    mCurrentCombinations.clear();

    mModeChanged = true;

    LOG_INFO("mode changed: "<<_name.c_str());

    return true;
}

void CAppEventSystem::SetProcessor(const char *_combinations, const CAppEventProcessor &_processor)
{
    if (mCurrentMode == "")
    {
        SetMode(std::string("normal"));
    }

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

    CAppEventCombinationTree *node = mCurrentModeRoot;
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
    for (auto i = mCurrentCombinations.begin(); i != mCurrentCombinations.end(); i++)
    {
        (*i)->mProcessor.Pressing();

        if (mModeChanged == true)
        {
            mModeChanged = false;
            return;
        }
    }
}

void CAppEventSystem::PressEvent(const CAppEvent &_event)
{
    mCurrentState.AddEvent(_event);

    bool fallback = true;

    // now we find key combination in current combination tree root
    // if we pressed key, which is completely wrong, we set fallback flag to true

    for (auto i = mCurrentNode->mChilds.begin(); i != mCurrentNode->mChilds.end(); i++)
    {
        // mOperationLock is used for fast determine, that node already in mCurrentCombinations list
        if (i->mOperationLock == false && mCurrentState.ContainsCombination(i->mCombination))
        {
            fallback = false;

            this->mCurrentCombinations.push_back(&*i);
            i->mOperationLock = true;
            i->mProcessor.BeginPressing();

            if (mModeChanged == true)
            {
                mModeChanged = false;
                return;
            }
        }
        else if (fallback == true && i->mCombination.ContainsEvent(_event) == true)
        {
            fallback = false;
        }
    }

    // if pressed key is completely wrong and current tree node is not &mTree (root)
    // try to process this key second time, using &mTree as current tree node

    if (fallback == true && mCurrentNode != mCurrentModeRoot)
    {
        mCurrentNode = mCurrentModeRoot;

        for (auto i = mCurrentNode->mChilds.begin(); i != mCurrentNode->mChilds.end(); i++)
        {
            if (i->mOperationLock == false && mCurrentState.ContainsCombination(i->mCombination))
            {
                this->mCurrentCombinations.push_back(&*i);
                i->mOperationLock = true;
                i->mProcessor.BeginPressing();

                if (mModeChanged == true)
                {
                    mModeChanged = false;
                    return;
                }
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

                    mCurrentNode = *i;
                }
                else
                {
                    // fall back to combinations tree root

                    mCurrentNode = mCurrentModeRoot;
                }
            }

            // process end pressing end remove

            (*i)->mOperationLock = false;
            (*i)->mProcessor.EndPressing();

            if (mModeChanged == true)
            {
                mModeChanged = false;
                return;
            }

            i = mCurrentCombinations.erase(i);
            continue;
        }

        i++;
    }
}

} // namespace drash
