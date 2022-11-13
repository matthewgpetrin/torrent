// Petrin 2022

#ifndef IFF_RAY_HH
#define IFF_RAY_HH

#include "common.hh"
#include "Surface.hh"

class Ray {
    // User Passed Variables ------------------------------------------------------------------------------------------
    point origin;
    double angle;

    double frequency;
    double power;
    double mds; // Minimum detectable signal: https://en.wikipedia.org/wiki/Minimum_detectable_signal

    // Auto Generated Variables ----------------------------------------------------------------------------------------
    double range;
    point terminus;
    line ray; // Line representation of ray

    // Reflected Ray Variables ----------------------------------------------------------------------------------------
    int reflectionIndex;
    Surface reflectionSurface;

public:
};


#endif //IFF_RAY_HH
