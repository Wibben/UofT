
#include "m1.h"
#include "find_in_range_test.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(find_pois_perf_public_toronto_canada) {

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

    TEST_FIXTURE(MapFixture, find_pois_perf) {
        //Generate random inputs
        std::vector<LatLon> intersection_LatLons;
        for(size_t i = 0; i < 1000; i++) {
            intersection_LatLons.push_back(getIntersectionPosition(rand_intersection(rng)));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(0);
            std::vector<POIData_test> result;
            double dist;
            for(size_t j = 0; j < 100000; j++) {
                result.clear();
                // Go through every POI and insert the ones within range into the vector
                for(POIIndex i=0; i<getNumPointsOfInterest(); i++) {
                    dist = find_distance_between_two_points(
                                intersection_LatLons[j],getPointOfInterestPosition(i));

                    if(dist<=500) result.push_back( POIData_test(
                                            i, // ID
                                            getPointOfInterestName(i), // Name
                                            getPointOfInterestType(i), // Type
                                            dist // Distance
                                    ));
                }
            }
        }
    } //find_pois_perf

} //street_queries_perf_public_toronto_canada

