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

    class Logger;

    class AppEventCombination {
    public:
        constexpr static const unsigned int catchEventsCountLimit = 3;

        void AddEvent(const AppEvent& _e);
        void RemoveEvent(const AppEvent& _e);
        inline unsigned int GetEventsCount() const;
        void Clear();

        bool ContainsEvent(const AppEvent& _e) const;
        bool ContainsCombination(const AppEventCombination& _c) const;

        bool operator==(const AppEventCombination& _src) const;
        bool operator!=(const AppEventCombination& _src) const;

        friend Logger& operator<<(Logger& _logger,
                                   const AppEventCombination& _c);

    private:
    protected:
        std::vector<AppEvent> catchEvents;
    };

    inline unsigned int AppEventCombination::GetEventsCount() const {
        return catchEvents.size();
    }

} // namespace drash

#endif // DRASH_APPEVENTCOMBINATION_H
