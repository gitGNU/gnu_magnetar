//      MAGNETAR
//      Copyright (c) 1993-2005 by Mark B. Hanson (mbh@panix.com).
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
//  02110-1301, USA.
//

#ifndef	MAGNETAR_TIME_H
#define	MAGNETAR_TIME_H

#ifdef	HAVE_CONFIG_H
#include <config.h>
#endif	// HAVE_CONFIG_H

#if	TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else	// TIME_WITH_SYS_TIME
#if	HAVE_SYS_TIME_H
#include <sys/time.h>
#else	// HAVE_SYS_TIME_H
#include <time.h>
#endif	// HAVE_SYS_TIME_H
#endif	// TIME_WITH_SYS_TIME_H

#include "stamp.h"

namespace XSCTime {
    void now(Stamp *const);
    void sleep(const long);
} // namespace XSCTime


inline void
XSCTime::now(Stamp *const s)
{
    struct timeval tv;

    gettimeofday(&tv, 0);
    s->set(tv.tv_sec, tv.tv_usec);
} // XSCTime::now


inline void
XSCTime::sleep(const long delta)
{
    usleep((unsigned int)delta);
} // XSCTime::sleep

#endif	// MAGNETAR_TIME_H
