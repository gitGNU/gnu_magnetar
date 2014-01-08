//      MAGNETAR
//      Copyright (c) 1993-2005 by Mark B. Hanson (mbh@panix.com).
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

#include "timing.h"
#include "util.h"
#include "xsc.h"
#include "sound.h"
#include "stages.h"

#include "castle.h"
#include "random.h"
#include "game.h"

    
const int Castle::points_awarded = 200;

namespace {

const double scales[MAX_RINGS] = { 23.65-5 , 30-5, 44.65-5 };
//const double scales[MAX_RINGS] = { 23.65 , 30, 44.65 };

} // namespace


Castle::Castle(void)
{
    //fprintf(stderr, "Castle::Castle()\n");
    numrings = MAX_RINGS;
    rings = new Ring *[numrings];

    for (int i = 0; i < numrings; i++) {
	rings[i] = new Ring(scales[i], (Difficulty::ring_speed[i] / args.fps), 0, fetch_gc(GC_BLACK), -1);
        //rings[i]->set_x(155);
        //rings[i]->set_y(55);
    }
    pause_sum = 0;
    reset();
} // Castle::Castle


Castle::~Castle(void)
{
    //fprintf(stderr, "Castle::~Castle()\n");
    for (int i = 0; i < MAX_RINGS; i++) {
	delete rings[i];
    }
    delete[] rings;
} // Castle::~Castle

double
Castle::get_x() const
{
  return rings[0]->get_x();
}
double
Castle::get_y() const
{
  return rings[0]->get_y();
}
double
Castle::get_diag() const
{
  return rings[0]->get_diag();
}
void
Castle::jump()
{
  int x = (Random::get() % int(ww() * 0.75)) - (ww() * 0.80/2);
  int y = (Random::get() % int(wh() * 0.95)) - (wh() * 0.95/2);
    for (int i = 0; i < MAX_RINGS; i++) {
      rings[i]->set_x(ww2() + x);
      rings[i]->set_y(wh2()+ y);
    }
}

void
Castle::render(const bool ink)
{
  if (!alive() && state != CASTLE_STATE_COLLAPSING)
    return;
    for (int i = 0; i < numrings; i++) {
	rings[i]->render(ink);
    }
} // Castle::render


void
Castle::turn(Minefield *minefield, King *king, Stats *stats, Game *game)
{
    bool go_again;

    for (int i = 0; i < numrings; i++) {
	rings[i]->turn();
    }
    if (state == CASTLE_STATE_NORMAL) {

      if ((time_now - time_of_death) - pause_sum > resting_time + staying_time)
        {
          play (REWARD_LEAVING);
        explode(stats);
        }
    } else if (state == CASTLE_STATE_REGENERATING) {
	// if currently regenerating, slowly increase the size of the rings
	go_again = false;
	for (int i = 0; i < numrings; i++) {
	    double tmp = rings[i]->get_scale();

	    if (tmp > scales[i]) {
		rings[i]->set_scale(tmp -
			(((tmp - scales[i]) * 4.00) / args.fps));
	    }

	    // close enough
	    if (rings[i]->get_scale() - scales[i] < 0.01) {
		rings[i]->set_scale(scales[i]);
	    } else {
		go_again = true;
	    }
	}
	if (!go_again) {
	    state = CASTLE_STATE_NORMAL;
	}
    } else if (state == CASTLE_STATE_COLLAPSING) {
	// collapse the rings inward
        go_again = false;
	for (int i = 0; i < numrings; i++) {
	    double tmp = rings[i]->get_scale();

	    if (tmp < 1000.0) {
		rings[i]->set_scale(tmp + (tmp * 8.0) / args.fps);
	    }

	    // close enough
	    if (1000.0 - rings[i]->get_scale() < 0.01) {
		rings[i]->set_scale(1000.0);
	    } else {
		go_again = true;
	    }
	}
        if (!go_again) {
          for (int i = 0; i < numrings; i++) {
            rings[i]->set_scale(0.0);
            rings[i]->snuff();
          }
          state = CASTLE_STATE_EXPLODING;
        }
    } else if (state == CASTLE_STATE_EXPLODING) {
	// expand rings beyond the edge of the screen
        go_again = false;
        if (!go_again) {
          state = CASTLE_STATE_RESTING;
          time_of_death = time_now; 
          pause_sum = 0;
          reset();
        }
    } else if (state == CASTLE_STATE_RESTING) {
	if ((time_now - time_of_death) - pause_sum > resting_time) {
          if (minefield->no_mines_left_to_launch())
            {
              //too close to end of level to respawn castle.
              reset ();
              return;
            }
          else if (game->state != STATE_ATTRACT &&
                   game->state != STATE_NORMAL)
            {
              //hey we're not actually playing the game, bub.
              reset ();
              return;
            }
          else if (stages->challenge() && game->state == STATE_NORMAL)
            {
              //hey man, we're in a challenging stage, man.
              reset ();
              return;
            }
	    state = CASTLE_STATE_REGENERATING;
            play (REWARD_ENTERING);
            // this is where we make the scales a little smaller so that
            // the rings can spin and grow into their proper shape.
            // we also change the spinning speeds and directions here
            // because they might have changed in Difficulty::increase.
	    for (int i = 0; i < numrings; i++) {
		if (i == numrings - 1) { 
		    rings[i]->set_scale(100);
		} else {
		    rings[i]->set_scale(scales[i + 1]);
		}
		rings[i]->refresh();
                rings[i]->set_theta(0.0);
                rings[i]->set_dtheta(Difficulty::ring_speed[i] / args.fps);
	    }
	}
    } else if (!rings[0]->remaining()) {
	// enemy has destroyed entire outer ring
	Ring *tmpwall = rings[0];

		    
	for (int i = 0; i < numrings - 1; i++) {
	    rings[i] = rings[i + 1];
	}
	rings[numrings - 1] = tmpwall;
	rings[numrings - 1]->set_scale(100); //hmm was 13
	rings[numrings - 1]->refresh();

	for (int i = 0; i < numrings; i++) {
	    rings[i]->set_dtheta(Difficulty::ring_speed[i] / args.fps);
	}
	state = CASTLE_STATE_REGENERATING;
        play (REWARD_ENTERING);
	//minefield->upgrade(this);
        stats->regenerated_a_ring();
    }
} // Castle::turn

int
Castle::get_resting_time() const
{
  //return 3;
  //return 13 + Random::get() % 18;
  return 3 + (Random::get() % 9);
}

int
Castle::get_staying_time() const
{
  return 13 + Random::get() % 4;
}

void
Castle::resize(const int nwidth, const int nheight)
{
    for (int i = 0; i < numrings; i++) {
	rings[i]->resize(nwidth, nheight);
    }
} // Castle::resize


bool
Castle::buzzer_collision(Buzzer *buzzer, Ship *ship, Stats *stats)
{
  if (!alive())
    return false;
  bool dead = false;
    for (int i = 0; i < numrings; i++) {
    if (hypot(buzzer->get_x() - get_x(), buzzer->get_y() - get_y()) <
	    (get_diag() + buzzer->get_diag()) / 2.0) {
	    if (rings[i]->hit(buzzer, ship->alive() ? stats : NULL, &dead)) {
              stats->hit_a_ring_segment();
              play (ASTEROID_EXPLODE);
              if (dead) {
                stats->destroyed_a_ring_segment();
              }
              return true;
	    }
	}
    }
    return false;
}

bool
Castle::collision(Laser *laser, Stats *stats)
{
  bool dead = false;
  if (!alive())
    return false;
    for (int i = 0; i < numrings; i++) {
	//if (within(laser->get_radius(), (rings[i]->get_size() / 2.0), 1.5)) {
    if (hypot(laser->get_x() - get_x(), laser->get_y() - get_y()) <
	    (get_diag() + laser->get_diag()) / 2.0) {
	    if (rings[i]->hit(laser, stats, &dead)) {
              stats->hit_a_ring_segment();
              if (dead) {
                stats->destroyed_a_ring_segment();
              }
              return true;
	    }
	}
    }
    return false;
} // Castle::collision


bool
Castle::opening(const double degrees) const
{
    for (int i = 0; i < numrings; i++) {
	if (!rings[i]->hole(degrees)) {
	    return false;
	}
    }
    return true;
} // Castle::opening


bool
Castle::hole(const int whichring, const double degrees) const
{
    return rings[whichring]->hole(degrees);
} // Castle::hole


double
Castle::ring_size(const int wallnum) const
{
    return rings[wallnum]->get_size();
} // Castle::ring_size


void
Castle::seg_center(const int ring_idx, /*const*/ double degrees,
	double * /*const*/ xptr, double * /*const*/ yptr)
{
    rings[ring_idx]->seg_center(degrees, xptr, yptr);
} // Castle::seg_center

int
Castle::get_random_reward()
{
  int r = Random::get() % 100;
  if (r < 55)
    return 0;
  else if (r < 80)
    return 1;
  else if (r < 96)
    return 2;
  else if (r < 98)
    return 3;
  else
    return 4;
  return 0;
}

void 
Castle::reset()
{
  reward_type = get_random_reward();
  state = CASTLE_STATE_RESTING;
  time_of_death = time_now; 
  int secs = get_resting_time();
  resting_time = secs * 1000000;
  secs = get_staying_time();
  staying_time = secs * 1000000;
  if (!game || 
      game->state == STATE_ATTRACT  || game->state == STATE_HIGH_SCORES ||
      game->state == STATE_COPYRIGHT)
    {
      if (reward_type == 2 || reward_type == 3 || reward_type == 4)
        reward_type = 1;
      resting_time /= 2;
    }
  jump();
  for (int i = 0; i < numrings; i++)
    rings[i]->set_gc(fetch_gc(GC_BLACK));
  switch (reward_type)
    {
    case 0:
      numrings = 1;
      rings[0]->set_gc(fetch_gc(GC_DULL_YELLOW));
      break;
    case 1:
      numrings = 2;
      rings[0]->set_gc(fetch_gc(GC_BRIGHT_ORANGE));
      rings[1]->set_gc(fetch_gc(GC_BRIGHT_ORANGE));
      break;
    case 2:
      numrings = 3;
      rings[0]->set_gc(fetch_gc(GC_BRIGHT_BLUE));
      rings[1]->set_gc(fetch_gc(GC_GREEN));
      rings[2]->set_gc(fetch_gc(GC_BRIGHT_GREY));
      break;
    case 3:
      numrings = 3;
      rings[0]->set_gc(fetch_gc(GC_BRIGHT_ORANGE));
      rings[1]->set_gc(fetch_gc(GC_BRIGHT_RED));
      rings[2]->set_gc(fetch_gc(GC_BRIGHT_ORANGE));
      break;
    case 4:
      numrings = 3;
      rings[0]->set_gc(fetch_gc(GC_BRIGHT_GREY));
      rings[1]->set_gc(fetch_gc(GC_BRIGHT_GREY));
      rings[2]->set_gc(fetch_gc(GC_BRIGHT_GREY));
      break;
    }
}

void
Castle::explode(Stats *stats)
{
  state = CASTLE_STATE_COLLAPSING;
  time_of_death = time_now; 
} // Castle::explode

void
Castle::zap()
{
  erase();
  state = CASTLE_STATE_RESTING;
  time_of_death = time_now; 
  reset();
} // Castle::zap

void
Castle::refresh(void) const
{
    for (int i = 0; i < numrings; i++) {
	rings[i]->refresh();
    }
} // Castle::refresh
    
int 
Castle::get_number_of_remaining_segments() const
{
  int remaining = 0;
  for (int i = 0; i < numrings; i++) 
    remaining = rings[i]->remaining();
  return remaining;
}

int 
Castle::get_total_number_of_segments() const
{
  int segs = 0;
  for (int i = 0; i < numrings; i++) 
    segs = rings[i]->get_num_segments();
  return segs;
}

bool
Castle::recently_died() const
{
  long diff = time_now.micros() - time_of_death.micros();
  if (diff > 2000000)
    return false;
  return true;
}
