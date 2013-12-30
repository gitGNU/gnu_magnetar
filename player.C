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

#include "global.h"

#include "player.h"
#include "game.h"

Player::Player(Game *game, unsigned int num)
{
  stats = new Stats(game->ship, game);
  id = num;
} // Player::Player


Player::~Player(void)
{
} // Player::~Player
    
bool Player::isAlive()
{
  if (stats->lives < 0)
    return false;
  return true;
}

bool Player::is_thrust_key (KeySym ks)
{
  if (get_id() % 2 == 1)
    {
      if (ks == args.p1_thrust)
        return true;
    }
  else
    {
      if (ks == args.p2_thrust)
        return true;
    }
  return false;
}

bool Player::is_cw_key (KeySym ks)
{
  if (get_id() % 2 == 1)
    {
      if (ks == args.p1_cw)
        return true;
    }
  else
    {
      if (ks == args.p2_cw)
        return true;
    }
  return false;
}

bool Player::is_ccw_key (KeySym ks)
{
  if (get_id() % 2 == 1)
    {
      if (ks == args.p1_ccw)
        return true;
    }
  else
    {
      if (ks == args.p2_ccw)
        return true;
    }
  return false;
}

bool Player::is_fire_key (KeySym ks)
{
  if (get_id() % 2 == 1)
    {
      if (ks == args.p1_fire)
        return true;
    }
  else
    {
      if (ks == args.p2_fire)
        return true;
    }
  return false;
}
