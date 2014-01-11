//      MAGNETAR
//      Copyright (C) 2013, 2014 Ben Asselstine
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
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

#include "player.h"
#include "ship.h"
#include "king.h"
#include "castle.h"
#include "minefield.h"
#include "auto-pilot.h"
#include <list>

#ifdef	MAGNETAR_GAME_H

class Game;

#else	// MAGNETAR_GAME_H
#define	MAGNETAR_GAME_H

enum stats_states {
    STATE_NORMAL,
    STATE_GET_READY,
    STATE_WAVE_COMPLETE,
    STATE_ADD_PLAYER_BONUSES,
    STATE_SHOW_PLAYER_SCORE,
    STATE_PAUSED,
    STATE_OVER,
    STATE_HIGH_SCORES,
    STATE_ATTRACT,
    STATE_INITIALS,
    STATE_PRESS_START,
    STATE_TEST,
    STATE_COPYRIGHT,
    STATE_PATTERN,
};

class Game {
private:
    std::list<Player*> *players;
    unsigned int coins;
    Player *active;
    Stats *empty;
    bool reset_flag;
    bool reset_soon_flag;

protected:
    void removeAllPlayers();
    std::list<Player*>::iterator playerErase(std::list<Player*>::iterator it);
    void createPlayers(unsigned int num_players);
public:
    Game(void);
    ~Game(void);
    Player * player();
    Player * player(unsigned int id);
    Player * p() {return player();};
    Player * p(unsigned int id) {return player(id);};
    void nextPlayer();
    void addCoin();
    bool canStart(unsigned int num_players);
    void start(unsigned int num_players);
    enum stats_states state;
    unsigned int high_score;
    char high_score_initials[4];
    enum stats_states last_state;
    bool changed(void);
    unsigned int get_number_of_players();
    unsigned int count_number_of_players_alive();
    Ship		*ship;
    King		*king;
    King		*queen;
    Castle		*castle;
    Minefield	        *minefield;
    void load_high_scores();
    void reset_high_scores();
    void save_high_scores();
    Stats *stats();
    AutoPilot *pilot;
    unsigned int get_expired_mines();
    unsigned int get_coins() {return coins;};
    char * get_credits();
    bool is_fire_key (KeySym ks) {return p() ? p()->is_fire_key(ks) : false;};
    bool is_thrust_key (KeySym ks) {return p() ? p()->is_thrust_key(ks) : false;};
    bool is_cw_key (KeySym ks) {return p() ? p()->is_cw_key(ks) : false;};
    bool is_ccw_key (KeySym ks) {return p() ? p()->is_ccw_key(ks) : false;};
    void cancel_game_and_go_into_test_mode();
    void reset() {reset_flag = true;};
    bool get_reset_flag() {return reset_flag;};
    void reset_soon() {reset_soon_flag = true;};
    bool get_reset_soon_flag() {return reset_soon_flag;};
};

inline bool
Game::changed(void)
{
  if (state != last_state)
    return true;
  if (stats()->lives != stats()->last_lives)
    return true;
  if (stats()->score != stats()->last_score)
    return true;
  return false;
} // Game::changed

#endif	// MAGNETAR_GAME_H
