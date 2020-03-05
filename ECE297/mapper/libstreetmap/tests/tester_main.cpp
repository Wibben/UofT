#include <random>
#include <iostream>
#include <unittest++/UnitTest++.h>

int main(int argc, char** argv) {
    //Run the unit tests
    int num_failures = UnitTest::RunAllTests();

    return num_failures;
}