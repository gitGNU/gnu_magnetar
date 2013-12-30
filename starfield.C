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

#include "random.h"
#include "util.h"
#include "xsc.h"

#include "starfield.h"


Starfield::Starfield(const int number, const bool bright)
{
    //fprintf(stderr, "Starfield::Starfield()\n");
    int dim = (int)hypot(ww(), wh());

    wander = dim / 2;
    dim += wander;

    set_x(ww2());
    set_y(wh2());

    if (number > 0)
      {
        num_points = number;
        pts = new coords[num_points];

        for (int i = 0; i < num_points; i++) {
          pts[i].x = (Random::get() % 1000000) /1000000.0;
          pts[i].y = (Random::get() % 1000000) /1000000.0;
        }
        for (int i = 0; i < num_points; i++)
          {
            pts[i].x = (pts[i].x * ww() - ww2());
            pts[i].y = (pts[i].y * wh() - wh2());
          }
      } 
    else 
      {
        // hey let's do the original starfield.
        // these stars are slightly askew from the originals
        // because they were taken from a screenshot of a video
        // on youtube that was slightly askew.
        num_points = 21;
        pts = new coords[num_points];
        pts[0].x =0.16582914572864; pts[0].y=0.89072847682119;
        pts[1].x =0.27889447236181; pts[1].y=0.83112582781457;
        pts[2].x =0.46482412060302; pts[2].y=0.81456953642384;
        pts[3].x =0.48743718592965; pts[3].y=0.79139072847682;
        pts[4].x =0.45477386934673; pts[4].y=0.77814569536424;
        pts[5].x =0.84673366834171; pts[5].y=0.88741721854305;
        pts[6].x =0.74623115577889; pts[6].y=0.89072847682119;
        pts[7].x =0.64070351758794; pts[7].y=0.80794701986755;
        pts[8].x =0.6356783919598;  pts[8].y=0.63245033112583;
        pts[9].x =0.88190954773869; pts[9].y=0.63907284768212;
        pts[10].x=0.15326633165829; pts[10].y=0.48344370860927;
        pts[11].x=0.0929648241206;  pts[11].y=0.35430463576159;
        pts[12].x=0.78391959798995; pts[12].y=0.45033112582781;
        pts[13].x=0.66582914572864; pts[13].y=0.40728476821192;
        pts[14].x=0.48743718592965; pts[14].y=0.42715231788079;
        pts[15].x=0.4748743718593;  pts[15].y=0.35099337748344;
        pts[16].x=0.89195979899497; pts[16].y=0.32119205298013;
        pts[17].x=0.82914572864322; pts[17].y=0.25827814569536;
        pts[18].x=0.73366834170854; pts[18].y=0.26158940397351;
        pts[19].x=0.79899497487437; pts[19].y=0.20198675496689;
        pts[20].x=0.85427135678392; pts[20].y=0.20529801324503;
        for (int i = 0; i < num_points; i++)
          {
            pts[i].x = (pts[i].x * ww() - ww2());
            pts[i].y = (pts[i].y * wh() - wh2());
          }
      }
    points = pts;

    xpoints = new XPoint[num_points];

    starsize = max(ww(),wh()) / 400.0;
    starsize = round (starsize);
    if (starsize < 2.0)
      starsize = 2;
    set_size(3.3);

    set_gc(fetch_gc(bright ? GC_BRIGHT_GREY : GC_DULL_GREY));

    set_dx(30.0 / args.fps);
    set_dy(-15.0 / args.fps);
    set_dtheta(22 / args.fps);
} // Starfield::Starfield


Starfield::~Starfield(void)
{
    //fprintf(stderr, "Starfield::~Starfield()\n");
    delete[] pts;
} // Starfield::~Starfield


void
Starfield::move(void)
{
    if ((get_x() + get_dx()) > ww2() + (wander / 2.0) ||
	    (get_x() + get_dx()) < ww2() - (wander / 2.0)) {
	set_dx(-get_dx());
    }
    if ((get_y() + get_dy()) > wh2() + (wander / 2.0) ||
	    (get_y() + get_dy()) < wh2() - (wander / 2.0)) {
	set_dy(-get_dy());
    }
    Xything::move();
} // Starfield::move


void
Starfield::render(const bool ink)
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
                 0, 360*64, false);
    }
}

void
Starfield::resize(const int nwidth, const int nheight)
{
    starsize = max(ww(),wh()) / 400.0;
    starsize = round (starsize);
    if (starsize < 2.0)
      starsize = 2;
    int dim = (int)hypot(nwidth, nheight);
    wander = dim / 2;
    dim += wander;
    Xything::resize(nwidth, nheight);
} // Starfield::resize
