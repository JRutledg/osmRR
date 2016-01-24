//
// Created by joe on 13/12/15.
//

#ifndef CAIROEXPERIMENTS_OSMROAD_H
#define CAIROEXPERIMENTS_OSMROAD_H

#include "OSMTypes.h"

#include <string>
#include <memory>
#include <vector>

namespace osmMapper {

    class osmRoad {
    public:
        static std::shared_ptr<osmRoad> create(OSM_id_t wayId, const std::string &name, const std::vector<OSM_id_t>
        &segments) {
            return std::make_shared<osmRoad>(wayId, name, segments);
        }

        int wayId() const {
            return m_wayId;
        }

        const std::string name() const {
            return m_name;
        }

        const std::vector<OSM_id_t> segments() const {
            return m_segments;
        }

        osmRoad(OSM_id_t wayId, const std::string &name, const std::vector<OSM_id_t> &segments) : m_wayId(wayId), m_name
                (name), m_segments(segments) { }

    private:
        const OSM_id_t m_wayId;
        const std::string m_name;
        const std::vector<OSM_id_t> m_segments;
    };

} // namespace osmMapper

#endif //CAIROEXPERIMENTS_OSMROAD_H
