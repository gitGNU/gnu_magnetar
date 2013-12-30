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

#include "random.h"
#include "trig.h"
#include "util.h"
#include "xsc.h"

#include "spikes.h"

using namespace Trig;

namespace {
const double lifespan = 0.20; //seconds
const int num_animation_frames = 6;
/*
 * NUM_ANIMATION_FRAMES are shown over LIFESPAN seconds.
 */

const coords spikes_points[] = {
    {  COS00 / 2.0, -SIN00 / 2.0 }, { -COS00 / 2.0,  SIN00 / 2.0 },
    {  COS15 / 2.0, -SIN15 / 2.0 }, { -COS15 / 2.0,  SIN15 / 2.0 },
    {  COS30 / 2.0, -SIN30 / 2.0 }, { -COS30 / 2.0,  SIN30 / 2.0 },
    {  COS45 / 2.0, -SIN45 / 2.0 }, { -COS45 / 2.0,  SIN45 / 2.0 },
    {  COS60 / 2.0, -SIN60 / 2.0 }, { -COS60 / 2.0,  SIN60 / 2.0 },
    {  COS75 / 2.0, -SIN75 / 2.0 }, { -COS75 / 2.0,  SIN75 / 2.0 },
    {  COS90 / 2.0,  SIN90 / 2.0 }, { -COS90 / 2.0, -SIN90 / 2.0 },
    {  COS75 / 2.0,  SIN75 / 2.0 }, { -COS75 / 2.0, -SIN75 / 2.0 },
    {  COS60 / 2.0,  SIN60 / 2.0 }, { -COS60 / 2.0, -SIN60 / 2.0 },
    {  COS45 / 2.0,  SIN45 / 2.0 }, { -COS45 / 2.0, -SIN45 / 2.0 },
    {  COS30 / 2.0,  SIN30 / 2.0 }, { -COS30 / 2.0, -SIN30 / 2.0 },
    {  COS15 / 2.0,  SIN15 / 2.0 }, { -COS15 / 2.0, -SIN15 / 2.0 },
};

} // namespace


Spikes::Spikes(GC g)
{
    age = 0.0;
    step = 10.0;
    //fprintf(stderr, "Spikes::Spikes()\n");
    set_scale(90.0);
    paused = false;
    points = spikes_points;
    num_points = sizeof(spikes_points) / sizeof(coords);
    xpoints = new XPoint[num_points];
    set_gc(g);
    randomizer = new double[num_points];
    int old_seed = Random::get();
    Random::seed(0);
    for (int i = 0; i < num_points; i++) {
	randomizer[i] = (Random::get() % 900) / 1000.0 + 0.10;
    }
    Random::seed(old_seed);
} // Spikes::Spikes


Spikes::~Spikes(void)
{
    delete[] randomizer;
    //fprintf(stderr, "Spikes::~Spikes()\n");
} // Spikes::~Spikes


void
Spikes::render(const bool ink)
{
    if (ink) {
	set_xpoints();
    }
    paint_points(ink);
} // Spikes::render

void
Spikes::set_xpoints(void)
{
    for (int i = 0; i < num_points; i++) {
	double nsize = randomizer[i] * size;
	xpoints[i].x = (short)((nsize * points[i].x) + get_x());
	xpoints[i].y = (short)((nsize * points[i].y) + get_y());
    }
} // Spikes::set_xpoints


void
Spikes::resize(const int nwidth, const int nheight)
{
    Xything::resize(nwidth, nheight);
} // Spikes::resize

void
Spikes::set_step (const double s)
{
  step = s;
} // Spikes::set_step

void 
Spikes::turn ()
{
  if (age > lifespan)
    return;
  age += (1.0/args.fps);
  double frame = age / lifespan * 1.0 * (double)num_animation_frames;
  size =  10 + (int(frame) * step);
} // Spikes::turn

bool
Spikes::alive()
{
  if (age > lifespan) 
    return true;
  return false;
} // Spikes::alive
