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

#include <X11/keysym.h>

#include "global.h"

#include "xsc.h"

#include "difficulty.h"

#include "castle.h"
#include "random.h"
//buzzers
double Difficulty::avg_time_for_buzzer_to_leave_a_ring = 2.0;
const double Difficulty::slowest_avg_time_for_buzzer_to_leave_a_ring = 2.0;
const double Difficulty::fastest_avg_time_for_buzzer_to_leave_a_ring = 0.1;

double Difficulty::speed_of_buzzers = slowest_speed_of_buzzers;
const double Difficulty::fastest_speed_of_buzzers = 2.0; //pixels per frame per second, i guess.
const double Difficulty::slowest_speed_of_buzzers = 5.0;

//king
double Difficulty::turning_speed_of_king = 4.0;
const double Difficulty::fastest_turning_speed_of_king = 10.0;
const double Difficulty::slowest_turning_speed_of_king = 4.0;

double Difficulty::avg_time_to_launch_new_buzzer_from_king = Difficulty::slowest_avg_time_to_launch_new_buzzer_from_king;
const double Difficulty::slowest_avg_time_to_launch_new_buzzer_from_king = 2;
const double Difficulty::fastest_avg_time_to_launch_new_buzzer_from_king = 0.5;

//castle
double Difficulty::ring_speed[MAX_RINGS] = { -45.0};//, -195.0};//, 90.0 };
const double Difficulty::fastest_ring_speed = 225.0;
const double Difficulty::slowest_ring_speed = 45.0;

double Difficulty::avg_time_to_expire_outside_buzzers = Difficulty::fastest_avg_time_to_expire_outside_buzzers;
const double Difficulty::slowest_avg_time_to_expire_outside_buzzers = 60.0;
const double Difficulty::fastest_avg_time_to_expire_outside_buzzers = 13.0;

//i don't know what these units of measurements are.
double Difficulty::buzzer_turning_radius = Difficulty::largest_buzzer_turning_radius;
const double Difficulty::largest_buzzer_turning_radius = 70;
const double Difficulty::smallest_buzzer_turning_radius = 180;

void
Difficulty::init()
{
  speed_of_buzzers = slowest_speed_of_buzzers;
  avg_time_for_buzzer_to_leave_a_ring = 
    slowest_avg_time_for_buzzer_to_leave_a_ring;
  turning_speed_of_king = slowest_turning_speed_of_king;
  avg_time_to_launch_new_buzzer_from_king =
   slowest_avg_time_to_launch_new_buzzer_from_king; 
  ring_speed[0] = -slowest_ring_speed*4;
  //ring_speed[1] = slowest_ring_speed*2;
  //ring_speed[2] = slowest_ring_speed*2;
}

