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

#ifdef	MAGNETAR_STATS_H

class Stats;

#else	// MAGNETAR_STATS_H
#define	MAGNETAR_STATS_H

#include "tthing.h"
#include "text.h"
#include "menu.h"
class Game;


class Ship;

class Stats : public Tthing {
public:

    unsigned int last_score;
    bool polarity;
    Ship *ship;
    Game *game;
    Menu *menu;

    Text text_copyright;
    Text text_music;
    Text text_sound;
    Text text_www;
    Text text_primary;
    Text text_secondary;
    Text text_tertiary;
    Text text_quaternary;
    Text text_p1;
    Text text_p2;
    Text text_p1_score;
    Text text_p2_score;
    Text text_mines_to_go;
    Text text_lives_left;
    Text text_energy_bar;
    Text text_initials[3];
    Text text_initial_boxes[3];
    char initials[4];
    unsigned int current_initial;
    unsigned int score;
    unsigned int score_in_wave;
    unsigned int buzzers_destroyed;
    unsigned int ring_segments_destroyed;
    unsigned int ring_segments_hit;
    unsigned int castles_destroyed;
    unsigned int lasers_shot;
    unsigned int fireballs_launched;
    unsigned int rings_regenerated;
    unsigned int killed_by_buzzer;
    unsigned int killed_by_fireball;
    unsigned int buzzers_launched;


    unsigned int bonus_points;  //for calculation
    bool died_on_challenging_stage;

    unsigned int mines_expired;
    unsigned int mines_passed;
    unsigned int mines_shot;
    unsigned int rewards_received;
    unsigned int total_mines_in_wave;
    unsigned int waves;

    unsigned int num_polarity_switches_in_wave;
    unsigned int num_castles_destroyed_in_wave;
    int seconds_left;
    char * get_file();
    void fill_energy_bar(char *buf, size_t buflen, double filled);
    void test_menu();
    void finish_test_mode();
    int calculate_max_energy_ticks();
public:
    Stats(Ship *s, Game *g);
    ~Stats(void);

    void add_expired_mines (unsigned int num, bool passed);
    unsigned int get_expired_mines();
    void reset_expired_mines();
    void shot_mine() {mines_shot++;};
    void reset_shot_mines() {mines_shot = 0;};
    unsigned int get_mines_shot(){return mines_shot;};
    unsigned int get_rewards_received();
    void add_received_reward();
    void set_total_mines(unsigned int total) {total_mines_in_wave = total;};
    void next_wave();
    int get_stage()const {return waves;};
    void set_died_on_challenging_stage(){died_on_challenging_stage = true;};
    void clear_died_on_challenging_stage(){died_on_challenging_stage = false;};
    void switched_polarity() {num_polarity_switches_in_wave++;};
    void clear_polarity_switches() {num_polarity_switches_in_wave=0;};
    void clear_castles_destroyed_in_wave() {num_castles_destroyed_in_wave=0;};
    void clear_score_in_this_wave() {score_in_wave =0;};
    void clear_lasers_shot(){lasers_shot=0;};
    void clear_stats_file();
    void write_stats_record();

    void add_score(unsigned int);
    int lives;
    int last_lives;
    unsigned int level; //the number of times we've killed the king.

    void render(const bool);

    void reset(void);
    void resize(const int, const int);

    void destroyed_a_buzzer();
    void destroyed_a_ring_segment();
    void destroyed_a_castle();
    void hit_a_ring_segment();
    void shot_a_laser();
    void launched_a_fireball();
    void regenerated_a_ring();
    void killed_by_a_buzzer();
    void killed_by_a_fireball();
    void launched_a_buzzer();
    void initial_change_left();
    void initial_change_right();
    void accept_initial();
    bool completed_entering_initials() { return current_initial >= 3 ? true : false;};
    char *get_initials(){return initials;};
    void set_seconds_left_to_enter_initials(int secs){seconds_left = secs;};
    void clear_various_wave_statistics();
};

inline unsigned int
Stats::get_expired_mines() {return mines_expired;};

inline void
Stats::add_expired_mines(unsigned int num, bool passed)
{
  mines_expired += num;
  if (passed)
    mines_passed += num;
}
inline void
Stats::reset_expired_mines()
{
  mines_expired = 0;
  mines_passed = 0;
}
inline unsigned int
Stats::get_rewards_received() {return rewards_received;};

inline void
Stats::add_received_reward()
{
  rewards_received += 1;
}
inline void
Stats::killed_by_a_buzzer()
{
  killed_by_buzzer++;
}

inline void
Stats::killed_by_a_fireball()
{
  killed_by_fireball++;
}

inline void
Stats::launched_a_buzzer()
{
  buzzers_launched++;
}

inline void
Stats::regenerated_a_ring()
{
  rings_regenerated++;
}

inline void
Stats::launched_a_fireball()
{
  fireballs_launched++;
}

inline void
Stats::shot_a_laser()
{
  lasers_shot++;
}


inline void
Stats::destroyed_a_buzzer()
{
  buzzers_destroyed++;
}

inline void
Stats::hit_a_ring_segment()
{
  ring_segments_hit++;
}

inline void
Stats::destroyed_a_ring_segment()
{
  ring_segments_destroyed++;
}


#endif	// MAGNETAR_STATS_H
