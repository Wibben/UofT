#pragma once

#include <unordered_map>
#include <cmath>
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "draw_functions.h"
#include "find_in_range.h"
#include "directions.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
// For R-Trees
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bgi = boost::geometry::index;

void draw_main_canvas (ezgl::renderer &g);

//Converts lon to x coordinate for drawing on the map
double x_from_lon(double lon);
//Converts lat to y coordinate for drawing on the map
double y_from_lat(double lat);
//Converts x coordinate to lon
double lon_from_x(double x);
//Converts y coordinate to lat
double lat_from_y(double y);
//Return a point2d which is used for drawing functions
ezgl::point2d make_point2d(LatLon point);
//Calculates area of a polygon for use in drawing feature order
double polygon_area(std::vector<LatLon>);

//Changes background colour by filling a rectangle over the entire map area
void colour_background(int colour, ezgl::renderer &g);
//Draws rectangles at intersections
void draw_intersection(int intersection_id, ezgl::renderer &g);
//Draws lines in between curve points to draw a full street segment
void draw_street_segment(int street_segment_id, ezgl::renderer &g);
//Draws names of street segments
ezgl::point2d draw_street_segment_name(int street_segment_id, ezgl::renderer &g);
//Draw both polygon features and multi-segment line features
void draw_feature(int featureID, ezgl::renderer &g);
// Draws names of features
void draw_feature_name(int featureID, ezgl::renderer &g);
// Draws names of POIs
void draw_poi_name(int poiID, ezgl::renderer &g);
// Draws a marker at where the user clicked
void draw_marker(ezgl::renderer &g);
// Draws a circle indicating the area of List of POIs
void draw_list_of_pois_range(ezgl::renderer &g);
//Draws the path showing directions between two intersections
void draw_path_street_segment(int street_segment_id, ezgl::renderer &g);
void draw_path_endpoints(ezgl::renderer &g);

// Sets the positions for the marker's points
void set_marker_points(double mouse_x, double mouse_y, 
                       ezgl::canvas * canvas, bool reset_range);
// Sets center for the List of POI's range indicator to be same as mouse position
void set_poi_range_center();
// Sets the intersection to be highlighted
void set_highlighted_intersection_ID(int highlight);
void reset_highlighted_intersection_ID();

void reset_roads_drawn();

//Max/min lat/lon values to set up drawing area found in load_map()
extern double max_lat, min_lat, max_lon, min_lon;
// Mouse x,y position as per projection, set whenever mouse clicks on map
extern double mouse_x,mouse_y;
// Canvas width and height, set whenever a drawing occurs
extern double canvas_width,canvas_height;

extern bool* highlighted_intersection;

extern int colour_theme;

//Global data structures made in other files
extern std::vector<std::vector<LatLon>> feature_LatLon_from_feature_id;
extern std::vector<std::vector<ezgl::point2d>> feature_point2d_from_feature_id;
extern std::unordered_map<OSMID, unsigned> street_segments_from_OSMID;
extern std::vector<std::string> segment_type_from_segment_id;
extern std::unordered_map<std::string, std::string> segment_type_from_segment_name;
extern std::vector<std::pair<double, int>> feature_id_by_area;

extern std::vector<int> level_5_features;
extern std::vector<int> level_4_features;
extern std::vector<int> level_1_features;

extern std::vector<int> level_3_roads;
extern std::vector<int> level_2_roads;
extern std::vector<int> level_1_roads;

extern std::vector<ezgl::point2d> roads_drawn;

// For implementing an R-Tree for points of interest
class RTreeLatLon
{
public:
    RTreeLatLon() {}
    RTreeLatLon(LatLon ll) : point(ll.lat(),ll.lon()) {}
    RTreeLatLon(double lat, double lon) : point(lat,lon) {}
    
    double get_lat() const { return point.lat(); }
    double get_lon() const { return point.lon(); }
    void set_lat(double lat) { point = LatLon(lat,point.lon()); }
    void set_lon(double lon) { point = LatLon(point.lat(),lon); }
    
    LatLon point;
};

extern bgi::rtree<std::pair<RTreeLatLon,POIIndex>, bgi::quadratic<50> > poi_rtree;
extern std::vector<unsigned> shortest_path;