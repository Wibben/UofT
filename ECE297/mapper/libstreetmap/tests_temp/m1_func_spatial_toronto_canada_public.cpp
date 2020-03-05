
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(spatial_queries_public_toronto_canada) {

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


    struct MapFixture : BaseMapFixture {};

    TEST_FIXTURE(MapFixture, closest_poi) {
        std::set<unsigned> valid_results;
        unsigned actual;

        ECE297_CHECK_EQUAL(368, find_closest_point_of_interest(LatLon(43.64250946044921875, -79.35969543457031250)));

        ECE297_CHECK_EQUAL(723, find_closest_point_of_interest(LatLon(43.71545028686523438, -79.45131683349609375)));

        ECE297_CHECK_EQUAL(1091, find_closest_point_of_interest(LatLon(43.85248184204101562, -79.43147277832031250)));

        ECE297_CHECK_EQUAL(1098, find_closest_point_of_interest(LatLon(43.77970123291015625, -79.60785675048828125)));

        ECE297_CHECK_EQUAL(1126, find_closest_point_of_interest(LatLon(43.83801269531250000, -79.22557830810546875)));

        ECE297_CHECK_EQUAL(1224, find_closest_point_of_interest(LatLon(43.77691268920898438, -79.58845520019531250)));

        ECE297_CHECK_EQUAL(1455, find_closest_point_of_interest(LatLon(43.65697860717773438, -79.70158386230468750)));

        ECE297_CHECK_EQUAL(2434, find_closest_point_of_interest(LatLon(43.56536483764648438, -79.34043121337890625)));

        ECE297_CHECK_EQUAL(2489, find_closest_point_of_interest(LatLon(43.71529388427734375, -79.55553436279296875)));

        valid_results = {4203, 5753};
        actual = find_closest_point_of_interest(LatLon(43.74922943115234375, -79.02179718017578125));
        ECE297_CHECK(valid_results.count(actual));

        ECE297_CHECK_EQUAL(4982, find_closest_point_of_interest(LatLon(43.77373886108398438, -79.47676086425781250)));

        ECE297_CHECK_EQUAL(5037, find_closest_point_of_interest(LatLon(43.71936035156250000, -79.52051544189453125)));

        ECE297_CHECK_EQUAL(5208, find_closest_point_of_interest(LatLon(43.87590789794921875, -79.01165771484375000)));

        ECE297_CHECK_EQUAL(5228, find_closest_point_of_interest(LatLon(43.57031250000000000, -79.68901062011718750)));

        ECE297_CHECK_EQUAL(5506, find_closest_point_of_interest(LatLon(43.87495422363281250, -79.56262969970703125)));

        ECE297_CHECK_EQUAL(5516, find_closest_point_of_interest(LatLon(43.90193939208984375, -79.57024383544921875)));

        ECE297_CHECK_EQUAL(6005, find_closest_point_of_interest(LatLon(43.56804275512695312, -79.77233123779296875)));

        ECE297_CHECK_EQUAL(6118, find_closest_point_of_interest(LatLon(43.69831466674804688, -79.65619659423828125)));

        ECE297_CHECK_EQUAL(6409, find_closest_point_of_interest(LatLon(43.73253250122070312, -79.09117126464843750)));

        ECE297_CHECK_EQUAL(6498, find_closest_point_of_interest(LatLon(43.81575393676757812, -79.77579498291015625)));

        ECE297_CHECK_EQUAL(6687, find_closest_point_of_interest(LatLon(43.48199844360351562, -79.40149688720703125)));

        ECE297_CHECK_EQUAL(6687, find_closest_point_of_interest(LatLon(43.59196853637695312, -79.37844085693359375)));

        ECE297_CHECK_EQUAL(6848, find_closest_point_of_interest(LatLon(43.82280349731445312, -79.18561553955078125)));

        ECE297_CHECK_EQUAL(6941, find_closest_point_of_interest(LatLon(43.71036148071289062, -79.60934448242187500)));

        ECE297_CHECK_EQUAL(7207, find_closest_point_of_interest(LatLon(43.73324203491210938, -79.53833770751953125)));

        ECE297_CHECK_EQUAL(7337, find_closest_point_of_interest(LatLon(43.83701705932617188, -79.14602661132812500)));

        ECE297_CHECK_EQUAL(8278, find_closest_point_of_interest(LatLon(43.58732604980468750, -79.09316253662109375)));

        ECE297_CHECK_EQUAL(8392, find_closest_point_of_interest(LatLon(43.60358428955078125, -79.29006958007812500)));

        ECE297_CHECK_EQUAL(8427, find_closest_point_of_interest(LatLon(43.91736602783203125, -79.71571350097656250)));

        ECE297_CHECK_EQUAL(8455, find_closest_point_of_interest(LatLon(43.89463424682617188, -79.20743560791015625)));

    } //closest_poi

    TEST_FIXTURE(MapFixture, closest_intersection) {
        std::set<unsigned> valid_results;
        unsigned actual;

        ECE297_CHECK_EQUAL(2136, find_closest_intersection(LatLon(43.71936035156250000, -79.52051544189453125)));

        ECE297_CHECK_EQUAL(4253, find_closest_intersection(LatLon(43.77373886108398438, -79.47676086425781250)));

        valid_results = {6084, 6093, 6100, 6101, 97673};
        actual = find_closest_intersection(LatLon(43.48199844360351562, -79.40149688720703125));
        ECE297_CHECK(valid_results.count(actual));

        ECE297_CHECK_EQUAL(7137, find_closest_intersection(LatLon(43.73253250122070312, -79.09117126464843750)));

        ECE297_CHECK_EQUAL(10260, find_closest_intersection(LatLon(43.82280349731445312, -79.18561553955078125)));

        ECE297_CHECK_EQUAL(10432, find_closest_intersection(LatLon(43.83701705932617188, -79.14602661132812500)));

        ECE297_CHECK_EQUAL(10545, find_closest_intersection(LatLon(43.60358428955078125, -79.29006958007812500)));

        ECE297_CHECK_EQUAL(10549, find_closest_intersection(LatLon(43.56536483764648438, -79.34043121337890625)));

        ECE297_CHECK_EQUAL(21784, find_closest_intersection(LatLon(43.71036148071289062, -79.60934448242187500)));

        ECE297_CHECK_EQUAL(28348, find_closest_intersection(LatLon(43.71545028686523438, -79.45131683349609375)));

        ECE297_CHECK_EQUAL(34716, find_closest_intersection(LatLon(43.69831466674804688, -79.65619659423828125)));

        ECE297_CHECK_EQUAL(41421, find_closest_intersection(LatLon(43.90193939208984375, -79.57024383544921875)));

        ECE297_CHECK_EQUAL(42815, find_closest_intersection(LatLon(43.90982437133789062, -79.60358428955078125)));

        ECE297_CHECK_EQUAL(42935, find_closest_intersection(LatLon(43.77970123291015625, -79.60785675048828125)));

        ECE297_CHECK_EQUAL(43170, find_closest_intersection(LatLon(43.91736602783203125, -79.71571350097656250)));

        ECE297_CHECK_EQUAL(43259, find_closest_intersection(LatLon(43.81575393676757812, -79.77579498291015625)));

        ECE297_CHECK_EQUAL(43659, find_closest_intersection(LatLon(43.77691268920898438, -79.58845520019531250)));

        valid_results = {45565, 96913};
        actual = find_closest_intersection(LatLon(43.74922943115234375, -79.02179718017578125));
        ECE297_CHECK(valid_results.count(actual));

        ECE297_CHECK_EQUAL(57597, find_closest_intersection(LatLon(43.73324203491210938, -79.53833770751953125)));

        ECE297_CHECK_EQUAL(65218, find_closest_intersection(LatLon(43.56804275512695312, -79.77233123779296875)));

        ECE297_CHECK_EQUAL(66201, find_closest_intersection(LatLon(43.57031250000000000, -79.68901062011718750)));

        ECE297_CHECK_EQUAL(71302, find_closest_intersection(LatLon(43.58732604980468750, -79.09316253662109375)));

        ECE297_CHECK_EQUAL(71564, find_closest_intersection(LatLon(43.71529388427734375, -79.55553436279296875)));

        ECE297_CHECK_EQUAL(75116, find_closest_intersection(LatLon(43.83801269531250000, -79.22557830810546875)));

        ECE297_CHECK_EQUAL(83449, find_closest_intersection(LatLon(43.87590789794921875, -79.01165771484375000)));

        ECE297_CHECK_EQUAL(83714, find_closest_intersection(LatLon(43.89463424682617188, -79.20743560791015625)));

        ECE297_CHECK_EQUAL(87011, find_closest_intersection(LatLon(43.85248184204101562, -79.43147277832031250)));

        ECE297_CHECK_EQUAL(91612, find_closest_intersection(LatLon(43.87495422363281250, -79.56262969970703125)));

        ECE297_CHECK_EQUAL(103748, find_closest_intersection(LatLon(43.65697860717773438, -79.70158386230468750)));

        ECE297_CHECK_EQUAL(107411, find_closest_intersection(LatLon(43.64250946044921875, -79.35969543457031250)));

    } //closest_intersection

} //spatial_queries_public_toronto_canada

