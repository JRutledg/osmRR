//
// Created by joe on 12/12/15.
//

#ifndef CAIROEXPERIMENTS_OSMPARSER_H
#define CAIROEXPERIMENTS_OSMPARSER_H

#include "osmRoad.h"
#include "osmNode.h"
#include "osmBounds.h"
#include "osmData.h"

#include <pugixml.hpp>

#include <string>
#include <memory>
#include <list>
#include <unordered_map>
#include <map>

namespace osmMapper {

    class osmParser {
    public:
        std::shared_ptr<osmData> parseFile(const std::string &filename);

    private:
        std::shared_ptr<osmBounds> populateBounds(const pugi::xml_node &osmRoot);

        std::unordered_map<OSM_id_t, std::shared_ptr<osmNode> > populateNodes(const pugi::xml_node &osmRoot);

        std::map<OSM_id_t, std::shared_ptr<osmRoad> > populateRoads(const pugi::xml_node &osmRoot);
    };
} // namespace osmMapper

#endif //CAIROEXPERIMENTS_OSMPARSER_H
