#include "osmParser.h"
#include "display.h"
#include "mapRenderer.h"

#include <boost/graph/directed_graph.hpp>

#include <iostream>

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
    cairo_surface_t *sfc;
    cairo_t *ctx;
    int x, y;
    struct timespec ts={0, 5000000};

    osmParser p;
    auto mapData=p.parseFile("/home/joe/osmData.xml");
    auto disp=xDisplay(x, y);
    if (mapData) {
        auto renderer= mapRenderer(disp, *mapData);
        renderer.drawAllRoads(*mapData->bounds());

        /*
        typedef boost::directed_graph<> Graph;
        Graph g;
        boost::graph_traits<Graph>::vertex_descriptor v0 = g.add_vertex();
        boost::graph_traits<Graph>::vertex_descriptor v1 = g.add_vertex();

        g.add_edge(v0, v1);

        double x0 = 20, y0 = 20, x1 = 200, y1 = 400, x2 = 450, y2 = 100;
        double dx0 = 1, dx1 = 1.5, dx2 = 2;
        double dy0 = 2, dy1 = 1.5, dy2 = 1;
        int running;

        x = y = 0;
        sfc = cairo_create_x11_surface(&x, &y);
        int w= cairo_xlib_surface_get_width(sfc);
        int h= cairo_xlib_surface_get_height(sfc);
        cout << "Display: " <<  w << "x" << h << endl;
        int wmul= w / fabs(mapData->bounds()->getMaxLongitude() - mapData->bounds()->getMinLongitude());
        int hmul= h / fabs(mapData->bounds()->getMaxLatitude() - mapData->bounds()->getMinLatitude());
        cout << "wmul: " << wmul << " hmul: " << hmul << endl;
        int wmin= fabs(mapData->bounds()->getMinLongitude() - mapData->bounds()->getMinLongitude()) * wmul;
        int wmax= fabs(mapData->bounds()->getMaxLongitude() - mapData->bounds()->getMinLongitude()) * wmul;
        int hmin= fabs(mapData->bounds()->getMinLatitude() - mapData->bounds()->getMinLatitude()) * hmul;
        int hmax= fabs(mapData->bounds()->getMaxLatitude() - mapData->bounds()->getMinLatitude()) * hmul;
        cout << "Screen bounds: wmin " << wmin << " wmax " << wmax << " hmin " << hmin << " hmax " << hmax << endl;

        ctx = cairo_create(sfc);

        for (running = 1; running;) {
            cairo_push_group(ctx);
            cairo_set_source_rgb(ctx, 1, 1, 1);
            cairo_paint(ctx);
            cairo_move_to(ctx, x0, y0);
            cairo_line_to(ctx, x1, y1);
            cairo_line_to(ctx, x2, y2);
            cairo_line_to(ctx, x0, y0);
            cairo_set_source_rgb(ctx, 0, 0, 1);
            cairo_fill_preserve(ctx);
            cairo_set_line_width(ctx, 5);
            cairo_set_source_rgb(ctx, 1, 1, 0);
            cairo_stroke(ctx);
            cairo_set_source_rgb(ctx, 0, 0, 0);
            cairo_move_to(ctx, x0, y0);
            cairo_show_text(ctx, "P0");
            cairo_move_to(ctx, x1, y1);
            cairo_show_text(ctx, "P1");
            cairo_move_to(ctx, x2, y2);
            cairo_show_text(ctx, "P2");
            cairo_pop_group_to_source(ctx);
            cairo_paint(ctx);
            cairo_surface_flush(sfc);

            x0 += dx0;
            y0 += dy0;
            x1 += dx1;
            y1 += dy1;
            x2 += dx2;
            y2 += dy2;

            switch (cairo_check_event(sfc, 0)) {
                case 0xff1b:   // Esc
                case -1:       // left mouse button
                    running = 0;
                    break;
            }

            nanosleep(&ts, NULL);
        }
        */
    }
    else {
        cerr << "Failed to load map data" << endl;
    }
    return 0;
}
