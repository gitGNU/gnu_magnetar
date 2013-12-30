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

#ifdef	MAGNETAR_KING_H

class King;

#else	// MAGNETAR_KING_H
#define	MAGNETAR_KING_H
#include <list>

#include "castle.h"
#include "fireball.h"
#include "laser.h"
#include "minefield.h"
#include "ship.h"
#include "stats.h"
#include "tthing.h"
#include "magnet-info.h"

class King : public virtual Tthing {
private:
    std::list<Fireball*> *magnets;
    bool living;
    double animation_age;
    bool left; //side of the screen
    bool draw_force_arrow;

public:
    King(bool left);
    ~King(void);

    void render(const bool);

    void turn(Castle *, Ship *, Stats *);
    void attract();
    void resize(const int, const int);
    bool hit(Castle *, Laser *, Minefield *, Stats *);
    bool fireball_hit (Laser *, Stats *);
    void reincarnate(void);
    void pause(const bool) const;
    void show_force_arrow(bool b);

    void reset();
    bool alive(void) const;
    float get_aligned_strength(Ething *) const;
    void add_attract_fireballs();
    void add_fireball(MagnetInfo i);
    int count_left_magnets();
    int count_right_magnets();
};

inline void
King::reincarnate(void)
{
    living = true;
} // King::reincarnate

inline bool
King::alive(void) const
{
    return living;
} // King::alive

#endif	// MAGNETAR_KING_H
