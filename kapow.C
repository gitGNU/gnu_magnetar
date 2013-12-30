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
#include "util.h"
#include "xsc.h"

#include "kapow.h"

namespace {

  const coords kapow_points[] = {
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
}

Kapow::Kapow(GC g, const double duration, const int num_frames)
{
    //fprintf(stderr, "Kapow::Kapow()\n");
    lifespan = duration;
    int dim = (int)hypot(ww(), wh());

    wander = dim / 2;
    dim += wander;

    set_x(ww2());
    set_y(wh2());

    num_points = sizeof (kapow_points) / sizeof (coords);
    pts = new coords[num_points];
    for (int i = 0; i < num_points; i++) {
	double r = (Random::get() % 100) / 1000.0 + 0.10;
        pts[i].x = r * kapow_points[i].x * 6300;
        pts[i].y = r * kapow_points[i].y * 6300;
    }
    
    points = pts;

    xpoints = new XPoint[num_points];

    starsize = max(ww(),wh()) / 400.0;
    starsize = round (starsize);
    if (starsize < 2.0)
      starsize = 2.0;
    set_size(1.0);

    set_gc(gc);

    set_dx(30 / args.fps);
    set_dy(-15 / args.fps);
    set_dtheta(50 / args.fps);

    age = 0.0;
    step = 1.0;
    set_scale(90.0);

    paused = false;
    number_of_frames = num_frames;
    frame = 0;
} // Kapow::Kapow
    
Kapow::Kapow(const Kapow &o)
  :starsize (o.starsize), wander(o.wander), age(o.age), step(o.step), 
    paused(o.paused), lifespan(o.lifespan), frame(o.frame),
    number_of_frames(o.number_of_frames)
{

  set_x(o.get_x());
  set_y(o.get_y());

  //gc_token g = lookup_gc(o.get_gc());
    //set_gc(fetch_gc(g));
    set_gc(o.get_gc());
    num_points = sizeof (kapow_points) / sizeof (coords);
    pts = new coords[num_points];
    for (int i = 0; i < num_points; i++) {
	double r = (Random::get() % 100) / 1000.0 + 0.10;
        pts[i].x = r * kapow_points[i].x * 6300;
        pts[i].y = r * kapow_points[i].y * 6300;
    }
    
    points = pts;

    xpoints = new XPoint[num_points];

    set_size(o.get_size());


    set_dx(o.get_dx());
    set_dy(o.get_dy());
    set_dtheta(o.get_dtheta());
    set_scale(o.get_scale());
}


Kapow::~Kapow(void)
{
    //fprintf(stderr, "Kapow::~Kapow()\n");
    delete[] pts;
} // Kapow::~Kapow


void
Kapow::render(const bool ink)
{
    GC thisgc;

    if (ink) {
	thisgc = gc;
        Tthing::set_xpoints();
    } else {
	thisgc = fetch_gc(GC_BLACK);
    }
    for (int i = 0; i < num_points; i++) {
	double sx = xpoints[i].x - (starsize / 2.0);
	double sy = xpoints[i].y - (starsize / 2.0);

        _XFillArc(display, window, thisgc, (int)sx, (int)sy,
                 (int)starsize, (int)starsize,
                 0, 360*64, true);
    }
} // Kapow::render

void
Kapow::resize(const int nwidth, const int nheight)
{
    starsize = max(ww(),wh()) / 400.0;
    starsize = round (starsize);
    if (starsize < 2.0)
      starsize = 2.0;
    Xything::resize(nwidth, nheight);
} // Kapow::resize

void
Kapow::turn ()
{
  if (age > lifespan)
    return;
  age += (1.0/args.fps);

  //have we crossed a frame threshold?
  if (age < lifespan / number_of_frames * (frame+1))
    return;
  frame++;
  
  //make the explosion happen faster at the start, and slower at the end.
  double phase = age / lifespan * 1.0 * 1.0;
  switch (int(phase))
    {
    case 0:
      set_scale(get_scale() / (step * 1.1));
      break;
    case 1:
      set_scale(get_scale() / step);
      break;
    case 2:
      set_scale(get_scale() / (step / 1.1));
      break;
    }

} // Kapow::turn


bool
Kapow::alive() const
{
  if (age > lifespan)
    return false;
  return true;
} // Kapow::alive

void
Kapow::set_step (const double s)
{
  step = s;
} // Kapow::set_step
