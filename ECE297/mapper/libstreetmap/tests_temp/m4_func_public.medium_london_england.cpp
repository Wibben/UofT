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


SUITE(medium_london_england_public) {
    TEST(medium_london_england) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(312607, 113450, 56.04690), DeliveryInfo(292114, 207726, 40.02195), DeliveryInfo(223329, 165342, 99.62160), DeliveryInfo(356036, 302100, 33.86950), DeliveryInfo(9523, 418797, 114.51802), DeliveryInfo(270642, 322732, 99.23920), DeliveryInfo(227943, 111348, 175.12781), DeliveryInfo(434947, 261501, 194.48256), DeliveryInfo(320326, 193685, 68.13868), DeliveryInfo(150965, 375109, 122.38348), DeliveryInfo(42677, 78819, 173.35054), DeliveryInfo(173483, 212348, 3.59156), DeliveryInfo(54282, 149303, 168.42726), DeliveryInfo(41131, 34036, 152.62354)};
        depots = {334742, 173626, 340909, 301888, 356671, 259838, 355045, 82742, 125922, 413154, 392772, 164196, 198567, 116630, 370422, 272114, 55924, 284370, 89814, 198486};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 519.569763184;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR medium_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR medium_london_england: INVALID" << std::endl;
        }

    } //medium_london_england

} //medium_london_england_public

