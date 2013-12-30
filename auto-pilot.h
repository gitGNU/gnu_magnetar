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

#ifdef	MAGNETAR_AUTO_PILOT_H

class AutoPilot;

#else	// MAGNETAR_AUTO_PILOT_H
#define	MAGNETAR_AUTO_PILOT_H

#include "ship.h"

class AutoPilot {
private:

    Game *game;
    bool initial_rotation_completed;
    Stamp thrust_until;
    Stamp thrust_start;
    bool secondary_rotation_completed;
    double secondary_theta;
    bool initial_thrusting_started;
public:
    AutoPilot(Game *game);
    void turn();
    void move();
    void reset();
    ~AutoPilot(void);
};

#endif	// MAGNETAR_AUTO_PILOT_H
