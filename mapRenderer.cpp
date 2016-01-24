//
// Created by joe on 24/01/16.
//

#include "mapRenderer.h"

#include "OSMTypes.h"
#include "osmData.h"
#include "osmRoad.h"
#include "osmNode.h"
#include "osmBounds.h"

using namespace osmMapper;

void mapRenderer::drawRoad(const osmRoad &road, const osmBounds &bounds) {
    auto nodesToRender=m_data.getNodes(road);
    /*
            cairo_move_to(ctx, startX, startY);
            cairo_line_to(ctx, x1, y1);
            */
}

void mapRenderer::drawAllRoads(const osmBounds &bounds) {

}
