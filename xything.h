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

#ifdef	MAGNETAR_XYTHING_H

class Xything;

#else	// MAGNETAR_XYTHING_H
#define	MAGNETAR_XYTHING_H

#include "trig.h"
#include "xsc.h"

#include "thing.h"

using namespace Trig;

class Xything : virtual public Thing {
private:
    double dx;
    double dy;
protected:

public:
    Xything(void);
    virtual ~Xything(void);

    double get_dx(void) const;
    void set_dx(const double);

    double get_dy(void) const;
    void set_dy(const double);

    double get_angle(void) const;
    double get_radius(void) const;

    virtual void move(void);
    virtual void resize(const int, const int);
    virtual void bounce(const double);
};


inline void
Xything::set_dx(const double new_dx)
{
    dx = new_dx;
} // Xything::set_dx




inline void
Xything::set_dy(const double new_dy)
{
    dy = new_dy;
} // Xything::set_dy


inline double
Xything::get_angle(void) const
{
    return xatan2(get_y() - wh2(), ww2() - get_x());
} // Xything::get_angle


inline double
Xything::get_radius(void) const
{
    return hypot(get_x() - ww2(), get_y() - wh2());
} // Xything::get_radius

#endif	// MAGNETAR_XYTHING_H
