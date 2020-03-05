#pragma once
#include "m3.h"
#include "m4.h"
#include "StreetsDatabaseAPI.h"
#include "draw_functions.h"
#include "directions.h"

#define NUM_THREADS 8

bool find_node(unsigned node, const std::vector<unsigned>& nodes);
std::vector<int> legal_next_nodes ( int deliveries,
                                    const std::vector<float>& weight,
                                    float truck_weight,
                                    float truck_capacity,
                                    const std::vector<bool>& pickUpStatus,
                                    const std::vector<bool>& dropOffStatus);
void two_opt(std::vector<unsigned>& path, 
             const std::vector<float>& weight,
             const std::vector<std::vector<double>>& time,
             float capacity);   
void knuthShuffle (std::vector<unsigned>& path, unsigned start, unsigned end);
bool legality_check(const std::vector<unsigned>& path,
                    const std::vector<float>& weight, 
                    float capacity);
double time_check(const std::vector<unsigned>& path, 
                  const std::vector<std::vector<double>>& time);

void multithread_process(std::vector<unsigned>& path, 
                         unsigned initial_pick_up, 
                         const std::vector<DeliveryInfo>& deliveries,
                         const std::vector<float>& weight,
                         const std::vector<unsigned>& nodes,
                         const std::vector<std::vector<double>>& time,
                         int truck_capacity);
