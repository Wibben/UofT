
#include "m1.h"
#include "unit_test_util.h"

#include "StreetsDatabaseAPI.h"

#include <unittest++/UnitTest++.h>

#include <random>
#include <algorithm>
#include <set>

using ece297test::relative_error;
using ece297test::sorted;

SUITE(distance_time_queries_perf_public_toronto_canada) {

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

    TEST_FIXTURE(MapFixture, distance_between_two_points_perf) {
        //Verify Functionality
        double expected;

        expected = 2356.55189424585660163;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.79279327392578125, -79.40152740478515625), LatLon(43.78813934326171875, -79.37289428710937500)), 0.001000000);

        expected = 2428.15175589732825756;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.71529388427734375, -79.55553436279296875), LatLon(43.73324203491210938, -79.53833770751953125)), 0.001000000);

        expected = 2949.60709968117043900;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.59196853637695312, -79.37844085693359375), LatLon(43.59408187866210938, -79.41493988037109375)), 0.001000000);

        expected = 3548.21484590875252252;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.82280349731445312, -79.18561553955078125), LatLon(43.83701705932617188, -79.14602661132812500)), 0.001000000);

        expected = 7083.20920347177525400;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.64250946044921875, -79.35969543457031250), LatLon(43.60358428955078125, -79.29006958007812500)), 0.001000000);

        expected = 9272.78419905099690368;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.54529571533203125, -79.10993194580078125), LatLon(43.56031036376953125, -79.22308349609375000)), 0.001000000);

        expected = 9502.64656263026699889;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.71036148071289062, -79.60934448242187500), LatLon(43.65697860717773438, -79.70158386230468750)), 0.001000000);

        expected = 10259.69455752463727549;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.59600830078125000, -79.11286163330078125), LatLon(43.52816772460937500, -79.19911193847656250)), 0.001000000);

        expected = 10810.79465902316587744;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.87495422363281250, -79.56262969970703125), LatLon(43.85248184204101562, -79.43147277832031250)), 0.001000000);

        expected = 14257.15447348564339336;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.61819458007812500, -79.22166442871093750), LatLon(43.72824478149414062, -79.31253051757812500)), 0.001000000);

        expected = 14466.27648594317543029;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.77970123291015625, -79.60785675048828125), LatLon(43.71545028686523438, -79.45131683349609375)), 0.001000000);

        expected = 16151.57112973360381147;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.73253250122070312, -79.09117126464843750), LatLon(43.58732604980468750, -79.09316253662109375)), 0.001000000);

        expected = 17244.03077702475638944;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.56804275512695312, -79.77233123779296875), LatLon(43.69831466674804688, -79.65619659423828125)), 0.001000000);

        expected = 23088.60558041352851433;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.84590911865234375, -79.17972564697265625), LatLon(43.66649627685546875, -79.32427978515625000)), 0.001000000);

        expected = 26696.83790673241310287;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.77907943725585938, -79.08944702148437500), LatLon(43.88920211791992188, -79.38510131835937500)), 0.001000000);

        expected = 28346.57113680840484449;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.57031250000000000, -79.68901062011718750), LatLon(43.77373886108398438, -79.47676086425781250)), 0.001000000);

        expected = 29221.49002234368890640;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.65841293334960938, -79.78363037109375000), LatLon(43.66227722167968750, -79.42051696777343750)), 0.001000000);

        expected = 30851.35060581533980439;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.77691268920898438, -79.58845520019531250), LatLon(43.56536483764648438, -79.34043121337890625)), 0.001000000);

        expected = 31805.81615727611278999;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.71936035156250000, -79.52051544189453125), LatLon(43.89463424682617188, -79.20743560791015625)), 0.001000000);

        expected = 34408.83086704561719671;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.74295425415039062, -79.03385925292968750), LatLon(43.64625930786132812, -79.44028472900390625)), 0.001000000);

        expected = 36762.76484817622986156;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.89926147460937500, -79.00795745849609375), LatLon(43.57030868530273438, -79.05248260498046875)), 0.001000000);

        expected = 38140.16821192804491147;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.87306213378906250, -79.04129791259765625), LatLon(43.88061904907226562, -79.51689147949218750)), 0.001000000);

        expected = 38384.24630290480854455;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.89250564575195312, -79.44643402099609375), LatLon(43.57640457153320312, -79.25479125976562500)), 0.001000000);

        expected = 40275.23822157075483119;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.83801269531250000, -79.22557830810546875), LatLon(43.91736602783203125, -79.71571350097656250)), 0.001000000);

        expected = 41625.44069501305057202;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.56082534790039062, -79.01221466064453125), LatLon(43.72190093994140625, -79.47900390625000000)), 0.001000000);

        expected = 44869.30046041508467169;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.87590789794921875, -79.01165771484375000), LatLon(43.90193939208984375, -79.57024383544921875)), 0.001000000);

        expected = 45646.59117179606255377;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.64857482910156250, -79.20729827880859375), LatLon(43.71325683593750000, -79.76767730712890625)), 0.001000000);

        expected = 50284.11633087694644928;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.90982437133789062, -79.60358428955078125), LatLon(43.48199844360351562, -79.40149688720703125)), 0.001000000);

        expected = 51318.54446459973405581;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.65852355957031250, -79.65676879882812500), LatLon(43.54739761352539062, -79.03836822509765625)), 0.001000000);

        expected = 60998.15420726743468549;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_distance_between_two_points(LatLon(43.74922943115234375, -79.02179718017578125), LatLon(43.81575393676757812, -79.77579498291015625)), 0.001000000);

        //Generate random inputs
        std::vector<LatLon> latlons_1;
        std::vector<LatLon> latlons_2;
        for(size_t i = 0; i < 30000000; i++) {
            latlons_1.push_back(LatLon(rand_lat(rng), rand_lon(rng)));
            latlons_2.push_back(LatLon(rand_lat(rng), rand_lon(rng)));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(1084);
            double result;
            for(size_t i = 0; i < 30000000; i++) {
                result += find_distance_between_two_points(latlons_1[i], latlons_2[i]);
            }
        }
    } //distance_between_two_points_perf

    TEST_FIXTURE(MapFixture, street_segment_length_perf) {
        //Verify Functionality
        double expected;

        expected = 9.09774878682763521;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(140688), 0.001000000);

        expected = 9.24207725918289391;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(151372), 0.001000000);

        expected = 17.09496565388253231;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(28196), 0.001000000);

        expected = 23.48891492243527068;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(124495), 0.001000000);

        expected = 24.15866891885319490;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(90597), 0.001000000);

        expected = 25.82816268275384530;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(36389), 0.001000000);

        expected = 27.41978422292287121;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(142034), 0.001000000);

        expected = 29.72096698861328434;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(113732), 0.001000000);

        expected = 32.14021668534559240;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(155517), 0.001000000);

        expected = 44.83770116140563999;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(117861), 0.001000000);

        expected = 49.78241523119176293;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(126916), 0.001000000);

        expected = 50.86959588221137096;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(77997), 0.001000000);

        expected = 54.44416223401825050;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(110253), 0.001000000);

        expected = 55.41091787626419318;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(84468), 0.001000000);

        expected = 57.29100715801643418;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(122449), 0.001000000);

        expected = 58.21534292233229735;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(142671), 0.001000000);

        expected = 62.77545881717573195;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(43905), 0.001000000);

        expected = 66.43213964050811171;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(30625), 0.001000000);

        expected = 81.76324865924725316;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(139629), 0.001000000);

        expected = 88.29971916585265035;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(86014), 0.001000000);

        expected = 88.64990923313939675;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(139231), 0.001000000);

        expected = 125.91658222530665512;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(67668), 0.001000000);

        expected = 158.35320150200374201;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(107519), 0.001000000);

        expected = 165.05292411409178044;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(42404), 0.001000000);

        expected = 187.72345269100949849;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(106519), 0.001000000);

        expected = 222.14266565772751960;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(40265), 0.001000000);

        expected = 304.82387440614292018;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(111218), 0.001000000);

        expected = 313.42368622788245602;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(99426), 0.001000000);

        expected = 530.64198183238954698;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(89824), 0.001000000);

        expected = 660.70680896110570757;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_length(10), 0.001000000);

        //Generate random inputs
        std::vector<unsigned> segment_ids;
        for(size_t i = 0; i < 1600000; i++) {
            segment_ids.push_back(rand_segment(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(982);
            double result;
            for(size_t i = 0; i < 1600000; i++) {
                result += find_street_segment_length(segment_ids[i]);
            }
        }
    } //street_segment_length_perf

    TEST_FIXTURE(MapFixture, street_length_perf) {
        //Verify Functionality
        double expected;

        expected = 68.94498340320835439;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(9098), 0.001000000);

        expected = 83.59169625867053810;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(10487), 0.001000000);

        expected = 88.39675553219957749;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(20909), 0.001000000);

        expected = 91.52527038753788702;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(12181), 0.001000000);

        expected = 143.61104414393872730;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(11357), 0.001000000);

        expected = 186.37679273336874530;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(18773), 0.001000000);

        expected = 192.41808057775466523;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(19096), 0.001000000);

        expected = 199.21033255849334864;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(5903), 0.001000000);

        expected = 229.30745138354708956;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(14456), 0.001000000);

        expected = 272.98694841705327008;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(5701), 0.001000000);

        expected = 325.71336610022336799;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(16463), 0.001000000);

        expected = 331.54461504482202372;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(4118), 0.001000000);

        expected = 374.18997794636300114;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(15846), 0.001000000);

        expected = 403.56821987986796785;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(17063), 0.001000000);

        expected = 422.42757786293145728;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(3791), 0.001000000);

        expected = 434.95506486046878081;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(14321), 0.001000000);

        expected = 455.12257706863618978;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(12077), 0.001000000);

        expected = 460.37021757360480478;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(14953), 0.001000000);

        expected = 520.60634217883693964;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(16738), 0.001000000);

        expected = 538.28395173888827685;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(11564), 0.001000000);

        expected = 541.75094288859770586;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(4893), 0.001000000);

        expected = 580.87837195423321646;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(18915), 0.001000000);

        expected = 582.75535934102310875;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(18719), 0.001000000);

        expected = 594.61681327446206069;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(19181), 0.001000000);

        expected = 702.92987682766056423;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(20351), 0.001000000);

        expected = 728.87313627090520640;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(5414), 0.001000000);

        expected = 985.62795764283407607;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(13368), 0.001000000);

        expected = 997.22009402007211065;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(14823), 0.001000000);

        expected = 1118.82766310318356773;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(15291), 0.001000000);

        expected = 2408.61874267665962179;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_length(2), 0.001000000);

        //Generate random inputs
        std::vector<unsigned> street_ids;
        for(size_t i = 0; i < 400000; i++) {
            street_ids.push_back(rand_street(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(1152);
            double result;
            for(size_t i = 0; i < 400000; i++) {
                result += find_street_length(street_ids[i]);
            }
        }
    } //street_length_perf

    TEST_FIXTURE(MapFixture, street_segment_travel_time_perf) {
        //Verify Functionality
        double expected;

        expected = 0.65503791265158984;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(140688), 0.001000000);

        expected = 0.66542956266116826;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(151372), 0.001000000);

        expected = 1.23083752707954219;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(28196), 0.001000000);

        expected = 1.61415724181060294;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(117861), 0.001000000);

        expected = 1.69120187441533942;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(124495), 0.001000000);

        expected = 1.73942416215743023;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(90597), 0.001000000);

        expected = 1.85962771315827680;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(36389), 0.001000000);

        expected = 1.97422446405044671;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(142034), 0.001000000);

        expected = 2.13990962318015621;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(113732), 0.001000000);

        expected = 2.31409560134488235;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(155517), 0.001000000);

        expected = 3.43746042948098607;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(122449), 0.001000000);

        expected = 3.58433389664580737;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(126916), 0.001000000);

        expected = 3.91997968084931436;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(110253), 0.001000000);

        expected = 4.19150469040792473;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(142671), 0.001000000);

        expected = 4.51983303483665289;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(43905), 0.001000000);

        expected = 4.57826362939902332;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(77997), 0.001000000);

        expected = 4.78311405411658352;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(30625), 0.001000000);

        expected = 4.98698260886377653;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(84468), 0.001000000);

        expected = 6.38279346478603671;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(139231), 0.001000000);

        expected = 7.35869237933225229;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(139629), 0.001000000);

        expected = 7.94697472492673818;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(86014), 0.001000000);

        expected = 9.99641995459773902;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(40265), 0.001000000);

        expected = 11.33249240027759974;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(67668), 0.001000000);

        expected = 14.25178813518033571;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(107519), 0.001000000);

        expected = 14.85476317026826010;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(42404), 0.001000000);

        expected = 16.89511074219085529;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(106519), 0.001000000);

        expected = 21.94731895724229176;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(111218), 0.001000000);

        expected = 22.56650540840753649;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(99426), 0.001000000);

        expected = 38.20622269193204090;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(89824), 0.001000000);

        expected = 47.57089024519960674;
        ECE297_CHECK_RELATIVE_ERROR(expected, find_street_segment_travel_time(10), 0.001000000);

        //Generate random inputs
        std::vector<unsigned> segment_ids;
        for(size_t i = 0; i < 250000000; i++) {
            segment_ids.push_back(rand_segment(rng));
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(1619);
            double result;
            for(size_t i = 0; i < 250000000; i++) {
                result += find_street_segment_travel_time(segment_ids[i]);
            }
        }
    } //street_segment_travel_time_perf

} //distance_time_queries_perf_public_toronto_canada

