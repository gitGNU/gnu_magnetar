//      MAGNETAR
//      Copyright (C) 2013, 2014 Ben Asselstine
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; version 2 of the License, or   
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

#ifndef MAGNETAR_SOUND_H
#define MAGNETAR_SOUND_H


enum {
  SHIP_EXPLODE,
  SHIP_FIRE_LASER,
  ASTEROID_EXPLODE,
  GAME_START,
  EXTRA_SHIP_AWARDED,
  REWARD_OBTAINED,
  STAGE_PREPARE,
  CHALLENGING_STAGE_PREPARE,
  REWARD_LEAVING,
  REWARD_ENTERING,
  COIN_UP,
  GAME_OVER,
  SHIP_INVERT_POLARITY,
  CONGRATULATIONS,
  ASTEROID_LEAVING,
  SHIP_DEFLECT,
  STAGE_START,
  SHIP_THRUST,
  BONUS,
  MAX_SOUNDS
};

void sound_init();
void play(int snd);
void thrust_sound(bool on);
void bonus_sound(bool on);
void sound_pause(bool pause);
void sound_free();

extern bool thrust_sound_on;
extern bool bonus_sound_on;
extern double thrust_volume;
extern double bonus_volume;
extern double effects_volume;

#endif /* SOUND_H */

