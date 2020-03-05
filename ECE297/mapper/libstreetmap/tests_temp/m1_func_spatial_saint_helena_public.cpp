
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(spatial_queries_public_saint_helena) {

    struct BaseMapFixture {
        BaseMapFixture() {
            //Load the map
            try {
                load_map("/cad2/ece297s/public/maps//saint-helena.streets.bin");
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

        ECE297_CHECK_EQUAL(0, find_closest_point_of_interest(LatLon(-14.38582324981689453, -14.21776580810546875)));

        ECE297_CHECK_EQUAL(0, find_closest_point_of_interest(LatLon(-12.75326442718505859, -13.43280696868896484)));

        ECE297_CHECK_EQUAL(0, find_closest_point_of_interest(LatLon(-12.72489166259765625, -12.93563938140869141)));

        ECE297_CHECK_EQUAL(0, find_closest_point_of_interest(LatLon(-11.99445438385009766, -12.92930889129638672)));

        ECE297_CHECK_EQUAL(0, find_closest_point_of_interest(LatLon(-11.77335262298583984, -12.40951538085937500)));

        ECE297_CHECK_EQUAL(0, find_closest_point_of_interest(LatLon(-10.55166912078857422, -12.17775917053222656)));

        ECE297_CHECK_EQUAL(0, find_closest_point_of_interest(LatLon(-8.25660419464111328, -11.97569370269775391)));

        ECE297_CHECK_EQUAL(1, find_closest_point_of_interest(LatLon(-15.96532535552978516, -10.10347843170166016)));

        ECE297_CHECK_EQUAL(1, find_closest_point_of_interest(LatLon(-14.43496608734130859, -9.42596817016601562)));

        ECE297_CHECK_EQUAL(1, find_closest_point_of_interest(LatLon(-14.03187465667724609, -6.68261575698852539)));

        ECE297_CHECK_EQUAL(1, find_closest_point_of_interest(LatLon(-11.36636924743652344, -6.66052150726318359)));

        ECE297_CHECK_EQUAL(1, find_closest_point_of_interest(LatLon(-11.17502117156982422, -6.02467346191406250)));

        ECE297_CHECK_EQUAL(1, find_closest_point_of_interest(LatLon(-11.05981922149658203, -5.89088439941406250)));

        ECE297_CHECK_EQUAL(1, find_closest_point_of_interest(LatLon(-8.73443222045898438, -5.77834224700927734)));

        ECE297_CHECK_EQUAL(1, find_closest_point_of_interest(LatLon(-8.30572605133056641, -5.73728227615356445)));

    } //closest_poi

    TEST_FIXTURE(MapFixture, closest_intersection) {
        std::set<unsigned> valid_results;
        unsigned actual;

        valid_results = {4, 7, 8, 9, 10, 61, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 167, 169, 170, 171, 172, 190, 204, 213, 214, 217, 218, 219, 222, 224, 236, 238, 262, 265, 266, 267, 276, 277, 278, 282, 284, 285, 286, 287, 288, 289, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318};
        actual = find_closest_intersection(LatLon(-11.68280792236328125, -11.81244564056396484));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {4, 8, 9, 10, 61, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 167, 169, 170, 171, 172, 190, 194, 213, 214, 217, 218, 219, 222, 224, 236, 238, 262, 265, 266, 267, 271, 276, 277, 278, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318};
        actual = find_closest_intersection(LatLon(-10.50049591064453125, -12.39301395416259766));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {4, 8, 9, 10, 61, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 167, 169, 170, 171, 172, 190, 194, 213, 214, 217, 218, 219, 222, 224, 262, 265, 266, 267, 271, 276, 277, 278, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318};
        actual = find_closest_intersection(LatLon(-10.55166912078857422, -12.17775917053222656));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {4, 8, 9, 10, 61, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 167, 169, 170, 171, 172, 190, 214, 217, 218, 219, 222, 224, 236, 238, 262, 265, 266, 267, 276, 277, 278, 282, 284, 285, 286, 287, 288, 289, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318};
        actual = find_closest_intersection(LatLon(-11.35332679748535156, -11.62172412872314453));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {4, 8, 9, 10, 61, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 167, 169, 170, 171, 172, 190, 214, 217, 218, 219, 222, 224, 236, 238, 262, 265, 266, 276, 277, 278, 282, 284, 285, 286, 287, 288, 289, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318};
        actual = find_closest_intersection(LatLon(-11.60812187194824219, -11.42399120330810547));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {4, 8, 61, 85, 87, 88, 89, 90, 91, 92, 93, 94, 169, 170, 171, 172, 217, 218, 219, 222, 224, 262, 265, 276, 286, 287, 288, 310, 312, 313, 314, 315, 316, 317};
        actual = find_closest_intersection(LatLon(-13.01890945434570312, -9.63973808288574219));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {4, 10, 236, 238, 243, 290, 291, 305};
        actual = find_closest_intersection(LatLon(-14.43496608734130859, -9.42596817016601562));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {4, 85, 88, 90, 91, 92, 93, 94, 169, 170, 171, 172, 217, 218, 219, 222, 224, 286, 287, 288, 310, 312, 313, 314, 316, 317};
        actual = find_closest_intersection(LatLon(-13.73339653015136719, -8.86724281311035156));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {61, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 167, 169, 170, 171, 172, 194, 213, 214, 217, 218, 219, 222, 224, 262, 265, 266, 267, 271, 276, 277, 278, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318};
        actual = find_closest_intersection(LatLon(-8.11186981201171875, -12.34554195404052734));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {61, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 167, 169, 170, 171, 172, 194, 213, 214, 217, 218, 219, 222, 224, 262, 265, 266, 267, 271, 276, 277, 278, 282, 283, 284, 285, 286, 287, 288, 289, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318};
        actual = find_closest_intersection(LatLon(-8.25660419464111328, -11.97569370269775391));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {61, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 167, 169, 170, 171, 172, 217, 218, 219, 222, 224, 262, 265, 276, 277, 278, 282, 285, 286, 287, 288, 289, 310, 311, 312, 313, 314, 315, 316, 317};
        actual = find_closest_intersection(LatLon(-11.67993831634521484, -10.65622806549072266));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {61, 85, 88, 89, 90, 91, 92, 93, 94, 169, 170, 171, 172, 217, 218, 219, 222, 224, 286, 287, 288, 289, 309, 310, 311, 312, 313, 314, 315, 316, 317};
        actual = find_closest_intersection(LatLon(-8.39069080352783203, -7.95042800903320312));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {61, 90, 91, 92, 93, 94, 169, 171, 172, 217, 218, 224, 286, 287, 288, 310, 311, 312, 313, 314, 315, 316, 317};
        actual = find_closest_intersection(LatLon(-8.73443222045898438, -5.77834224700927734));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {61, 90, 91, 92, 93, 94, 171, 217, 218, 286, 312, 313, 314, 317};
        actual = find_closest_intersection(LatLon(-11.36636924743652344, -6.66052150726318359));
        ECE297_CHECK(valid_results.count(actual));

        valid_results = {61, 90, 91, 92, 93, 94, 171, 217, 218, 312, 313, 314};
        actual = find_closest_intersection(LatLon(-14.03187465667724609, -6.68261575698852539));
        ECE297_CHECK(valid_results.count(actual));

    } //closest_intersection

} //spatial_queries_public_saint_helena

