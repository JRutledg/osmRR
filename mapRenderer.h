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
        mapRenderer(std::shared_ptr<display> disp, std::shared_ptr<osmData> osmMap) :
                m_disp(disp), m_data(osmMap), m_needsRedraw(true) { }

        void drawRoad(const osmRoad &road, const osmBounds &bounds);

        void drawAllRoads(const osmBounds &bounds);

    private:
        screenPoint compassPointToScreenPoint(const compassPoint &inPoint, const osmBounds &bounds);

        std::shared_ptr<display> m_disp;
        std::shared_ptr<osmData> m_data;
        bool m_needsRedraw;
    };
}
#endif //OSMMAPPER_MAPRENDERER_H
