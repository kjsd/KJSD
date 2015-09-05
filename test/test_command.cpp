/**
 * @file test_command.cpp
 *
 * @version $Id:$
 *
 * @brief A unit test suite of Command
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
#include <vector>
#include "../include/kjsd/command.hpp"

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

/**
 * @class TEST_Command
 *
 * @brief Commandクラスのユニットテストスイート
 *
 * @see Command
 */
// CppUnit::TestFixtureを継承させる
class TEST_Command : public CppUnit::TestFixture
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

    //---> ここからフレームワークのお約束事

    /// テストスイート宣言開始マクロ．このクラス名を指定する．
    CPPUNIT_TEST_SUITE(TEST_Command);

    /// 以下テストケース宣言マクロ．テスト実行時にはこの順番で呼ばれる．
    /// あるテストケースで別機能を使う必要がある場合が考えられるから，
    /// 単体で完結する機能からテストされる順番になるよう注意する．
	CPPUNIT_TEST(test_exec);

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

	void test_exec();

private:
	Foo f_;
	Bar b_;
};

/// このテストスイートをCppUnitへ登録
CPPUNIT_TEST_SUITE_REGISTRATION(TEST_Command);

void TEST_Command::setUp()
{
}

void TEST_Command::tearDown()
{
}

void TEST_Command::test_exec()
{
	cout << endl;

	Command<void()> c0;
	Command<int(int)> c1;
	Command<int(int, int)> c2(1, 2);
	Command<int(int, int, int)> c3(1, 2, 3);

	c0 = new InstanceFunction<Foo, void()>(&f_, &Foo::arg0);
	c0();
	c1 = new InstanceFunction<Foo, int(int)>(&f_, &Foo::arg1);
    c1.bind(1);
    CPPUNIT_ASSERT(c1() == 1);
	c2 = new InstanceFunction<Foo, int(int, int)>(&f_, &Foo::arg2);
    CPPUNIT_ASSERT(c2() == 3);
	c3 = new InstanceFunction<Foo, int(int, int, int)>(&f_, &Foo::arg3);
    CPPUNIT_ASSERT(c3() == 6);

	Command<int(int, int, int)> cc3;
    
    cc3();
    cc3 = c3;
    CPPUNIT_ASSERT(cc3() == 6);

	c0 = new InstanceFunction<Bar, void()>(&b_, &Bar::arg0);
	c0();
	c1 = new InstanceFunction<Bar, int(int)>(&b_, &Bar::arg1);
	c1.bind(11);
    CPPUNIT_ASSERT(c1() == 11);
	c2 = new InstanceFunction<Bar, int(int,int)>(&b_, &Bar::arg2);
	c2.bind(11, 22);
    CPPUNIT_ASSERT(c2() == 33);
	c3 = new InstanceFunction<Bar, int(int,int,int)>(&b_, &Bar::arg3);
	c3.bind(11, 22, 33);
    CPPUNIT_ASSERT(c3() == 66);

	c0 = new StaticFunction<void()>(&static_arg0);
	c0();
	c1 = new StaticFunction<int(int)>(&static_arg1);
	c1.bind(111);
    CPPUNIT_ASSERT(c1() == 111);
	c2 = new StaticFunction<int(int,int)>(&static_arg2);
	c2.bind(111, 222);
    CPPUNIT_ASSERT(c2() == 333);
	c3 = new StaticFunction<int(int,int,int)>(&static_arg3);
	c3.bind(111, 222, 333);
    CPPUNIT_ASSERT(c3() == 666);
}
