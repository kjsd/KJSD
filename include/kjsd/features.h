/**
 * @file features.h
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
#ifndef KJSD_FEATURES_H
#define KJSD_FEATURES_H

// Check feature POSIX_REALTIME_EXTENSION
#ifndef KJSD_HAVE_POSIX_REALTIME_EXTENSION
#ifdef __linux__
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif // _POSIX_C_SOURCE
#include <features.h>
#define KJSD_HAVE_POSIX_REALTIME_EXTENSION
#endif
#endif // KJSD_HAVE_POSIX_REALTIME_EXTENSION

// Check feature TERMIOS
#ifndef KJSD_HAVE_TERMIOS
#ifdef __linux__
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#include <features.h>
#define KJSD_HAVE_TERMIOS
#endif 

#ifdef __MACH__
#define KJSD_HAVE_TERMIOS
#endif // __MACH__

#ifdef __unix__
#define KJSD_HAVE_TERMIOS
#endif
#endif // KJSD_HAVE_TERMIOS

// Check feature WIN32
#ifndef KJSD_HAVE_WIN32
#ifdef _WIN32
#define KJSD_HAVE_WIN32
#endif // _WIN32
#endif // KJSD_HAVE_WIN32

// Check feature MACH
#ifndef KJSD_HAVE_MACH
#ifdef __MACH__
#define KJSD_HAVE_MACH
#endif // __MACH__
#endif // KJSD_HAVE_MACH

#endif // KJSD_FEATURES_H
