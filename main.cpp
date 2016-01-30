#include "osmParser.h"
#include "xDisplay.h"
#include "mapRenderer.h"

#include <boost/graph/directed_graph.hpp>
#include <boost/timer/timer.hpp>

#include <iostream>
#include <X.h>
#include <Xlib.h>
#include <Xutil.h>


using namespace std;
using namespace osmMapper;

/*! Check for Xlib Mouse/Keypress events. All other events are discarded.
 * @param sfc Pointer to Xlib surface.
 * @param block If block is set to 0, this function always returns immediately
 * and does not block. if set to a non-zero value, the function will block
 * until the next event is received.
 * @return The function returns 0 if no event occured (and block is set). A
 * positive value indicates that a key was pressed and the X11 key symbol as
 * defined in <X11/keysymdef.h> is returned. A negative value indicates a mouse
 * button event. -1 is button 1 (left button), -2 is the middle button, and -3
 * the right button.
 */
int cairo_check_event(cairo_surface_t *sfc, int block) {
    char keybuf[8];
    KeySym key;
    XEvent e;

    for (; ;) {
        if (block || XPending(cairo_xlib_surface_get_display(sfc)))
            XNextEvent(cairo_xlib_surface_get_display(sfc), &e);
        else
            return 0;

        switch (e.type) {
            case ButtonPress:
                return -e.xbutton.button;
            case KeyPress:
                XLookupString(&e.xkey, keybuf, sizeof(keybuf), &key, NULL);
                return key;
            default:
                fprintf(stderr, "Dropping unhandled XEevent.type = %d.\n", e.type);
        }
    }
}



int main(int argc, char **argv) {
    int x= 0, y= 0;
    struct timespec ts={0, 5000000};

    osmParser p;
    std::shared_ptr<osmData> mapData;
    {
        boost::timer::auto_cpu_timer parse_time;
        mapData= p.parseFile("/home/joe/map.xml");
    }
    std::shared_ptr<xDisplay> xDisp= std::make_shared<xDisplay>(x, y);
    if (mapData && xDisp) {
        auto renderer= mapRenderer(xDisp, mapData);
        for (int i=0; i<1; i++) {
            boost::timer::auto_cpu_timer render_time;
            renderer.drawAllRoads(*mapData->bounds());
        }
        sleep(20);

        /*
         * TODO: Catch input events
        for (int running = 1; running;) {
               switch (cairo_check_event(sfc, 0)) {
                case 0xff1b:   // Esc
                case -1:       // left mouse button
                    running = 0;
                    break;
            }

            usleep(15000);
        }
        */
    }
    else {
        cerr << "Failed to load map data" << endl;
    }
    return 0;
}
