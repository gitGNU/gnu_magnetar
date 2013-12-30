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

#include "global.h"

#include "stamp.h"


Stamp::Stamp(void)
{
    //fprintf(stderr, "Stamp::Stamp()\n");
} // Stamp::Stamp


Stamp::~Stamp(void)
{
    //fprintf(stderr, "Stamp::~Stamp()\n");
} // Stamp::~Stamp


Stamp &
Stamp::operator=(const long l)
{
    sec = l;
    usec = 0;

    return *this;
} // Stamp::operator=


bool
Stamp::operator==(const Stamp &s) const
{
    return (sec == s.sec) && (usec == s.usec);
} // Stamp::operator==


bool
Stamp::operator!=(const Stamp &s) const
{
    return !(*this == s);
} // Stamp::operator!=


Stamp &
Stamp::operator+=(const Stamp &s)
{
    sec += s.sec;
    usec += s.usec;

//    normalize();

    return *this;
} // Stamp::operator+


Stamp &
Stamp::operator-=(const Stamp &s)
{
    sec -= s.sec;
    usec -= s.usec;

//    normalize();

    return *this;
} // Stamp::operator-


bool
Stamp::operator>(const long x)
{
    return micros() > x;
} // Stamp::operator>


bool
Stamp::operator<(const long x)
{
    return micros() < x;
} // Stamp::operator<


long
Stamp::micros(void) const
{
    return (sec * 1000000) + usec;
} // Stamp::micros


void
Stamp::normalize(void)
{
    while (usec < 0) {
	sec--;
	usec += 1000000;
    }

    while (usec >= 1000000) {
	sec++;
	usec -= 1000000;
    }
} // Stamp::normalize


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


Stamp
operator+(const Stamp &s1, const Stamp &s2)
{
    Stamp s = s1;

    return s += s2;
} // ::operator+


Stamp
operator-(const Stamp &s1, const Stamp &s2)
{
    Stamp s = s1;

    return s -= s2;
} // ::operator-
