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

#ifndef	MAGNETAR_MAGNETAR_H
#define	MAGNETAR_MAGNETAR_H

#include "args.h"
#include "timing.h"
#include "difficulty.h"

class Stages;
class Game;

extern char *program;
extern Display *display;
extern Window game_window;
extern Window stats_window;
extern int _wwidth;
extern int _wwidth2;
extern int _gwheight;
extern int _gwheight2;
extern struct Args::info args;
extern Game *game;
extern Stamp time_now;
extern GC **gcs;
extern GC **bright_gcs;
extern int _display_height;
extern int _display_width;
extern Stages *stages;

const double DEFAULT_FPS	= 30.0;

void set_window_width(int w);
void set_window_height(int h);
int ww();
int wh();
int ww2();
int wh2();
void set_display_width(int h);
void set_display_height(int w);
int display_width();
int display_height();
#define MAX_RINGS 3

#endif	// MAGNETAR_MAGNETAR_H
