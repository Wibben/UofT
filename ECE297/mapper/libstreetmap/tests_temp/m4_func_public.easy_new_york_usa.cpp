#include <random>
#include <iostream>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"
#include "m4.h"

#include "unit_test_util.h"
#include "courier_verify.h"

using ece297test::relative_error;
using ece297test::courier_path_is_legal_with_capacity;


SUITE(easy_new_york_usa_public) {
    TEST(easy_new_york_usa) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(103003, 92509, 53.31088), DeliveryInfo(130297, 35659, 169.31749), DeliveryInfo(92083, 27233, 124.38162), DeliveryInfo(132133, 98065, 25.56255), DeliveryInfo(105332, 53994, 129.98366), DeliveryInfo(131543, 116751, 41.05344), DeliveryInfo(129316, 49864, 90.72692), DeliveryInfo(102110, 79624, 79.29823), DeliveryInfo(33701, 41062, 4.35320), DeliveryInfo(40662, 124272, 133.52373), DeliveryInfo(62670, 31428, 123.70870), DeliveryInfo(83905, 107801, 142.89053), DeliveryInfo(79661, 128948, 69.00546), DeliveryInfo(99578, 46566, 162.74165), DeliveryInfo(140192, 118225, 102.08238), DeliveryInfo(144031, 84141, 18.80107), DeliveryInfo(115300, 121108, 146.41916), DeliveryInfo(83190, 113619, 198.81140), DeliveryInfo(26114, 78178, 24.81209), DeliveryInfo(28363, 135276, 104.19133), DeliveryInfo(113405, 79529, 19.50759), DeliveryInfo(39272, 9856, 50.89673), DeliveryInfo(78230, 36824, 36.02774), DeliveryInfo(109156, 43385, 94.95014), DeliveryInfo(72236, 69752, 138.08812)};
        depots = {9, 37291, 117542, 98651};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2402.911621094;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR easy_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR easy_new_york_usa: INVALID" << std::endl;
        }

    } //easy_new_york_usa

} //easy_new_york_usa_public

