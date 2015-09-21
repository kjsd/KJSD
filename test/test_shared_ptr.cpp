/**
 * @file test_shared_ptr.cpp
 *
 * @brief A unit test suite of SharedPtr
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2015 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <iostream>
#include <cstring>
#include <kjsd/cunit.h>
#include <kjsd/shared_ptr.hpp>

namespace test_sp_ns
{
    struct Foo
    {
        int f_;
    };
}

using namespace std;
using namespace kjsd;
using namespace test_sp_ns;

static int* int_p1_;

static void setUp()
{
	int_p1_ = new int(1);

	cout << "int_p1_: " << reinterpret_cast<size_t>(int_p1_) << endl;
}

static void tearDown()
{
}

static const char* test_use_count()
{
	SharedPtr<int> p1(int_p1_);
    KJSD_CUNIT_ASSERT(p1.use_count() == 1);

	SharedPtr<int> p11(int_p1_);
    KJSD_CUNIT_ASSERT(p1.use_count() == 2);
    KJSD_CUNIT_ASSERT(p11.use_count() == 2);
    return 0;
}

static const char* test_astah()
{
	SharedPtr<int> p1(int_p1_);
    KJSD_CUNIT_ASSERT(*p1 == 1);

	*p1 = 2;
    KJSD_CUNIT_ASSERT(*p1 == 2);
    return 0;
}

static const char* test_new()
{
	SharedPtr<int> p1(int_p1_);
    KJSD_CUNIT_ASSERT(*p1 == 1);

	SharedPtr<int> p = new int(3);
    KJSD_CUNIT_ASSERT(*p == 3);

	SharedPtr<int> pp = p1;
    KJSD_CUNIT_ASSERT(*pp == 1);
    return 0;
}

static const char* test_copy()
{
	SharedPtr<int> p1(int_p1_);
    KJSD_CUNIT_ASSERT(*p1 == 1);
	SharedPtr<int> p(new int(3));
    KJSD_CUNIT_ASSERT(*p == 3);

	p1 = p;
    KJSD_CUNIT_ASSERT(p1.use_count() == 2);
    KJSD_CUNIT_ASSERT(*p1 == 3);
    KJSD_CUNIT_ASSERT(p.use_count() == 2);
    KJSD_CUNIT_ASSERT(*p == 3);

	p1 = new int(1);
    KJSD_CUNIT_ASSERT(*p1 == 1);

	p1 = int_p1_;
    KJSD_CUNIT_ASSERT(*p1 == 1);
    return 0;
}

static const char* test_member()
{
	SharedPtr<Foo> p = new Foo();
	p->f_ = 3;
    KJSD_CUNIT_ASSERT(p->f_ == 3);
    KJSD_CUNIT_ASSERT((*p).f_ == 3);

	delete int_p1_;
    return 0;
}

static const char* test_compare()
{
	SharedPtr<int> p;
	SharedPtr<int> p1(int_p1_);

    KJSD_CUNIT_ASSERT(p == 0);
    KJSD_CUNIT_ASSERT(p != p1);

	p = p1;
    KJSD_CUNIT_ASSERT(p == p1);
    return 0;
}

const char* test_shared_ptr()
{
    const KJSD_CUNIT_Func f[] = {
        test_use_count,
        test_new,
        test_copy,
        test_astah,
        test_member,
        test_compare
    };

    for (int i = 0; i < KJSD_CUTIL_SIZEOFA(f); i++)
    {
        setUp();
        KJSD_CUNIT_RUN(f[i]);
        tearDown();
    }
    return 0;
}
