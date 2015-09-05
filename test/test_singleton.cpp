/**
 * @file test_singleton.cpp
 *
 * @version $Id: 1dfac53cdf1068d28f0ab483695c256624b91bf3 $
 *
 * @brief A unit test suite of Singleton
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
#include <cstring>
#include "../include/kjsd/singleton.hpp"

using namespace std;
using namespace kjsd;

/**
 * @class TEST_Singleton
 *
 * @brief Singletonクラスのユニットテストスイート
 *
 * @see Singleton
 */
// CppUnit::TestFixtureを継承させる
class TEST_Singleton : public CppUnit::TestFixture
{
    //---> ここからフレームワークのお約束事

    /// テストスイート宣言開始マクロ．このクラス名を指定する．
    CPPUNIT_TEST_SUITE(TEST_Singleton);

    /// 以下テストケース宣言マクロ．テスト実行時にはこの順番で呼ばれる．
    /// あるテストケースで別機能を使う必要がある場合が考えられるから，
    /// 単体で完結する機能からテストされる順番になるよう注意する．
    CPPUNIT_TEST(test_new);

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

    class SingletonImpl : public Singleton<SingletonImpl>
    {
    public:
        int aaa;
    private:
        friend class Singleton<SingletonImpl>;
        SingletonImpl() : aaa(10) {}
        ~SingletonImpl() {}
    };

    void test_new();
};

/// このテストスイートをCppUnitへ登録
CPPUNIT_TEST_SUITE_REGISTRATION(TEST_Singleton);

void TEST_Singleton::setUp()
{
}

void TEST_Singleton::tearDown()
{
}

void TEST_Singleton::test_new()
{
    // new SingletonImpl();
    SingletonImpl* ins = SingletonImpl::instance();

    CPPUNIT_ASSERT(ins != 0);
    CPPUNIT_ASSERT(ins == SingletonImpl::instance());
    CPPUNIT_ASSERT(ins->aaa == 10);

    // delete ins;
}
