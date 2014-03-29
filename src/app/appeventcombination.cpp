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

#include "appeventcombination.h"

#include <algorithm>
#include "../diag/logger.h"

namespace drash {

    void CAppEventCombination::AddEvent(const CAppEvent& _e) {
        if (catchEvents.size() < catchEventsCountLimit) {
            auto i = std::find(catchEvents.begin(), catchEvents.end(), _e);

            if (i != catchEvents.end()) {
                return;
            }

            catchEvents.push_back(_e);
            std::sort(catchEvents.begin(), catchEvents.end());
        }
    }

    void CAppEventCombination::RemoveEvent(const CAppEvent& _e) {
        auto i = std::find(catchEvents.begin(), catchEvents.end(), _e);

        if (i != catchEvents.end()) {
            catchEvents.erase(i);
        }
    }

    void CAppEventCombination::Clear() { catchEvents.clear(); }

    bool CAppEventCombination::ContainsEvent(const CAppEvent& _e) const {
        return std::find(catchEvents.begin(), catchEvents.end(), _e) !=
               catchEvents.end();
    }

    bool CAppEventCombination::ContainsCombination(
        const CAppEventCombination& _c) const {
        bool res = false;

        for (unsigned int i = 0; i < catchEvents.size(); i++) {
            if (catchEvents[i] == *_c.catchEvents.begin()) {
                if (i + _c.catchEvents.size() <= catchEvents.size()) {
                    res = true;
                    i++;

                    for (unsigned int j = 1; j < _c.catchEvents.size();
                         j++, i++) {
                        if (catchEvents[i] != _c.catchEvents[j]) {
                            res = false;
                            break;
                        }
                    }
                }

                break;
            }
        }

        return res;
    }

    bool CAppEventCombination::
    operator==(const CAppEventCombination& _src) const {
        if (catchEvents.size() != _src.catchEvents.size()) {
            return false;
        }

        unsigned int last_elem = catchEvents.size() - 1;

        if (catchEvents[0] != _src.catchEvents[0] ||
            catchEvents[last_elem] != _src.catchEvents[last_elem]) {
            return false;
        }

        for (unsigned int i = 1; i < last_elem; i++) {
            if (catchEvents[i] != _src.catchEvents[i]) {
                return false;
            }
        }

        return true;
    }

    bool CAppEventCombination::
    operator!=(const CAppEventCombination& _src) const {
        return !(*this == _src);
    }

    CLogger& operator<<(CLogger& _logger, const CAppEventCombination& _c) {
        auto i = _c.catchEvents.begin();

        if (i != _c.catchEvents.end()) {
            _logger << i->ToString().c_str();
            i++;

            while (i != _c.catchEvents.end()) {
                _logger << '-' << i->ToString().c_str();
                i++;
            }
        }

        return _logger;
    }

} // namespace drash
