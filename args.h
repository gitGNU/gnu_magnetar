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

#ifndef	MAGNETAR_ARGS_H
#define	MAGNETAR_ARGS_H

#include <X11/Xlib.h>
namespace Args {

struct info {
    KeySym p1_ccw;	// the key which turns p1's ship counterclockwise
    KeySym p1_cw;	// the key which turns p1's ship clockwise
    KeySym p1_thrust;	// the key which thrusts p1's ship
    KeySym p1_fire;	// the key which fires p1's ship's laser
    KeySym p2_ccw;	// the key which turns p2's ship counterclockwise
    KeySym p2_cw;	// the key which turns p2's ship clockwise
    KeySym p2_thrust;	// the key which thrusts p2's ship
    KeySym p2_fire;	// the key which fires p2's ship's laser
    KeySym pause;	// the key which pauses the game
    KeySym iconify;	// the key which iconifies the window
    KeySym quit;	// the key which quits the game
    KeySym add_coin;	// the key which inserts coins into the game
    KeySym one;       	// the key which starts a 1 player game
    KeySym two;	        // the key which starts a 2 player game
    KeySym test;        // the key which starts test mode
    double fps;		// frames per second
    int x;		// x offset of the window
    int y;		// y offset of the window
    int percent;	// percent of the display the window should take up
    bool nopause;	// don't pause when the cursor leaves the window
    bool quiet;         // don't play any sounds
    bool quiet_attract; // don't play sounds during attract mode
    bool bw;		// black and white display
    bool stats;         // save stats to a file.
    bool free_play;     // don't require credits
    unsigned int lives; // the number of lives a player starts with
    bool flip_p2;       // invert display for second player
    int extra_life_at;  // how many points for a free ship
    int coins_per_credit; //how many coins does one credit cost
    int starting_level; // start the game at this level.
    int aspect_x;
    int aspect_y;       // aspect ratio pair
};

void init(struct info *const);
void set(struct info *const, const int, const char *const []);
void config_set(struct info *const a, const int argc, const char *const argv[]);
void load(struct info *const a);
void save(struct info *const a);
void help(const char *);
void version(const char *);
char *get_config_file(const char *file);
void initialize_config_dir();
void set_to_factory_defaults(struct info *const a);
void load(struct info *const a, FILE *fp);
void load(struct info *const a, char *filename);
void save(struct info *const a, FILE *fp);
void save(struct info *const a, char *filename);

} // namespace Args

#endif	// MAGNETAR_ARGS_H
