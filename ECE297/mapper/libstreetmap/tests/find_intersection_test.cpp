
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(find_intersections_perf_public_toronto_canada) {

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

    TEST_FIXTURE(MapFixture, find_intersection_perf) {
        //Generate random inputs
        std::vector<std::string> street_names_prefixes_1;
        std::vector<std::string> street_names_prefixes_2;
        for(size_t i = 0; i < 10000; i++) {
            std::string rand_street_name_1 = getStreetName(rand_street(rng));
            unsigned rand_prefix_end_pos_1 = (1.0*rng()/rng.max())*rand_street_name_1.length();
            street_names_prefixes_1.push_back(rand_street_name_1.substr(0, rand_prefix_end_pos_1));
            std::string rand_street_name_2 = getStreetName(rand_street(rng));
            unsigned rand_prefix_end_pos_2 = (1.0*rng()/rng.max())*rand_street_name_2.length();
            street_names_prefixes_2.push_back(rand_street_name_2.substr(0, rand_prefix_end_pos_2));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(0);
            std::vector<unsigned> result;
            std::vector<unsigned> street_1;
            std::vector<unsigned> street_2;
            for(size_t i = 0; i < 10000; i++) {
                street_1 = find_street_ids_from_partial_street_name(street_names_prefixes_1[i]);
                street_2 = find_street_ids_from_partial_street_name(street_names_prefixes_2[i]);
                
                for(unsigned j=0; j<street_1.size(); j++) {
                    for(unsigned k=0; k<street_2.size(); k++) {
                        result = find_intersection_ids_from_street_ids(street_1[j], street_2[k]);
                    }
                }
            }
        }
    } //find_intersection_perf

} //street_queries_perf_public_toronto_canada

