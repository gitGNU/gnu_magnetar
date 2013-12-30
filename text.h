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

#ifdef	MAGNETAR_TEXT_H

class Text;

#else	// MAGNETAR_TEXT_H
#define	MAGNETAR_TEXT_H

#include "thing.h"

class Text : public Thing {
private:
    char *message;
    int length;


public:
    Text(void);
    Text(const Text &o);
    ~Text(void);

    void render(const bool);
    double get_charwidth();

    void set_position(const double, const double);
    double calculate_width();
    double get_charheight();

    void set_message(const char *);
    void set_message(const char);
    char *get_message() {return message;};
};


inline void
Text::set_message (const char letter)
{
  if (message)
     free ((char *)message);
  char buf[2];
  snprintf (buf, sizeof (buf), "%c", letter);
  message = strdup (buf);
  length = 1;
}

inline void
Text::set_message(const char *msg)
{
  if (message)
     free ((char *)message);
    message = strdup (msg);
    length = message ? strlen(message) : 0;
} // Text::set_message

#endif	// MAGNETAR_TEXT_H
