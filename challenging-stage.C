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

#include "xsc.h"
#include "challenging-stage.h"
#include "king.h"

ChallengingStage::ChallengingStage(int num, int mines, double mspeed, int minesatatime, double avglaunchtime)
  :Stage(num, mines, mspeed, minesatatime, avglaunchtime, 0)
{
  challenging_stage = true;
} // ChallengingStage::ChallengingStage

    
ChallengingStage::~ChallengingStage(void)
{
} // ChallengingStage::~ChallengingStage

