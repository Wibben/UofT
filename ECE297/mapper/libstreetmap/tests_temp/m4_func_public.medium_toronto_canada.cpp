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


SUITE(medium_toronto_canada_public) {
    TEST(medium_toronto_canada) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(36562, 82230, 3.64505), DeliveryInfo(9948, 60866, 150.28346), DeliveryInfo(76659, 29045, 132.79056), DeliveryInfo(6484, 34406, 133.22345), DeliveryInfo(40770, 65709, 29.19379), DeliveryInfo(82359, 1812, 13.61199), DeliveryInfo(47674, 2183, 64.46082), DeliveryInfo(31204, 104855, 188.11136), DeliveryInfo(34461, 83109, 122.91425), DeliveryInfo(6521, 9457, 194.16504), DeliveryInfo(97367, 64481, 140.79376), DeliveryInfo(50814, 33554, 55.83173), DeliveryInfo(92110, 54468, 53.42627), DeliveryInfo(5100, 27925, 139.53435), DeliveryInfo(94321, 49660, 62.54547), DeliveryInfo(38992, 36012, 132.25157), DeliveryInfo(40025, 94671, 115.65539), DeliveryInfo(13222, 16241, 1.23825), DeliveryInfo(106719, 21310, 171.64159), DeliveryInfo(19170, 90809, 111.32867), DeliveryInfo(65097, 63734, 146.61469), DeliveryInfo(64448, 38689, 37.15287), DeliveryInfo(40237, 71378, 5.99976), DeliveryInfo(41335, 86592, 14.50743), DeliveryInfo(26296, 53232, 88.43934), DeliveryInfo(25937, 86444, 55.23724), DeliveryInfo(19278, 38190, 157.20303), DeliveryInfo(5619, 56083, 147.67279), DeliveryInfo(25454, 72021, 113.03408), DeliveryInfo(35806, 65870, 68.13067), DeliveryInfo(78964, 75815, 135.47527), DeliveryInfo(47022, 105509, 126.80992), DeliveryInfo(66621, 52531, 41.46676), DeliveryInfo(55141, 69186, 41.83722), DeliveryInfo(106035, 83507, 124.71667), DeliveryInfo(71722, 63858, 198.43752), DeliveryInfo(17323, 36946, 177.14240), DeliveryInfo(82598, 70534, 40.57933), DeliveryInfo(65984, 89606, 4.58260), DeliveryInfo(43347, 41771, 6.86531), DeliveryInfo(105542, 57720, 195.18217), DeliveryInfo(81098, 33188, 39.01222), DeliveryInfo(63326, 40515, 159.00911), DeliveryInfo(79363, 8918, 128.93202), DeliveryInfo(83543, 86195, 77.88078), DeliveryInfo(68296, 75994, 183.02200), DeliveryInfo(104205, 17694, 55.50504), DeliveryInfo(98501, 40675, 83.74704), DeliveryInfo(71692, 49450, 153.42888), DeliveryInfo(20510, 75179, 165.24220)};
        depots = {9, 37026, 44133, 61663, 94906};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 306.382446289;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR medium_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR medium_toronto_canada: INVALID" << std::endl;
        }

    } //medium_toronto_canada

} //medium_toronto_canada_public

