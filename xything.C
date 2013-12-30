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
#include "xsc.h"

#include "xything.h"

using namespace Trig;


Xything::Xything(void)
{
    //fprintf(stderr, "Xything::Xything()\n");
    dx = dy = 0.0;
} // Xything::Xything


Xything::~Xything(void)
{
    //fprintf(stderr, "Xything::~Xything()\n");
} // Xything::~Xything


void
Xything::move(void)
{
    set_x(get_x() + get_dx());
    if (get_x() < 0) {
      set_x(get_x() + ww());
    } else if (get_x() >= ww()) {
      set_x(get_x() - ww());
    }

    set_y(get_y() + get_dy());
    if (get_y() < 0) {
        set_y(get_y() + wh());
    } else if (get_y() >= wh()) {
      set_y(get_y() - wh());
    }
} // Xything::move


void
Xything::resize(const int nwidth, const int nheight)
{
  set_dy(get_dy() * (double)nwidth / ww());
  set_dx(get_dx() * (double)nheight / wh());
  Thing::resize(nwidth, nheight);
} // Xything::resize


double
Xything::get_dx(void) const
{
    return dx;
} // Xything::get_dx


double
Xything::get_dy(void) const
{
    return dy;
} // Xything::get_dy

void
Xything::bounce(const double phi)
{
    const double cos_phi = xcos(phi);
    const double sin_phi = xsin(phi);

    // see: http://www.psky.com/prog/2dcollision.htm

    // translate the coordinate system so the object can be
    // bounced by merely negating the y portion of its vector
    const double ndx = (dx * cos_phi) + (dy * -sin_phi);
    const double ndy = (dx * sin_phi) + (dy * cos_phi);

    // translate the coordinates back to the original orientation
    // by negating the sin() terms in the above translation
    // (the use of -ndy instead of ndy is the bounce negation mentioned above)
    dx = (ndx * cos_phi) + (-ndy * sin_phi);
    dy = (ndx * -sin_phi) + (-ndy * cos_phi);
} // Xything::bounce
