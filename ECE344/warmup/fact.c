#include "common.h"

// Recursivfe factorial function
int factorial(int n)
{
    if(n==1) return 1;
    else return n*factorial(n-1);
}

int main(int argc, char **argv)
{
    int n;
    double f;
    
    // Checking if input is empty
    if(argc>1) {
        n = atoi(argv[1]); // Parse as integer
        f = atof(argv[1]); // Parse as double
    } else n = 0;
    
    // Limiting input between 12 and 1, also if the value is valid, make
    // sure the original value is not a decimal (atoi will parse up to a
    // decimal point, so atof is used to check if it is a floating point)
    if(n>12) printf("Overflow\n");
    else if(n>0 && f-n==0) printf("%d\n", factorial(n));
    else printf("Huh?\n");
    
    return 0;
}
