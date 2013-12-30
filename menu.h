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

#ifdef	MAGNETAR_MENU_H

class Menu;

#else	// MAGNETAR_MENU_H
#define	MAGNETAR_MENU_H

#include "text.h"
#include <list>
#include <string>

class Menu: public std::list<std::pair<Text, Menu *> > {
private:

    double x;
    double y;
    GC selected_color;
    GC unselected_color;
    double scale;
    iterator active;
    iterator selected_submenu;
public:
    Menu(double x, double y, GC selected_color, GC unselected_color, double scale);
    ~Menu(void);

    bool is_submenu_selected() {return selected_submenu != end();};
    Menu *get_selected_submenu() {return (*selected_submenu).second;};
    unsigned int get_active_num();
    void set_active(iterator i) {active = i;};
    void set_selected_color(GC c) {selected_color = c;};
    void set_unselected_color(GC c){unselected_color = c;};
    void update();
    void set_positions();
    void select();
    void unselect();
    iterator get_active(){return active;};
    bool is_active (char *title);
    void render(const bool);
    void draw();
    void erase();
    void resize(const int, const int);
    void up();
    void down();
    void add_entry(Text t, Menu *submenu = NULL);
    std::list<std::string> get_selections();
    void delete_submenus();
};

inline void
Menu::draw()
{
  for (iterator i = begin(); i != end(); i++)
    {
    (*i).first.render(true);
    if ((*i).second)
      (*i).second->render(true);
    }
} // Menu::draw


inline void
Menu::erase()
{
  for (iterator i = begin(); i != end(); i++)
    {
    (*i).first.render(false);
    if ((*i).second)
      (*i).second->render(false);
    }
} // Menu::erase

#endif	// MAGNETAR_MENU_H
