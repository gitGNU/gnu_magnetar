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

#ifdef	MAGNETAR_MINEFIELD_H

class Minefield;

#else	// MAGNETAR_MINEFIELD_H
#define	MAGNETAR_MINEFIELD_H

#include "buzzer.h"
#include "castle.h"
#include "king.h"
#include "laser.h"
#include "ship.h"
#include "stats.h"
#include "kapow.h"
#include "king.h"
#include <list>

class Minefield {
public:
static const int max_number_of_mines = 9;
private:

    Buzzer *buzzers;
    std::list<Kapow> *expl;
    std::list<unsigned int> *magnetic_mines; //indices of mines that split
    unsigned int mines_expired; //how many have died or fallen through
    int mines; // the total number of mines to launch

    unsigned int mines_at_a_time;
    unsigned int mines_left; //how many left to launch.
    std::list<std::pair<float, float> >drop_zones;
public:
    Minefield(unsigned int num_mines, unsigned int mines_at_a_time);
    ~Minefield(void);

    void render(const bool) const;
    void draw(void) const;
    void erase(void) const;

    void set_drop_zones(std::list<std::pair<float, float> > d) {drop_zones = d;};
    void move(Castle *, Ship *, King *, Stats *);
    void resize(const int, const int) const;
    bool hit(Laser *, Stats *);
    void launch(King *, Castle *, Stats *);
    int expire();
    void upgrade(Castle *) const;
    void snuff(void);
    int snuff_visible();
    void pause(bool) const;
    void spawn_buzzers(Castle *,int);
    void adjust_speed();
    void turn(Castle *, Ship *);
    bool no_mines_left_to_launch() const {return mines_left==0 ? true: false;};
    void snuff_visible_and_future_mines(Stats *stats);
    void reset(unsigned int max_mines, unsigned int mines_at_a_time, double magnetic_percent);
};


inline void
Minefield::draw(void) const
{
    render(true);
} // Minefield::draw


inline void
Minefield::erase(void) const
{
    render(false);
} // Minefield::erase

#endif	// MAGNETAR_MINEFIELD_H
