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

#include "game.h"
#include "args.h"
#include "sound.h"

const char *high_score_file = "high-score";

Game::Game()
{
  ship = new Ship;
  king = new King (true);
  king->add_attract_fireballs();
  queen = new King (false);
  castle = new Castle;
  minefield = new Minefield(0, 0); 
  players = new std::list<Player*>();
  empty = new Stats(ship, this);
  active = NULL;
  coins = 0;
  snprintf (high_score_initials, sizeof (high_score_initials), "AAA");
  load_high_scores();
  pilot = new AutoPilot(this);
} // Game::Game


Game::~Game(void)
{
  delete ship;
  delete king;
  delete queen;
  delete castle;
  delete minefield;
  delete empty;
  for (std::list<Player*>::iterator it = players->begin(); it != players->end();
       it++)
    {
      active = *it;
      (*it)->stats->write_stats_record();
    }
  removeAllPlayers();
  delete players;
  delete pilot;
} // Game::~Game

Player * Game::player(unsigned int id)
{
  for (std::list<Player*>::iterator it = players->begin(); it != players->end();
       it++)
    {
      if ((*it)->get_id() == id)
        return (*it);
    }
  return NULL;
}

Player * Game::player()
{
  return active;
}

void Game::nextPlayer()
{
  std::list<Player*>::iterator it;

  if (!active)
    it = players->begin();
  else
    {
      for (it = players->begin(); it != players->end(); ++it)
        {
          if ((*it) == active)
            {
              it++;
              break;
            }
        }
    }

  while ((it == players->end()) || ((*it)->isDead()))
    {
      if (it == players->end())
        {
          it = players->begin();
          continue;
        }
      it++;
    }

  active = *it;
}

void Game::addCoin()
{
  coins++;
  play (COIN_UP);
}

unsigned int Game::get_number_of_players()
{
  return players->size();
}

unsigned int Game::count_number_of_players_alive()
{
  unsigned int count = 0;
  for (std::list<Player*>::iterator it = players->begin(); it != players->end();
       it++)
    {
      if ((*it)->isAlive())
        count++;
    }
  return count;
}

bool Game::canStart(unsigned int num_players)
{
  if (coins / args.coins_per_credit >= num_players)
    {
      switch (state)
        {
        case STATE_OVER:
        case STATE_HIGH_SCORES:
        case STATE_ATTRACT:
        case STATE_PRESS_START:
          return true;
          break;
        default:
          break;
        }
    }
  else if (args.free_play)
    {
      switch (state)
        {
        case STATE_HIGH_SCORES:
        case STATE_ATTRACT:
        case STATE_PRESS_START:
          return true;
          break;
        default:
          break;
        }
    }
  return false;
}

void Game::start(unsigned int num_players)
{
  if (canStart(num_players) == false)
    return;
  play (GAME_START);
  removeAllPlayers();
  if (num_players <= coins / args.coins_per_credit * num_players)
    coins -= args.coins_per_credit * num_players;
  else
    coins = 0; //we're on freeplay
  createPlayers(num_players);
  active = *players->begin();
  for (unsigned int i = 0; i < num_players; i++)
    {
      active->stats->clear_stats_file();
      active->stats->write_stats_record();
      nextPlayer();
    }
}

void Game::createPlayers(unsigned int num_players)
{
  for (unsigned int i = 0; i < num_players; i++)
    {
      Player *p = new Player(this, i+1);
      players->push_back(p);
    }
}

std::list<Player*>::iterator Game::playerErase(std::list<Player*>::iterator it)
{
  delete (*it);
  return players->erase (it);
}

void Game::removeAllPlayers()
{
  std::list<Player*>::iterator it = players->begin();
  while (!players->empty())
    it = playerErase(it);

}
    
void Game::reset_high_scores()
{
  char *file = Args::get_config_file(high_score_file);
  remove (file);
  free (file);
  load_high_scores();
}

void Game::load_high_scores()
{
  char *file = Args::get_config_file(high_score_file);
  if (file)
    {
      char *line = NULL;
      size_t n;
      FILE *fp = fopen (file, "r");
      if (fp)
        {
          if (getline (&line, &n, fp) != -1)
            sscanf (line, "%u %c%c%c", &high_score, &high_score_initials[0], &high_score_initials[1], &high_score_initials[2]);
          free (line);
          fclose (fp);
        }
      else
        {
          high_score = 10210;
          strcpy (high_score_initials, "MTA");
        }
      free (file);
    }
}

void Game::save_high_scores()
{
  char *file = Args::get_config_file (high_score_file);
  if (file)
    {
      FILE *fp = fopen (file, "w");
      if (fp)
        {
          fprintf (fp, "%u %c%c%c\n", high_score, high_score_initials[0], high_score_initials[1], high_score_initials[2]);
          fclose (fp);
        }
      free (file);
    }
}

Stats *Game::stats()
{
  if (p())
    return p()->stats;
  return empty;
}
    
unsigned int Game::get_expired_mines()
{
  if (!p())
    return 0;
return stats()->get_expired_mines();
}

char *Game::get_credits()
{
  char buf[1024];
  float remainder = fmod ((double)coins, (double)args.coins_per_credit);
  if (remainder > 0)
    {
      if (coins/args.coins_per_credit > 0)
        snprintf (buf, sizeof (buf), "%d %d / %d", coins/args.coins_per_credit, (int)remainder, args.coins_per_credit);
      else
        snprintf (buf, sizeof (buf), "%d / %d", (int)remainder, args.coins_per_credit);
    }
  else
    snprintf (buf, sizeof (buf), "%d", coins/args.coins_per_credit);
  return strdup (buf);
}

void 
Game::cancel_game_and_go_into_test_mode()
{
  bool old_quiet = args.quiet;
  args.quiet = true;
  game->stats()->erase();
  game->minefield->erase();
  game->king->erase();
  game->queen->erase();
  game->king->reset();
  game->king->hit(NULL, NULL, NULL, NULL);
  game->queen->hit(NULL, NULL, NULL, NULL);
  game->minefield->snuff_visible();
  game->minefield->reset(0, 0, 0);
  if (game->castle->alive())
    game->castle->explode(NULL);
  game->ship->erase();
  game->ship->hit(game->ship);
  game->ship->hit(game->ship);
  game->ship->erase();
  removeAllPlayers();
  active = NULL;
  args.quiet = old_quiet;
  game->ship->thrust(KEY_UP);
}
