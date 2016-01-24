//
// Created by joe on 13/12/15.
//

#ifndef CAIROEXPERIMENTS_OSMTYPES_H
#define CAIROEXPERIMENTS_OSMTYPES_H

#include <cstdint>

namespace osmMapper {
    struct colour_t {
        double r;
        double g;
        double b;
    };

    typedef int_fast64_t OSM_id_t;

    class compassPoint {
    public:
        compassPoint(double latitude, double longitude) : m_lat(latitude), m_lon(longitude) { }

        double lat(void) const { return m_lat; }

        double lon(void) const { return m_lon; }

    private:
        double m_lat;
        double m_lon;
    };

    class screenPoint {
    public:
        screenPoint(double x, double y) : m_x(x), m_y(y) { }

        double x(void) const { return m_x; }

        double y(void) const { return m_y; }

        friend inline bool operator==(const screenPoint& lhs, const screenPoint& rhs){
            return (lhs.x() == rhs.x() && lhs.y() == rhs.y());
        }

        friend inline bool operator!=(const screenPoint& lhs, const screenPoint& rhs){
            return !(lhs == rhs);
        }

    private:
        double m_x;
        double m_y;
    };

} // namespace osmMapper

#endif //CAIROEXPERIMENTS_OSMTYPES_H
