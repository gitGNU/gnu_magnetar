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

#include "util.h"
#include "xsc.h"

#include "thing.h"


Thing::Thing(void)
{
    //fprintf(stderr, "Thing::Thing()\n");
    x = y = 0.0;
    gc = fetch_gc(GC_DULL_BLUE);
    xpoints = NULL;
    points = NULL;
    num_points = 0;
    window = game_window;
} // Thing::Thing


Thing::~Thing(void)
{
    //fprintf(stderr, "Thing::~Thing()\n");
    delete[] xpoints;
} // Thing::~Thing


void
Thing::set_size(const double new_size)
{
    size = new_size;
    scale = min(ww(),wh()) / size;
    diag = hypot(size / 2.0, size / 2.0);
} // Thing::set_size


void
Thing::set_scale(const double new_scale)
{
    scale = new_scale;
    size = min(ww(), wh()) / scale;
    diag = hypot(size / 2.0, size / 2.0);
} // Thing::set_scale


void
Thing::set_points(const coords *new_points, const int new_num_points)
{
    if (new_num_points > num_points) {
	delete[] xpoints;
	xpoints = new XPoint[new_num_points];
    }

    points = new_points;
    num_points = new_num_points;
} // Thing::set_points


XPoint *
Thing::get_xpoints(void) const
{
    return xpoints;
} // Thing::get_xpoints


void
Thing::set_xpoints(void)
{
    for (int i = 0; i < num_points; i++) {
	xpoints[i].x = (short)((size * points[i].x) + get_x());
	xpoints[i].y = (short)((size * points[i].y) + get_y());
    }
} // Thing::set_xpoints


void
Thing::paint_points(const bool ink) const
{
    GC thisgc = ink ? gc : fetch_gc(GC_BLACK);

    for (int i = 1; i < num_points; i += 2) {
	_XDrawLine(display, window, thisgc,
		xpoints[i - 1].x, xpoints[i - 1].y,
		xpoints[i].x, xpoints[i].y, false);
    }
} // Thing::paint_points


void
Thing::render(const bool ink)
{
    if (ink) {
	set_xpoints();
    }
    paint_points(ink);
} // Thing::render


void
Thing::resize(const int nwidth, const int nheight)
{
  set_x(get_x() *(double)nwidth / ww());
  set_y(get_y() *(double)nheight / wh());

    size = min(nwidth,nheight) / scale;
    diag = hypot(size / 2.0, size / 2.0);
} // Thing::resize

double
Thing::get_x(void) const
{
    return x;
} // Thing::get_x

void
Thing::set_x(double nx)
{
    x = nx;
} // Thing::set_x

double
Thing::get_y(void) const
{
    return y;
} // Thing::get_y

void
Thing::set_y(double ny)
{
    y = ny;
} // Thing::set_y

