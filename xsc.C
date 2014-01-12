//      MAGNETAR
//      Copyright (c) 1993-2005, 2013 by Mark B. Hanson (mbh@panix.com).
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

#include <signal.h>
#include <sys/param.h>
#include <stdio.h>
#include <GL/glx.h>

#include <X11/Xatom.h>
#include <X11/xpm.h>
#include <X11/Xcursor/Xcursor.h>
#include "icon.xpm"

#include "global.h"

#include "random.h"
#include "args.h"
#include "timing.h"
#include "trig.h"
#include "util.h"

#include "xsc.h"

#include "castle.h"
#include "king.h"
#include "minefield.h"
#include "ship.h"
#include "starfield.h"
#include "stats.h"
#include "sound.h"
#include "stages.h"
#include "game.h"

// extern
struct Args::info	args;
char		*program;
Window		game_window;
Display		*display;
int		_wwidth;
int		_wwidth2;
int		_gwheight;
int		_gwheight2;
Stamp		time_now;
int             _display_width, _display_height;
Stages          *stages;
Game            *game;
XpmAttributes   xpmattr; //for the xpm icon
XcursorImage *cursor_image;
Cursor cursor;

namespace {

Starfield	*light_starfield;
bool		iconified = false;
XEvent		event;
double          aspect;
int		screen_number;
XWMHints	wm_hints;
Atom		delete_atom;

const double total_elapsed_time_to_show_wave_complete = 2.0;
const double total_elapsed_time_to_show_get_ready = 3.0;
const double total_elapsed_time_to_add_player_bonuses = 5.0;
const double total_elapsed_time_to_show_player_scores = 2.0;
const double total_elapsed_time_to_show_game_over = 3.07;
const double total_elapsed_time_to_show_attract = 7.0;
const double total_elapsed_time_to_show_high_scores = 5.0;
const double total_elapsed_time_to_enter_initials = 35.0;
const double total_elapsed_time_to_show_initials = 2.0;
const double total_elapsed_time_to_show_copyright = 8.0;
const double total_elapsed_time_to_show_initial_copyright = 2.0;
const double total_elapsed_time_to_show_initial_grid = 0.37;

void
key_repeat(const bool on)
{
    XKeyboardControl kbcontrol;
    static XKeyboardState origkbstate =
	    (XGetKeyboardControl(display, &origkbstate), origkbstate);

    kbcontrol.auto_repeat_mode = (on ?
	    origkbstate.global_auto_repeat : AutoRepeatModeOff);
    XChangeKeyboardControl(display, KBAutoRepeatMode, &kbcontrol);
} // ::key_repeat


void
pause_stuff(const bool pause_state)
{
    game->castle->pause(pause_state);
    game->king->pause(pause_state);
    game->queen->pause(pause_state);
    game->minefield->pause(pause_state); //comment me. how is minefield paused?
    game->ship->pause(pause_state);
    sound_pause(pause_state);
} // :: pause_stuff

void
quit(const int code)
{
    sound_free();
    key_repeat(true);

    delete light_starfield;
    delete stages;
    delete game;


    XFreeCursor (display, cursor);
    XcursorImageDestroy (cursor_image);
    XDestroyWindow(display, game_window);
    free_all_gcs();
    XCloseDisplay(display);

    if (code >= 0)
      exit(code);
} // ::quit


extern "C" RETSIGTYPE
catcher(int sig)
{
    fprintf(stderr, "magnetar: caught signal %d -- shutting down\n", sig);
    quit(EXIT_FAILURE);
} // ::catcher




inline bool
handle_event(void)
{
    if (!XPending(display)) {
	// no events to handle
	return false;
    }

    XNextEvent(display, &event);
    switch (event.type) {
	case Expose:
	    if (event.xexpose.count != 0) {
		// more exposure events on the way, we can ignore this one
		break;
	    }

	    if (game->state != STATE_PAUSED) {
		game->stats()->erase();
		game->king->erase();
		game->queen->erase();
		game->minefield->erase();
		// the other stuff doesn't change shape,
		// so doesn't really need to be erased
	    }

	    game->stats()->draw();
	    light_starfield->draw();
            if (game->state != STATE_HIGH_SCORES &&
                game->state != STATE_PRESS_START)
              {
                game->ship->draw();
                game->castle->draw();
                game->king->draw();
                game->queen->draw();
                game->minefield->draw();
              }

	    // flush events to avoid flickering
	    XSync(display, False);

	    break;
	case ConfigureNotify: {
	    int nw = event.xconfigure.width;
	    int nh = event.xconfigure.height;
	    int ngh = (int)(nh);

	    if (nw == ww() && nh == wh()) 
		break;

            if (nw == 0 || nh == 0)
              break;

            //game->stats()->resize(nw, nh - ngh);
	    game->stats()->resize(nw, ngh);
	    game->ship->resize(nw, ngh);
	    game->king->resize(nw, ngh);
	    game->queen->resize(nw, ngh);
	    game->castle->resize(nw, ngh);
	    game->minefield->resize(nw, ngh);
	    light_starfield->resize(nw, ngh);

            set_window_width(nw);
            set_window_height(nh);
            fill_gcs();
	}
	    break;
	case KeyPress: {
	    KeySym ks = XLookupKeysym(&event.xkey, 0);


            if (ks == XK_BackSpace ||
                (ks == args.p1_fire && (game->state == STATE_ATTRACT ||
                 game->state == STATE_HIGH_SCORES)))
              {
                game->state = STATE_TEST;
                game->cancel_game_and_go_into_test_mode();
                game->state = STATE_COPYRIGHT;
                play (GAME_START);
                game->stats()->draw();
              }
            else if (ks == XK_t)
              {
                game->stats()->erase();
                game->state = STATE_TEST;
                game->cancel_game_and_go_into_test_mode();
                game->stats()->menu->update();
              }
            else if (game->state == STATE_TEST && ks == args.p1_ccw &&
                     game->stats()->menu->is_submenu_selected() == false)
              game->stats()->menu->up();
            else if (game->state == STATE_TEST && ks == args.p1_cw &&
                     game->stats()->menu->is_submenu_selected() == false)
              game->stats()->menu->down();
            else if (game->state == STATE_TEST && 
                     (ks == args.p1_thrust  || ks == args.p1_fire) &&
                     game->stats()->menu->is_submenu_selected() == false)
              {
                game->stats()->menu->select();
                if (game->stats()->menu->is_active("finish"))
                  {
                    game->stats()->finish_test_mode();
                    game->stats()->menu->erase();
                    game->state = STATE_HIGH_SCORES;
                  }
                else if (game->stats()->menu->is_active("reset high score"))
                  {
                    game->reset_high_scores();
                    bool old_quiet = args.quiet;
                    args.quiet = false;
                    play(SHIP_DEFLECT);
                    args.quiet = old_quiet;
                  }
                else if (game->stats()->menu->is_active("restore defaults"))
                  {
                    Args::set_to_factory_defaults(&args);
                    bool old_quiet = args.quiet;
                    args.quiet = false;
                    play(SHIP_DEFLECT);
                    args.quiet = old_quiet;
                  }
              }
            else if (game->state == STATE_TEST && ks == args.p1_ccw &&
                     game->stats()->menu->is_submenu_selected() == true)
              game->stats()->menu->get_selected_submenu()->up();
            else if (game->state == STATE_TEST && ks == args.p1_cw &&
                     game->stats()->menu->is_submenu_selected() == true)
              game->stats()->menu->get_selected_submenu()->down();
            else if (game->state == STATE_TEST && 
                     (ks == args.p1_thrust || ks == args.p1_fire) &&
                     game->stats()->menu->is_submenu_selected() == true)
              {
                if (game->stats()->menu->is_active("sound test"))
                  {
                    int n=game->stats()->menu->get_selected_submenu()->get_active_num();
                    bool old_quiet = args.quiet;
                    args.quiet = false;
                    if (n >= MAX_SOUNDS)
                      {
                        game->stats()->menu->unselect();
                        bonus_sound(false);
                      }
                    else if (n == BONUS)
                      bonus_sound(true);
                    else
                      play(n);
                    args.quiet = old_quiet;
                  }
                else if (game->stats()->menu->get_selected_submenu()->is_submenu_selected() == false)
                  {
                    game->stats()->menu->get_selected_submenu()->select();
                    game->stats()->menu->unselect();
                  }
              }
            else if (game->is_ccw_key (ks)) {
		if (game->state == STATE_NORMAL ||
                    game->state == STATE_WAVE_COMPLETE ||
                    game->state == STATE_ADD_PLAYER_BONUSES ||
                    game->state == STATE_SHOW_PLAYER_SCORE) {
                  if (game->ship->alive())
                    {
                      if (args.flip_p2 && game->p() && game->p()->get_id() % 2 == 0)
                        game->ship->rotate_cw(KEY_DOWN);
                      else
                        game->ship->rotate_ccw(KEY_DOWN);
                    }
		}
                else if (game->state == STATE_INITIALS) {
                  game->stats()->initial_change_left();
                }
	    }
            if (game->is_cw_key (ks)) {
		if (game->state == STATE_NORMAL ||
                    game->state == STATE_WAVE_COMPLETE||
                    game->state == STATE_ADD_PLAYER_BONUSES ||
                    game->state == STATE_SHOW_PLAYER_SCORE) {
                  if (game->ship->alive())
                    {
                      if (args.flip_p2 && game->p() && game->p()->get_id() % 2 == 0)
                        game->ship->rotate_ccw(KEY_DOWN);
                      else
                        game->ship->rotate_cw(KEY_DOWN);
                    }
		}
                else if (game->state == STATE_INITIALS) {
                  game->stats()->initial_change_right();
                }
	    } 
            if (game->is_thrust_key (ks)) {
		if (game->state == STATE_NORMAL ||
                    game->state == STATE_SHOW_PLAYER_SCORE) {
                  if (game->ship->alive())
		    game->ship->thrust(KEY_DOWN);
		}
	    } 
            if (game->is_fire_key(ks)) {
		if (game->state == STATE_NORMAL || 
                    game->state == STATE_SHOW_PLAYER_SCORE) {
                  if (game->ship->alive() && game->king->alive())
		    game->ship->fire(game->stats());
		}
                else if (game->state == STATE_INITIALS) {
                  if (game->stats()->completed_entering_initials() == false)
                    play (SHIP_FIRE_LASER);
                  game->stats()->accept_initial();
                }
	    } 
            if (ks == args.pause) {
		if (game->state == STATE_OVER || game->state == STATE_HIGH_SCORES || game->state == STATE_ATTRACT || game->state == STATE_PRESS_START || iconified || game->state == STATE_TEST || game->state == STATE_COPYRIGHT) {
                  ;
		} else {
		    if (game->state == STATE_PAUSED) {
			game->state = STATE_NORMAL;
			pause_stuff(false);
			key_repeat(false);
		    } else {
			game->state = STATE_PAUSED;
			pause_stuff(true);
			key_repeat(true);
		    }
		}
	    } 
            if (ks == args.add_coin) {
              game->addCoin();
		if (game->state == STATE_HIGH_SCORES ||
                    game->state == STATE_ATTRACT ||
                    game->state == STATE_PRESS_START ||
                    game->state == STATE_COPYRIGHT) {
                    game->stats()->erase();
                    game->state = STATE_PRESS_START;
                    game->king->erase();
                    game->queen->erase();
                    game->castle->erase();
                    game->ship->erase();
                    game->minefield->erase();
                    light_starfield->erase();
                    light_starfield->draw();
                    game->stats()->erase();
                    game->stats()->draw();
                    game->pilot->reset();
		}
	    }
            if (ks == args.one && args.free_play && (game->state == STATE_HIGH_SCORES || game->state == STATE_ATTRACT || game->state == STATE_PRESS_START))
              {
              if (game->castle->alive())
                game->castle->zap();
              game->stats()->erase();
              game->king->erase();
              game->queen->erase();
              game->start(1);
              stages->start_stage(game->stats()->get_stage());
              game->state = STATE_NORMAL;
              }
            if (ks == args.two && args.free_play && (game->state == STATE_HIGH_SCORES || game->state == STATE_ATTRACT || game->state == STATE_PRESS_START))
              {
              if (game->castle->alive())
                game->castle->zap();
              game->stats()->erase();
              game->king->erase();
              game->queen->erase();
              game->start(2);
              stages->start_stage(game->stats()->get_stage());
              game->state = STATE_GET_READY;
              }

            if (ks == args.one && (game->state == STATE_PRESS_START  || game->state == STATE_HIGH_SCORES || game->state == STATE_COPYRIGHT) && game->canStart(1)) {
              if (game->castle->alive())
                game->castle->zap();
              game->stats()->erase();
              game->king->erase();
              game->queen->erase();
              game->start(1);
              stages->start_stage(game->stats()->get_stage());
              game->state = STATE_NORMAL;
            }
            if (ks == args.two && (game->state == STATE_PRESS_START || game->state == STATE_HIGH_SCORES || game->state == STATE_COPYRIGHT) && game->canStart(2)) {
              if (game->castle->alive())
                game->castle->zap();
              game->stats()->erase();
              game->king->erase();
              game->queen->erase();
              game->start(2);
              stages->start_stage(game->stats()->get_stage());
              game->state = STATE_GET_READY;
            }
            if (ks == args.quit) {
		quit(EXIT_SUCCESS);
	    }
            if (ks == args.reset)
              {
                if (game->castle->alive())
                  game->castle->zap();
                game->stats()->erase();
                game->king->erase();
                game->queen->erase();
                game->minefield->erase();
                light_starfield->erase();
                game->cancel_game_and_go_into_test_mode();
                game->reset_soon();
                game->state = STATE_PATTERN;
              }
	}
	    break;
	case KeyRelease: {
	    KeySym ks = XLookupKeysym(&event.xkey, 0);

	    if (game->is_ccw_key(ks)) {
		if (game->state == STATE_NORMAL ||
                    game->state == STATE_WAVE_COMPLETE ||
                    game->state == STATE_ADD_PLAYER_BONUSES ||
                    game->state == STATE_SHOW_PLAYER_SCORE) {
                  if (args.flip_p2 && game->p() && game->p()->get_id() % 2 == 0)
                    game->ship->rotate_cw(KEY_UP);
                  else
                    game->ship->rotate_ccw(KEY_UP);
		}
	    } 
            if (game->is_cw_key(ks)) {
		if (game->state == STATE_NORMAL ||
                    game->state == STATE_WAVE_COMPLETE ||
                    game->state == STATE_ADD_PLAYER_BONUSES ||
                    game->state == STATE_SHOW_PLAYER_SCORE) {
                  if (args.flip_p2 && game->p() && game->p()->get_id() % 2 == 0)
                    game->ship->rotate_ccw(KEY_UP);
                  else
                    game->ship->rotate_cw(KEY_UP);
		}
	    } 
            if (game->is_thrust_key (ks)) {
		if (game->state == STATE_NORMAL ||
                    game->state == STATE_WAVE_COMPLETE ||
                    game->state == STATE_ADD_PLAYER_BONUSES ||
                    game->state == STATE_SHOW_PLAYER_SCORE) {
                  game->ship->thrust(KEY_UP);
		}
	    }
            if (ks == args.reset)
              game->reset();
	}
	    break;
	case LeaveNotify:
	    if (game->state == STATE_NORMAL) {
                if (args.nopause == false)
                  {
                    game->state = STATE_PAUSED;
                    pause_stuff(true);
                  }
	    }
	    key_repeat(true);
	    break;
	case EnterNotify:
	    if (game->state == STATE_OVER) {
		key_repeat(false);
	    }
	    break;
	case MapNotify:
	    iconified = false;
	    break;
	case UnmapNotify:
	    iconified = true;
	    if (game->state == STATE_NORMAL) {
		game->state = STATE_PAUSED;
		pause_stuff(true);
	    }
	    key_repeat(true);
	    break;
	case ClientMessage:
	    if (event.xclient.format == 32 &&
		    (Atom)event.xclient.data.l[0] == delete_atom) {
		quit(EXIT_SUCCESS);
	    }
	    break;
	default:
	    // ignore it.
	    break;
    }

    return true;
} // ::handle_event


bool
change_states()
{
  if (game->state == STATE_PATTERN && game->get_reset_soon_flag() == false)
    {
      //okay we are just beginning.
      static double age;
      age += (1.0/args.fps);
      if (age > total_elapsed_time_to_show_initial_grid)
        {
          age = 0;
          game->state = STATE_COPYRIGHT;
          draw_grid(false);
          play (STAGE_START);
          return false;
        }
    }
  else if (game->state == STATE_NORMAL && game->castle->resting() && !game->king->alive())
    {
      game->state = STATE_SHOW_PLAYER_SCORE;
      return false;
    }
  else if (game->state == STATE_NORMAL && !game->ship->alive() &&
           game->ship->resting() && game->stats()->lives > -1 && stages->challenge() == false)
    {
      /*the time of death gets set when ship is hit. */
      game->state = STATE_SHOW_PLAYER_SCORE;
      game->king->erase();
      game->queen->erase();
      game->castle->erase();
      game->ship->erase();
      game->minefield->erase();
      return false;
    }
  else if (game->state == STATE_NORMAL && !game->ship->alive() &&
           game->ship->resting() && stages->challenge() == true)
    {
      game->stats()->lives++;
      game->stats()->set_died_on_challenging_stage();
      int remaining = stages->get_max_mines() - game->get_expired_mines();
      game->stats()->add_expired_mines(remaining, false);
      game->minefield->erase();
      game->minefield->snuff();
      game->ship->reincarnate();
      return true;
    }
  else if (game->state == STATE_NORMAL && !game->ship->alive() && game->stats()->lives < 0)
    {
      game->state = STATE_OVER;
      play (GAME_OVER);
      return false;
    }
  else if (game->state == STATE_COPYRIGHT)
    {
      static bool first = true;
      static double age;
      age += (1.0/args.fps);
      if (age > total_elapsed_time_to_show_initial_copyright && first)
        {
          age = 0.0;
          game->state = STATE_HIGH_SCORES;
          first = false;
        }
      else if (age > total_elapsed_time_to_show_copyright && !first)
        {
          age = 0.0;
          game->state = STATE_HIGH_SCORES;
          play (GAME_OVER);
        }
      return true;
    }
  else if (game->state == STATE_WAVE_COMPLETE)
    {
        static double age;
        age += (1.0/args.fps);
        if (age > total_elapsed_time_to_show_wave_complete)
          {
            age = 0.0;
            game->state = STATE_ADD_PLAYER_BONUSES;
              craziness = 0;
            return true;
          }
        return true;
    }
  else if (game->state == STATE_ADD_PLAYER_BONUSES)
    {
        static double age;
        age += (1.0/args.fps);
        if (age > total_elapsed_time_to_add_player_bonuses)
          {
            bonus_sound(false);
            age = 0.0;
            //here we reset the king and then kill it
            //the reset is so that we don't see the glitchy sliding effect
            //during the player scores.
            //and the hit is so that we start the next stage after the
            //player scores are shown.
            game->king->reset();
            game->king->hit(NULL, NULL, NULL, NULL);
            game->stats()->clear_various_wave_statistics();
            game->ship->reset_energy(); // so ship can thrust in prepare mode.
            game->state = STATE_SHOW_PLAYER_SCORE;
            if (stages->challenge_upcoming())
              play (CHALLENGING_STAGE_PREPARE);
            else
              play (STAGE_PREPARE);
            return true;
          }
        return true;
    }
  else if (game->state == STATE_SHOW_PLAYER_SCORE)
    {
      static double age;
      age += (1.0/args.fps);
      if (age > total_elapsed_time_to_show_player_scores)
        {
          age = 0.0;
          if (game->ship->alive() == false && game->stats()->lives < 0)
            {
              game->king->hit(NULL, NULL, NULL, NULL);
              game->queen->hit(NULL, NULL, NULL, NULL);
              game->state = STATE_OVER;
              play (GAME_OVER);
              game->stats()->write_stats_record();
              return true;
            }
          if (game->count_number_of_players_alive() > 1 &&
              game->ship->is_respawning())
            {
              game->state = STATE_GET_READY;
              game->stats()->erase();
              game->minefield->erase();
              game->castle->erase();
              game->king->erase();
              game->queen->erase();
              game->ship->erase();
              game->nextPlayer();
              stages->start_stage(game->stats()->get_stage());
              play (STAGE_START);
              game->stats()->clear_various_wave_statistics();
              game->ship->reset_energy(); //this is so we can thrust in prepare mode.
              game->castle->reset();
            }
          else
            {
              game->state = STATE_NORMAL;
              if (game->ship->alive() == false)
                game->ship->reincarnate();
              if (!game->king->alive())
                {
                  game->stats()->next_wave();
                  stages->start_stage(game->stats()->get_stage());
                  play (STAGE_START);
                }
            }
          return true;
        }
      return true;
    }
  else if (game->state == STATE_INITIALS && game->stats()->completed_entering_initials() == false)
    {
      static double age;
      age += (1.0/args.fps);
      game->stats()->set_seconds_left_to_enter_initials(total_elapsed_time_to_enter_initials - age);
      if (age > total_elapsed_time_to_enter_initials)
        {
          bonus_sound(false);
          age = 0.0;
          strcpy (game->high_score_initials, game->stats()->get_initials());
          game->high_score = game->stats()->score;
          game->save_high_scores();
          if (game->count_number_of_players_alive() > 0)
            {
              game->state = STATE_GET_READY;
              game->stats()->erase();
              game->minefield->erase();
              game->castle->erase();
              game->king->erase();
              game->queen->erase();
              game->ship->erase();
              game->nextPlayer();
              stages->start_stage(game->stats()->get_stage());
              play (STAGE_START);
              game->stats()->clear_various_wave_statistics();
              game->castle->reset();
            }
          else
            {
              game->state = STATE_HIGH_SCORES;
              game->king->reincarnate();
              game->queen->reincarnate();
            }
          return false;
        }
    }
  else if (game->state == STATE_INITIALS && game->stats()->completed_entering_initials() == true)
    {
      static double age;
      age += (1.0/args.fps);
      if (age > total_elapsed_time_to_show_initials)
        {
          bonus_sound(false);
          age = 0.0;
          strcpy (game->high_score_initials, game->stats()->get_initials());
          game->save_high_scores();
          if (game->count_number_of_players_alive() > 0)
            {
              game->state = STATE_GET_READY;
              game->stats()->erase();
              game->minefield->erase();
              game->castle->erase();
              game->king->erase();
              game->queen->erase();
              game->ship->erase();
              game->nextPlayer();
              stages->start_stage(game->stats()->get_stage());
              play (STAGE_START);
              game->stats()->clear_various_wave_statistics();
              game->castle->reset();
            }
          else
            {
              game->state = STATE_HIGH_SCORES;
              game->king->reincarnate();
              game->queen->reincarnate();
            }
          return false;
        }
    }
  else if (game->state == STATE_OVER)
    {
      static double age;
      age += (1.0/args.fps);
      if (age > total_elapsed_time_to_show_game_over)
        {
          age = 0.0;
          if (game->stats()->score > game->high_score)
            {
              game->high_score = game->stats()->score;
              game->state = STATE_INITIALS;
              play (CONGRATULATIONS);
              bonus_sound(true);
              game->stats()->erase();
              game->minefield->erase();
              game->castle->erase();
              game->king->erase();
              game->queen->erase();
              game->ship->erase();
              game->stats()->clear_various_wave_statistics();
              game->castle->reset();
              game->minefield->snuff();
              game->minefield->reset(0, 0, 0);
              game->king->hit (NULL, NULL, NULL, NULL);
              game->queen->hit (NULL, NULL, NULL, NULL);
              return false;
            }
          else
            {
              if (game->count_number_of_players_alive() > 0)
                {
                  game->state = STATE_GET_READY;
                  game->stats()->erase();
                  game->minefield->erase();
                  game->castle->erase();
                  game->king->erase();
                  game->queen->erase();
                  game->ship->erase();
                  game->nextPlayer();
                  stages->start_stage(game->stats()->get_stage());
                  play (STAGE_START);
                  game->stats()->clear_various_wave_statistics();
                  game->castle->reset();
                }
              else
                {
                  game->state = STATE_HIGH_SCORES;
                  game->king->erase();
                  game->queen->erase();
                  game->castle->erase();
                  game->ship->erase();
                  game->minefield->erase();
                }
            }
          return false;
        }
    }
  else if (game->state == STATE_HIGH_SCORES)
    {
      static double age;
      age += (1.0/args.fps);
      if (age > total_elapsed_time_to_show_high_scores)
        {
          age = 0.0;
          game->castle->refresh();
          game->minefield->erase();
          game->minefield->snuff();
          game->castle->reset();
          if (game->get_coins() > 0)
            game->state = STATE_PRESS_START;
          else
            {
              game->state = STATE_ATTRACT;
              game->pilot->reset();
              game->king->reset();
              game->queen->reset();
              game->minefield->reset(stages->get_stage().get_max_mines(),stages->get_stage().get_mines_at_a_time(), stages->get_stage().get_magnetic_mine_percent());
              game->minefield->set_drop_zones(stages->get_stage().get_drop_zones());
              game->king->add_attract_fireballs();
            }
        }
    }
  else if (game->state == STATE_ATTRACT)
    {
      static double age;
      age += (1.0/args.fps);
      if (age > total_elapsed_time_to_show_attract)
        {
          age = 0.0;
          game->state = STATE_HIGH_SCORES;
          game->pilot->reset();
          game->king->erase();
          game->queen->erase();
          game->castle->erase();
          game->ship->erase();
          game->minefield->erase();
          return false;
        }
      else if (age + (1.0/args.fps) > total_elapsed_time_to_show_attract)
        {
          //hackish.
          game->ship->erase();
          return false;
        }
    }
  else if (game->state == STATE_PRESS_START)
    {
      game->king->erase();
      game->queen->erase();
      game->castle->erase();
      game->ship->erase();
      return false;
    }
  else if (game->state == STATE_GET_READY)
    {
      static double age;
      age += (1.0/args.fps);
      if (age > total_elapsed_time_to_show_get_ready)
        {
          age = 0.0;
          game->state = STATE_NORMAL;
          return true;
        }
      light_starfield->erase();
      light_starfield->turn();
      light_starfield->draw();
      game->ship->draw();
      game->stats()->erase();
      game->stats()->draw();
      game->king->erase();
      game->queen->erase();
      game->castle->erase();
      return false;
    }

    if (game->state == STATE_PAUSED || game->state == STATE_HIGH_SCORES ||
        game->state == STATE_PATTERN)
      return false;

    return true;
}

inline void
animate(void)
{
    // erase the last frame
    light_starfield->erase();
    if (!game)
      return;
    game->king->erase();
    game->queen->erase();
    game->castle->erase();
    game->ship->erase();
    game->minefield->erase();
    game->stats()->erase();

    if (game->state == STATE_ATTRACT)
      {
        game->pilot->turn();
        game->pilot->move();
      }
    // calculate new positions
    game->ship->turn();
    game->ship->move(game->castle, game->king, game->queen, game->minefield, game->stats());
    light_starfield->turn();
    /*
    if (game->state == STATE_ATTRACT)
      king->attract();
    else
    */
    game->king->turn(game->castle, game->ship, game->stats());
    game->queen->turn(game->castle, game->ship, game->stats());
    game->castle->turn(game->minefield, game->king, game->stats(), game);
    game->minefield->turn(game->castle, game->ship);
    game->minefield->move(game->castle, game->ship, game->king, game->stats());
    if (game->state != STATE_HIGH_SCORES && game->state != STATE_OVER && game->state != STATE_INITIALS && game->state != STATE_GET_READY && game->state != STATE_WAVE_COMPLETE && game->state != STATE_PRESS_START)
      game->minefield->launch(game->king, game->castle, game->stats());	// maybe launch another buzzer
    int fallen = game->minefield->expire(); //maybe snuff a mine
    if (fallen)
      {
        //end of stage/level/wave.
        if (game->ship->alive() && stages->challenge() == false &&
            game->ship->is_respawning() == false)
          {
            game->stats()->add_score (10 * fallen);
            play (ASTEROID_LEAVING);
          }
        else if (game->state == STATE_ATTRACT)
          play (ASTEROID_LEAVING);
      game->stats()->add_expired_mines(fallen, game->ship->alive() && stages->challenge() == false);
      }
    if (stages->challenge())
      {
        if (game->get_expired_mines() > 0)
          {
            //printf("stages->get_max_mines() is %d\n", stages->get_max_mines());
            //printf("stats->get_expired_mines() is %d\n", stats->get_expired_mines());
          }
      }
      if (game->get_expired_mines() >= stages->get_max_mines() &&
          game->state != STATE_WAVE_COMPLETE &&
          game->state != STATE_ADD_PLAYER_BONUSES &&
          game->state != STATE_OVER)
        {
          game->king->hit (NULL, NULL, NULL, NULL);
          game->queen->hit (NULL, NULL, NULL, NULL);
          if (game->castle->alive())
            game->castle->explode(game->stats());
          else
            game->castle->reset();
          game->minefield->snuff();
          if (game->state == STATE_NORMAL ||
              game->state == STATE_SHOW_PLAYER_SCORE)
            {
              game->stats()->write_stats_record();
              game->state=STATE_WAVE_COMPLETE;
              game->ship->reset_impervious();
              bonus_sound(true);
              if (stages->challenge() == false)
                craziness = 1;
              else
                {
                  if (game->stats()->died_on_challenging_stage == false)
                    game->stats()->add_score(100); 
                //did we shoot them all?
                if (game->stats()->get_mines_shot() >=
                    stages->get_max_mines() * 0.70 &&
                    game->stats()->died_on_challenging_stage == false)
                  game->stats()->add_score(1000);
                }
            }
          //state change, so return.
          return;
        }

    // draw new frame
    //if (game->state != STATE_PRESS_START)
    light_starfield->draw();
    game->stats()->draw();
    game->king->draw();
    game->queen->draw();
    game->castle->draw();
    game->ship->draw();
    game->minefield->draw();

    // flush events
    XSync(display, False);
} // ::animate


void
snooze(void)
{
  static const long desired = (long)((1.0 / args.fps) * 1000000);
  static Stamp tv_begin = (XSCTime::now(&tv_begin), tv_begin);
  static long sleepfor;

  XSCTime::now(&time_now);
  sleepfor += desired - (time_now - tv_begin).micros();

  if (sleepfor > 0) {
    // sleep if we have the time to spare
    XSCTime::sleep(sleepfor);
  } else if (sleepfor < desired * -2L) {
    // keep the game from running wild if we have a sleep deficit
    sleepfor = desired * -2L;
  }

  tv_begin = time_now;
} // ::snooze

} // namespace


static Bool wait_for_window( Display *d, XEvent *e, XPointer arg ) {
      return (e->type == MapNotify) && (e->xmap.window == (Window) arg);
}

void 
set_display_width(int w)
{
    _display_width = w;
}

void 
set_display_height(int h)
{
  _display_height = h;
}

int 
display_width()
{
  return _display_width;
}

int 
display_height()
{
  return _display_height;
}

void set_window_width(int w)
{
  _wwidth = w;
  _wwidth2 = w/2;
}

void set_window_height(int h)
{
  _gwheight = h;
  _gwheight2 = h / 2;
}

int ww()
{
  return _wwidth;
}

int wh()
{
  return _gwheight;
}

int ww2()
{
  return _wwidth2;
}

int wh2()
{
  return _gwheight2;
}

int
main(const int argc, char **const argv)
{
  char *display_name = NULL;
  char title[] = "MAGNETAR";
  char *window_title = title;
  char *icon_title = title;
  Pixmap icon, mask;
  XSizeHints size_hints;
  XClassHint class_hints;
  XTextProperty windowName, iconName;


  program = argv[0];

  //printf("%s version %s\n", title, VERSION);

  XSCTime::now(&time_now);
  Random::seed((unsigned int)time_now.get_usec());

  Trig::init();

  Args::init(&args);
  Args::load(&args);
  Args::set(&args, argc, argv);

  Difficulty::init();

  sound_init();

  if ((display = XOpenDisplay(display_name)) == NULL) {
    fprintf(stderr, "%s: cannot connect to X server %s\n",
            program, XDisplayName(display_name));
    exit(1);
  }

  screen_number = DefaultScreen(display);

  set_display_width(DisplayWidth(display, screen_number));
  set_display_height(DisplayHeight(display, screen_number));

  if (args.aspect_x == -1 || args.aspect_y == -1)
    {
      args.aspect_x = display_width();
      args.aspect_y = display_height();
    }

  set_window_height((int)(display_height() * args.percent / 100.0));
  aspect = (double)args.aspect_x / (double)args.aspect_y;
  set_window_width((int)(wh() * aspect));


  //XSetWindowAttributes atts;
  init_gc(args.colour);

  game_window = XCreateSimpleWindow(display,
                                    RootWindow(display, screen_number),
                                    0, 0,
                                    ww(), wh(),
                                    0,
                                    args.colour != 3 ? WhitePixel(display, screen_number) : BlackPixel(display, screen_number),
                                    args.colour != 3 ? BlackPixel(display, screen_number) : WhitePixel(display, screen_number));

  if (XStringListToTextProperty(&window_title, 1, &windowName) == 0) {
    fprintf(stderr, "%s: structure allocation for windowName failed.\n",
            program);
    exit(1);
  }

  if (XStringListToTextProperty(&icon_title, 1, &iconName) == 0) {
    fprintf(stderr, "%s: structure allocation for iconName failed.\n",
            program);
    exit(1);
  }

  size_hints.flags = PSize | PAspect | PMinSize | PMaxSize;
  size_hints.min_aspect.x =
    size_hints.max_aspect.x =
    size_hints.min_width = (int)(100.0 * aspect);
  size_hints.min_aspect.y =
    size_hints.max_aspect.y =
    size_hints.min_height = 100;
  size_hints.max_width = display_width();
  size_hints.max_height = display_height();

  wm_hints.flags = StateHint | IconPixmapHint | InputHint;
  wm_hints.initial_state = NormalState;
  wm_hints.input = True;
  XpmCreatePixmapFromData(display, game_window, icon_xpm, &icon, &mask,
                          &xpmattr);
  wm_hints.icon_pixmap = icon;


  class_hints.res_name = program;
  class_hints.res_class = window_title;

  XSetWMProperties(display, game_window, &windowName, &iconName, argv, argc,
                   &size_hints, &wm_hints, &class_hints);

  delete_atom = XInternAtom(display, "WM_DELETE_WINDOW", False);

  XSetWMProtocols(display, game_window, &delete_atom, 1);

  XSelectInput(display, game_window, (ExposureMask | KeyPressMask |
                                      KeyReleaseMask | StructureNotifyMask | LeaveWindowMask |
                                      EnterWindowMask));


  XMapWindow(display, game_window);
  XIfEvent(display, &event, wait_for_window, (XPointer) game_window);
  XNextEvent(display, &event);
  XSync(display, True);

  cursor_image = XcursorImageCreate (1, 1);
  cursor = XcursorImageLoadCursor (display, cursor_image);
  XDefineCursor (display, game_window, cursor);


  if (args.x != -1 && args.x != -1)
    {
      XWindowChanges changes;
      changes.x = args.x;
      changes.y = args.y;
      XConfigureWindow  (display, game_window, CWX | CWY, &changes);
    }

  game = new Game();
  game->cancel_game_and_go_into_test_mode();
  game->state = STATE_PATTERN;
  light_starfield = new Starfield(11, true);
  stages = new Stages(game->king, game->queen, game->minefield, game->castle, game->ship, light_starfield, game);

  fill_gcs();

  key_repeat(false);

  signal(SIGHUP, catcher);
  signal(SIGINT, catcher);
  signal(SIGQUIT, catcher);
  //signal(SIGSEGV, catcher);
  signal(SIGTERM, catcher);

  bool first = true;
  for (;;) {
    while (handle_event());
    bool do_animation = change_states();
    if (game->get_reset_flag())
      break;
    if (game->changed()) {
	game->stats()->erase();
	game->stats()->draw();
    }
    if (do_animation == false && game->state == STATE_HIGH_SCORES)
      game->ship->erase();
    if (do_animation)
      animate();
    if (game->state == STATE_SHOW_PLAYER_SCORE)
      if (game->ship->alive())
        game->stats()->draw();
    snooze();
    if (first)
      first = false;

    if (game->get_reset_soon_flag() || game->state == STATE_PATTERN)
      draw_grid(true);

  }

  bool reset = game->get_reset_flag();
  quit(-1); //restart the program
  if (reset)
    execvp (argv[0], argv);
  exit (EXIT_FAILURE);
} // ::main
