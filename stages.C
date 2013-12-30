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
#include "stages.h"
#include "challenging-stage.h"
#include "random.h"
#include "sound.h"
    
Stages::Stages(King *k, King *q, Minefield *mf, Castle *c, Ship *s, Starfield *l, Game *g)
        : attract(Stage(0, 24+(Random::get()%3)+20, 6.1, 10+8, 0.1, 0.1)),
        empty(Stage(0, 1, 6.1, 1, 10.1, 10.1))
{
  king = k;
  queen = q;
  minefield = mf;
  castle = c;
  ship = s;
  starfield = l;
  game = g;
  int m = 10;
  attract.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.0, 0.5));
  attract.left(MagnetInfo(0.33, 1.00, 22,  1, false, 0.0, 0.2));
  attract.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.1));
  attract.right(MagnetInfo(0.11, 3.00, 33,  1, false, 0.0, 0.9));
  attract.drop(0.05, 0.95);
  empty.drop(0.00, 0.00);

  double ms = 9.1; //mine speed accelerates until stage 16
  stages = new std::vector<Stage>();
  ms -= 0.44;
  Stage stage = Stage(1, 
                      24+(Random::get()%3)+20, 
                      ms, 
                      m+8, 
                      0.1, 
                      0.0);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.0, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, false, 0.0, 0.0));
  stage.drop(0.05, 0.95);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(2, 
                27+(Random::get()%3)+20, 
                ms, 
                m+9.9, 
                0.1,
                0.0);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.0, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, false, 0.0, 0.0));
  stage.drop(0.05, 0.95);
  stages->push_back(stage);

  stage = ChallengingStage(3, 55, 8.1, 55, 2.0);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.0, 0.5));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.drop(0.45, 0.55);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(4, 
                30+(Random::get()%3)+40, 
                8.22, 
                m+11.8, 
                0.1,
                0.01941176470588);
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.66, 1.00, 22,  1, false, 0.0, 0.0));
  stage.drop(0.05, 0.90);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(5, 
                33+(Random::get()%3)+40, 
                7.78, 
                m+13.7, 
                0.1,
                0.03882352941176);
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.66, 1.00, 22,  1, false, 0.0, 0.0));
  stage.drop(0.05, 0.90);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(6, 
                36+(Random::get()%3)+40, 
                7.34, 
                m+15.6, 
                0.1,
                0.05823529411765);
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, false, 0.0, 0.0));
  stage.right(MagnetInfo(0.66, 1.00, 22,  1, false, 0.0, 0.0));
  stage.drop(0.05, 0.90);
  stages->push_back(stage);

  stage = ChallengingStage(7, 55, 8.1, 55, 2.0);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.0, 0.5));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.drop(0.42, 0.58);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(8, 
                39+(Random::get()%3)+60, 
                6.9, 
                m+17.5, 
                0.1,
                0.07764705882353);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, true, 0.0, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, true, 0.0, 0.0));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, true, 0.0, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, true, 0.0, 0.0));
  stage.drop(0.00, 0.20);
  stage.drop(0.40, 0.60);
  stage.drop(0.80, 1.00);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(9, 
                42+(Random::get()%3)+60, 
                6.46, 
                m+19.4, 
                0.1,
                0.09705882352941);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, true, 0.0, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, true, 0.0, 0.0));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, true, 0.0, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, true, 0.0, 0.0));
  stage.drop(0.00, 0.20);
  stage.drop(0.40, 0.60);
  stage.drop(0.80, 1.00);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(10, 
                45+(Random::get()%3)+60, 
                6.02, 
                m+21.4,  //whoops. this one is not regular.
                0.1,
                0.11647058823529);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, true, 0.0, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, true, 0.0, 0.0));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, true, 0.0, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, true, 0.0, 0.0));
  stage.drop(0.00, 0.20);
  stage.drop(0.40, 0.60);
  stage.drop(0.80, 1.00);
  stages->push_back(stage);

  ms -= 0.44;
  stage = ChallengingStage(11, 55, 8.1, 55, 2.0);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.0, 0.5));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.drop(0.40, 0.60);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(12, 
                48+(Random::get()%3)+70, 
                5.58, 
                m+23.2, 
                0.1,
                0.13588235294118);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.5, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, true, 0.0, 0.0));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.0, 0.8));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.2, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, true, 0.3, 0.0));
  stage.drop(0.00, 0.30);
  stage.drop(0.40, 0.60);
  stage.drop(0.80, 0.95);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(13, 
                51+(Random::get()%3)+70, 
                5.14, 
                m+25.1, 
                0.1,
                0.15529411764706);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.6, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, true, 0.0, 0.0));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.0, 0.8));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.2, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, true, 0.3, 0.0));
  stage.drop(0.00, 0.30);
  stage.drop(0.40, 0.60);
  stage.drop(0.80, 0.95);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(14, 
                54+(Random::get()%3)+70, 
                4.7, 
                m+27, 
                0.1,
                0.17470588235294);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.6, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, true, 0.0, 0.0));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.0, 0.8));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.2, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, true, 0.3, 0.0));
  stage.drop(0.00, 0.30);
  stage.drop(0.40, 0.60);
  stage.drop(0.80, 0.95);
  stages->push_back(stage);

  stage = ChallengingStage(15, 100, 8.1, 55, 2.0);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.0, 0.5));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.drop(0.05, 0.15);
  stage.drop(0.80, 0.95);
  stages->push_back(stage);

  ms -= 0.44;
  stage = Stage(16, 
                57+(Random::get()%3)+80, 
                4.7, 
                ms, 
                0.1,
                0.19411764705882);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.5, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, false, 0.5, 0.0));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.5, 0.8));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.5, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, false, 0.5, 0.0));
  stage.right(MagnetInfo(0.33, 3.00, 22,  1, true, 0.5, 0.2));
  stage.drop(0.00, 0.50);
  stage.drop(0.60, 1.00);
  stages->push_back(stage);

  stage = Stage(17, 
                60+(Random::get()%3)+80, 
                ms, 
                m+30.8, 
                0.1,
                0.21352941176471);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.5, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, false, 0.5, 0.0));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.5, 0.8));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.5, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, false, 0.5, 0.0));
  stage.right(MagnetInfo(0.33, 3.00, 22,  1, true, 0.5, 0.2));
  stage.drop(0.00, 0.50);
  stage.drop(0.60, 1.00);
  stages->push_back(stage);

  stage = Stage(18, 
                63+(Random::get()%3)+80, 
                ms, 
                m+32.7, 
                0.1,
                0.23294117647059);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.5, 0.5));
  stage.left(MagnetInfo(0.33, 1.00, 22,  1, false, 0.5, 0.0));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.5, 0.8));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.5, 0.0));
  stage.right(MagnetInfo(0.11, 3.00, 33,  1, false, 0.5, 0.0));
  stage.right(MagnetInfo(0.33, 3.00, 22,  1, true, 0.5, 0.2));
  stage.drop(0.00, 0.50);
  stage.drop(0.60, 1.00);
  stages->push_back(stage);

  stage = ChallengingStage(19, 75, 8.1, 55, 2.0);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.0, 0.5));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.drop(0.05, 0.95);
  stages->push_back(stage);

  stage = Stage(20, 
                66+(Random::get()%3)+90, 
                ms, 
                m+34.6, 
                0.1,
                0.25235294117647);
  stage.left(MagnetInfo(0.66, 3.00, 5, -1, true, 0.4, 0.5));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.4, 0.0));
  stage.left(MagnetInfo(0.33, 0.75, 22,  1, true, 0.4, 0.8));
  stage.right(MagnetInfo(0.99, 3.00, 11,  -1, true, 0.4, 0.0));
  stage.right(MagnetInfo(0.11, 1.00, 33,  1, true, 0.4, 0.0));
  stage.right(MagnetInfo(0.33, 1.00, 22,  1, true, 0.4, 0.2));
  stage.drop(0.00, 1.00);
  stages->push_back(stage);

  stage = Stage(21, 
                69+(Random::get()%3)+90, 
                ms, 
                m+36.5, 
                0.1,
                0.27176470588235);
  stage.left(MagnetInfo(0.66, 3.00, 5, -1, true, 0.4, 0.5));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.4, 0.0));
  stage.left(MagnetInfo(0.33, 0.75, 22,  1, true, 0.4, 0.8));
  stage.right(MagnetInfo(0.99, 3.00, 11,  -1, true, 0.4, 0.0));
  stage.right(MagnetInfo(0.11, 1.00, 33,  1, true, 0.4, 0.0));
  stage.right(MagnetInfo(0.33, 1.00, 22,  1, true, 0.4, 0.2));
  stage.drop(0.00, 1.00);
  stages->push_back(stage);

  stage = Stage(22, 
                72+(Random::get()%3)+90, 
                ms, 
                m+38.4, 
                0.1,
                0.29117647058824);
  stage.left(MagnetInfo(0.66, 3.00, 5, -1, true, 0.4, 0.5));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.4, 0.0));
  stage.left(MagnetInfo(0.33, 0.75, 22,  1, true, 0.4, 0.8));
  stage.right(MagnetInfo(0.99, 3.00, 11,  -1, true, 0.4, 0.0));
  stage.right(MagnetInfo(0.11, 1.00, 33,  1, true, 0.4, 0.0));
  stage.right(MagnetInfo(0.33, 1.00, 22,  1, true, 0.4, 0.2));
  stage.drop(0.00, 1.00);
  stages->push_back(stage);

  stage = ChallengingStage(23, 100, 8.1, 55, 2.0);
  stage.left(MagnetInfo(0.66, 1.00, 5, -1, false, 0.0, 0.5));
  stage.right(MagnetInfo(0.99, 0.75, 11,  -1, false, 0.0, 0.0));
  stage.drop(0.00, 1.00);
  stages->push_back(stage);

  stage = Stage(24, 
                75+(Random::get()%3)+100, 
                ms, 
                m+40.3, 
                0.1,
                0.31058823529412);
  stage.left(MagnetInfo(0.66, 3.00, 5, -1, true, 0.3, 0.5));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.3, 0.0));
  stage.left(MagnetInfo(0.33, 0.75, 22,  1, true, 0.3, 0.8));
  stage.right(MagnetInfo(0.99, 3.00, 11,  -1, true, 0.3, 0.0));
  stage.right(MagnetInfo(0.11, 1.00, 33,  1, true, 0.3, 0.0));
  stage.right(MagnetInfo(0.33, 1.00, 22,  1, true, 0.3, 0.2));
  stage.drop(0.00, 1.00);
  stages->push_back(stage);

  stage = Stage(25, 
                78+(Random::get()%3)+100, 
                ms, 
                m+44.2, 
                0.1,
                0.33);
  stage.left(MagnetInfo(0.66, 3.00, 5, -1, true, 0.3, 0.5));
  stage.left(MagnetInfo(0.33, 3.00, 22,  1, true, 0.3, 0.0));
  stage.left(MagnetInfo(0.33, 0.75, 22,  1, true, 0.3, 0.8));
  stage.right(MagnetInfo(0.99, 3.00, 11,  -1, true, 0.3, 0.0));
  stage.right(MagnetInfo(0.11, 1.00, 33,  1, true, 0.3, 0.0));
  stage.right(MagnetInfo(0.33, 1.00, 22,  1, true, 0.3, 0.2));
  stage.drop(0.00, 1.00);
  stages->push_back(stage);

} // Stages::Stages


Stages::~Stages(void)
{
  delete stages;
} // Stages::~Stages

Stage
Stages::get(int stage_num)
{
  stage_num--;
  int num = stage_num % (stages->size());
  return stages->at(num);
}

void 
Stages::start_stage(int stage_num)
{
  start_stage(get(stage_num));
}

void 
Stages::start_stage(Stage stage)
{
  castle->refresh();
  minefield->erase();
  minefield->snuff();
  if (ship->alive() == false)
    ship->reincarnate();
  ship->reset_energy();
  king->reset();
  if (!king->alive())
    king->reincarnate();
  if (!queen->alive())
    queen->reincarnate();
  game->p()->stats->set_total_mines(stage.get_max_mines());
  minefield->reset(stage.get_max_mines(), stage.get_mines_at_a_time(), stage.get_magnetic_mine_percent());
  minefield->set_drop_zones(stage.get_drop_zones());
  stage.add_magnets(king);
  if (king->count_left_magnets() == 0)
    king->show_force_arrow(false);
  else
    king->show_force_arrow(true);
  if (king->count_right_magnets() == 0)
    queen->show_force_arrow(false);
  else
    queen->show_force_arrow(true);

  if (stage.get_id() != 1)
    play (STAGE_START);
}


unsigned int
Stages::get_max_mines()
{
  return get_stage().get_max_mines();
}
double
Stages::get_avg_launch_time()
{
  return get_stage().get_avg_launch_time();
}
    
bool 
Stages::challenge()
{
  if (!game->p())
    return false;
  return get(game->p()->stats->get_stage()).get_challenging_stage();
}
bool 
Stages::challenge_upcoming()
{
  return get(game->p()->stats->get_stage()+1).get_challenging_stage();
}
bool 
Stages::finished_challenge()
{
  return get(game->p()->stats->get_stage()-1).get_challenging_stage();
}
    
unsigned int 
Stages::get_stage_num()
{
  return get_stage().get_id();
}
unsigned int 
Stages::get_next_stage_num()
{
  return get(game->p()->stats->get_stage()+1).get_id();
}

double
Stages::get_mine_speed()
{
  return get_stage().get_mine_speed();
}

Stage
Stages::get_stage()
{
  if (game->state == STATE_ATTRACT)
    return attract;
  else if (game->p() == NULL)
    return empty;
  else
    return get(game->p()->stats->get_stage());
}
    
unsigned int 
Stages::get_max_stages()
{
  return stages->size();
}
