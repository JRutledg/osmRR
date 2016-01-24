//
// Created by joe on 12/12/15.
//
#include "osmParser.h"
#include "osmBounds.h"

#include <pugixml.hpp>

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <cstring>
#include <memory>

using namespace std;
using namespace pugi;
using namespace osmMapper;

std::shared_ptr<osmBounds> osmParser::populateBounds(const pugi::xml_node &osmRoot){
    auto bounds= osmRoot.child("bounds");
    float minLat= boost::lexical_cast<float>(bounds.attribute("minlat").value());
    float maxLat= boost::lexical_cast<float>(bounds.attribute("maxlat").value());
    float minLon= boost::lexical_cast<float>(bounds.attribute("minlon").value());
    float maxLon= boost::lexical_cast<float>(bounds.attribute("maxlon").value());
    return osmBounds::create(minLat, maxLat, minLon, maxLon);
}

std::unordered_map<OSM_id_t, std::shared_ptr<osmNode> > osmParser::populateNodes(const pugi::xml_node &osmRoot){
    unordered_map<OSM_id_t, std::shared_ptr<osmNode> > nodes;
    for (xml_node node: osmRoot.children("node")) {
        try {
            OSM_id_t id= boost::lexical_cast<OSM_id_t>(node.attribute("id").value());
            float latitude= boost::lexical_cast<float>(node.attribute("lat").value());
            float longitude= boost::lexical_cast<float>(node.attribute("lon").value());
            nodes[id]= osmNode::create(id, compassPoint(latitude, longitude));
        }
        catch(exception &e)
        {
            cout << "Hmm, unexpected data: " << e.what() << endl;
        }
    }
    return nodes;
}

std::map<OSM_id_t, std::shared_ptr<osmRoad> > osmParser::populateRoads(const pugi::xml_node &osmRoot) {
    map<OSM_id_t, shared_ptr<osmRoad> > roads;
    for (xml_node way: osmRoot.children("way")) {
        bool highway = false;
        for (xml_node child: way.children()) {
            if (strcmp(child.name(), "tag") == 0) {
                for (xml_attribute attr: child.attributes()) {
                    if (strcmp(attr.value(), "highway") == 0) {
                        highway = true;
                    }
                }
            }
        }
        if (highway) {
            string name;
            vector<OSM_id_t> segments;
            OSM_id_t id= boost::lexical_cast<OSM_id_t>(way.attribute("id").value());
            for (xml_node child: way.children()) {
                if (strcmp(child.name(), "tag") == 0) {
                    // Tags are metadata
                    for (xml_attribute nattr: child.attributes()) {
                        if (strcmp(nattr.value(), "name") == 0) {
                            cout << "Found name" << endl;
                            name= child.attribute("v").value();
                        }
                    }
                }
                else if (strcmp(child.name(), "nd") == 0) {
                    // nd is a node reference
                    cout << "Found segment" << endl;
                    segments.push_back(boost::lexical_cast<OSM_id_t>(child.attribute("ref").value()));
                }
            }
            roads[id]= osmRoad::create(id, name, segments);
        }
    }
    return roads;
}

shared_ptr<osmData> osmParser::parseFile(const std::string &filename) {
    xml_document doc;

    xml_parse_result result = doc.load_file(filename.c_str());
    if (result.status != xml_parse_status::status_ok) {
        cout << "Couldn't parse file " << filename << endl;
        return nullptr;
    }
    const xml_node root = doc.child("osm");

    auto nodes= populateNodes(root);
    auto roads= populateRoads(root);
    cout << nodes.size() << " nodes found" << endl;
    for (auto road: roads) {
        cout << "Way: " << road.second->wayId()
        << " Name: " << road.second->name()
        << " Segments: " << road.second->segments().size()
        << endl;
    }
    shared_ptr<osmBounds> bounds= populateBounds(root);
    cout << "Bounds are: " << bounds->getMinLatitude() << " " << bounds->getMaxLatitude() << " " << bounds->getMinLongitude() <<
            " " << bounds->getMaxLongitude() << endl;
    return make_shared<osmData>(bounds, nodes, roads);
}
