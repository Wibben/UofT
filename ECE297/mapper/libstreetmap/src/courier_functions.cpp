#include "courier_functions.h"
#include <thread>

std::vector<int> legal_next_nodes ( int deliveries,
                                    const std::vector<float>& weight,
                                    float truck_weight,
                                    float truck_capacity,
                                    const std::vector<bool>& pickUpStatus,
                                    const std::vector<bool>& dropOffStatus)
{
    std::vector<int> legalNodes;
    
    for(int j = 0; j < deliveries; j++){
        if(pickUpStatus[j] == true){
            if(dropOffStatus[j] == false)
                legalNodes.push_back(2*j+1);
        }else{
            if(truck_weight + weight[j] <= truck_capacity)
                legalNodes.push_back(2*j);
        }
    }
    return legalNodes;
}

bool find_node(unsigned node, const std::vector<unsigned>& nodes){
    for(unsigned i = 0; i < nodes.size(); i++)
        if(node == nodes[i]) return true;
    return false;
}

void two_opt(std::vector<unsigned>& path, 
             const std::vector<float>& weight,
             const std::vector<std::vector<double>>& time,
             float capacity){
    
    if(path.size() < 9) return;
    //Split path into 3 sections that will be permuted to find the best solution
    unsigned start, opt1, opt2, end;
    start = 0;
    opt1 = path.size() / 3;
    opt2 = 2 * path.size() / 3;
    end = path.size() - 1;
    
    double path_time = time_check(path,time);
    //unsigned int shuffle_starts[3] = {start,opt1,opt2};
    //unsigned int shuffle_ends[3] = {opt1,opt2,end};
    
    for(int i = 0; i < 100000; i++){
        std::vector<unsigned> temp = path;
        /*
        // Multithread the Knuth shuffle for better performance times
        #pragma omp parallel for
        for(int j=0; j<3; j++) {
            knuthShuffle(temp,shuffle_starts[j],shuffle_ends[j]);
        }
        */
        knuthShuffle(temp,start,opt1);
        knuthShuffle(temp,opt1, opt2);
        knuthShuffle(temp,opt2,end);
        double temp_time = time_check(temp,time);
        if(legality_check(temp,weight,capacity) && temp_time < path_time) {
                path = temp;
                path_time = temp_time;
        }
    }
}

void knuthShuffle (std::vector<unsigned>& path, unsigned start, unsigned end){
    for(unsigned i = start; i <= end-2; i++){
        unsigned j = i + rand()%(end-i);
        unsigned temp = path[i];
        path[i] = path[j];
        path[j] = temp;
    }
}

bool legality_check(const std::vector<unsigned>& path, 
                    const std::vector<float>& weight, 
                    float capacity)
{
    std::vector<unsigned> delivery_index;
    std::vector<unsigned> delivery_type;
    float truck_weight = 0;
    for(unsigned i = 0; i < path.size(); i++){
        delivery_index.push_back(path[i]/2);
        delivery_type.push_back(path[i]%2);
    }
    if(delivery_type[0] == 1 || delivery_type.back() == 0)
        return false;
    for(unsigned j = 0; j < path.size(); j++){
        if(delivery_type[j] == 0){ 
            truck_weight += weight[path[j]/2];
            //check if pick up is before drop off
            for(unsigned l = 0; l < j; l++){
                if(delivery_type[l] == 1 && delivery_index[l] == delivery_index[j])
                    return false;
            }
        } else {
            truck_weight -= weight[path[j]/2];
        }
        if(truck_weight > capacity)
            return false;
    }
    return true;
}

double time_check(const std::vector<unsigned>& path, 
                  const std::vector<std::vector<double>>& time)
{
    double t = 0;
    for(unsigned i = 0; i < path.size()-1; i++){
        t += time[path[i]][path[i+1]];
    }
    return t;
}

// Portion of algorithm to multithread
void multithread_process(std::vector<unsigned>& path, 
                         unsigned initial_pick_up, 
                         const std::vector<DeliveryInfo>& deliveries,
                         const std::vector<float>& weight,
                         const std::vector<unsigned>& nodes,
                         const std::vector<std::vector<double>>& time,
                         int truck_capacity)
{
/*******************************Greedy Order***********************************/
    std::vector<int> legalNodes; 
    double shortest_distance = INT_MAX;
    int shortest_index = -1;
    float truck_weight = 0; // Weight
    //Set up pick up / drop off delivery status 
    std::vector<bool> pickUpStatus;
    std::vector<bool> dropOffStatus;
    for(unsigned i = 0; i < deliveries.size(); i++){
        pickUpStatus.push_back(false);
        dropOffStatus.push_back(false);
    }
    
    //find path from first pick up
    path.push_back(initial_pick_up*2);
    truck_weight += weight[initial_pick_up];
    pickUpStatus[initial_pick_up] = true;
    for(unsigned a = 1; a < nodes.size(); a++){
        legalNodes = legal_next_nodes(deliveries.size(), weight, truck_weight, truck_capacity,pickUpStatus,dropOffStatus);
        //reset 
        shortest_distance = INT_MAX;
        //find closest node
        for(unsigned b = 0; b < legalNodes.size(); b++){
            if(shortest_distance > time[legalNodes[b]][path.back()]){
                shortest_distance = time[legalNodes[b]][path.back()];
                shortest_index = legalNodes[b];
            }
        }
        //closest node found
        path.push_back(shortest_index);
        if(shortest_index%2 == 0){
            truck_weight += weight[shortest_index/2];
            pickUpStatus[shortest_index/2] = true;
        }else{
            truck_weight -= weight[shortest_index/2];
            dropOffStatus[shortest_index/2] = true;
        }
    }
/***************************2-Opt Shuffle***************************************/    
    //two_opt(path,weight,time,truck_capacity);
}
