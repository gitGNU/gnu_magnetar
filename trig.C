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

#include "trig.h"


namespace {

const int N = 4;
double sintab[90 * N];

double
sinlook(const double angle)
{
    const int a = (int)(Trig::normalize(angle) + 0.5);
    double retval;

    if (a < 90) {
	retval = sintab[a * N];
    } else if (a == 90) {
	retval = 1.0;
    } else if (a < 180) {
	retval = sintab[(180 - a) * N];
    } else if (a < 270) {
	retval = -sintab[(a - 180) * N];
    } else if (a == 270) {
	retval =-1.0;
    } else {
	retval = -sintab[(360 - a) * N];
    }

    return retval;
} // ::sinlook

} // namespace


void
Trig::init(void)
{
    for (int angle = 0; angle < 90 * N; angle++) {
	sintab[angle] = sin(deg2rad(angle / (double)N));
    }
} // Trig::init


double
Trig::xsin(const double degrees)
{
    return sinlook(degrees);
} // Trig::xsin


double
Trig::xcos(const double degrees)
{
    return sinlook(degrees + 90.0);
} // Trig::xcos


double
Trig::xatan2(const double y, const double x)
{
    return rad2deg(atan2(y, x)) + 180.0;
} // Trig::xatan2


double
Trig::wedge(const double here, const double there)
{
    const double h = Trig::normalize(here);
    const double t = Trig::normalize(there);
    double diff = fabs(t - h);
    int direction = sign(t - h);

    if (diff >= 180.0) {
	diff = 360.0 - diff;
	direction = -direction;
    }

    return (diff * direction);
} // Trig::wedge
