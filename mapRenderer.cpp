//
// Created by joe on 24/01/16.
//

#include "mapRenderer.h"

#include "OSMTypes.h"
#include "osmData.h"
#include "osmRoad.h"
#include "osmNode.h"
#include "osmBounds.h"

#include <iostream>
#include <cmath>

#include <algorithm>

using namespace std;
using namespace osmMapper;

// #define OSM_DEBUG

//! Calculate a screen pixel coordinate from a lat/long coordinate
//! Yes. This is hacky and *wrong* but it serves to get us going for now.
//! This should really be handling the projection correctly...
//!
//! \param inPoint The geographic coordinate you need converted
//! \param bounds The bounds within which this coordinate exists (i.e. the "zoom" level)
//! \returns The inPoint in screen space
screenPoint mapRenderer::compassPointToScreenPoint(const compassPoint &inPoint, const osmBounds &bounds){
    double latOffset= 0;
    double lonOffset= 0;

    if(bounds.getMinLatitude() < 0)
        latOffset= fabs(bounds.getMinLatitude());

    if(bounds.getMinLongitude() < 0)
        lonOffset= fabs(bounds.getMinLongitude());

    double minLonBound=bounds.getMinLongitude() + lonOffset;
    double maxLonBound=bounds.getMaxLongitude() + lonOffset;
    double minLatBound=bounds.getMinLatitude() + latOffset;
    double maxLatBound=bounds.getMaxLatitude() + latOffset;

    double screenX=(((inPoint.lon() + lonOffset) - minLonBound) * m_disp->width()) / (maxLonBound - minLonBound);
    double screenY=((maxLatBound - (inPoint.lat() + latOffset)) * m_disp->height()) / (maxLatBound - minLatBound);

#ifdef OSM_DEBUG
    cout << "Point lat: " << inPoint.lat() << " Point lon: " << inPoint.lon() << endl;
    cout << "Bounds min lat: " << bounds.getMinLatitude() << " Bounds min lon: " << bounds.getMinLongitude() << endl;
    cout << "Bounds max lat: " << bounds.getMaxLatitude() << " Bounds max lon: " << bounds.getMaxLongitude() << endl;
    cout << "latOffset: " << latOffset << " lonOffset: " << lonOffset << endl;
    cout << "minx: " << minLonBound << " maxx: " << maxLonBound << " miny: " << minLatBound << " maxy: " << maxLatBound << endl;
#endif // OSM_DEBUG

    return screenPoint(screenX,screenY);
}

void mapRenderer::drawRoad(const osmRoad &road, const osmBounds &bounds) {
    auto nodesToRender= m_data->getNodes(road);
    const auto startNode= nodesToRender.front();
    screenPoint prevNodeLoc= compassPointToScreenPoint(startNode.location(), bounds);

#ifdef OSM_DEBUG
    cout << "Drawing road " << road.name();
#endif
    colour_t roadColour;
    double roadWidth;
    switch(road.category())
    {
        case motorway:
            roadColour= blue;
            roadWidth= 8.0;
            break;
        case trunk:
            roadColour= green;
            roadWidth= 7.0;
            break;
        case primary:
            roadColour= pink;
            roadWidth= 6.0;
            break;
        case secondary:
            roadColour= orange;
            roadWidth= 4.0;
            break;
        case tertiary:
            roadColour= yellow;
            roadWidth= 2.0;
            break;
        case minor: // Intentional fall through
        default:
            roadColour= white;
            roadWidth= 1.0;
            break;
    }
    m_disp->startPath(prevNodeLoc, roadColour, roadWidth, false);
    for (auto node : nodesToRender){
        screenPoint newNodeLoc= compassPointToScreenPoint(node.location(), bounds);
        m_disp->drawLine(prevNodeLoc, newNodeLoc);
        prevNodeLoc= newNodeLoc;
    }
    m_disp->endPath();
    cout << endl;
}

bool roadSorter (const osmRoad &i, const osmRoad &j) { return (i.category() > j.category()); }

void mapRenderer::drawAllRoads(const osmBounds &bounds) {
#ifdef OSM_DEBUG
    cout << "Rendering all roads" << endl;
#endif
    auto roads= m_data->getRoads();
    // Painters algo, sort by road priority
    std::sort(roads.begin(), roads.end(), roadSorter);
    m_disp->startDrawing();
    for (auto road : roads)
    {
        drawRoad(road, bounds);
    }
    m_disp->finishDrawing();
}

#ifdef OSM_DEBUG
#undef OSM_DEBUG
#endif
