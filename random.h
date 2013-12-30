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

#ifndef	MAGNETAR_RANDOM_H
#define	MAGNETAR_RANDOM_H

#ifdef	HAVE_CONFIG_H
#include <config.h>
#endif	// HAVE_CONFIG_H

namespace Random {
    int get(void);
    void seed(unsigned int);
} // namespace Random


#if	HAVE_RANDOM && HAVE_SRANDOM

inline int
Random::get(void)
{
    return random();
} // Random::get


inline void
Random::seed(unsigned int x)
{
    srandom(x);
} // Random::seed

#else	// HAVE_RANDOM && HAVE_SRANDOM

inline int
Random::get(void)
{
    return rand();
} // Random::get


inline void
Random::seed(unsigned int x)
{
    srand(x);
} // Random::seed

#endif	// HAVE_RANDOM && HAVE_SRANDOM

#endif	// MAGNETAR_RANDOM_H
