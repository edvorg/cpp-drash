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
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#ifndef DRASH_APPEVENTPROCESSOR_H
#define DRASH_APPEVENTPROCESSOR_H

#include <functional>

namespace drash
{

class CAppEventProcessor
{
public:
    typedef std::function<void ()> FuncT;

    CAppEventProcessor();
    CAppEventProcessor(const FuncT &_begin,
                       const FuncT &_processing = [] () {},
                       const FuncT &_end = [] () {});
    CAppEventProcessor(const CAppEventProcessor &_src);

    inline void Begin();
    inline void Processing();
    inline void End();
protected:
private:

    FuncT mBegin = [] () {};
    FuncT mProcessing = [] () {};
    FuncT mEnd = [] () {};
};

inline void CAppEventProcessor::Begin()
{
    mBegin();
}

inline void CAppEventProcessor::Processing()
{
    mProcessing();
}

inline void CAppEventProcessor::End()
{
    mEnd();
}

} // namespace drash

#endif // DRASH_APPEVENTPROCESSOR_H