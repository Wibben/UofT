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


SUITE(hard_london_england_public) {
    TEST(hard_london_england) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(20499, 330511, 178.03163), DeliveryInfo(331993, 318988, 164.94493), DeliveryInfo(160875, 288159, 57.18738), DeliveryInfo(156725, 317385, 92.00729), DeliveryInfo(247846, 204241, 83.65553), DeliveryInfo(82438, 166141, 136.10985), DeliveryInfo(308122, 325961, 158.40143), DeliveryInfo(379111, 391356, 195.22554), DeliveryInfo(143917, 174230, 131.74852), DeliveryInfo(104252, 125421, 33.08278), DeliveryInfo(259042, 69630, 138.92177), DeliveryInfo(177386, 335789, 92.67679), DeliveryInfo(188998, 331033, 1.19564), DeliveryInfo(274507, 254529, 114.70366), DeliveryInfo(148822, 161730, 59.93352), DeliveryInfo(265213, 191618, 50.78416), DeliveryInfo(105693, 53147, 2.47673), DeliveryInfo(267776, 39985, 154.07709), DeliveryInfo(426193, 77054, 55.23943), DeliveryInfo(428944, 26065, 62.75043), DeliveryInfo(370223, 424214, 26.04098), DeliveryInfo(395913, 77485, 24.32761), DeliveryInfo(26210, 146958, 118.04330), DeliveryInfo(22588, 112242, 68.26944), DeliveryInfo(381463, 418841, 34.10250), DeliveryInfo(288278, 102310, 161.94672), DeliveryInfo(261649, 138512, 130.02480), DeliveryInfo(221633, 163870, 26.90716)};
        depots = {39};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 201.225357056;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR hard_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_london_england: INVALID" << std::endl;
        }

    } //hard_london_england

    TEST(hard_multi_london_england) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(367443, 237274, 65.49703), DeliveryInfo(367443, 32038, 7.25534), DeliveryInfo(244397, 415198, 22.53954), DeliveryInfo(217674, 378142, 6.18117), DeliveryInfo(350891, 202107, 171.48422), DeliveryInfo(414991, 396731, 115.00910), DeliveryInfo(426793, 427549, 4.56941), DeliveryInfo(56992, 251355, 170.13731), DeliveryInfo(167654, 98289, 98.00179), DeliveryInfo(212434, 51648, 44.03192), DeliveryInfo(367443, 315631, 64.68565), DeliveryInfo(137622, 304777, 41.35348), DeliveryInfo(20929, 156777, 173.65221), DeliveryInfo(2959, 20749, 165.84598), DeliveryInfo(195907, 420936, 151.49455), DeliveryInfo(367443, 419737, 193.37956), DeliveryInfo(398670, 183698, 24.91690), DeliveryInfo(276118, 394595, 163.48021), DeliveryInfo(313967, 391914, 153.09592), DeliveryInfo(251536, 36528, 92.59637), DeliveryInfo(433190, 436561, 119.04930), DeliveryInfo(360348, 195018, 28.43804), DeliveryInfo(115947, 204837, 132.55122), DeliveryInfo(222130, 298065, 180.40952), DeliveryInfo(419868, 70575, 147.55412), DeliveryInfo(364502, 298065, 185.33681), DeliveryInfo(126861, 233747, 192.62813), DeliveryInfo(353392, 298065, 152.43340)};
        depots = {49, 186028};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 325.980895996;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR hard_multi_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_multi_london_england: INVALID" << std::endl;
        }

    } //hard_multi_london_england

} //hard_london_england_public

