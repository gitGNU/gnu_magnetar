//      MAGNETAR
//      Copyright (C) 2013 Ben Asselstine
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
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

#include "xsc.h"
#include "magnet-info.h"
MagnetInfo::MagnetInfo(float s, float l, float sp, int d, bool b, double swit, float sh, float sw)
{
    strength = s;
    length = l;
    speed = sp;
    bouncy = b;
    switchy = swit;
    starting_height = sh;
    starting_width = sw;
    dir = d;
} // MagnetInfo::MagnetInfo

MagnetInfo::MagnetInfo(float s, float l, float sp, int d, bool b, double swit, float sh)
{
    strength = s;
    length = l;
    speed = sp;
    bouncy = b;
    switchy = swit;
    starting_height = sh;
    starting_width = 0;
    dir = d;
} // MagnetInfo::MagnetInfo

MagnetInfo::~MagnetInfo(void)
{
} // MagnetInfo::~MagnetInfo

