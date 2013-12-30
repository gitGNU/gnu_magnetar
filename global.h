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

#ifndef	MAGNETAR_GLOBAL_H
#define	MAGNETAR_GLOBAL_H

#ifdef	HAVE_CONFIG_H
#include <config.h>
#endif	// HAVE_CONFIG_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* gross, but this seems to work the best for now */
#include <stdio.h>
#include <cstdio>

#include <stddef.h>
#include <cstddef>

#include <cstdlib>
#include <stdlib.h>

#include <math.h>
#include <cmath>

#include <limits.h>
#include <climits>

#ifdef	DEBUG
#undef	NDEBUG
#include <assert.h>
#include <cassert>
#endif	// DEBUG


#ifndef	NULL
#define	NULL	0
#endif	/* NULL */

struct coords {
    double x;
    double y;
};

enum key_state {
    KEY_UP,
    KEY_DOWN
};


inline int
within(const double a, const double b, const double c)
{
    return (a >= (b - c) && a <= (b + c));
} // ::within


inline int
sign(const double a)
{
    return (a > 0.0) ? +1 : -1;
} // ::sign


inline double
max(const double a, const double b)
{
    return (a > b) ? a : b;
} // ::max


inline double
min(const double a, const double b)
{
    return (a < b) ? a : b;
} // ::min


inline double
sq(const double x)
{
    return x * x;
} // ::sq

#endif	// MAGNETAR_GLOBAL_H
