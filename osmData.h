//
// Created by joe on 13/01/16.
//

#ifndef OSMMAPPER_OSMDATA_H
#define OSMMAPPER_OSMDATA_H

#include "osmBounds.h"
#include "osmNode.h"
#include "osmRoad.h"

#include <list>
#include <unordered_map>
#include <map>

namespace osmMapper {

    class osmData {
    public:
        osmData(std::shared_ptr<osmBounds> bounds, std::unordered_map<OSM_id_t, std::shared_ptr<osmNode> > nodes,
                std::map<OSM_id_t, std::shared_ptr<osmRoad> > roads) : m_bounds(bounds), m_nodes(nodes),
                                                                       m_roads(roads) { };

        std::shared_ptr<osmBounds> bounds(void) const;

        std::shared_ptr<osmNode> node(OSM_id_t nodeID) const;

        std::shared_ptr<osmRoad> road(OSM_id_t roadID) const;

        std::vector<osmNode> getNodes(const osmRoad &road) const;

        std::vector<osmRoad> getRoads(void) const;

    private:
        std::shared_ptr<osmBounds> m_bounds;
        std::unordered_map<OSM_id_t, std::shared_ptr<osmNode> > m_nodes;
        std::map<OSM_id_t, std::shared_ptr<osmRoad> > m_roads;
    };

} // namespace osmMapper

#endif //OSMMAPPER_OSMDATA_H
