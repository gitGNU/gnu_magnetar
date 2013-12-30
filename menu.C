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

#include "util.h"
#include "xsc.h"

#include "menu.h"

Menu::Menu(double pos_x, double pos_y, GC sel, GC unsel, double s)
 :x(pos_x), y(pos_y), selected_color(sel), unselected_color(unsel), scale(s)
{
  active = begin();
  selected_submenu = end();
} // Menu::Menu


Menu::~Menu(void)
{
} // Menu::~Menu

void
Menu::set_positions()
{
  double heights = 0;

  for (iterator i = begin(); i != end(); i++)
    {
      (*i).first.set_x(x);
      (*i).first.set_y(int(heights)+(wh()*.10));
      (*i).first.set_scale(scale);
      heights += (*i).first.get_charheight();
    }
  for (iterator i = begin(); i != end(); i++)
    {
      if ((*i).second)
        {
          Menu *menu = (*i).second;
          menu->set_positions();
          for (iterator j = menu->begin(); j != menu->end(); j++)
            (*j).first.set_x(ww2());
        }
    }
}

void
Menu::update()
{
  for (iterator i = begin(); i != end(); i++)
    {
      if (i == active)
        {
          (*i).first.set_gc(selected_color);
            {
          if ((*i).second)
            (*i).second->update();
            }
        }
      else
        {
          (*i).first.set_gc(unselected_color);
          if ((*i).second)
            (*i).second->erase();
        }
    }
}

void
Menu::render(const bool ink)
{
  for (iterator i = begin(); i != end(); i++)
    {
      (*i).first.render(ink);
    if ((*i).second)
      {
        if (active!= i && ink == true)
      ;//(*i).second->render(false);
        else
      (*i).second->render(ink);
      }
    }
} // Menu::render

void
Menu::resize(const int nwidth, const int nheight)
{
  for (iterator i = begin(); i != end(); i++)
    {
      (*i).first.resize(nwidth, nheight);
      if ((*i).second)
        (*i).second->resize(nwidth, nheight);
    }
} // Menu::resize
    
void Menu::up()
{
  if (active == begin())
    {
      active = end();
      active--;
    }
  else
    active--;
  update();
}

void Menu::down()
{
  active++;
  if (active== end())
    active = begin();
  update();
}
    
void Menu::add_entry(Text t, Menu *submenu)
{
  push_back(std::pair<Text, Menu*>(t, submenu));
}

void Menu::unselect()
{
  if ((*active).second)
    {
    selected_submenu = end();
    (*active).second->set_selected_color(fetch_gc(GC_DULL_YELLOW));
    (*active).second->set_unselected_color(fetch_gc(GC_DULL_BLUE));
    (*active).second->update();
    }

}

void Menu::select()
{
  //okay here we select a submenu if there is one
  if ((*active).second)
    {
    selected_submenu = active;
    (*active).second->set_selected_color(fetch_gc(GC_DULL_RED));
    (*active).second->set_unselected_color(fetch_gc(GC_DULL_BLUE));
    //(*active).second->set_active((*active).second->begin()++);
    (*active).second->update();
    }

}

std::list<std::string> Menu::get_selections()
{
  std::list<std::string> selections;
  for (iterator i = begin(); i != end(); i++)
    {
      if ((*i).second)
        {
          std::string s;
          s = (*(*i).second->get_active()).first.get_message();
          selections.push_back(s);
        }
    }
  return selections;
}

unsigned int Menu::get_active_num()
{
  int count = 0;
  for (iterator i = begin(); i != end(); i++)
    {
      if (i == active)
        return count;
      count++;
    }
  return count;
}

bool Menu::is_active(char *title)
{
  char *message = (*get_active()).first.get_message();
  if (strcmp(message, title) == 0)
    return true;
  return false;
}

void Menu::delete_submenus()
{
  for (iterator i = begin(); i != end(); i++)
    {
      if ((*i).second)
        delete (*i).second;
    }
}
