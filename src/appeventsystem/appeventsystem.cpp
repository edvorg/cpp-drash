// DRASH_LICENSE_BEGIN
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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "appeventsystem.h"

#include <algorithm>
#include <cstring>

#include "../diag/logger.h"

namespace drash
{

CAppEventCombinationTree::CAppEventCombinationTree()
{
}

CAppEventSystem::CAppEventSystem()
{
}

bool CAppEventSystem::Init()
{
    if (mCurrentMode == "")
    {
        if (SetMode(std::string("normal")) == false)
        {
            return false;
        }
    }

    return true;
}

void CAppEventSystem::Release()
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
        (*i)->mState |= STATE_END;
    }

    LOG_INFO("mode changed: "<<_name.c_str());

    return true;
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
    for (auto i = mCurrentCombinations.begin(); i != mCurrentCombinations.end();)
    {
        CAppEventCombinationTree *c = *i;

        if (c->mState & STATE_BEGIN)
        {
            c->mState ^= STATE_BEGIN;
            c->mState |= STATE_PROCESSING;
            c->mProcessor.Begin();
        }

        if (c->mState & STATE_CANCEL)
        {
            c->mState = STATE_NORMAL;
            i = mCurrentCombinations.erase(i);
            continue;
        }

        if (c->mState & STATE_END)
        {
            c->mProcessor.End();
            c->mState = STATE_NORMAL;
            i = mCurrentCombinations.erase(i);
            continue;
        }

        if (c->mState & STATE_PROCESSING)
        {
            c->mProcessor.Processing();
        }

        i++;
    }
}

void CAppEventSystem::BeginEvent(const CAppEvent &_event)
{
    mCurrentState.AddEvent(_event);

    // now we find key combination in current combination tree root
    // if we pressed key, which is completely wrong, we set res flag to true

    int res = PressEventImpl(_event);

    // if pressed key is completely wrong and current tree node is not &mTree (root)
    // try to process this key second time, using &mTree as current tree node

    if (res == 0 && mCurrentNode != mCurrentModeRoot)
    {
        mCurrentNode = mCurrentModeRoot;

        PressEventImpl(_event);
    }
}

void CAppEventSystem::EndEvent(const CAppEvent &_event)
{
    mCurrentState.RemoveEvent(_event);

    // find key combinations, thas is not intersect current events state

    for (auto i = mCurrentCombinations.begin(); i != mCurrentCombinations.end(); i++)
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

            (*i)->mState |= STATE_END;
        }
    }
}

void CAppEventSystem::CancelEvent(const CAppEvent &_event)
{
    mCurrentState.RemoveEvent(_event);

    // find key combinations, thas is not intersect current events state

    for (auto i = mCurrentCombinations.begin(); i != mCurrentCombinations.end(); i++)
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

            (*i)->mState |= STATE_CANCEL;
        }
    }
}

int CAppEventSystem::PressEventImpl(const CAppEvent &_event)
{
    int res = 0;

    for (auto i = mCurrentNode->mChilds.begin(); i != mCurrentNode->mChilds.end(); i++)
    {       // mOperationLock is used for fast determine, that node already in mCurrentCombinations list
        if (i->mState == STATE_NORMAL && mCurrentState.ContainsCombination(i->mCombination))
        {
            res = 1;

            i->mState = STATE_BEGIN;
            this->mCurrentCombinations.push_back(&*i);
        }
        else if (res == 0 && i->mCombination.ContainsEvent(_event) == true)
        {
            res = 1;
        }
    }

    return res;
}

} // namespace drash
