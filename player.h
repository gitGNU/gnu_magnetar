//      MAGNETAR
//      Copyright (C) 2013 Ben Asselstine
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
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

#include "stats.h"
#ifdef	MAGNETAR_PLAYER_H

class Player;

#else	// MAGNETAR_PLAYER_H
#define	MAGNETAR_PLAYER_H

class Game;

class Player {
private:
    unsigned int id;

public:
    Player(Game *game, unsigned int id);
    ~Player(void);
    Stats		*stats;
    bool isAlive();
    bool isDead() {return !isAlive();};
    unsigned int get_id() {return id;};
    bool is_thrust_key (KeySym ks);
    bool is_cw_key (KeySym ks);
    bool is_ccw_key (KeySym ks);
    bool is_fire_key (KeySym ks);

};

#endif	// MAGNETAR_PLAYER_H
