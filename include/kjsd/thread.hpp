/**
 * @file thread.hpp
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
#ifndef KJSD_THREAD_HPP
#define KJSD_THREAD_HPP

#if !defined(HAVE_PTHREAD) && !defined(HAVE_WIN32)
#error "Should be defined either HAVE_PTHREAD or HAVE_WIN32"
#endif

#if defined(HAVE_PTHREAD)
#include <pthread.h>
#elif defined(HAVE_WIN32)
#include <windows.h>
#endif

#if defined(__GNUC__)
#define KJSD_THREAD_VAR __thread
#elif defined(_MSC_VER)
#define KJSD_THREAD_VAR __declspec( thread )
#endif

namespace kjsd
{
    class Thread
    {
    public:
        Thread();
        virtual ~Thread();

        bool start();
        void join();

	protected:
        bool is_self() const;
		operator bool() const;

    private:
        virtual void run() = 0;

#if defined(HAVE_PTHREAD)
        pthread_t dsc_;
        static void* entry(void* arg);
#elif defined(HAVE_WIN32)
        HANDLE dsc_;
        UINT32 id_;
        static unsigned __stdcall entry(void* arg);
#endif
    };
}

#endif // KJSD_THREAD_HPP
