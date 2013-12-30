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

#ifdef	MAGNETAR_LASER_H

class Laser;

#else	// MAGNETAR_LASER_H
#define	MAGNETAR_LASER_H

#include "castle.h"
#include "king.h"
#include "minefield.h"
#include "ship.h"
#include "stats.h"
#include "xything.h"

class Laser : public virtual Xything {
private:
    double age;
    double lifespan;
    double mag;


    Ship *ship;

public:
    Laser(void);
    ~Laser(void);

    void render(const bool);
    void snuff(void);

    void move(Castle *, King *, Minefield *, Stats *);
    void launch(void);
    void resize(const int, const int);

    bool alive(void) const;

    void set_ship(Ship *);
};


inline void
Laser::snuff(void)
{
    age = lifespan;
} // Laser::snuff


inline bool
Laser::alive(void) const
{
    return (age < lifespan);
} // Laser::alive


inline void
Laser::set_ship(Ship *s)
{
    ship = s;
} // Laser::set_ship

#endif	// MAGNETAR_LASER_H
