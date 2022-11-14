// Petrin 2022
// This file contains non-member functions used by the IFF program, generally focusing on line-segment interaction
#ifndef IFF_COMMON_HH
#define IFF_COMMON_HH

#include <cmath>
#include <iostream>
#include <vector>

// Degree macros for readability ---------------------------------------------------------------------------------------
#define _360_DEGREES 2 * M_PI
#define _270_DEGREES 3 * M_PI / 2
#define _180_DEGREES M_PI
#define _90_DEGREES  M_PI / 2

// Point definition ----------------------------------------------------------------------------------------------------
struct point {
    double x;
    double y;
};

// Line definition -----------------------------------------------------------------------------------------------------
struct line {
    point point0;
    point point1;
};

// Constrains angle range to 0 - 180 degrees ---------------------------------------------------------------------------
double normalizeAngle180(double angle);

// Constrains angle range to 0 - 360 degrees ---------------------------------------------------------------------------
double normalizeAngle360(double angle);

// Returns angle of line counterclockwise relative to the X axis -------------------------------------------------------
double getAngle(line line);

// Returns absolute angle between two lines ----------------------------------------------------------------------------
// NOTE: This function does NOT return an incident angle - This angle is always towards y = line0.point0 (the user)
double getAngleBetween(line line0, line line1);

// Returns absolute distance between two points ------------------------------------------------------------------------
double getDistanceBetween(point point0, point point1);

// Returns if a given point is on a line segment -----------------------------------------------------------------------
// NOTE: Code is taken from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool isOnSegment(point p, point q, point r);

// Returns getOrientation of a line segment ----------------------------------------------------------------------------
int getOrientation(point p, point q, point r);

// Returns if two line segments intersect ------------------------------------------------------------------------------
// NOTE: Code is taken from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool doIntersect(line line0, line line1);

// Returns the intersection of two line segments -----------------------------------------------------------------------
// This is awkward and inefficient. Needs to be fixed eventually - Matt
point getIntersection(line line0, line line1);

#endif //IFF_COMMON_HH
