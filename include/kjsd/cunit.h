/**
 * @file cunit.h
 *
 * @version $Id:$
 *
 * @brief
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#ifndef KJSD_CUNIT_H
#define KJSD_CUNIT_H

#ifdef __cplusplus
extern "C" {
#endif

#define KJSD_CUNIT_L KJSD_CUNIT__(__LINE__)
#define KJSD_CUNIT__(x) KJSD_CUNIT___(x)
#define KJSD_CUNIT___(x) #x

#define KJSD_CUNIT_MSG(msg) "*** Error: " msg "[" __FILE__ "," KJSD_CUNIT_L "]"

/** 
 * @brief テスト関数型
 *
 * UTEST_runに与えるテスト関数の型
 */
typedef const char* (*KJSD_CUNIT_Func)();

/** 
 * @brief テスト回数パラメータ
 *
 * UTEST_runを実行する度にカウントアップする。テストモジュール本体で実
 * 装しておく。
 */
extern int KJSD_CUNIT_run_count;

/** 
 * @brief 条件評価
 *
 * 指定された条件が真になるかどうかの評価を行う
 *
 * @param[in] message 評価が偽のときのメッセージ
 * @param[in] test 評価式
 */
#define KJSD_CUNIT_ASSERT_M(message, test)                          \
    do { if (!(test)) return KJSD_CUNIT_MSG(message); } while (0)
#define KJSD_CUNIT_ASSERT(test) KJSD_CUNIT_ASSERT_M("", test)

/** 
 * @brief 比較一致評価
 *
 * 指定されたふたつの値が一致かどうかの評価を行う
 *
 * @param[in] message 比較が偽のときのメッセージ
 * @param[in] x 評価式1
 * @param[in] y 評価式2
 */
#define KJSD_CUNIT_ASSERT_EQUAL_M(message, x, y)                        \
    do { if ((x) != (y)) return KJSD_CUNIT_MSG(message); } while (0)
#define KJSD_CUNIT_ASSERT_EQUAL(x, y) KJSD_CUNIT_ASSERT_EQUAL_M("", x, y)

/** 
 * @brief 比較不一致評価
 *
 * 指定されたふたつの値が不一致かどうかの評価を行う
 *
 * @param[in] message 比較が真のときのメッセージ
 * @param[in] x 評価式1
 * @param[in] y 評価式2
 */
#define KJSD_CUNIT_ASSERT_NOT_EQUAL_M(message, x, y)                    \
    do { if ((x) == (y)) return KJSD_CUNIT_MSG(message); } while (0)
#define KJSD_CUNIT_ASSERT_NOT_EQUAL(x, y) KJSD_CUNIT_ASSERT_NOT_EQUAL_M("",x,y)

/** 
 * @brief テスト関数実行
 *
 * 指定されたテスト関数を実行し、失敗した場合はメッセージを返して呼び
 * 出し元関数を抜ける。
 *
 * @param[in] test 実行するテスト関数へのポインタ
 */
#define KJSD_CUNIT_RUN(test)                        \
    do {                                            \
        const char *message = (test)();             \
        KJSD_CUNIT_run_count++;                     \
        if (message) return message;                \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* KJSD_CUNIT_H_ */
