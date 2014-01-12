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

#include "random.h"
#include "timing.h"
#include "trig.h"
#include "util.h"
#include "xsc.h"
#include "stages.h"

#include "ship.h"

using namespace Trig;


/*
const coords ship_points[2][NUM_SHIP_POINTS] = {
    {
    {  0.50, -0.10 }, { -0.50, -0.18 },
    { -0.50, -0.18 }, {  0.30, -0.03 },
    {  0.30, -0.03 }, { -0.45, -0.50 },
    { -0.45, -0.50 }, { -0.05,  0.00 },
    { -0.05,  0.00 }, { -0.45,  0.50 },
    { -0.45,  0.50 }, {  0.30,  0.03 },
    {  0.30,  0.03 }, { -0.50,  0.18 },
    { -0.50,  0.18 }, {  0.50,  0.10 },
    { -0.50,  0.18 }, {  0.50,  0.10 }, //dup
    { -0.50,  0.18 }, {  0.50,  0.10 }, //dup
    { -0.50,  0.18 }, {  0.50,  0.10 }, //dup
    },
    {
    {  0.50, -0.10 }, { -0.50, -0.18 },
    { -0.50, -0.18 }, {  0.30, -0.03 },
    {  0.30, -0.03 }, { -0.45, -0.50 },
    { -0.45, -0.50 }, { -0.05,  0.00 },
    { -0.05,  0.00 }, { -0.45,  0.50 },
    { -0.45,  0.50 }, {  0.30,  0.03 },
    {  0.30,  0.03 }, { -0.50,  0.18 },
    { -0.50,  0.18 }, {  0.50,  0.10 },
    { -0.10,  0.00 }, { -0.53,  0.00 },
    { -0.10,  0.02 }, { -0.43,  0.02 },
    { -0.10,  -0.02 }, { -0.43,  -0.02 },
    }
};
*/
const coords ship_points[2][NUM_SHIP_POINTS] = {
    {
    {  0.50, -0.03 }, { -0.50, -0.18 },
    { -0.50, -0.18 }, {  0.30, -0.03 },
    {  0.30, -0.03 }, { -0.45, -0.43 }, //-0.50
    { -0.45, -0.43 }, { -0.05,  0.00 },
    { -0.05,  0.00 }, { -0.45,  0.43 },
    { -0.45,  0.43 }, {  0.30,  0.03 },
    {  0.30,  0.03 }, { -0.50,  0.18 },
    { -0.50,  0.18 }, {  0.50,  0.03 },
    { -0.50,  0.18 }, {  0.50,  0.03 }, //dup
    { -0.50,  0.18 }, {  0.50,  0.03 }, //dup
    { -0.50,  0.18 }, {  0.50,  0.03 }, //dup
    },
    {
    {  0.50, -0.02 }, { -0.50, -0.18 },
    { -0.50, -0.18 }, {  0.30, -0.03 },
    {  0.30, -0.03 }, { -0.45, -0.43 },
    { -0.45, -0.43 }, { -0.05,  0.00 },
    { -0.05,  0.00 }, { -0.45,  0.43 },
    { -0.45,  0.43 }, {  0.30,  0.03 },
    {  0.30,  0.03 }, { -0.50,  0.18 },
    { -0.50,  0.18 }, {  0.50,  0.03 },
    { -0.10,  0.00 }, { -0.53,  0.00 },
    { -0.10,  0.02 }, { -0.43,  0.02 },
    { -0.10,  -0.02 }, { -0.43,  -0.02 },
    }
};

const double max_turn_rate = 540.0;


Ship::Ship(void)
{
    int i;
    polarity = false;

    //fprintf(stderr, "Ship::Ship()\n");
    set_scale(25.0);
    user_rotating_cw = false;
    user_rotating_ccw = false;
    user_thrusting = false;

    theta = 180.0;

    points = ship_points[user_thrusting];
    num_points = NUM_SHIP_POINTS;

    xpoints = new XPoint[num_points];
    Tthing::set_xpoints();

    set_x(initial_x());
    set_y(initial_y());

    max_shots = 15;
    lasers = new Laser[max_shots];
    for (i = 0; i < max_shots; i++) {
	lasers[i].set_ship(this);
    }

    state = SHIP_DEAD;
    impervious = false;

    energy = 1.0;
    explosion = NULL;
    spikes = NULL;
    time_of_death = 0;
    pause_sum = 0;
} // Ship::Ship


Ship::~Ship(void)
{
    //fprintf(stderr, "Ship::~Ship()\n");
    delete[] lasers;
} // Ship::~Ship



double
Ship::initial_x(void)
{
    return ww2();
} // Ship::initial_x


double
Ship::initial_y(void)
{
    return wh2();
} // Ship::initial_y


void
Ship::render(const bool ink)
{
    for (int i = 0; i < max_shots; i++) {
	lasers[i].render(ink);
    }

    if (state == SHIP_EXPLODING) {
      if (explosion)
        explosion->render(ink);
    }

    if (spikes)
      spikes->render(ink);

    if (!alive()) {
	return;
    }

    if (time_of_death && ink)
      {
        if (is_respawning() && time_of_death.micros() != 0)
          {
            int diff = (time_now.micros() - time_of_death.micros()) - pause_sum ;
            if (diff % 250000 < 125000)
              gc = fetch_gc(GC_DULL_RED);
            else
              gc = fetch_gc(GC_BRIGHT_GREY);
          }
        else
          {
            gc = fetch_gc(GC_BRIGHT_GREY);
          }
      }
    else
      gc = fetch_gc(GC_BRIGHT_GREY);

    if (ink) {
	Tthing::set_xpoints();
    }

    paint_points(ink);

} // Ship::render


bool 
Ship::is_respawning()
{
  if (stages->challenge())
    return false;
  if (time_of_death)
    {
      if ((time_now - time_of_death) - pause_sum < 7500000L) 
        return true;
    }
  return false;
}

void
Ship::move(Castle *castle, King *king, King *queen, Minefield *minefield, Stats *stats)
{
    int i;
    int t = user_thrusting;
    if (has_energy_left() == false)
      t = 0;
    points = ship_points[t];
    Tthing::set_xpoints();

    for (i = 0; i < max_shots; i++) {
	lasers[i].move(castle, king, minefield, stats);
    }

    if (state == SHIP_EXPLODING) {
      if ((time_now - time_of_death) - pause_sum > 2500000L) {
        state = SHIP_RESTING;
        stats->lives--;
      }

      if (explosion)
        {
          if (explosion->alive())
            explosion->turn();
          else
            {
              delete explosion;
              explosion = NULL;
            }
        }
    }

    if (state == SHIP_RESTING &&
	    (time_now - time_of_death) - pause_sum > 4000000L) {
	if (stats->lives < 0)
	    state = SHIP_DEAD;
        //else
          //reincarnate(); 
        // not a fan of this call to reincarnate being here,
        // but if we move it to state_change it will break the showing
        // of player scores when the ship dies.
    }

    if (!alive()) {
	return;
    }

    if (user_thrusting && has_energy_left()) {
	const double acceleration = max(ww(),wh()) / 0.75;
	const double delta = acceleration / sq(args.fps);

	set_dx(get_dx() + xcos(theta) * delta);
	set_dy(get_dy() + -xsin(theta) * delta);

	const double speed = hypot(get_dx(), get_dy());
	const double max_speed = (max(ww(),wh()) / 2.25) / args.fps;

        if (stages->challenge() == false)
          lose_energy (1 / args.fps/ 25.0);
	if (speed > max_speed) {
	    double limit_factor;
            if (impervious)
              limit_factor = (max_speed*5) / speed;
            else
              limit_factor = max_speed / speed;

	    set_dx(get_dx() * limit_factor);
	    set_dy(get_dy() * limit_factor);
	}
    } else {
	const double acceleration = ww() / 11.0;
	const double delta = -acceleration / sq(args.fps);
	double speed = hypot(get_dx(), get_dy()) + delta;

	if (speed < 0.0) {
	    speed = 0.0;
	}

	const double drift_angle = xatan2(get_dy(), -get_dx());

	set_dx(xcos(drift_angle) * speed);
	set_dy(-xsin(drift_angle) * speed);
        if (bonus_sound_on == false)
          {
            user_thrusting = false;
            thrust_sound (false);
          }
    }

    if (king && castle)
      {
        if (king->alive()) {
          for (i = 0; i < num_points; i++) {
            if (hypot(castle->get_x() - (xpoints[i].x + get_dx()),
                      castle->get_y() - (xpoints[i].y + get_dy())) <
                castle->ring_size(CASTLE_RING_OUTER) / 2.0) {
              if (castle->alive() && castle->recently_died() == false)
                {
                  spikes = new Spikes(fetch_gc(GC_BRIGHT_GREY));
                  spikes->set_x(castle->get_x());
                  spikes->set_y(castle->get_y());
                  spikes->set_size(170);
                  spikes->set_step(70);

                  int type = castle->get_reward_type();
                  switch (type)
                    {
                    case 0:
                      add_energy((float)castle->get_number_of_remaining_segments()/
                                 (float)castle->get_total_number_of_segments());
                      break;
                    case 1:
                        {
                          int snuffed = minefield->snuff_visible();
                          stats->add_expired_mines(snuffed, false);
                        }
                      break;
                    case 2:
                        {
                          craziness = 2;
                          add_energy(1.0);
                          impervious = true;
                          started_impervious_at = time_now;
                          int snuffed = minefield->snuff_visible();
                          stats->add_expired_mines(snuffed, false);
                          set_dx(get_dx() + xcos(theta) * 20);
                          set_dy(get_dy() + -xsin(theta) * 20);
                        }
                      break;
                    case 3:
                        {
                          minefield->snuff_visible_and_future_mines(stats);
                        }
                      break;
                    case 4:
                        {
                          int snuffed = minefield->snuff_visible();
                          stats->add_expired_mines(snuffed, false);
                          stats->lives++;
                          add_energy(1.0);
                          play (EXTRA_SHIP_AWARDED);
                        }
                      break;
                    }
                  castle->explode(stats);
                  play (REWARD_OBTAINED);
                  stats->add_received_reward();
                  stats->add_score(Castle::points_awarded);
                  stats->destroyed_a_castle();
                }
              break;
            }
          }
        }
      }

    if (king)
      {
        float strength = king->get_aligned_strength (this);
        if (polarity)
          strength *= -1;
        if (strength != 0)
          {
          set_dx(get_dx() + (20.0/args.fps *strength));
            king->set_gc(fetch_gc(GC_BRIGHT_RED));
            queen->set_gc(fetch_gc(GC_BRIGHT_RED));
          }
        else
          {
            king->set_gc(fetch_gc(GC_DULL_RED));
            queen->set_gc(fetch_gc(GC_DULL_RED));
          }
      }
    Xything::move();
} // Ship::move


void
Ship::turn(void)
{

      if (spikes)
        {
          if (spikes->alive())
            spikes->turn();
          else
            {
              delete spikes;
              spikes = NULL;
            }
        }

  if (impervious)
    {
      long diff = time_now.micros() - started_impervious_at.micros();
      if (diff > 5000000)
        {
          impervious = false;
          craziness = 0;
        }
    }

    if (!alive()) {
	return;
    }

    if ((user_rotating_cw || user_rotating_ccw) &&
	    !(user_rotating_cw && user_rotating_ccw)) {
	const double acceleration = 720.0;
	const double delta = acceleration / sq(args.fps);

	if (user_rotating_cw) {
	    dtheta -= delta;
	} else if (user_rotating_ccw) {
	    dtheta += delta;
	}

	const double max_rate = max_turn_rate / args.fps;

	if (fabs(dtheta) > max_rate) {
	    dtheta = sign(dtheta) * max_rate;
	}
    } else {
	const double acceleration = 1440.0;
	const double delta = acceleration / sq(args.fps);
	const int direction = sign(dtheta);

	dtheta += -direction * delta;
	if (sign(dtheta) != direction) {
	    dtheta = 0.0;
	}
    }

    Ething::turn();
} // Ship::turn


void
Ship::fire(Stats *stats)
{
    if (!alive()) {
	return;
    }

    int shoot = stages->challenge();
    if (shoot == 0)
      {
        //reverse polarity and engage inertial dampners
        if (has_energy_left() == true)
          {
            polarity = !polarity;
            set_dx(get_dx() / 2);
            set_dy(get_dy() / 2);
            lose_energy(.10);
            stats->switched_polarity();
            play (SHIP_INVERT_POLARITY);
          }
      }
    else
      {
        for (int i = 0; i < max_shots; i++) 
          {
            if (!lasers[i].alive()) 
              {
                lasers[i].launch();
                stats->shot_a_laser();
                play (SHIP_FIRE_LASER);
                return;
              }
          }
      }
} // Ship::fire


void
Ship::resize(const int nwidth, const int nheight)
{
    int i;

    Xything::resize(nwidth, nheight);
    for (i = 0; i < max_shots; i++) {
	lasers[i].resize(nwidth, nheight);
    }
    if (explosion)
      explosion->resize(nwidth, nheight);
} // Ship::resize

void
Ship::kill()
{
    state = SHIP_EXPLODING;

    explosion = new Kapow(fetch_gc(GC_BRIGHT_YELLOW), 0.45, 14);
    explosion->set_x(get_x());
    explosion->set_y(get_y());
    explosion->set_scale(100000);
    explosion->set_step(1.28);

    set_x(ww2());		// send the buzzers back home
    set_y(wh2());
    time_of_death = time_now;
    pause_sum = 0;
}

bool
Ship::hit(Xything *badguy)
{
    if (!alive() || hypot(badguy->get_x() - get_x(), badguy->get_y() - get_y()) >=
	    (diag + badguy->get_diag()) / 2.0) {
	return false;
    }

    if (is_respawning())
      return false;
    for (int i = 0; i < max_shots; i++) {
	lasers[i].snuff();
    }

    
    lose_energy((50 + (Random::get() % 30 )) / 100.0 );

    if (!impervious)
      {
        if (has_energy_left() == false)
          {
            kill();
            play(SHIP_EXPLODE);
            if (bonus_sound_on == false)
              {
                user_thrusting = false;
                thrust_sound (false);
              }
          }
        else
          {
            play(SHIP_DEFLECT);
            play(ASTEROID_EXPLODE);
          }
      }
    return true;
} // Ship::hit


void
Ship::reincarnate(void)
{
    theta = 180.0;
    set_x(initial_x());
    set_y(initial_y());

    dtheta = 0.0;
    set_dx(0.0);
    set_dy(0.0);
    Tthing::set_xpoints();

    state = SHIP_LIVING;
    reset_energy();
} // Ship::reincarnate

double
Ship::lose_energy(double amount)
{
  if (impervious)
    return amount;

  if (has_energy_left())
    {
      energy -= amount;
      if (energy < 0)
        {
          double remainder = amount + energy;
          energy = 0.0;
          return remainder;
        }
      return amount;
    }
  else
    return 0;
} // Ship::lose_energy
void
Ship::add_energy(double amount)
{
  energy += amount;
  if (energy > 1)
    energy = 1.0;
} // Ship::add_energy
void
Ship::reset_impervious()
{
  impervious = false;
}
    
void
Ship::clear_keystrokes()
{
  if (thrusting())
    thrust(KEY_UP);
  if (rotating_ccw())
    rotate_ccw(KEY_UP);
  if (rotating_cw())
    rotate_cw(KEY_UP);
}
