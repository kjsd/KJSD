/**
 * @file ioserial.hpp
 *
 * @version $Id:$
 *
 * @brief This file contains the definitions of serial I/O streaming
 * modules.
 *
 * Usage:
 * @code
 * SerialStream sio("/dev/ttyS0");
 * char c;
 * sio >> c; // read char
 * sio << c; // write char
 * @endcode
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#ifndef KJSD_IOSERIAL_HPP
#define KJSD_IOSERIAL_HPP

#include "./features.h"

#if defined(KJSD_HAVE_TERMIOS)
#include <termios.h>
#elif defined(KJSD_HAVE_WIN32)
#include <windows.h>
#else
#error "Unsupported platform"
#endif

#include <iostream>

namespace kjsd
{
    class SerialBuffer : public std::streambuf
    {
    public:
        enum BaudRate
        {
            BR_110,
            BR_300,
            BR_600,
            BR_1200,
            BR_2400,
            BR_4800,
            BR_9600,
            BR_19200,
            BR_38400,
            BR_57600,
            BR_115200
        };

        enum CharSize
        {
            CS_5,
            CS_6,
            CS_7,
            CS_8
        };

        enum Parity
        {
            PR_NONE,
            PR_ODD,
            PR_EVEN
        };

        enum StopBit
        {
            SB_1,
            SB_2
        };

        enum FlowControl
        {
            FC_NONE,
            FC_XONOFF,
            FC_HARDWARE
        };

        SerialBuffer(const char* name,
                     SerialBuffer::BaudRate br, SerialBuffer::CharSize cs,
                     SerialBuffer::Parity pr, SerialBuffer::StopBit sb,
                     SerialBuffer::FlowControl fc);
        virtual ~SerialBuffer();

        operator bool() const;

    protected:
        virtual int_type underflow();
        virtual int_type overflow(int_type c = traits_type::eof());
        virtual int_type sync();

    private:
        char_type buf_[1];
#ifdef KJSD_HAVE_TERMIOS
        int fd_;
        struct termios oldtio_;
#endif
#ifdef KJSD_HAVE_WIN32
        HANDLE fd_; 
#endif

        bool read_one(char* c);
        void write_one(char c);

        char_type* begin()
        {
            return buf_;
        }

        char_type* end()
        {
            return &buf_[1];
        }
    };

    class SerialStream : public std::iostream
    {
    public:
        SerialStream(const char* name,
                     SerialBuffer::BaudRate br = SerialBuffer::BR_115200,
                     SerialBuffer::CharSize cs = SerialBuffer::CS_8,
                     SerialBuffer::Parity pr = SerialBuffer::PR_NONE,
                     SerialBuffer::StopBit sb = SerialBuffer::SB_1,
                     SerialBuffer::FlowControl fc = SerialBuffer::FC_NONE) :
            std::iostream(new SerialBuffer(name, br, cs, pr, sb, fc))
        {
            unsetf(std::ios::skipws);
        }

        virtual ~SerialStream()
        {
            delete rdbuf();
        }

        operator bool() const
        {
            return *(static_cast<SerialBuffer*>(rdbuf()));
        }
    };
}

#endif // KJSD_IOSERIAL_HPP
