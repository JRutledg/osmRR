//
// Created by joe on 17/01/16.
//
// Display related classes
// Intended to abstract the display type (X, Windows, PDF, etc.) from the rest of the rendering

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

    /*! Base for all rendered output*/
    class display {
    public:
        //! CTOR
        //! \param x Initial width of display in display units
        //! \param y Initial height of display in display units
        display(int &x, int &y) : m_width(x), m_height(y) {}

        //! DTOR
        virtual ~display(){}

        //! Sets the display to fullsize/fullscreen
        virtual void fullSize(bool full= true){}

        //! Gets the current width of the display in display units
        virtual int width(void) const {return m_width;}

        //! Gets the current height of the display in display units
        virtual int height(void) const {return m_height;}

        //! Renders a line on the display
        //! \param from The position to draw line from
        //! \param to The position to draw the line to
        virtual void drawLine(const screenPoint &from, const screenPoint &to){};

    protected:
        int m_width;    //!< Width of display in display units
        int m_height;   //!< Height of display in display units
    };

    /*! A display that operates via XLib */
    class xDisplay : public display {
    public:
        xDisplay(int &x, int &y) : display(x, y) {createX11CairoSurface(&x, &y);}
        ~xDisplay(){}

        void fullSize(bool full= true){}

        int width(void) const {return m_width;}

        int height(void) const {return m_height;}

        void drawLine(const screenPoint &from, const screenPoint &to);

    private:
        //! Creates an X11 Cairo surface, of x and y in size or fullscreen if x and y are 0
        //! \param[in,out] x The initial width of the surface, returns the actual width of the surface
        //! \param[in,out] y The intial height of the surface, returns the actual height of the surface
        void createX11CairoSurface(int *x, int *y);

        //! Frees up an X11 Cairo surface
        void closeX11CairoSurface(cairo_surface_t *sfc);

        void fullscreen(Display *dpy, Window win);

        //! Temporary. This needs to move somewhere else.
        screenPoint compassPointToScreenPoint(const compassPoint &inPoint);

        Cairo::RefPtr<Cairo::XlibSurface> m_surface;    //!< CairoMM reference to the surface
        Cairo::RefPtr<Cairo::Context> m_context;        //!< CairoMM reference to the draw context
    };

} // namespace osmMapper

#endif //OSMMAPPER_DISPLAY_H
