
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(intersection_queries_public_saint_helena) {

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

    TEST_FIXTURE(MapFixture, intersection_street_segments) {
        std::vector<unsigned> expected;

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(258)));

        expected = {2};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(0)));

        expected = {50, 51, 297};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(63)));

        expected = {60, 182};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(88)));

        expected = {100};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(140)));

        expected = {137, 173};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(178)));

        expected = {143};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(186)));

        expected = {180, 213, 214};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(221)));

        expected = {188};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(223)));

        expected = {199, 200};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(244)));

        expected = {218, 219};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(254)));

        expected = {229, 232};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(263)));

        expected = {264, 286};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(296)));

        expected = {270, 271};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(75)));

        expected = {279, 280};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_segments(83)));

    } //intersection_street_segments

    TEST_FIXTURE(MapFixture, intersection_street_names) {
        std::vector<std::string> expected;

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(258)));

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(0)));

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(140)));

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(186)));

        expected = {"<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(223)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(75)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(83)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(88)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(178)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(254)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(263)));

        expected = {"<unknown>", "<unknown>"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(296)));

        expected = {"<unknown>", "<unknown>", "The Run"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(63)));

        expected = {"<unknown>", "Casons", "Casons"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(221)));

        expected = {"Casons", "Casons"};
        ECE297_CHECK_EQUAL(expected, sorted(find_intersection_street_names(244)));

    } //intersection_street_names

    TEST_FIXTURE(MapFixture, directly_connected) {
        ECE297_CHECK(!are_directly_connected(91, 229));

        ECE297_CHECK(!are_directly_connected(188, 236));

        ECE297_CHECK(!are_directly_connected(290, 162));

        ECE297_CHECK(!are_directly_connected(295, 292));

        ECE297_CHECK(!are_directly_connected(314, 231));

        ECE297_CHECK(!are_directly_connected(323, 58));

        ECE297_CHECK(are_directly_connected(0, 0));

        ECE297_CHECK(are_directly_connected(63, 309));

        ECE297_CHECK(are_directly_connected(75, 74));

        ECE297_CHECK(are_directly_connected(83, 113));

        ECE297_CHECK(are_directly_connected(140, 139));

        ECE297_CHECK(are_directly_connected(175, 17));

        ECE297_CHECK(are_directly_connected(186, 185));

        ECE297_CHECK(are_directly_connected(221, 249));

        ECE297_CHECK(are_directly_connected(223, 227));

    } //directly_connected

    TEST_FIXTURE(MapFixture, adjacent_intersections) {
        std::vector<unsigned> expected;

        expected = {};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(258)));

        expected = {5, 17};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(175)));

        expected = {57, 59};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(58)));

        expected = {74, 76};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(75)));

        expected = {87, 219};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(88)));

        expected = {90, 94};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(91)));

        expected = {139};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(140)));

        expected = {160, 253};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(254)));

        expected = {205, 207};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(206)));

        expected = {227};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(223)));

        expected = {228, 237, 248};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(229)));

        expected = {243, 245};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(244)));

        expected = {245, 296};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(295)));

        expected = {291, 293};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(292)));

        expected = {322};
        ECE297_CHECK_EQUAL(expected, sorted(find_adjacent_intersections(323)));

    } //adjacent_intersections

} //intersection_queries_public_saint_helena

