//
// Created by joe on 13/12/15.
//

#ifndef CAIROEXPERIMENTS_OSMNODE_H
#define CAIROEXPERIMENTS_OSMNODE_H

#include "OSMTypes.h"

#include <string>
#include <memory>

namespace osmMapper {

    class osmNode {
    public:
        static std::shared_ptr<osmNode> create(OSM_id_t id, const compassPoint &location) {
            return std::make_shared<osmNode>(id, location);
        }

        osmNode(OSM_id_t id, const compassPoint &location) : m_id(id), m_location(location) { }

        compassPoint location(void) const {return m_location;}

    private:
        const OSM_id_t m_id;
        const compassPoint m_location;
    };

} // namespace osmMapper

#endif //CAIROEXPERIMENTS_OSMNODE_H
