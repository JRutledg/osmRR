//
// Created by joe on 17/01/16.
//

#include "display.h"
#include "OSMTypes.h"

#include <cairomm/xlib_surface.h>

#include <X11/Xatom.h>

using namespace osmMapper;
using namespace Cairo;

void xDisplay::fullscreen(Display *dpy, Window win) {
    Atom atoms[2]={XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False), None};
    XChangeProperty(dpy, win, XInternAtom(dpy, "_NET_WM_STATE", False),
                    XA_ATOM, 32, PropModeReplace, (unsigned char *) atoms, 1);
}

/*! Open an X11 window and create a cairo surface base on that window. If x and
 * y are set to 0 the function opens a full screen window and stores to window
 * dimensions to x and y.
 * @param x Pointer to width of window.
 * @param y Pointer to height of window.
 * @return Returns a pointer to a valid Xlib cairo surface. The function does
 * not return on error (exit(3)).
 */
void xDisplay::createX11CairoSurface(int *x, int *y) {
    Display *dsp;
    Drawable da;
    Screen *scr;
    Visual *vis;
    int screen;

    if ((dsp=XOpenDisplay(NULL)) == NULL)
        exit(1);
    screen=DefaultScreen(dsp);
    scr=DefaultScreenOfDisplay(dsp);
    vis=DefaultVisual(dsp, screen);
    if (!*x || !*y) {
        *x=WidthOfScreen(scr), *y=HeightOfScreen(scr);
        da=XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, *x, *y, 0, 0, 0);
        fullscreen(dsp, da);
    }
    else
        da=XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, *x, *y, 0, 0, 0);
    XSelectInput(dsp, da, ButtonPressMask | KeyPressMask);
    XMapWindow(dsp, da);
    m_surface= XlibSurface::create(dsp, da, vis, *x, *y);
    m_context= Context::create(m_surface);
}

/*! Destroy cairo Xlib surface and close X connection.
 */
void xDisplay::closeX11CairoSurface(cairo_surface_t *sfc) {
    Display *dsp=cairo_xlib_surface_get_display(sfc);

    cairo_surface_destroy(sfc);
    XCloseDisplay(dsp);
}

screenPoint xDisplay::compassPointToScreenPoint(const compassPoint &inPoint) {
    return screenPoint(inPoint.lat(), inPoint.lon());
}

void xDisplay::drawLine(const screenPoint &from, const screenPoint &to) {
    m_context->move_to(from.x(), from.y());
    m_context->line_to(to.x(), to.y());
}
