## Process this file with automake to produce Makefile.in
#      MAGNETAR
#      Copyright (c) 1993-2002 by Mark B. Hanson (mbh@panix.com).
#      Copyright (C) 2013 Ben Asselstine
# 
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; version 2 of the License, or
#   (at your option) any later version.
# 
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
# 
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
#   02110-1301, USA.

SUBDIRS=sounds

man1_MANS = magnetar.1
# for development
#AM_CXXFLAGS = -g -DDEBUG -Wall -W -Wshadow -Wcast-qual -Wcast-align \
#-Wconversion -Winline -Werror
#AM_CXXFLAGS = -std=c++11
AM_CXXFLAGS=${SDL_CFLAGS} -Wall -DMAGNETAR_DATADIR='"$(MAGNETAR_DATADIR)"' -Wno-write-strings

bin_PROGRAMS = magnetar
magnetar_SOURCES = \
	args.C args.h \
	buzzer.C buzzer.h \
	castle.C castle.h \
	difficulty.C difficulty.h \
	ething.C ething.h \
	fireball.C fireball.h \
	kapow.C kapow.h \
	king.C king.h \
	laser.C laser.h \
	minefield.C minefield.h \
	ring.C ring.h \
	cinematronics.C \
	ship.C ship.h \
	spikes.C spikes.h \
	stamp.C stamp.h \
	starfield.C starfield.h \
	stats.C stats.h \
	text.C text.h \
	thing.C thing.h \
	trig.C trig.h \
	tthing.C tthing.h \
	util.C util.h \
	xsc.C xsc.h \
	xything.C xything.h \
	sound.C sound.h \
	stages.C stages.h \
	stage.C stage.h \
	magnet-info.C magnet-info.h \
	challenging-stage.C challenging-stage.h \
	player.C player.h \
	game.C game.h \
	font.h \
	global.h \
	timing.h \
	random.h \
	menu.C menu.h \
	auto-pilot.C auto-pilot.h

magnetar_LDADD=${SDL_LIBS}

EXTRA_DIST = \
	icon.xpm \
	magnetar.1 \
	HACKING \
	README
