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


SUITE(extreme_london_england_public) {
    TEST(extreme_london_england) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(213465, 200094, 125.63939), DeliveryInfo(59002, 321463, 138.90828), DeliveryInfo(169790, 320943, 41.75622), DeliveryInfo(39315, 220437, 14.52685), DeliveryInfo(349892, 207769, 25.68835), DeliveryInfo(342430, 276994, 2.24971), DeliveryInfo(30437, 306362, 195.97539), DeliveryInfo(116228, 3976, 128.12622), DeliveryInfo(323219, 38530, 181.16568), DeliveryInfo(173700, 207439, 48.73264), DeliveryInfo(152996, 10929, 173.43008), DeliveryInfo(30748, 148195, 43.76711), DeliveryInfo(192987, 97938, 82.10384), DeliveryInfo(375153, 299401, 34.42117), DeliveryInfo(245805, 63523, 144.34010), DeliveryInfo(259709, 206482, 41.04451), DeliveryInfo(353422, 435891, 59.36185), DeliveryInfo(235088, 198355, 55.57969), DeliveryInfo(270168, 15714, 87.04560), DeliveryInfo(242595, 284696, 178.14987), DeliveryInfo(334355, 19547, 73.01449), DeliveryInfo(261345, 245231, 82.35532), DeliveryInfo(215876, 328719, 173.95419), DeliveryInfo(252656, 97922, 142.96730), DeliveryInfo(201742, 190713, 175.02370), DeliveryInfo(266079, 278752, 168.74875), DeliveryInfo(243409, 298139, 70.99045), DeliveryInfo(156322, 149487, 179.85307), DeliveryInfo(401665, 202299, 87.64339), DeliveryInfo(158540, 434218, 34.37556), DeliveryInfo(223234, 417347, 142.85555), DeliveryInfo(64723, 338129, 179.98189), DeliveryInfo(241313, 387137, 105.46758), DeliveryInfo(163020, 83628, 25.42519), DeliveryInfo(432418, 413406, 99.18584), DeliveryInfo(336561, 20627, 199.83517), DeliveryInfo(60442, 352002, 43.79069), DeliveryInfo(287428, 66136, 20.41799), DeliveryInfo(113676, 175115, 196.69122), DeliveryInfo(179046, 298495, 82.16702), DeliveryInfo(39098, 188132, 84.37139), DeliveryInfo(424643, 417548, 91.24757), DeliveryInfo(372239, 430343, 11.19065)};
        depots = {59};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 1212.614257812;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR extreme_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_london_england: INVALID" << std::endl;
        }

    } //extreme_london_england

    TEST(extreme_multi_london_england) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(34879, 389264, 137.08173), DeliveryInfo(291829, 231525, 71.91736), DeliveryInfo(129725, 383125, 122.56682), DeliveryInfo(195441, 389264, 23.21515), DeliveryInfo(89516, 394484, 18.25418), DeliveryInfo(89516, 76650, 147.26566), DeliveryInfo(89516, 310581, 59.99919), DeliveryInfo(286772, 17241, 21.01382), DeliveryInfo(394891, 31461, 158.01956), DeliveryInfo(66940, 347829, 162.01428), DeliveryInfo(343938, 41336, 191.56882), DeliveryInfo(89516, 130528, 18.27139), DeliveryInfo(343938, 83342, 178.26596), DeliveryInfo(422492, 66208, 75.63104), DeliveryInfo(135963, 409382, 186.29137), DeliveryInfo(143440, 49854, 70.28852), DeliveryInfo(64254, 293818, 97.13382), DeliveryInfo(36527, 138649, 146.55731), DeliveryInfo(242272, 96989, 68.50053), DeliveryInfo(219488, 257177, 189.17386), DeliveryInfo(343938, 83342, 11.40528), DeliveryInfo(335283, 31461, 144.21942), DeliveryInfo(89516, 272137, 15.51426), DeliveryInfo(150084, 187224, 88.62766), DeliveryInfo(116559, 394484, 145.45842), DeliveryInfo(25457, 17241, 23.84434), DeliveryInfo(143440, 147035, 189.91982), DeliveryInfo(105571, 114243, 19.79551), DeliveryInfo(69656, 138649, 148.92365), DeliveryInfo(343938, 17241, 93.21634), DeliveryInfo(360534, 394484, 45.94852), DeliveryInfo(105571, 23238, 180.90318), DeliveryInfo(343938, 257177, 177.74478), DeliveryInfo(89516, 257177, 118.59480), DeliveryInfo(274269, 24644, 89.59936), DeliveryInfo(105571, 69040, 50.64853), DeliveryInfo(89516, 83342, 55.16647), DeliveryInfo(403738, 118970, 140.38144), DeliveryInfo(400133, 158490, 152.14987), DeliveryInfo(86129, 158490, 26.47551), DeliveryInfo(231240, 121008, 199.44727), DeliveryInfo(59697, 259279, 118.72143), DeliveryInfo(2586, 158490, 1.91211), DeliveryInfo(152228, 158490, 99.38187), DeliveryInfo(260440, 76650, 62.36740), DeliveryInfo(264388, 234780, 136.90015), DeliveryInfo(62758, 318743, 106.40794), DeliveryInfo(143440, 390891, 17.67936), DeliveryInfo(254647, 286103, 0.43400), DeliveryInfo(143440, 155660, 149.67912), DeliveryInfo(89516, 138649, 160.95218), DeliveryInfo(33082, 247326, 122.76397), DeliveryInfo(249835, 314504, 139.63599), DeliveryInfo(429827, 362691, 168.15720), DeliveryInfo(343938, 158490, 115.99158), DeliveryInfo(89516, 343518, 29.86299), DeliveryInfo(179361, 204300, 116.28070), DeliveryInfo(354374, 310032, 185.58627), DeliveryInfo(143440, 168741, 34.75976), DeliveryInfo(336040, 40447, 88.45197), DeliveryInfo(343938, 394685, 65.40082), DeliveryInfo(143440, 17241, 163.03503), DeliveryInfo(319729, 394484, 64.14413), DeliveryInfo(143440, 17241, 101.15028), DeliveryInfo(143440, 25775, 25.66063), DeliveryInfo(11296, 338914, 64.33150)};
        depots = {68};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 1620.838867188;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR extreme_multi_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_multi_london_england: INVALID" << std::endl;
        }

    } //extreme_multi_london_england

} //extreme_london_england_public

