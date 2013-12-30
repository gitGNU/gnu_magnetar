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

#ifdef	MAGNETAR_FIREBALL_H

class Fireball;

#else	// MAGNETAR_FIREBALL_H
#define	MAGNETAR_FIREBALL_H

#include "king.h"
#include "ship.h"
#include "stats.h"
#include "xything.h"
#include "ething.h"
#include "laser.h"
#include "magnet-info.h"

class Fireball : public virtual Xything {
private:
    bool flying;
    double *randomizer;
    bool paused;
    bool aligned;

    float strength;
    float length;
    float speed; //lower is faster
    float starting_height;
    float starting_width;
    int dir;
    bool bouncy;
    double switchy;
    void set_randomizer(void);
    unsigned int last_switch_pos;

public:
    Fireball(float strength, float length, float speed, int dir, float starting_height, float starting_width, bool bouncy, double switchy);
    Fireball(MagnetInfo i);
    ~Fireball(void);

    void render(const bool);

    void move(Ship *, bool wrap, Stats *);
    void launch(King *, Stats *);
    void set_xpoints(void);

    static double get_width();

    bool hit(Laser *laser, Stats *stats);

    bool alive(void) const;
    void snuff(void);
    void pause(const bool);
    float get_strength() const {return strength;};
    float get_length() const {return length;};
    float get_speed() const {return speed;};
    void set_speed(float s) {speed =s;};
    int get_dir() const {return dir;};
    bool should_switch();
    bool is_aligned (Ething *ship);
    void set_bouncy(bool b) {bouncy = b;};
    void set_switchy(double s) {switchy = s;};
};


inline bool
Fireball::alive(void) const
{
    return flying;
} // Fireball::alive


inline void
Fireball::snuff(void)
{
    flying = false;
} // Fireball::snuff


inline void
Fireball::pause(const bool pause_state)
{
    paused = pause_state;
} // Fireball::pause

#endif	// MAGNETAR_FIREBALL_H
