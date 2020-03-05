#include <assert.h>
#include "common.h"
#include "point.h"
#include <math.h>

void
point_translate(struct point *p, double x, double y)
{
    p->x += x;
    p->y += y;
}

double
point_distance(const struct point *p1, const struct point *p2)
{
    int dy = p1->y - p2->y;
    int dx = p1->x - p2->x;
    
    // Calculate distance using pythagorean theorem
    return sqrt(dy*dy + dx*dx);
}

int
point_compare(const struct point *p1, const struct point *p2)
{
    // Calculate Euclidean lengths using pathagorean theorem
    double length1 = sqrt(p1->x * p1->x + p1->y * p1->y);
    double length2 = sqrt(p2->x * p2->x + p2->y * p2->y);
    
    if(length1>length2) return 1;
    else if(length1<length2) return -1;
    return 0;
}
