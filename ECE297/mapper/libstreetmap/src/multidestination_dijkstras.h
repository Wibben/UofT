#pragma once
#include "m3.h"
#include "StreetsDatabaseAPI.h"
#include "directions.h"

struct path_time_vector{
    std::vector<double> travel_time;
    std::vector<std::vector<unsigned>> path;
};

void find_path_between_all_intersections(
                  std::vector<double>& output,
		  const unsigned intersect_id_start, 
                  const std::vector<unsigned>& nodes,
                  const double right_turn_penalty, 
                  const double left_turn_penalty);