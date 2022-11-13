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
double normalizeAngle180(double angle) {
    if (angle < 0)
        return std::fmod(angle + (2 * M_PI), M_PI);
    else
        return std::fmod(angle, M_PI);
}

// Constrains angle range to 0 - 360 degrees ---------------------------------------------------------------------------
double normalizeAngle360(double angle) {
    if (angle < 0)
        return std::fmod(angle + (2 * M_PI), 2 * M_PI);
    else
        return std::fmod(angle, 2 * M_PI);
}

// Returns angle of line counterclockwise relative to the X axis -------------------------------------------------------
double getAngle(line line) {
    point p0 = line.point0;
    point p1 = line.point1;

    // Calculates angle of the line counterclockwise from the X axis
    double a = atan2(p1.y - p0.y, p1.x - p0.x);

    return normalizeAngle360(a);
}

// Returns absolute angle between two lines ----------------------------------------------------------------------------
// NOTE: This function does NOT return an incident angle - This angle is always towards y = line0.point0 (the user)
double getAngleBetween(line line0, line line1) {
    // Constrains both angles to 0 - 180 degrees for convenience
    double a0 = getAngle(line0);
    double a1 = getAngle(line1);

    return normalizeAngle180(a1 - a0);
}

// Returns absolute distance between two points ------------------------------------------------------------------------
double getDistanceBetween(point point0, point point1) {
    return sqrt(pow(point1.x - point0.x, 2) + pow(point1.y - point0.y, 2));
}

// Returns if a given point is on a line segment -----------------------------------------------------------------------
// NOTE: Code is taken from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool isOnSegment(point p, point q, point r) {
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;
    return false;
}

// Returns getOrientation of a line segment -------------------------------------------------------------------------------
// NOTE: Code is taken from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
// 0 --> Collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int getOrientation(point p, point q, point r) {
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ for details of below formula.
    int o = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (o == 0) return 0;  // Collinear

    return (o > 0) ? 1 : 2; // Clockwise or counterclockwise
}

// Returns if two line segments intersect ------------------------------------------------------------------------------
// NOTE: Code is taken from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool doIntersect(line line0, line line1) {
    point p1 = line0.point0;
    point q1 = line0.point1;
    point p2 = line1.point0;
    point q2 = line1.point1;

    // Find the four orientations needed for general and special cases
    int o1 = getOrientation(p1, q1, p2);
    int o2 = getOrientation(p1, q1, q2);
    int o3 = getOrientation(p2, q2, p1);
    int o4 = getOrientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p, q1 and q are collinear and q lies on segment p1q1
    if (o1 == 0 && isOnSegment(p1, p2, q1)) return true;

    // p, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && isOnSegment(p1, q2, q1)) return true;

    // q, q2 and p are collinear and p lies on segment p2q2
    if (o3 == 0 && isOnSegment(p2, p1, q2)) return true;

    // q, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && isOnSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

// Returns the intersection of two line segments -----------------------------------------------------------------------
// This is awkward and inefficient. Needs to be fixed eventually - Matt
point getIntersection(line line0, line line1) {
    double m0 = (line0.point1.y - line0.point0.y) / (line0.point1.x - line0.point0.x);
    double m1 = (line1.point1.y - line1.point0.y) / (line1.point1.x - line1.point0.x);

    double b0 = line0.point0.y - (m0 * line0.point0.x);
    double b1 = line1.point0.y - (m1 * line1.point0.x);

    double x = (b1 - b0) / (m0 - m1);
    double y = m0 * x + b0;

    return {x, y};
}

#endif //IFF_COMMON_HH
