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

    //! OSM defines more road types than I initially care about
    //! So I split them up as follows:
    //! OSM Name     Enum
    //! motorway     motorway
    //! trunk        trunk
    //! primary      primary
    //! secondary    secondary
    //! tertiary     tertiary
    //! unclassified minor
    //! residential  minor
    //! service      minor
    //! "link" types follow their parent at the moment, though their size should be smaller
    //! everything else gets rendered as minor until I implement them
    enum road_category_t {
        motorway, trunk, primary, secondary, tertiary, minor
    };

    class osmRoad {
    public:
        static std::shared_ptr<osmRoad> create(OSM_id_t wayId, const std::string &name, const std::vector<OSM_id_t>
        &segments, const road_category_t &cat) {
            return std::make_shared<osmRoad>(wayId, name, segments, cat);
        }

        OSM_id_t wayId() const {
            return m_wayId;
        }

        const std::string name() const {
            return m_name;
        }

        const road_category_t category() const {
            return m_category;
        }

        const std::vector<OSM_id_t> segments() const {
            return m_segments;
        }

        osmRoad(OSM_id_t wayId, const std::string &name, const std::vector<OSM_id_t> &segments,
                const road_category_t &cat)
                : m_wayId(wayId), m_name(name), m_segments(segments), m_category(cat) { }

    private:
        OSM_id_t m_wayId;
        std::string m_name;
        std::vector<OSM_id_t> m_segments;
        road_category_t m_category;
    };
} // namespace osmMapper

#endif //CAIROEXPERIMENTS_OSMROAD_H
