//
// Created by joe on 17/01/16.
//

#ifndef OSMMAPPER_DISPLAY_H
#define OSMMAPPER_DISPLAY_H

#include "OSMTypes.h"

#include <cairommconfig.h>
#include <context.h>
#include <surface.h>
#include <xlib_surface.h>
#include <refptr.h>

#ifndef CAIRO_HAS_XLIB_SURFACE
#error Cairo built without support for X
#endif

namespace osmMapper {

    class display {
    public:
        display(int &x, int &y) : m_width(x), m_height(y) {}

        virtual ~display(){}

        virtual void fullSize(bool full= true){}

        virtual void closeX11CairoSurface(cairo_surface_t *sfc){}

        virtual int width(void) const {return m_width;}

        virtual int height(void) const {return m_height;}

        virtual void drawLine(const screenPoint &from, const screenPoint &to){};

    protected:
        int m_width;
        int m_height;
    };

    class xDisplay : public display {
    public:
        xDisplay(int &x, int &y) : display(x, y) {createX11CairoSurface(&x, &y);}
        ~xDisplay(){}

        void fullSize(bool full= true){}

        void closeX11CairoSurface(cairo_surface_t *sfc);

        int width(void) const {return m_width;}

        int height(void) const {return m_height;}

        void drawLine(const screenPoint &from, const screenPoint &to);

    private:
        void createX11CairoSurface(int *x, int *y);
        void fullscreen(Display *dpy, Window win);
        screenPoint compassPointToScreenPoint(const compassPoint &inPoint);
        Cairo::RefPtr<Cairo::XlibSurface> m_surface;
        Cairo::RefPtr<Cairo::Context> m_context;
    };

} // namespace osmMapper

#endif //OSMMAPPER_DISPLAY_H
