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

#ifdef	MAGNETAR_STAMP_H

class Stamp;

#else	// MAGNETAR_STAMP_H
#define	MAGNETAR_STAMP_H

class Stamp {
private:
    long sec;
    long usec;

public:
    Stamp(void);
    ~Stamp(void);

    Stamp &operator=(const long);
    bool operator==(const Stamp &) const;
    bool operator!=(const Stamp &) const;
    Stamp &operator+=(const Stamp &);
    Stamp &operator-=(const Stamp &);
    bool operator>(const long);
    bool operator<(const long);

    long micros(void) const;
    void normalize(void);

    operator double() const;
    void set(const long, const long);
    long get_sec(void) const;
    long get_usec(void) const;
};

extern Stamp operator+(const Stamp &s1, const Stamp &s2);
extern Stamp operator-(const Stamp &s1, const Stamp &s2);


inline
Stamp::operator double() const
{
    return (double)(sec + (usec / 1000000.0));
} // Stamp::operator double


inline void
Stamp::set(const long s, const long u)
{
    sec = s;
    usec = u;
} // Stamp::set


inline long
Stamp::get_sec(void) const
{
    return sec;
} // Stamp::get_sec


inline long
Stamp::get_usec(void) const
{
    return usec;
} // Stamp::get_usec

#endif	// MAGNETAR_STAMP_H
