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

#ifdef	MAGNETAR_TTHING_H

class Tthing;

#else	// MAGNETAR_TTHING_H
#define	MAGNETAR_TTHING_H

#include "thing.h"

class Tthing : virtual public Thing {
protected:
    double theta;
    double dtheta;

public:
    Tthing(void);
    virtual ~Tthing(void);

    double get_theta(void) const;
    void set_theta(const double);

    double get_dtheta(void) const;
    void set_dtheta(const double);

    virtual void set_xpoints(void);
    virtual void turn(void);
};


inline double
Tthing::get_theta(void) const
{
    return theta;
} // Tthing::get_theta


inline void
Tthing::set_theta(const double ntheta)
{
    theta = ntheta;
} // Tthing::get_theta


inline double
Tthing::get_dtheta(void) const
{
    return dtheta;
} // Tthing::get_theta


inline void
Tthing::set_dtheta(const double ndtheta)
{
    dtheta = ndtheta;
} // Tthing::get_theta

#endif	// MAGNETAR_TTHING_H
