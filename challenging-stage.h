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

#ifdef	MAGNETAR_CHALLENGING_STAGE_H

class ChallengingStage;

#else	// MAGNETAR_CHALLENGING_STAGE_H
#define	MAGNETAR_CHALLENGING_STAGE_H

#include "stage.h"

class ChallengingStage :public Stage {
private:

public:
    ChallengingStage(int stage_num, int max_mines, double mine_speed, int mines_at_a_time, double avg_launch_time);
    ~ChallengingStage(void);
};

#endif	// MAGNETAR_CHALLENGING_STAGE_H
