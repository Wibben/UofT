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


SUITE(hard_new_york_usa_public) {
    TEST(hard_new_york_usa) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(79274, 107852, 9.03236), DeliveryInfo(131836, 134224, 1.24750), DeliveryInfo(32838, 47873, 18.30515), DeliveryInfo(138718, 67681, 8.13210), DeliveryInfo(132549, 35145, 144.50546), DeliveryInfo(140236, 111773, 23.16188), DeliveryInfo(144731, 125343, 47.34474), DeliveryInfo(139508, 91431, 177.90135), DeliveryInfo(140562, 3339, 76.03653), DeliveryInfo(8759, 139465, 159.52058), DeliveryInfo(133197, 70118, 18.19793), DeliveryInfo(23579, 84064, 32.29961), DeliveryInfo(70975, 82252, 134.67482), DeliveryInfo(145857, 98741, 88.47421), DeliveryInfo(61374, 145411, 138.17381), DeliveryInfo(68726, 144688, 187.68988), DeliveryInfo(6992, 142079, 178.04446), DeliveryInfo(76136, 51517, 184.31548), DeliveryInfo(74214, 133453, 92.31055), DeliveryInfo(104898, 52422, 122.50576), DeliveryInfo(48755, 143532, 75.75082), DeliveryInfo(49275, 5303, 168.29533), DeliveryInfo(65453, 51632, 184.18819), DeliveryInfo(52380, 64944, 148.07014), DeliveryInfo(67525, 53104, 93.68166), DeliveryInfo(142846, 115886, 107.30930), DeliveryInfo(72726, 81458, 127.11867), DeliveryInfo(84039, 80414, 145.65105), DeliveryInfo(56014, 61796, 121.51439), DeliveryInfo(142593, 141347, 21.03427), DeliveryInfo(119816, 40211, 145.23701), DeliveryInfo(68437, 110300, 136.16119), DeliveryInfo(79590, 18212, 37.18279), DeliveryInfo(6932, 126928, 50.59377), DeliveryInfo(118070, 69872, 11.92303), DeliveryInfo(10704, 137200, 136.59579), DeliveryInfo(40773, 76143, 15.50330), DeliveryInfo(126339, 16263, 159.89265), DeliveryInfo(129417, 30226, 178.17670), DeliveryInfo(117234, 1059, 167.32101), DeliveryInfo(138719, 4518, 152.81595), DeliveryInfo(127565, 125766, 178.59151), DeliveryInfo(72012, 96886, 190.23483), DeliveryInfo(65156, 6876, 25.88994), DeliveryInfo(12204, 116890, 133.43823), DeliveryInfo(140279, 135468, 196.91699), DeliveryInfo(99585, 141419, 180.45494), DeliveryInfo(101827, 71632, 140.13742), DeliveryInfo(121781, 140798, 173.54956), DeliveryInfo(140637, 124359, 10.89069), DeliveryInfo(120394, 102795, 104.52218), DeliveryInfo(68277, 110732, 189.93277), DeliveryInfo(98565, 70645, 44.79901), DeliveryInfo(19041, 10266, 93.17857), DeliveryInfo(94142, 34444, 22.67771), DeliveryInfo(105453, 131867, 75.82985), DeliveryInfo(130940, 46516, 182.78758), DeliveryInfo(83422, 110570, 139.78836), DeliveryInfo(92252, 32184, 124.38094), DeliveryInfo(45980, 111418, 192.43457), DeliveryInfo(138650, 83008, 9.16011), DeliveryInfo(23903, 23796, 167.58372), DeliveryInfo(78096, 80441, 34.02677), DeliveryInfo(81654, 109620, 106.18466), DeliveryInfo(54171, 20786, 39.56749), DeliveryInfo(130848, 31140, 162.15407), DeliveryInfo(28391, 117893, 138.94620), DeliveryInfo(30536, 108808, 71.78390), DeliveryInfo(38738, 120895, 80.35440), DeliveryInfo(27263, 47280, 187.23871), DeliveryInfo(55710, 141103, 199.40604), DeliveryInfo(83979, 111902, 128.67783), DeliveryInfo(42384, 119493, 7.08560), DeliveryInfo(17255, 87017, 28.90478), DeliveryInfo(122764, 16474, 62.85184)};
        depots = {16, 62153, 988, 66961};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 890.142700195;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR hard_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_new_york_usa: INVALID" << std::endl;
        }

    } //hard_new_york_usa

    TEST(hard_multi_new_york_usa) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(13062, 129422, 197.83781), DeliveryInfo(112446, 37369, 29.55739), DeliveryInfo(71319, 3044, 164.85197), DeliveryInfo(6531, 36271, 169.42699), DeliveryInfo(67403, 6127, 10.47738), DeliveryInfo(90264, 84142, 153.56921), DeliveryInfo(66852, 100031, 139.74147), DeliveryInfo(143779, 35642, 60.18294), DeliveryInfo(10169, 27484, 90.83884), DeliveryInfo(68986, 116018, 81.49701), DeliveryInfo(144473, 28551, 141.96619), DeliveryInfo(93132, 144449, 49.75852), DeliveryInfo(139504, 115923, 93.40656), DeliveryInfo(107402, 132360, 27.79426), DeliveryInfo(134198, 99609, 56.81369), DeliveryInfo(96031, 109826, 53.48162), DeliveryInfo(49944, 22743, 11.24384), DeliveryInfo(72125, 107228, 151.42471), DeliveryInfo(63718, 109826, 21.83752), DeliveryInfo(99728, 74404, 118.75954), DeliveryInfo(114407, 62700, 41.60979), DeliveryInfo(107989, 52667, 146.16904), DeliveryInfo(56727, 60014, 125.80856), DeliveryInfo(92545, 91057, 105.22785), DeliveryInfo(116900, 77491, 53.65336), DeliveryInfo(141875, 109717, 101.17857), DeliveryInfo(6891, 27439, 190.77266), DeliveryInfo(145633, 109826, 187.00691), DeliveryInfo(69306, 63207, 10.90100), DeliveryInfo(27940, 102594, 1.95440), DeliveryInfo(32716, 69465, 141.02583), DeliveryInfo(81052, 113084, 57.68417), DeliveryInfo(52969, 34658, 72.41798), DeliveryInfo(67403, 91719, 88.77116), DeliveryInfo(10273, 55537, 72.54241), DeliveryInfo(12873, 100876, 94.78125), DeliveryInfo(38832, 134127, 185.81726), DeliveryInfo(52969, 28517, 185.24112), DeliveryInfo(49512, 82997, 174.28442), DeliveryInfo(66271, 21223, 83.39344), DeliveryInfo(22096, 49007, 84.61935), DeliveryInfo(73649, 8817, 60.71252), DeliveryInfo(81324, 132630, 54.30763), DeliveryInfo(69416, 141893, 83.91496), DeliveryInfo(82124, 8252, 58.60396), DeliveryInfo(64478, 99609, 71.87118), DeliveryInfo(62856, 138804, 93.81203), DeliveryInfo(129344, 92144, 0.78522), DeliveryInfo(54466, 133657, 103.37914), DeliveryInfo(37979, 74850, 14.30980), DeliveryInfo(102357, 132684, 148.28180), DeliveryInfo(139437, 77933, 111.06792), DeliveryInfo(80623, 88983, 160.12587), DeliveryInfo(52969, 110483, 36.30040), DeliveryInfo(52969, 99609, 56.54592), DeliveryInfo(67403, 43832, 128.16492), DeliveryInfo(111709, 75808, 48.25947), DeliveryInfo(19713, 6363, 133.06882), DeliveryInfo(21624, 55679, 165.42661), DeliveryInfo(125340, 32721, 107.91933), DeliveryInfo(1328, 67858, 173.75882), DeliveryInfo(67403, 5250, 111.81429), DeliveryInfo(112970, 112020, 187.21326), DeliveryInfo(117605, 55289, 170.99582), DeliveryInfo(58034, 101332, 139.34511), DeliveryInfo(52228, 99766, 127.74605), DeliveryInfo(67589, 129886, 29.25713), DeliveryInfo(124366, 104465, 70.92149), DeliveryInfo(86770, 99609, 50.99213), DeliveryInfo(81933, 116267, 41.10047), DeliveryInfo(3651, 76928, 160.83212), DeliveryInfo(87316, 81612, 127.42159), DeliveryInfo(58506, 109826, 167.48099), DeliveryInfo(95118, 99609, 74.58488), DeliveryInfo(59820, 109826, 86.73544)};
        depots = {19, 74583, 88898, 51116, 118080, 20194, 13135};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 391.997894287;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR hard_multi_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_multi_new_york_usa: INVALID" << std::endl;
        }

    } //hard_multi_new_york_usa

} //hard_new_york_usa_public

