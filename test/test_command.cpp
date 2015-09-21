/**
 * @file test_command.cpp
 *
 * @brief A unit test suite of Command
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <iostream>
#include <kjsd/cunit.h>
#include <kjsd/command.hpp>

using namespace std;
using namespace kjsd;

static void static_arg0()
{
	cout << "I am static_arg0" << endl;
}

static int static_arg1(int a)
{
	cout << "I am static_arg1: " << a << endl;
	return a;
}

static int static_arg2(int a, int b)
{
	cout << "I am static_arg2: " << a << ',' << b << endl;
	return a + b;
}

static int static_arg3(int a, int b, int c)
{
	cout << "I am static_arg3: " << a << ',' << b << ',' << c << endl;
	return a + b + c;
}

namespace test_command_ns
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
            return a;
        }
        int arg2(int a, int b)
        {
            cout << "I am Foo::arg2: " << a << ',' << b << endl;
            return a + b;
        }
        int arg3(int a, int b, int c)
        {
            cout << "I am Foo::arg3: " << a << ',' << b << ',' << c << endl;
            return a + b + c;
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
            return a;
        }
        int arg2(int a, int b)
        {
            cout << "I am Bar::arg2: " << a << ',' << b << endl;
            return a + b;
        }
        int arg3(int a, int b, int c)
        {
            cout << "I am Bar::arg3: " << a << ',' << b << ',' << c << endl;
            return a + b + c;
        }
    };
}

using namespace test_command_ns;

static Foo f_;
static Bar b_;

static const char* test_exec()
{
	Command<void()> c0;
	Command<int(int)> c1;
	Command<int(int, int)> c2(1, 2);
	Command<int(int, int, int)> c3(1, 2, 3);

	c0 = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
	c0();
	c1 = new InstanceFunction<Foo, int(int)>(&f_, &Foo::arg1);
    c1.bind(1);
    KJSD_CUNIT_ASSERT(c1() == 1);
	c2 = new InstanceFunction<Foo, int(int, int)>(&f_, &Foo::arg2);
    KJSD_CUNIT_ASSERT(c2() == 3);
	c3 = new InstanceFunction<Foo, int(int, int, int)>(&f_, &Foo::arg3);
    KJSD_CUNIT_ASSERT(c3() == 6);

	Command<int(int, int, int)> cc3;
    
    cc3();
    cc3 = c3;
    KJSD_CUNIT_ASSERT(cc3() == 6);

	c0 = new InstanceFunction<Bar, void()>(&b_, &Bar::arg0);
	c0();
	c1 = new InstanceFunction<Bar, int(int)>(&b_, &Bar::arg1);
	c1.bind(11);
    KJSD_CUNIT_ASSERT(c1() == 11);
	c2 = new InstanceFunction<Bar, int(int,int)>(&b_, &Bar::arg2);
	c2.bind(11, 22);
    KJSD_CUNIT_ASSERT(c2() == 33);
	c3 = new InstanceFunction<Bar, int(int,int,int)>(&b_, &Bar::arg3);
	c3.bind(11, 22, 33);
    KJSD_CUNIT_ASSERT(c3() == 66);

	c0 = new StaticFunction<void()>(&static_arg0);
	c0();
	c1 = new StaticFunction<int(int)>(&static_arg1);
	c1.bind(111);
    KJSD_CUNIT_ASSERT(c1() == 111);
	c2 = new StaticFunction<int(int,int)>(&static_arg2);
	c2.bind(111, 222);
    KJSD_CUNIT_ASSERT(c2() == 333);
	c3 = new StaticFunction<int(int,int,int)>(&static_arg3);
	c3.bind(111, 222, 333);
    KJSD_CUNIT_ASSERT(c3() == 666);

    return 0;
}

const char* test_command()
{
    KJSD_CUNIT_RUN(test_exec);
    return 0;
}
