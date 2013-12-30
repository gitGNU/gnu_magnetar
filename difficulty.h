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

#ifndef	MAGNETAR_DIFFICULTY_H
#define	MAGNETAR_DIFFICULTY_H

#include "stats.h"
class Difficulty
{
public:
  static double avg_time_for_buzzer_to_leave_a_ring;
  static const double slowest_avg_time_for_buzzer_to_leave_a_ring;
  static const double fastest_avg_time_for_buzzer_to_leave_a_ring;

  static double speed_of_buzzers;
  static const double slowest_speed_of_buzzers;
  static const double fastest_speed_of_buzzers;

  static double turning_speed_of_king;
  static const double slowest_turning_speed_of_king;
  static const double fastest_turning_speed_of_king;

  static double avg_time_to_launch_new_buzzer_from_king;
  static const double slowest_avg_time_to_launch_new_buzzer_from_king;
  static const double fastest_avg_time_to_launch_new_buzzer_from_king;

  static double avg_time_to_expire_outside_buzzers;
  static const double slowest_avg_time_to_expire_outside_buzzers;
  static const double fastest_avg_time_to_expire_outside_buzzers;

  static double buzzer_turning_radius;
  static const double largest_buzzer_turning_radius;
  static const double smallest_buzzer_turning_radius;

  static double ring_speed[];
  static const double slowest_ring_speed;
  static const double fastest_ring_speed;

  static void init();
};

#endif	//MAGNETAR_DIFFICULTY_H
