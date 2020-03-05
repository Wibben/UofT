#include "m3.h"
#include "StreetsDatabaseAPI.h"
#include "draw_functions.h"
#include "directions.h"

//A vector with x and y components used to find turn types. This is not std::vector
struct direction_vector{
    double x;
    double y;
};

// Returns the turn type between two given segments.
// street_segment1 is the incoming segment and street_segment2 is the outgoing
// one.
// If the two street segments do not intersect, turn type is NONE.
// Otherwise if the two segments have the same street ID, turn type is 
// STRAIGHT.  
// If the two segments have different street ids, turn type is LEFT if 
// going from street_segment1 to street_segment2 involves a LEFT turn 
// and RIGHT otherwise.  Note that this means that even a 0-degree turn
// (same direction) is considered a RIGHT turn when the two street segments
// have different street IDs.
TurnType find_turn_type(unsigned street_segment1, unsigned street_segment2) {
    InfoStreetSegment ss_info1 = getInfoStreetSegment(street_segment1);
    InfoStreetSegment ss_info2 = getInfoStreetSegment(street_segment2);
    double temp1, temp2, delta;
    
    //Two vectors (math vectors not std::vector) will be made that will be used
    //to determine if a right or left turn is being made
    direction_vector incoming, outgoing;
    
    if(ss_info1.streetID == ss_info2.streetID){
        return TurnType::STRAIGHT;
    }
    
    //If the FROM intersection of street_segment1 is the shared intersection
    if(ss_info1.from == ss_info2.from || ss_info1.from == ss_info2.to){
        //If street_segment1 has curve points then the incoming vector will
        //be from curve point 0 to the FROM intersection
        if(ss_info1.curvePointCount != 0){
            temp1 = x_from_lon(getStreetSegmentCurvePoint(0, street_segment1).lon());
            temp2 = x_from_lon(getIntersectionPosition(ss_info1.from).lon());
            incoming.x = temp2 - temp1;
            temp1 = y_from_lat(getStreetSegmentCurvePoint(0, street_segment1).lat());
            temp2 = y_from_lat(getIntersectionPosition(ss_info1.from).lat());
            incoming.y = temp2 - temp1;
        }
        //If street_segment1 does NOT have curve points then the incoming vector 
        //will be from the TO intersection to the FROM intersection
        else{
            temp1 = x_from_lon(getIntersectionPosition(ss_info1.to).lon());
            temp2 = x_from_lon(getIntersectionPosition(ss_info1.from).lon());
            incoming.x = temp2 - temp1;
            temp1 = y_from_lat(getIntersectionPosition(ss_info1.to).lat());
            temp2 = y_from_lat(getIntersectionPosition(ss_info1.from).lat());
            incoming.y = temp2 - temp1;
        }
        
        if(ss_info2.curvePointCount != 0){
            //If street_segment2 has curve points and the shared intersection is
            //the FROM intersection then the outgoing vector will be from the 
            //FROM intersection to curve point 0
            if(ss_info1.from == ss_info2.from){
                temp1 = x_from_lon(getIntersectionPosition(ss_info2.from).lon());
                temp2 = x_from_lon(getStreetSegmentCurvePoint(0, street_segment2).lon());
                outgoing.x = temp2 - temp1;
                temp1 = y_from_lat(getIntersectionPosition(ss_info2.from).lat());
                temp2 = y_from_lat(getStreetSegmentCurvePoint(0, street_segment2).lat());
                outgoing.y = temp2 - temp1;
            }
            //If street_segment2 has curve points and the shared intersection is
            //the TO intersection then the outgoing vector will be from the 
            //TO intersection to the last curve point
            else if(ss_info1.from == ss_info2.to){
                temp1 = x_from_lon(getIntersectionPosition(ss_info2.to).lon());
                temp2 = x_from_lon(getStreetSegmentCurvePoint(
                        ss_info2.curvePointCount-1, street_segment2).lon());
                outgoing.x = temp2 - temp1;
                temp1 = y_from_lat(getIntersectionPosition(ss_info2.to).lat());
                temp2 = y_from_lat(getStreetSegmentCurvePoint(
                        ss_info2.curvePointCount-1, street_segment2).lat());
                outgoing.y = temp2 - temp1;
            }
        }
        //If street_segment2 does NOT have curve points the outgoing vector will
        //be from the street_segment1's FROM intersection to street_segment2's
        //intersection that is not the same as street_segment1's FROM intersection
        else{
            temp1 = x_from_lon(getIntersectionPosition(ss_info1.from).lon());
            temp2 = x_from_lon(getIntersectionPosition(
                    get_other_intersection(street_segment2, ss_info1.from)).lon());
            outgoing.x = temp2 - temp1;
            temp1 = y_from_lat(getIntersectionPosition(ss_info1.from).lat());
            temp2 = y_from_lat(getIntersectionPosition(
                    get_other_intersection(street_segment2, ss_info1.from)).lat());
            outgoing.y = temp2 - temp1;
            
        }
        
        //The two vectors made allow us to use the following formula to calculate
        //delta which can determine if a right or left turn is being made
        outgoing.y *= -1;
        delta = (incoming.x * outgoing.y) + (incoming.y * outgoing.x);
        if(delta >= 0){
            return TurnType::RIGHT;
        }
        else{
            return TurnType::LEFT;
        }
        
    }
    //If the TO intersection of street_segment 1 is the shared intersection
    else if(ss_info1.to == ss_info2.from || ss_info1.to == ss_info2.to) {
        //If street_segment1 has curve points then the incoming vector will
        //be from the last curve point to the TO intersection
        if(ss_info1.curvePointCount != 0){
            temp1 = x_from_lon(getStreetSegmentCurvePoint(
                    ss_info1.curvePointCount-1, street_segment1).lon());
            temp2 = x_from_lon(getIntersectionPosition(ss_info1.to).lon());
            incoming.x = temp2 - temp1;
            temp1 = y_from_lat(getStreetSegmentCurvePoint(
                    ss_info1.curvePointCount-1, street_segment1).lat());
            temp2 = y_from_lat(getIntersectionPosition(ss_info1.to).lat());
            incoming.y = temp2 - temp1;
        }
        //If street_segment1 does NOT have curve points then the incoming vector 
        //will be from the FROM intersection to the TO intersection
        else{
            temp1 = x_from_lon(getIntersectionPosition(ss_info1.from).lon());
            temp2 = x_from_lon(getIntersectionPosition(ss_info1.to).lon());
            incoming.x = temp2 - temp1;
            temp1 = y_from_lat(getIntersectionPosition(ss_info1.from).lat());
            temp2 = y_from_lat(getIntersectionPosition(ss_info1.to).lat());
            incoming.y = temp2 - temp1;
        }
        
        if(ss_info2.curvePointCount != 0){
            //If street_segment2 has curve points and the shared intersection is
            //the FROM intersection then the outgoing vector will be from the 
            //FROM intersection to curve point 0
            if(ss_info1.to == ss_info2.from){
                temp1 = x_from_lon(getIntersectionPosition(ss_info2.from).lon());
                temp2 = x_from_lon(getStreetSegmentCurvePoint(0, street_segment2).lon());
                outgoing.x = temp2 - temp1;
                temp1 = y_from_lat(getIntersectionPosition(ss_info2.from).lat());
                temp2 = y_from_lat(getStreetSegmentCurvePoint(0, street_segment2).lat());
                outgoing.y = temp2 - temp1;
            }
            //If street_segment2 has curve points and the shared intersection is
            //the TO intersection then the outgoing vector will be from the 
            //TO intersection to the last curve point
            else if(ss_info1.to == ss_info2.to){
                temp1 = x_from_lon(getIntersectionPosition(ss_info2.to).lon());
                temp2 = x_from_lon(getStreetSegmentCurvePoint(
                        ss_info2.curvePointCount-1, street_segment2).lon());
                outgoing.x = temp2 - temp1;
                temp1 = y_from_lat(getIntersectionPosition(ss_info2.to).lat());
                temp2 = y_from_lat(getStreetSegmentCurvePoint(
                        ss_info2.curvePointCount-1, street_segment2).lat());
                outgoing.y = temp2 - temp1;
            }
        }
        //If street_segment2 does NOT have curve points the outgoing vector will
        //be from the street_segment1's TO intersection to street_segment2's
        //intersection that is not the same as street_segment1's TO intersection
        else{
            temp1 = x_from_lon(getIntersectionPosition(ss_info1.to).lon());
            temp2 = x_from_lon(getIntersectionPosition(
                    get_other_intersection(street_segment2, ss_info1.to)).lon());
            outgoing.x = temp2 - temp1;
            temp1 = y_from_lat(getIntersectionPosition(ss_info1.to).lat());
            temp2 = y_from_lat(getIntersectionPosition(
                    get_other_intersection(street_segment2, ss_info1.to)).lat());
            outgoing.y = temp2 - temp1;
        }
        
        //The two vectors made allow us to use the following formula to calculate
        //delta which can determine if a right or left turn is being made
        outgoing.y *= -1;
        delta = (incoming.x * outgoing.y) + (incoming.y * outgoing.x);
        if(delta >= 0){
            return TurnType::RIGHT;
        }
        else{
            return TurnType::LEFT;
        }
        
    }
    //If the two street segments have no shared intersection then the turn type
    //is NONE
    else{
        return TurnType::NONE;
    }
}

// Returns the time required to travel along the path specified, in seconds.
// The path is given as a vector of street segment ids, and this function can
// assume the vector either forms a legal path or has size == 0.  The travel
// time is the sum of the length/speed-limit of each street segment, plus the
// given right_turn_penalty and left_turn_penalty (in seconds) per turn implied
// by the path.  If the turn type is STRAIGHT, then there is no penalty
double compute_path_travel_time(const std::vector<unsigned>& path, 
                                const double right_turn_penalty, 
                                const double left_turn_penalty){
    
    double travel_time = 0;
    
    if (path.size() == 0) return travel_time;
    else {
        for (unsigned i = 0; i < path.size(); i++) {
            
            //Add the travel time for the current street segment to the total
            //travel time
            travel_time = travel_time + find_street_segment_travel_time(path[i]);
            
            //Add the turn penalty between the current street segment and the
            //next one
            if(i < path.size()-1){
                if(find_turn_type(path[i], path[i+1]) == TurnType::LEFT) {
                    travel_time = travel_time + left_turn_penalty;
                }
                else if(find_turn_type(path[i], path[i+1]) == TurnType::RIGHT) {
                    travel_time = travel_time + right_turn_penalty;
                }
            }
        }
        
        return travel_time;
    }
}

// Returns a path (route) between the start intersection and the end
// intersection, if one exists. This routine should return the shortest path
// between the given intersections, where the time penalties to turn right and
// left are given by right_turn_penalty and left_turn_penalty, respectively (in
// seconds).  If no path exists, this routine returns an empty (size == 0)
// vector.  If more than one path exists, the path with the shortest travel
// time is returned. The path is returned as a vector of street segment ids;
// traversing these street segments, in the returned order, would take one from
// the start to the end intersection.
std::vector<unsigned> find_path_between_intersections(
		  const unsigned intersect_id_start, 
                  const unsigned intersect_id_end,
                  const double right_turn_penalty, 
                  const double left_turn_penalty)
{
    std::vector<unsigned> path;
    
    if(intersect_id_start == intersect_id_end) return path;
    else {
        //Priority queue of pairs. The first value of the pair is the travel
        //time and the second is the intersection id. The top value of the
        //priority queue is the one with the smallest travel time.
        std::priority_queue<std::pair<double, unsigned>, std::vector<std::pair<double,
                unsigned>>, std::greater<std::pair<double, unsigned>>> unvisited;
        
        std::vector<bool> is_intersection_visited;
        std::vector<double> travel_time_to_intersection;
        
        //Contains the intersection id used to travel to a certain intersection.
        //Used to efficiently build a path of street segments at the end.
        std::vector<unsigned> int_to_intersection;
        //Contains the street segment id used to travel to a certain intersection
        //Used to efficiently build a path of street segments at the end.
        std::vector<unsigned> ss_to_intersection;
        unsigned current_intersection;
        
        is_intersection_visited.resize(getNumIntersections());
        travel_time_to_intersection.resize(getNumIntersections());
        int_to_intersection.resize(getNumIntersections());
        ss_to_intersection.resize(getNumIntersections());
        
        //Set all intersections to unvisited and set travel time to each
        //intersection to be as large as possible
        std::fill(is_intersection_visited.begin(), 
                is_intersection_visited.end(), false);
        std::fill(travel_time_to_intersection.begin(), 
                travel_time_to_intersection.end(), DBL_MAX);
        
        //Add the starting intersection to the priority queue
        travel_time_to_intersection[intersect_id_start] = 0;
        unvisited.emplace(std::make_pair(0,intersect_id_start));
        
        while(!unvisited.empty()){
            //Get next intersection with the shortest travel time so far
            current_intersection = unvisited.top().second;
            unvisited.pop();
            
            //If this is the destination then the shortest path has been found
            if(current_intersection == intersect_id_end){
                //Work backwards from the destination intersection to the 
                //starting intersection and build the path of street segments.
                //This process makes building the path efficient because only
                //one vector for the path is made and no vectors ever have to
                //be copied.
                while(current_intersection != intersect_id_start){
                    //Add the street segment used to travel to the current intersection
                    path.push_back(ss_to_intersection[current_intersection]);
                    //Find the intersection used to travel to the current intersection
                    //and make that the current intersection.
                    current_intersection = int_to_intersection[current_intersection];
                }
                //The path is backwards so reverse it.
                std::reverse(path.begin(), path.end());
                return path;
            }
            
            //Only work on this intersection if it has not been visited before.
            //With this implementation, duplicate intersections can be added to
            //the priority queue that do not need to be visited multiple times.
            if(!is_intersection_visited[current_intersection]){
                
                //Work through all the street segments for the current intersection
                for(unsigned i = 0; 
                i < street_segments_from_intersection[current_intersection].size(); i++){
                    unsigned currentSegment = 
                        street_segments_from_intersection[current_intersection][i];
                    
                    //Check if this segment can be legally traveled (in case of
                    //one way streets)
                    if(is_street_segment_valid(currentSegment, current_intersection)){
                        
                        unsigned other_intersection = 
                            get_other_intersection(currentSegment, current_intersection);
                        double temp_travel_time = 
                            travel_time_to_intersection[current_intersection];
                        
                        //This is the travel time to this other intersection if
                        //the path was through this current intersection
                        temp_travel_time = temp_travel_time + 
                                find_street_segment_travel_time(currentSegment);
                        
                        //Add the turn penalty
                        TurnType tt;
                        //There is no turn penalty if the current intersection
                        //is the starting intersection
                        if(current_intersection == intersect_id_start){
                            tt = TurnType::NONE;
                        }
                        else{
                            tt = find_turn_type(ss_to_intersection[current_intersection],
                                    currentSegment);
                        }
                        
                        if(tt == TurnType::RIGHT){
                            temp_travel_time = temp_travel_time + right_turn_penalty;
                        }
                        else if(tt == TurnType::LEFT){
                            temp_travel_time = temp_travel_time + left_turn_penalty;
                        }
                        
                        //Check if this new travel time is better
                        if(temp_travel_time < travel_time_to_intersection[other_intersection]){
                            //Update the travel time
                            travel_time_to_intersection[other_intersection] = temp_travel_time;
                            
                            //Update the street segment and intersection used to
                            //travel to the other intersection
                            ss_to_intersection[other_intersection] = currentSegment;
                            int_to_intersection[other_intersection] = current_intersection;
                            
                            //Add the other intersection to the unvisited ones
                            unvisited.emplace(std::make_pair(temp_travel_time,other_intersection));
                        }
                    }
                }
                
                //The current intersection has now been visited and will not be again
                is_intersection_visited[current_intersection] = true;
            }
        }
        
        //If all the nodes in the priority queue are removed and the destination
        //intersection has still not been reached then there is no path
        return path;
    }
}