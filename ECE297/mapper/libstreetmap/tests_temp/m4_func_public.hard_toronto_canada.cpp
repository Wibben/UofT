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


SUITE(hard_toronto_canada_public) {
    TEST(hard_toronto_canada) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(23189, 92605, 114.35927), DeliveryInfo(22739, 105801, 36.11969), DeliveryInfo(102168, 70747, 133.65018), DeliveryInfo(73529, 23966, 27.79453), DeliveryInfo(76548, 25649, 69.74749), DeliveryInfo(106184, 56342, 181.15286), DeliveryInfo(104727, 6329, 29.53294), DeliveryInfo(105074, 99780, 184.55353), DeliveryInfo(81630, 64798, 183.54843), DeliveryInfo(62121, 14056, 66.25425), DeliveryInfo(73397, 26925, 158.69901), DeliveryInfo(35650, 21665, 160.19560), DeliveryInfo(53625, 35208, 2.01616), DeliveryInfo(98173, 80313, 122.29246), DeliveryInfo(89653, 63691, 179.44002), DeliveryInfo(12849, 83330, 149.54332), DeliveryInfo(7644, 64847, 5.73868), DeliveryInfo(94080, 82969, 11.65919), DeliveryInfo(87300, 63926, 0.97038), DeliveryInfo(51178, 22508, 41.35726), DeliveryInfo(39005, 93653, 156.17305), DeliveryInfo(82136, 64030, 29.33303), DeliveryInfo(48361, 96808, 134.64973), DeliveryInfo(3949, 89513, 76.94942), DeliveryInfo(103887, 105256, 25.21689), DeliveryInfo(89223, 73370, 44.42744), DeliveryInfo(106372, 19862, 156.78815), DeliveryInfo(70104, 62599, 120.70844), DeliveryInfo(41711, 87790, 117.11275), DeliveryInfo(55265, 97531, 149.54445), DeliveryInfo(59033, 84125, 60.44098), DeliveryInfo(62536, 88982, 146.62743), DeliveryInfo(24454, 1044, 52.67068), DeliveryInfo(68085, 93338, 66.56115), DeliveryInfo(48740, 13562, 173.16936), DeliveryInfo(50283, 87043, 58.36174), DeliveryInfo(14179, 19604, 179.81543), DeliveryInfo(17558, 99377, 70.16290), DeliveryInfo(17800, 52031, 33.66113), DeliveryInfo(48519, 60659, 56.32948), DeliveryInfo(94993, 9005, 80.24043), DeliveryInfo(40339, 59901, 85.63799), DeliveryInfo(7971, 43353, 31.29645), DeliveryInfo(107744, 12090, 110.95179), DeliveryInfo(50962, 3364, 153.99826), DeliveryInfo(26171, 63132, 50.29581), DeliveryInfo(45703, 7514, 28.93465), DeliveryInfo(108614, 72147, 104.31692), DeliveryInfo(36693, 82874, 82.04520), DeliveryInfo(38362, 104847, 3.77469), DeliveryInfo(31562, 15478, 8.01946), DeliveryInfo(28847, 19784, 107.45689), DeliveryInfo(20302, 788, 51.23975), DeliveryInfo(29944, 59881, 194.03067), DeliveryInfo(56696, 34639, 54.07952), DeliveryInfo(99187, 102249, 72.61247), DeliveryInfo(106883, 94519, 76.69012), DeliveryInfo(97506, 36211, 108.94017), DeliveryInfo(82337, 90270, 50.93311), DeliveryInfo(38448, 97305, 192.33228), DeliveryInfo(97438, 53342, 71.35098), DeliveryInfo(87922, 58398, 183.36356), DeliveryInfo(78527, 16642, 142.34731), DeliveryInfo(12111, 61250, 47.14893), DeliveryInfo(36306, 10366, 125.87531), DeliveryInfo(103854, 18617, 127.09382), DeliveryInfo(104671, 98949, 145.73836), DeliveryInfo(54156, 33735, 136.32069), DeliveryInfo(52214, 12268, 136.52774), DeliveryInfo(81025, 19163, 130.36644), DeliveryInfo(46017, 3466, 118.46980), DeliveryInfo(103299, 100878, 55.72112), DeliveryInfo(5207, 98197, 114.14323), DeliveryInfo(59268, 21829, 7.74652), DeliveryInfo(9088, 37355, 132.25296), DeliveryInfo(30362, 56987, 182.51825), DeliveryInfo(52852, 26433, 138.76147), DeliveryInfo(21142, 58391, 155.10136), DeliveryInfo(90026, 88186, 98.23347), DeliveryInfo(86296, 100049, 27.64576), DeliveryInfo(52607, 27793, 88.34814), DeliveryInfo(60805, 93912, 53.08633), DeliveryInfo(90686, 82458, 130.25894), DeliveryInfo(58155, 73527, 129.11908), DeliveryInfo(39037, 105310, 107.37556), DeliveryInfo(108282, 73556, 125.99871), DeliveryInfo(68697, 83613, 83.55355), DeliveryInfo(50843, 50400, 13.48232), DeliveryInfo(96372, 81149, 5.09741), DeliveryInfo(17720, 33118, 57.09571), DeliveryInfo(75827, 66081, 66.96934), DeliveryInfo(39544, 99952, 76.67171), DeliveryInfo(98705, 22647, 20.33341), DeliveryInfo(103298, 30594, 114.14059), DeliveryInfo(61813, 5120, 80.59782), DeliveryInfo(104428, 1708, 137.70113), DeliveryInfo(74157, 56701, 171.37102), DeliveryInfo(5162, 2487, 50.41157), DeliveryInfo(41485, 83233, 16.87761), DeliveryInfo(104461, 7506, 99.07015)};
        depots = {12, 46283, 736, 49864, 91418, 103248, 62581, 107776, 34239, 78113};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 537.726074219;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR hard_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_toronto_canada: INVALID" << std::endl;
        }

    } //hard_toronto_canada

    TEST(hard_multi_toronto_canada) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(74490, 99880, 138.55623), DeliveryInfo(60559, 55406, 47.71730), DeliveryInfo(15037, 75607, 161.93600), DeliveryInfo(49350, 106669, 12.78501), DeliveryInfo(24362, 86580, 145.21992), DeliveryInfo(4863, 68300, 110.70396), DeliveryInfo(989, 22727, 190.55290), DeliveryInfo(61012, 107566, 178.86870), DeliveryInfo(28282, 44826, 170.99500), DeliveryInfo(68915, 62657, 99.35236), DeliveryInfo(54844, 36713, 38.01253), DeliveryInfo(42243, 86580, 102.70174), DeliveryInfo(42243, 51728, 116.13731), DeliveryInfo(50192, 77791, 64.20718), DeliveryInfo(79849, 23822, 145.05844), DeliveryInfo(42243, 98287, 115.84576), DeliveryInfo(50192, 94398, 190.89732), DeliveryInfo(60037, 47068, 4.83141), DeliveryInfo(51610, 67285, 17.23112), DeliveryInfo(61155, 82273, 163.34363), DeliveryInfo(50192, 76398, 160.38022), DeliveryInfo(53109, 22340, 113.35343), DeliveryInfo(20806, 98765, 83.69699), DeliveryInfo(80415, 6145, 137.38342), DeliveryInfo(102853, 103362, 35.43953), DeliveryInfo(9586, 88341, 101.46423), DeliveryInfo(46806, 7906, 180.25812), DeliveryInfo(15037, 6566, 39.50509), DeliveryInfo(16454, 44690, 150.43883), DeliveryInfo(108448, 47770, 32.23473), DeliveryInfo(44546, 58034, 2.41940), DeliveryInfo(53709, 1224, 186.75096), DeliveryInfo(36870, 18735, 55.32803), DeliveryInfo(14679, 98805, 139.45200), DeliveryInfo(84125, 68385, 86.88641), DeliveryInfo(71511, 27827, 93.99104), DeliveryInfo(64614, 36280, 41.16446), DeliveryInfo(96318, 96376, 49.62871), DeliveryInfo(92611, 51018, 27.40766), DeliveryInfo(69352, 26541, 195.19963), DeliveryInfo(68616, 84210, 81.57967), DeliveryInfo(60773, 82828, 132.33929), DeliveryInfo(86395, 20466, 150.26027), DeliveryInfo(65021, 105662, 13.31292), DeliveryInfo(3909, 54939, 28.14578), DeliveryInfo(107584, 39136, 24.85013), DeliveryInfo(93336, 98805, 140.53529), DeliveryInfo(9727, 86580, 179.45335), DeliveryInfo(87576, 33352, 192.52917), DeliveryInfo(51692, 86580, 175.95291), DeliveryInfo(67807, 98608, 23.20482), DeliveryInfo(39444, 39219, 119.87338), DeliveryInfo(15037, 47378, 7.80324), DeliveryInfo(16103, 32640, 70.03410), DeliveryInfo(43215, 50531, 15.77850), DeliveryInfo(70831, 4562, 44.19549), DeliveryInfo(99932, 85643, 160.39336), DeliveryInfo(41171, 41462, 147.59537), DeliveryInfo(43567, 74323, 176.28851), DeliveryInfo(83186, 74292, 22.76558), DeliveryInfo(83735, 74333, 117.55574), DeliveryInfo(107067, 21261, 133.14215), DeliveryInfo(20433, 75263, 104.79792), DeliveryInfo(24366, 26525, 100.35497), DeliveryInfo(48014, 5404, 34.72157), DeliveryInfo(108031, 75459, 45.18030), DeliveryInfo(40558, 75119, 98.45424), DeliveryInfo(15037, 16936, 84.52962), DeliveryInfo(79978, 99530, 129.62335), DeliveryInfo(87051, 78564, 48.94826), DeliveryInfo(15804, 9007, 181.48770), DeliveryInfo(57286, 32310, 192.83679), DeliveryInfo(74175, 55738, 24.89543), DeliveryInfo(51371, 27010, 127.13496), DeliveryInfo(103884, 83418, 132.02815), DeliveryInfo(67216, 89383, 130.72821), DeliveryInfo(98564, 56452, 181.11974), DeliveryInfo(7650, 96967, 30.79873), DeliveryInfo(5132, 44689, 85.62118), DeliveryInfo(103834, 39741, 20.03627), DeliveryInfo(85195, 98805, 170.81677), DeliveryInfo(58489, 25809, 96.57871), DeliveryInfo(7572, 30252, 151.05038), DeliveryInfo(47448, 66262, 152.51259), DeliveryInfo(81784, 61805, 134.88603), DeliveryInfo(76221, 93630, 83.58484), DeliveryInfo(38064, 98805, 123.83329), DeliveryInfo(2719, 4738, 156.43973), DeliveryInfo(41357, 2725, 102.16376), DeliveryInfo(60356, 69739, 146.86534), DeliveryInfo(49782, 41567, 136.97188), DeliveryInfo(15037, 13982, 169.29582), DeliveryInfo(50192, 2266, 78.70354), DeliveryInfo(74264, 98805, 99.02190), DeliveryInfo(28917, 86324, 86.51377), DeliveryInfo(57705, 36493, 106.12165), DeliveryInfo(50331, 97481, 197.88016), DeliveryInfo(105649, 21235, 73.17490), DeliveryInfo(37191, 46690, 26.01777), DeliveryInfo(38892, 86580, 103.70360)};
        depots = {14, 55539, 66199};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 919.844726562;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR hard_multi_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_multi_toronto_canada: INVALID" << std::endl;
        }

    } //hard_multi_toronto_canada

} //hard_toronto_canada_public

