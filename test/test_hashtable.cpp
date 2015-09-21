/**
 * @file test_hashtable.cpp
 *
 * @brief A unit test suite of HashTable
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <kjsd/cunit.h>
#include <kjsd/cutil.h>
#include <kjsd/timer.hpp>
#include <kjsd/hash_table.hpp>

#define FIND(num)                                                       \
    do                                                                  \
    {                                                                   \
        assert((num) <= NUM_OF_TESTELEMENT);                            \
        for (int i = 0; i < (num); i++)                                 \
        {                                                               \
            int key = static_cast<int>(rand() % (num));                 \
            HashTable<int, int>::iterator it = htv_->find(key);         \
            KJSD_CUNIT_ASSERT_EQUAL(key, (*it).first);                  \
            KJSD_CUNIT_ASSERT_EQUAL(key, (*it).second);                 \
            ostringstream sstr;                                         \
            sstr << key;                                                \
            HashTable<string, int>::iterator its =                      \
                htc_->find(sstr.str());                                 \
            KJSD_CUNIT_ASSERT_EQUAL(sstr.str(), (*its).first);          \
            KJSD_CUNIT_ASSERT_EQUAL(key, (*its).second);                \
            HashTable<const char*, int>::iterator itp =                 \
                htp_->find(sstr.str().c_str());                         \
            KJSD_CUNIT_ASSERT(strcmp((*itp).first, sstr.str().c_str()) == 0); \
            KJSD_CUNIT_ASSERT_EQUAL(key, (*itp).second);                \
        }                                                               \
    }                                                                   \
    while(0)

using namespace std;
using namespace kjsd;


static HashTable<int, int> *htv_;
static HashTable<string, int> *htc_;
static HashTable<const char*, int> *htp_;

/// ハッシュテーブルは速度が重要なので，各テストケース時間計測用に使う
static Timer t_;

static const int NUM_OF_TESTELEMENT = 50000;

static char* htp_tbl_[NUM_OF_TESTELEMENT];

static void setUp()
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

static void tearDown()
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

static const char* test_insert()
{
    // A setUp() is same thing.
    return 0;
}

static const char* test_size()
{
    // サイズ値が想定どおりの値か？
    KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htv_->size()));
    KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htc_->size()));
    KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htp_->size()));
    return 0;
}

static const char* test_end()
{
    HashTable<int, int>::iterator iti = htv_->find(NUM_OF_TESTELEMENT + 1);
    // 無効なキーを検索したときに end() が返ってきてるか？
    KJSD_CUNIT_ASSERT(iti == htv_->end());

    HashTable<string, int>::iterator its = htc_->find("");
    KJSD_CUNIT_ASSERT(its == htc_->end());

    HashTable<const char*, int>::iterator itp = htp_->find("");
    KJSD_CUNIT_ASSERT(itp == htp_->end());
    return 0;
}

static const char* test_find_all()
{
    FIND(NUM_OF_TESTELEMENT);
    return 0;
}

static const char* test_find_one()
{
    FIND(1);
    return 0;
}

static const char* test_erase()
{
    for (int i = 0; i < NUM_OF_TESTELEMENT; i++)
    {
        HashTable<int, int>::iterator it = htv_->find(i);

        htv_->erase(it);

        // 汎用チェック用CppUnitマクロ
        // 条件式が真になればテストOK．偽ならテストNG
        KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT - i - 1 ==
                       static_cast<int>(htv_->size()));

        // 削除済みのキーをもう一度検索して無効な値が返ってくるか？
        it = htv_->find(i);
        KJSD_CUNIT_ASSERT(it == htv_->end());

        ostringstream sstr;
        sstr << i;
        HashTable<string, int>::iterator its = htc_->find(sstr.str());

        htc_->erase(its);
        KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT - i - 1 ==
                       static_cast<int>(htc_->size()));

        HashTable<const char*, int>::iterator itp = htp_->find(htp_tbl_[i]);

        htp_->erase(itp);
        KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT - i - 1 ==
                       static_cast<int>(htp_->size()));
    }
    return 0;
}

static const char* test_clear()
{
    htv_->clear();
    // データが空になっているか？
    KJSD_CUNIT_ASSERT(htv_->empty());

    htc_->clear();
    KJSD_CUNIT_ASSERT(htc_->empty());

    htp_->clear();
    KJSD_CUNIT_ASSERT(htp_->empty());
    return 0;
}

static const char* test_empty()
{
    // setUp()が実行されているので空ではないはず？
    KJSD_CUNIT_ASSERT(!(htv_->empty()));
    KJSD_CUNIT_ASSERT(!(htc_->empty()));
    KJSD_CUNIT_ASSERT(!(htp_->empty()));

    HashTable<int, int> tmp;
    // 新規作成オブジェクトなので空のはず？
    KJSD_CUNIT_ASSERT(tmp.empty());

    HashTable<string, int> tmps;
    KJSD_CUNIT_ASSERT(tmps.empty());

    HashTable<const char*, int> tmpp;
    KJSD_CUNIT_ASSERT(tmpp.empty());
    return 0;
}

static const char* test_iterator()
{
    HashTable<int, int> tmp;
    size_t cnt = 0;

    for (HashTable<int, int>::iterator it = tmp.begin();
         it != tmp.end(); ++it)
    {
        ++cnt;
    }
    KJSD_CUNIT_ASSERT(cnt == 0);

    cnt = 0;
    for (HashTable<int, int>::iterator it = htv_->begin();
         it != htv_->end(); ++it)
    {
        ++cnt;
    }
    KJSD_CUNIT_ASSERT(cnt == NUM_OF_TESTELEMENT);

    cnt = 0;
    for (HashTable<string, int>::iterator it = htc_->begin();
         it != htc_->end(); ++it)
    {
        ++cnt;
    }
    KJSD_CUNIT_ASSERT(cnt == NUM_OF_TESTELEMENT);

    cnt = 0;
    for (HashTable<const char*, int>::iterator it = htp_->begin();
         it != htp_->end(); ++it)
    {
        ++cnt;
    }
    KJSD_CUNIT_ASSERT(cnt == NUM_OF_TESTELEMENT);

    HashTable<int, int>::iterator it = htv_->begin();
    int v = (*(it++)).second;
    KJSD_CUNIT_ASSERT(v != (*it).second);

    v = (*(++it)).second;
    KJSD_CUNIT_ASSERT(v == (*it).second);
    return 0;
}

static const char* test_operator()
{
    KJSD_CUNIT_ASSERT((*htv_)[0] == 0);
    (*htv_)[0] = 100;
    KJSD_CUNIT_ASSERT((*htv_)[0] == 100);
    KJSD_CUNIT_ASSERT((*(htv_->find(0))).second == 100);
    KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htv_->size()));

    KJSD_CUNIT_ASSERT((*htc_)["0"] == 0);
    (*htc_)["0"] = 100;
    KJSD_CUNIT_ASSERT((*htc_)["0"] == 100);
    KJSD_CUNIT_ASSERT((*(htc_->find("0"))).second == 100);
    KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htc_->size()));

    KJSD_CUNIT_ASSERT((*htp_)["0"] == 0);
    (*htp_)["0"] = 100;
    KJSD_CUNIT_ASSERT((*htp_)["0"] == 100);
    KJSD_CUNIT_ASSERT((*(htp_->find("0"))).second == 100);
    KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT == static_cast<int>(htp_->size()));

    const int i = NUM_OF_TESTELEMENT;
    KJSD_CUNIT_ASSERT(htv_->find(i) == htv_->end());
    (*htv_)[i] = 100;
    KJSD_CUNIT_ASSERT((*htv_)[i] == 100);
    KJSD_CUNIT_ASSERT((*(htv_->find(i))).second == 100);
    KJSD_CUNIT_ASSERT(NUM_OF_TESTELEMENT + 1 == static_cast<int>(htv_->size()));
    return 0;
}

static const char* test_bucket_count()
{
    HashTable<int, int> ht;
    KJSD_CUNIT_ASSERT(ht.bucket_count()
                   == (HashTable<int, int>::DEFAULT_EXPECTED_SIZE));

    HashTable<int, int> ht1(4);
    KJSD_CUNIT_ASSERT(ht1.bucket_count() == 5);
    return 0;
}

static const char* test_rehash()
{
    HashTable<int, int>::size_type sz = htv_->size();
    HashTable<int, int>::size_type cnt = htv_->bucket_count();

    htv_->rehash(cnt);
    KJSD_CUNIT_ASSERT(htv_->bucket_count() == cnt);

    htv_->rehash(sz / 2);
    KJSD_CUNIT_ASSERT(htv_->bucket_count() == cnt);

    htv_->rehash(sz * 2);
    KJSD_CUNIT_ASSERT(htv_->bucket_count() >= sz * 2);
    return 0;
}

static const char* test_count()
{
    const int n = NUM_OF_TESTELEMENT;
    KJSD_CUNIT_ASSERT(htv_->count(0) == 1);
    KJSD_CUNIT_ASSERT(htv_->count(n) == 0);
    return 0;
}

const char* test_hashtable()
{
    const KJSD_CUNIT_Func f[] = {
        test_insert,
        test_size,
        test_end,
        test_empty,
        test_find_all,
        test_find_one,
        test_erase,
        test_clear,
        test_iterator,
        test_operator,
        test_bucket_count,
        test_rehash,
        test_count
    };

    for (int i = 0; i < KJSD_CUTIL_SIZEOFA(f); i++)
    {
        setUp();
        KJSD_CUNIT_RUN(f[i]);
        tearDown();
    }
    return 0;
}
