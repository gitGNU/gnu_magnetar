//      MAGNETAR
//      Copyright (c) 1993-2005 by Mark B. Hanson (mbh@panix.com).
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

#ifdef	MAGNETAR_ETHING_H

class Ething;

#else	//MAGNETAR_ETHING_H
#define	MAGNETAR_ETHING_H

#include "tthing.h"
#include "xything.h"

class Ething : public Xything, public Tthing {
public:
    Ething(void);
    ~Ething(void);
};

#endif	//MAGNETAR_ETHING_H
