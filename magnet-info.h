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

#ifdef	MAGNETAR_MAGNET_INFO_H

class MagnetInfo;

#else	// MAGNETAR_MAGNET_INFO_H
#define	MAGNETAR_MAGNET_INFO_H

class MagnetInfo {
private:
    float strength;
    float length;
    float speed; //lower is faster
    bool bouncy;
    double switchy;
    float starting_height;
    float starting_width;
    int dir;

public:
    MagnetInfo(float s, float l, float sp, int d, bool bouncy, double switchy, float sh, float sw);
    MagnetInfo(float s, float l, float sp, int d, bool bouncy, double switchy, float sh);
    ~MagnetInfo(void);
    double get_strength() const {return strength;};
    double get_length() const {return length;};
    double get_speed() const {return speed;};
    int get_dir() const {return dir;};
    float get_starting_height() const {return starting_height;};
    float get_starting_width() const {return starting_width;};
    void set_starting_width(float w) {starting_width = w;};
    bool get_bouncy() const {return bouncy;};
    void set_bouncy(bool b) {bouncy=b;};
    double get_switchy() const {return switchy;};
    void set_switchy(double sw) {switchy=sw;};
};

#endif	// MAGNETAR_MAGNET_INFO_H
