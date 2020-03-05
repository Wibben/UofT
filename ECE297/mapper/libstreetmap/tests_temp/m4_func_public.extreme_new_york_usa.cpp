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


SUITE(extreme_new_york_usa_public) {
    TEST(extreme_new_york_usa) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(18613, 89096, 57.29919), DeliveryInfo(8076, 68261, 88.81586), DeliveryInfo(11856, 57956, 30.48721), DeliveryInfo(7548, 31086, 47.98988), DeliveryInfo(95001, 108810, 119.71503), DeliveryInfo(82326, 28861, 164.35193), DeliveryInfo(42923, 33945, 31.94946), DeliveryInfo(67283, 73848, 32.62012), DeliveryInfo(62732, 58169, 5.67174), DeliveryInfo(108383, 138250, 180.40195), DeliveryInfo(20896, 110851, 182.32812), DeliveryInfo(59583, 77503, 161.09662), DeliveryInfo(23383, 38253, 95.22641), DeliveryInfo(34291, 95837, 74.08122), DeliveryInfo(58319, 26798, 174.69424), DeliveryInfo(64297, 136385, 66.28336), DeliveryInfo(50671, 53691, 164.19490), DeliveryInfo(17935, 73006, 52.16916), DeliveryInfo(102299, 56645, 57.45093), DeliveryInfo(73777, 146110, 13.90317), DeliveryInfo(121666, 22808, 119.98164), DeliveryInfo(24213, 111526, 33.65812), DeliveryInfo(64756, 136355, 111.07706), DeliveryInfo(67291, 124884, 0.55406), DeliveryInfo(37514, 94718, 145.10243), DeliveryInfo(50829, 135777, 39.22399), DeliveryInfo(1466, 142598, 181.20567), DeliveryInfo(123648, 68504, 178.69272), DeliveryInfo(126502, 114426, 76.52003), DeliveryInfo(2155, 27769, 98.73018), DeliveryInfo(63880, 57083, 4.67578), DeliveryInfo(12526, 141323, 104.58342), DeliveryInfo(143258, 123215, 146.08705), DeliveryInfo(121581, 143622, 168.30269), DeliveryInfo(65665, 9969, 139.17325), DeliveryInfo(76984, 116440, 31.52648), DeliveryInfo(93342, 107911, 14.83639), DeliveryInfo(94661, 46298, 167.56364), DeliveryInfo(85756, 41102, 64.33760), DeliveryInfo(141318, 2441, 40.34159), DeliveryInfo(120191, 103909, 128.71513), DeliveryInfo(128688, 11389, 8.30555), DeliveryInfo(35685, 28983, 117.14552), DeliveryInfo(38758, 50289, 130.90321), DeliveryInfo(144690, 144894, 29.29015), DeliveryInfo(88447, 32775, 64.57568), DeliveryInfo(128470, 125279, 132.59097), DeliveryInfo(11123, 58311, 98.92383), DeliveryInfo(7730, 139882, 152.12097), DeliveryInfo(26862, 66350, 31.55325), DeliveryInfo(67939, 126815, 106.73778), DeliveryInfo(108518, 85319, 139.55220), DeliveryInfo(139469, 69195, 124.24613), DeliveryInfo(39028, 37223, 85.20247), DeliveryInfo(79778, 5446, 8.32127), DeliveryInfo(122377, 101023, 76.18945), DeliveryInfo(3656, 124030, 140.65887), DeliveryInfo(19476, 45077, 144.60754), DeliveryInfo(53261, 133572, 150.82446), DeliveryInfo(3730, 29082, 47.91736), DeliveryInfo(133861, 59242, 19.01476), DeliveryInfo(32900, 59610, 61.74179), DeliveryInfo(95677, 58758, 138.00890), DeliveryInfo(49048, 106817, 27.29518), DeliveryInfo(58498, 41974, 165.42043), DeliveryInfo(109398, 53252, 9.15159), DeliveryInfo(129435, 120680, 156.45778), DeliveryInfo(112017, 114933, 173.52544), DeliveryInfo(5528, 77768, 46.76288), DeliveryInfo(47403, 11346, 90.98752), DeliveryInfo(10776, 15414, 58.77391), DeliveryInfo(107892, 59579, 75.08154), DeliveryInfo(44794, 3439, 60.96470), DeliveryInfo(35213, 132734, 27.33279), DeliveryInfo(67593, 76258, 180.89139), DeliveryInfo(143664, 135969, 7.96507), DeliveryInfo(14253, 93678, 82.06094), DeliveryInfo(14009, 92141, 163.21872), DeliveryInfo(38639, 36573, 131.35271), DeliveryInfo(82158, 87431, 126.85066), DeliveryInfo(101365, 64676, 8.16927), DeliveryInfo(39421, 52923, 138.96854), DeliveryInfo(100487, 110006, 150.64841), DeliveryInfo(25185, 79164, 148.80296), DeliveryInfo(8733, 114433, 67.69508), DeliveryInfo(23373, 120369, 109.22731), DeliveryInfo(49855, 83131, 111.32298), DeliveryInfo(72104, 20596, 71.18803), DeliveryInfo(83261, 115070, 117.54702), DeliveryInfo(120513, 145366, 112.25629), DeliveryInfo(42499, 40241, 123.20718), DeliveryInfo(66259, 122763, 133.63197), DeliveryInfo(49824, 107935, 148.52510), DeliveryInfo(32608, 105397, 55.43435), DeliveryInfo(35491, 34355, 71.29066), DeliveryInfo(3545, 104575, 71.20758), DeliveryInfo(6779, 15558, 61.35959), DeliveryInfo(125217, 121917, 88.74390), DeliveryInfo(89133, 2723, 156.94028), DeliveryInfo(129597, 122330, 64.57832), DeliveryInfo(57995, 111715, 59.72168), DeliveryInfo(22669, 23569, 25.44969), DeliveryInfo(116587, 67758, 82.18613), DeliveryInfo(116134, 15607, 6.61413), DeliveryInfo(65828, 53341, 70.59698), DeliveryInfo(125022, 40045, 186.96368), DeliveryInfo(119232, 122314, 124.51246), DeliveryInfo(37214, 49998, 141.04437), DeliveryInfo(77693, 1117, 153.01183), DeliveryInfo(145209, 114364, 33.91280), DeliveryInfo(52771, 72834, 4.93460), DeliveryInfo(52355, 123450, 197.96411), DeliveryInfo(52328, 57126, 125.52868), DeliveryInfo(46995, 54476, 194.99478), DeliveryInfo(11001, 47250, 193.28387), DeliveryInfo(50204, 3154, 5.59765), DeliveryInfo(41130, 54077, 4.03195), DeliveryInfo(126580, 65809, 26.48119), DeliveryInfo(11088, 122361, 73.74551), DeliveryInfo(1698, 94027, 169.69832), DeliveryInfo(16805, 13076, 107.75479), DeliveryInfo(33577, 71217, 31.79602), DeliveryInfo(92864, 91098, 25.79854), DeliveryInfo(2277, 20926, 121.18573), DeliveryInfo(40548, 76259, 156.39026), DeliveryInfo(80422, 17794, 114.04040), DeliveryInfo(30260, 124046, 44.37099), DeliveryInfo(50844, 83260, 32.12513)};
        depots = {117723};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2872.663574219;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR extreme_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_new_york_usa: INVALID" << std::endl;
        }

    } //extreme_new_york_usa

    TEST(extreme_multi_new_york_usa) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(47523, 118644, 172.47223), DeliveryInfo(1747, 42677, 6.87302), DeliveryInfo(21494, 74426, 167.53369), DeliveryInfo(55085, 114750, 18.53877), DeliveryInfo(86381, 131003, 85.18672), DeliveryInfo(1910, 62886, 48.05904), DeliveryInfo(17514, 124892, 57.67505), DeliveryInfo(90557, 118644, 32.03027), DeliveryInfo(37242, 61116, 133.02754), DeliveryInfo(46444, 49640, 172.29903), DeliveryInfo(112189, 129286, 46.64079), DeliveryInfo(96167, 20422, 197.70428), DeliveryInfo(137188, 91383, 183.23151), DeliveryInfo(17514, 86648, 167.93733), DeliveryInfo(35513, 10036, 103.05208), DeliveryInfo(55085, 128807, 27.02585), DeliveryInfo(31031, 141357, 164.76216), DeliveryInfo(15093, 84276, 34.40419), DeliveryInfo(37242, 49640, 124.43941), DeliveryInfo(25321, 42294, 14.43604), DeliveryInfo(118366, 5437, 42.00080), DeliveryInfo(15093, 362, 20.95934), DeliveryInfo(96167, 22679, 128.45171), DeliveryInfo(133375, 99650, 91.97070), DeliveryInfo(56898, 76251, 117.84732), DeliveryInfo(104725, 36132, 7.98950), DeliveryInfo(107498, 71746, 61.24446), DeliveryInfo(30293, 78184, 131.48782), DeliveryInfo(108711, 42620, 48.48662), DeliveryInfo(132812, 61116, 97.80152), DeliveryInfo(134502, 96222, 177.11232), DeliveryInfo(78009, 17090, 188.52264), DeliveryInfo(37242, 136686, 176.39389), DeliveryInfo(98199, 107134, 109.64389), DeliveryInfo(114071, 6699, 19.97214), DeliveryInfo(85375, 133077, 75.02955), DeliveryInfo(49825, 71746, 151.50095), DeliveryInfo(128112, 113690, 102.67147), DeliveryInfo(104725, 22886, 54.41062), DeliveryInfo(104725, 21207, 55.05885), DeliveryInfo(5864, 113585, 145.94620), DeliveryInfo(128041, 143902, 169.46432), DeliveryInfo(75012, 34242, 12.37167), DeliveryInfo(77026, 93476, 192.78342), DeliveryInfo(46444, 129365, 48.37100), DeliveryInfo(17514, 61116, 116.66487), DeliveryInfo(118366, 81141, 130.18469), DeliveryInfo(74664, 117914, 144.73796), DeliveryInfo(85983, 19898, 45.96376), DeliveryInfo(31031, 50847, 116.57906), DeliveryInfo(104725, 67001, 187.76631), DeliveryInfo(104725, 133095, 67.74371), DeliveryInfo(118366, 106016, 56.76450), DeliveryInfo(55085, 83623, 79.22715), DeliveryInfo(107498, 98303, 173.77060), DeliveryInfo(104725, 19918, 81.13544), DeliveryInfo(25401, 95350, 89.08516), DeliveryInfo(96167, 113585, 29.78330), DeliveryInfo(46444, 57793, 69.77087), DeliveryInfo(118366, 13205, 109.76553), DeliveryInfo(108711, 86648, 91.79794), DeliveryInfo(17514, 86263, 178.59599), DeliveryInfo(1910, 133095, 7.44812), DeliveryInfo(80752, 121845, 128.45900), DeliveryInfo(23952, 16561, 44.20446), DeliveryInfo(108069, 84832, 193.33188), DeliveryInfo(43253, 76140, 123.58534), DeliveryInfo(109499, 58303, 188.27849), DeliveryInfo(85315, 113690, 190.92882), DeliveryInfo(30725, 137529, 125.14909), DeliveryInfo(104725, 133077, 71.96139), DeliveryInfo(111017, 91433, 48.15688), DeliveryInfo(92408, 126263, 180.71648), DeliveryInfo(83808, 130959, 165.21184), DeliveryInfo(103, 33748, 141.10423), DeliveryInfo(109244, 54312, 42.22682), DeliveryInfo(46444, 19054, 130.78293), DeliveryInfo(46444, 59321, 22.87063), DeliveryInfo(37242, 136191, 188.42975), DeliveryInfo(17288, 113690, 92.45476), DeliveryInfo(48362, 131003, 83.63033), DeliveryInfo(43622, 51860, 119.82873), DeliveryInfo(92555, 62886, 52.57827), DeliveryInfo(31031, 61116, 5.75181), DeliveryInfo(97706, 91383, 45.54598), DeliveryInfo(35563, 133095, 150.04921), DeliveryInfo(66964, 36051, 25.36825), DeliveryInfo(104725, 109350, 150.83109), DeliveryInfo(3620, 66967, 167.31085), DeliveryInfo(28649, 96222, 62.09114), DeliveryInfo(46444, 50847, 1.18127), DeliveryInfo(107498, 86648, 20.96711), DeliveryInfo(139207, 101034, 103.45560), DeliveryInfo(17417, 47935, 105.32626), DeliveryInfo(26718, 32650, 3.63235), DeliveryInfo(65893, 8770, 137.24721), DeliveryInfo(55085, 145287, 59.81156), DeliveryInfo(51903, 126535, 163.88914), DeliveryInfo(46412, 59635, 93.15637), DeliveryInfo(37242, 72074, 151.34332), DeliveryInfo(87614, 116390, 93.25175), DeliveryInfo(37242, 13205, 155.45399), DeliveryInfo(104725, 57030, 119.74492), DeliveryInfo(142971, 95730, 6.89595), DeliveryInfo(57917, 113585, 74.40570), DeliveryInfo(128748, 99598, 37.41897), DeliveryInfo(96167, 62886, 50.98932), DeliveryInfo(8485, 78102, 105.53154), DeliveryInfo(4868, 121367, 113.12485), DeliveryInfo(108711, 131003, 49.58840), DeliveryInfo(17514, 96222, 81.54669), DeliveryInfo(46364, 130655, 140.15517), DeliveryInfo(47245, 131003, 29.78359), DeliveryInfo(37242, 112771, 83.94995), DeliveryInfo(55085, 99650, 148.23445), DeliveryInfo(51776, 133095, 25.02138), DeliveryInfo(55085, 107134, 6.98958), DeliveryInfo(55729, 9527, 193.90492), DeliveryInfo(91732, 113690, 184.76707), DeliveryInfo(118366, 80261, 90.95434), DeliveryInfo(96167, 133077, 57.02316), DeliveryInfo(133686, 137656, 164.79451), DeliveryInfo(118366, 42620, 195.93291), DeliveryInfo(99800, 104730, 77.66193), DeliveryInfo(137277, 129834, 19.11588), DeliveryInfo(37242, 69691, 142.45247), DeliveryInfo(96167, 73507, 122.80174), DeliveryInfo(8837, 1810, 163.08661), DeliveryInfo(107498, 139332, 153.49013), DeliveryInfo(46444, 40001, 121.76962), DeliveryInfo(28649, 133095, 141.55963), DeliveryInfo(102571, 95375, 25.14473), DeliveryInfo(118366, 118644, 161.31839), DeliveryInfo(78191, 81618, 0.57271), DeliveryInfo(94474, 63203, 45.31679), DeliveryInfo(46527, 113568, 86.77325), DeliveryInfo(118366, 47147, 31.42550), DeliveryInfo(103911, 133077, 140.40607), DeliveryInfo(83600, 15217, 140.83707), DeliveryInfo(105835, 91383, 146.46410), DeliveryInfo(89136, 91383, 168.46269), DeliveryInfo(39633, 91941, 62.92023), DeliveryInfo(28649, 39899, 22.52594), DeliveryInfo(24990, 86648, 149.77150), DeliveryInfo(115321, 96222, 19.89136), DeliveryInfo(118531, 32650, 175.68759), DeliveryInfo(75001, 71158, 15.41175), DeliveryInfo(52787, 113851, 140.54611), DeliveryInfo(104725, 104151, 101.40900), DeliveryInfo(90118, 74426, 114.02425), DeliveryInfo(71624, 48568, 64.53047), DeliveryInfo(107498, 68144, 150.34753), DeliveryInfo(51488, 74426, 25.63767), DeliveryInfo(107498, 49640, 156.05685), DeliveryInfo(135432, 36686, 20.65192), DeliveryInfo(57436, 76140, 88.95892), DeliveryInfo(39334, 129834, 135.68217), DeliveryInfo(94940, 86648, 114.32783), DeliveryInfo(55085, 71746, 118.44542), DeliveryInfo(26376, 112571, 78.73593), DeliveryInfo(96720, 75611, 62.04766), DeliveryInfo(78095, 76140, 102.89741), DeliveryInfo(68364, 129834, 160.88499), DeliveryInfo(28649, 61116, 79.64287), DeliveryInfo(136476, 57852, 40.86706), DeliveryInfo(15093, 94233, 93.99148), DeliveryInfo(116422, 60618, 62.61290), DeliveryInfo(46444, 78105, 187.59169), DeliveryInfo(38068, 12463, 38.06366), DeliveryInfo(122293, 139223, 170.68022), DeliveryInfo(28649, 74426, 104.91049), DeliveryInfo(49853, 139143, 134.31958), DeliveryInfo(4440, 85828, 140.19168), DeliveryInfo(57234, 39192, 192.91693), DeliveryInfo(26995, 107134, 93.20885), DeliveryInfo(118366, 63193, 84.43138), DeliveryInfo(108078, 91383, 187.01712), DeliveryInfo(58442, 57406, 103.52294), DeliveryInfo(31031, 103902, 156.10333), DeliveryInfo(17514, 76140, 64.45344), DeliveryInfo(45519, 122427, 32.11029), DeliveryInfo(37242, 77823, 195.99121), DeliveryInfo(108816, 80749, 91.87255), DeliveryInfo(99229, 61160, 179.70970), DeliveryInfo(23892, 50847, 167.01508), DeliveryInfo(74239, 126677, 184.75804), DeliveryInfo(55085, 33497, 55.42635), DeliveryInfo(43892, 131003, 85.29504), DeliveryInfo(79053, 74426, 76.98441), DeliveryInfo(135481, 53004, 114.15056), DeliveryInfo(141086, 50610, 161.57170), DeliveryInfo(118366, 5328, 28.03493)};
        depots = {26, 99444};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 3378.359619141;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, right_turn_penalty, left_turn_penalty);
        	std::cout << "QoR extreme_multi_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_multi_new_york_usa: INVALID" << std::endl;
        }

    } //extreme_multi_new_york_usa

} //extreme_new_york_usa_public

