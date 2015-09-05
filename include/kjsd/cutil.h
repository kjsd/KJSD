/**
 * @file cutil.h
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
#ifndef KJSD_CUTIL_H
#define KJSD_CUTIL_H

#include "./features.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG

#define KJSD_CUTIL_ASSERT(x)
#define KJSD_CUTIL_SASSERT(x, msg)

#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
#define KJSD_CUTIL_DPRINTF(...)
#define KJSD_CUTIL_ASSERT_MSG(x, ...)
#else
#define KJSD_CUTIL_DPRINTF(msg)
#define KJSD_CUTIL_ASSERT_MSG(x, msg)
#endif

#else // NDEBUG

#include <stdio.h>
#include <stdlib.h>

#define KJSD_CUTIL_ASSERT(x)                                            \
    do {                                                                \
        if (!(x)) {                                                     \
            printf("Assertion false at %s:%d\n", __FILE__, __LINE__);   \
            abort();                                                    \
        }                                                               \
    } while (0)

#define KJSD_CUTIL_SASSERT(exp, msg) static char KJSD_##msg##__[(exp)?1:-1]

#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
#define KJSD_CUTIL_DPRINTF(...)                 \
    do {                                        \
        printf("[%s:%d] ", __FILE__, __LINE__); \
        printf(__VA_ARGS__);                    \
    } while (0)

#define KJSD_CUTIL_ASSERT_MSG(x, ...)                                   \
    do {                                                                \
        if (!(x)) {                                                     \
            printf("Assertion false at %s:%d: ", __FILE__, __LINE__);   \
            printf(__VA_ARGS__);                                        \
            abort();                                                    \
        }                                                               \
    } while (0)

#else

#define KJSD_CUTIL_DPRINTF(msg)                 \
    do {                                        \
        printf("[%s:%d] ", __FILE__, __LINE__); \
        printf msg;                             \
    } while (0)

#define KJSD_CUTIL_ASSERT_MSG(x, msg)                                   \
    do {                                                                \
        if (!(x)) {                                                     \
            printf("Assertion false at %s:%d: ", __FILE__, __LINE__);   \
            printf msg;                                                 \
            abort();                                                    \
        }                                                               \
    } while (0)

#endif
#endif // NDEBUG

/**
 * @brief 配列の要素数取得
 *
 * @param[in] x 取得する配列名
 *
 * @return 配列の要素数
 */
#define KJSD_CUTIL_SIZEOFA(x) (sizeof(x)/sizeof(x[0]))

/**
 * @brief 構造体メンバの要素へのオフセットを取得
 *
 * @param[in] type memberの構造体型タイプ
 * @param[in] member オフセットを取得するメンバ名
 *
 * @return memberまでのオフセット
 */
#define KJSD_CUTIL_OFFSETOF(type, member) ((_UWORD)&((type*)0)->member)

#define KJSD_DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

#if defined(KJSD_HAVE_POSIX_REALTIME_EXTENSION) || defined(KJSD_HAVE_MACH)
#include <time.h>
#include <errno.h>
#define KJSD_CUTIL_MSLEEP(ms)                               \
    do {                                                    \
        struct timespec req;                                \
        struct timespec rem = { 0, 0 };                     \
        req.tv_sec = (int)(ms) / 1000;                      \
        req.tv_nsec = (ms % 1000) * 1e+6;                   \
        for (; nanosleep(&req, &rem) == EINTR; req = rem);  \
    } while (0)
#elif defined(KJSD_HAVE_WIN32)
#include <windows.h>
#define KJSD_CUTIL_MSLEEP(ms) Sleep(ms)
#else
#include <unistd.h>
#define KJSD_CUTIL_MSLEEP(ms) sleep(KJSD_DIV_ROUND_UP(ms, 1000))
#endif

#ifdef __cplusplus
}
#endif

#endif /* KJSD_CUTIL_H */
