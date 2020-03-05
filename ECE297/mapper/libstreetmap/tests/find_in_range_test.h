#pragma once

#include "m1.h"
#include "m2.h"
#include "StreetsDatabaseAPI.h"
#include <sstream>
#include <string>

// POI class to store all relevant information on POIs: ID, name, distance
// as well as to provide custom compare functions to sort by different things
class POIData_test
{  
public:
    POIIndex ID;
    std::string name,type;
    double dist;
    
    // Constructor
    POIData_test(POIIndex id, std::string n, std::string t, double d) : 
        ID(id),name(n),type(t),dist(d) {}
};