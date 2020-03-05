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


SUITE(easy_london_england_public) {
    TEST(easy_london_england) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(88693, 230067, 143.19493), DeliveryInfo(74411, 52846, 162.73782), DeliveryInfo(165996, 80437, 117.61046), DeliveryInfo(409505, 213096, 174.49744), DeliveryInfo(348295, 71958, 165.85783), DeliveryInfo(13105, 270900, 123.38269), DeliveryInfo(342697, 78362, 5.95716)};
        depots = {19};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 1034.523559570;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR easy_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR easy_london_england: INVALID" << std::endl;
        }

    } //easy_london_england

} //easy_london_england_public

