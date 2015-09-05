/**
 * @file mutex.hpp
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
#ifndef KJSD_MUTEX_HPP
#define KJSD_MUTEX_HPP

#if !defined(HAVE_PTHREAD) && !defined(HAVE_WIN32)
#error "Should be defined either HAVE_PTHREAD or HAVE_WIN32"
#endif

#if defined(HAVE_PTHREAD)
#include <pthread.h>
#elif defined(HAVE_WIN32)
#include <windows.h>
#endif

namespace kjsd
{
    class Mutex
    {
    public:
        Mutex();
        virtual ~Mutex();

        void lock();
        void unlock();

    private:
#if defined(HAVE_PTHREAD)
        pthread_mutex_t mtx_;
#elif defined(HAVE_WIN32)
        CRITICAL_SECTION *cts_;
#endif
    };
}


#endif /* KJSD_MUTEX_HPP */
