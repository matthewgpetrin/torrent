// Petrin 2022

#ifndef IFF_COMMON_HH
#define IFF_COMMON_HH

#include <cmath>
#include <iostream>

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

#endif //IFF_COMMON_HH
