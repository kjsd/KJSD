/**
 * @file ioserial.cpp
 *
 * @version $Id:$
 *
 * @brief This file contains the implementation of serial I/O
 * streaming modules.
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include "../include/kjsd/ioserial.hpp"

#ifdef KJSD_HAVE_WIN32
#include <windows.h>
#endif // KJSD_HAVE_WIN32
#ifdef KJSD_HAVE_TERMIOS
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif // KJSD_HAVE_TERMIOS

#include <cstring>

using namespace std;
using namespace kjsd;

namespace kjsd
{
#ifdef KJSD_HAVE_TERMIOS
	static inline speed_t get_baud(SerialBuffer::BaudRate br)
	{
		switch (br)
		{
		case SerialBuffer::BR_110:
			return B110;
		case SerialBuffer::BR_300:
			return B300;
		case SerialBuffer::BR_600:
			return B600;
		case SerialBuffer::BR_1200:
			return B1200;
		case SerialBuffer::BR_2400:
			return B2400;
		case SerialBuffer::BR_4800:
			return B4800;
		case SerialBuffer::BR_9600:
			return B9600;
		case SerialBuffer::BR_19200:
			return B19200;
		case SerialBuffer::BR_38400:
			return B38400;
		case SerialBuffer::BR_57600:
			return B57600;
		case SerialBuffer::BR_115200:
			return B115200;
		default:
			return B115200;
		}
	}

	static inline tcflag_t get_char_size(SerialBuffer::CharSize cs)
	{
		switch (cs)
		{
		case SerialBuffer::CS_5:
			return CS5;
		case SerialBuffer::CS_6:
			return CS6;
		case SerialBuffer::CS_7:
			return CS7;
		case SerialBuffer::CS_8:
			return CS8;
		default:
			return CS8;
		}
	}
#endif // KJSD_HAVE_TERMIOS

#ifdef KJSD_HAVE_WIN32
	static inline DWORD get_baud(SerialBuffer::BaudRate br)
	{
		switch (br)
		{
		case SerialBuffer::BR_110:
			return CBR_110;
		case SerialBuffer::BR_300:
			return CBR_300;
		case SerialBuffer::BR_600:
			return CBR_600;
		case SerialBuffer::BR_1200:
			return CBR_1200;
		case SerialBuffer::BR_2400:
			return CBR_2400;
		case SerialBuffer::BR_4800:
			return CBR_4800;
		case SerialBuffer::BR_9600:
			return CBR_9600;
		case SerialBuffer::BR_19200:
			return CBR_19200;
		case SerialBuffer::BR_38400:
			return CBR_38400;
		case SerialBuffer::BR_57600:
			return CBR_57600;
		case SerialBuffer::BR_115200:
			return CBR_115200;
		default:
			return CBR_115200;
		}
	}

	static inline BYTE get_char_size(SerialBuffer::CharSize cs)
	{
		switch (cs)
		{
		case SerialBuffer::CS_5:
			return 5;
		case SerialBuffer::CS_6:
			return 6;
		case SerialBuffer::CS_7:
			return 7;
		case SerialBuffer::CS_8:
			return 8;
		default:
			return 8;
		}
	}
#endif // KJSD_HAVE_WIN32
}

SerialBuffer::SerialBuffer(const char* name,
						   SerialBuffer::BaudRate br,
						   SerialBuffer::CharSize cs,
						   SerialBuffer::Parity pr,
						   SerialBuffer::StopBit sb,
						   SerialBuffer::FlowControl fc)
#ifdef KJSD_HAVE_WIN32
{
	setg(begin(), end(), end());

    fd_ = CreateFileA(reinterpret_cast<LPCSTR>(name),
                      GENERIC_READ | GENERIC_WRITE, 0, NULL, 
                      OPEN_EXISTING, 0, NULL);
    if (fd_ == INVALID_HANDLE_VALUE) return;

    DCB comdcb;
    COMMTIMEOUTS comtout;

    ZeroMemory(&comdcb, sizeof(DCB));
    comdcb.DCBlength = sizeof(DCB);
    comdcb.wReserved = 0;

    comdcb.BaudRate = get_baud(br);
    comdcb.fParity = (pr == SerialBuffer::PR_NONE) ? FALSE: TRUE;
	switch (pr)
	{
	case SerialBuffer::PR_EVEN:
		comdcb.Parity = EVENPARITY;
		break;
	case SerialBuffer::PR_ODD:
		comdcb.Parity = ODDPARITY;
		break;
	case SerialBuffer::PR_NONE:
		// fallthrough
	default:
		comdcb.Parity = NOPARITY;
		break;
	}
    comdcb.ByteSize = get_char_size(cs);
	if (sb == SerialBuffer::SB_2)
	{
		comdcb.StopBits = TWOSTOPBITS;
	}
	else
	{
		comdcb.StopBits = ONESTOPBIT;
	}

	if (fc == SerialBuffer::FC_HARDWARE)
	{
		comdcb.fOutxCtsFlow = TRUE;
		comdcb.fOutxDsrFlow = TRUE;
	}
	else
	{
		comdcb.fOutxCtsFlow = FALSE;
		comdcb.fOutxDsrFlow = FALSE;
	}
    comdcb.fDtrControl = DTR_CONTROL_ENABLE;
    comdcb.fRtsControl = RTS_CONTROL_ENABLE;
    comdcb.fDsrSensitivity = FALSE;

	if (fc == SerialBuffer::FC_XONOFF)
	{
		comdcb.fOutX = TRUE;
		comdcb.fInX = TRUE;
	}
	else
	{
		comdcb.fOutX = FALSE;
		comdcb.fInX = FALSE;
	}
    comdcb.fTXContinueOnXoff = TRUE;
    comdcb.XonLim = 2048;
    comdcb.XoffLim = 2048;
    comdcb.XonChar = 0x11;
    comdcb.XoffChar = 0x13;

    comdcb.fBinary = TRUE;
    comdcb.fNull = FALSE;
    comdcb.fAbortOnError = TRUE;
    comdcb.fErrorChar = FALSE;
    comdcb.ErrorChar = 0x00;
    comdcb.EofChar = 0x03;
    comdcb.EvtChar = 0x02;

    if (!SetCommState(fd_, &comdcb))
    {
		CloseHandle(fd_);
        fd_ = INVALID_HANDLE_VALUE;
		return;
    }

    comtout.ReadIntervalTimeout = 500;
    comtout.ReadTotalTimeoutMultiplier  = 0;
    comtout.ReadTotalTimeoutConstant    = 0;
    comtout.WriteTotalTimeoutMultiplier = 0;
    comtout.WriteTotalTimeoutConstant   = 0;
    if(!SetCommTimeouts( fd_, &comtout ))
    {
		CloseHandle(fd_);
        fd_ = INVALID_HANDLE_VALUE;
		return;
    }
}
#endif // KJSD_HAVE_WIN32
#ifdef KJSD_HAVE_TERMIOS
{
	setg(begin(), end(), end());

    struct termios newtio;
    memset(&newtio, 0, sizeof(newtio));
    memset(&oldtio_, 0, sizeof(oldtio_));

    fd_ = open(name, O_RDWR | O_NOCTTY);
    if (fd_ < 0) return;

    tcgetattr(fd_, &oldtio_);
    newtio.c_cflag = get_char_size(cs) | CLOCAL | CREAD;
    newtio.c_iflag = (pr == SerialBuffer::PR_NONE) ? IGNPAR: INPCK;
	switch (pr)
	{
	case SerialBuffer::PR_EVEN:
		newtio.c_cflag |= PARENB;
		break;
	case SerialBuffer::PR_ODD:
		newtio.c_cflag |= PARENB | PARODD;
		break;
	case SerialBuffer::PR_NONE:
		// fallthrough
	default:
		break;
	}
	if (sb == SerialBuffer::SB_2)
	{
		newtio.c_cflag |= CSTOPB;
	}
	switch (fc)
	{
	case SerialBuffer::FC_XONOFF:
		newtio.c_iflag |= IXON | IXOFF;
		break;
	case SerialBuffer::FC_HARDWARE:
		newtio.c_cflag |= CRTSCTS;
		break;
	case SerialBuffer::FC_NONE:
		// fallthrough
	default:
		break;
	}
    newtio.c_cc[VMIN] = 1;
    cfsetispeed(&newtio, get_baud(br));
    cfsetospeed(&newtio, get_baud(br));
    tcflush(fd_, TCIFLUSH);
    tcsetattr(fd_, TCSANOW, &newtio);
}
#endif // KJSD_HAVE_TERMIOS

SerialBuffer::~SerialBuffer()
{
	if (!*this) return;

#ifdef KJSD_HAVE_WIN32
    CloseHandle(fd_);
#endif
#ifdef KJSD_HAVE_TERMIOS
    tcsetattr(fd_, TCSANOW, &oldtio_);
	close(fd_);
#endif
}

SerialBuffer::operator bool() const
{
#ifdef KJSD_HAVE_WIN32
	return fd_ != INVALID_HANDLE_VALUE;
#endif
#ifdef KJSD_HAVE_TERMIOS
	return fd_ >= 0;
#endif
}

SerialBuffer::int_type SerialBuffer::underflow()
{
	if (!*this) return traits_type::eof();

	char_type& c = *begin();

	if (read_one(&c))
	{
		setg(begin(), begin(), end());
		return traits_type::to_int_type(c);
	}
	else
	{
		return traits_type::eof();
	}
}

SerialBuffer::int_type SerialBuffer::overflow(int_type c)
{
	if (!*this || (c == traits_type::eof())) return traits_type::eof();

	write_one(c);
    return 1;
}

SerialBuffer::int_type SerialBuffer::sync()
{
	if (!*this) return -1;

	return 0;
}

bool SerialBuffer::read_one(char* c)
#ifdef KJSD_HAVE_WIN32
{
    DWORD dw;
    DWORD dwErrors;
    COMSTAT ComStat;
    DWORD dwResult = 0;

    while (1)
    {
        dw = 0;
        ClearCommError(fd_, &dwErrors, &ComStat);
        if (ReadFile(fd_, c, 1, &dw, NULL))
        {
            if (dw == 1) break;
        }
        else
        {
            return false;
        }
    }

    return true;
}
#endif // KJSD_HAVE_WIN32
#ifdef KJSD_HAVE_TERMIOS
{
    return (read(fd_, c, 1) >= 0);
}
#endif // KJSD_HAVE_TERMIOS

void SerialBuffer::write_one(char c)
#ifdef KJSD_HAVE_WIN32
{
    DWORD dw;
    WriteFile(fd_, &c, sizeof(char_type), &dw, NULL);
}
#endif // KJSD_HAVE_WIN32
#ifdef KJSD_HAVE_TERMIOS
{
	write(fd_, &c, sizeof(char_type));
}
#endif // KJSD_HAVE_TERMIOS
