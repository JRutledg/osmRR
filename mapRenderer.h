//
// Created by joe on 24/01/16.
//

#ifndef OSMMAPPER_MAPRENDERER_H
#define OSMMAPPER_MAPRENDERER_H

#include "osmData.h"
#include "display.h"

namespace osmMapper {

    class mapRenderer {
    public:
        mapRenderer(display &disp, osmData &osmMap) : m_disp(disp), m_data(osmMap){}
        void drawRoad(const osmRoad &road, const osmBounds &bounds);
        void drawAllRoads(const osmBounds &bounds);

    private:
        display m_disp;
        osmData m_data;
    };

}
#endif //OSMMAPPER_MAPRENDERER_H
