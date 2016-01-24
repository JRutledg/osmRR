//
// Created by joe on 13/12/15.
//

#ifndef CAIROEXPERIMENTS_OSMBOUNDS_H
#define CAIROEXPERIMENTS_OSMBOUNDS_H

#include "OSMTypes.h"

#include <string>
#include <memory>

namespace osmMapper {

    class osmBounds {
    public:
        static std::shared_ptr<osmBounds> create(float minLatitude, float maxLatitude, float minLongitude, float
        maxLongitude) {
            return std::make_shared<osmBounds>(minLatitude, maxLatitude, minLongitude, minLatitude);
        }

        const float getMinLatitude() const {
            return m_minLatitude;
        }

        const float getMaxLatitude() const {
            return m_maxLatitude;
        }

        const float getMinLongitude() const {
            return m_minLongitude;
        }

        const float getMaxLongitude() const {
            return m_maxLongitude;
        }

        osmBounds(float minLatitude, float maxLatitude, float minLongitude, float
        maxLongitude) : m_minLatitude(minLatitude), m_maxLatitude(maxLatitude), m_minLongitude(minLongitude),
                        m_maxLongitude(maxLongitude) { }

    private:
        const float m_minLatitude;
        const float m_maxLatitude;
        const float m_minLongitude;
        const float m_maxLongitude;
    };

} // namespace osmMapper;

#endif //CAIROEXPERIMENTS_OSMBOUNDS_H
