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
/// このヘッダは必ずincludeする
#include <cppunit/extensions/HelperMacros.h>

#include <cassert>
#include <iostream>
#include <cstring>
#include <kjsd/shared_ptr.hpp>

using namespace std;
using namespace kjsd;

/**
 * @class TEST_SharedPtr
 *
 * @brief SharedPtrクラスのユニットテストスイート
 *
 * @see SharedPtr
 */
// CppUnit::TestFixtureを継承させる
class TEST_SharedPtr : public CppUnit::TestFixture
{
    //---> ここからフレームワークのお約束事

    /// テストスイート宣言開始マクロ．このクラス名を指定する．
    CPPUNIT_TEST_SUITE(TEST_SharedPtr);

    /// 以下テストケース宣言マクロ．テスト実行時にはこの順番で呼ばれる．
    /// あるテストケースで別機能を使う必要がある場合が考えられるから，
    /// 単体で完結する機能からテストされる順番になるよう注意する．
    CPPUNIT_TEST(test_use_count);
    CPPUNIT_TEST(test_new);
    CPPUNIT_TEST(test_copy);
    CPPUNIT_TEST(test_astah);
    CPPUNIT_TEST(test_member);
    CPPUNIT_TEST(test_compare);

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

	struct Foo
	{
		int f_;
	};

    void test_use_count();
    void test_new();
    void test_copy();
    void test_astah();
    void test_member();
    void test_compare();

	int* int_p1_;
	int* int_p2_;
	char* ch_p1_;
	char* ch_p2_;
};

/// このテストスイートをCppUnitへ登録
CPPUNIT_TEST_SUITE_REGISTRATION(TEST_SharedPtr);

void TEST_SharedPtr::setUp()
{
	cout << endl;

	int_p1_ = new int(1);

	cout << "int_p1_: " << reinterpret_cast<size_t>(int_p1_) << endl;
}

void TEST_SharedPtr::tearDown()
{
}

void TEST_SharedPtr::test_use_count()
{
	SharedPtr<int> p1(int_p1_);
    CPPUNIT_ASSERT(p1.use_count() == 1);

	SharedPtr<int> p11(int_p1_);
    CPPUNIT_ASSERT(p1.use_count() == 2);
    CPPUNIT_ASSERT(p11.use_count() == 2);
}

void TEST_SharedPtr::test_astah()
{
	SharedPtr<int> p1(int_p1_);
    CPPUNIT_ASSERT(*p1 == 1);

	*p1 = 2;
    CPPUNIT_ASSERT(*p1 == 2);
}

void TEST_SharedPtr::test_new()
{
	SharedPtr<int> p1(int_p1_);
    CPPUNIT_ASSERT(*p1 == 1);

	SharedPtr<int> p = new int(3);
    CPPUNIT_ASSERT(*p == 3);

	SharedPtr<int> pp = p1;
    CPPUNIT_ASSERT(*pp == 1);
}

void TEST_SharedPtr::test_copy()
{
	SharedPtr<int> p1(int_p1_);
    CPPUNIT_ASSERT(*p1 == 1);
	SharedPtr<int> p(new int(3));
    CPPUNIT_ASSERT(*p == 3);

	p1 = p;
    CPPUNIT_ASSERT(p1.use_count() == 2);
    CPPUNIT_ASSERT(*p1 == 3);
    CPPUNIT_ASSERT(p.use_count() == 2);
    CPPUNIT_ASSERT(*p == 3);

	p1 = new int(1);
    CPPUNIT_ASSERT(*p1 == 1);

	p1 = int_p1_;
    CPPUNIT_ASSERT(*p1 == 1);
}

void TEST_SharedPtr::test_member()
{
	SharedPtr<Foo> p = new Foo();
	p->f_ = 3;
    CPPUNIT_ASSERT(p->f_ == 3);
    CPPUNIT_ASSERT((*p).f_ == 3);

	delete int_p1_;
}

void TEST_SharedPtr::test_compare()
{
	SharedPtr<int> p;
	SharedPtr<int> p1(int_p1_);

    CPPUNIT_ASSERT(p == 0);
    CPPUNIT_ASSERT(p != p1);

	p = p1;
    CPPUNIT_ASSERT(p == p1);
}
