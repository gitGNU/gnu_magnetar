//      MAGNETAR
//      Copyright (c) 1993-2005, 2013 by Mark B. Hanson (mbh@panix.com).
//      Copyright (C) 2013, 2014 Ben Asselstine
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

#include "ship.h"	// ship data points

#include "stats.h"
#include "stages.h"
#include "buzzer.h"


Stats::Stats(Ship *s, Game *g)
{
  ship = s;
  game = g;
    //fprintf(stderr, "Stats::Stats()\n");
    last_score = UINT_MAX;	// ensure we get displayed first time
    set_window(game_window);
    text_primary.set_window(game_window);
    text_secondary.set_window(game_window);
    text_p1_score.set_window(game_window);
    text_p2_score.set_window(game_window);
    text_p1.set_window(game_window);
    text_p2.set_window(game_window);
    text_tertiary.set_window(game_window);
    text_quaternary.set_window(game_window);
    text_music.set_window(game_window);
    text_sound.set_window(game_window);
    text_www.set_window(game_window);
    text_mines_to_go.set_window(game_window);
    text_lives_left.set_window(game_window);
    text_energy_bar.set_window(game_window);
    text_initials[0].set_window(game_window);
    text_initial_boxes[0].set_window(game_window);
    text_initials[1].set_window(game_window);
    text_initial_boxes[1].set_window(game_window);
    text_initials[2].set_window(game_window);
    text_initial_boxes[2].set_window(game_window);
    text_copyright.set_window(game_window);
    text_intro.set_window(game_window);
    text_other.set_window(game_window);

    set_scale(8.0);
    text_primary.set_scale(scale);
    text_secondary.set_scale(scale);
    text_p1_score.set_scale(scale);
    text_p2_score.set_scale(scale);
    text_p1.set_scale(scale);
    text_p2.set_scale(scale);
    text_tertiary.set_scale(scale);
    text_quaternary.set_scale(scale);
    text_music.set_scale(scale*1.5);
    text_sound.set_scale(scale*1.5);
    text_www.set_scale(scale*1.5);
    text_other.set_scale(scale*1.5);
    text_mines_to_go.set_scale(scale*1.1);
    text_lives_left.set_scale(scale*1.1);
    text_energy_bar.set_scale(scale*1.0);

    text_initials[0].set_scale(scale / 4.0);
    text_initial_boxes[0].set_scale(scale / 4.0);
    text_initials[1].set_scale(scale / 4.0);
    text_initial_boxes[1].set_scale(scale / 4.0);
    text_initials[2].set_scale(scale / 4.0);
    text_initial_boxes[2].set_scale(scale / 4.0);

    text_copyright.set_scale(scale / 2.3);
    text_intro.set_scale(scale*1.8);

    set_gc(fetch_gc(GC_BRIGHT_GREY));
    text_primary.set_gc(fetch_gc(GC_DULL_BLUE));
    text_secondary.set_gc(fetch_gc(GC_DULL_BLUE));
    text_p1.set_gc(fetch_gc(GC_DULL_BLUE));
    text_p2.set_gc(fetch_gc(GC_DULL_BLUE));
    text_p1_score.set_gc(fetch_gc(GC_DULL_BLUE));
    text_p2_score.set_gc(fetch_gc(GC_DULL_BLUE));
    text_tertiary.set_gc(fetch_gc(GC_DULL_BLUE));
    text_quaternary.set_gc(fetch_gc(GC_DULL_BLUE));
    text_www.set_gc(fetch_gc(GC_DULL_BLUE));
    text_mines_to_go.set_gc(fetch_gc(GC_DULL_ORANGE));
    text_lives_left.set_gc(fetch_gc(GC_BRIGHT_GREY));
    text_energy_bar.set_gc(fetch_gc(GC_DULL_YELLOW));
    text_initials[0].set_gc(fetch_gc(GC_BRIGHT_BLUE));
    text_initial_boxes[0].set_gc(fetch_gc(GC_BRIGHT_YELLOW));
    text_initials[1].set_gc(fetch_gc(GC_BRIGHT_BLUE));
    text_initial_boxes[1].set_gc(fetch_gc(GC_BRIGHT_RED));
    text_initials[2].set_gc(fetch_gc(GC_BRIGHT_BLUE));
    text_initial_boxes[2].set_gc(fetch_gc(GC_BRIGHT_RED));
    text_copyright.set_gc(fetch_gc(GC_BRIGHT_RED));
    text_intro.set_gc(fetch_gc(GC_BRIGHT_RED));
    text_other.set_gc(fetch_gc(GC_DULL_BLUE));
    text_sound.set_gc(fetch_gc(GC_DULL_BLUE));
    text_music.set_gc(fetch_gc(GC_DULL_BLUE));
    text_www.set_gc(fetch_gc(GC_DULL_BLUE));

    set_theta(0.0);
    reset();

    test_menu();
} // Stats::Stats



void Stats::test_menu()
{
  double ss = 1.1;
    menu = new Menu(30, 60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text m1, m2, m3, m4 ,m5 ,m6 ,m7 ,m8 ,m9, m10, m11, m12, m13;
    m1.set_message("cocktail mode");
    m2.set_message("coins per credit");
    m3.set_message("extra ship at");
    m4.set_message("starting ships");
    m5.set_message("sounds");
    m6.set_message("starting stage");
    m7.set_message("sound test");
    m8.set_message("music loudness");
    m9.set_message("thrust loudness");
    m10.set_message("sound loudness");
    m11.set_message("reset high score");
    m12.set_message("restore defaults");
    m13.set_message("finish");


    Menu *cocktail = new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text cocktail1, cocktail2;
    cocktail1.set_message("on");
    cocktail2.set_message("off");
    cocktail->add_entry(cocktail1);
    if (args.flip_p2)
      cocktail->set_active(--cocktail->end());
    cocktail->add_entry(cocktail2);
    if (args.flip_p2 == false)
      cocktail->set_active(--cocktail->end());
    menu->add_entry(m1, cocktail);
    Menu *coins = new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text coins1, coins2, coins3, coins4, freeplay;
    coins1.set_message("1 coin");
    coins2.set_message("2 coins");
    coins3.set_message("3 coins");
    coins4.set_message("4 coins");
    freeplay.set_message("free play");
    coins->add_entry(coins1);
    if (args.coins_per_credit == 1 && args.free_play == false)
      coins->set_active(--coins->end());
    coins->add_entry(coins2);
    if (args.coins_per_credit == 2 && args.free_play == false)
      coins->set_active(--coins->end());
    coins->add_entry(coins3);
    if (args.coins_per_credit == 3 && args.free_play == false)
      coins->set_active(--coins->end());
    coins->add_entry(coins4);
    if (args.coins_per_credit == 4 && args.free_play == false)
      coins->set_active(--coins->end());
    coins->add_entry(freeplay);
    if (args.free_play)
      coins->set_active(--coins->end());
    if (coins->get_active() == coins->end())
      coins->set_active(coins->begin()++);
    menu->add_entry(m2, coins);


    Menu *extraship = new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text extraship5000, extraship10000, extraship15000, extraship20000;
    extraship5000.set_message(" 5000 pts");
    extraship10000.set_message("10000 pts");
    extraship15000.set_message("15000 pts");
    extraship20000.set_message("20000 pts");
    extraship->add_entry(extraship5000);
    if (args.extra_life_at == 5000)
      extraship->set_active(--extraship->end());
    extraship->add_entry(extraship10000);
    if (args.extra_life_at == 10000)
      extraship->set_active(--extraship->end());
    extraship->add_entry(extraship15000);
    if (args.extra_life_at == 15000)
      extraship->set_active(--extraship->end());
    extraship->add_entry(extraship20000);
    if (args.extra_life_at == 20000)
      extraship->set_active(--extraship->end());
    if (extraship->get_active() == extraship->end())
      extraship->set_active(extraship->begin()++);
    menu->add_entry(m3, extraship);

    Menu *ships = new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text ships1, ships2, ships3, ships4, ships5;
    ships1.set_message("1 ship");
    ships2.set_message("2 ships");
    ships3.set_message("3 ships");
    ships4.set_message("4 ships");
    ships5.set_message("5 ships");
    ships->add_entry(ships1);
    if (args.lives == 1)
      ships->set_active(--ships->end());
    ships->add_entry(ships2);
    if (args.lives == 2)
      ships->set_active(--ships->end());
    ships->add_entry(ships3);
    if (args.lives == 3)
      ships->set_active(--ships->end());
    ships->add_entry(ships4);
    if (args.lives == 4)
      ships->set_active(--ships->end());
    ships->add_entry(ships5);
    if (args.lives == 5)
      ships->set_active(--ships->end());
    if (ships->get_active() == ships->end())
      ships->set_active(ships->begin()++);
    menu->add_entry(m4, ships);
    Menu *sound = new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text sound1, sound2, sound3;
    sound1.set_message("no");
    sound2.set_message("yes");
    sound3.set_message("no attract");

    sound->add_entry(sound2);
    if (args.quiet == false && args.quiet_attract == false)
      sound->set_active(--sound->end());
    sound->add_entry(sound1);
    if (args.quiet)
      sound->set_active(--sound->end());
    sound->add_entry(sound3);
    if (args.quiet == false && args.quiet_attract == true)
      sound->set_active(--sound->end());

    menu->add_entry(m5, sound);

    Menu *level = new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text l1, l3, l4, l7, l8, l11, l12, l15, l16, l19, l20, l23, l24;
    l1.set_message("STAGE 1");
    l3.set_message("CHALLENGING STAGE 3");
    l4.set_message("STAGE 4");
    l7.set_message("CHALLENGING STAGE 7");
    l8.set_message("STAGE 8");
    l11.set_message("CHALLENGING STAGE 11");
    l12.set_message("STAGE 12");
    l15.set_message("CHALLENGING STAGE 15");
    l16.set_message("STAGE 16");
    l19.set_message("CHALLENGING STAGE 19");
    l20.set_message("STAGE 20");
    l23.set_message("CHALLENGING STAGE 23");
    l24.set_message("STAGE 24");
    level->add_entry(l1);
    if (args.starting_level == 1)
      level->set_active(--level->end());
    level->add_entry(l3);
    if (args.starting_level == 3)
      level->set_active(--level->end());
    level->add_entry(l4);
    if (args.starting_level == 4)
      level->set_active(--level->end());
    level->add_entry(l7);
    if (args.starting_level == 7)
      level->set_active(--level->end());
    level->add_entry(l8);
    if (args.starting_level == 8)
      level->set_active(--level->end());
    level->add_entry(l11);
    if (args.starting_level == 11)
      level->set_active(--level->end());
    level->add_entry(l12);
    if (args.starting_level == 12)
      level->set_active(--level->end());
    level->add_entry(l15);
    if (args.starting_level == 15)
      level->set_active(--level->end());
    level->add_entry(l16);
    if (args.starting_level == 16)
      level->set_active(--level->end());
    level->add_entry(l19);
    if (args.starting_level == 19)
      level->set_active(--level->end());
    level->add_entry(l20);
    if (args.starting_level == 20)
      level->set_active(--level->end());
    level->add_entry(l23);
    if (args.starting_level == 23)
      level->set_active(--level->end());
    level->add_entry(l24);
    if (args.starting_level == 24)
      level->set_active(--level->end());
    if (level->get_active() == level->end())
      level->set_active(level->begin()++);
    menu->add_entry(m6, level);

    Menu *effects = new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text effect1, effect2, effect3, effect4, effect5, effect6, effect7, effect8,
         effect9, effect10, effect11, effect12, effect13, effect14, effect15,
         effect16, effect17, effect18, effect19, effect20;
    effect1.set_message("ship destroyed");
    effect2.set_message("ship fire laser");
    effect3.set_message("asteroid destroyed");
    effect4.set_message("game start");
    effect5.set_message("extra ship awarded");
    effect6.set_message("bonus obtained");
    effect7.set_message("stage prepare");
    effect8.set_message("challenging stage");
    effect9.set_message("bonus disappearing");
    effect10.set_message("bonus appearing");
    effect11.set_message("coin up");
    effect12.set_message("game over");
    effect13.set_message("ship invert polarity");
    effect14.set_message("congratulations");
    effect15.set_message("asteroid leaving");
    effect16.set_message("ship deflect");
    effect17.set_message("stage start");
    effect18.set_message("ship thrust");
    effect19.set_message("music");
    effect20.set_message("finish testing sounds");
    effects->add_entry(effect1);
    effects->add_entry(effect2);
    effects->add_entry(effect3);
    effects->add_entry(effect4);
    effects->add_entry(effect5);
    effects->add_entry(effect6);
    effects->add_entry(effect7);
    effects->add_entry(effect8);
    effects->add_entry(effect9);
    effects->add_entry(effect10);
    effects->add_entry(effect11);
    effects->add_entry(effect12);
    effects->add_entry(effect13);
    effects->add_entry(effect14);
    effects->add_entry(effect15);
    effects->add_entry(effect16);
    effects->add_entry(effect17);
    effects->add_entry(effect18);
    effects->add_entry(effect19);
    effects->add_entry(effect20);
    effects->set_active(effects->begin()++);
    menu->add_entry(m7, effects);
    Menu *bonus = new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text bonus1, bonus2, bonus3, bonus4, bonus5, bonus6, bonus7, bonus8,
         bonus9, bonus10, bonus11, bonus12;
    bonus1.set_message("0");
    bonus2.set_message("1");
    bonus3.set_message("2");
    bonus4.set_message("3");
    bonus5.set_message("4");
    bonus6.set_message("5");
    bonus7.set_message("6");
    bonus8.set_message("7");
    bonus9.set_message("8");
    bonus10.set_message("9");
    bonus11.set_message("10");
    bonus12.set_message("11");
    menu->add_entry(m8, bonus);
    bonus->add_entry(bonus1);
    if (bonus_volume == 0.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus2);
    if (bonus_volume == 1.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus3);
    if (bonus_volume == 2.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus4);
    if (bonus_volume == 3.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus5);
    if (bonus_volume == 4.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus6);
    if (bonus_volume == 5.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus7);
    if (bonus_volume == 6.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus8);
    if (bonus_volume == 7.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus9);
    if (bonus_volume == 8.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus10);
    if (bonus_volume == 9.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus11);
    if (bonus_volume == 10.0)
      bonus->set_active(--bonus->end());
    bonus->add_entry(bonus12);
    if (bonus_volume == 11.0)
      bonus->set_active(--bonus->end());

    Menu *thrust= new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text thrus1, thrus2, thrus3, thrus4, thrus5, thrus6, thrus7, thrus8,
         thrus9, thrus10, thrus11, thrus12;
    thrus1.set_message("0 ");
    thrus2.set_message("1 ");
    thrus3.set_message("2 ");
    thrus4.set_message("3 ");
    thrus5.set_message("4 ");
    thrus6.set_message("5 ");
    thrus7.set_message("6 ");
    thrus8.set_message("7 ");
    thrus9.set_message("8 ");
    thrus10.set_message("9 ");
    thrus11.set_message("10 ");
    thrus12.set_message("11 ");
    menu->add_entry(m9, thrust);
    thrust->add_entry(thrus1);
    if (thrust_volume == 0.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus2);
    if (thrust_volume == 1.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus3);
    if (thrust_volume == 2.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus4);
    if (thrust_volume == 3.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus5);
    if (thrust_volume == 4.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus6);
    if (thrust_volume == 5.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus7);
    if (thrust_volume == 6.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus8);
    if (thrust_volume == 7.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus9);
    if (thrust_volume == 8.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus10);
    if (thrust_volume == 9.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus11);
    if (thrust_volume == 10.0)
      thrust->set_active(--thrust->end());
    thrust->add_entry(thrus12);
    if (thrust_volume == 11.0)
      thrust->set_active(--thrust->end());

    Menu *volume = new Menu(ww2(),60, fetch_gc(GC_DULL_YELLOW), fetch_gc(GC_DULL_BLUE), scale*ss);
    Text vol1, vol2, vol3, vol4, vol5, vol6, vol7, vol8, vol9, vol10, vol11,
         vol12;
    vol1.set_message("0  ");
    vol2.set_message("1  ");
    vol3.set_message("2  ");
    vol4.set_message("3  ");
    vol5.set_message("4  ");
    vol6.set_message("5  ");
    vol7.set_message("6  ");
    vol8.set_message("7  ");
    vol9.set_message("8  ");
    vol10.set_message("9  ");
    vol11.set_message("10  ");
    vol12.set_message("11  ");
    menu->add_entry(m10, volume);
    volume->add_entry(vol1);
    if (effects_volume == 0.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol2);
    if (effects_volume == 1.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol3);
    if (effects_volume == 2.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol4);
    if (effects_volume == 3.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol5);
    if (effects_volume == 4.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol6);
    if (effects_volume == 5.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol7);
    if (effects_volume == 6.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol8);
    if (effects_volume == 7.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol9);
    if (effects_volume == 8.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol10);
    if (effects_volume == 9.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol11);
    if (effects_volume == 10.0)
      volume->set_active(--volume->end());
    volume->add_entry(vol12);
    if (effects_volume == 11.0)
      volume->set_active(--volume->end());
    menu->add_entry(m11);
    menu->add_entry(m12);
    menu->add_entry(m13);
    menu->set_active(menu->begin()++);
}

void Stats::finish_test_mode()
{
  std::list<std::string> selections = menu->get_selections();
  for (std::list<std::string>::iterator i = selections.begin(); 
       i != selections.end(); i++)
    {
      if (strcmp((*i).c_str(),"on") == 0)
        args.flip_p2 = true;
      else if (strcmp((*i).c_str(), "off") == 0)
        args.flip_p2 = false;
      else if (strcmp((*i).c_str(),"1 coin") == 0)
        {
          args.coins_per_credit = 1;
          args.free_play = false;
        }
      else if (strcmp((*i).c_str(),"2 coins") == 0)
        {
          args.coins_per_credit = 2;
          args.free_play = false;
        }
      else if (strcmp((*i).c_str(),"3 coins") == 0)
        {
          args.coins_per_credit = 3;
          args.free_play = false;
        }
      else if (strcmp((*i).c_str(),"4 coins") == 0)
        {
          args.coins_per_credit = 4;
          args.free_play = false;
        }
      else if (strcmp((*i).c_str(),"free play") == 0)
        {
          args.coins_per_credit = 1;
          args.free_play = true;
        }
      else if (strcmp((*i).c_str()," 5000 pts") == 0)
        args.extra_life_at = 5000;
      else if (strcmp((*i).c_str(),"10000 pts") == 0)
        args.extra_life_at = 10000;
      else if (strcmp((*i).c_str(),"15000 pts") == 0)
        args.extra_life_at = 15000;
      else if (strcmp((*i).c_str(),"20000 pts") == 0)
        args.extra_life_at = 20000;
      else if (strcmp((*i).c_str(),"1 ship") == 0)
        args.lives = 1;
      else if (strcmp((*i).c_str(),"2 ships") == 0)
        args.lives = 2;
      else if (strcmp((*i).c_str(),"3 ships") == 0)
        args.lives = 3;
      else if (strcmp((*i).c_str(),"4 ships") == 0)
        args.lives = 4;
      else if (strcmp((*i).c_str(),"5 ships") == 0)
        args.lives = 5;
      else if (strcmp((*i).c_str(),"no") == 0)
        {
          args.quiet = true;
          args.quiet_attract = false;
        }
      else if (strcmp((*i).c_str(),"yes") == 0)
        {
          args.quiet = false;
          args.quiet_attract = false;
        }
      else if (strcmp((*i).c_str(),"no attract") == 0)
        {
          args.quiet = false;
          args.quiet_attract = true;
        }
      else if (strcmp((*i).c_str(),"STAGE 1") == 0)
        args.starting_level = 1;
      else if (strcmp ((*i).c_str(), "CHALLENGING STAGE 3") == 0)
        args.starting_level = 3;
      else if (strcmp((*i).c_str(),"STAGE 4") == 0)
        args.starting_level = 4;
      else if (strcmp ((*i).c_str(), "CHALLENGING STAGE 7") == 0)
        args.starting_level = 7;
      else if (strcmp((*i).c_str(),"STAGE 8") == 0)
        args.starting_level = 8;
      else if (strcmp ((*i).c_str(), "CHALLENGING STAGE 11") == 0)
        args.starting_level = 11;
      else if (strcmp((*i).c_str(),"STAGE 12") == 0)
        args.starting_level = 12;
      else if (strcmp ((*i).c_str(), "CHALLENGING STAGE 15") == 0)
        args.starting_level = 15;
      else if (strcmp((*i).c_str(),"STAGE 16") == 0)
        args.starting_level = 16;
      else if (strcmp ((*i).c_str(), "CHALLENGING STAGE 19") == 0)
        args.starting_level = 19;
      else if (strcmp((*i).c_str(),"STAGE 20") == 0)
        args.starting_level = 20;
      else if (strcmp ((*i).c_str(), "CHALLENGING STAGE 23") == 0)
        args.starting_level = 23;
      else if (strcmp((*i).c_str(),"STAGE 24") == 0)
        args.starting_level = 24;

      //bonus music volume
      else if (strcmp((*i).c_str(), "0") == 0)
        bonus_volume = 0;
      else if (strcmp((*i).c_str(), "1") == 0)
        bonus_volume = 1;
      else if (strcmp((*i).c_str(), "2") == 0)
        bonus_volume = 2;
      else if (strcmp((*i).c_str(), "3") == 0)
        bonus_volume = 3;
      else if (strcmp((*i).c_str(), "4") == 0)
        bonus_volume = 4;
      else if (strcmp((*i).c_str(), "5") == 0)
        bonus_volume = 5;
      else if (strcmp((*i).c_str(), "6") == 0)
        bonus_volume = 6;
      else if (strcmp((*i).c_str(), "7") == 0)
        bonus_volume = 7;
      else if (strcmp((*i).c_str(), "8") == 0)
        bonus_volume = 8;
      else if (strcmp((*i).c_str(), "9") == 0)
        bonus_volume = 9;
      else if (strcmp((*i).c_str(), "10") == 0)
        bonus_volume = 10;
      else if (strcmp((*i).c_str(), "11") == 0)
        bonus_volume = 11;
      //thrust volume
      else if (strcmp((*i).c_str(), "0 ") == 0)
        thrust_volume = 0;
      else if (strcmp((*i).c_str(), "1 ") == 0)
        thrust_volume = 1;
      else if (strcmp((*i).c_str(), "2 ") == 0)
        thrust_volume = 2;
      else if (strcmp((*i).c_str(), "3 ") == 0)
        thrust_volume = 3;
      else if (strcmp((*i).c_str(), "4 ") == 0)
        thrust_volume = 4;
      else if (strcmp((*i).c_str(), "5 ") == 0)
        thrust_volume = 5;
      else if (strcmp((*i).c_str(), "6 ") == 0)
        thrust_volume = 6;
      else if (strcmp((*i).c_str(), "7 ") == 0)
        thrust_volume = 7;
      else if (strcmp((*i).c_str(), "8 ") == 0)
        thrust_volume = 8;
      else if (strcmp((*i).c_str(), "9 ") == 0)
        thrust_volume = 9;
      else if (strcmp((*i).c_str(), "10 ") == 0)
        thrust_volume = 10;
      else if (strcmp((*i).c_str(), "11 ") == 0)
        thrust_volume = 11;
      //thrust volume
      else if (strcmp((*i).c_str(), "0  ") == 0)
        effects_volume = 0;
      else if (strcmp((*i).c_str(), "1  ") == 0)
        effects_volume = 1;
      else if (strcmp((*i).c_str(), "2  ") == 0)
        effects_volume = 2;
      else if (strcmp((*i).c_str(), "3  ") == 0)
        effects_volume = 3;
      else if (strcmp((*i).c_str(), "4  ") == 0)
        effects_volume = 4;
      else if (strcmp((*i).c_str(), "5  ") == 0)
        effects_volume = 5;
      else if (strcmp((*i).c_str(), "6  ") == 0)
        effects_volume = 6;
      else if (strcmp((*i).c_str(), "7  ") == 0)
        effects_volume = 7;
      else if (strcmp((*i).c_str(), "8  ") == 0)
        effects_volume = 8;
      else if (strcmp((*i).c_str(), "9  ") == 0)
        effects_volume = 9;
      else if (strcmp((*i).c_str(), "10  ") == 0)
        effects_volume = 10;
      else if (strcmp((*i).c_str(), "11  ") == 0)
        effects_volume = 11;
    }
  Args::save(&args);
}

Stats::~Stats(void)
{
  menu->delete_submenus();
  delete menu;
    //fprintf(stderr, "Stats::~Stats()\n");
} // Stats::~Stats


char *
Stats::get_file()
{
  char *file = NULL;
  if (game->get_number_of_players() == 0)
    return NULL;
  if (asprintf(&file, "%s/%s-stats-p%d.csv", getenv("HOME"), PACKAGE, game->p()->get_id()) == -1)
    return NULL;
  return file;
}

void
Stats::clear_stats_file()
{
  char *file = get_file();
  if (file)
    {
      FILE *fp = fopen (file, "w");
      if (fp)
        {
          fprintf (fp, "time, score, lives left, buzzers destroyed, ring segments destroyed, ring segments hit, castles destroyed, lasers shot, fireballs launched, rings regenerated, kills by buzzers, kills by fireballs, buzzers launched, waves\n");
          fclose (fp);
        }
      free (file);
    }
}
void
Stats::write_stats_record()
{
  char *file = get_file();
  if (file)
    {
      FILE *fp = fopen (file, "a");
      if (fp)
        {
          Stamp now;
          XSCTime::now(&now);

          fprintf (fp, 
                   "%lf, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
                   double(now),
                   score, lives, buzzers_destroyed,
                   ring_segments_destroyed, ring_segments_hit, 
                   castles_destroyed, lasers_shot, fireballs_launched, 
                   rings_regenerated, killed_by_buzzer, killed_by_fireball, 
                   buzzers_launched, waves);
          fclose (fp);
        }
      free (file);
    }
}
          

void
Stats::reset(void)
{
  score = 0;
  lives = args.lives-1;
  lasers_shot = 0;
  ring_segments_destroyed = 0;
  castles_destroyed = 0;
  ring_segments_hit = 0;
  buzzers_destroyed = 0;
  fireballs_launched = 0;
  rings_regenerated = 0;
  killed_by_buzzer = 0;
  killed_by_fireball = 0; 
  buzzers_launched = 0;

  mines_expired = 0;
  rewards_received = 0;
  total_mines_in_wave = 0;
  waves = args.starting_level; //starts at 1 not 0
  bonus_points = 0;

  died_on_challenging_stage = false;
  mines_passed = 0;
  mines_shot = 0;
  num_polarity_switches_in_wave = 0;
  num_castles_destroyed_in_wave = 0;
  score_in_wave = 0;

  game->state = STATE_NORMAL;
  snprintf (initials, sizeof (initials), "AAA");
  current_initial = 0;

} // Stats::reset

int
Stats::calculate_max_energy_ticks()
{
  double start = text_lives_left.get_x() + text_lives_left.calculate_width();
  double finish = text_mines_to_go.get_x();
  int max = (finish-start-(text_energy_bar.get_charwidth()/3*16)) / (text_energy_bar.get_charwidth()/3);
  return max;
}

void
Stats::fill_energy_bar(char *buf, size_t buflen, double filled)
{
  int max = calculate_max_energy_ticks();
  unsigned int ticks = max * filled;
  for (unsigned int i = 0; i < ticks; i++)
    {
      buf[i] = '|';
      if (i + 1 == buflen)
        {
          buf[i]='\0';
          break;
        }
    }
  if (buflen > ticks)
    buf[ticks]='\0';
  return;
}

void
Stats::render(const bool ink)
{
    //const double charwidth = text_primary.get_size() / 6.0;

    if (ink) {
      char buf[256];
      static char scorestr[20 + 1];	// 64 bits worth + terminator

      last_score = score;

      text_copyright.set_message("");
      text_music.set_message("");
      text_sound.set_message("");
      text_www.set_message("");
      text_secondary.set_message("");
      text_primary.set_message("");
      text_p1_score.set_message("");
      text_p1.set_message("");
      text_p2_score.set_message("");
      text_p2.set_message("");
      text_tertiary.set_message("");
      text_mines_to_go.set_message("");
      text_energy_bar.set_message("");
      text_quaternary.set_message("");
      text_lives_left.set_message("");
      text_intro.set_message("");
      text_other.set_message("");
      for (int i = 0; i < 3; i++)
        {
          text_initials[i].set_message("");
          text_initial_boxes[i].set_message("");
        }

	switch (game->state) 
          {
          case STATE_COPYRIGHT:
            text_copyright.set_message("MAGNETAR");
            text_intro.set_message("your ship is at the mercy of the");
            text_other.set_message("Some code: Mark B. Hanson");
            text_music.set_message("Music: Andrey Avkhimovich");
            text_sound.set_message("Some Sounds: Stephen M. Cameron");
            text_www.set_message("http://magnetar.nongnu.org");
            text_secondary.set_message("with a name from 1992");
            text_primary.set_message("the 1979 arcade game");
            text_tertiary.set_message("Licensed under the gpl");
            text_quaternary.set_message("@ 2013-2014 Ben Asselstine");
            break;
          case STATE_TEST:
            text_primary.set_message("TEST MODE");
            text_tertiary.set_message("use thrust to select");
            text_quaternary.set_message("use left/right to go up/down");
            break;
          case STATE_NORMAL:
            if (stages->challenge() == false)
              {
                snprintf (buf, sizeof (buf), "%03d", total_mines_in_wave - mines_expired);
                text_mines_to_go.set_message(buf);
                fill_energy_bar (buf, sizeof (buf), ship->get_energy());
                text_energy_bar.set_message(buf);
                if (ship->get_energy() > 0.5)
                  text_energy_bar.set_gc(fetch_gc(GC_DULL_YELLOW));
                else if (ship->get_energy() > 0.1)
                  text_energy_bar.set_gc(fetch_gc(GC_DULL_ORANGE));
                else
                  text_energy_bar.set_gc(fetch_gc(GC_DULL_RED));
              }
            if (stages->challenge() == false)
              {
                if (lives >= 0)
                  {
                    snprintf (buf, sizeof (buf), "%03d", lives % 1000);
                    text_lives_left.set_message(buf);
                  }
              }
            break;
          case STATE_WAVE_COMPLETE:
            if (stages->challenge() == false)
              {
                snprintf (buf, sizeof (buf), "STAGE %d COMPLETE", (stages->get_stage_num())%256 );
                text_tertiary.set_message(buf);
              }
            break;
          case STATE_ADD_PLAYER_BONUSES:
            if (stages->challenge() == false)
              {
                snprintf (buf, sizeof (buf), "%5d ENERGY POINTS", 
                          bonus_points);
                text_secondary.set_message(buf);
              }

            if (stages->challenge())
              snprintf (buf, sizeof (buf), "%d pts FOR %d ASTEROIDS", 
                        mines_shot * 10, mines_shot);
            else
              snprintf (buf, sizeof (buf), "%d pts FOR %d ASTEROIDS", 
                        mines_passed * 10, mines_passed);
            text_tertiary.set_message(buf);

            if (stages->challenge())
              {
                if (!died_on_challenging_stage)
                  text_quaternary.set_message("100 POINTS FOR NOT CRASHING");
              }
            else
              {
                if (num_castles_destroyed_in_wave == 1)
                  snprintf (buf, sizeof (buf), "%d pts for %d bonus",
                            num_castles_destroyed_in_wave * 
                            Castle::points_awarded, 
                            num_castles_destroyed_in_wave);
                else if (num_castles_destroyed_in_wave > 1)
                  snprintf (buf, sizeof (buf), "%d pts for %d bonuses",
                            num_castles_destroyed_in_wave * 
                            Castle::points_awarded,
                            num_castles_destroyed_in_wave);
                else
                  buf[0]='\0';
                text_quaternary.set_message(buf);
              }
            if (stages->challenge())
              {
                if (get_mines_shot() == stages->get_max_mines())
                  text_secondary.set_message("SPECIAL BONUS 1000 POINTS");
                ship->lose_energy(1);
                //text_energy_bar.set_message("");
              }
            else
              {
                if (ship->has_energy_left())
                  {
                    fill_energy_bar (buf, sizeof (buf), ship->get_energy());
                    text_energy_bar.set_message(buf);
                      {
                        double amount = ship->lose_energy(1 / args.fps/ 4);
                        int bonus = int((double)amount * 500.0);
                        bonus_points += bonus;
                        add_score (bonus);
                      }
                  }
                else
                  {
                    text_energy_bar.set_message("");
                    snprintf (buf, sizeof (buf), "%5d ENERGY POINTS", 
                              bonus_points);
                    text_secondary.set_message(buf);


                  }
              }
            if (stages->challenge() == false)
              {
                if (ship->has_energy_left() == false)
                  {
                    snprintf (buf, sizeof (buf), "%d TOTAL PTS", 
                              score_in_wave);
                    text_copyright.set_message(buf);
                  }
              }
            else 
              {
                    snprintf (buf, sizeof (buf), "%d TOTAL PTS", 
                              score_in_wave);
                    text_copyright.set_message(buf);
              }
            break;
          case STATE_SHOW_PLAYER_SCORE:
            bonus_points = 0;
            if (ship->alive() == false) 
              {
                if (lives != 0) 
                  {
                    memset (buf, 0, sizeof (buf));
                    if (lives == 1)
                      snprintf (buf, sizeof (buf), "%3d SHIP LEFT", 
                                lives %999);
                    else if (lives > 0)
                      snprintf (buf, sizeof (buf), "%3d SHIPS LEFT", 
                                lives %999);
                    text_secondary.set_message(buf);
                  } 
                else 
                  {
                    strcpy (buf, "FINAL SHIP");
                    text_secondary.set_message(buf);
                  }
              }

            sprintf(scorestr, "%u", score);
            text_primary.set_message(scorestr);
            if (ship->alive() ||
                (ship->alive() == false && stages->challenge()))
              {
                if (stages->challenge_upcoming())
                  {
                    snprintf (buf, sizeof (buf), "PRESS FIRE TO SHOOT");
                    text_quaternary.set_message("CHALLENGING STAGE");
                  }
                else
                  snprintf (buf, sizeof (buf), "PREPARE FOR STAGE %d", stages->get_next_stage_num()%256 );
                text_tertiary.set_message(buf);

              }

            break;
          case STATE_GET_READY:
            sprintf(scorestr, "%u", score);
            text_primary.set_message(scorestr);
            snprintf (buf, sizeof (buf), "PREPARE FOR STAGE %d", stages->get_stage_num());
            text_tertiary.set_message(buf);
            snprintf (buf, sizeof (buf), "Player %d", game->p()->get_id());
            text_quaternary.set_message(buf);
            break;
          case STATE_PAUSED:
            text_primary.set_message("PAUSED");
            text_tertiary.set_message("hit p to keep playing");
            break;
          case STATE_OVER:

            text_primary.set_message("GAME OVER");
            if (game->get_number_of_players() > 1)
              {
                snprintf (buf, sizeof (buf), "Player %d", game->p()->get_id());
                text_secondary.set_message(buf);
              }
            sprintf(scorestr, "%u", score);
            text_tertiary.set_message(scorestr);
            text_quaternary.set_message("FINAL SCORE");
            break;
          case STATE_INITIALS:
            text_primary.set_message("CONGRATULATIONS");
            snprintf (buf, sizeof (buf), "%u IS THE TOP SCORE!", score);
            text_secondary.set_message(buf);
            if (completed_entering_initials())
              {
                text_tertiary.set_message("");
                text_quaternary.set_message("");
              }
            else
              {
                if (seconds_left <= 10)
                  {
                    snprintf (buf, sizeof (buf), "%d SECONDS LEFT", seconds_left);
                    text_quaternary.set_message(buf);
                    text_tertiary.set_message("TO FINISH ENTERING INITIALS");
                  }
                else
                  {
                    text_tertiary.set_message("PRESS FIRE TO SELECT THE INITIAL");
                    text_quaternary.set_message("USE LEFT AND RIGHT TO CHANGE LETTERS");
                  }
              }

            for (unsigned int i = 0; i < 3; i++)
              {
                if (i < current_initial)
                  text_initial_boxes[i].set_message(" ");
                else
                  text_initial_boxes[i].set_message("0");
              }
            text_initials[0].set_message(initials[0]);
            text_initials[1].set_message(initials[1]);
            text_initials[2].set_message(initials[2]);
            break;
          case STATE_HIGH_SCORES:
            if (game->high_score > 0)
              {
                text_primary.set_message("HIGH SCORE");
                snprintf (buf, sizeof (buf), "%d by %s", game->high_score, game->high_score_initials);
                text_secondary.set_message(buf);
              }
            else
              text_secondary.set_message("magnetar");

            if (game->p(1))
              sprintf(scorestr, "%u", game->p(1)->stats->score);
            else
              sprintf(scorestr, "%u", 0);
            text_p1.set_message("PLAYER ONE");
            text_p1_score.set_message(scorestr);
            text_p2.set_message("PLAYER TWO");
            if (game->p(2))
              sprintf(scorestr, "%u", game->p(2)->stats->score);
            else
              sprintf(scorestr, "0");
            text_p2_score.set_message(scorestr);
            if (game->high_score > 0)
              text_tertiary.set_message("MAGNETAR");
            break;
          case STATE_ATTRACT:
            text_secondary.set_message("MAGNETAR");
            text_p1_score.set_message(" COINS");
            text_p1.set_message(" INSERT");
            text_p2_score.set_message("COINS ");
            text_p2.set_message("INSERT ");
            static int count = 0;
            if (count > 2000 / args.fps)
              {
                text_tertiary.render(false);
                text_tertiary.set_message("SWITCH POLARITY OF THE SHIP");
              }
            else
              {
                text_tertiary.render(false);
                text_tertiary.set_message("STEER CLEAR OF ASTEROIDS");
              }
            count++;
            if (count > 4000 / args.fps)
              count = 0;
            break;
          case STATE_PRESS_START:
            if (game->canStart(2))
              {
                text_primary.set_message("PRESS");
                text_secondary.set_message("ONE OR TWO PLAYER START");
              }
            else if (game->canStart(1))
              {
                text_primary.set_message("PRESS");
                text_secondary.set_message("ONE PLAYER START");
              }
            else
              {
                text_primary.set_message("INSERT");
                text_secondary.set_message("MORE COINS");
              }
            snprintf (buf, sizeof (buf), "CREDITS %s", game->get_credits());
            text_tertiary.set_message(buf);
            break;
          default:
            break;
          }
	last_lives = lives;
	game->last_state = game->state;
    }

    // display the score

    //y = 0.0 / 2.0;

    text_primary.set_position(-1.0, text_primary.get_charheight()/2);
    text_primary.render(ink);

    text_secondary.set_position(-1.0, (text_primary.get_charheight()/2) + (text_primary.get_charheight()));
    text_secondary.render(ink);


    text_tertiary.set_position(-1.0, wh()-((text_tertiary.get_charheight()/2+text_tertiary.get_charheight())));
    text_tertiary.render(ink);

    text_quaternary.set_position(-1.0, wh()-((text_quaternary.get_charheight()/2)+(text_quaternary.get_charheight()*2)));
    text_quaternary.render(ink);

    text_www.set_position(-1.0, wh()-((text_www.get_charheight()/2)+(text_www.get_charheight()*6)));
    text_www.render(ink);

    text_sound.set_position(-1.0, wh()-((text_sound.get_charheight()/2)+(text_sound.get_charheight()*7)));
    text_sound.render(ink);

    text_music.set_position(-1.0, wh()-((text_music.get_charheight()/2)+(text_music.get_charheight()*8)));
    text_music.render(ink);

    text_other.set_position(-1.0, wh()-((text_other.get_charheight()/2)+(text_other.get_charheight()*9)));
    text_other.render(ink);
    // display the p1 and p2 score (during high score state)
    text_p1.set_position((ww2() + ((text_p1.get_size()/text_p1.get_scale())/ 2.0) - (text_p1.calculate_width() / 2.0))/4.0, 
                         (wh()-(text_p1.get_charheight()*1.5)) / 2);
    text_p1.render(ink);
    text_p1_score.set_position(((ww2()+ ((text_p1.get_size()/text_p1.get_scale())/ 2.0) - (text_p1.calculate_width() / 2.0))/4.0)+(text_p1.calculate_width()/2.0)-(text_p1_score.calculate_width()/2.0), 
                               ((wh()+(text_p1_score.get_charheight()*0.5)) / 2));
    text_p1_score.render(ink);
    text_p2.set_position((ww2() + ((text_p2.get_size()/text_p2.get_scale())/ 2.0) - (text_p2.calculate_width() / 2.0))*1.75, 
                         (wh()-(text_p1.get_charheight()*1.5)) / 2);

    text_p2.render(ink);
    text_p2_score.set_position(((ww2() + ((text_p2.get_size()/text_p2.get_scale())/ 2.0) - (text_p2.calculate_width() / 2.0))*1.75) + (text_p2.calculate_width()/2.0)- (text_p2_score.calculate_width()/2.0), 
                               ((wh()+(text_p1_score.get_charheight()*0.5)) / 2));
    text_p2_score.render(ink);
   
    text_mines_to_go.set_position(ww()-(text_mines_to_go.get_size()*1.5), wh()-(text_mines_to_go.get_charheight()));
    text_mines_to_go.render(ink);

    text_lives_left.set_position((text_lives_left.get_size()), wh()-(text_lives_left.get_charheight()));
    text_lives_left.render(ink);
    if (game->state == STATE_ADD_PLAYER_BONUSES)
      {
        text_energy_bar.set_scale(scale);
        text_energy_bar.set_position(text_mines_to_go.get_size()*1.8, wh2()/2);
      }
    else
      {
        text_energy_bar.set_scale(scale);
        text_energy_bar.set_position(text_mines_to_go.get_size()*1.8, wh()-(text_energy_bar.get_charheight()));
      }
    text_energy_bar.render(ink);

    text_initial_boxes[0].set_position(ww2() - (text_initial_boxes[0].get_size()/2.0/4.0)-(text_initial_boxes[0].get_size()/4.0)+(text_initial_boxes[0].get_size()/4.0/2.0), wh2() - (text_initial_boxes[0].get_size()/2.0/4.0));
    text_initial_boxes[0].render(ink);
    text_initial_boxes[1].set_position(ww2() - (text_initial_boxes[1].get_size()/2.0/4.0)+(text_initial_boxes[1].get_size()/4.0/2.0), wh2() - (text_initial_boxes[1].get_size()/2.0/4.0));
    text_initial_boxes[1].render(ink);
    text_initial_boxes[2].set_position(ww2() - (text_initial_boxes[2].get_size()/2.0/4.0)+(text_initial_boxes[2].get_size()/4.0)+(text_initial_boxes[2].get_size()/4.0/2.0), wh2() - (text_initial_boxes[2].get_size()/2.0/4.0));
    text_initial_boxes[2].render(ink);
    text_initials[0].set_position(ww2() - (text_initials[0].get_size()/2.0/4.0)-(text_initials[0].get_size()/4.0)+(text_initials[0].get_size()/4.0/2.0), wh2() - (text_initials[0].get_size()/2.0/4.0) - (text_initials[0].get_size()/20.0));
    text_initials[0].render(ink);
    text_initials[1].set_position(ww2() - (text_initials[1].get_size()/2.0/4.0)+(text_initials[1].get_size()/4.0/2.0), wh2() - (text_initials[1].get_size()/2.0/4.0) - (text_initials[1].get_size()/20.0));
    text_initials[1].render(ink);
    text_initials[2].set_position(ww2() - (text_initials[2].get_size()/2.0/4.0)+(text_initials[2].get_size()/4.0)+(text_initials[2].get_size()/4.0/2.0), wh2() - (text_initials[2].get_size()/2.0/4.0) - (text_initials[2].get_size()/20.0));
    text_initials[2].render(ink);

    text_copyright.set_position(-1, wh2()-(text_copyright.get_charheight()/2));
    text_copyright.render(ink);

    text_intro.set_position(-1.0, text_copyright.get_y() - text_intro.get_charheight());
    text_intro.render(ink);

    if (game->state == STATE_TEST)
      {
        menu->set_positions();
        menu->render(ink);
      }

    if (lives >= 0 || !ink) {

	GC save_gc = gc;
	set_gc(save_gc);
	set_scale(10.0);
	set_theta(0.0);
    }
} // Stats::render


void
Stats::resize(const int nwidth, const int nheight)
{
    Thing::resize(nwidth, nheight);
    text_primary.resize(nwidth, nheight);
    text_secondary.resize(nwidth, nheight);
    text_tertiary.resize(nwidth, nheight);
    text_quaternary.resize(nwidth, nheight);
    text_p1_score.resize(nwidth, nheight);
    text_p1.resize(nwidth, nheight);
    text_p2_score.resize(nwidth, nheight);
    text_p2.resize(nwidth, nheight);
    text_mines_to_go.resize(nwidth, nheight);
    text_lives_left.resize(nwidth, nheight);
    text_energy_bar.resize(nwidth, nheight);
    text_initial_boxes[0].resize(nwidth, nheight);
    text_initial_boxes[1].resize(nwidth, nheight);
    text_initial_boxes[2].resize(nwidth, nheight);
    text_initials[0].resize(nwidth, nheight);
    text_initials[1].resize(nwidth, nheight);
    text_initials[2].resize(nwidth, nheight);
    text_www.resize(nwidth, nheight);
    text_music.resize(nwidth, nheight);
    text_sound.resize(nwidth, nheight);
    text_copyright.resize(nwidth, nheight);
    text_intro.resize(nwidth, nheight);
    text_other.resize(nwidth, nheight);
    menu->resize(nwidth, nheight);
} // Stats::resize
    
void 
Stats::add_score(unsigned int s)
{
  const int extra_life_at = args.extra_life_at;
  if (score %extra_life_at> (score +s)%extra_life_at)
    {
      lives++;
      play(EXTRA_SHIP_AWARDED);
    }
  score += s;
  score_in_wave += s;
} // Stats::add_score

void
Stats::destroyed_a_castle()
{
  castles_destroyed++;
  num_castles_destroyed_in_wave++;
}

void Stats::initial_change_right()
{
  if (current_initial >= 3)
    return;
  char letter = initials[current_initial];
  text_initials[current_initial].erase();
  if (letter == 'Z')
    letter = 'A';
  else
    letter++;
  text_initials[current_initial].draw();
  initials[current_initial] = letter;
}

void Stats::initial_change_left()
{
  if (current_initial >= 3)
    return;
  char letter = initials[current_initial];
  text_initials[current_initial].erase();
  if (letter == 'A')
    letter = 'Z';
  else
    letter--;
  initials[current_initial] = letter;
  text_initials[current_initial].draw();
}

void Stats::accept_initial()
{
  if (current_initial >= 3)
    return;
  //colour the complete initials and boxes.
  current_initial++;
  for (unsigned int i = 0; i < current_initial; i++)
    {
      text_initials[i].set_gc(fetch_gc(GC_BRIGHT_RED));
      text_initial_boxes[i].erase();
      text_initial_boxes[i].set_message(" ");
      text_initial_boxes[i].draw();
      text_initials[i].draw();
    }
  text_initial_boxes[current_initial].set_gc(fetch_gc(GC_BRIGHT_YELLOW));
}

void Stats::clear_various_wave_statistics()
{
  reset_expired_mines();
  reset_shot_mines();
  clear_died_on_challenging_stage();
  clear_lasers_shot();
  clear_polarity_switches();
  clear_castles_destroyed_in_wave();
  clear_score_in_this_wave();
}
    
void 
Stats::next_wave()
{
  if (stages->get_max_stages() == waves)
    {
      double s = Buzzer::buzzer_scale;
      if (s == 33.0)
        s = 22.0;
      else if (s == 22.0)
        s = 17.0;
      else if (s == 17.0)
        s = 14.0;
      Buzzer::buzzer_scale = s;
    }
  waves++;
};
