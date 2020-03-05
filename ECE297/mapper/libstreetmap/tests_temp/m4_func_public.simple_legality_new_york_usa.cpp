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


SUITE(simple_legality_new_york_usa_public) {
    TEST(simple_legality_new_york_usa) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        std::vector<CourierSubpath> result_path;
        float right_turn_penalty;
        float left_turn_penalty;
        float truck_capacity;

        deliveries = {DeliveryInfo(11880, 12456, 50.03543), DeliveryInfo(14610, 68733, 60.50912), DeliveryInfo(141668, 136099, 35.54058)};
        depots = {28047, 43295, 38755};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 13465.874023438;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(8634, 24721, 68.27714), DeliveryInfo(141376, 107083, 5.74371), DeliveryInfo(134111, 105311, 54.44788), DeliveryInfo(589, 94229, 53.46008), DeliveryInfo(110938, 131434, 171.11781)};
        depots = {132800, 125683, 95240};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 8434.584960938;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(24861, 29632, 156.64452)};
        depots = {6};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(8459, 59736, 182.53178), DeliveryInfo(23987, 118053, 192.09618), DeliveryInfo(31582, 60654, 74.25520), DeliveryInfo(44218, 144441, 172.51857), DeliveryInfo(103523, 79704, 44.38072)};
        depots = {52573, 108225, 39078};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 7508.874511719;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(17789, 121623, 122.91624), DeliveryInfo(17789, 74425, 47.65577), DeliveryInfo(68748, 118014, 162.54781), DeliveryInfo(140863, 34805, 27.45177), DeliveryInfo(4974, 96651, 126.61769), DeliveryInfo(17789, 54923, 191.67456), DeliveryInfo(140863, 141309, 124.65140), DeliveryInfo(48688, 10669, 90.33015)};
        depots = {14463, 110999, 21208};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 10859.388671875;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(27017, 27374, 115.91381), DeliveryInfo(32010, 126381, 75.65656)};
        depots = {90184};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 1328.184082031;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(27204, 115970, 35.08236), DeliveryInfo(79443, 57875, 60.36134), DeliveryInfo(79081, 115887, 102.00575)};
        depots = {35742, 50479, 33343};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 8968.174804688;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(55460, 4378, 159.20335)};
        depots = {136817};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(57890, 71348, 28.59369), DeliveryInfo(48149, 12090, 46.00364), DeliveryInfo(50771, 106963, 41.82777)};
        depots = {2121, 65064, 60281};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5104.119140625;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(66138, 136424, 4.68502), DeliveryInfo(86864, 124565, 150.65356)};
        depots = {24041};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 14852.536132812;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(68180, 41652, 55.20467), DeliveryInfo(133736, 115310, 99.69247), DeliveryInfo(60479, 128542, 6.81267), DeliveryInfo(53288, 128542, 119.94157), DeliveryInfo(52344, 128542, 54.26281), DeliveryInfo(58003, 71886, 196.75076), DeliveryInfo(129186, 71886, 155.84526), DeliveryInfo(87445, 63050, 184.55672)};
        depots = {56073, 60390, 2381};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 10305.068359375;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(69968, 91843, 175.53738)};
        depots = {34119, 23138};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(81971, 24549, 72.53727)};
        depots = {56094, 65377};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(71196, 26181, 24.15597)};
        depots = {76866};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(73998, 86069, 131.37521), DeliveryInfo(29661, 17518, 117.05125), DeliveryInfo(18559, 120613, 77.03854), DeliveryInfo(44709, 2939, 59.02165), DeliveryInfo(73998, 21069, 13.02049), DeliveryInfo(73998, 2750, 112.16380), DeliveryInfo(76394, 86423, 34.13472), DeliveryInfo(18559, 82927, 127.61972)};
        depots = {73530, 115013, 94997};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 13573.001953125;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(104315, 6054, 16.54290), DeliveryInfo(34240, 43722, 142.46921), DeliveryInfo(46908, 42869, 131.73322), DeliveryInfo(103111, 77935, 93.85989), DeliveryInfo(50630, 17063, 110.77976)};
        depots = {118627, 120358, 58758};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 10332.823242188;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(106394, 33758, 148.86475), DeliveryInfo(35382, 105267, 186.24405), DeliveryInfo(35420, 42160, 49.75138), DeliveryInfo(26972, 105267, 186.60216), DeliveryInfo(106394, 11626, 124.07938), DeliveryInfo(136912, 100572, 35.82661), DeliveryInfo(106394, 42160, 50.41379), DeliveryInfo(106394, 42160, 50.41379), DeliveryInfo(35420, 105267, 90.22083)};
        depots = {2071, 8450, 76850};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5430.350585938;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(110530, 93861, 19.22806), DeliveryInfo(110530, 143938, 113.67527), DeliveryInfo(42463, 69047, 157.47266), DeliveryInfo(14792, 24651, 90.84747), DeliveryInfo(69345, 24651, 162.92372), DeliveryInfo(110530, 143938, 113.67527), DeliveryInfo(25638, 24651, 129.00230), DeliveryInfo(25638, 143938, 70.30033), DeliveryInfo(110530, 94002, 67.31814)};
        depots = {10138, 84342, 128110};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 7369.531250000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(112140, 3941, 91.05830), DeliveryInfo(89202, 104405, 75.33926), DeliveryInfo(107292, 139366, 101.23453), DeliveryInfo(122894, 121083, 92.10358), DeliveryInfo(140514, 127972, 131.64763)};
        depots = {69807, 74584, 145670};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 12779.961914062;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(111234, 7128, 35.97311), DeliveryInfo(111234, 109216, 57.91961), DeliveryInfo(111234, 114737, 37.27143), DeliveryInfo(57573, 128684, 44.35627), DeliveryInfo(31045, 128684, 113.11504), DeliveryInfo(61152, 107471, 129.07108), DeliveryInfo(39185, 128684, 190.33194), DeliveryInfo(57573, 109216, 121.80888), DeliveryInfo(111234, 109216, 57.91961)};
        depots = {84856, 124518, 39013};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 13193.651367188;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(113962, 55803, 66.15946), DeliveryInfo(134066, 143498, 45.42579)};
        depots = {19502, 97273};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 11119.174804688;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(129164, 10905, 1.66533), DeliveryInfo(136659, 23102, 69.64507)};
        depots = {37656, 51026};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2802.345214844;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(25001, 55206, 159.62805), DeliveryInfo(48439, 132115, 6.15951), DeliveryInfo(84719, 79442, 125.63981), DeliveryInfo(142496, 54526, 159.22833), DeliveryInfo(124306, 140773, 111.09024)};
        depots = {79341, 102451, 63409};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 3171.211669922;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(87923, 74768, 173.83640), DeliveryInfo(75602, 144426, 56.51704), DeliveryInfo(119438, 113780, 102.99988), DeliveryInfo(45579, 53582, 127.61862), DeliveryInfo(88383, 53582, 44.21967), DeliveryInfo(72522, 137833, 78.60152), DeliveryInfo(49511, 113780, 18.57643), DeliveryInfo(88248, 53582, 107.25372)};
        depots = {115361, 73545, 135822};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 10054.899414062;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(117314, 34589, 46.58144), DeliveryInfo(131505, 45107, 168.12914), DeliveryInfo(138737, 118652, 176.08020), DeliveryInfo(37416, 45107, 38.50279), DeliveryInfo(88894, 118652, 94.24657), DeliveryInfo(14663, 119590, 161.54309), DeliveryInfo(60852, 71486, 132.61261), DeliveryInfo(51281, 45107, 167.31036)};
        depots = {138032, 46521, 70928};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 10738.934570312;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(143959, 79773, 28.66476)};
        depots = {29105, 100847};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 5000.000000000;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(118950, 85965, 184.61324), DeliveryInfo(127546, 121231, 65.88518)};
        depots = {116802};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 10739.620117188;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(130322, 69093, 76.93291), DeliveryInfo(127629, 38115, 28.54039), DeliveryInfo(122349, 133240, 73.34542), DeliveryInfo(33372, 89976, 56.93892), DeliveryInfo(67904, 24126, 98.85957)};
        depots = {80896, 31851, 44128};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 3812.814697266;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(143937, 22529, 65.56896), DeliveryInfo(58042, 97900, 182.31299)};
        depots = {120453, 119548};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 2286.546142578;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

        deliveries = {DeliveryInfo(128675, 92868, 20.42503), DeliveryInfo(88739, 131302, 120.76764), DeliveryInfo(88739, 10173, 174.53944), DeliveryInfo(33269, 70112, 166.97858), DeliveryInfo(52268, 6258, 193.52501), DeliveryInfo(88739, 30611, 94.40701), DeliveryInfo(98005, 96477, 45.52258), DeliveryInfo(33269, 15279, 22.48336)};
        depots = {135794, 64969, 131148};
        right_turn_penalty = 15.000000000;
        left_turn_penalty = 15.000000000;
        truck_capacity = 10248.227539062;
        result_path = traveling_courier(deliveries, depots, right_turn_penalty, left_turn_penalty, truck_capacity);
        CHECK(courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity));

    } //simple_legality_new_york_usa

} //simple_legality_new_york_usa_public

