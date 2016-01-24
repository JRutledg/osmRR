//
// Created by joe on 13/12/15.
//

#ifndef CAIROEXPERIMENTS_OSMTYPES_H
#define CAIROEXPERIMENTS_OSMTYPES_H

#include <cstdint>

namespace osmMapper {

    typedef int_fast64_t OSM_id_t;

    class compassPoint {
    public:
        compassPoint(float latitude, float longitude) : m_lat(latitude), m_lon(longitude) { }

        float lat(void) const { return m_lat; }

        float lon(void) const { return m_lon; }

    private:
        float m_lat;
        float m_lon;
    };

    class screenPoint {
    public:
        screenPoint(int x, int y) : m_x(x), m_y(y) { }

        int x(void) const { return m_x; }

        int y(void) const { return m_y; }

    private:
        int m_x;
        int m_y;
    };

} // namespace osmMapper

#endif //CAIROEXPERIMENTS_OSMTYPES_H
