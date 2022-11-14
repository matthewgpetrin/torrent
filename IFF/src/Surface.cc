// Petrin 2022

#include "Surface.hh"

// Constructors --------------------------------------------------------------------------------------------------------
Surface::Surface() : surface({0, 0, 0, 0}), reflectivity(0) {}

Surface::Surface(const line &surface, double reflectivity) :
        surface(surface), reflectivity(reflectivity), point0(surface.point0), point1(surface.point1) {}

Surface::Surface(const point &point0, const point &point1, double reflectivity) :
        point0(point0), point1(point1), reflectivity(reflectivity), surface({point0, point1}) {}


// Getters and Setters -------------------------------------------------------------------------------------------------
const point &Surface::getPoint0() const {
    return point0;
}

void Surface::setPoint0(const point &point0) {
    Surface::point0 = point0;
}

const point &Surface::getPoint1() const {
    return point1;
}

void Surface::setPoint1(const point &point1) {
    Surface::point1 = point1;
}

const line &Surface::getSurface() const {
    return surface;
}

void Surface::setSurface(const line &surface) {
    Surface::surface = surface;
}

double Surface::getReflectivity() const {
    return reflectivity;
}

void Surface::setReflectivity(double reflectivity) {
    Surface::reflectivity = reflectivity;
}
