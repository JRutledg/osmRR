//
// Created by joe on 12/12/15.
//
#include "osmParser.h"

#include <boost/lexical_cast.hpp>

#include <iostream>

using namespace std;
using namespace pugi;
using namespace osmMapper;

std::shared_ptr<osmBounds> osmParser::populateBounds(const pugi::xml_node &osmRoot) {
    auto bounds=osmRoot.child("bounds");
    float minLat=boost::lexical_cast<float>(bounds.attribute("minlat").value());
    float maxLat=boost::lexical_cast<float>(bounds.attribute("maxlat").value());
    float minLon=boost::lexical_cast<float>(bounds.attribute("minlon").value());
    float maxLon=boost::lexical_cast<float>(bounds.attribute("maxlon").value());
    return osmBounds::create(minLat, maxLat, minLon, maxLon);
}

std::unordered_map<OSM_id_t, std::shared_ptr<osmNode> > osmParser::populateNodes(const pugi::xml_node &osmRoot) {
    unordered_map<OSM_id_t, std::shared_ptr<osmNode> > nodes;
    for (xml_node node: osmRoot.children("node")) {
        try {
            OSM_id_t id=boost::lexical_cast<OSM_id_t>(node.attribute("id").value());
            float latitude=boost::lexical_cast<float>(node.attribute("lat").value());
            float longitude=boost::lexical_cast<float>(node.attribute("lon").value());
            nodes[id]=osmNode::create(id, compassPoint(latitude, longitude));
        }
        catch (exception &e) {
            cout << "Hmm, unexpected data: " << e.what() << endl;
        }
    }
    return nodes;
}

std::map<OSM_id_t, std::shared_ptr<osmRoad> > osmParser::populateRoads(const pugi::xml_node &osmRoot) {
    map<OSM_id_t, shared_ptr<osmRoad> > roads;
    for (xml_node way: osmRoot.children("way")) {
        bool highway=false;
        for (xml_node child: way.children()) {
            if (strcmp(child.name(), "tag") == 0) {
                for (xml_attribute attr: child.attributes()) {
                    if (strcmp(attr.value(), "highway") == 0) {
                        highway=true;
                    }
                }
            }
        }
        if (highway) {
            string name;
            road_category_t roadCategory=minor;
            vector<OSM_id_t> segments;
            OSM_id_t id=boost::lexical_cast<OSM_id_t>(way.attribute("id").value());
            for (xml_node child: way.children()) {
                if (strcmp(child.name(), "tag") == 0) {
                    // Tags are metadata
                    for (xml_attribute nattr: child.attributes()) {
                        if (strcmp(nattr.value(), "name") == 0) {
#ifdef OSM_DEBUG
                            cout << "Found name" << endl;
#endif
                            name=child.attribute("v").value();
                        }
                        if (strcmp(nattr.value(), "highway") == 0) {
                            // Road type
                            if (strcmp(child.attribute("v").value(), "motorway") == 0) {
                                roadCategory=motorway;
                            }
                            else if (strcmp(child.attribute("v").value(), "trunk") == 0) {
                                roadCategory=trunk;
                            }
                            else if (strcmp(child.attribute("v").value(), "primary") == 0) {
                                roadCategory=primary;
                            }
                            else if (strcmp(child.attribute("v").value(), "secondary") == 0) {
                                roadCategory=secondary;
                            }
                            else if (strcmp(child.attribute("v").value(), "tertiary") == 0) {
                                roadCategory=tertiary;
                            }
                            else if (strcmp(child.attribute("v").value(), "motorway_link") == 0) {
                                roadCategory=motorway;
                            }
                            else if (strcmp(child.attribute("v").value(), "trunk_link") == 0) {
                                roadCategory=trunk;
                            }
                            else if (strcmp(child.attribute("v").value(), "primary_link") == 0) {
                                roadCategory=primary;
                            }
                            else if (strcmp(child.attribute("v").value(), "secondary_link") == 0) {
                                roadCategory=secondary;
                            }
                            else if (strcmp(child.attribute("v").value(), "tertiary_link") == 0) {
                                roadCategory=tertiary;
                            }
                        }
                    }
                }
                else if (strcmp(child.name(), "nd") == 0) {
                    // nd is a node reference
#ifdef OSM_DEBUG
                    cout << "Found segment" << endl;
#endif
                    segments.push_back(boost::lexical_cast<OSM_id_t>(child.attribute("ref").value()));
                }
            }
            roads[id]=osmRoad::create(id, name, segments, roadCategory);
        }
    }
    return roads;
}

shared_ptr<osmData> osmParser::parseFile(const std::string &filename) {
    xml_document doc;

    xml_parse_result result=doc.load_file(filename.c_str());
    if (result.status != xml_parse_status::status_ok) {
        cout << "Couldn't parse file " << filename << endl;
        return nullptr;
    }
    const xml_node root=doc.child("osm");

    auto nodes=populateNodes(root);
    auto roads=populateRoads(root);
    cout << nodes.size() << " nodes found" << endl;
#ifdef OSM_DEBUG
    for (auto road: roads) {
        cout << "Way: " << road.second->wayId()
        << " Name: " << road.second->name()
        << " Segments: " << road.second->segments().size()
        << endl;
    }
#endif
    cout << roads.size() << " roads found" << endl;
    shared_ptr<osmBounds> bounds=populateBounds(root);
    cout << "Bounds are: " << bounds->getMinLatitude() << " " << bounds->getMaxLatitude() << " " <<
    bounds->getMinLongitude() <<
    " " << bounds->getMaxLongitude() << endl;
    return make_shared<osmData>(bounds, nodes, roads);
}
