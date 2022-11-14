// Petrin 2022

#include "../include/Ray.hh"

// Default Constructor -------------------------------------------------------------------------------------------------
Ray::Ray() : origin(), angle(), frequency(), power(), mds(), range(), terminus(), ray(), index(), surface() {}

// Initial Ray Constructor ---------------------------------------------------------------------------------------------
Ray::Ray(point &origin, double angle, double frequency, double power, double mds) :
        origin(origin), angle(angle), frequency(frequency), power(power), mds(mds), index(0),
        terminus(), ray(), surface() {
    this->range = getDistanceAtPower(mds);
    this->terminus.x = range * cos(angle) + origin.x;
    this->terminus.y = range * sin(angle) + origin.y;
    this->ray = {origin, terminus};
}

// Reflected Ray Constructor -------------------------------------------------------------------------------------------
Ray::Ray(point &origin, double angle, double frequency, double power, double mds, int index, Surface surface) :
        origin(origin), angle(angle), frequency(frequency), power(power), mds(mds), index(index), surface(surface),
        terminus(), ray() {
    this->range = getDistanceAtPower(mds);
    this->terminus.x = range * cos(angle) + origin.x;
    this->terminus.y = range * sin(angle) + origin.y;
    this->ray = {origin, terminus};
}

// Copy Constructor ----------------------------------------------------------------------------------------------------
Ray::Ray(const Ray &ray0) : origin(ray0.origin), angle(ray0.angle), frequency(ray0.frequency), power(ray0.power), mds
        (ray0.mds), index(ray0.index), terminus(ray0.terminus), ray(ray0.ray), surface(ray0.surface) {}


// Return path loss given distance -------------------------------------------------------------------------------------
// See https://en.wikipedia.org/wiki/Free-space_path_loss
double Ray::getPathLoss(double distance) {
    return pow(4 * M_PI * distance * this->frequency / LIGHTSPEED, 2);
}

// Return power level given distance -----------------------------------------------------------------------------------
double Ray::getPowerAtDistance(double distance) {
    return this->power / getPathLoss(distance);
}

// Return distance given power level -----------------------------------------------------------------------------------
double Ray::getDistanceAtPower(double power) {
    return LIGHTSPEED * sqrt(this->power / power) / (4 * M_PI * this->frequency);
}

// Return origin of reflected ray --------------------------------------------------------------------------------------
point Ray::getReflectionOrigin(Surface surface) {
    return getIntersection(this->ray, surface.getSurface());
}

// Return angle of reflected ray ---------------------------------------------------------------------------------------
double Ray::getReflectionAngle(Surface surface) {
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
double Ray::getReflectionPower(Surface surface) {
    double p = getPowerAtDistance(getDistanceBetween(this->origin, getReflectionOrigin(surface)));
    double r = surface.getReflectivity();
    return p * r;
    // This could be optimized by redefining terminus after first calling getReflectionOrigin
}

// Return reflected ray given surface ----------------------------------------------------------------------------------
Ray Ray::getReflection(Surface surface) {
    point reflectionOrigin = getReflectionOrigin(surface);
    double reflectionAngle = getReflectionAngle(surface);
    double reflectionPower = getReflectionPower(surface);

    // Unchanged/simple variables
    double reflectionFrequency = this->frequency;
    double reflectionMds = this->mds;
    int reflectionIndex = this->index++;
    Surface reflectionSurface = surface; // This needs to be changed to a pointer in the future - Matt

    Ray reflection(reflectionOrigin, reflectionAngle, reflectionFrequency, reflectionPower, reflectionMds,
                   reflectionIndex, reflectionSurface);

    return reflection;
}

// Return all reflected rays given all surfaces ------------------------------------------------------------------------
std::vector<Ray> Ray::getReflections(std::vector<Surface> &surfaces) {
    std::vector<Ray> reflections;
    // Loop through all input surfaces
    for (int j = 0; j < surfaces.size(); j++) {
        // If ray intersects with a surface
        if (doIntersect(this->ray, surfaces[j].getSurface())) {
            // Find point of intersection
            point i = getIntersection(this->ray, surfaces[j].getSurface());

            // If a point of intersection is closer than any others, it is the point of reflection
            // This if statement also stops it from intersecting with the surface it originated from
            if (getDistanceBetween(this->origin, i) < this->range && getDistanceBetween(this->origin, i) > 0.001) {
                // Redefine the range of the original ray
                Ray::setTerminus(i); // This needs to update range and vice versa - Matt

                // Calculate reflected ray
                Ray reflection(getReflection(surfaces[j]));
                //Ray original(this->origin,this->);

                // Updates original ray in array of rays
                Ray &old = reflections[this->index];
                old = reflection;
                reflections[this->index] = Ray(reflection); // Not sure how to correct this - Matt
                // Adds reflected ray to array of rays
                reflections.push_back(reflection);

                // Recursively calls function until intersections no longer occur
                reflection.getReflections(surfaces);
            }
        }
    }
    return reflections;
}
/*
// Return all reflected rays as lines given all surfaces ---------------------------------------------------------------
std::vector<Ray> Ray::getReflectionsLines(std::vector<Surface> surfaces) {
    // Initialize vector to return
    std::vector<Ray> reflections;
    // Loop through all input surfaces
    for (int j = 0; j < surfaces.size(); j++) {
        // If ray intersects with a surface
        if (doIntersect(this->ray, surfaces[j].getSurface())) {
            // Find point of intersection
            point i = getIntersection(this->ray, surfaces[j].getSurface());

            // If a point of intersection is closer than any others, it is the point of reflection
            // This if statement also stops it from intersecting with the surface it originated from
            if (getDistanceBetween(this->origin, i) < this->range && getDistanceBetween(this->origin, i) > 0.001) {
                // Redefine the range of the original ray
                Ray::setTerminus(i); // This needs to update range and vice versa - Matt

                // Calculate reflected ray
                //Ray reflection();
                //getReflection(surfaces[j]);

                // Updates original ray in array of rays
                //reflections[this->index] = *this; //
                // Not
                // sure how to correct this -
                // Matt
                // Adds reflected ray to array of rays
                reflections.push_back(getReflection(surfaces[j]));

                // Recursively calls function until intersections no longer occur
                reflections[this->index++].getReflectionsLines(surfaces);
            }
        }
    }
    return reflections;
}
*/

// Yuck ----------------------------------------------------------------------------------------------------------------
const point &Ray::getOrigin() const {
    return origin;
}

const double Ray::getAngle() const {
    return angle;
}

const double Ray::getFrequency() const {
    return frequency;
}

const double Ray::getPower() const {
    return power;
}

const double Ray::getMds() const {
    return mds;
}

double Ray::getRange() const {
    return range;
}

void Ray::setRange(double range) {
    Ray::range = range;
}

const point &Ray::getTerminus() const {
    return terminus;
}

void Ray::setTerminus(const point &terminus) {
    Ray::terminus = terminus;
    setRange(getDistanceBetween(this->origin, this->terminus));
}

const line Ray::getRay() const {
    return ray;
}

void Ray::setRay(const line &ray) {
    Ray::ray = ray;
}

const int Ray::getIndex() const {
    return index;
}

const Surface &Ray::getSurface() const {
    return surface;
}

//--------------------------------------------------------------------

Ray &Ray::operator=(const Ray &ray0) {
    origin = ray0.origin;
    angle = ray0.angle;

    frequency;
    power = ray0.power;
    mds = ray0.mds; // Minimum detectable signal: https://en.wikipedia.org/wiki/Minimum_detectable_signal

    // Auto Generated Variables ----------------------------------------------------------------------------------------
    range = ray0.range;
    terminus = ray0.terminus;
    ray = ray0.ray; // Line representation of ray

    // Reflected Ray Variables ----------------------------------------------------------------------------------------
    index = ray0.index;

    return *this;
}


