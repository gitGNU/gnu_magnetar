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
#include "global.h"
#include "xsc.h"
#include "game.h"
#include "sound.h"
#include <SDL.h>
#include <SDL_mixer.h>

Mix_Chunk * sounds[MAX_SOUNDS];
bool thrust_sound_on = false;
double thrust_volume = 11.0;
bool bonus_sound_on = false;
double bonus_volume = 4.0;
double effects_volume = 11.0;

const char * files[MAX_SOUNDS] = {
	     "bombexplosion.wav",
	     "flak_gun_sound.wav",
	     "bodyslam.wav",
	     "magnetar.wav",
	     "wave-complete.wav",
	     "reward-obtained.wav",
	     "level-begin.wav",
	     "challenging-stage.wav",
	     "reward-out.wav",
	     "reward-in.wav",
	     "coin-in.wav",
	     "game-over.wav",
	     "invert-polarity.wav",
	     "congratulations.wav",
             "asteroid-leaving.wav",
             "deflect.wav",
             "magnets.wav",
             "thrust.wav",
             "bonus.ogg"
};

Mix_Music *thrust;
Mix_Music *bonus;

void play(int snd)
{
  if (args.quiet)
    return;
  if (args.quiet_attract && game && game->state == STATE_ATTRACT)
    return;
  double foo = effects_volume * (float)(128.0/11.0);
  Mix_Volume (-1,  int(foo));
  Mix_PlayChannel(-1, sounds[snd], 0);
}

void thrust_sound(bool on)
{
  if (args.quiet)
    return;
  if (args.quiet_attract && game->state == STATE_ATTRACT)
    return;
  thrust_sound_on = on;
  if (on)
    {
      Mix_VolumeMusic((float)thrust_volume * (float)(128.0/11.0));
      if (Mix_PausedMusic())
        Mix_ResumeMusic();
      else
        Mix_FadeInMusic(thrust, 1, 0);
    }
  else
    Mix_PauseMusic();
}

void bonus_sound(bool on)
{
  if (args.quiet)
    return;
  if (args.quiet_attract && game->state == STATE_ATTRACT)
    return;
  bonus_sound_on = on;
  if (on)
    {
      Mix_VolumeMusic((float)bonus_volume * (float)(128.0/11.0));
      Mix_FadeInMusic(bonus, 1, 0);
    }
  else
    {
      Mix_PauseMusic();
      Mix_FadeInMusic(thrust, 1, 0);
      Mix_PauseMusic();
    }
}


void repeat_thrust()
{
  if (args.quiet)
    return;
  if (args.quiet_attract && game->state == STATE_ATTRACT)
    return;
  Mix_FadeInMusic(thrust, 1, 0);
}

void repeat_bonus()
{
  if (args.quiet)
    return;
  if (args.quiet_attract && game->state == STATE_ATTRACT)
    return;
  Mix_FadeInMusic(bonus, 1, 0);
}

void sound_init()
{
  //if (args.quiet)
    //return;
  SDL_Init(SDL_INIT_AUDIO);
  Mix_OpenAudio(44100, AUDIO_S16, 2, 1024); 
  for (int i = 0; i < MAX_SOUNDS; i++)
    {
      char *file = NULL;
      if (asprintf(&file, "%s/%s", MAGNETAR_DATADIR, files[i]) != -1)
        {
          sounds[i] = Mix_LoadWAV(file);
          free (file);
        }
    }
  char *file = NULL;
  if (asprintf(&file, "%s/%s", MAGNETAR_DATADIR, files[SHIP_THRUST]) != -1)
    {
      thrust = Mix_LoadMUS(file);
      Mix_HookMusicFinished(repeat_thrust);
      thrust_sound_on = false;
    }
  free (file);
  file = NULL;
  if (asprintf(&file, "%s/%s", MAGNETAR_DATADIR, files[BONUS]) != -1)
    {
      bonus = Mix_LoadMUS(file);
      //Mix_HookMusicFinished(repeat_bonus);
      thrust_sound_on = false;
    }
  free (file);

}

void sound_pause(bool pause)
{
  if (args.quiet)
    return;
  if (pause)
    Mix_PauseMusic();
  else
    Mix_ResumeMusic();
}

void sound_free()
{
  for (int i = 0; i < MAX_SOUNDS; i++)
    {
      if (sounds[i])
        Mix_FreeChunk(sounds[i]);
    }
  Mix_FreeMusic(thrust);
  Mix_FreeMusic(bonus);
  Mix_CloseAudio();
  SDL_Quit();
}
