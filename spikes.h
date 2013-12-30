//      MAGNETAR
//      Copyright (C) 2013 Ben Asselstine
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
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

#ifdef	MAGNETAR_SPIKES_H

class Spikes;

#else	// MAGNETAR_SPIKES_H
#define	MAGNETAR_SPIKES_H

#include "castle.h"
#include "king.h"
#include "laser.h"
#include "ship.h"
#include "stats.h"
#include "xything.h"

class Spikes: public virtual Xything {
private:
    bool paused;
    double step;
    double age;
    double *randomizer;

public:
    Spikes(GC);
    ~Spikes(void);

    void render(const bool);

    void turn();
    void set_step (const double);
    double get_step () const {return step;};
    void set_xpoints(void);
    void resize(const int, const int);
    bool alive();

    void pause(const bool);
};



inline void
Spikes::pause(const bool pause_state)
{
    paused = pause_state;
} // Spikes::pause

#endif	// MAGNETAR_SPIKES_H
