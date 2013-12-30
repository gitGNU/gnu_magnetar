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

#include "tthing.h"

using namespace Trig;


Tthing::Tthing(void)
{
    //fprintf(stderr, "Tthing::Tthing()\n");
    theta = dtheta = 0.0;
} // Tthing::Tthing


Tthing::~Tthing(void)
{
    //fprintf(stderr, "Tthing::~Tthing()\n");
} // Tthing::~Tthing


void
Tthing::turn(void)
{
    theta = normalize(theta + dtheta);
} // Tthing::turn


void
Tthing::set_xpoints(void)
{
    const double sine = xsin(theta);
    const double cosine = xcos(theta);

    for (int i = 0; i < num_points; i++) {
	const double tx = size * points[i].x;
	const double ty = size * points[i].y;

	xpoints[i].x = (short)((tx * cosine) + (ty * sine) + get_x());
	xpoints[i].y = (short)(-(tx * sine) + (ty * cosine) + get_y());
    }
} // Tthing::set_xpoints
