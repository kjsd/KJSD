/**
 * @file test_delegate.cpp
 *
 * @brief A unit test suite of Delegate
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <iostream>
#include <string>
#include <sstream>
#include <kjsd/cutil.h>
#include <kjsd/cunit.h>
#include <kjsd/delegate.hpp>

using namespace std;
using namespace kjsd;

static void static_arg0()
{
    cout << "I am static_arg0" << endl;
}

static int static_arg1(int a)
{
    cout << "I am static_arg1: " << a << endl;
    return 21;
}

static int static_arg2(int a, int b)
{
    cout << "I am static_arg2: " << a << ',' << b << endl;
    return 22;
}

static int static_arg3(int a, int b, int c)
{
    cout << "I am static_arg3: " << a << ',' << b << ',' << c << endl;
    return 23;
}

namespace test_delegate_ns
{
    class Foo
    {
    public:
        Foo() {}
        ~Foo() {}
        void arg0()
        {
            cout << "I am Foo::arg0" << endl;
        }
        int arg1(int a)
        {
            cout << "I am Foo::arg1: " << a << endl;
            return 1;
        }
        int arg2(int a, int b)
        {
            cout << "I am Foo::arg2: " << a << ',' << b << endl;
            return 2;
        }
        int arg3(int a, int b, int c)
        {
            cout << "I am Foo::arg3: " << a << ',' << b << ',' << c << endl;
            return 3;
        }
    };

    class Bar
    {
    public:
        Bar() {}
        ~Bar() {}
        void arg0()
        {
            cout << "I am Bar::arg0" << endl;
        }
        int arg1(int a)
        {
            cout << "I am Bar::arg1: " << a << endl;
            return 11;
        }
        int arg2(int a, int b)
        {
            cout << "I am Bar::arg2: " << a << ',' << b << endl;
            return 12;
        }
        int arg3(int a, int b, int c)
        {
            cout << "I am Bar::arg3: " << a << ',' << b << ',' << c << endl;
            return 13;
        }
    };
}

using namespace test_delegate_ns;

static Foo f_;
static Bar b_;

static Delegate<void()> d0_;
static Delegate<int(int)> d1_;
static Delegate<int(int, int)> d2_;
static Delegate<int(int, int, int)> d3_;

static void setUp()
{
    d0_ = 0;
    d1_ = 0;
    d2_ = 0;
    d3_ = 0;
}

static const char* test_nop()
{
    d0_();
    Delegate<void()> d0;
    d0();
    return 0;
}

static const char* test_ins_opEq()
{
    Delegate<void()> d0;
    Delegate<int(int)> d1;
    Delegate<int(int, int)> d2;
    Delegate<int(int, int, int)> d3;

    KJSD_CUNIT_ASSERT(d0_ == d0);
    d0_ = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
    d0 = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
    KJSD_CUNIT_ASSERT(d0_ == d0);

    KJSD_CUNIT_ASSERT(d1_ == d1);
    d1_ = new InstanceFunction<Foo, int(int)>(&f_, &Foo::arg1);
    d1 = d1_;
    KJSD_CUNIT_ASSERT(d1_ == d1);

    KJSD_CUNIT_ASSERT(d2_ == d2);
    d2_ = new InstanceFunction<Foo, int(int, int)>(&f_, &Foo::arg2);
    d2 = d2_;
    KJSD_CUNIT_ASSERT(d2_ == d2_);

    KJSD_CUNIT_ASSERT(d3_ == d3);
    d3_ = new InstanceFunction<Bar, int(int, int, int)>(&b_, &Bar::arg3);
    d3 = d3_;
    KJSD_CUNIT_ASSERT(d3_ == d3);
    return 0;
}

static const char* test_sta_opEq()
{
    Delegate<void()> d0;
    Delegate<int(int)> d1;
    Delegate<int(int, int)> d2;
    Delegate<int(int, int, int)> d3;

    KJSD_CUNIT_ASSERT(d0_ == d0);
    d0_ = new StaticFunction<void()>(&static_arg0);
    d0 = new StaticFunction<void()>(&static_arg0);
    KJSD_CUNIT_ASSERT(d0_ == d0);

    KJSD_CUNIT_ASSERT(d1_ == d1);
    d1_ = new StaticFunction<int(int)>(&static_arg1);
    d1 = d1_;
    KJSD_CUNIT_ASSERT(d1_ == d1);

    KJSD_CUNIT_ASSERT(d2_ == d2);
    d2_ = new StaticFunction<int(int, int)>(&static_arg2);
    d2 = d2_;
    KJSD_CUNIT_ASSERT(d2_ == d2);

    KJSD_CUNIT_ASSERT(d3_ == d3);
    d3_ = new StaticFunction<int(int, int, int)>(&static_arg3);
    d3 = d3_;
    KJSD_CUNIT_ASSERT(d3_ == d3);
    return 0;
}

static const char* test_ins_opNeq()
{
    Delegate<void()> d0;
    Delegate<int(int)> d1;
    Delegate<int(int, int)> d2;
    Delegate<int(int, int, int)> d3;

    d0_ = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
    KJSD_CUNIT_ASSERT(d0_ != d0);

    d1_ = new InstanceFunction<Foo, int(int)>(&f_, &Foo::arg1);
    KJSD_CUNIT_ASSERT(d1_ != d1);

    d2_ = new InstanceFunction<Foo, int(int,int)>(&f_, &Foo::arg2);
    KJSD_CUNIT_ASSERT(d2_ != d2);

    d3_ = new InstanceFunction<Foo, int(int,int,int)>(&f_, &Foo::arg3);
    KJSD_CUNIT_ASSERT(d3_ != d3);
    return 0;
}

static const char* test_sta_opNeq()
{
    Delegate<void()> d0;
    Delegate<int(int)> d1;
    Delegate<int(int, int)> d2;
    Delegate<int(int, int, int)> d3;

    d0_ = new StaticFunction<void()>(&static_arg0);
    KJSD_CUNIT_ASSERT(d0_ != d0);

    d1_ = new StaticFunction<int(int)>(&static_arg1);
    KJSD_CUNIT_ASSERT(d1_ != d1);

    d2_ = new StaticFunction<int(int,int)>(&static_arg2);
    KJSD_CUNIT_ASSERT(d2_ != d2);

    d3_ = new StaticFunction<int(int,int,int)>(&static_arg3);
    KJSD_CUNIT_ASSERT(d3_ != d3);
    return 0;
}

static const char* test_ins_exec()
{
    d0_();

    d0_ = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
    d0_();
    d0_ = new InstanceFunction<Bar, void()>(&b_, &Bar::arg0);
    d0_();

    d1_ = new InstanceFunction<Foo, int(int)>(&f_, &Foo::arg1);
    KJSD_CUNIT_ASSERT(d1_(1) == 1);
    d1_ = new InstanceFunction<Bar, int(int)>(&b_, &Bar::arg1);
    KJSD_CUNIT_ASSERT(d1_(1) == 11);

    d2_ = new InstanceFunction<Foo, int(int,int)>(&f_, &Foo::arg2);
    KJSD_CUNIT_ASSERT(d2_(1, 2) == 2);
    d2_ = new InstanceFunction<Bar, int(int,int)>(&b_, &Bar::arg2);
    KJSD_CUNIT_ASSERT(d2_(1, 2) == 12);

    d3_ = new InstanceFunction<Foo, int(int,int,int)>(&f_, &Foo::arg3);
    KJSD_CUNIT_ASSERT(d3_(1, 2, 3) == 3);
    d3_ = new InstanceFunction<Bar, int(int,int,int)>(&b_, &Bar::arg3);
    KJSD_CUNIT_ASSERT(d3_(1, 2, 3) == 13);
    return 0;
}

static const char* test_sta_exec()
{
    d0_ = new StaticFunction<void()>(&static_arg0);
    d0_();

    d1_ = new StaticFunction<int(int)>(&static_arg1);
    KJSD_CUNIT_ASSERT(d1_(1) == 21);

    d2_ = new StaticFunction<int(int,int)>(&static_arg2);
    KJSD_CUNIT_ASSERT(d2_(1, 2) == 22);

    d3_ = new StaticFunction<int(int,int,int)>(&static_arg3);
    KJSD_CUNIT_ASSERT(d3_(1, 2, 3) == 23);
    return 0;
}

const char* test_delegate()
{
    const KJSD_CUNIT_Func f[] = {
        test_nop,
        test_ins_opEq,
        test_sta_opEq,
        test_ins_opNeq,
        test_sta_opNeq,
        test_ins_exec,
        test_sta_exec
    };

    for (int i = 0; i < KJSD_CUTIL_LENGTH(f); i++)
    {
        setUp();
        KJSD_CUNIT_RUN(f[i]);
    }
    return 0;
}
