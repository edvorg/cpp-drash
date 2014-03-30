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

#include "test.h"

#include "test1.h"
#include "test2.h"
#include "test3.h"
#include "test4.h"
#include "test5.h"
#include "test6.h"
#include "test7.h"
#include "test8.h"
#include "test9.h"
#include "test10.h"
#include "test11.hpp"
#include "test12.hpp"
#include "test13.hpp"

#include <cstring>

namespace drash {

    namespace test {

        App* StartApp(greng::Greng& greng, const char* _name) {
            if (strcmp(_name, "test1") == 0)
                return new Test1(greng);
            if (strcmp(_name, "test2") == 0)
                return new Test2(greng);
            if (strcmp(_name, "test3") == 0)
                return new Test3(greng);
            if (strcmp(_name, "test4") == 0)
                return new Test4(greng);
            if (strcmp(_name, "test5") == 0)
                return new Test5(greng);
            if (strcmp(_name, "test6") == 0)
                return new Test6(greng);
            if (strcmp(_name, "test7") == 0)
                return new Test7(greng);
            if (strcmp(_name, "test8") == 0)
                return new Test8(greng);
            if (strcmp(_name, "test9") == 0)
                return new Test9(greng);
            if (strcmp(_name, "test10") == 0)
                return new Test10(greng);
            if (strcmp(_name, "test11") == 0)
                return new Test11(greng);
            if (strcmp(_name, "test12") == 0)
                return new Test12(greng);
            if (strcmp(_name, "test13") == 0)
                return new Test13(greng);
            return nullptr;
        }

    } // namespace test

} // namespace drash
