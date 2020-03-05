#include "multidestination_dijkstras.h"

// Returns a path (route) between the start intersection and the end
// intersection, if one exists. This routine should return the shortest path
// between the given intersections, where the time penalties to turn right and
// left are given by right_turn_penalty and left_turn_penalty, respectively (in
// seconds).  If no path exists, this routine returns an empty (size == 0)
// vector.  If more than one path exists, the path with the shortest travel
// time is returned. The path is returned as a vector of street segment ids;
// traversing these street segments, in the returned order, would take one from
// the start to the end intersection.
void find_path_between_all_intersections(
                  std::vector<double>& output,
		  const unsigned intersect_id_start, 
                  const std::vector<unsigned>& nodes, //this is all the nodes being passed in
                  const double right_turn_penalty, 
                  const double left_turn_penalty)
{
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

        bool done = true;
        for(unsigned a = 0; a < nodes.size(); a++){
            if(is_intersection_visited[nodes[a]] == false)
                done = false;
        }
        //If this is the destination then the shortest path has been found
        if(done){
            for(unsigned i = 0; i < nodes.size(); i++)
                output.push_back(travel_time_to_intersection[nodes[i]]);
            return;
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
}