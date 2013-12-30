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

#ifdef	MAGNETAR_STAGE_H

class Stage;

#else	// MAGNETAR_STAGE_H
#define	MAGNETAR_STAGE_H

#include "magnet-info.h"
#include "king.h"
#include <list>

class Stage {
private:
    int stage_num;
    int max_mines;
    double mine_speed;
    int mines_at_a_time;
    double avg_launch_time; //mine launches every this many seconds.
    std::list<MagnetInfo> lmagnets;
    std::list<MagnetInfo> rmagnets;

    double magnetic_percent; //how many of max_mines are magnetic.
    std::list<std::pair<float, float> > mine_drop_zones;
protected:
    bool challenging_stage;

public:
    inline int get_number_of_left_magnets() {return lmagnets.size();};
    inline int get_number_of_right_magnets(){return rmagnets.size();};
    Stage(int stage_num, int max_mines, double mine_speed, int mines_at_a_time, double avg_launch_time, double magnetic_mines);
    ~Stage(void);
    int get_id() const {return stage_num;};
    int get_max_mines() const {return max_mines;};
    double get_mine_speed() const {return mine_speed;};
    int get_mines_at_a_time() const {return mines_at_a_time;};
    double get_avg_launch_time() const {return avg_launch_time;};
    void left(MagnetInfo m);
    void right(MagnetInfo m);
    void drop(float l, float r);
    bool get_challenging_stage() const{return challenging_stage;};

    std::list<std::pair<float, float> > get_drop_zones() const {return mine_drop_zones;};
    void add_magnets(King *king);
    double get_magnetic_mine_percent() {return magnetic_percent;};
};

#endif	// MAGNETAR_STAGE_H
