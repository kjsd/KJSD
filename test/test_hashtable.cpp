/**
 * @file test_hashtable.cpp
 *
 * @version $Id: 1dfac53cdf1068d28f0ab483695c256624b91bf3 $
 *
 * @brief A unit test suite of HashTable
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
#include <cstdlib>
#include "../include/kjsd/timer.hpp"
#include "../include/kjsd/hash_table.hpp"

using namespace std;
using namespace kjsd;

/**
 * @class TEST_HashTable
 *
 * @brief HashTableクラスのユニットテストスイート
 *
 * @see HashTable
 */
// CppUnit::TestFixtureを継承させる
class TEST_HashTable : public CppUnit::TestFixture
{
    //---> ここからフレームワークのお約束事

    /// テストスイート宣言開始マクロ．このクラス名を指定する．
    CPPUNIT_TEST_SUITE(TEST_HashTable);

    /// 以下テストケース宣言マクロ．テスト実行時にはこの順番で呼ばれる．
    /// あるテストケースで別機能を使う必要がある場合が考えられるから，
    /// 単体で完結する機能からテストされる順番になるよう注意する．
    CPPUNIT_TEST(test_insert);
    CPPUNIT_TEST(test_size);
    CPPUNIT_TEST(test_end);
    CPPUNIT_TEST(test_empty);
    CPPUNIT_TEST(test_find_all);
    CPPUNIT_TEST(test_find_one);
    CPPUNIT_TEST(test_erase);
    CPPUNIT_TEST(test_clear);
    CPPUNIT_TEST(test_iterator);
    CPPUNIT_TEST(test_operator);
    CPPUNIT_TEST(test_bucket_count);
    CPPUNIT_TEST(test_rehash);
    CPPUNIT_TEST(test_count);


    /// テストスイート宣言終了マクロ
    CPPUNIT_TEST_SUITE_END();

public:
    /**
     * @brief 各テストケース実行前に呼ばれるコールバック．
     * 通常テストケースに共通して必要なリソースを全て確保する．
     * 今回の場合HashTableオブジェクトをひとつ作成して，
     * 充分な量の初期データをinsertしておく．
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

    /// テスト対象オブジェクト
    HashTable<int, int> *htv_;
    HashTable<string, int> *htc_;
    HashTable<const char*, int> *htp_;

    /// ハッシュテーブルは速度が重要なので，各テストケース時間計測用に使う
    Timer t_;

    static const int NUM_OF_TESTELEMENT = 50000;

    char* htp_tbl_[NUM_OF_TESTELEMENT];

    /**
     * @brief HashTable::insert()のテスト．
     * 返り値がvoidなのでNUM_OF_TESTELEMENT分挿入する間に
     * 例外が起きたり落ちたりしなければOKとする．
     * 挿入データはチェックし易いようキーと値が同じになるようにする．
     */
    void test_insert();

    /**
     * @brief HashTable::size()のテスト．
     * 返り値がNUM_OF_TESTELEMENTになればOK
     */
    void test_size();

    /**
     * @brief HashTable::end()のテスト．
     * 無効な値を検索した返り値が end() と一致すればOK
     */
    void test_end();

    /**
     * HashTable::empty()のテスト．
     * ht_の返り値はfalse，かつ，
     * 新規に作ったオブジェクトでの返り値はtrueならOK
     */
    void test_empty();

    /**
     * @brief HashTable::find()のテスト．
     * NUM_OF_TESTELEMENT回ランダム検索して全て想定通りの値が取得できればOK
     */
    void test_find_all();

    /**
     * @brief HashTable::find()の性能テスト．
     * 1回ランダム検索して想定通りの値が取得できればOK．時間にも注目．
     * データ量に依らず検索時間はO(1)に近くなるはず．
     */
    void test_find_one();

    /**
     * @brief HashTable::erase()のテスト．
     * NUM_OF_TESTELEMENT回削除して個数が1づつ減っていく，かつ，
     * 削除した値で検索して無効な値が返ってこればOK．
     */
    void test_erase();

    /**
     * @brief HashTable::clear()のテスト．
     * 実行後データが空になればOK
     */
    void test_clear();

    void test_iterator();
    void test_operator();
    void test_bucket_count();
    void test_rehash();
    void test_count();

private:
    void find(int num);
};

/// このテストスイートをCppUnitへ登録
CPPUNIT_TEST_SUITE_REGISTRATION(TEST_HashTable);


void TEST_HashTable::setUp()
{
    htv_ = new HashTable<int, int>;
    htc_ = new HashTable<string, int>(NUM_OF_TESTELEMENT);
    htp_ = new HashTable<const char*, int>(NUM_OF_TESTELEMENT);

#ifdef TEST_SPEED
    cout << endl;
    t_.start();
#endif
    for (int i = 0; i < NUM_OF_TESTELEMENT; i++)
    {
        htv_->insert(make_pair(i, i));
    }
#ifdef TEST_SPEED
    t_.check("Insertd(int)");
    t_.restart();
#endif

    ostringstream sstr;
    for (int i = 0; i < NUM_OF_TESTELEMENT; i++)
    {
        sstr.str("");
        sstr << i;
        htc_->insert(make_pair(sstr.str(), i));
    }
#ifdef TEST_SPEED
    t_.check("Insertd(string)");
    t_.restart();
#endif

    for (int i = 0; i < NUM_OF_TESTELEMENT; i++)
    {
        sstr.str("");
        sstr << i;

        string::size_type len = sstr.str().size();
        htp_tbl_[i] = new char[len + 1];
        memset(htp_tbl_[i], '\0', len + 1);
        sstr.str().copy(htp_tbl_[i], len);
        
        htp_->insert(make_pair(htp_tbl_[i], i));
    }

#ifdef TEST_SPEED
    t_.check("Insertd(char*)");
    t_.restart();
#endif
}

void TEST_HashTable::tearDown()
{
#ifdef TEST_SPEED
    t_.check("Finished.");
    t_.stop();
#endif
    delete htv_;
    delete htc_;
    delete htp_;

    for (int i = 0; i < NUM_OF_TESTELEMENT; i++)
    {
        delete[] htp_tbl_[i];
    }
}

void TEST_HashTable::test_insert()
{
    // A setUp() is same thing.
}

void TEST_HashTable::test_size()
{
    // サイズ値が想定どおりの値か？
    CPPUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htv_->size()));
    CPPUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htc_->size()));
    CPPUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htp_->size()));
}

void TEST_HashTable::test_end()
{
    HashTable<int, int>::iterator iti = htv_->find(NUM_OF_TESTELEMENT + 1);
    // 無効なキーを検索したときに end() が返ってきてるか？
    CPPUNIT_ASSERT(iti == htv_->end());

    HashTable<string, int>::iterator its = htc_->find("");
    CPPUNIT_ASSERT(its == htc_->end());

    HashTable<const char*, int>::iterator itp = htp_->find("");
    CPPUNIT_ASSERT(itp == htp_->end());
}

void TEST_HashTable::test_find_all()
{
    find(NUM_OF_TESTELEMENT);
}

void TEST_HashTable::test_find_one()
{
    find(1);
}

void TEST_HashTable::find(int num)
{
    assert(num <= NUM_OF_TESTELEMENT);

    for (int i = 0; i < num; i++)
    {
        int key = static_cast<int>(rand() % num);
        HashTable<int, int>::iterator it = htv_->find(key);
      
        // 比較チェック用CppUnitマクロ
        // 両者のoperator==が真になればテストOK．偽ならテストNG
        CPPUNIT_ASSERT_EQUAL(key, (*it).first);
        CPPUNIT_ASSERT_EQUAL(key, (*it).second);

        ostringstream sstr;
        sstr << key;
        HashTable<string, int>::iterator its = htc_->find(sstr.str());

        CPPUNIT_ASSERT_EQUAL(sstr.str(), (*its).first);
        CPPUNIT_ASSERT_EQUAL(key, (*its).second);

        HashTable<const char*, int>::iterator itp = htp_->find(sstr.str().c_str());

        CPPUNIT_ASSERT(strcmp((*itp).first, sstr.str().c_str()) == 0);
        CPPUNIT_ASSERT_EQUAL(key, (*itp).second);
    }
}

void TEST_HashTable::test_erase()
{
    for (int i = 0; i < NUM_OF_TESTELEMENT; i++)
    {
        HashTable<int, int>::iterator it = htv_->find(i);

        htv_->erase(it);

        // 汎用チェック用CppUnitマクロ
        // 条件式が真になればテストOK．偽ならテストNG
        CPPUNIT_ASSERT(NUM_OF_TESTELEMENT - i - 1 ==
                       static_cast<int>(htv_->size()));

        // 削除済みのキーをもう一度検索して無効な値が返ってくるか？
        it = htv_->find(i);
        CPPUNIT_ASSERT(it == htv_->end());

        ostringstream sstr;
        sstr << i;
        HashTable<string, int>::iterator its = htc_->find(sstr.str());

        htc_->erase(its);
        CPPUNIT_ASSERT(NUM_OF_TESTELEMENT - i - 1 ==
                       static_cast<int>(htc_->size()));

        HashTable<const char*, int>::iterator itp = htp_->find(htp_tbl_[i]);

        htp_->erase(itp);
        CPPUNIT_ASSERT(NUM_OF_TESTELEMENT - i - 1 ==
                       static_cast<int>(htp_->size()));
    }
}

void TEST_HashTable::test_clear()
{
    htv_->clear();
    // データが空になっているか？
    CPPUNIT_ASSERT(htv_->empty());

    htc_->clear();
    CPPUNIT_ASSERT(htc_->empty());

    htp_->clear();
    CPPUNIT_ASSERT(htp_->empty());
}

void TEST_HashTable::test_empty()
{
    // setUp()が実行されているので空ではないはず？
    CPPUNIT_ASSERT(!(htv_->empty()));
    CPPUNIT_ASSERT(!(htc_->empty()));
    CPPUNIT_ASSERT(!(htp_->empty()));

    HashTable<int, int> tmp;
    // 新規作成オブジェクトなので空のはず？
    CPPUNIT_ASSERT(tmp.empty());

    HashTable<string, int> tmps;
    CPPUNIT_ASSERT(tmps.empty());

    HashTable<const char*, int> tmpp;
    CPPUNIT_ASSERT(tmpp.empty());
}

void TEST_HashTable::test_iterator()
{
    HashTable<int, int> tmp;
    size_t cnt = 0;

    for (HashTable<int, int>::iterator it = tmp.begin();
         it != tmp.end(); ++it)
    {
        ++cnt;
    }
    CPPUNIT_ASSERT(cnt == 0);

    cnt = 0;
    for (HashTable<int, int>::iterator it = htv_->begin();
         it != htv_->end(); ++it)
    {
        ++cnt;
    }
    CPPUNIT_ASSERT(cnt == NUM_OF_TESTELEMENT);

    cnt = 0;
    for (HashTable<string, int>::iterator it = htc_->begin();
         it != htc_->end(); ++it)
    {
        ++cnt;
    }
    CPPUNIT_ASSERT(cnt == NUM_OF_TESTELEMENT);

    cnt = 0;
    for (HashTable<const char*, int>::iterator it = htp_->begin();
         it != htp_->end(); ++it)
    {
        ++cnt;
    }
    CPPUNIT_ASSERT(cnt == NUM_OF_TESTELEMENT);

    HashTable<int, int>::iterator it = htv_->begin();
    int v = (*(it++)).second;
    CPPUNIT_ASSERT(v != (*it).second);

    v = (*(++it)).second;
    CPPUNIT_ASSERT(v == (*it).second);
}

void TEST_HashTable::test_operator()
{
    CPPUNIT_ASSERT((*htv_)[0] == 0);
    (*htv_)[0] = 100;
    CPPUNIT_ASSERT((*htv_)[0] == 100);
    CPPUNIT_ASSERT((*(htv_->find(0))).second == 100);
    CPPUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htv_->size()));

    CPPUNIT_ASSERT((*htc_)["0"] == 0);
    (*htc_)["0"] = 100;
    CPPUNIT_ASSERT((*htc_)["0"] == 100);
    CPPUNIT_ASSERT((*(htc_->find("0"))).second == 100);
    CPPUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htc_->size()));

    CPPUNIT_ASSERT((*htp_)["0"] == 0);
    (*htp_)["0"] = 100;
    CPPUNIT_ASSERT((*htp_)["0"] == 100);
    CPPUNIT_ASSERT((*(htp_->find("0"))).second == 100);
    CPPUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htp_->size()));

    const int i = NUM_OF_TESTELEMENT;
    CPPUNIT_ASSERT(htv_->find(i) == htv_->end());
    (*htv_)[i] = 100;
    CPPUNIT_ASSERT((*htv_)[i] == 100);
    CPPUNIT_ASSERT((*(htv_->find(i))).second == 100);
    CPPUNIT_ASSERT(NUM_OF_TESTELEMENT + 1 == static_cast<int>(htv_->size()));
}

void TEST_HashTable::test_bucket_count()
{
    HashTable<int, int> ht;
    CPPUNIT_ASSERT(ht.bucket_count()
                   == (HashTable<int, int>::DEFAULT_EXPECTED_SIZE));

    HashTable<int, int> ht1(4);
    CPPUNIT_ASSERT(ht1.bucket_count() == 5);
}

void TEST_HashTable::test_rehash()
{
    HashTable<int, int>::size_type sz = htv_->size();
    HashTable<int, int>::size_type cnt = htv_->bucket_count();

    htv_->rehash(cnt);
    CPPUNIT_ASSERT(htv_->bucket_count() == cnt);

    htv_->rehash(sz / 2);
    CPPUNIT_ASSERT(htv_->bucket_count() == cnt);

    htv_->rehash(sz * 2);
    CPPUNIT_ASSERT(htv_->bucket_count() >= sz * 2);
}

void TEST_HashTable::test_count()
{
    const int n = NUM_OF_TESTELEMENT;
    CPPUNIT_ASSERT(htv_->count(0) == 1);
    CPPUNIT_ASSERT(htv_->count(n) == 0);
}
