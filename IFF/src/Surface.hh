//
// Created by Matthew on 11/10/2022.
//

#ifndef IFF_SURFACE_HH
#define IFF_SURFACE_HH

#include "common.hh"

class Surface {
private:
    point point0;
    point point1;
    struct line line;
    double reflectivity;

public:
    Surface(const struct line &line, double reflectivity);

    Surface(const point &point0, const point &point1, double reflectivity);

public:

};

#endif //IFF_SURFACE_HH
