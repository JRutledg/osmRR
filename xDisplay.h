//
// Created by joe on 24/01/16.
//

#ifndef OSMMAPPER_XDISPLAY_H
#define OSMMAPPER_XDISPLAY_H

#include "display.h"

#include <xlib_surface.h>

#ifndef CAIRO_HAS_XLIB_SURFACE
#error Cairo built without support for X
#endif

namespace osmMapper {

/*! A display that operates via XLib */
    class xDisplay : public display {
    public:
        xDisplay(int &x, int &y);

        ~xDisplay();

        void fullSize(bool full=true);

        int width(void) const;

        int height(void) const;

        void startDrawing(void);

        void finishDrawing(void);

        void startPath(const screenPoint &startPos, bool filled= false);

        void endPath(void);

        void drawLine(const screenPoint &from, const screenPoint &to);

    private:
        //! Creates an X11 Cairo surface, of x and y in size or fullscreen if x and y are 0
        //! \param[in,out] x The initial width of the surface, returns the actual width of the surface
        //! \param[in,out] y The intial height of the surface, returns the actual height of the surface
        void createX11CairoSurface(int *x, int *y);

        void fullscreen(Display *dpy, Window win);

        Cairo::RefPtr<Cairo::XlibSurface> m_surface;    //!< CairoMM reference to the surface
        Cairo::RefPtr<Cairo::Context> m_context;        //!< CairoMM reference to the draw context

        //! Nasty X bits
        Display *m_dsp;
        Drawable m_da;
        Screen *m_scr;
        Visual *m_vis;
    };
}

#endif //OSMMAPPER_XDISPLAY_H
