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

#ifndef	MAGNETAR_TRIG_H
#define	MAGNETAR_TRIG_H

namespace Trig {
    const double PI = 3.14159265358979323846;

    const double SQRT2 = 1.41421356237309504880;
    const double SQRT3 = 1.73205080756887729352;

    const double SIN00 = 0.0;
    const double SIN15 = 0.25881904510252076233;
    const double SIN30 = 0.5;
    const double SIN45 = (0.5 / SQRT2);
    const double SIN60 = (SQRT3 / 2.0);
    const double SIN75 = 0.99976741423148606665;
    const double SIN90 = 1.0;

    const double COS00 = SIN90;
    const double COS15 = SIN75;
    const double COS30 = SIN60;
    const double COS45 = SIN45;
    const double COS60 = SIN30;
    const double COS75 = SIN15;
    const double COS90 = SIN00;

    void init(void);
    double xsin(const double);
    double xcos(const double);
    double xatan2(const double, const double);
    double wedge(const double, const double);

    double rad2deg(const double);
    double deg2rad(const double);
    double normalize(const double);
} // namespace Trig


inline double
Trig::rad2deg(const double rad)
{
    return rad * (180.0 / PI);
} // Trig::rad2deg


inline double
Trig::deg2rad(const double deg)
{
    return deg * (PI / 180.0);
} // Trig::deg2rad


inline double
Trig::normalize(const double degree)
{
    double normed = degree;

    while (normed < 0.0) {
	normed += 360.0;
    }

    while (normed >= 360.0) {
	normed -= 360.0;
    }

    return normed;
} // Trig::normalize

#endif	// MAGNETAR_TRIG_H
