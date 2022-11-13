//
// Created by Matthew on 11/10/2022.
//

#ifndef IFF_RAY_HH
#define IFF_RAY_HH

#include "common.hh"
#include "Surface.hh"

class Ray {
    // User Defined Variables ------------------------------------------------------------------------------------------
    point origin;
    double angle;
    double range;

    double frequency;

    // Auto Generated --------------------------------------------------------------------------------------------------
    point terminus;
    struct line line;

    // Reflection Ray Variables ----------------------------------------------------------------------------------------
    int reflectionIndex;
    Surface reflectionSurface;

public:
};


#endif //IFF_RAY_HH
