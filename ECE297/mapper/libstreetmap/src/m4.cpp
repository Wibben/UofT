#include "m3.h"
#include "m4.h"
#include "StreetsDatabaseAPI.h"
#include "draw_functions.h"
#include "directions.h"
#include "courier_functions.h"
#include "multidestination_dijkstras.h"
#include <thread>

std::vector<CourierSubpath> traveling_courier(
		const std::vector<DeliveryInfo>& deliveries,
	       	const std::vector<unsigned>& depots, 
		const float right_turn_penalty, 
		const float left_turn_penalty, 
		const float truck_capacity)
{   
/*************************DATA STRUCTURE INITIALIZATION*************************/    
    //total and subpath
    std::vector<CourierSubpath> total_path;
    CourierSubpath subpath;
    //set up weight and total nodes
    std::vector<float> weight;
    std::vector<unsigned> nodes;
    //final path, stores delivery index of node
    std::vector<unsigned> path;
    //Set up weighted adjacency matrix between nodes
    std::vector<std::vector<double>> time;
    
    //fill vectors
    weight.resize(deliveries.size());
    nodes.resize(deliveries.size()*2);
    for(unsigned i = 0; i < deliveries.size(); i++){
        weight[i] = deliveries[i].itemWeight;
        //total nodes organized as {pickUp_1, dropOff_1, pickUp_2, dropOff_2, ...}
        //Nodes organized 0,1,2,3,4,.....
        //Even nodes are all pick ups, where: 
        //  delivery index = node_index / 2
        //Odd nodes are all drop offs, where:
        //  node_type = node_index % 2 (0 = pickUp, 1 = dropOff)
        nodes[2*i] = deliveries[i].pickUp;
        nodes[2*i+1] = deliveries[i].dropOff;
    }
    
    //Set up weighted adjacency matrix between nodes
    time.resize(nodes.size());
    #pragma omp parallel for
    for(unsigned a = 0; a < nodes.size(); a++){
        find_path_between_all_intersections(time[a],nodes[a], nodes, right_turn_penalty, left_turn_penalty);
    }
    
/**************************Multithreading************************************/
    
    // Launch the threads
    std::vector<unsigned> multithread_paths[NUM_THREADS];
    std::thread threads[NUM_THREADS];
    // Make sure to start the thread only if there is a valid starting depot
    for(unsigned i=0; i<NUM_THREADS && i<deliveries.size(); i++) {
        threads[i] = std::thread(multithread_process,std::ref(multithread_paths[i]),
                                 i,deliveries,weight,nodes,time,truck_capacity);
    }
    
    // Join the threads
    for(unsigned i=0; i<NUM_THREADS && i<deliveries.size(); i++) {
        threads[i].join();
    }
    
    // Compare the paths and select the best one
    int best_path = 0;
    double best_path_time = time_check(multithread_paths[0],time);
    for(unsigned i=1; i<NUM_THREADS && i<deliveries.size(); i++) {
        // Update the best path is a new one is found
        double path_time = time_check(multithread_paths[i],time);
        if(path_time<best_path_time) {
            best_path_time = path_time;
            best_path = i;
        }
    }
    // Assign the best path to path
    path = multithread_paths[best_path];
    
/**************************Create Path***************************************/
    // DEPOT -> NODE (FIRST PICKUP)
    double shortest_distance = INT_MAX;
    unsigned closest_depot = 0;
    for(unsigned l = 0; l <depots.size(); l++) {
        if(shortest_distance > find_distance_between_two_points(getIntersectionPosition(nodes[path[0]]),getIntersectionPosition(depots[l]))){
            shortest_distance = find_distance_between_two_points(getIntersectionPosition(nodes[path[0]]),getIntersectionPosition(depots[l]));
            closest_depot = l;
        }
    }
    subpath.pickUp_indices.clear();
    subpath.start_intersection = depots[closest_depot];
    subpath.end_intersection = nodes[path[0]];
    subpath.subpath = find_path_between_intersections(subpath.start_intersection, subpath.end_intersection, right_turn_penalty, left_turn_penalty);
    total_path.push_back(subpath);
    // NODE -> NODE
    for(unsigned i = 0; i < path.size()-1; i++){
        subpath.pickUp_indices.clear();
        //add deliveries to be picked up
        if(path[i] % 2 == 0)
                subpath.pickUp_indices.push_back(path[i]/2);
        subpath.start_intersection = nodes[path[i]];
        subpath.end_intersection = nodes[path[i+1]];
        subpath.subpath = find_path_between_intersections(subpath.start_intersection, subpath.end_intersection, right_turn_penalty, left_turn_penalty);
        total_path.push_back(subpath);
    }
    //subpath from last node to depot
    shortest_distance = INT_MAX;
    for(unsigned j = 0; j < depots.size(); j++){
        if(shortest_distance > find_distance_between_two_points(getIntersectionPosition(nodes[path.back()]),getIntersectionPosition(depots[j]))){
            shortest_distance = find_distance_between_two_points(getIntersectionPosition(nodes[path.back()]),getIntersectionPosition(depots[j]));
            closest_depot = j;
        }
    }
    subpath.pickUp_indices.clear();
    subpath.start_intersection = nodes[path.back()];
    subpath.end_intersection = depots[closest_depot];
    subpath.subpath = find_path_between_intersections(subpath.start_intersection, subpath.end_intersection, right_turn_penalty, left_turn_penalty);
    total_path.push_back(subpath);
    //return path
    return total_path;
}
