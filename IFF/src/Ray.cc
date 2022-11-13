// Petrin 2022

#include "Ray.hh"

// Default Constructor -------------------------------------------------------------------------------------------------
Ray::Ray() :
        origin(origin), angle(angle), frequency(frequency), power(power), mds(mds), index(index), surface(surface) {}

// Initial Ray Constructor ---------------------------------------------------------------------------------------------
Ray::Ray(const point &origin, const double angle, const double frequency, const double power, const double mds) :
        origin(origin), angle(angle), frequency(frequency), power(power), mds(mds), index(0), surface(surface) {
    this->range = getDistanceAtPower(mds);
    this->terminus.x = range * cos(angle) + origin.x;
    this->terminus.y = range * sin(angle) + origin.y;
    this->ray = {origin, terminus};
}

// Reflected Ray Constructor -------------------------------------------------------------------------------------------
Ray::Ray(const point &origin, const double angle, const double frequency, const double power, const double mds,
         const int index, const Surface &surface) :
        origin(origin), angle(angle), frequency(frequency), power(power), mds(mds), index(index), surface(surface) {
    this->range = getDistanceAtPower(mds);
    this->terminus.x = range * cos(angle) + origin.x;
    this->terminus.y = range * sin(angle) + origin.y;
    this->ray = {origin, terminus};
}

// Return path loss given distance -------------------------------------------------------------------------------------
// See https://en.wikipedia.org/wiki/Free-space_path_loss
double Ray::getPathLoss(double distance) {
    return pow(4 * M_PI * distance * frequency / LIGHTSPEED, 2);
}

// Return power level given distance -----------------------------------------------------------------------------------
double Ray::getPowerAtDistance(double distance) {
    return this->power - getPathLoss(distance);
}

// Return distance given power level -----------------------------------------------------------------------------------
double Ray::getDistanceAtPower(double power) {
    return LIGHTSPEED * sqrt(power) / (4 * M_PI * this->frequency);

}

// Return origin of reflected ray --------------------------------------------------------------------------------------
point Ray::getReflectionOrigin(Surface &surface) {
    return getIntersection(this->ray, surface.getSurface());
}

// Return angle of reflected ray ---------------------------------------------------------------------------------------
double Ray::getReflectionAngle(Surface &surface) {
    double angleOfSignal = getAngle(this->ray);
    double angleOfSurface = normalizeAngle180(getAngle(surface.getSurface()));
    double angleOfIntersection = getAngleBetween(this->ray, surface.getSurface());// This is NOT the angle of incidence

    double angleOfReflection;

    // Checks 4 possible cases of ray angle and defines reflection angle
    if (angleOfSignal < _360_DEGREES) {
        if (angleOfSignal < _270_DEGREES) {
            if (angleOfSignal < _180_DEGREES) {
                if (angleOfSignal < _90_DEGREES) {
                    angleOfReflection = angleOfSurface + angleOfIntersection;
                } else angleOfReflection = angleOfSurface - angleOfIntersection;
            } else angleOfReflection = M_PI + angleOfSurface + angleOfIntersection;
        } else angleOfReflection = M_PI + angleOfSurface - angleOfIntersection;
    } else throw "ERROR: Angle of ray is greater than 2 * PI"; // Error thrown in case angle of ray is > 360 degrees

    return normalizeAngle360(angleOfReflection);
}

// Return power of reflected ray ---------------------------------------------------------------------------------------
double Ray::getReflectionPower(Surface &surface) {
    double p = getPowerAtDistance(getDistanceBetween(this->origin, getReflectionOrigin(surface)));
    double r = surface.getReflectivity();
    return p * r;
    // This could be optimized by redefining terminus after first calling getReflectionOrigin
}

// Return reflected ray given surface ----------------------------------------------------------------------------------
Ray Ray::getReflection(Surface &surface) {
    point reflectionOrigin = getReflectionOrigin(surface);
    double reflectionAngle = getReflectionAngle(surface);
    double reflectionPower = getReflectionPower(surface);

    // Unchanged/simple variables
    double reflectionFrequency = this->frequency;
    double reflectionMds = this->mds;
    int reflectionIndex = this->index + 1;
    Surface reflectionSurface = surface;

    Ray reflection(reflectionOrigin, reflectionAngle, reflectionFrequency, reflectionPower, reflectionMds,
                   reflectionIndex, reflectionSurface);
    return reflection;
}

// Return all reflected rays given all surfaces ------------------------------------------------------------------------
std::vector<Ray> Ray::getReflections(std::vector<Surface> &surfaces) {
    Ray reflection;
    std::vector<Ray> reflections;
    // Loop through all input surfaces
    for (int j = 0; j < surfaces.size(); j++) {
        // If ray intersects with a surface
        if (doIntersect(this->ray, surfaces[j].getSurface())) {
            // Find point of intersection
            point i = getIntersection(this->ray, surfaces[j].getSurface());

            // If a point of intersection is closer than any others, it is the point of reflection
            // This if statement also stops it from intersecting with the surface it originated from
            if (getDistanceBetween(this->origin, i) < this->range && surfaces[i] != this->surface) {
                // Redefine the range of the original ray
                Ray::setTerminus(i); // This needs to update range and vice versa - Matt

                // Calculate reflected ray
                reflection = getReflection(surfaces[i]);

                // Updates original ray in array of rays
                reflections[this->index] = *this; // Not sure how to correct this - Matt
                // Adds reflected ray to array of rays
                reflections.push_back(reflection);

                // Recursively calls function until intersections no longer occur
                reflection.getReflections(surfaces);
            }
        }
    }
    return reflections;
}

