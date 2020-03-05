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


SUITE(medium_new_york_usa_public) {
    TEST(medium_new_york_usa) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(49099, 24912, 13.61199), DeliveryInfo(13359, 81737, 64.46082), DeliveryInfo(8708, 39004, 188.11136), DeliveryInfo(34831, 88240, 122.91425), DeliveryInfo(54749, 2434, 194.16504), DeliveryInfo(64020, 2932, 140.79376), DeliveryInfo(41904, 96716, 55.83173), DeliveryInfo(46277, 126011, 53.42627), DeliveryInfo(23263, 86591, 139.53435), DeliveryInfo(130754, 115750, 62.54547), DeliveryInfo(68238, 73145, 132.25157), DeliveryInfo(37500, 54622, 115.65539), DeliveryInfo(126662, 66687, 1.23825), DeliveryInfo(6848, 75313, 171.64159), DeliveryInfo(85039, 127132, 111.32867), DeliveryInfo(17756, 121946, 146.61469), DeliveryInfo(143312, 28617, 37.15287), DeliveryInfo(52362, 85588, 5.99976), DeliveryInfo(54034, 45059, 14.50743), DeliveryInfo(87418, 51955, 88.43934), DeliveryInfo(55508, 116284, 55.23724), DeliveryInfo(89465, 70543, 157.20303), DeliveryInfo(141687, 51285, 147.67279), DeliveryInfo(34182, 101811, 113.03408), DeliveryInfo(35312, 100958, 68.13067), DeliveryInfo(48083, 46204, 135.47527), DeliveryInfo(63145, 88456, 126.80992), DeliveryInfo(106039, 111606, 41.46676), DeliveryInfo(88609, 95853, 41.83722), DeliveryInfo(110599, 140809, 124.71667), DeliveryInfo(108905, 85754, 198.43752), DeliveryInfo(123693, 137467, 177.14240), DeliveryInfo(102945, 137149, 40.57933), DeliveryInfo(96315, 120330, 4.58260), DeliveryInfo(58211, 56094, 6.86531), DeliveryInfo(141732, 29221, 195.18217), DeliveryInfo(96275, 77511, 39.01222), DeliveryInfo(25744, 54408, 159.00911), DeliveryInfo(92910, 11976, 128.93202), DeliveryInfo(112188, 44568, 77.88078), DeliveryInfo(110425, 112141, 183.02200), DeliveryInfo(139936, 23761, 55.50504), DeliveryInfo(94720, 66406, 83.74704), DeliveryInfo(27542, 71485, 153.42888), DeliveryInfo(74048, 48360, 165.24220), DeliveryInfo(53749, 49614, 6.51730), DeliveryInfo(91714, 12700, 196.69681), DeliveryInfo(132276, 116084, 151.72243), DeliveryInfo(106575, 21811, 193.50153), DeliveryInfo(25888, 102052, 111.69054)};
        depots = {13, 49722, 59265, 82806, 127448, 110920, 8757, 86547, 7546, 142393};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 384.027862549;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR medium_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR medium_new_york_usa: INVALID" << std::endl;
        }

    } //medium_new_york_usa

} //medium_new_york_usa_public

