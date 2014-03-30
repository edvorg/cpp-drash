// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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

#include "appeventsystem.h"

#include <algorithm>
#include <cstring>

#include "../diag/logger.h"

namespace drash {

    AppEventCombinationTree::AppEventCombinationTree() {}

    AppEventSystem::AppEventSystem() { SetMode(std::string("normal")); }

    bool AppEventSystem::ValidateModeName(const std::string& _name) {
        return std::find(_name.begin(), _name.end(), ' ') == _name.end();
    }

    bool AppEventSystem::SetMode(const std::string& _name) {
        if (ValidateModeName(_name) == false) {
            LOG_ERR("AppEventSystem::SetMode(): invalid mode name \""
                    << _name.c_str() << "\"");
            return false;
        }

        auto i = trees.find(_name);

        if (i == trees.end()) {
            trees.insert(std::pair<std::string, AppEventCombinationTree>(
                _name, AppEventCombinationTree()));
            i = trees.find(_name);
        } else if (i->first == currentMode) {
            return true;
        }

        currentMode = _name;
        currentNode = currentModeRoot = &i->second;

        for (auto i = currentCombinations.begin();
             i != currentCombinations.end(); i++) {
            (*i)->state |= STATE_END;
        }

        LOG_INFO("mode changed: " << _name.c_str());

        return true;
    }

    void AppEventSystem::SetProcessor(const char* _combinations,
                                       const AppEventProcessor& _processor) {
        using std::list;
        using std::string;
        using std::for_each;

        char str[128];
        strncpy(str, _combinations, 127);
        str[127] = 0;

        list<string> simple_combinations;
        list<AppEventCombination> combinations;

        for (char* c = strtok(str, " "); c != nullptr;
             c = strtok(nullptr, " ")) {
            simple_combinations.push_back(string(c));
        }

        std::for_each(simple_combinations.begin(), simple_combinations.end(),
                      [&combinations](string& _str) {
            char str2[128];
            strncpy(str2, _str.c_str(), 127);
            str2[127] = 0;

            AppEventCombination comb;
            for (char* c2 = strtok(str2, "-"); c2 != nullptr;
                 c2 = strtok(nullptr, "-")) {
                AppEvent e;
                e.FromString(std::string(c2));
                comb.AddEvent(e);
            }

            combinations.push_back(comb);
        });

        AppEventCombinationTree* node = currentModeRoot;
        for (auto i = combinations.begin(); i != combinations.end(); i++) {
            bool found = false;

            for (auto j = node->childs.begin(); j != node->childs.end(); j++) {
                if (j->combination == *i) {
                    found = true;
                    node = &*j;
                    break;
                }
            }

            if (found == false) {
                node->childs.push_back(AppEventCombinationTree());
                node->childs.back().combination = *i;
                node = &node->childs.back();
            }
        };

        node->processor = _processor;
    }

    void AppEventSystem::Process() {
        for (auto i = currentCombinations.begin();
             i != currentCombinations.end();) {
            AppEventCombinationTree* c = *i;

            if (c->state & STATE_BEGIN) {
                c->state ^= STATE_BEGIN;
                c->state |= STATE_PROCESSING;
                c->processor.Begin();
            }

            if (c->state & STATE_CANCEL) {
                c->state = STATE_NORMAL;
                i = currentCombinations.erase(i);
                continue;
            }

            if (c->state & STATE_END) {
                c->processor.End();
                c->state = STATE_NORMAL;
                i = currentCombinations.erase(i);
                continue;
            }

            if (c->state & STATE_PROCESSING) {
                c->processor.Processing();
            }

            i++;
        }
    }

    void AppEventSystem::BeginEvent(const AppEvent& _event) {
        currentState.AddEvent(_event);

        // now we find key combination in current combination tree root
        // if we pressed key, which is completely wrong, we set res flag to true

        int res = PressEventImpl(_event);

        // if pressed key is completely wrong and current tree node is not
        // &tree (root)
        // try to process this key second time, using &tree as current tree
        // node

        if (res == 0 && currentNode != currentModeRoot) {
            currentNode = currentModeRoot;

            PressEventImpl(_event);
        }
    }

    void AppEventSystem::EndEvent(const AppEvent& _event) {
        currentState.RemoveEvent(_event);

        // find key combinations, thas is not intersect current events state

        for (auto i = currentCombinations.begin();
             i != currentCombinations.end(); i++) {
            if (currentState.ContainsCombination((*i)->combination) == false) {
                // if this tree node's combination is last one to be removed,
                // look for his childs

                if (currentCombinations.size() == 1) {
                    if ((*i)->childs.size() != 0) {
                        // if it has any - try to process next combinations from
                        // this tree node

                        currentNode = *i;
                    } else {
                        // fall back to combinations tree root

                        currentNode = currentModeRoot;
                    }
                }

                (*i)->state |= STATE_END;
            }
        }
    }

    void AppEventSystem::CancelEvent(const AppEvent& _event) {
        currentState.RemoveEvent(_event);

        // find key combinations, thas is not intersect current events state

        for (auto i = currentCombinations.begin();
             i != currentCombinations.end(); i++) {
            if (currentState.ContainsCombination((*i)->combination) == false) {
                // if this tree node's combination is last one to be removed,
                // look for his childs

                if (currentCombinations.size() == 1) {
                    if ((*i)->childs.size() != 0) {
                        // if it has any - try to process next combinations from
                        // this tree node

                        currentNode = *i;
                    } else {
                        // fall back to combinations tree root

                        currentNode = currentModeRoot;
                    }
                }

                (*i)->state |= STATE_CANCEL;
            }
        }
    }

    int AppEventSystem::PressEventImpl(const AppEvent& _event) {
        int res = 0;

        for (auto i = currentNode->childs.begin();
             i != currentNode->childs.end();
             i++) { // operationLock is used for fast determine, that node
                    // already in currentCombinations list
            if (i->state == STATE_NORMAL &&
                currentState.ContainsCombination(i->combination)) {
                res = 1;

                i->state = STATE_BEGIN;
                this->currentCombinations.push_back(&*i);
            } else if (res == 0 &&
                       i->combination.ContainsEvent(_event) == true) {
                res = 1;
            }
        }

        return res;
    }

} // namespace drash
