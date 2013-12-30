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
#include "trig.h"
#include "util.h"
#include "xsc.h"
#include "sound.h"

#include "fireball.h"

using namespace Trig;


namespace {

const coords fireball_points[] = {
    { -0.20, -1.00 }, { -0.20,  1.00 },
    { -0.20,  1.00 }, {  0.20,  1.00 },
    {  0.20,  1.00 }, {  0.20, -1.00 },
    {  0.20, -1.00 }, { -0.20, -1.00 },

    { -0.15, -0.66 }, { -0.15,  0.66 },
    { -0.10, -0.66 }, { -0.10,  0.66 },
    { -0.05, -0.66 }, { -0.05,  0.66 },
    {  0.05, -0.66 }, {  0.05,  0.66 },
    {  0.10, -0.66 }, {  0.10,  0.66 },
    {  0.15, -0.66 }, {  0.15,  0.66 },
};

} // namespace

const double fireball_width = 24.0;

Fireball::Fireball(MagnetInfo i)
  : strength(i.get_strength()), length(i.get_length()), speed(i.get_speed()),
    starting_height(i.get_starting_height()),
    starting_width(i.get_starting_width()),
    dir(i.get_dir()), bouncy(i.get_bouncy()), switchy(i.get_switchy())
{
    set_scale(fireball_width);
    flying = false;
    paused = false;
    points = fireball_points;
    num_points = sizeof(fireball_points) / sizeof(coords);
    xpoints = new XPoint[num_points];
    gc = fetch_gc(GC_DULL_RED);
    aligned = false;
    last_switch_pos = wh() * starting_height;
}

double Fireball::get_width()
{
  return ww() / fireball_width;
}

Fireball::Fireball(float s, float l, float sp, int d, float sh, float sw, bool b, double swit)
{
    //fprintf(stderr, "Fireball::Fireball()\n");
    strength = s;
    length = l;
    speed = sp;
    dir = d;
    starting_height = sh;
    starting_width = sw;
    bouncy = b;
    switchy = swit;
    set_scale(fireball_width);
    flying = false;
    paused = false;
    points = fireball_points;
    num_points = sizeof(fireball_points) / sizeof(coords);
    xpoints = new XPoint[num_points];
    gc = fetch_gc(GC_DULL_RED);
    aligned = false;
    last_switch_pos = wh() * sh;
} // Fireball::Fireball


Fireball::~Fireball(void)
{
  ;
} // Fireball::~Fireball


void
Fireball::set_randomizer(void)
{
    if (paused) {
	return;
    }

} // Fireball::set_randomizer


void
Fireball::render(const bool ink)
{
    if (!alive()) {
	return;
    }

    if (ink) {
      if (aligned)
        gc = fetch_gc(GC_BRIGHT_RED);
      else
        gc = fetch_gc(GC_DULL_RED);
	set_xpoints();
    }
    paint_points(ink);
} // Fireball::render


bool 
Fireball::should_switch()
{
  //okay the question is if this magnet should switch directions.
  //we have a variable switchy that tells us how often we should switch.
  //0 means not at all, and 1 means it stays still because it is so switchy.

  if (switchy >= 1)
    return true;

  double max_dist = switchy * wh();
  double dist = fabs(get_y() - last_switch_pos);
  if (dist > max_dist)
    {
      if (Random::get() % int(args.fps)  <= int(args.fps)/20)
        {
          last_switch_pos = get_y();
          return true;
        }
    }
  return false;
}

void
Fireball::move(Ship *ship, bool wrap, Stats *stats)
{
    if (!alive()) {
	return;
    }

    set_x(get_x() + get_dx());
    set_y(get_y() + get_dy());
    if (bouncy)
      {
        if ((get_y() >= wh() + (get_size()*length)) || (get_y() < 0 - (get_size()*length)))
          Xything::bounce(normalize(90+ 90.0));
      }
    else
      {
        /*
        if ((x >= ww() - get_size()) || (x < 0 + get_size()) || (y >= wh() - get_size()) || (y < 0 + get_size()))
          {
            if (y > wh() )
              {
                y = 0;
                const double deg = 90;
                const double mag = (ww() / speed) / args.fps; 
                dx = xcos(deg) * mag;
                dy = xsin(deg) * mag;
              }
            else if (y < 0)
              {
                y = wh();
                const double deg = 90;
                const double mag = (ww() / speed) / args.fps; 
                dx = xcos(deg) * mag;
                dy = -xsin(deg) * mag;
              }
          }
          */
        if ((get_y() >= wh() + (get_size()*length)) || (get_y() < 0 - (get_size()*length)))
          {
            if (wrap)
              {
                if (get_y() > wh())
                  {
                    set_y((-get_size()*length) + 0.01);
                    const double deg = 90;
                    const double mag = (ww() / speed) / args.fps; 
                    set_dx(xcos(deg) * mag);
                    set_dy(xsin(deg) * mag);
                  }
                else if (get_y() < 0)
                  {
                    set_y(wh()+(get_size()*length)-0.01);
                    const double deg = 90;
                    const double mag = (ww() / speed) / args.fps; 
                    set_dx(xcos(deg) * mag);
                    set_dy(-xsin(deg) * mag);
                  }
              }
          }
      }
    if (switchy != 0.0)
      {
        if (should_switch())
          Xything::bounce(normalize(90+ 90.0));
      }

    aligned = is_aligned (ship);
    /*
    if (ship->hit(this)) {
      stats->killed_by_a_fireball();
	snuff();
    }
    */
} // Fireball::move


void
Fireball::launch(King *king, Stats *stats)
{
    if (alive()) {
	return;
    }

    set_x(starting_width);
    set_y(wh()*starting_height);

    const double deg = 90;//king->get_theta();
    const double mag = (ww() / speed) / args.fps;
    set_dx(xcos(deg) * mag);
    set_dy(xsin(deg) * mag);
    set_dy(get_dy() * dir);

    flying = true;
} // Fireball::launch


void
Fireball::set_xpoints(void)
{
    for (int i = 0; i < 8; i++) {
      if (points[i].y  < 0)
        xpoints[i].y = (short)((size * (-length)) + get_y());
      else
        xpoints[i].y = (short)((size * (length)) + get_y());
    }
    for  (int i = 8; i < num_points; i++) {
      if (points[i].y  < 0)
        xpoints[i].y = (short)((size * (-length*fabs(strength))) + get_y());
      else
        xpoints[i].y = (short)((size * (length*fabs(strength))) + get_y());
    }
    for (int i = 0; i < num_points; i++) {
	xpoints[i].x = (short)((size * points[i].x) + get_x());
    }
} // Fireball::set_xpoints

bool
Fireball::hit(Laser *laser, Stats *stats)
{
  if (!alive())
    return false;
  double delta_x = fabs(laser->get_x() - get_x());
  double delta_y = fabs(laser->get_y() - get_y());
  if (delta_x < diag && delta_y < diag)
    {
      stats->add_score(0);
      return true;
    }
  return false;
} // Fireball::hit
    
bool 
Fireball::is_aligned (Ething *ship)
{
  float ydiff = get_y() - ship->get_y();
  if (fabs(ydiff) <= get_size() * get_length())
    return true;
  else
    return false;
}
