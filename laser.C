//      MAGNETAR
//      Copyright (c) 1993-2005 by Mark B. Hanson (mbh@panix.com).
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

#include "sound.h"
#include "laser.h"

using namespace Trig;


Laser::Laser(void)
{
    //fprintf(stderr, "Laser::Laser()\n");
    set_scale(175.0);
    lifespan = ww() * 0.70;
    age = lifespan;
    mag = (ww() / 2.0) / args.fps;
    gc = fetch_gc(GC_BRIGHT_YELLOW);
} // Laser::Laser


Laser::~Laser(void)
{
    //fprintf(stderr, "Laser::~Laser()\n");
} // Laser::~Laser


void
Laser::resize(const int nwidth, const int nheight)
{
    const double nlifespan = nwidth * 0.75;
    const double deg = ship->get_theta();

    if (alive()) {
	age *= nlifespan / lifespan;
    } else {
	age = nlifespan;
    }
    lifespan = nlifespan;

    mag = (nwidth / 2.0) / args.fps;
    set_dx(xcos(deg) * 1.2) /*+ ship->get_dx() / mag*/;
    set_dy(-xsin(deg) * 1.2)/* + ship->get_dy() / mag*/;
    Thing::resize(nwidth, nheight);
    //here we say the laser goes 1.1 times faster than the ship.

    if (size < 2.0) {
	size = 2.0;
    }
} // Laser::resize


void
Laser::render(const bool ink)
{
    if (!alive()) {
	return;
    }

    _XFillArc(display, window, (ink ? gc : fetch_gc(GC_BLACK)),
	    (int)(get_x() - (size / 2.0)),
	    (int)(get_y() - (size / 2.0)),
	    (int)size, (int)size,
	    0, 360*64, true);
} // Laser::render


void
Laser::move(Castle *castle, King *king, Minefield *minefield, Stats *stats)
{
    for (int i = 0; i < mag && alive(); i++) 
      {
        Xything::move();
        if (castle && minefield && stats)
          {
            if (castle->collision(this, stats)) 
              snuff();
            else if (minefield->hit(this, stats)) 
              snuff();
            else 
              age += hypot(get_dx(), get_dy());
          }
        else
          {
            age += hypot(get_dx(), get_dy());
          }
      }
} // Laser::move


void
Laser::launch(void)
{
    const double deg = ship->get_theta();

    set_x(ship->get_x());
    set_y(ship->get_y());

    set_dx(xcos(deg) * 1.1) /*+ ship->get_dx() / mag*/;
    set_dy(-xsin(deg) * 1.1)/* + ship->get_dy() / mag*/;

    age = 0;
    //play(BULLET_SOUND);
} // Laser::launch
