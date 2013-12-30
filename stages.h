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

#ifdef	MAGNETAR_STAGES_H

class Stages;

#else	// MAGNETAR_STAGES_H
#define	MAGNETAR_STAGES_H

#include "king.h"
#include "minefield.h"
#include "castle.h"
#include "ship.h"
#include "starfield.h"
#include "stage.h"
#include <vector>
#include "game.h"

class Stages {
private:
    std::vector<Stage> *stages;
    King *king;
    King *queen;
    Minefield *minefield;
    Castle *castle;
    Ship *ship;
    Starfield *starfield;
    Game *game;
    Stage attract;
    Stage empty;

    void start_stage(Stage stage);
    Stage get(int stage_num);
public:
    Stages(King *king, King *queen, Minefield *minefield, Castle *castle, Ship *ship, Starfield *light_starfield, Game *game);
    ~Stages(void);

    unsigned int get_stage_num();
    unsigned int get_next_stage_num();
    unsigned int get_max_mines();
    void start_stage(int stage);
    double get_avg_launch_time();
    bool challenge();
    bool challenge_upcoming();
    bool finished_challenge();
    double get_mine_speed();
    unsigned int get_max_stages();
    Stage get_stage();
};

#endif	// MAGNETAR_STAGES_H
