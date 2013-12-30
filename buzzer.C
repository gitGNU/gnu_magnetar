//      MAGNETAR
//      Copyright (c) 1993-2005 by Mark B. Hanson (mbh@panix.com).
//      Copyright (C) 2013 Ben Asselstine
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
#include "font.h"

#include "buzzer.h"

using namespace Trig;

double Buzzer::buzzer_scale = 33.0;
namespace {
const double time_to_wait_before_reattaching_mine_to_ring = 0.05;
const double avg_time_to_leave_a_ring_when_player_dead = 4.0;

const coords buzzer_points[] = {
    {  COS00 / 2.0, -SIN00 / 2.0 }, { -COS00 / 2.0,  SIN00 / 2.0 },
    {  COS30 / 2.0, -SIN30 / 2.0 }, { -COS30 / 2.0,  SIN30 / 2.0 },
    {  COS60 / 2.0, -SIN60 / 2.0 }, { -COS60 / 2.0,  SIN60 / 2.0 },
    {  COS90 / 2.0,  SIN90 / 2.0 }, { -COS90 / 2.0, -SIN90 / 2.0 },
    {  COS60 / 2.0,  SIN60 / 2.0 }, { -COS60 / 2.0, -SIN60 / 2.0 },
    {  COS30 / 2.0,  SIN30 / 2.0 }, { -COS30 / 2.0, -SIN30 / 2.0 },

    {  COS00 / 1.0, -SIN00 / 1.9 }, {  COS30 / 1.9, -SIN30 / 1.9 },
    {  COS30 / 1.0, -SIN30 / 1.9 }, {  COS60 / 1.9, -SIN60 / 1.9 },
    {  COS60 / 1.0, -SIN60 / 1.9 }, {  COS90 / 1.9, -SIN90 / 1.9 },
    {  COS90 / 1.0, -SIN90 / 1.9 }, { -COS60 / 1.9, -SIN60 / 1.9 },
    { -COS60 / 1.0, -SIN60 / 1.9 }, { -COS30 / 1.9, -SIN30 / 1.9 },
    { -COS30 / 1.0, -SIN30 / 1.9 }, { -COS00 / 1.9, -SIN00 / 1.9 },
    { -COS00 / 1.0, -SIN00 / 1.9 }, { -COS30 / 1.9,  SIN30 / 1.9 },
    { -COS30 / 1.0,  SIN30 / 1.9 }, { -COS60 / 1.9,  SIN60 / 1.9 },
    { -COS60 / 1.0,  SIN60 / 1.9 }, { -COS90 / 1.9,  SIN90 / 1.9 },
    { -COS90 / 1.0,  SIN90 / 1.9 }, {  COS60 / 1.9,  SIN60 / 1.9 },
    {  COS60 / 1.0,  SIN60 / 1.9 }, {  COS30 / 1.9,  SIN30 / 1.9 },
    {  COS30 / 1.0,  SIN30 / 1.9 }, {  COS00 / 1.9, -SIN00 / 1.9 },

};

} //namespace


Buzzer::Buzzer(void)
{
    //fprintf(stderr, "Buzzer::Buzzer()\n");
    set_scale(buzzer_scale);
    state = MINE_DEAD;
    paused = false;
    points = buzzer_points;
    num_points = sizeof (buzzer_points) / sizeof (coords);
    xpoints = new XPoint[num_points];
    set_gc(fetch_gc(GC_DULL_ORANGE));
    adjust_speed();
    magnetic = false;

} // Buzzer::Buzzer


Buzzer::~Buzzer(void)
{
    //fprintf(stderr, "Buzzer::~Buzzer()\n");
} // Buzzer::~Buzzer

void
Buzzer::set_randomizer(void)
{
  if (paused) {
    return;
  }

} // Buzzer::setrandomizer

void
Buzzer::render(const bool ink)
{
  if (!alive()) {
    return;
  }

  //set_points(cinematronics_points[0], cinematronics_count[0]);
  if (ink) {
      set_xpoints();
  }
  paint_points(ink);
} // Buzzer::render


void
Buzzer::move(Castle *castle, Ship *ship, King *king, Stats *stats, bool &buzzer_dead)
{
    if (!alive()) {
	return;
    }

        //update_speed();
    set_x(get_x() + get_dx());
    if (get_x() < 0) {
        set_x(get_x() + ww());
    } else if (get_x() >= ww()) {
      set_y(get_y() - ww());
    }
    set_y(get_y() + get_dy());
    
    if (castle->buzzer_collision(this, ship, stats))
      snuff();
    else if (ship->hit(this))
      {
        snuff();
        stats->killed_by_a_buzzer();
      }
    if (state == MINE_DEAD)
      buzzer_dead = true;
    if (king && magnetic)
      {
        float strength = king->get_aligned_strength (this);
        if (strength != 0)
          set_dx(get_dx() + (20.0/args.fps *strength));
      }
} // Buzzer::move

void
Buzzer::upgrade(Castle *castle)
{
  return;
} // Buzzer::upgrade

void
Buzzer::launch(std::list<std::pair<float, float> > zones, bool is_magnetic_mine)
{
  set_y(-15 + -(Random::get() % wh()));

  while (1)
    {
      set_x((Random::get() % ww()));
      bool found = false;
      for (std::list<std::pair<float, float> >::iterator i = zones.begin();
           i != zones.end(); i++)
        {
          float percent = (float)get_x() / (float)ww();
          //printf("is %f between %f and %f\n", percent, (*i).first, (*i).second);
          if ((*i).first < percent && (*i).second > percent)
            found = true;
        }
      if (found == true)
        break;
    }

    theta = -90;
    dtheta = 2.00; //crapola
    update_speed();

    magnetic = is_magnetic_mine;
    if (magnetic)
    set_gc(fetch_gc(GC_DULL_BLUE));
    else
    set_gc(fetch_gc(GC_DULL_ORANGE));

    state = MINE_LIVING;
} // Buzzer::launch



bool
Buzzer::hit(Laser *laser, Stats *stats)
{
    if (alive() && hypot(laser->get_x() - get_x(), laser->get_y() - get_y()) <
	    (diag + laser->get_diag()) / 2.0) {
	snuff();
        if (stages->challenge())
          stats->add_score(10);
        play (ASTEROID_EXPLODE);
        stats->destroyed_a_buzzer();
	return true;
    }
    return false;
} // Buzzer::hit

void
Buzzer::set_xpoints(void)
{
  const double sine = xsin(theta);
  const double cosine = xcos(theta);

  for (int i = 0; i < num_points; i++) {
    double nsize =  size;
    const double tx = nsize * points[i].x;
    const double ty = nsize * points[i].y;

    xpoints[i].x = (short)((tx * cosine) + (ty * sine) + get_x());
    xpoints[i].y = (short)(-(tx * sine) + (ty * cosine) + get_y());

  }
} // Buzzer::set_xpoints

void
Buzzer::resize(const int nwidth, const int nheight)
{
    speed *= (double)nwidth / ww();
    Xything::resize(nwidth, nheight);
} // Buzzer::resize


void
Buzzer::place_randomly(Castle *castle)
{
  return;
} // Buzzer::place_randomly

void
Buzzer::adjust_speed()
{
  speed = (max(ww(),wh())/ (stages->get_mine_speed() + (Random::get() % 3)) / args.fps);
} // Buzzer::adjust_speed

bool
Buzzer::riding_ring(Castle *c) const
{
  return false;
} // Buzzer::riding_ring

bool
Buzzer::inside() const
{
  if (get_y() > wh()+get_size())
    return false;
  if (get_y() < 0)
    return false;
  return true;
}
bool
Buzzer::outside() const
{
  if (get_y() > wh()+get_size())
    return true;
  if (get_y() < -wh() + -15)
    return true;
  return false;
} // Buzzer::outside

void
Buzzer::update_speed()
{
  set_dx(xcos(-90) * speed);
  set_dy(-xsin(-90) * speed);

} // Buzzer::update_speed


void
Buzzer::turn(Castle *castle, Ship *ship)
{
  if (!alive()) {
    return;
  }

  Ething::turn();
} // Buzzer::turn

