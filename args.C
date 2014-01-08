//      MAGNETAR
//      Copyright (c) 1993-2005 by Mark B. Hanson (mbh@panix.com).
//      Copyright (C) 2013, 2014 Ben Asselstine
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; version 2 of the License.   
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

#include <X11/keysym.h>

#include "global.h"

#include "xsc.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "args.h"
#include "sound.h"

const char *config_dir = "." PACKAGE;
const char *config_file = "config";

void
Args::init(struct info *const a)
{
    a->p1_ccw = XK_d;
    a->p1_cw = XK_f;
    a->p1_thrust = XK_j;
    a->p1_fire = XK_k;
    a->p2_ccw = XK_d;
    a->p2_cw = XK_f;
    a->p2_thrust = XK_j;
    a->p2_fire = XK_k;
    a->pause = XK_p;
    a->quit = XK_q;
    a->add_coin = XK_space;
    a->one = XK_1;
    a->two = XK_2;
    a->test = XK_t;
    a->fps = DEFAULT_FPS;
    a->x = -1;	// no specified position
    a->y = -1;	// no specified position
    a->percent = 87;
    a->nopause = false;
    a->colour= 0;
    a->quiet = false;
    a->free_play = false;
    a->stats = false;
    a->lives = 3;
    a->flip_p2 = false;
    a->extra_life_at = 10000;
    a->coins_per_credit = 1;
    a->quiet_attract = false;
    a->starting_level = 1;
    a->aspect_x = -1;
    a->aspect_y = -1;
    initialize_config_dir();
} // Args::init

void
Args::initialize_config_dir()
{
  char *dir = NULL;
  if (asprintf (&dir, "%s/%s", getenv ("HOME"), config_dir) != -1)
    {
      mkdir(dir, 0755);
      free (dir);
    }
}

char *Args::get_config_file(const char *file)
{
  char *f = NULL;
  if (asprintf (&f, "%s/%s/%s", getenv ("HOME"), config_dir, file) != -1)
    return f;
  return NULL;
}

void
Args::version(const char *program_name)
{
  printf("%s version %s\n", program_name, VERSION);
  printf ("Send bug reports to %s.\n", PACKAGE_BUGREPORT);
}

void
Args::help(const char *program_name)
{
  printf ("\
Usage: %s [OPTION]...\n\
", program_name);
  printf("\
Pilot a spaceship through asteroid fields near a magnetar.\n\
\n\
      --coin KEYCODE      change the keycode which adds coins\n\
      --one KEYCODE       change the keycode which starts a 1 player game\n\
      --p1-ccw KEYCODE    the key to rotate player one's ship anti-clockwise\n\
      --p1-cw KEYCODE     the key to rotate player one's ship clockwise\n\
      --p1-fire KEYCODE   the key to invert the magnetism of player one's ship\n\
      --p1-thrust KEYCODE the key to propel player one's ship forward\n\
      --p2-ccw KEYCODE    the key to rotate player two's ship anti-clockwise\n\
      --p2-cw KEYCODE     the key to rotate player two's ship clockwise\n\
      --p2-fire KEYCODE   the key to invert the magnetism of player two's ship\n\
      --p2-thrust KEYCODE the key to propel player two's ship forward\n\
      --pause KEYCODE     specify keycode which suspends and resumes the game\n\
      --quit KEYCODE      change the keycode which ends the game\n\
      --test KEYCODE      change the keycode which starts the test mode\n\
      --two KEYCODE       change the keycode which starts a 2 player game\n\
      --aspect INT/INT    change the aspect ratio\n\
  -f, --fps FLOAT         try to draw this many frames per second (default %.0f)\n\
      --greyscale         don't draw colour, only draw in variations of grey\n\
      --inverted-mono     don't draw colour, only draw in white and black\n\
      --monochrome        don't draw colour, only draw in black and white\n\
      --nopause           don't pause when the cursor leaves the window\n\
  -p, --percent INT       percent of the display the window should take up\n\
      --stats             put game statistics in ~/magnetar-stats-p[1-2].csv\n\
      --windowpos INT,INT specify the x and y coordinates of the window\n\
      --help              display this help and exit\n\
      --version           display version information and exit\n\
", DEFAULT_FPS);
  fprintf (stdout, "\nKEYCODE refers to the textual representation of an X key symbol.\nFor example, `Return' refers to the enter key, and `Left' refers to the left\narrow and so on.  The default key-bindings are:\n\n");
  fprintf (stdout, "  D turns the ship counter-clockwise\n\
  F turns the ship clockwise\n\
  J runs the thruster\n\
  K fires the laser\n\
  P toggles the pause (moving the cursor out of the window pauses the game)\n\
  Q quits the game\n\
  1 starts a one player game\n\
  2 starts a two player game\n\
  space adds a coin to the game\n\n");
  fprintf(stdout, "High scores are stored in ~/.magnetar/high-score.\n");
  fprintf(stdout, "Configuration is stored in ~/.magnetar/config.\n\n");
  fprintf (stdout, "Send bug reports to %s.\n", PACKAGE_BUGREPORT);

} // Args::help

void
Args::load(struct info *const a)
{
  char *file = get_config_file (config_file);
  if (file)
    {
      load (a, file);
      free (file);
    }
}

void
Args::load(struct info *const a, char *filename)
{
  FILE *fp = fopen (filename, "r");
  if (fp)
    {
      load (a, fp);
      fclose (fp);
    }
}

static void
make_command_line (const char *cmd, int *argc, char ***argv)
{
  char *s1 = strdup (cmd);
  if (s1)
    {
      char *term;
      *argv = NULL;
      *argc = 0;
      for (term = strtok (s1, " \t\n"); term != NULL; term = strtok (NULL, " \t\n"))
        {
          *argv = (char **) realloc (*argv, sizeof (char *) * ((*argc) + 1));
          if (*argv)
            (*argv)[*argc] = strdup (term);
          (*argc)++;
        }
      *argv = (char **) realloc (*argv, sizeof (char *) * ((*argc) + 1));
      if (*argv)
        (*argv)[(*argc)] = 0;
      free (s1);
    }
}

void
Args::load(struct info *const a, FILE *fp)
{
  std::string lines = "args ";
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1)
    lines = lines + std::string(line);

  free(line);
  int argc = 0;
  char **argv = NULL;
  make_command_line (lines.c_str(), &argc, &argv);
  config_set(a, argc, argv);
  for (char **p = argv; NULL != *p; ++p)
    free(*p);

  free(argv);
}

void
Args::save(struct info *const a)
{
  char *file = get_config_file(config_file);
  if (file)
    {
      save (a, file);
      free (file);
    }
}

void
Args::save(struct info *const a, char *filename)
{
  FILE *fp = fopen (filename, "w");
  if (fp)
    {
      save (a, fp);
      fclose (fp);
    }
}

void
Args::save(struct info *const a, FILE *fp)
{
  if (a->flip_p2)
    fprintf (fp, "cocktail\n");

  if (a->free_play)
    fprintf (fp, "free-play\n");
  else if (a->coins_per_credit > 1)
    fprintf (fp, "coins-per-credit %d\n", a->coins_per_credit);

  if (a->extra_life_at == 5000)
    fprintf (fp, "extra-ship-5000\n");
  else if (a->extra_life_at == 15000)
    fprintf (fp, "extra-ship-15000\n");
  else if (a->extra_life_at == 20000)
    fprintf (fp, "extra-ship-20000\n");

  if (a->lives != 3)
    fprintf (fp, "lives %d\n", a->lives);

  if (a->quiet)
    fprintf (fp, "quiet\n");
  else if (a->quiet_attract)
    fprintf (fp, "quiet-attract\n");

  if (a->starting_level != 1)
    fprintf (fp, "starting-level %d\n", a->starting_level);

  if (bonus_volume != 11)
    fprintf (fp, "bonus-volume %d\n", int(bonus_volume));

  if (thrust_volume != 11)
    fprintf (fp, "thrust-volume %d\n", int(thrust_volume));

  if (effects_volume != 11)
    fprintf (fp, "effects-volume %d\n", int(effects_volume));
}

void
Args::set_to_factory_defaults(struct info *const a)
{
  char *file = Args::get_config_file (config_file);
  remove (file);
  free (file);
  Args::init(a);
}

void
Args::config_set(struct info *const a, const int argc,
	const char *const argv[])
{
    for (int i = 1; i < argc; i++) {
	if (!strcmp(argv[i], "quiet")) {
          args.quiet = true;
	} else if (!strcmp(argv[i], "quiet-attract")) {
          args.quiet_attract = true;
	} else if (!strcmp(argv[i], "cocktail")) {
          args.flip_p2 = true;
          a->p2_ccw = XK_x;
          a->p2_cw = XK_c;
          a->p2_thrust = XK_n;
          a->p2_fire = XK_m;
	} else if (!strcmp(argv[i], "extra-ship-15000")) {
          args.extra_life_at = 15000;
	} else if (!strcmp(argv[i], "extra-ship-5000")) {
          args.extra_life_at = 5000;
	} else if (!strcmp(argv[i], "extra-ship-20000")) {
          args.extra_life_at = 20000;
	} else if (!strcmp(argv[i], "free-play")) {
          args.free_play = true;
	} else if (!strcmp(argv[i], "lives")) {
	    if (++i < argc) {
		if ((a->lives = atoi(argv[i])) <= 0) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "lives");
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "lives");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "starting-level")) {
	    if (++i < argc) {
		if ((a->starting_level = atoi(argv[i])) <= 0) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "starting-level");
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "starting-level");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "bonus-volume")) {
	    if (++i < argc) {
		if ((bonus_volume = atof(argv[i])) <= 0) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "bonus-volume");
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "bonus-volume");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "thrust-volume")) {
	    if (++i < argc) {
		if ((thrust_volume = atof(argv[i])) <= 0) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "thrust-volume");
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "thrust-volume");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "effects-volume")) {
	    if (++i < argc) {
		if ((effects_volume = atof(argv[i])) <= 0) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "effects-volume");
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "effects-volume");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "coins-per-credit")) {
	    if (++i < argc) {
		if ((a->coins_per_credit = atoi(argv[i])) <= 0) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "coins-per-credit");
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--coins-per-credit");
		exit(1);
	    }
	} else {
	    fprintf(stderr, "%s: unknown argument `%s'!\n", program, argv[i]);
	    exit(1);
	}
    }
}

void
Args::set(struct info *const a, const int argc,
	const char *const argv[])
{
    for (int i = 1; i < argc; i++) {
	if (!strcmp(argv[i], "--help")) {
          help(basename(argv[0]));
          exit(1);
        } else if (!strcmp(argv[i], "--version")) {
          version(basename(argv[0]));
          exit(1);
        } else if (!strcmp(argv[i], "--p1-ccw")) {
	    if (++i < argc) {
		if ((a->p1_ccw = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--p1-ccw");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--p1-cw")) {
	    if (++i < argc) {
		if ((a->p1_cw = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--p1-cw");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--p1-thrust")) {
	    if (++i < argc) {
		if ((a->p1_thrust = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--p1-thrust");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--p1-fire")) {
	    if (++i < argc) {
		if ((a->p1_fire = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--p1-fire");
		exit(1);
	    }
        } else if (!strcmp(argv[i], "--p2-ccw")) {
	    if (++i < argc) {
		if ((a->p2_ccw = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--p2-ccw");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--p2-cw")) {
	    if (++i < argc) {
		if ((a->p2_cw = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--p2-cw");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--p2-thrust")) {
	    if (++i < argc) {
		if ((a->p2_thrust = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--p2-thrust");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--p2-fire")) {
	    if (++i < argc) {
		if ((a->p2_fire = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--p2-fire");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--pause")) {
	    if (++i < argc) {
		if ((a->pause = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--pause");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--quit")) {
	    if (++i < argc) {
		if ((a->quit = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--quit");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--coin")) {
	    if (++i < argc) {
		if ((a->add_coin = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--coin");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--one")) {
	    if (++i < argc) {
		if ((a->one = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--one");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--two")) {
	    if (++i < argc) {
		if ((a->two = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--one");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--test")) {
	    if (++i < argc) {
		if ((a->test = XStringToKeysym(argv[i])) == NoSymbol) {
		    fprintf(stderr, "%s: cannot convert `%s' to a KeySym!\n",
			    program, argv[i]);
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--one");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--stats")) {
          args.stats = true;
	} else if (!strcmp(argv[i], "--fps") || !strcmp(argv[i], "-f")) {
	    if (++i < argc) {
		if ((a->fps = atoi(argv[i])) <= 0) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "--fps");
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--fps");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--windowpos")) {
	    if (++i < argc) {
              int x = 0, y = 0;
              int retval = sscanf (argv[i], "%d,%d", &x, &y);
              if (retval != 2 || x < 0 || y < 0) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "--windowpos");
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--windowpos");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--aspect")) {
	    if (++i < argc) {
              int x = 0, y = 0;
              int retval = sscanf (argv[i], "%d/%d", &x, &y);
              if (retval != 2 || x <= 0 || y <= 0) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "--aspect");
		    exit(1);
              } else {
                a->aspect_x = x;
                a->aspect_y = y;
              }

	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--aspect");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--percent") || !strcmp(argv[1], "-p")) {
	    if (++i < argc) {
		if ((a->percent = atoi(argv[i])) <= 0 || a->percent > 100) {
		    fprintf(stderr, "%s: bad argument for `%s'!\n",
			    program, "--percent");
		    exit(1);
		}
	    } else {
		fprintf(stderr, "%s: no argument for `%s'!\n",
			program, "--percent");
		exit(1);
	    }
	} else if (!strcmp(argv[i], "--nopause")) {
	    a->nopause = true;
	} else if (!strcmp(argv[i], "--greyscale")) {
	    a->colour = 1;
	} else if (!strcmp(argv[i], "--monochrome")) {
	    a->colour = 2;
	} else if (!strcmp(argv[i], "--inverted-mono")) {
	    a->colour = 3;
	} else {
	    fprintf(stderr, "%s: unknown argument `%s'!\n", program, argv[i]);
	    exit(1);
	}
    }
} // Args::set
