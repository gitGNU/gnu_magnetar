//      MAGNETAR
//      Copyright (c) 1993-2005 by Mark B. Hanson (mbh@panix.com).
//      Copyright (C) 2013 Ben Asselstine
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

#ifdef	MAGNETAR_RING_H

class Ring;

#else	// MAGNETAR_RING_H
#define	MAGNETAR_RING_H

#include "laser.h"
#include "stats.h"
#include "tthing.h"

typedef enum _hitstatus {
    RING_DULL,
    RING_BRIGHT,
    RING_DEAD
} hitstatus;


class Ring : public Tthing {
private:
    hitstatus *hits;
    int numsegments;
    unsigned int value;

    int which_seg(const double) const;

public:
    Ring(const double, const double, const unsigned int,
	    const GC, const int);
    ~Ring(void);

    void render(const bool);

    void seg_center(const double, double *const, double *const) const;
    bool hit(Xything *, Stats *, bool *dead);
    int remaining(void) const;
    bool hole(const double) const;
    void refresh(void);
    void snuff();
    int get_num_segments() const {return numsegments;};

    unsigned int get_value(void) const;
    void set_value(const unsigned int);
    void set_gc(const GC);
};


inline unsigned int
Ring::get_value(void) const
{
    return value;
} // Ring::get_value


inline void
Ring::set_value(const unsigned int v)
{
    value = v;
} // Ring::set_value


inline void
Ring::set_gc(const GC ngc)
{
  gc = ngc;
} // Ring::set_gc

#endif	// MAGNETAR_RING_H
