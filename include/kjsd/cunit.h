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

/** 
 * @brief テスト関数型
 *
 * UTEST_runに与えるテスト関数の型
 */
typedef char* (*KJSD_CUNIT_Func)();

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
#define KJSD_CUNIT_ASSERT(message, test)            \
    do { if (!(test)) return message; } while (0)

/** 
 * @brief 比較一致評価
 *
 * 指定されたふたつの値が一致かどうかの評価を行う
 *
 * @param[in] message 比較が偽のときのメッセージ
 * @param[in] x 評価式1
 * @param[in] y 評価式2
 */
#define KJSD_CUNIT_ASSERT_EQUALS(message, x, y)         \
    do { if ((x) != (y)) return message; } while (0)

/** 
 * @brief 比較不一致評価
 *
 * 指定されたふたつの値が不一致かどうかの評価を行う
 *
 * @param[in] message 比較が真のときのメッセージ
 * @param[in] x 評価式1
 * @param[in] y 評価式2
 */
#define KJSD_CUNIT_ASSERT_NOT_EQUALS(message, x, y)     \
    do { if ((x) == (y)) return message; } while (0)

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
        char *message = (test)();                   \
        KJSD_CUNIT_run_count++;                     \
        if (message) return message;                \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* KJSD_CUNIT_H_ */
