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

#include "global.h"

#include "font.h"
#include "util.h"
#include "xsc.h"

#include "text.h"


Text::Text(void)
{
    //fprintf(stderr, "Text::Text()\n");
    set_scale(6.0);
    set_gc(fetch_gc(GC_DULL_BLUE));
    message = NULL;
} // Text::Text

    
Text::Text(const Text &o)
        :message(strdup(o.message)), length(o.length)
{
}

Text::~Text(void)
{
        free (message);
    //fprintf(stderr, "Text::~Text()\n");
} // Text::~Text


double
Text::get_charwidth()
{
  double spacing = 0.02;
  return size * ((0.09*2)+0.01+spacing);  // -0.09 to +0.09 widths in cinematronics.C
} // Text::get_charwidth()

void
Text::set_position(const double xpos, const double ypos)
{
    if (xpos == -1) {
	double width = calculate_width();
        //center it
	set_x(ww2() + (get_charwidth() / 2.0) - (width / 2.0));
    } else {
	set_x(xpos);
    }

    set_y(ypos);
} // Text::set_position

double Text::calculate_width()
{
  if (!message)
    return 0.0;
  double width = 0;
  for (int i = 0; i < length; i++) {
    if ((message[i] >= '0' && message[i] <= '9') ||
        (message[i] >= 'A' && message[i] <= 'Z') ||
        (message[i] >= 'a' && message[i] <= 'z') ||
        message[i] == ' ' || message[i] == '>' || message[i] == '<' ||
        message[i] == '/') {
      width += get_charwidth();
    } else if (message[i] == '|') {
      width += get_charwidth() / 3.0;
    } else {
      continue;
    }
  }
  return width;
} // Text::calculate_width

void
Text::render(const bool ink)
{
  if (!message)
    return;
    const double savex = get_x();
    double width;
    int offset;

    for (int i = 0; i < length; i++) {
	width = get_charwidth();
	if (message[i] >= '0' && message[i] <= '9') {
	    offset = font_digit_offset + message[i] - '0';
	} else if (message[i] >= 'A' && message[i] <= 'Z') {
	    offset = font_upper_offset + message[i] - 'A';
	} else if (message[i] >= 'a' && message[i] <= 'z') {
	    offset = font_lower_offset + message[i] - 'a';
	} else if (message[i] == '|') {
	    offset = font_bar_offset;
	    width /= 3.0;
	} else if (message[i] == '@') {
	    offset = font_copyright_offset;
	} else if (message[i] == '>') {
	    offset = font_copyright_offset+1;
	} else if (message[i] == '<') {
	    offset = font_copyright_offset+2;
	} else if (message[i] == '/') {
	    offset = font_copyright_offset+3;
	} else if (message[i] == ' ') {
            set_x(get_x() + width);
	    continue;
	} else {
	    continue;
	}
	set_points(cinematronics_points[offset], cinematronics_count[offset]);
	set_xpoints();
	paint_points(ink);

        set_x(get_x() + width);
    }

    set_x(savex);
} // Text::render

double
Text::get_charheight()
{
  double spacing = 0.09;
  return size * ((0.12*2)+0.01+spacing);  
}
