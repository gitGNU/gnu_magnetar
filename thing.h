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

#ifdef	MAGNETAR_THING_H

class Thing;

#else	// MAGNETAR_THING_H
#define	MAGNETAR_THING_H

class Thing {
private:
    double x;
    double y;
protected:
    double size;
    double scale;
    double diag;

    Window window;
    GC gc;

    int num_points;
    const struct coords *points;
    XPoint *xpoints;

public:
    Thing(void);
    virtual ~Thing(void);

    double get_size(void) const;
    void set_size(const double);

    double get_scale(void) const;
    void set_scale(const double);

    Window get_window(void) const;
    void set_window(const Window);

    GC get_gc(void) const;
    void set_gc(const GC);

    double get_x(void) const;
    void set_x(double);

    double get_y(void) const;
    void set_y(double);

    double get_diag(void) const;

    virtual void set_points(const struct coords *, const int);

    virtual XPoint *get_xpoints(void) const;
    virtual void set_xpoints(void);

    virtual void paint_points(const bool) const;

    virtual void render(const bool);
    void draw(void);
    void erase(void);

    virtual void resize(const int, const int);
};


inline double
Thing::get_size(void) const
{
    return size;
} // Thing::get_size


inline double
Thing::get_scale(void) const
{
    return scale;
} // Thing::get_scale


inline Window
Thing::get_window(void) const
{
    return window;
} // Thing::get_window


inline void
Thing::set_window(const Window w)
{
    window = w;
} // Thing::set_window


inline GC
Thing::get_gc(void) const
{
    return gc;
} // Thing::get_gc


inline void
Thing::set_gc(const GC ngc)
{
    gc = ngc;
} // Thing::set_gc


inline double
Thing::get_diag(void) const
{
    return diag;
} // Thing::get_diag


inline void
Thing::draw(void)
{
    render(true);
} // Thing::draw


inline void
Thing::erase(void)
{
    render(false);
} // Thing::erase

#endif	// MAGNETAR_THING_H
