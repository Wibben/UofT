/* 
 * Copyright 2019 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <utility>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include "m1.h"
#include "m2.h"
#include "draw_functions.h"
#include "callback_definitions.h"
#include "directions.h"
#include "TrieNode.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <set>
// For R-Trees
// Instructions on how to use them from:
// https://www.boost.org/doc/libs/1_54_0/libs/geometry/doc/html/geometry/spatial_indexes/rtree_quickstart.html
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bgi = boost::geometry::index;

//Data structures initialization
std::vector<std::vector<unsigned>> street_segments_from_intersection;
std::vector<std::vector<std::string>> intersections_Name;
std::vector<std::vector<unsigned>> streets_segments;
std::vector<std::set<unsigned>> intersections_from_street_id;

std::vector<std::vector<LatLon>> feature_LatLon_from_feature_id;
std::vector<std::vector<ezgl::point2d>> feature_point2d_from_feature_id;
std::unordered_map<OSMID, unsigned> street_segments_from_OSMID;
std::vector<std::string> segment_type_from_segment_id;
std::unordered_map<std::string, std::string> segment_type_from_segment_name;
std::vector<std::pair<double, int>> feature_id_by_area;

// Create the rtree for closest POIs
BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(
        RTreeLatLon, float,
        boost::geometry::cs::spherical_equatorial<boost::geometry::degree>,
        get_lon, get_lat, set_lon, set_lat)
bgi::rtree<std::pair<RTreeLatLon,POIIndex>, bgi::quadratic<50> > poi_rtree;

//Highest zoom - major features only
std::vector<int> level_5_features;
//Lower zoom - major and multi-segment lines
std::vector<int> level_4_features;
//Lowest zoom - all features
std::vector<int> level_1_features;

//Highest zoom - major roads only
std::vector<int> level_3_roads;
//Medium priority roads and major roads
std::vector<int> level_2_roads;
//Lowest zoom - all roads
std::vector<int> level_1_roads;

//Data of where the names have been drawn, used to avoid clustering
std::vector<ezgl::point2d> roads_drawn;

double * street_segment_length;
double * segment_travel_time;

//used to determine coordinates of drawing area
double max_lat, min_lat, max_lon, min_lon;

bool* highlighted_intersection;
//Creating the root node for the trie to store street names
TrieNode street_names_trie(true, false);

bool load_map(std::string map_path) {
    bool load_successful = false; //Indicates whether the map has loaded 
    //successfully
    
    // Continually update endpoint intersections for directions
    do_get_from = do_get_to = true;
    from_intersection = to_intersection = -1; // Reset to and from intersections
    
    // ".streets.bin" is the last 12 characters of the filepath
    // So to get OSM path remove that and add on ".osm.bin" to the end
    std::string OSM_path = map_path.substr(0,map_path.length()-12) + ".osm.bin";
    
    if(loadOSMDatabaseBIN(OSM_path)){
        std::cout << "Successfully loaded OSM data '" << OSM_path << "'" << std::endl;
    }
    
    if (!loadStreetsDatabaseBIN(map_path)) {
        return load_successful;
    } else {
        load_successful = true; //Make sure this is updated to reflect whether
                                //loading the map succeeded or failed
        std::cout << "Successfully loaded map '" << map_path << "'" << std::endl;
        
        //Initialize and load data structures
        street_segments_from_intersection.resize(getNumIntersections());
        intersections_Name.resize(getNumIntersections());
        streets_segments.resize(getNumStreets());
        intersections_from_street_id.resize(getNumStreets());
        feature_LatLon_from_feature_id.resize(getNumFeatures());
        feature_point2d_from_feature_id.resize(getNumFeatures());
        segment_type_from_segment_id.resize(getNumStreetSegments());
        
        //set up initial values
        max_lat = getIntersectionPosition(0).lat();
        min_lat = max_lat;
        max_lon = getIntersectionPosition(0).lon();
        min_lon = max_lon;
        
        highlighted_intersection = new bool[getNumIntersections()];
        
        for(int a=0; a < getNumIntersections();a++){
            //Load data structures
            for(int b=0; b<getIntersectionStreetSegmentCount(a); b++) {
                //Load street_segments_from_intersection
                street_segments_from_intersection[a].push_back(getIntersectionStreetSegment(b,a));
               
                //Load intersections_Name
                intersections_Name[a].push_back(getStreetName(
                    getInfoStreetSegment(street_segments_from_intersection[a][b]).streetID));
            }
            
            //Find max/min lat/lon values to set up drawing area
            max_lat = std::max(max_lat, getIntersectionPosition(a).lat());
            min_lat = std::min(min_lat, getIntersectionPosition(a).lat());
            max_lon = std::max(max_lon, getIntersectionPosition(a).lon());
            min_lon = std::min(min_lon, getIntersectionPosition(a).lon());
            highlighted_intersection[a] = false;
        }
        
        // Building an array of all street segment lengths and travel times so
        // find_street_length and find_street_segment_travel_time run faster
        street_segment_length = new double [getNumStreetSegments()];
        segment_travel_time = new double [getNumStreetSegments()];
        
        for(int i=0; i<getNumStreetSegments(); i++) {
            // Pre-calculate the length of all street segments
            street_segment_length[i] = find_street_segment_length(i);
            
            // Time to travel through each street segment
            // time(s) = 3.6 * distance(m) / speed limit(km/h)
            // 3.6 multiplier to convert speed limit into m/s
            InfoStreetSegment segment = getInfoStreetSegment(i);
            segment_travel_time[i] = 
                    3.6*street_segment_length[i]/segment.speedLimit;
            
            //Load streets_segments
            streets_segments[getInfoStreetSegment(i).streetID].push_back(i);
            
            street_segments_from_OSMID[getInfoStreetSegment(i).wayOSMID] = i;
                    
        }
        
        for(int i = 0; i < getNumStreets(); i++) {
            //Building trie tree used for street_ids_from_partial_street_name
            std::string temp_str = getStreetName(i);
            //Changing street name to lowercase to allow for case-insensitive
            //searching
            std::transform(temp_str.begin(), temp_str.end(), 
                                    temp_str.begin(), ::tolower);
            street_names_trie.insert(temp_str, i);  //inserting name into trie
            
            //Building intersections_from_street_id for use in find_all_street_intersections
            InfoStreetSegment ss_info;
            std::vector<unsigned> street_segments = find_street_street_segments(i);
            
            // Loop through street's street segments and push intersections into
            // the set, the set by nature will be unique
            for (unsigned j = 0; j < street_segments.size(); j++) {
                ss_info = getInfoStreetSegment(street_segments[j]);

                intersections_from_street_id[i].insert(ss_info.from);
                if(ss_info.to != ss_info.from) // Preemptive check to make it faster
                    intersections_from_street_id[i].insert(ss_info.to);
            }
        }
        
        //Building data structures related to features
        for(int i = 0; i < getNumFeatures(); i++){
            for(int j = 0; j < getFeaturePointCount(i); j++){
                feature_LatLon_from_feature_id[i].push_back(getFeaturePoint(j, i));
                feature_point2d_from_feature_id[i].push_back(make_point2d(getFeaturePoint(j, i)));
            }
            
            std::pair<double, int> tempPair = std::make_pair(polygon_area(feature_LatLon_from_feature_id[i]), i);
            feature_id_by_area.push_back(tempPair);
        }
        
        // Building data structures related to points of interests
        for(int i=0; i<getNumPointsOfInterest(); i++) {
            // Build the rtree
            RTreeLatLon new_latlon(getPointOfInterestPosition(i));
            poi_rtree.insert(std::make_pair(new_latlon,i));
        }
        
        //Sort features by area in descending order
        std::sort(feature_id_by_area.rbegin(), feature_id_by_area.rend());

        for (int i = 0; i < getNumFeatures(); i++) {
            int current_id = feature_id_by_area[i].second;

            double lon1, lon2, lat1, lat2;
            lon1 = feature_LatLon_from_feature_id[current_id][0].lon();
            lat1 = feature_LatLon_from_feature_id[current_id][0].lat();
            lon2 = feature_LatLon_from_feature_id[current_id].back().lon();
            lat2 = feature_LatLon_from_feature_id[current_id].back().lat();
            
            //If the feature is a building
            if (getFeatureType(current_id) == 6) {
                level_1_features.push_back(current_id);
            }
            //If the feature is a multi-segment line
            else if (!((lon1 == lon2) && (lat1 == lat2))) {
                level_4_features.push_back(current_id);
                level_1_features.push_back(current_id);
            }
            else{
                level_5_features.push_back(current_id);
                level_4_features.push_back(current_id);
                level_1_features.push_back(current_id);
            }
        }
        
        //Looping through all ways available in OSM data
        for(unsigned i = 0; i < getNumberOfWays(); i++){
            //Loop through tag types for current way
            for(unsigned j = 0; j < getTagCount(getWayByIndex(i)); j++) {
                std::string key, value;
                std::tie(key, value) = getTagPair(getWayByIndex(i), j);
                //Checking if the current tag is the highway tag (what kind of
                //road it is)
                if(key == "highway"){
                    //Get OSMID for the current way
                    OSMID wayOSMID = getWayByIndex(i)->id();
                    //Use OSMID to get the associated street segment ID
                    unsigned ssID = street_segments_from_OSMID[wayOSMID];
                    //Set road type for the street segment to the found value
                    segment_type_from_segment_id[ssID] = value;
                    
                    std::string street_name = getStreetName(getInfoStreetSegment(ssID).streetID);
                    if(!(street_name == "")){
                        segment_type_from_segment_name[street_name] = value;
                    }
                    
                }
                
            }
        }
        
        ezgl::point2d notdrawn(-1,-1);
        for(int i = 0; i < getNumStreetSegments(); i++){
            std::string segment_type, street_name;
            if(segment_type_from_segment_id[i] == ""){
                street_name = getStreetName(getInfoStreetSegment(i).streetID);
                segment_type = segment_type_from_segment_name[street_name];
            }
            else{
                segment_type = segment_type_from_segment_id[i];
            }
            //std::string segment_type = segment_type_from_segment_id[i];
            if(segment_type == "motorway" || segment_type == "motorway_link" ||
               segment_type == "trunk" || segment_type == "trunk_link" || 
               segment_type == "primary" || segment_type == "primary_link"){
                level_3_roads.push_back(i);
                level_2_roads.push_back(i);
                level_1_roads.push_back(i);
            }
            else if(segment_type == "secondary" || segment_type == "secondary_link" ||
                    segment_type == "tertiary" || segment_type == "tertiary_link" ){
                level_2_roads.push_back(i);
                level_1_roads.push_back(i);
            }
            else {
                level_1_roads.push_back(i);
            }
            roads_drawn.push_back(notdrawn);
        }
        

    }
    
    return load_successful;
}

void close_map() {
    // Reset List of POIs circle
    find_poi_range = 0;
    
    // Deallocates memory used in trie
    street_names_trie.clear_TrieNode();
    
    // Clears the vectors for next map load
    street_segments_from_intersection.clear();
    intersections_Name.clear();
    streets_segments.clear();
    intersections_from_street_id.clear();
    
    feature_LatLon_from_feature_id.clear();
    feature_point2d_from_feature_id.clear();
    street_segments_from_OSMID.clear();
    segment_type_from_segment_id.clear();
    segment_type_from_segment_name.clear();
    feature_id_by_area.clear();
    
    poi_rtree.clear();
    
    level_5_features.clear();
    level_4_features.clear();
    level_1_features.clear();

    level_3_roads.clear();
    level_2_roads.clear();
    level_1_roads.clear();
    
    // Deallocate used memory
    delete [] street_segment_length;
    street_segment_length = NULL;
    delete [] segment_travel_time;
    segment_travel_time = NULL;
    delete [] highlighted_intersection;
    highlighted_intersection = NULL;
    
    closeOSMDatabase();
    closeStreetDatabase(); 
    //Clean-up your map related data structures here
}

//Returns the street segments for the given intersection 
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id){
    //fetch intersection ID from loaded data structure
    return street_segments_from_intersection[intersection_id];
}

//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
std::vector<std::string> find_intersection_street_names(unsigned intersection_id){
    //fetch intersection Name from loaded data structure
    return intersections_Name[intersection_id];
}

//Returns true if you can get from intersection1 to intersection2 using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2){
    //corner case: intersection connected to itself
    if(intersection_id1 == intersection_id2)
        return true;
    // find directly connected intersections and check
    // if intersection id2 is included
    std::vector<unsigned> x = find_adjacent_intersections(intersection_id1);
    for(int i = 0; i < static_cast<int>(x.size()); i++)
        if(x[i] == intersection_id2)
            return true;
    
    //return false if not found
    return false;
}

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id){
    std::vector<unsigned> x;
    //find street segments connected to intersection
    std::vector<unsigned> y = find_intersection_street_segments(intersection_id);
    //set flags and temporary variable
    int tobeadded;
    bool add;
    //go through each street segment
    for(int i =0; i < static_cast<int>(y.size());i++){
        //reset flag and temp
        add = true;
        tobeadded = -1;
        //find if there is a path between the intersections
        if(getInfoStreetSegment(y[i]).from == static_cast<int>(intersection_id)){
            tobeadded = getInfoStreetSegment(y[i]).to;
        }
        else{
            if(getInfoStreetSegment(y[i]).oneWay == false)
                tobeadded = getInfoStreetSegment(y[i]).from;
        }
        
        //check if intersection has already been added
        for(int j =0; j< static_cast<int>(x.size()); j++){
            if(static_cast<int>(x[j]) == tobeadded){
                add = false;
            }
        }
        //add if flag was set
        if(add && tobeadded >= 0){
            x.push_back(tobeadded);
        }
    }
    return x;
}

//Returns all street segments for the given street
std::vector<unsigned> find_street_street_segments(unsigned street_id){
    return streets_segments[street_id];
}

//Returns all intersections along the a given street
std::vector<unsigned> find_all_street_intersections(unsigned street_id){
    return std::vector<unsigned>(intersections_from_street_id[street_id].begin(),
                                intersections_from_street_id[street_id].end());
}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
std::vector<unsigned> find_intersection_ids_from_street_ids(unsigned street_id1, 
                                                            unsigned street_id2){
            
    std::vector<unsigned> common_intersection_ids;
    std::vector<unsigned> street_intersections1 = find_all_street_intersections(street_id1);
    std::vector<unsigned> street_intersections2 = find_all_street_intersections(street_id2);
    
    //Loop through street 1's intersections
    for (unsigned i = 0; i < street_intersections1.size(); i++) {
        //If this street 1 intersection is also found as an intersection of
        //street 2 then add it to the vector of common intersections
        if (std::find(street_intersections2.begin(), street_intersections2.end(),
                street_intersections1[i]) != street_intersections2.end()) {
            
            common_intersection_ids.push_back(street_intersections1[i]);
        }
    }
    
    return common_intersection_ids;
    
}

//Returns the distance between two coordinates in meters
double find_distance_between_two_points(LatLon point1, LatLon point2){
    // Distance is found by projecting the lat/lon coordinates
    // to x,y coordinates and using the Pythagorean theorem    
    double lat1,lat2,lon1,lon2;
    double x1,x2,y1,y2;
    
    // Conversion to radians
    lat1 = point1.lat()*DEG_TO_RAD;
    lat2 = point2.lat()*DEG_TO_RAD;
    lon1 = point1.lon()*DEG_TO_RAD;
    lon2 = point2.lon()*DEG_TO_RAD;
    
    // Conversion to (x,y), convert lat/lon to radians
    // (x,y) = (lon*cos(lat_avg),lat)
    // lat_avg is average latitude between points
    double cos_lat_avg = cos((lat1+lat2)/2);
    x1 = lon1*cos_lat_avg;
    y1 = lat1;
    x2 = lon2*cos_lat_avg;
    y2 = lat2;
    
    // Compute distance using formula
    // d = R * sqrt( (y2-y1)^2 + (x2-x1)^2 )
    double dx = x2-x1;
    double dy = y2-y1;
    double d = EARTH_RADIUS_IN_METERS * sqrt(dy*dy+dx*dx);
    
    return d;
}

//Returns the length of the given street segment in meters
double find_street_segment_length(unsigned street_segment_id){
    // Get information about the street segment
    InfoStreetSegment segment = getInfoStreetSegment(street_segment_id);
    
    // Let lat/lon values of the endpoints of the street segment
    LatLon from = getIntersectionPosition(segment.from);
    LatLon to = getIntersectionPosition(segment.to);
    
    double length = 0;
    
    // If the street segment is straight, calculate straight distance
    // Otherwise, calculate distance between from and first curve point,
    // last curve point and to, and then distance between each curve point
    if(segment.curvePointCount == 0) 
        length = find_distance_between_two_points(from,to);
    else {
        length += find_distance_between_two_points( // From to first point
                        from,getStreetSegmentCurvePoint(0,street_segment_id)); 
        length += find_distance_between_two_points(  // last point to to
                        getStreetSegmentCurvePoint(
                                segment.curvePointCount-1,street_segment_id),to);
        // Go through all the curve points and find the distance between them
        for(int i=0; i<segment.curvePointCount-1; i++) {
            length += find_distance_between_two_points(
                            getStreetSegmentCurvePoint(i,street_segment_id),
                            getStreetSegmentCurvePoint(i+1,street_segment_id));
        }
    }
    
    return length;
}

//Returns the length of the specified street in meters
double find_street_length(unsigned street_id){
    // Get all street segments id
    std::vector<unsigned> segment_ids = find_street_street_segments(street_id);
    
    double length = 0;
    
    // Loop through all street segment ids and get the sum of their lengths
    for(std::vector<unsigned>::iterator it=segment_ids.begin(); 
            it!=segment_ids.end(); ++it) {
        length += street_segment_length[*it];
    }
    
    return length;
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
double find_street_segment_travel_time(unsigned street_segment_id){
    return segment_travel_time[street_segment_id];
}

//Returns the nearest point of interest to the given position
unsigned find_closest_point_of_interest(LatLon my_position){
    // R-Tree queries require a vector
    std::vector<std::pair<RTreeLatLon,POIIndex>> closest_poi;
    
    poi_rtree.query(bgi::nearest(RTreeLatLon(my_position),1),
                    std::back_inserter(closest_poi));
    
    return closest_poi[0].second;
}

//Returns the nearest intersection to the given position
unsigned find_closest_intersection(LatLon my_position){
    // Get num of intersections, indexing starts from 0 to num_intersections-1
    int num_intersections = getNumIntersections();
    
    // Keeps track of closest intersection and its distance
    IntersectionIndex closest_intersection = 0;
    double closest_dist = find_distance_between_two_points(
                                my_position,getIntersectionPosition(0));
    
    double dist;
    
    // Go through every intersection and find one with the shortest distance
    for(IntersectionIndex i=1; i<num_intersections; i++) {
        dist = find_distance_between_two_points(
                    my_position,getIntersectionPosition(i));
        
        if(dist<closest_dist) { // Update intersection if new closest is found
            closest_intersection = i;
            closest_dist = dist;
        }
    }
    
    return closest_intersection;
}

//Returns all street ids corresponding to street names that start with the given prefix
//The function should be case-insensitive to the street prefix. For example, 
//both "bloo" and "BloO" are prefixes to "Bloor Street East".
//If no street names match the given prefix, this routine returns an empty (length 0) 
//vector.
//You can choose what to return if the street prefix passed in is an empty (length 0) 
//string, but your program must not crash if street_prefix is a length 0 string.
std::vector<unsigned> find_street_ids_from_partial_street_name(std::string street_prefix){
    std::vector<unsigned> matching_street_ids;
    
    //If the street_prefix is a length 0 string then an empty vector is returned
    if(street_prefix.length() == 0){
        return matching_street_ids;
    }

    //Changing street name to lowercase to allow for case-insensitive
    //searching
    std::string lowercase_prefix = street_prefix;
    std::transform(lowercase_prefix.begin(), lowercase_prefix.end(), lowercase_prefix.begin(), ::tolower);
    
    //Use the TrieNode class function to find the corresponding street ids
    return street_names_trie.find_prefix_in_street_names(lowercase_prefix);
}
