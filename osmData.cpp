//
// Created by joe on 13/01/16.
//

#include "osmData.h"

using namespace osmMapper;

std::shared_ptr<osmBounds> osmData::bounds(void) const
{
    return m_bounds;
}

std::shared_ptr<osmNode> osmData::node(OSM_id_t nodeID) const
{
    return m_nodes.at(nodeID);
}

std::shared_ptr<osmRoad> osmData::road(OSM_id_t roadID) const
{
    return m_roads.at(roadID);
}

std::vector<osmNode> osmData::getNodes(const osmRoad &road) const
{
    std::vector<osmNode> nodes;
    for (auto segment : road.segments()) {
            nodes.push_back(*m_nodes.at(segment));
    }
    return nodes;
}

std::map<OSM_id_t, std::shared_ptr<osmRoad> >::iterator osmData::roadBegin()
{
    return m_roads.begin();
}
