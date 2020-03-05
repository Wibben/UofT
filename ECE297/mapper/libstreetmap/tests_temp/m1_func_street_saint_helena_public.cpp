
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(street_queries_public_saint_helena) {

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

    TEST_FIXTURE(MapFixture, street_street_segments) {
        std::vector<unsigned> expected;

        expected = {49};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(1)));

        expected = {61};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(2)));

        expected = {62, 63};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(3)));

        expected = {138, 174, 175};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(4)));

        expected = {190, 306, 319};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(5)));

        expected = {199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(6)));

        expected = {215, 295};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(7)));

        expected = {255, 256, 257, 258};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(8)));

        expected = {296};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(9)));

        expected = {297, 298};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(10)));

        expected = {299};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(11)));

        expected = {300, 301};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(12)));

        expected = {307, 308};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(13)));

        expected = {316};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(15)));

        expected = {317, 318};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_street_segments(16)));

    } //street_street_segments

    TEST_FIXTURE(MapFixture, all_street_intersections) {
        std::vector<unsigned> expected;

        expected = {60, 61};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(1)));

        expected = {60, 311};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(11)));

        expected = {62, 66};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(9)));

        expected = {62, 286, 287, 288, 289};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(8)));

        expected = {63, 309, 310};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(10)));

        expected = {64, 230, 231, 299};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(5)));

        expected = {90, 91};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(2)));

        expected = {92, 93, 94};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(3)));

        expected = {105, 208, 250};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(7)));

        expected = {155, 182, 221, 229, 234, 235, 237, 239, 241, 242, 243, 244, 245, 246, 247, 248, 249};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(6)));

        expected = {180, 181, 211, 212};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(4)));

        expected = {289, 309, 311};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(16)));

        expected = {311, 315, 317};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(13)));

        expected = {312, 313, 314};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(12)));

        expected = {316, 318};
        ECE297_CHECK_EQUAL(expected, sorted(find_all_street_intersections(15)));

    } //all_street_intersections

    TEST_FIXTURE(MapFixture, intersection_ids_from_street_ids) {
        std::vector<unsigned> expected;

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(4, 10)));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(5, 9)));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(8, 6)));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(11, 6)));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(13, 5)));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(13, 9)));

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(14, 5)));

        expected = {60};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(1, 11)));

        expected = {62, 287, 288};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(8, 0)));

        expected = {93, 94};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(3, 0)));

        expected = {311};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(11, 13)));

        expected = {311};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(13, 11)));

        expected = {311};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(13, 16)));

        expected = {313};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(12, 14)));

        expected = {316, 318};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_ids_from_street_ids(15, 0)));

    } //intersection_ids_from_street_ids

    TEST_FIXTURE(MapFixture, street_ids_from_partial_street_name) {
        std::vector<unsigned> expected;

        expected = {1};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Fi")));

        expected = {2};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Castle Terrace")));

        expected = {2, 3, 5, 6, 9};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("C")));

        expected = {3};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Church Lane")));

        expected = {4};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Sandy Bay Road")));

        expected = {5};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Constitution H")));

        expected = {5};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Constitution Hill")));

        expected = {6};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Casons")));

        expected = {9};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("China Lane")));

        expected = {10};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("The R")));

        expected = {11};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Sid")));

        expected = {11};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Side Pat")));

        expected = {12};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Seafront")));

        expected = {13};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Nap")));

        expected = {13};
        ECE297_CHECK_EQUAL(expected, sorted(find_street_ids_from_partial_street_name("Napoleon")));

    } //street_ids_from_partial_street_name

} //street_queries_public_saint_helena

