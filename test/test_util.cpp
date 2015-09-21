/**
 * @file test_util.cpp
 *
 * @brief A unit test suite of Util
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <iostream>
#include <cstring>
#include <kjsd/cunit.h>
#include <kjsd/util.hpp>
#include <kjsd/cutil.h>

using namespace std;
using namespace kjsd;

static const char* test_sleep();

static const char* test_sleep()
{
    cout << "sleep 11ms..." << flush;
    KJSD_MSLEEP(11);
    cout << "done." << endl;

    cout << "sleep 111ms..." << flush;
    KJSD_MSLEEP(111);
    cout << "done." << endl;

    cout << "sleep 1111ms..." << flush;
    KJSD_MSLEEP(1111);
    cout << "done." << endl;

    cout << "sleep 5111ms..." << flush;
    KJSD_MSLEEP(5111);
    cout << "done." << endl;

    cout << "sleep 11111ms..." << flush;
    KJSD_MSLEEP(11111);
    cout << "done." << endl;

    return 0;
}

const char* test_util()
{
    KJSD_CUNIT_RUN(test_sleep);
    return 0;
}
