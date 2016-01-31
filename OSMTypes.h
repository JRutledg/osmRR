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

    static const colour_t blue={0.329411765, 0.333333333, 0.921568627};
    static const colour_t pink={0.988235294, 0.380392157, 0.619607843};
    static const colour_t orange={0.996078431, 0.666666667, 0.141176471};
    static const colour_t green={0.0, 0.662745098, 0.121568627};
    static const colour_t yellow={0.988235294, 0.890196078, 0.035294118};
    static const colour_t white={1.0, 1.0, 1.0};
    static const colour_t black={0.0, 0.0, 0.0};

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

        friend inline bool operator==(const screenPoint &lhs, const screenPoint &rhs) {
            return (lhs.x() == rhs.x() && lhs.y() == rhs.y());
        }

        friend inline bool operator!=(const screenPoint &lhs, const screenPoint &rhs) {
            return !(lhs == rhs);
        }

    private:
        double m_x;
        double m_y;
    };
} // namespace osmMapper

#endif //CAIROEXPERIMENTS_OSMTYPES_H
