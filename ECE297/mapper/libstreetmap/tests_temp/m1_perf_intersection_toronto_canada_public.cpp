
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(intersection_queries_perf_public_toronto_canada) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                load_map("/cad2/ece297s/public/maps//toronto_canada.streets.bin");
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test setup: load_map threw an exceptinon !!!!" << std::endl;
                throw; // re-throw exceptinon
            }
        }
    
        ~BaseMapFixture() {
            //Clean-up
            try {
                close_map();
            } catch (const std::exception& e) {
                std::cout << "!!!! BaseMapFixture test teardown: close_map threw an exceptinon. what(): " << e.what() << " !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            } catch (...) {
                std::cout << "!!!! BaseMapFixture test teardown: close_map threw an exceptinon !!!!" << std::endl;
                std::terminate(); // we're in a destructor
            }
        }
    };


    struct MapFixture : BaseMapFixture {
        MapFixture()
            : BaseMapFixture()
            , rng(3)
            , rand_intersection(0, getNumIntersections()-1)
            , rand_street(1, getNumStreets()-1) // Start from 1 to avoid getting id 0 (<unknown>)
            , rand_segment(0, getNumStreetSegments()-1)
            , rand_poi(0, getNumPointsOfInterest()-1)
            , rand_lat(43.479999542, 43.919979095)
            , rand_lon(-79.789978027, -79.000007629)
        { }

        std::minstd_rand rng;
        std::uniform_int_distribution<int> rand_intersection;
        std::uniform_int_distribution<int> rand_street;
        std::uniform_int_distribution<int> rand_segment;
        std::uniform_int_distribution<int> rand_poi;
        std::uniform_real_distribution<double> rand_lat;
        std::uniform_real_distribution<double> rand_lon;
    };

    TEST_FIXTURE(MapFixture, intersection_street_segments_perf) {
        //Verify Functionality
        std::vector<unsigned> expected;

        expected = {4, 136438};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(7)));

        expected = {18588, 18598, 103969, 114105};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(19446)));

        expected = {20364, 57744, 57746, 86784};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(21121)));

        expected = {24373, 24374, 151729};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(25096)));

        expected = {27172, 27173, 32957};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(27769)));

        expected = {28811, 28812, 37168};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(29244)));

        expected = {30037, 30038};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(30279)));

        expected = {50680, 50728};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(46668)));

        expected = {58170, 58202, 58204};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(53792)));

        expected = {64846, 64847};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(58255)));

        expected = {66448, 66449, 115607};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(59320)));

        expected = {70514, 70538, 70543};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(61948)));

        expected = {71315, 71326, 71327};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(62481)));

        expected = {80302, 80304, 80307, 80312};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(68571)));

        expected = {88902, 88903, 88904};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(73462)));

        expected = {89904, 89905, 144677, 144678};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(74152)));

        expected = {92678, 92785, 92834};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(76038)));

        expected = {93396, 93397, 156235};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(76703)));

        expected = {96304, 96325, 96326, 96379};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(78437)));

        expected = {101180, 101181, 101182};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(81284)));

        expected = {106843, 106844, 106848};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(84448)));

        expected = {108860};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(85859)));

        expected = {111762, 111767, 111768};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(87529)));

        expected = {128042, 128043, 128048};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(96023)));

        expected = {128537, 128538, 128560, 128561};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(96297)));

        expected = {130034, 130040, 130041};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(97027)));

        expected = {131940, 131941, 131946, 131947};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(97956)));

        expected = {132745, 145459};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(98395)));

        expected = {145836, 145837, 145847};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(104396)));

        expected = {153426, 153427};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(107254)));

        //Generate random inputs
        std::vector<unsigned> intersection_ids;
        for(size_t i = 0; i < 15000000; i++) {
            intersection_ids.push_back(rand_intersection(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(1500);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 15000000; i++) {
                result = find_intersection_street_segments(intersection_ids[i]);
            }
        }
    } //intersection_street_segments_perf

    TEST_FIXTURE(MapFixture, intersection_street_names_perf) {
        //Verify Functionality
        std::vector<std::string> expected;

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(85859)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(46668)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(98395)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(62481)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(73462)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(96023)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(97027)));

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(104396)));

        expected = {"<unknown>", "<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(74152)));

        expected = {"<unknown>", "<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(96297)));

        expected = {"<unknown>", "<unknown>", "<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(97956)));

        expected = {"<unknown>", "Bristol Road Elementary School", "Bristol Road Elementary School"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(25096)));

        expected = {"<unknown>", "Ebenezer Road", "Ebenezer Road"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(76703)));

        expected = {"<unknown>", "Unity Drive", "Unity Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(27769)));

        expected = {"Avenue Road", "Avenue Road", "Lawrence Avenue West", "Lawrence Avenue West"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(68571)));

        expected = {"Baggott Lane", "Ventura Lane", "Ventura Lane"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(87529)));

        expected = {"Billy Bishop Way", "Billy Bishop Way", "Billy Bishop Way"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(59320)));

        expected = {"Box Grove Bypass", "Box Grove Bypass", "Riverwalk Drive", "Riverwalk Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(78437)));

        expected = {"Cambrett Drive", "Cambrett Drive", "Drifton Crescent"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(61948)));

        expected = {"Georgian Drive", "Hays Boulevard", "Hays Boulevard"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(84448)));

        expected = {"Gleda Crescent", "Gleda Crescent", "Glenada Crescent"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(29244)));

        expected = {"Goldberry Square", "Lawrence Avenue East", "Lawrence Avenue East", "Paulander Avenue"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(19446)));

        expected = {"Grainger Crescent", "Grainger Crescent", "Wheatley Crescent"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(81284)));

        expected = {"Highway 401 Collectors", "Highway 401 Collectors"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(7)));

        expected = {"Highway 401 Express", "Highway 401 Express"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(30279)));

        expected = {"Markham Road", "Markham Road"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(107254)));

        expected = {"Neilson Road", "Neilson Road", "Sheppard Avenue East", "Sheppard Avenue East"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(21121)));

        expected = {"Packard Boulevard", "Packard Boulevard", "Saratoga Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(53792)));

        expected = {"Thicket Trail", "Timberland Drive", "Timberland Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(76038)));

        expected = {"Tobermory Drive", "Tobermory Drive"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(58255)));

        //Generate random inputs
        std::vector<unsigned> intersection_ids;
        for(size_t i = 0; i < 1000000; i++) {
            intersection_ids.push_back(rand_intersection(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(1180);
            std::vector<std::string> result;
            for(size_t i = 0; i < 1000000; i++) {
                result = find_intersection_street_names(intersection_ids[i]);
            }
        }
    } //intersection_street_names_perf

    TEST_FIXTURE(MapFixture, directly_connected_perf) {
        //Verify Functionality
        ECE297_CHECK(!are_directly_connected(7339, 68888));

        ECE297_CHECK(!are_directly_connected(20279, 40207));

        ECE297_CHECK(!are_directly_connected(23403, 62657));

        ECE297_CHECK(!are_directly_connected(23724, 102589));

        ECE297_CHECK(!are_directly_connected(27422, 59223));

        ECE297_CHECK(!are_directly_connected(34457, 25685));

        ECE297_CHECK(!are_directly_connected(51942, 34676));

        ECE297_CHECK(!are_directly_connected(73025, 37132));

        ECE297_CHECK(!are_directly_connected(84608, 30577));

        ECE297_CHECK(!are_directly_connected(86940, 58216));

        ECE297_CHECK(!are_directly_connected(90185, 32307));

        ECE297_CHECK(!are_directly_connected(92540, 80276));

        ECE297_CHECK(!are_directly_connected(100735, 59293));

        ECE297_CHECK(are_directly_connected(7, 7));

        ECE297_CHECK(are_directly_connected(19446, 82786));

        ECE297_CHECK(are_directly_connected(27769, 27769));

        ECE297_CHECK(are_directly_connected(29244, 36715));

        ECE297_CHECK(are_directly_connected(53792, 53791));

        ECE297_CHECK(are_directly_connected(59320, 59318));

        ECE297_CHECK(are_directly_connected(68571, 68575));

        ECE297_CHECK(are_directly_connected(76038, 76123));

        ECE297_CHECK(are_directly_connected(76703, 108319));

        ECE297_CHECK(are_directly_connected(78437, 78497));

        ECE297_CHECK(are_directly_connected(81284, 81252));

        ECE297_CHECK(are_directly_connected(84448, 84450));

        ECE297_CHECK(are_directly_connected(85859, 85851));

        ECE297_CHECK(are_directly_connected(87529, 87531));

        ECE297_CHECK(are_directly_connected(88038, 88037));

        ECE297_CHECK(are_directly_connected(97027, 97016));

        ECE297_CHECK(are_directly_connected(98395, 104153));

        //Generate random inputs
        std::vector<unsigned> intersection_ids_1;
        std::vector<unsigned> intersection_ids_2;
        for(size_t i = 0; i < 2000000; i++) {
            intersection_ids_1.push_back(rand_intersection(rng));
            intersection_ids_2.push_back(rand_intersection(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(1069);
            bool result;
            for(size_t i = 0; i < 2000000; i++) {
                result &= are_directly_connected(intersection_ids_1[i], intersection_ids_2[i]);
            }
        }
    } //directly_connected_perf

    TEST_FIXTURE(MapFixture, adjacent_intersections_perf) {
        //Verify Functionality
        std::vector<unsigned> expected;

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(96689)));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(99450)));

        expected = {334, 12297, 70895};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(335)));

        expected = {872, 53528};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(53524)));

        expected = {2872, 3335, 33218};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(3334)));

        expected = {6893, 12158, 44787};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(6892)));

        expected = {7017, 45085};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(12987)));

        expected = {8846, 8848, 8849};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(8847)));

        expected = {9029, 9031, 9032};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(9030)));

        expected = {19119, 19122};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(19121)));

        expected = {21321, 21322, 71667};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(82352)));

        expected = {21340, 21342, 43881};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(94656)));

        expected = {25007, 42119};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(42118)));

        expected = {31414, 31416, 32504, 83117};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(31415)));

        expected = {42473, 43292, 43294};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(43293)));

        expected = {43088, 43257};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(43087)));

        expected = {43481, 50183, 84148};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(84149)));

        expected = {44134, 44139};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(44140)));

        expected = {47808, 52362};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(52356)));

        expected = {48567, 50480, 107104};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(49051)));

        expected = {55637, 55638, 55642};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(55639)));

        expected = {57419, 57440, 57471};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(57427)));

        expected = {63366, 63371};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(63367)));

        expected = {71209, 71211, 98949};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(71210)));

        expected = {79773, 79775, 79784};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(79774)));

        expected = {80428, 80474, 83484};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(87423)));

        expected = {89787, 89790};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(89792)));

        expected = {90615, 90616, 90618};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(90617)));

        expected = {97305, 97311};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(97309)));

        expected = {105018, 105020};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(105019)));

        //Generate random inputs
        std::vector<unsigned> intersection_ids;
        for(size_t i = 0; i < 1000000; i++) {
            intersection_ids.push_back(rand_intersection(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(1041);
            std::vector<unsigned> result;
            for(size_t i = 0; i < 1000000; i++) {
                result = find_adjacent_intersections(intersection_ids[i]);
            }
        }
    } //adjacent_intersections_perf

} //intersection_queries_perf_public_toronto_canada

