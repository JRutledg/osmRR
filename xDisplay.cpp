//
// Created by joe on 24/01/16.
//

#include "xDisplay.h"

#include <X11/Xatom.h>

#include <boost/date_time/posix_time/posix_time.hpp>

using namespace osmMapper;
using namespace Cairo;
using namespace boost::interprocess;
using namespace boost::posix_time;
using namespace std;

xDisplay::xDisplay(int &x, int &y) : display(x, y), m_dsp(nullptr), m_da(), m_scr(nullptr), m_vis(nullptr), m_mq
        (nullptr), m_eventThread(), m_stopping(false) {
    createX11CairoSurface(&x, &y);
    m_width=x;
    m_height=y;
    cout << "X Display: " << m_width << "x" << m_height << endl;
    try {
        message_queue::remove("eventsQueue"); // Just in case
        //Create a message_queue.
        m_mq=make_shared<message_queue>(open_or_create, "eventsQueue", 100, sizeof(event));
    }
    catch (interprocess_exception &ex) {
        cout << "Fatal: couldn't construct with message queue" << endl;
        cout << ex.what() << endl;
        abort();
    }
    m_eventThread=std::thread(&xDisplay::pollEvents, this);
}

xDisplay::~xDisplay() {
    m_stopping=true;
    m_eventThread.join();
    message_queue::remove("eventsQueue"); // This can fail
    XCloseDisplay(m_dsp);
}

void xDisplay::pollEvents(void) {
    char keybuf[8];
    KeySym key;
    XEvent e;
    event newEvent {event::NONE, 0};
    int priority=0;

    XNextEvent(m_dsp, &e);
    while (!m_stopping) {
        switch (e.type) {
            case KeyPress:
                cout << "Got key press!" << endl;
                XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
                newEvent.type=event::KEY;
                newEvent.value=key;
                m_mq->send(&newEvent, sizeof(newEvent), priority);
                break;
            case ButtonPress:
                cout << "Got button press!" << endl;
                newEvent.type=event::BUTTON;
                newEvent.value=0;
                m_mq->send(&newEvent, sizeof(newEvent), priority);
                break;
            default:
                fprintf(stderr, "Dropping unhandled XEvent.type = %d.\n", e.type);
                m_mq->send(&newEvent, sizeof(newEvent), priority);
                break;
        }
        XNextEvent(m_dsp, &e);
    }
}

event xDisplay::getEvent(const int timeoutInMs) {
    event newEvent {event::NONE, 0};
    unsigned int priority=0;
    unsigned long receivedBytes;

    if (timeoutInMs == 0) {
        m_mq->receive(&newEvent, sizeof(newEvent), receivedBytes, priority);
        if (sizeof(newEvent) != receivedBytes)
            newEvent.type=event::NONE;
    }
    else {
        ptime timeoutTime=microsec_clock::universal_time() + milliseconds(timeoutInMs);
        m_mq->timed_receive(&newEvent, sizeof(newEvent), receivedBytes, priority, timeoutTime);
        if (sizeof(newEvent) != receivedBytes)
            newEvent.type=event::NONE;
    }
    return newEvent;
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

    XInitThreads();
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
    m_surface=XlibSurface::create(m_dsp, m_da, m_vis, *x, *y);
    m_context=Context::create(m_surface);
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
    m_context->set_antialias(ANTIALIAS_GRAY);
    m_context->push_group();
    m_context->set_source_rgb(m_background.r, m_background.g, m_background.b);
    m_context->paint();
}

void xDisplay::finishDrawing(void) {
    m_context->pop_group_to_source();
    m_context->paint();
    m_surface->flush();
    XSync(m_dsp, false); // Yuk. I think m_surface->flush is supposed to do this but I found this needed...
}

void xDisplay::startPath(const screenPoint &startPos, const colour_t &colour, const double thickness, bool filled) {
    m_context->set_line_width(thickness);
    m_context->set_source_rgb(colour.r, colour.g, colour.b);
    m_context->set_line_cap(LINE_CAP_ROUND);
    m_context->set_line_join(LINE_JOIN_ROUND);
    m_context->move_to(startPos.x(), startPos.y());
}

void xDisplay::endPath(void) {
    m_context->stroke();
}

void xDisplay::drawLine(const screenPoint &from, const screenPoint &to) {
#ifdef OSM_DEBUG
    cout << "From " << from.x() << "x" << from.y() << endl;
    cout << "To " << to.x() << "x" << to.y() << endl;
#endif

    if (to == from)
        return;

    // Draw it
    m_context->move_to(from.x(), from.y());
    m_context->line_to(to.x(), to.y());
}
