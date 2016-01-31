#include "osmParser.h"
#include "xDisplay.h"
#include "mapRenderer.h"

#include <boost/graph/directed_graph.hpp>
#include <boost/timer/timer.hpp>

using namespace std;
using namespace osmMapper;

int main(int argc, char **argv) {
    int x=0, y=0;
    struct timespec ts={0, 5000000};

    osmParser p;
    std::shared_ptr<osmData> mapData;
    {
        boost::timer::auto_cpu_timer parse_time;
        mapData=p.parseFile("/home/joe/map.xml");
    }
    std::shared_ptr<xDisplay> xDisp=std::make_shared<xDisplay>(x, y);
    if (mapData && xDisp) {
        auto renderer=mapRenderer(xDisp, mapData);
        bool stopping=false;
        do {
            auto receivedEvent=xDisp->getEvent();
            //! TODO: Lift this processing out of main into something of its own
            switch (receivedEvent.type) {
                case event::BUTTON:
                    //doMouseythings();
                    //break;
                    // Intentional drop through atm
                case event::KEY:
                    stopping=true;
                    break;
                default:
                    break;
            }
            if (!stopping) {
                boost::timer::auto_cpu_timer render_time;
                renderer.drawAllRoads(*mapData->bounds());
            }
        }
        while (!stopping);
    }
    else {
        cerr << "Failed to load map data" << endl;
    }
    return 0;
}
