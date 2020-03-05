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


SUITE(easy_toronto_canada_public) {
    TEST(easy_toronto_canada) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(76703, 68888, 75.05315), DeliveryInfo(46668, 62481, 90.76368), DeliveryInfo(97027, 20279, 53.31088), DeliveryInfo(68571, 78437, 169.31749), DeliveryInfo(59320, 73025, 124.38162), DeliveryInfo(53792, 40207, 25.56255), DeliveryInfo(96297, 37132, 129.98366), DeliveryInfo(74152, 26554, 41.05344), DeliveryInfo(76038, 7339, 90.72692), DeliveryInfo(73462, 30577, 79.29823), DeliveryInfo(30279, 84608, 4.35320), DeliveryInfo(85859, 23403, 133.52373), DeliveryInfo(61948, 51942, 123.70870), DeliveryInfo(81284, 58216, 142.89053), DeliveryInfo(87529, 96023, 69.00546), DeliveryInfo(21121, 34676, 162.74165), DeliveryInfo(104396, 59293, 102.08238), DeliveryInfo(58255, 62657, 18.80107), DeliveryInfo(98395, 90185, 146.41916), DeliveryInfo(25096, 32307, 198.81140), DeliveryInfo(19446, 100735, 24.81209), DeliveryInfo(29244, 88038, 104.19133), DeliveryInfo(84448, 59223, 19.50759), DeliveryInfo(107254, 86940, 50.89673), DeliveryInfo(97956, 27422, 36.02774)};
        depots = {7, 27769};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2427.750000000;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR easy_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR easy_toronto_canada: INVALID" << std::endl;
        }

    } //easy_toronto_canada

} //easy_toronto_canada_public

