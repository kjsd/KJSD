/**
 * @file test_argument.cpp
 *
 * @brief A unit test suite of Argument
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
#include <kjsd/argument.hpp>

using namespace std;
using namespace kjsd;

static const char* TEST_ARGV0 = "foo";

static const char* test_arg()
{
    const char *opt = "aaa";
    const int argc = 2;
    const char* argv[] = { TEST_ARGV0, opt };

    Argument arg(argc, argv);

    Argument::iterator it = arg.begin();
    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::ARG);
    KJSD_CUNIT_ASSERT_EQUAL(*it, '\0');
    KJSD_CUNIT_ASSERT(strcmp(it.argument(), opt) == 0);
    return 0;
}

static const char* test_long()
{
    const char* opt = "--aaa";
    const char* optarg = "aaa";
    const int argc = 2;
    const char* argv[] = { TEST_ARGV0, opt };

    Argument arg(argc, argv);

    Argument::iterator it = arg.begin();
    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::LONG);
    KJSD_CUNIT_ASSERT_EQUAL(*it, '\0');
    KJSD_CUNIT_ASSERT(strcmp(it.argument(), optarg) == 0);
    return 0;
}

static const char* test_short()
{
    const char* opt = "-a";
    const int argc = 2;
    const char* argv[] = { TEST_ARGV0, opt };

    Argument arg(argc, argv);

    Argument::iterator it = arg.begin();
    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    KJSD_CUNIT_ASSERT_EQUAL(*it, 'a');
    KJSD_CUNIT_ASSERT(strcmp(it.argument(), opt) == 0);
    return 0;
}

static const char* test_complex()
{
    const int argc = 6;
    const char* argv[] = {
        TEST_ARGV0,
        "-abc",
        "-d",
        "eee",
        "--ffff",
        "ggg"
    };

    Argument arg(argc, argv);

    Argument::iterator it = arg.begin();

    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    KJSD_CUNIT_ASSERT_EQUAL(*it, 'a');

    ++it;
    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    KJSD_CUNIT_ASSERT_EQUAL(*it, 'b');
    
    ++it;
    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    KJSD_CUNIT_ASSERT_EQUAL(*it, 'c');

    ++it;
    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    KJSD_CUNIT_ASSERT_EQUAL(*it, 'd');

    ++it;
    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::ARG);
    KJSD_CUNIT_ASSERT(strcmp(it.argument(), "eee") == 0);

    ++it;
    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::LONG);
    KJSD_CUNIT_ASSERT(strcmp(it.argument(), "ffff") == 0);

    ++it;
    KJSD_CUNIT_ASSERT_EQUAL(it.status(), Argument::ARG);
    KJSD_CUNIT_ASSERT(strcmp(it.argument(), "ggg") == 0);

    ++it;
    KJSD_CUNIT_ASSERT(it == arg.end());
    return 0;
}

const char* test_argument()
{
    KJSD_CUNIT_RUN(test_arg);
    KJSD_CUNIT_RUN(test_long);
    KJSD_CUNIT_RUN(test_short);
    KJSD_CUNIT_RUN(test_complex);
    return 0;
}
