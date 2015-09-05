/**
 * @file test_util.cpp
 *
 * @version $Id: 1dfac53cdf1068d28f0ab483695c256624b91bf3 $
 *
 * @brief A unit test suite of Util
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
#include "../include/kjsd/util.hpp"
#include "../include/kjsd/cutil.h"

using namespace std;
using namespace kjsd;

/**
 * @class TEST_Util
 *
 * @brief Utilクラスのユニットテストスイート
 *
 * @see Util
 */
// CppUnit::TestFixtureを継承させる
class TEST_Util : public CppUnit::TestFixture
{
    //---> ここからフレームワークのお約束事

    /// テストスイート宣言開始マクロ．このクラス名を指定する．
    CPPUNIT_TEST_SUITE(TEST_Util);

    /// 以下テストケース宣言マクロ．テスト実行時にはこの順番で呼ばれる．
    /// あるテストケースで別機能を使う必要がある場合が考えられるから，
    /// 単体で完結する機能からテストされる順番になるよう注意する．
    CPPUNIT_TEST(test_sleep);

    /// テストスイート宣言終了マクロ
    CPPUNIT_TEST_SUITE_END();

public:
    /**
     * @brief 各テストケース実行前に呼ばれるコールバック．
     * 通常テストケースに共通して必要なリソースを全て確保する．
     * 今回の場合Utilオブジェクトをひとつ作成して，
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

    void test_sleep();
};

/// このテストスイートをCppUnitへ登録
CPPUNIT_TEST_SUITE_REGISTRATION(TEST_Util);

void TEST_Util::setUp()
{
}

void TEST_Util::tearDown()
{
}

void TEST_Util::test_sleep()
{
    cout << endl;

    cout << "sleep 11ms..." << flush;
    KJSD_CUTIL_MSLEEP(11);
    cout << "done." << endl;

    cout << "sleep 111ms..." << flush;
    KJSD_CUTIL_MSLEEP(111);
    cout << "done." << endl;

    cout << "sleep 1111ms..." << flush;
    KJSD_CUTIL_MSLEEP(1111);
    cout << "done." << endl;

    cout << "sleep 5111ms..." << flush;
    KJSD_CUTIL_MSLEEP(5111);
    cout << "done." << endl;

    cout << "sleep 11111ms..." << flush;
    KJSD_CUTIL_MSLEEP(11111);
    cout << "done." << endl;
}
