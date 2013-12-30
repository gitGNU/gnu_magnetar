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

#ifdef	MAGNETAR_BUZZER_H

class Buzzer;

#else	// MAGNETAR_BUZZER_H
#define	MAGNETAR_BUZZER_H

#include "castle.h"
#include "ething.h"
#include "king.h"
#include "minefield.h"
#include "stats.h"
#include "xything.h"
#include "kapow.h"
#include "sound.h"
#include "ship.h"
#include "king.h"

class Buzzer : public virtual Ething {
private:
    enum mine_states {
	MINE_LIVING,
	MINE_DEAD
    };
    enum mine_states state;
    double speed;
    bool paused;
    bool magnetic;

    void set_randomizer(void);
    void update_speed();


public:
    static double buzzer_scale;
    Buzzer(void);
    ~Buzzer(void);

    void render(const bool);
    void turn(Castle *, Ship *);

    void move(Castle *, Ship *, King *, Stats *, bool &buzzer_dead);
    void launch(std::list<std::pair<float, float> > zones, bool magnetic);
    bool hit(Laser *laser, Stats *stats);
    void set_xpoints(void);
    void upgrade(Castle *);
    void resize(const int, const int);
    void place_randomly(Castle *);
    void adjust_speed();
    bool alive(void) const;
    void snuff(void);
    void pause(const bool);
    bool riding_ring(Castle *c) const;
    bool outside() const;
    bool inside() const;
};


inline bool
Buzzer::alive(void) const
{
    return (state == MINE_LIVING);
} // Buzzer::alive


inline void
Buzzer::snuff(void)
{
  state = MINE_DEAD;
} // Buzzer:snuff

inline void
Buzzer::pause(const bool pause_state)
{
  paused = pause_state;
} // Buzzer::pause

#endif	// MAGNETAR_BUZZER_H
