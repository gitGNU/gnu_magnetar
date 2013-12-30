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

#include "global.h"

#include "random.h"
#include "util.h"
#include "xsc.h"

#include "auto-pilot.h"

AutoPilot::AutoPilot(Game *g)
{
  game = g;
  initial_rotation_completed = false;
  initial_thrusting_started = false;
  Stamp thrust_until  = time_now;
  Stamp thrust_start = time_now;
  secondary_rotation_completed = false;
  secondary_theta = Random::get() % 2 == 0 ? 238.0 : 242.0;
} // AutoPilot::AutoPilot
    
AutoPilot::~AutoPilot(void)
{
} // AutoPilot::~AutoPilot

void AutoPilot::turn()
{
  if (initial_rotation_completed == false)
    {
      if (game->ship->alive() == false && game->ship->rotating_ccw())
        game->ship->rotate_ccw(KEY_DOWN);
      //keep turning ccw until what?
      if (game->ship->rotating_ccw() == false && 
          game->ship->get_theta() <= 240)
        game->ship->rotate_ccw(KEY_DOWN);
      else if (game->ship->get_theta() > 240.0)
        {
          game->ship->rotate_ccw(KEY_UP);
          initial_rotation_completed = true;
        }
    } 
  else if (initial_rotation_completed && secondary_rotation_completed == false)
    {
      double diff = abs(secondary_theta - game->ship->get_theta());
      if (diff < 10 + (Random::get() % 50))
        {
        game->ship->rotate_cw(KEY_UP);
        game->ship->rotate_ccw(KEY_DOWN);
        }
      else if (diff > 100)
        {
        secondary_rotation_completed = true;
        game->ship->rotate_ccw(KEY_UP);
        }

    }
}

void AutoPilot::move()
{
  if (game->ship->alive() == false && game->ship->thrusting())
    {
      game->ship->thrust(KEY_UP);
      return;
    }

  if (initial_rotation_completed)
    {
      if (game->ship->thrusting() == false && initial_thrusting_started == false)
        {
          game->ship->thrust(KEY_DOWN);
          long usecs = time_now.get_usec();
          initial_thrusting_started = true;
          if (usecs < 500000)
            {
            usecs += 300000;
          thrust_until.set(time_now.get_sec(), usecs);
            }
          else
            {
            usecs -= 300000;
          thrust_until.set(time_now.get_sec()+1, usecs);
            }
        }
      else if (game->ship->thrusting() == false && secondary_rotation_completed == true && thrust_until.get_sec() < time_now.get_sec() && Random::get() % int(args.fps) == 0)
        {
          initial_thrusting_started = false;
          secondary_theta = 70;
          secondary_rotation_completed = false;
        }
      else if (game->ship->thrusting() == true)
        {
          if (time_now.get_sec() > thrust_until.get_sec())
            {
              game->ship->thrust(KEY_UP);
            }
        }
    }
}

void AutoPilot::reset()
{
  initial_rotation_completed = false;
    game->ship->rotate_ccw(KEY_UP);
    game->ship->thrust(KEY_UP);
    game->ship->reincarnate();
    thrust_start = time_now;
    thrust_until = time_now;
    secondary_rotation_completed = false;
  secondary_theta = Random::get() % 2 == 0 ? 238.0 : 242.0;
          initial_thrusting_started = false;
}
