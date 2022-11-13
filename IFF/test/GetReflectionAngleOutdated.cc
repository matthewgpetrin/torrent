// Petrin 2022

#include <cmath>
#include <iostream>
#include "../src/common.hh"
#include <iomanip>

#define _360_DEGREES 2 * M_PI
#define _270_DEGREES 3 * M_PI / 2
#define _180_DEGREES M_PI
#define _90_DEGREES  M_PI / 2


// Constrains angle range to 0 - 360 degrees
double normalizeAngle(double angle) {
    if (angle < 0)
        return std::fmod(angle + (2 * M_PI), 2 * M_PI);
    else
        return std::fmod(angle, 2 * M_PI);
}

double getAngle(line line) {
    point p0 = line.point0;
    point p1 = line.point1;

    // Calculates angle of the line counterclockwise from the X axis
    double a = atan2(p1.y - p0.y, p1.x - p0.x);

    return normalizeAngle(a);
}

// NOTE: This function does NOT return an incident angle - This angle is always towards y = line0.point0 (the user)
double getAngleBetween(line line0, line line1) {
    // Constrains both angles to 0 - 180 degrees for convenience
    double a0 = std::fmod(getAngle(line0), M_PI);
    double a1 = std::fmod(getAngle(line1), M_PI);

    return fabs(a1 - a0);
}


double getAngleReflection(line signal, line surface) {
    double angleOfSignal = getAngle(signal);
    double angleOfSurface = std::fmod(getAngle(surface), M_PI);
    double angleOfIntersect = getAngleBetween(signal, surface);
    double angleOfReflection;

    // Checks 4 possible cases of ray angle and defines reflection angle
    if (angleOfSignal < _360_DEGREES) {
        if (angleOfSignal < _270_DEGREES) {
            if (angleOfSignal < _180_DEGREES) {
                if (angleOfSignal < _90_DEGREES) {
                    angleOfReflection = angleOfSurface + angleOfIntersect;
                } else angleOfReflection = angleOfSurface - angleOfIntersect;
            } else angleOfReflection = M_PI + angleOfSurface + angleOfIntersect;
        } else angleOfReflection = M_PI + angleOfSurface - angleOfIntersect;
    } else throw std::runtime_error; // Error thrown in case angle of ray is > 360 degrees

    return normalizeAngle(angleOfReflection);
}


int main() {
    line signal{{0, 1},
                {3, 1}};


    point point0 = {3, 3};
    point point1 = {0, 0};
    line wall = {point0, point1};

    std::cout << getAngle(wall);
    std::cout << "\n \n";
    std::cout << std::setprecision(10) << getAngle(signal);
    std::cout << "\n \n";

    std::cout << getAngleBetween(signal, wall);
    std::cout << "\n \n";

    std::cout << getAngleReflection(signal, wall);


}
