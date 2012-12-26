// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

#include "test.h"

#include "test1.h"
#include "test2.h"
#include "test3.h"
#include "test4.h"
#include "test5.h"

namespace drash
{

namespace test
{

CApp *StartApp( const char *_name )
{
    if ( strcmp(_name, "test1" ) == 0 ) return new CTest1();
    if ( strcmp(_name, "test2") == 0) return new CTest2();
    if ( strcmp(_name, "test3") == 0) return new CTest3();
    if ( strcmp(_name, "test4") == 0) return new drash::test::CTest4;
    if ( strcmp(_name, "test5") == 0) return new drash::test::CTest5;
    return nullptr;
}

} // namespace test

}// namespace drash
