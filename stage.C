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

#include "xsc.h"
#include "stage.h"
#include "king.h"

Stage::Stage(int num, int mines, double mspeed, int minesatatime, double avglaunchtime, double magnetic)
{
  stage_num = num;
  max_mines = mines;
  mine_speed = mspeed;
  mines_at_a_time = minesatatime;
  avg_launch_time = avglaunchtime;
  lmagnets = std::list<MagnetInfo>();
  rmagnets = std::list<MagnetInfo>();
  challenging_stage = false;
  magnetic_percent = magnetic;
} // Stage::Stage

    
Stage::~Stage(void)
{
} // Stage::~Stage

void 
Stage::left (MagnetInfo m)
{
  double w = Fireball::get_width() * 0.55;
  m.set_starting_width((lmagnets.size() * w)+(w/2));
  lmagnets.push_back(m);
  //here we stack the magnets from the left.
  //changing the starting width accordingly.
} // Stage::add_left_magnet

void 
Stage::right(MagnetInfo m)
{
  double w = Fireball::get_width() * 0.55;
  m.set_starting_width(ww()-((rmagnets.size() * w)+(w/2)));
  rmagnets.push_back(m);
} // Stage::add_right_magnet

void Stage::add_magnets(King *king)
{
  double w = Fireball::get_width() * 0.55;
  int count =0;
  for (std::list<MagnetInfo>::iterator i = lmagnets.begin(); 
       i != lmagnets.end(); i++)
    {
      (*i).set_starting_width((count * w)+(w/2));
      king->add_fireball(*i);
      count++;
    }
  count =0;
  for (std::list<MagnetInfo>::iterator i = rmagnets.begin(); 
       i != rmagnets.end(); i++)
    {
      (*i).set_starting_width(ww()-((count * w)+(w/2)));
      count++;
    king->add_fireball(*i);
    }
}

void
Stage::drop(float l,  float r)
{
  mine_drop_zones.push_back(std::pair<float, float>(l, r));
}
