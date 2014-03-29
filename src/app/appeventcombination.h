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

#ifndef DRASH_APPEVENTCOMBINATION_H
#define DRASH_APPEVENTCOMBINATION_H

#include "appevent.h"
#include <vector>

namespace drash {

    class CLogger;

    class CAppEventCombination {
    public:
        constexpr static const unsigned int catchEventsCountLimit = 3;

        void AddEvent(const CAppEvent& _e);
        void RemoveEvent(const CAppEvent& _e);
        inline unsigned int GetEventsCount() const;
        void Clear();

        bool ContainsEvent(const CAppEvent& _e) const;
        bool ContainsCombination(const CAppEventCombination& _c) const;

        bool operator==(const CAppEventCombination& _src) const;
        bool operator!=(const CAppEventCombination& _src) const;

        friend CLogger& operator<<(CLogger& _logger,
                                   const CAppEventCombination& _c);

    private:
    protected:
        std::vector<CAppEvent> catchEvents;
    };

    inline unsigned int CAppEventCombination::GetEventsCount() const {
        return catchEvents.size();
    }

} // namespace drash

#endif // DRASH_APPEVENTCOMBINATION_H
