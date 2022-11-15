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
Ray::Ray(const Ray &ray0) :
        origin(ray0.origin), angle(ray0.angle), frequency(ray0.frequency), power(ray0.power), mds(ray0.mds),
        range(ray0.range), index(ray0.index), terminus(ray0.terminus), ray(ray0.ray), surface(ray0.surface) {}


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
    double angleOfSignal = this->angle;
    double angleOfSurface = ::getAngle(surface.getSurface());
    double angleOfIntersection = getAngleBetween(this->ray, surface.getSurface());// This is NOT the angle of incidence

    double angleOfReflection;

    // Checks 4 possible cases of ray angle and defines reflection angle
    if (angleOfSignal < _360_DEGREES) {
        if (angleOfSignal < _270_DEGREES) {
            if (angleOfSignal < _180_DEGREES) {
                if (angleOfSignal < _90_DEGREES) {
                    angleOfReflection = angleOfSurface + angleOfIntersection;
                } else angleOfReflection = angleOfSurface - angleOfIntersection;
            } else angleOfReflection = angleOfSurface + angleOfIntersection;
        } else angleOfReflection = angleOfSurface - angleOfIntersection;
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
    int reflectionIndex = this->index + 1;
    //std::cout << "reflected index is now " << reflectionIndex << "\n";

    Surface reflectionSurface = surface; // This needs to be changed to a pointer in the future - Matt

    Ray reflection(reflectionOrigin, reflectionAngle, reflectionFrequency, reflectionPower, reflectionMds,
                   reflectionIndex, reflectionSurface);

    return reflection;
}

// Return all reflected rays given all surfaces ------------------------------------------------------------------------
std::vector<Ray> Ray::getReflections(std::vector<Surface> &surfaces) {
    std::vector<Ray> reflections{*this};
    // Loop through all input surfaces
    for (int j = 0; j < surfaces.size(); j++) {
        // If ray intersects with a surface
        if (doIntersect(this->ray, surfaces[j].getSurface()) &&
            // This if statement also stops it from intersecting with the surface it originated from - Needs to be corrected
            getDistanceBetween(this->origin, getIntersection(this->ray, surfaces[j].getSurface())) > 0.001) {
            // Find point of intersection
            point i = getIntersection(this->ray, surfaces[j].getSurface());
            // If a point of intersection is closer than any others, it is the point of reflection
            if (getDistanceBetween(this->origin, i) < this->range) {
                // Redefine the range of the original ray
                updateTerminus(i);

                Ray incident(*this);
                Ray reflection(getReflection(surfaces[j]));

                // Updates and add rays to reflections vector
                reflections.pop_back();
                reflections.push_back(incident);
                reflections.push_back(reflection);

                // Recursively calls function until intersections no longer occur
                reflection.getReflections(surfaces, reflections);
            }
        }
    }

    return reflections;
}

// Overloaded for recursive calls --------------------------------------------------------------------------------------
std::vector<Ray> Ray::getReflections(std::vector<Surface> &surfaces, std::vector<Ray> &reflections) {
    // Loop through all input surfaces
    for (int j = 0; j < surfaces.size(); j++) {
        // If ray intersects with a surface
        if (doIntersect(this->ray, surfaces[j].getSurface()) &&
            // This if statement also stops it from intersecting with the surface it originated from - Needs to be corrected
            getDistanceBetween(this->origin, getIntersection(this->ray, surfaces[j].getSurface())) > 0.001) {
            // Find point of intersection
            point i = getIntersection(this->ray, surfaces[j].getSurface());
            // If a point of intersection is closer than any others, it is the point of reflection
            if (getDistanceBetween(this->origin, i) < this->range) {
                // Redefine the range of the original ray
                updateTerminus(i);

                Ray incident(*this);
                Ray reflection(getReflection(surfaces[j]));

                // Updates and add rays to reflections vector
                reflections.pop_back();
                reflections.push_back(incident);
                reflections.push_back(reflection);

                // Recursively calls function until intersections no longer occur
                reflection.getReflections(surfaces, reflections);
            }
        }
    }

    return reflections;
}

// Updates range and terminus ------------------------------------------------------------------------------------------
void Ray::updateTerminus(point terminus) {
    this->terminus = terminus;
    this->range = getDistanceBetween(this->origin, this->terminus);
}

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