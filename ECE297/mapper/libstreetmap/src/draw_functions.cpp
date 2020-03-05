#include "draw_functions.h"
#include "callback_definitions.h"
#include "color.hpp"
#include <set>

BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(
        RTreeLatLon, float,
        boost::geometry::cs::spherical_equatorial<boost::geometry::degree>,
        get_lon, get_lat, set_lon, set_lat)

// Holds the drawing position of the map marker and POI range
double mouse_x,mouse_y,canvas_width,canvas_height;
double poi_range_x,poi_range_y;
double width_percent;

void draw_main_canvas (ezgl::renderer &g) {
    colour_background(colour_theme, g);
    
    ezgl::rectangle visible_world = g.get_visible_world();
    double max_x, min_x, max_width, width;
    max_x = x_from_lon(max_lon);
    min_x = x_from_lon(min_lon);
    
    //Width of the whole map
    max_width = std::abs(max_x-min_x);
    
    //Width of the current visible world
    width = std::abs(visible_world.right() - visible_world.left());
    
    width_percent = (width/max_width)*100;
    
    //Draw features
    if(width_percent >= 0 && width_percent <= 5){
        for(unsigned int i = 0; i < level_1_features.size(); i++){
            draw_feature(level_1_features[i], g);
        }
    }
    else if(width_percent > 5 && width_percent <= 50){
        for(unsigned int i = 0; i < level_4_features.size(); i++){
            draw_feature(level_4_features[i], g);
        }
    }
    else if(width_percent > 50){
        for(unsigned int i = 0; i < level_5_features.size(); i++){
            draw_feature(level_5_features[i], g);
        }
    }
    
    //Draw street segments and street segment names
    reset_roads_drawn();
    if(width_percent >= 0 && width_percent <= 5){
        for(unsigned int i = 0; i < level_1_roads.size(); i++){
            draw_street_segment(level_1_roads[i], g);
        }
    
        //do_draw_directions will be true when the directions button is used
        if(do_draw_directions){
            for(unsigned i = 0; i < shortest_path.size(); i++){
                draw_path_street_segment(shortest_path[i], g);
            }
            draw_path_endpoints(g);
        }
        
        // Draw the names on top of directions if applicable
        for(unsigned int i = 0; i < level_1_roads.size(); i++){
            roads_drawn[i] = draw_street_segment_name(level_1_roads[i], g);
        }
    }
    else if(width_percent > 5 && width_percent <= 55){
        for(unsigned int i = 0; i < level_2_roads.size(); i++){
            draw_street_segment(level_2_roads[i], g);
        }
    
        //do_draw_directions will be true when the directions button is used
        if(do_draw_directions){
            for(unsigned i = 0; i < shortest_path.size(); i++){
                draw_path_street_segment(shortest_path[i], g);
            }
            draw_path_endpoints(g);
        }
        
        // Draw the names on top of directions if applicable
        for(unsigned int i = 0; i < level_2_roads.size(); i++){
            if(i%3 == 0)
            roads_drawn[i] = draw_street_segment_name(level_2_roads[i], g);
        }
    }
    else if(width_percent > 55){
        for(unsigned int i = 0; i < level_3_roads.size(); i++){
            draw_street_segment(level_3_roads[i], g);
        }
    
        //do_draw_directions will be true when the directions button is used
        if(do_draw_directions){
            for(unsigned i = 0; i < shortest_path.size(); i++){
                draw_path_street_segment(shortest_path[i], g);
            }
            draw_path_endpoints(g);
        }
        
        // Draw the names on top of directions if applicable
        for(unsigned int i = 0; i < level_3_roads.size(); i++){
            roads_drawn[i] = draw_street_segment_name(level_3_roads[i], g);
        }
    }
    
    //Draw feature names
    for(int i=0; i<getNumFeatures(); i++) {
        draw_feature_name(i,g);
    }
    
    // Draw POI names 
    if(width_percent < 20) {
        for(int i=0; i<getNumPointsOfInterest(); i++) {
            draw_poi_name(i,g);
        }
    }
    
    // Draw the highlighted intersections
    for(int i = 0; i < getNumIntersections(); i++) {
        draw_intersection(i, g);
    }
    
    draw_list_of_pois_range(g);
    draw_marker(g);
}

//Converts lon to x coordinate for drawing on the map
double x_from_lon(double lon){
    lon = lon*DEG_TO_RAD;
    double cos_lat_avg = cos(DEG_TO_RAD*(max_lat+min_lat)/2);
    return lon*cos_lat_avg;
}

//Converts lat to y coordinate for drawing on the map
double y_from_lat(double lat){
    return lat*DEG_TO_RAD;
}

//Converts x coordinate to lon
double lon_from_x(double x){
    double cos_lat_avg = cos(DEG_TO_RAD*(max_lat+min_lat)/2);
    return x/(cos_lat_avg*DEG_TO_RAD);
}

//Converts y coordinate to lat
double lat_from_y(double y){
    return y/DEG_TO_RAD;
}

//Return a point2d which is used for drawing functions
ezgl::point2d make_point2d(LatLon point) {
    double tempX = x_from_lon(point.lon());
    double tempY = y_from_lat(point.lat());
    ezgl::point2d tempPoint(tempX, tempY);
    return tempPoint;
}

// Sets the positions for the marker's points
void set_marker_points(double x, double y, 
                       ezgl::canvas * canvas, bool reset_range)
{   
    mouse_x = x;
    mouse_y = y;
    canvas_width = canvas->width();
    canvas_height = canvas->height();
    // When user clicks on a new location set the List of POIs range to 0
    if(reset_range) find_poi_range = 0;
}

// Sets center for the List of POI's range indicator to be same as mouse position
void set_poi_range_center()
{
    poi_range_x = mouse_x;
    poi_range_y = mouse_y;
}

//Calculates area of a polygon for use in drawing feature order
double polygon_area(std::vector<LatLon> points){
    double x1, x2, y1, y2, sum;
    sum = 0;

    x1 = x_from_lon(points[0].lon());
    x2 = x_from_lon(points.back().lon());
    y1 = y_from_lat(points[0].lat());
    y2 = y_from_lat(points.back().lat());
    
    if ((x1 == x2) && (y1 == y2)) {
        for(unsigned int i = 0; i < points.size()-1; i++){
            
            x1 = x_from_lon(points[i].lon());
            x2 = x_from_lon(points[i+1].lon());
            y1 = y_from_lat(points[i].lat());
            y2 = y_from_lat(points[i+1].lat());
            
            sum = sum +(x1*y2 - x2*y1);
        }
        return std::abs(sum*0.5);
    }
    else{
        return 0;
    }
}

//Changes background colour by filling a rectangle over the entire map area
void colour_background(int colour, ezgl::renderer &g){
    ezgl::rectangle visible_world = g.get_visible_world();
    ezgl::point2d start = visible_world.top_left();
    ezgl::point2d end = visible_world.bottom_right();
    
    //Pick background colour
    if(colour == 0){
        //g.set_color(239,238,236,255);
        g.set_color(245,245,245,255);
    }
    else if(colour == 1){
        g.set_color(36,47,62,255);
    }
    g.fill_rectangle(start, end);
}

void draw_intersection(int intersection_id, ezgl::renderer &g){
    // Only draw the highlighted intersections
    // Highlight the selected intersection in red
    if(highlighted_intersection[intersection_id]) {
        g.set_color(255,0,0,255);
        g.set_coordinate_system(ezgl::SCREEN);
    
        // Draw a rectangle CENTERED around the intersection
        LatLon tempLatLon = getIntersectionPosition(intersection_id);
        ezgl::rectangle screen = g.get_visible_world();
        double inter_x = 
            (x_from_lon(tempLatLon.lon())-screen.left())/(screen.width()) * canvas_width;
        double inter_y = 
            (screen.top()-y_from_lat(tempLatLon.lat()))/(screen.height()) * canvas_height;
        
        ezgl::point2d tempPoint(inter_x-4,inter_y-4);
        g.fill_rectangle(tempPoint,8,8);
        
        g.set_coordinate_system(ezgl::WORLD);
    }
}

//Draws lines in between curve points to draw a full street segment
void draw_street_segment(int street_segment_id, ezgl::renderer &g) {
    InfoStreetSegment ss_info = getInfoStreetSegment(street_segment_id);
    ezgl::point2d tempPoint1(0, 0);
    ezgl::point2d tempPoint2(0, 0);
    
    std::string segment_type = segment_type_from_segment_id[street_segment_id];
    if (segment_type == "") {
        std::string street_name = getStreetName(ss_info.streetID);
        segment_type = segment_type_from_segment_name[street_name];
    }
    
    g.set_line_width(5);
    // When the zoom gets too close, white roads can't be seen
    g.set_color(250,250,250,255);
    if(colour_theme == 1){
        g.set_color(56,65,78,255);
    }
    
    if(width_percent>8 && width_percent<25) g.set_line_width(3);
    
    if(segment_type == "motorway" || 
       segment_type == "motorway_link" ||
       segment_type == "primary" ||
       segment_type == "primary_link" ||
       segment_type == "trunk" ||
       segment_type == "trunk_link"){
        if(colour_theme == 0){
            g.set_color(255,235,161,255);
        }
        else{
            g.set_color(116,104,85,255);
        }
        g.set_line_width(5);
    }
    
    //Make a point2d for the FROM intersection.
    LatLon tempLatLon = getIntersectionPosition(ss_info.from);
    tempPoint1 = make_point2d(tempLatLon);
    
    //If there are no curve points then draw a line between the FROM intersection
    //and the TO intersection.
    if (ss_info.curvePointCount == 0) {
        //Make a point2d for the TO intersection.
        tempLatLon = getIntersectionPosition(ss_info.to);
        tempPoint2 = make_point2d(tempLatLon);
        //Draw using the two made point2ds
        g.draw_line(tempPoint1, tempPoint2);
    }
    //If there are curve points, draw lines between them
    else {
        //Draw a line between the FROM intersection and the first curve point
        tempPoint2 = make_point2d(getStreetSegmentCurvePoint(0, street_segment_id));
        g.draw_line(tempPoint1, tempPoint2);
        
        //Draw lines between the curve points
        for (int j = 0; j < ss_info.curvePointCount - 1; j++) {
            tempPoint1 = make_point2d(getStreetSegmentCurvePoint(j, street_segment_id));
            tempPoint2 = make_point2d(getStreetSegmentCurvePoint(j + 1, street_segment_id));
            g.draw_line(tempPoint1, tempPoint2);
        }
        
        //Draw a line between the last curve point and the TO intersection
        tempPoint1 = make_point2d(getStreetSegmentCurvePoint(ss_info.curvePointCount - 1, street_segment_id));
        tempLatLon = getIntersectionPosition(ss_info.to);
        tempPoint2 = make_point2d(tempLatLon);
        g.draw_line(tempPoint1, tempPoint2);
    }
    
    g.set_line_width(0);
}

//Draws names of street segments
ezgl::point2d draw_street_segment_name(int street_segment_id, ezgl::renderer &g){
    bool draw_name = false;
    //get information about street segment
    InfoStreetSegment info = getInfoStreetSegment(street_segment_id);
    ezgl::point2d from = make_point2d(getIntersectionPosition(info.from));
    ezgl::point2d to = make_point2d(getIntersectionPosition(info.to));
    std::string name = getStreetName(info.streetID);
    
    // Set whether to draw or not based on feature visibility vs current world
    ezgl::rectangle screen = g.get_visible_world();
    double length = find_distance_between_two_points(getIntersectionPosition(info.from),getIntersectionPosition(info.to));
    LatLon top_left(lat_from_y(screen.top()),lon_from_x(screen.left()));
    LatLon bottom_right(lat_from_y(screen.bottom()),lon_from_x(screen.right()));
    double screen_diagonal = find_distance_between_two_points(top_left,bottom_right);
    
    //check is segment is long enough to display
    bool long_enough = (length >= 0.1 * screen_diagonal);
    //check if segment is in screen
    bool visible = (from.x >= screen.left() && from.x <= screen.right() && 
                    from.y >= screen.bottom() && from.y <= screen.top()) ||
                   (to.x >= screen.left() && to.x <= screen.right() && 
                    to.y >= screen.bottom() && to.y <= screen.top());
       
    //checks for names that dont need to be displayed
    if(visible && long_enough)
        if(name.find("Ramp") == std::string::npos)
            if(name.find("Exit") == std::string::npos)
                if(name.find("To ") == std::string::npos)
                    if(name.find("<unknown>") == std::string::npos)
                        if(name.find("Switchback") == std::string::npos)
                            if(name.find("Switchover") == std::string::npos)
                                if(name.find("Vehicle Lane") == std::string::npos)
                                    if(name.find("Exchange Lane") == std::string::npos)
                                        draw_name = true;
                        
    //if all checks were passed, draw street segment name
    if(draw_name) {
        //set point to be printed from
        ezgl::point2d name_point((from.x + to.x)/2,(from.y + to.y)/2);
    
        //check if curved
        if(info.curvePointCount > 0){
            //find longest straight distance on curve to display name
            double distance = 0;
            
            //assume first segment is the longest to compare
            distance = find_distance_between_two_points(getIntersectionPosition(info.from),getStreetSegmentCurvePoint(0, street_segment_id));
            to = make_point2d(getStreetSegmentCurvePoint(0, street_segment_id));
            //check all middle segments
            for(int i = 0; i < info.curvePointCount - 1; i++){
                if(distance <= find_distance_between_two_points(getStreetSegmentCurvePoint(i, street_segment_id),getStreetSegmentCurvePoint(i+1, street_segment_id))){
                    distance = find_distance_between_two_points(getStreetSegmentCurvePoint(i, street_segment_id),getStreetSegmentCurvePoint(i+1, street_segment_id));
                    from = make_point2d(getStreetSegmentCurvePoint(i, street_segment_id));
                    to = make_point2d(getStreetSegmentCurvePoint(i+1, street_segment_id));
                }
            }
            //check last segment
            if(distance <= find_distance_between_two_points(getStreetSegmentCurvePoint(info.curvePointCount-1, street_segment_id),getIntersectionPosition(info.to))){
                    distance = find_distance_between_two_points(getStreetSegmentCurvePoint(info.curvePointCount-1, street_segment_id),getIntersectionPosition(info.to));
                    from = make_point2d(getStreetSegmentCurvePoint(info.curvePointCount-1, street_segment_id));
                    to = make_point2d(getIntersectionPosition(info.to));
                }
            //if distance is long enough, draw on segment
            if(distance >= 0.025 *screen_diagonal){
                name_point.x = (from.x + to.x)/2;
                name_point.y = (from.y + to.y)/2;
            } else {
                //if no segment is long enough find appropriate formatting
                if(info.curvePointCount == 1){
                    //draw on midpoint and set angle using intersection coordinates
                    name_point = make_point2d(getStreetSegmentCurvePoint(0, street_segment_id));
                    from = make_point2d(getIntersectionPosition(info.from));
                    to = make_point2d(getIntersectionPosition(info.to));
                }
                else if (info.curvePointCount == 2){
                    //draw on midpoint and set angle using street segment curve points
                    from = make_point2d(getStreetSegmentCurvePoint(0, street_segment_id));
                    to = make_point2d(getStreetSegmentCurvePoint(1, street_segment_id));
                    name_point.x = (from.x + to.x)/2;
                    name_point.y = (from.y + to.y)/2;
                }
                else if(info.curvePointCount > 2){
                    //draw on mid point and find angle with surrounding street segment curve points
                    int middle = info.curvePointCount / 2;
                    name_point = make_point2d(getStreetSegmentCurvePoint(middle, street_segment_id));
                    from = make_point2d(getStreetSegmentCurvePoint(middle-1, street_segment_id));
                    to = make_point2d(getStreetSegmentCurvePoint(middle+1, street_segment_id));
                }
            }
        }
        //Find orientation angle
        double angle;
            if(from.x == to.x) angle = 90;
            else if (from.y == to.y) angle = 0;
            else angle = atan((to.y-from.y)/(to.x-from.x)) / DEG_TO_RAD;
                
        //set orientation
        g.set_text_rotation(angle);
        
        //set arrow color
        if(colour_theme == 0){
            //g.set_color(255,235,161,255);
            g.set_color(139,151,170,127);
        }
        else{
            g.set_color(116,104,85,255);
        }
        
        //check if one way
        double theta = angle;
        double theta_1 = 150;
        double theta_2 = 180 - theta_1;
        double l = width_percent/400000;
        if(info.oneWay){
            if(((to.y-from.y) > 0 && (to.x-from.x) < 0) || ((to.y-from.y) < 0 && (to.x-from.x) < 0)){
               // name = "<- " + name;
                theta += 180;
            } //else 
               // name = name + " ->";
            
            // The marker is a triangle
            std::vector<ezgl::point2d> marker_points;
            marker_points.push_back(ezgl::point2d(name_point.x,name_point.y));
            marker_points.push_back(ezgl::point2d(name_point.x + l*cos((theta + theta_1) * DEG_TO_RAD), name_point.y + l*sin((theta + theta_1) * DEG_TO_RAD)));
            marker_points.push_back(ezgl::point2d(name_point.x + l*cos((theta + theta_1 + 2*theta_2) * DEG_TO_RAD), name_point.y + l*sin((theta + theta_1 + 2*theta_2) * DEG_TO_RAD)));
            // draw one way marker
            g.fill_poly(marker_points);
            
        }
        //set text color
        if(colour_theme == 0)
            g.set_color(0,0,0,255);
        else
            g.set_color(255,255,255,255);
        
        //check if too close to other strings
        double min_distance = -1;
        for(int j = 0; j < getNumStreetSegments(); j++){
            if(roads_drawn[j].x != -1)
                if(min_distance  > (roads_drawn[j].x - name_point.x)*(roads_drawn[j].x - name_point.x) + 
                    (roads_drawn[j].y - name_point.y)*(roads_drawn[j].y - name_point.y))
                    min_distance = (roads_drawn[j].x - name_point.x)*(roads_drawn[j].x - name_point.x) + 
                    (roads_drawn[j].y - name_point.y)*(roads_drawn[j].y - name_point.y); 
        }
        if(min_distance == -1 || min_distance > 0.2 * screen_diagonal){
            //draw text
            g.draw_text(name_point,name);
            //reset formatting
            g.set_text_rotation(0);
            return name_point;
        }   
        
    }
    ezgl::point2d notdrawn(-1,-1);
    return notdrawn;
}

//Draw both polygon features and multi-segment line features
void draw_feature(int featureID, ezgl::renderer &g) {
    //Value for feature type:
    //Unknown = 0, Park = 1, Beach = 2, Lake = 3, River = 4
    //Island = 5, Building = 6, Greenspace = 7, Golfcourse = 8, Stream = 9
    
    //Select colour based on feature type
    if (getFeatureType(featureID) == Park || 
        getFeatureType(featureID) == Greenspace || 
        getFeatureType(featureID) == Golfcourse) {
        g.set_color(192,236,174,255);
    } else if (getFeatureType(featureID) == Lake || 
               getFeatureType(featureID) == River || 
               getFeatureType(featureID) == Stream) {
        g.set_color(170,218,255,255);
    } else if (getFeatureType(featureID) == Building) {
        g.set_color(215,218,221,255);
    } else if (getFeatureType(featureID) == Beach) {
        g.set_color(250,242,199,255);
    } else if(getFeatureType(featureID) == Island){
        //g.set_color(239,238,236,255);
        g.set_color(245,245,245,255);
    }
    else{
        //g.set_color(239,238,236,255);
        g.set_color(245,245,245,255);
    }
    
    if(colour_theme == 1) {
        if (getFeatureType(featureID) == Park ||
                getFeatureType(featureID) == Greenspace ||
                getFeatureType(featureID) == Golfcourse) {
            g.set_color(38, 60, 63, 255);
        } else if (getFeatureType(featureID) == Lake ||
                getFeatureType(featureID) == River ||
                getFeatureType(featureID) == Stream) {
            g.set_color(23, 38, 60, 255);
        } else if (getFeatureType(featureID) == Building) {
            g.set_color(50,60,70,255);
        } else if (getFeatureType(featureID) == Beach) {
            g.set_color(36,47,62,255);
        } else if (getFeatureType(featureID) == Island) {
            //g.set_color(239,238,236,255);
            g.set_color(36,47,62,255);
        } else {
            //g.set_color(239,238,236,255);
            g.set_color(36,47,62,255);
        }
    }
    
    g.set_line_width(0);
    
    if(feature_LatLon_from_feature_id[featureID].size() > 1){
        //If the first and last points are the same, the feature is a polygon.
        //If not, the feature is a multi-segment line
        double lon1, lon2, lat1, lat2;
        lon1 = feature_LatLon_from_feature_id[featureID][0].lon();
        lat1 = feature_LatLon_from_feature_id[featureID][0].lat();
        lon2 = feature_LatLon_from_feature_id[featureID].back().lon();
        lat2 = feature_LatLon_from_feature_id[featureID].back().lat();
        
        //If the feature is a polygon
        if ((lon1 == lon2) && (lat1 == lat2)) {
            g.fill_poly(feature_point2d_from_feature_id[featureID]);
        }
        //If the feature is multi-segment line
        else {
            for (int i = 0; i < getFeaturePointCount(featureID)-1; i++) {
                ezgl::point2d tempPoint1 = make_point2d(feature_LatLon_from_feature_id[featureID][i]);
                ezgl::point2d tempPoint2 = make_point2d(feature_LatLon_from_feature_id[featureID][i+1]);
                g.draw_line(tempPoint1, tempPoint2);
            }
        }
    }
    g.set_line_width(0);
}

// Draws names of features
void draw_feature_name(int featureID, ezgl::renderer &g)
{
    bool draw_name = false;
    // Select colour based on feature type
    if (getFeatureType(featureID) == Park || 
        getFeatureType(featureID) == Greenspace || 
        getFeatureType(featureID) == Golfcourse) {
        g.set_color(134,165,122,255);
    } else if (getFeatureType(featureID) == Lake || 
               getFeatureType(featureID) == River || 
               getFeatureType(featureID) == Stream) {
        g.set_color(119,153,179,255);
    } else if (getFeatureType(featureID) == Building ||
               getFeatureType(featureID) == Beach ||
               getFeatureType(featureID) == Island) {
        g.set_color(151,153,155,255);
    } else {
        g.set_color(172,172,172,255);
    }
    g.set_line_width(0);
    
    // Set whether to draw or not based on feature size vs current world
    ezgl::rectangle screen = g.get_visible_world();
    
    if(getFeatureType(featureID) == River || 
       getFeatureType(featureID) == Stream) {
        double length = 0;
        LatLon top_left(lat_from_y(screen.top()),lon_from_x(screen.left()));
        LatLon bottom_right(lat_from_y(screen.bottom()),lon_from_x(screen.right()));
        double screen_diagonal = 
            find_distance_between_two_points(top_left,bottom_right);
        
        // Calculate the length of the feature
        LatLon p1,p2;
        for(int i=0; i<getFeaturePointCount(featureID)-1; i++) {
            p1 = feature_LatLon_from_feature_id[featureID][i];
            p2 = feature_LatLon_from_feature_id[featureID][i+1];
            length += find_distance_between_two_points(p1,p2);
        }
        
        // Draw if length of river/stream is 10% of screen diagonal
        if(length > 0.15*screen_diagonal) draw_name = true;
    } else {
        // polygon_area uses the mapped x,y positions to calculate area
        double area = polygon_area(feature_LatLon_from_feature_id[featureID]);
        
        // Draw if area of feature is 0.2% of screen area
        if(area > 0.002*screen.area()) draw_name = true;
    }
    
    if((getFeatureType(featureID) == River || 
        getFeatureType(featureID) == Stream) && draw_name &&
        getFeatureName(featureID) != "<noname>") {
        // Draw the name at approximately the "middle" point
        int middle = getFeaturePointCount(featureID)/2;
        ezgl::point2d name_point = feature_point2d_from_feature_id[featureID][middle];
        
        g.draw_text(name_point, getFeatureName(featureID));
    } else if(draw_name && getFeatureName(featureID) != "<noname>") {
        // Find the centroid and place the name there, method is described in:
        // https://www.seas.upenn.edu/~sys502/extra_materials/Polygon%20Area%20and%20Centroid.pdf
        ezgl::point2d centroid(0,0);
        double area = 0;
        ezgl::point2d p1(0,0),p2(0,0);
        for(int i=0; i<getFeaturePointCount(featureID)-1; i++) {
            p1 = feature_point2d_from_feature_id[featureID][i];
            p2 = feature_point2d_from_feature_id[featureID][i+1];
            
            centroid.x += (p1.x+p2.x)*(p1.x*p2.y-p2.x*p1.y);
            centroid.y += (p1.y+p2.y)*(p1.x*p2.y-p2.x*p1.y);
            
            area += (p1.x*p2.y-p2.x*p1.y);
        }
        area /= 2;
        centroid.x /= (6*area);
        centroid.y /= (6*area);
        // Draw the name of the feature
        g.draw_text(centroid, getFeatureName(featureID));
    }
}

// Draws names of POIs
// Only draw the POI if the distance between it and the next closest
// POI is greater than 5% of screen diagonal
void draw_poi_name(int poiID, ezgl::renderer &g)
{
    // Set the colour
    g.set_color(191,87,0,255);
    
    // Getting length of screen diagonal in terms of lat/lon coordinates
    ezgl::rectangle screen = g.get_visible_world();
    LatLon top_left(lat_from_y(screen.top()),lon_from_x(screen.left()));
    LatLon bottom_right(lat_from_y(screen.bottom()),lon_from_x(screen.right()));
    double screen_diagonal = find_distance_between_two_points(top_left,bottom_right);
    
    // Find the distance of the second closest POI, if the distance is
    // greater than 5% of screen diagonal, the name can be drawn
    // R-Tree queries require a vector
    std::vector<std::pair<RTreeLatLon,POIIndex>> closest_poi;
    poi_rtree.query(
        bgi::nearest(RTreeLatLon(getPointOfInterestPosition(poiID)),2),
        std::back_inserter(closest_poi));
    
    double distance = find_distance_between_two_points(
        getPointOfInterestPosition(poiID),
        getPointOfInterestPosition(closest_poi[0].second));
    if(distance > 0.05*screen_diagonal) {
        ezgl::point2d poi_point = make_point2d(getPointOfInterestPosition(poiID));
        g.draw_text(poi_point, getPointOfInterestName(poiID));
    }
}

// Draws a marker at where the user clicked
void draw_marker(ezgl::renderer &g)
{   
    // Map the point the mouse clicked to the correct absolute point on the
    // screen: Take the ratio of the mouse position to the visible screen 
    // size and multiply by the current canvas dimensions
    ezgl::rectangle screen = g.get_visible_world();
    double marker_x = 
        (mouse_x-screen.left())/(screen.width()) * canvas_width;
    double marker_y = 
        (screen.top()-mouse_y)/(screen.height()) * canvas_height;
    
    // The marker is a triangle
    std::vector<ezgl::point2d> marker_points;
    marker_points.push_back(ezgl::point2d(marker_x,marker_y));
    marker_points.push_back(ezgl::point2d(marker_x+16,marker_y-30));
    marker_points.push_back(ezgl::point2d(marker_x-16,marker_y-30));
    
    // Draw marker
    g.set_coordinate_system(ezgl::SCREEN);
    g.set_color(0,255,0,255);
    // Bottom half of marker
    g.fill_poly(marker_points);
    // Top half of marker
    g.fill_arc(ezgl::point2d(marker_x,marker_y-30),16,0,180);
    // Center of marker
    //g.set_color(181,101,29,255);
    //g.fill_arc(ezgl::point2d(marker_x,marker_y-30),8,0,360);
    // Outline marker
    g.set_color(127,127,127,255);
    g.draw_line(marker_points[0],marker_points[1]);
    g.draw_line(marker_points[0],marker_points[2]);
    g.draw_arc(ezgl::point2d(marker_x,marker_y-30),16,0,180);
    //g.draw_arc(ezgl::point2d(marker_x,marker_y-30),8,0,360);
    g.set_coordinate_system(ezgl::WORLD);
}

// Draws a circle indicating the area of List of POIs
void draw_list_of_pois_range(ezgl::renderer &g)
{
    // Convert the radius to pixels
    // Uses the conversion 1m = 0.00000016 in x,y
    // Found experimentally...
    double radius = find_poi_range*0.00000016;
    
    // Draw a semi-transparent blue circle with a darker border
    g.set_color(135,206,235,127);
    g.fill_arc(ezgl::point2d(poi_range_x,poi_range_y),radius,0,360);
    g.set_color(127, 0, 255,127);
    g.set_line_width(2);
    g.draw_arc(ezgl::point2d(poi_range_x,poi_range_y),radius,0,360);
    
    g.set_line_width(0);
}

void set_highlighted_intersection_ID(int highlight)
{
    highlighted_intersection[highlight] = true;
}

void reset_highlighted_intersection_ID(){
    for(int i=0; i< getNumIntersections();i++)
        highlighted_intersection[i] = false;
}

void reset_roads_drawn(){
    ezgl::point2d notdrawn(-1,-1);
    for(int i = 0; i < getNumStreetSegments(); i++){
        roads_drawn[i] = notdrawn;
    }
}

//Draws the path showing directions between two intersections
void draw_path_street_segment(int street_segment_id, ezgl::renderer &g) {
    InfoStreetSegment ss_info = getInfoStreetSegment(street_segment_id);
    ezgl::point2d tempPoint1(0, 0);
    ezgl::point2d tempPoint2(0, 0);
    
    g.set_line_width(5);
    g.set_color(0, 179, 253, 255);
    g.set_line_cap(ezgl::line_cap::round);
    
    if(width_percent>8 && width_percent<25) g.set_line_width(3);
    
    if(segment_type_from_segment_id[street_segment_id] == "motorway" || 
       segment_type_from_segment_id[street_segment_id] == "motorway_link"){
        g.set_line_width(5);
    }
    
    //Make a point2d for the FROM intersection.
    LatLon tempLatLon = getIntersectionPosition(ss_info.from);
    tempPoint1 = make_point2d(tempLatLon);
    
    //If there are no curve points then draw a line between the FROM intersection
    //and the TO intersection.
    if (ss_info.curvePointCount == 0) {
        //Make a point2d for the TO intersection.
        tempLatLon = getIntersectionPosition(ss_info.to);
        tempPoint2 = make_point2d(tempLatLon);
        //Draw using the two made point2ds
        g.draw_line(tempPoint1, tempPoint2);
    }
    //If there are curve points, draw lines between them
    else {
        //Draw a line between the FROM intersection and the first curve point
        tempPoint2 = make_point2d(getStreetSegmentCurvePoint(0, street_segment_id));
        g.draw_line(tempPoint1, tempPoint2);
        
        //Draw lines between the curve points
        for (int j = 0; j < ss_info.curvePointCount - 1; j++) {
            tempPoint1 = make_point2d(getStreetSegmentCurvePoint(j, street_segment_id));
            tempPoint2 = make_point2d(getStreetSegmentCurvePoint(j + 1, street_segment_id));
            g.draw_line(tempPoint1, tempPoint2);
        }
        
        //Draw a line between the last curve point and the TO intersection
        tempPoint1 = make_point2d(getStreetSegmentCurvePoint(ss_info.curvePointCount - 1, street_segment_id));
        tempLatLon = getIntersectionPosition(ss_info.to);
        tempPoint2 = make_point2d(tempLatLon);
        g.draw_line(tempPoint1, tempPoint2);
    }
    
    g.set_line_width(0);
}

// Draws the endpoints 
void draw_path_endpoints(ezgl::renderer &g)
{
    g.set_coordinate_system(ezgl::SCREEN);
    double inter_x,inter_y;
    ezgl::rectangle screen = g.get_visible_world();
    LatLon tempLatLon;
    
    // Draw a circle filled with white around the FROM intersection
    tempLatLon = getIntersectionPosition(from_intersection);
    inter_x = 
        (x_from_lon(tempLatLon.lon())-screen.left())/(screen.width()) * canvas_width;
    inter_y = 
        (screen.top()-y_from_lat(tempLatLon.lat()))/(screen.height()) * canvas_height;
    
    // Draw a black circle with a white center
    g.set_color(0,0,0);
    g.fill_arc(ezgl::point2d(inter_x,inter_y),6,0,360);
    g.set_color(255,255,255);
    g.fill_arc(ezgl::point2d(inter_x,inter_y),4,0,360);
    
    // Draw a circle with white fill and black center around the TO intersection
    tempLatLon = getIntersectionPosition(to_intersection);
    inter_x = 
        (x_from_lon(tempLatLon.lon())-screen.left())/(screen.width()) * canvas_width;
    inter_y = 
        (screen.top()-y_from_lat(tempLatLon.lat()))/(screen.height()) * canvas_height;
    
    // Draw a black circle with white filling and another black center
    g.set_color(0,0,0);
    g.fill_arc(ezgl::point2d(inter_x,inter_y),6,0,360);
    g.set_color(255,255,255);
    g.fill_arc(ezgl::point2d(inter_x,inter_y),4,0,360);
    g.set_color(0,0,0);
    g.fill_arc(ezgl::point2d(inter_x,inter_y),2,0,360);
    
    g.set_coordinate_system(ezgl::WORLD);
}
