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

#include <algorithm>
#include "random.h"
#include "util.h"
#include "xsc.h"
#include "sound.h"
#include "stages.h"

#include "minefield.h"

Minefield::Minefield(unsigned int m, unsigned int mt)
{
  mines_left = m;
  mines = m;
  mines_at_a_time = mt;
    //fprintf(stderr, "Minefield::Minefield()\n");
    if (m > 0)
      buzzers = new Buzzer[mines_at_a_time];
    else
      buzzers = NULL;
    expl = new std::list<Kapow>();
    expl->clear();
    mines_expired = 0;
    magnetic_mines = new std::list<unsigned int>();
} // Minefield::Minefield


Minefield::~Minefield(void)
{
    //fprintf(stderr, "Minefield::~Minefield()\n");
    delete[] buzzers;
    delete expl;
    expl = NULL;
    delete magnetic_mines;
} // Minefield::~Minefield


void
Minefield::render(const bool ink) const
{
    for (unsigned int i = 0; i < mines_at_a_time; i++) {
	buzzers[i].render(ink);
    }
    for (std::list<Kapow>::iterator i = expl->begin(); i != expl->end(); i++) {
      (*i).render(ink);
    }
} // Minefield::render


void
Minefield::move(Castle *castle, Ship *ship, King *king, Stats *stats)
{
  bool buzzer_dead = false;
    for (unsigned int i = 0; i < mines_at_a_time; i++) {
      buzzer_dead = false;
	buzzers[i].move(castle, ship, king, stats, buzzer_dead);
        if (buzzer_dead)
          {
            gc_token g = lookup_gc(buzzers[i].get_gc());
            //printf("buzzer color is %d\n", g);
                Kapow *k = new Kapow(buzzers[i].get_gc(), 0.2, 7);
                k->set_scale(150000);
                k->set_step(1.3);
                k->set_x(buzzers[i].get_x());
                k->set_y(buzzers[i].get_y());
                expl->push_back(*k);
                expl->back().set_gc(fetch_gc(g));
                delete k;
                mines_expired++;
                stats->add_expired_mines(1, false);
          }
    }
    for (std::list<Kapow>::iterator i = expl->begin(); i != expl->end(); i++) {
      (*i).turn();
    }
    std::list<Kapow>::iterator k = expl->begin();
    while (k != expl->end()) {
      if (!(*k).alive())
        expl->erase(k++);
      else
        ++k;
    }

} // Minefield::move


void
Minefield::resize(const int nwidth, const int nheight) const
{
    for (unsigned int i = 0; i < mines_at_a_time; i++) {
	buzzers[i].resize(nwidth, nheight);
    }
} // Minefield::resize


bool
Minefield::hit(Laser *laser, Stats *stats)
{
    for (unsigned int i = 0; i < mines_at_a_time; i++) {
	if (buzzers[i].hit(laser, stats)) {
                gc_token g = lookup_gc(buzzers[i].get_gc());
                Kapow *k = new Kapow(buzzers[i].get_gc(), 0.2, 7);
                k->set_scale(150000);
                k->set_step(1.3);
                k->set_x(laser->get_x());
                k->set_y(laser->get_y());
                expl->push_back(*k);
                expl->back().set_gc(fetch_gc(g));
                delete k;
                stats->add_expired_mines(1, false);
                stats->shot_mine();
                mines_expired++;
	    return true;
	}
    }
    return false;
} // Minefield::hit


void
Minefield::launch(King *king, Castle *castle, Stats *stats)
{
    if (!king->alive()) {
	return;
    }

    if (mines_left == 0)
      return;

    for (unsigned int i = 0; i < mines_at_a_time; i++) {
	if (buzzers[i].alive() == false &&
		(Random::get() % (int)(stages->get_avg_launch_time() * args.fps) == 0)) {
            bool magnetic = false;
            if (std::find(magnetic_mines->begin(), magnetic_mines->end(), mines_left) != magnetic_mines->end())
              magnetic = true;
	    buzzers[i].launch(drop_zones, magnetic);
            stats->launched_a_buzzer();
            mines_left--;
	    return;
	}
    }
} // Minefield::launch

int
Minefield::expire()
{
  int count = 0;
    for (unsigned int i = 0; i < mines_at_a_time; i++) 
      {
	if (buzzers[i].alive() && buzzers[i].outside())
          {
            count++;
            mines_expired++;
	    buzzers[i].snuff();
	}
    }
    return count;
} // Minefield::expire

void
Minefield::upgrade(Castle *castle) const
{
    for (unsigned int i = 0; i < mines_at_a_time; i++) {
	buzzers[i].upgrade(castle);
    }
} // Minefield::upgrade

void
Minefield::spawn_buzzers(Castle *castle, int num) 
{
} // Minefield::spawn_buzzers

int
Minefield::snuff_visible()
{
  int snuffed = 0;
  for (unsigned int i = 0; i < mines_at_a_time; i++) {
    if (buzzers[i].inside() && buzzers[i].alive())
      {
        gc_token g = lookup_gc(buzzers[i].get_gc());
        Kapow *k = new Kapow(buzzers[i].get_gc(), 0.2, 7);
        k->set_scale(150000);
        k->set_step(1.3);
        k->set_x(buzzers[i].get_x());
        k->set_y(buzzers[i].get_y());
        expl->push_back(*k);
        expl->back().set_gc(fetch_gc(g));
        delete k;
        snuffed++;
        buzzers[i].snuff();
        mines_expired++;
      }
  }
  return snuffed;
}
void
Minefield::snuff(void)
{
    for (unsigned int i = 0; i < mines_at_a_time; i++) {

	buzzers[i].snuff();
    }
    mines_left = mines;
} // Minefield::snuff


void
Minefield::pause(bool pause_state) const
{
    for (unsigned int i = 0; i < mines_at_a_time; i++) {
	buzzers[i].pause(pause_state);
    }
} // Minefield::pause

void
Minefield::adjust_speed()
{
    for (unsigned int i = 0; i < mines_at_a_time; i++)
      buzzers[i].adjust_speed();
} // Minefield::adjust_speed


void
Minefield::turn(Castle *castle, Ship *ship)
{
    for (unsigned int i = 0; i < mines_at_a_time; i++)
      buzzers[i].turn(castle, ship);
}
    
void 
Minefield::snuff_visible_and_future_mines(Stats *stats)
{
  int snuffed = snuff_visible();
  snuffed += mines_left;
  mines_expired += mines_left;
  stats->add_expired_mines(snuffed, false);
  mines_left = 0;
 //i do not know why this is needed.  it should not be unnecessary.
  int remaining = stages->get_max_mines() - stats->get_expired_mines();
  if (remaining > 0)
    stats->add_expired_mines(remaining, false);
  snuff();
}

void Minefield::reset(unsigned int m, unsigned int mt, double magnetic_percent)
{
  mines_left = m;
  mines = m;
  mines_at_a_time = mt;
  if (buzzers)
    delete[] buzzers;
  buzzers = new Buzzer[mines_at_a_time];
  if (expl)
    delete expl;
  expl = new std::list<Kapow>();
  expl->clear();
  mines_expired = 0;
  //okay which mines are magnetic?
  magnetic_mines->clear();
  int num = magnetic_percent * mines;
  for (int i = 0; i < num; i++)
    {
      while (1)
        {
          unsigned int id = Random::get() % mines;
          if (std::find (magnetic_mines->begin(), magnetic_mines->end(), id) ==
              magnetic_mines->end())
            {
              magnetic_mines->push_back(id);
              break;
            }
        }
    }
}
