//
// Created by joe on 24/01/16.
//

#include "xDisplay.h"

#include <cairomm/xlib_surface.h>

#include <X11/Xatom.h>

#include <iostream>

using namespace std;
using namespace osmMapper;
using namespace Cairo;

xDisplay::xDisplay(int &x, int &y) : display(x, y), m_dsp(nullptr), m_da(), m_scr(nullptr), m_vis(nullptr) {
    createX11CairoSurface(&x, &y);
    m_width=x;
    m_height=y;
    cout << "X Display: " << m_width << "x" << m_height << endl;
}

xDisplay::~xDisplay() {
    XCloseDisplay(m_dsp);
}

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
    int screen;

    if ((m_dsp=XOpenDisplay(NULL)) == NULL)
        exit(1);
    screen=DefaultScreen(m_dsp);
    m_scr=DefaultScreenOfDisplay(m_dsp);
    m_vis=DefaultVisual(m_dsp, screen);
    if (!*x || !*y) {
        *x=WidthOfScreen(m_scr), *y=HeightOfScreen(m_scr);
        m_da=XCreateSimpleWindow(m_dsp, DefaultRootWindow(m_dsp), 0, 0, *x, *y, 0, 0, 0);
        fullscreen(m_dsp, m_da);
    }
    else
        m_da=XCreateSimpleWindow(m_dsp, DefaultRootWindow(m_dsp), 0, 0, *x, *y, 0, 0, 0);
    XSelectInput(m_dsp, m_da, ButtonPressMask | KeyPressMask);
    XMapWindow(m_dsp, m_da);
    m_surface= XlibSurface::create(m_dsp, m_da, m_vis, *x, *y);
    m_context= Context::create(m_surface);
}

void xDisplay::fullSize(bool full) {

}

int xDisplay::width(void) const {
    return m_width;
}

int xDisplay::height(void) const {
    return m_height;
}

void xDisplay::startDrawing(void) {
    m_context->push_group();
    m_context->set_source_rgb(m_background.r, m_background.g, m_background.b);
    m_context->paint();
}

void xDisplay::finishDrawing(void) {
    m_context->pop_group_to_source();
    m_context->paint();
    m_surface->flush();
}

void xDisplay::drawLine(const screenPoint &from, const screenPoint &to){
    // TODO: Expose these in a better way, this is just some intial hard-coded defaults
    m_context->set_line_width(2.0);
    m_context->set_source_rgb(1.0,1.0,1.0);

    cout << "From " << from.x() << "x" << from.y() << endl;
    cout << "To " << to.x() << "x" << to.y() << endl;

    // Draw it
    m_context->move_to(from.x(), from.y());
    m_context->line_to(to.x(), to.y());
}
