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

#ifdef	MAGNETAR_KAPOW_H

class Kapow;

#else	// MAGNETAR_KAPOW_H
#define	MAGNETAR_KAPOW_H

#include "castle.h"
#include "ship.h"
#include "ething.h"

class Kapow: public Ething {
private:
    double starsize;
    struct coords *pts;
    int wander;
    double age;
    double step;
    bool paused;
    double lifespan;
    int frame;
    int number_of_frames;

public:
    Kapow(GC gc, const double, const int);
    Kapow(const Kapow &o);
    ~Kapow(void);

    void render(const bool);
    void draw();
    void erase();
    bool alive() const;
    void set_step (const double);
    void turn ();

    void resize(const int, const int);
    void pause(const bool);
};

inline void
Kapow::pause(const bool pause_state)
{
  paused = pause_state;
} // Kapow::pause

inline void
Kapow::draw()
{
    render(true);
} // Kapow::draw


inline void
Kapow::erase()
{
    render(false);
} // Kapow::erase

#endif	// MAGNETAR_KAPOW_H
