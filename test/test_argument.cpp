/**
 * @file test_argument.cpp
 *
 * @version $Id: 1dfac53cdf1068d28f0ab483695c256624b91bf3 $
 *
 * @brief A unit test suite of Argument
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
#include "../include/kjsd/argument.hpp"

using namespace std;
using namespace kjsd;

/**
 * @class TEST_Argument
 *
 * @brief Argumentクラスのユニットテストスイート
 *
 * @see Argument
 */
// CppUnit::TestFixtureを継承させる
class TEST_Argument : public CppUnit::TestFixture
{
    //---> ここからフレームワークのお約束事

    /// テストスイート宣言開始マクロ．このクラス名を指定する．
    CPPUNIT_TEST_SUITE(TEST_Argument);

    /// 以下テストケース宣言マクロ．テスト実行時にはこの順番で呼ばれる．
    /// あるテストケースで別機能を使う必要がある場合が考えられるから，
    /// 単体で完結する機能からテストされる順番になるよう注意する．
    CPPUNIT_TEST(test_arg);
    CPPUNIT_TEST(test_long);
    CPPUNIT_TEST(test_short);
    CPPUNIT_TEST(test_complex);

    /// テストスイート宣言終了マクロ
    CPPUNIT_TEST_SUITE_END();

public:
    /**
     * @brief 各テストケース実行前に呼ばれるコールバック．
     * 通常テストケースに共通して必要なリソースを全て確保する．
     * 今回の場合Argumentオブジェクトをひとつ作成して，
     * 充分な量の初期データをstoreしておく．
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

    void test_arg();
    void test_long();
    void test_short();
    void test_complex();

    static const char* TEST_ARGV0;
};

/// このテストスイートをCppUnitへ登録
CPPUNIT_TEST_SUITE_REGISTRATION(TEST_Argument);

const char* TEST_Argument::TEST_ARGV0;

void TEST_Argument::setUp()
{
}

void TEST_Argument::tearDown()
{
}

void TEST_Argument::test_arg()
{
    const char *opt = "aaa";
    const int argc = 2;
    const char* argv[] = { TEST_ARGV0, opt };

    Argument arg(argc, argv);

    Argument::iterator it = arg.begin();
    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::ARG);
    CPPUNIT_ASSERT_EQUAL(*it, '\0');
    CPPUNIT_ASSERT(strcmp(it.argument(), opt) == 0);
}

void TEST_Argument::test_long()
{
    const char* opt = "--aaa";
    const char* optarg = "aaa";
    const int argc = 2;
    const char* argv[] = { TEST_ARGV0, opt };

    Argument arg(argc, argv);

    Argument::iterator it = arg.begin();
    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::LONG);
    CPPUNIT_ASSERT_EQUAL(*it, '\0');
    CPPUNIT_ASSERT(strcmp(it.argument(), optarg) == 0);
}

void TEST_Argument::test_short()
{
    const char* opt = "-a";
    const int argc = 2;
    const char* argv[] = { TEST_ARGV0, opt };

    Argument arg(argc, argv);

    Argument::iterator it = arg.begin();
    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    CPPUNIT_ASSERT_EQUAL(*it, 'a');
    CPPUNIT_ASSERT(strcmp(it.argument(), opt) == 0);
}

void TEST_Argument::test_complex()
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

    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    CPPUNIT_ASSERT_EQUAL(*it, 'a');

    ++it;
    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    CPPUNIT_ASSERT_EQUAL(*it, 'b');
    
    ++it;
    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    CPPUNIT_ASSERT_EQUAL(*it, 'c');

    ++it;
    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::SHORT);
    CPPUNIT_ASSERT_EQUAL(*it, 'd');

    ++it;
    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::ARG);
    CPPUNIT_ASSERT(strcmp(it.argument(), "eee") == 0);

    ++it;
    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::LONG);
    CPPUNIT_ASSERT(strcmp(it.argument(), "ffff") == 0);

    ++it;
    CPPUNIT_ASSERT_EQUAL(it.status(), Argument::ARG);
    CPPUNIT_ASSERT(strcmp(it.argument(), "ggg") == 0);

    ++it;
    CPPUNIT_ASSERT(it == arg.end());
}
