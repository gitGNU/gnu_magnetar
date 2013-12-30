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

#include "trig.h"
#include "util.h"
#include "xsc.h"

#include "king.h"

using namespace Trig;


namespace {

const int num_animation_frames = 26;
const double total_elapsed_time_to_show_full_animation = 0.4;
const coords king_points[num_animation_frames][4] = {
    {
        { -0.50,  0.37 }, { -0.30,  0.00 },
        { -0.30,  0.00 }, { -0.50, -0.37 },
    },
    {
        { -0.49,  0.37 }, { -0.29,  0.00 },
        { -0.29,  0.00 }, { -0.49, -0.37 },
    },
    {
        { -0.48,  0.37 }, { -0.28,  0.00 },
        { -0.28,  0.00 }, { -0.48, -0.37 },
    },
    {
        { -0.47,  0.37 }, { -0.27,  0.00 },
        { -0.27,  0.00 }, { -0.47, -0.37 },
    },
    {
        { -0.46,  0.37 }, { -0.26,  0.00 },
        { -0.26,  0.00 }, { -0.46, -0.37 },
    },
    {
        { -0.45,  0.37 }, { -0.25,  0.00 },
        { -0.25,  0.00 }, { -0.45, -0.37 },
    },
    {
        { -0.44,  0.37 }, { -0.24,  0.00 },
        { -0.24,  0.00 }, { -0.44, -0.37 },
    },
    {
        { -0.43,  0.37 }, { -0.23,  0.00 },
        { -0.23,  0.00 }, { -0.43, -0.37 },
    },
    {
        { -0.42,  0.37 }, { -0.22,  0.00 },
        { -0.22,  0.00 }, { -0.42, -0.37 },
    },
    {
        { -0.41,  0.37 }, { -0.21,  0.00 },
        { -0.21,  0.00 }, { -0.41, -0.37 },
    },
    {
        { -0.40,  0.37 }, { -0.20,  0.00 },
        { -0.20,  0.00 }, { -0.40, -0.37 },
    },
    {
        { -0.39,  0.37 }, { -0.19,  0.00 },
        { -0.19,  0.00 }, { -0.39, -0.37 },
    },
    {
        { -0.38,  0.37 }, { -0.18,  0.00 },
        { -0.18,  0.00 }, { -0.38, -0.37 },
    },
    {
        { -0.37,  0.37 }, { -0.17,  0.00 },
        { -0.17,  0.00 }, { -0.37, -0.37 },
    },
    {
        { -0.36,  0.37 }, { -0.16,  0.00 },
        { -0.16,  0.00 }, { -0.36, -0.37 },
    },
    {
        { -0.35,  0.37 }, { -0.15,  0.00 },
        { -0.15,  0.00 }, { -0.35, -0.37 },
    },
    {
        { -0.34,  0.37 }, { -0.14,  0.00 },
        { -0.14,  0.00 }, { -0.34, -0.37 },
    },
    {
        { -0.33,  0.37 }, { -0.13,  0.00 },
        { -0.13,  0.00 }, { -0.33, -0.37 },
    },
    {
        { -0.32,  0.37 }, { -0.12,  0.00 },
        { -0.12,  0.00 }, { -0.32, -0.37 },
    },
    {
        { -0.31,  0.37 }, { -0.11,  0.00 },
        { -0.11,  0.00 }, { -0.31, -0.37 },
    },
    {
        { -0.30,  0.37 }, { -0.10,  0.00 },
        { -0.10,  0.00 }, { -0.30, -0.37 },
    },
    {
        { -0.29,  0.37 }, { -0.09,  0.00 },
        { -0.09,  0.00 }, { -0.29, -0.37 },
    },
    {
        { -0.28,  0.37 }, { -0.08,  0.00 },
        { -0.08,  0.00 }, { -0.28, -0.37 },
    },
    {
        { -0.27,  0.37 }, { -0.07,  0.00 },
        { -0.07,  0.00 }, { -0.27, -0.37 },
    },
    {
        { -0.26,  0.37 }, { -0.06,  0.00 },
        { -0.06,  0.00 }, { -0.26, -0.37 },
    },
    {
        { -0.25,  0.37 }, { -0.05,  0.00 },
        { -0.05,  0.00 }, { -0.25, -0.37 },
    },
};

} // namespace


King::King(bool l)
{
  draw_force_arrow = true;
  left = l;
  magnets = new std::list<Fireball*>();
  magnets->clear();
    //fprintf(stderr, "King::King()\n");
    set_scale(18.0);
    num_points = sizeof(king_points[0]) / sizeof(coords);
    points = king_points[0];
    xpoints = new XPoint[num_points];
    if (left)
      theta = 0.0;
    else
      theta = 180;
    //dtheta = 32.0 / args.fps;
    if (left)
      set_x(get_size() + (get_size()/1.5));
    else
      set_x(ww() - get_size() - (get_size()/1.5));
    set_y(wh2());
    gc = fetch_gc(GC_DULL_RED);
    living = true;
    animation_age = 0;
} // King::King


King::~King(void)
{
  for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
    delete *i;
    //fprintf(stderr, "King::~King()\n");
    delete magnets;
} // King::~King

    
void 
King::add_fireball(MagnetInfo i)
{
  Fireball *fireball = new Fireball(i);
  fireball->launch(this, NULL);
  magnets->push_back(fireball);
}

void
King::add_attract_fireballs()
{
  reset();
        for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
          (*i)->snuff(); 
    Fireball *fireball = new Fireball(0.66, 1.00, 5, -1, 0.5, 22, false, 0);
    fireball->launch(this, NULL);
    magnets->push_back(fireball);
    fireball = new Fireball(0.33, 2.00, 22,  1, 0.0, fireball->get_size()+5, false, 0);
    fireball->launch(this, NULL);
    magnets->push_back(fireball);
    fireball = new Fireball(0.99, 0.75, 11,  -1, 0.0, ww() -fireball->get_size()+5, false, 0);
    fireball->launch(this, NULL);
    magnets->push_back(fireball);
    fireball = new Fireball(0.11, 3.00, 33,  1, 0.0, ww() -(fireball->get_size()*2), false, 0);
    fireball->launch(this, NULL);
    magnets->push_back(fireball);
}

void
King::reset()
{
  for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
    delete *i;
  delete magnets;
  magnets = new std::list<Fireball*>();
  magnets->clear();
  living = true;
  animation_age = 0;
}

void
King::render(const bool ink)
{
  if (draw_force_arrow)
    {
      if (alive()) {
        if (ink) {
          set_xpoints();
        }
        paint_points(ink);
      }
    }
    for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++)
      (*i)->render(ink);
} // King::render

void
King::attract()
{
  animation_age += (1.0/args.fps);
  animation_age = fmod (animation_age, 
                        total_elapsed_time_to_show_full_animation);
  double frame = animation_age / total_elapsed_time_to_show_full_animation 
    * 1.0 * (double)num_animation_frames;
  points = king_points[int(frame)];
  //Tthing::turn();
  set_xpoints();
} // King::attract

void
King::turn(Castle *castle, Ship *ship, Stats *stats)
{
  set_xpoints();
  animation_age += (1.0/args.fps);
  animation_age = fmod (animation_age, 
                        total_elapsed_time_to_show_full_animation);
  double frame = animation_age / total_elapsed_time_to_show_full_animation 
    * 1.0 * (double)num_animation_frames;
    points = king_points[int(frame)];
    if (alive() && ship->alive()) {
	//dtheta = (wedge(theta, ship->get_angle()) * Difficulty::turning_speed_of_king) / args.fps;

      if (left)
        {
          if (ship->get_polarity())
            theta = 180;
          else
            theta = 0;
        }
      else
        {
          if (ship->get_polarity())
            theta = 0;
          else
            theta = 180;
        }

	Tthing::turn();

	}

    if (alive() == false)
      {
        //speed it up bro.
    for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++)
      {
        (*i)->set_bouncy(false);
        (*i)->set_switchy(0.0);
      if ((*i)->get_dy() > 0)
        (*i)->set_dy((*i)->get_dy()+ (40 / args.fps));
      else
        (*i)->set_dy((*i)->get_dy()- (40 / args.fps));
      }
      }

    for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++)
      (*i)->move(ship, alive(), stats);
    set_y(ship->get_y());
} // King::turn


void
King::resize(const int nwidth, const int nheight)
{
    Tthing::resize(nwidth, nheight);
    for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
      (*i)->resize(nwidth, nheight);
} // King::resize


bool
King::hit(Castle *castle, Laser *laser, Minefield *minefield, Stats *stats)
{
    //if (alive() && laser->get_radius() < diag) {
	//stats->lives++;
        //for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
          //(*i)->snuff(); 
	living = false;
	//return true;
    //}
    return false;
} // King::hit


void
King::pause(const bool pause_state) const
{
  for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
    (*i)->pause(pause_state);
} // King::pause
    
bool 
King::fireball_hit (Laser *laser, Stats *stats)
{
  for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
    if ((*i)->hit(laser, stats))
      return true;
  return false;
} // King::fireball_hit

float 
King::get_aligned_strength(Ething *ship) const
{
  float total = 0;
  for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
    {
      Fireball *fireball = (*i);
      if (fireball->is_aligned(ship))
        {
          float xdiff = fireball->get_x() - ww2();
          if (xdiff < 0)
            total += fireball->get_strength();
          else
            total += -fireball->get_strength();
        }
    }
  return total;
} // King::get_num_aligned
    
void
King::show_force_arrow(bool b)
{
  draw_force_arrow = b;
}
int
King::count_left_magnets()
{
  int count = 0;
  for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
    {
      if ((*i)->get_x() < ww2())
        count++;
    }
  return count;
}
int
King::count_right_magnets()
{
  int count= 0;
  for (std::list<Fireball*>::iterator i = magnets->begin(); i != magnets->end(); i++) 
    {
      if ((*i)->get_x() > ww2())
        count++;
    }
  return count;
}
