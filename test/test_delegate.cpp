/**
 * @file test_delegate.cpp
 *
 * @version $Id:$
 *
 * @brief A unit test suite of Delegate
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
/// このヘッダは必ずincludeする
#include <cppunit/extensions/HelperMacros.h>

#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include "../include/kjsd/delegate.hpp"

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

/**
 * @class TEST_Delegate
 *
 * @brief Delegateクラスのユニットテストスイート
 *
 * @see Delegate
 */
// CppUnit::TestFixtureを継承させる
class TEST_Delegate : public CppUnit::TestFixture
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

    //---> ここからフレームワークのお約束事

    /// テストスイート宣言開始マクロ．このクラス名を指定する．
    CPPUNIT_TEST_SUITE(TEST_Delegate);

    /// 以下テストケース宣言マクロ．テスト実行時にはこの順番で呼ばれる．
    /// あるテストケースで別機能を使う必要がある場合が考えられるから，
    /// 単体で完結する機能からテストされる順番になるよう注意する．
    CPPUNIT_TEST(test_nop);
    CPPUNIT_TEST(test_ins_opEq);
    CPPUNIT_TEST(test_sta_opEq);
    CPPUNIT_TEST(test_ins_opNeq);
    CPPUNIT_TEST(test_sta_opNeq);
    CPPUNIT_TEST(test_ins_exec);
    CPPUNIT_TEST(test_sta_exec);

    /// テストスイート宣言終了マクロ
    CPPUNIT_TEST_SUITE_END();

public:
    /**
     * @brief 各テストケース実行前に呼ばれるコールバック．
     * 通常テストケースに共通して必要なリソースを全て確保する．
     */
    void setUp();

    /**
     * @brief 各テストケース終了後に呼ばれるコールバック．
     * テストケースが互いに影響し合わないようにするため，setUp()で確保した
     * テスト用リソースを全て開放しておく．
     */
    void tearDown();

protected:
    //<--- ここまでフレームワークのお約束事．
    // あとはクラス宣言後にもうひとつ(CPPUNIT_TEST_SUITE_REGISTRATION)すれば
    // お約束事は終了．

    // 以下は実際のテストケース宣言．テスト対象によって大きく変わる部分．
    // 上のCPPUNIT_TEST()で登録したメソッドは全てここで宣言する．

    void test_nop();
    void test_ins_opEq();
    void test_sta_opEq();
    void test_ins_opNeq();
    void test_sta_opNeq();
    void test_ins_exec();
    void test_sta_exec();

private:
    Foo f_;
    Bar b_;

    Delegate<void()> d0_;
    Delegate<int(int)> d1_;
    Delegate<int(int, int)> d2_;
    Delegate<int(int, int, int)> d3_;
};

/// このテストスイートをCppUnitへ登録
CPPUNIT_TEST_SUITE_REGISTRATION(TEST_Delegate);

void TEST_Delegate::setUp()
{
	cout << endl;

    d0_ = 0;
    d1_ = 0;
    d2_ = 0;
    d3_ = 0;
}

void TEST_Delegate::tearDown()
{
}

void TEST_Delegate::test_nop()
{
	d0_();
    Delegate<void()> d0;
	d0();
}

void TEST_Delegate::test_ins_opEq()
{
    Delegate<void()> d0;
    Delegate<int(int)> d1;
    Delegate<int(int, int)> d2;
    Delegate<int(int, int, int)> d3;

    CPPUNIT_ASSERT(d0_ == d0);
    d0_ = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
    d0 = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
    CPPUNIT_ASSERT(d0_ == d0);

    CPPUNIT_ASSERT(d1_ == d1);
    d1_ = new InstanceFunction<Foo, int(int)>(&f_, &Foo::arg1);
    d1 = d1_;
    CPPUNIT_ASSERT(d1_ == d1);

    CPPUNIT_ASSERT(d2_ == d2);
    d2_ = new InstanceFunction<Foo, int(int, int)>(&f_, &Foo::arg2);
    d2 = d2_;
    CPPUNIT_ASSERT(d2_ == d2_);

    CPPUNIT_ASSERT(d3_ == d3);
    d3_ = new InstanceFunction<Bar, int(int, int, int)>(&b_, &Bar::arg3);
    d3 = d3_;
    CPPUNIT_ASSERT(d3_ == d3);
}

void TEST_Delegate::test_sta_opEq()
{
    Delegate<void()> d0;
    Delegate<int(int)> d1;
    Delegate<int(int, int)> d2;
    Delegate<int(int, int, int)> d3;

    CPPUNIT_ASSERT(d0_ == d0);
    d0_ = new StaticFunction<void()>(&static_arg0);
    d0 = new StaticFunction<void()>(&static_arg0);
    CPPUNIT_ASSERT(d0_ == d0);

    CPPUNIT_ASSERT(d1_ == d1);
    d1_ = new StaticFunction<int(int)>(&static_arg1);
    d1 = d1_;
    CPPUNIT_ASSERT(d1_ == d1);

    CPPUNIT_ASSERT(d2_ == d2);
    d2_ = new StaticFunction<int(int, int)>(&static_arg2);
    d2 = d2_;
    CPPUNIT_ASSERT(d2_ == d2);

    CPPUNIT_ASSERT(d3_ == d3);
    d3_ = new StaticFunction<int(int, int, int)>(&static_arg3);
    d3 = d3_;
    CPPUNIT_ASSERT(d3_ == d3);
}

void TEST_Delegate::test_ins_opNeq()
{
    Delegate<void()> d0;
    Delegate<int(int)> d1;
    Delegate<int(int, int)> d2;
    Delegate<int(int, int, int)> d3;

    d0_ = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
    CPPUNIT_ASSERT(d0_ != d0);

    d1_ = new InstanceFunction<Foo, int(int)>(&f_, &Foo::arg1);
    CPPUNIT_ASSERT(d1_ != d1);

    d2_ = new InstanceFunction<Foo, int(int,int)>(&f_, &Foo::arg2);
    CPPUNIT_ASSERT(d2_ != d2);

    d3_ = new InstanceFunction<Foo, int(int,int,int)>(&f_, &Foo::arg3);
    CPPUNIT_ASSERT(d3_ != d3);
}

void TEST_Delegate::test_sta_opNeq()
{
    Delegate<void()> d0;
    Delegate<int(int)> d1;
    Delegate<int(int, int)> d2;
    Delegate<int(int, int, int)> d3;

    d0_ = new StaticFunction<void()>(&static_arg0);
    CPPUNIT_ASSERT(d0_ != d0);

    d1_ = new StaticFunction<int(int)>(&static_arg1);
    CPPUNIT_ASSERT(d1_ != d1);

    d2_ = new StaticFunction<int(int,int)>(&static_arg2);
    CPPUNIT_ASSERT(d2_ != d2);

    d3_ = new StaticFunction<int(int,int,int)>(&static_arg3);
    CPPUNIT_ASSERT(d3_ != d3);
}

void TEST_Delegate::test_ins_exec()
{
    d0_();

    d0_ = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
    d0_();
    d0_ = new InstanceFunction<Bar, void()>(&b_, &Bar::arg0);
    d0_();

    d1_ = new InstanceFunction<Foo, int(int)>(&f_, &Foo::arg1);
    CPPUNIT_ASSERT(d1_(1) == 1);
    d1_ = new InstanceFunction<Bar, int(int)>(&b_, &Bar::arg1);
    CPPUNIT_ASSERT(d1_(1) == 11);

    d2_ = new InstanceFunction<Foo, int(int,int)>(&f_, &Foo::arg2);
    CPPUNIT_ASSERT(d2_(1, 2) == 2);
    d2_ = new InstanceFunction<Bar, int(int,int)>(&b_, &Bar::arg2);
    CPPUNIT_ASSERT(d2_(1, 2) == 12);

    d3_ = new InstanceFunction<Foo, int(int,int,int)>(&f_, &Foo::arg3);
    CPPUNIT_ASSERT(d3_(1, 2, 3) == 3);
    d3_ = new InstanceFunction<Bar, int(int,int,int)>(&b_, &Bar::arg3);
    CPPUNIT_ASSERT(d3_(1, 2, 3) == 13);
}

void TEST_Delegate::test_sta_exec()
{
    d0_ = new StaticFunction<void()>(&static_arg0);
    d0_();

    d1_ = new StaticFunction<int(int)>(&static_arg1);
    CPPUNIT_ASSERT(d1_(1) == 21);

    d2_ = new StaticFunction<int(int,int)>(&static_arg2);
    CPPUNIT_ASSERT(d2_(1, 2) == 22);

    d3_ = new StaticFunction<int(int,int,int)>(&static_arg3);
    CPPUNIT_ASSERT(d3_(1, 2, 3) == 23);
}
