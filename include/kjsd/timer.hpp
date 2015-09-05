/**
 * @file timer.hpp
 *
 * @version $Id:$
 *
 * @brief Consists of a single header file timer.h that can be included
 * in any code that needs to compute the execution time of parts of a
 * program.
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#ifndef KJSD_TIMER_HPP
#define KJSD_TIMER_HPP

#include "./features.h"

#include <iostream>
#include <iomanip>

#if defined(KJSD_HAVE_POSIX_REALTIME_EXTENSION)
#include <time.h>
#elif defined(KJSD_HAVE_WIN32)
#include <windows.h>
#elif defined(KJSD_HAVE_MACH)
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <ctime>
#endif

namespace kjsd
{
    /**
     * @class Timer
     *
     * @brief A simple C++ class that is useful for timing how long it takes
     * to run.
     * A timer needs to be explicitly started using 'start' or
     * 'restart'.
     */
    class Timer
    {
        /**
         * @brief Allow timers to be printed to ostreams using the
         * syntax 'os << t' for an ostream 'os' and a timer 't'.
         * For example, "cout << t" will print out the total
         * amount of time 't' has been "running".
         */
        friend std::ostream &operator<<(std::ostream &os, Timer &t);

    private:
        bool running_;
        double start_clock_;
        time_t start_time_;
        double acc_time_;

    public:
        /**
         * @brief Return the total time that the timer has been in
         * the "running" state since it was first "started" or
         * last "restarted".
         */
        double elapsed_time();

        /**
         * @brief This is the wrapper to return the current time
         * for some method of time measurement.
         */
        double my_clock();

        /**
         * @brief A constructor. Timer::running is initially false.
         */
        Timer() :
            running_(false), start_clock_(0), start_time_(0), acc_time_(0)
        {}

        /**
         * @brief Start a timer.  If it is already running, let it
         * continue running. Print an optional message.
         */
        void start(const char *msg = 0);

        /**
         * @brief Turn the timer off and start it again from 0.
         * Print an optional message.
         */
        void restart(const char *msg = 0);

        /**
         * @brief Stop the timer and print an optional message.
         */
        void stop(const char *msg = 0);

        /**
         * @brief Print out an optional message followed by the
         * current timer timing.
         */
        void check(const char *msg = 0);
    };

    inline double Timer::elapsed_time()
    {
        time_t acc_sec = time(0) - start_time_;
        if (acc_sec < 3600)
            return (my_clock() - start_clock_);
        else
            return (1.0 * acc_sec);
    }

    inline double Timer::my_clock()
    {
#if defined(KJSD_HAVE_POSIX_REALTIME_EXTENSION) || defined(KJSD_HAVE_MACH)
        struct timespec ts;
#ifdef KJSD_HAVE_MACH
        clock_serv_t cclock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
        clock_get_time(cclock, &mts);
        mach_port_deallocate(mach_task_self(), cclock);
        ts.tv_sec = mts.tv_sec;
        ts.tv_nsec = mts.tv_nsec;
#else
        clock_gettime(CLOCK_REALTIME, &ts);
#endif // KJSD_HAVE_MACH
        return ts.tv_sec + static_cast<double>(ts.tv_nsec) * 1e-9;
#elif defined(KJSD_HAVE_WIN32)
        LARGE_INTEGER freq;
        LARGE_INTEGER count;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&count);
        return static_cast<double>(count.QuadPart) / freq.QuadPart;
#else
        return static_cast<double>(clock()) / CLOCKS_PER_SEC;
#endif
    }

    inline void Timer::start(const char *msg)
    {
        // Print an optional message, something like "Starting timer t";
        if (msg) std::cout << msg << std::endl;

        // Return immediately if the timer is already running
        if (running_) return;

        // Set timer status to running and set the start time
        running_ = true;
        start_clock_ = my_clock();
        start_time_ = time(0);
    }

    inline void Timer::restart(const char *msg)
    {
        // Print an optional message, something like "Restarting timer t";
        if (msg) std::cout << msg << std::endl;

        // Set timer status to running, reset accumulated time,
        // and set start time
        running_ = true;
        acc_time_ = 0;
        start_clock_ = my_clock();
        start_time_ = time(0);
    }

    inline void Timer::stop(const char *msg)
    {
        // Print an optional message, something like "Stopping timer t";
        if (msg) std::cout << msg << std::endl;

        // Compute accumulated running time and set timer status
        // to not running
        if (running_) acc_time_ += elapsed_time();
        running_ = false;
    }

    inline void Timer::check(const char *msg)
    {
        // Print an optional message, something like "Checking timer t";
        if (msg) std::cout << msg << " : ";

        std::cout << "Elapsed time [" << std::setiosflags(std::ios::fixed)
                  << std::setprecision(6)
                  << acc_time_ + (running_ ? elapsed_time() : 0)
                  << "] second\n";
    }

    inline std::ostream &operator<<(std::ostream &os, Timer &t)
    {
        os << std::setprecision(6) << std::setiosflags(std::ios::fixed)
           << t.acc_time_ + (t.running_ ? t.elapsed_time() : 0);
        return os;
    }
}

#endif // KJSD_TIMER_HPP
