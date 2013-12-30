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

#ifdef	MAGNETAR_CASTLE_H

class Castle;
class Buzzer;

#else	//MAGNETAR_CASTLE_H
#define	MAGNETAR_CASTLE_H

#include "laser.h"
#include "minefield.h"
#include "ring.h"
#include "stats.h"
#include "ship.h"
#include "game.h"
#include <list>

class Starfield;

enum castle_rings {
    CASTLE_RING_OUTER,
    CASTLE_RING_MIDDLE,
    CASTLE_RING_INNER,
};

class Castle {
private:

    enum castle_states {
	CASTLE_STATE_NORMAL,
	CASTLE_STATE_REGENERATING,
	CASTLE_STATE_COLLAPSING,
	CASTLE_STATE_EXPLODING,
	CASTLE_STATE_RESTING
    };

    int numrings;
    Ring **rings;
    enum castle_states state;
    Stamp time_of_death;
    Stamp pause_start;
    Stamp pause_sum;
    double explosion_age;
    double explosion_count;

    long resting_time;
    long staying_time;
    int get_resting_time() const;
    int get_staying_time() const;
    int reward_type;
    int get_random_reward();
public:
    static const int points_awarded;
    Castle(void);
    ~Castle(void);

    void render(const bool);
    void draw();
    void erase();

    void turn(Minefield *, King *, Stats *, Game *);
    void resize(const int, const int);
    bool collision(Laser *, Stats *);
    bool buzzer_collision(Buzzer *buzzer, Ship *ship, Stats *stats);
    bool opening(const double) const;
    bool hole(const int, const double) const;
    double ring_size(const int) const;
    void seg_center(const int, /*const*/ double,
	    double * /*const*/, double * /*const*/);
    void explode(Stats *);
    void reset();
    void refresh(void) const;
    int get_reward_type() const {return reward_type;};
    bool recently_died() const;

    int get_numrings(void) const;
    void pause(const bool);
    bool alive() const;
    bool resting() const;
    double get_x() const;
    double get_y() const;
    double get_diag() const;
    void jump();
    int get_number_of_remaining_segments() const;
    int get_total_number_of_segments() const;
};


inline bool
Castle::resting() const
{
  return state == CASTLE_STATE_RESTING;
}

inline void
Castle::draw()
{
    render(true);
} // Castle::draw


inline void
Castle::erase()
{
    render(false);
} // Castle::erase


inline int
Castle::get_numrings(void) const
{
    return numrings;
} // Castle::get_numrings


inline void
Castle::pause(const bool pause_state)
{
    if (pause_state) {
	pause_start = time_now;
    } else {
	pause_sum += time_now - pause_start;
    }
} // Castle::pause


inline bool
Castle::alive(void) const
{
    return (state != CASTLE_STATE_EXPLODING && state != CASTLE_STATE_RESTING);
            //&& state != CASTLE_STATE_COLLAPSING);
            //can't say collapsing is dead because we want to have it showing.
} // Castle::alive

#endif	//MAGNETAR_CASTLE_H
