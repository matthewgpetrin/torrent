// Petrin 2022

#include "../include/RadioRay.hh"

// Constructor ---------------------------------------------------------------------------------------------------------
RadioRay::RadioRay(const point &origin, double angle, double power, double mds, double frequency) :
        origin(origin), angle(angle), power(power), mds(mds), frequency(frequency), points({origin}), reflectance(1) {
    this->range = distanceAtPower(power, mds);
    double x = range * cos(angle) + origin.x;
    double y = range * sin(angle) + origin.y;
    point terminus = {x, y};

    points.push_back(terminus);
}

// Generates the point vector representing reflection points ------------------------------------------------------------
void RadioRay::reflectRay(std::vector<Surface> &surfaces) {
    bool reflected = false;
    Surface mirror;
    for (int i = 0; i < surfaces.size(); i++) {
        Surface surface = surfaces[i];
        if (this->canReflectOn(surface)) {
            if (this->mayReflectOn(surface)) {
                mirror = surface;
                reflected = true;
            }
        }
    }
    if (reflected) {
        this->reflectance *= mirror.getReflectivity();
        //this->points.pop_back();
        this->points.back() = reflectionOrigin(mirror);
        this->points.push_back(reflectionTerminus(mirror));

        this->reflectRay(surfaces);
    }
}

// Returns if a surface is capable of reflecting a ray------------------------------------------------------------------
bool RadioRay::canReflectOn(Surface surface) {
    line ray = {points[points.size() - 2], points.back()};

    bool canReflect = doIntersect(ray, surface.getSurface());
    return canReflect;
}

// Returns if a surface is THE surface that will reflect a ray ---------------------------------------------------------
bool RadioRay::mayReflectOn(Surface surface) {
    line ray = {points[points.size() - 2], points.back()};
    point intersect = getIntersection(ray, surface.getSurface());

    double currentDistance = getDistanceBetween(ray.point0, ray.point1);
    double potentialDistance = getDistanceBetween(ray.point0, intersect);

    bool mayReflect = (potentialDistance < currentDistance && potentialDistance > 0.001);
    return mayReflect;
}

// Returns the origin of a reflection given a surface ------------------------------------------------------------------
point RadioRay::reflectionOrigin(Surface surface) {
    line ray = {points[points.size() - 2], points.back()};
    point reflectionOrigin = getIntersection(ray, surface.getSurface());

    return reflectionOrigin;
}

// Returns the terminus of a reflection given a surface ----------------------------------------------------------------
point RadioRay::reflectionTerminus(Surface surface) {
    double reflectionAngle = this->reflectionAngle(surface);
    double reflectionRange = this->reflectionRange(surface);

    double x = reflectionRange * cos(reflectionAngle) + reflectionOrigin(surface).x;
    double y = reflectionRange * sin(reflectionAngle) + reflectionOrigin(surface).y;
    point reflectionTerminus = {x, y};

    return reflectionTerminus;
}

// Returns the angle of a reflection given a surface -------------------------------------------------------------------
double RadioRay::reflectionAngle(Surface surface) {
    line ray = {points[points.size() - 2], points.back()};
    double surfaceAngle = ::getAngle(surface.getSurface());
    double incidentAngle = ::getAngle(ray);
    double intersectAngle = getAngleBetween(ray, surface.getSurface());// This is NOT the angle of incidence

    double reflectionAngle;

    // Checks 4 possible cases of ray angle and defines reflection angle
    if (incidentAngle < _360_DEGREES) {
        if (incidentAngle < _270_DEGREES) {
            if (incidentAngle < _180_DEGREES) {
                if (incidentAngle < _90_DEGREES) {
                    reflectionAngle = surfaceAngle + intersectAngle;
                } else reflectionAngle = surfaceAngle - intersectAngle;
            } else reflectionAngle = surfaceAngle + intersectAngle;
        } else reflectionAngle = surfaceAngle - intersectAngle;
    } else throw "ERROR: Angle of ray is greater than 2 * PI"; // Error thrown in case angle of ray is > 360 degrees

    return normalizeAngle360(reflectionAngle);
}

// Returns the ray of a reflection given a surface ---------------------------------------------------------------------
double RadioRay::reflectionRange(Surface surface) {
    double mileage = this->mileage();
    double effectivePower = reflectance * this->power;

    return distanceAtPower(effectivePower, mds) - mileage;
}

double RadioRay::mileage(){
    double mileage = 0;
    for (int i = 0; i < this->points.size() - 1; i++)
        mileage += getDistanceBetween(points[i], points[i + 1]);

    return mileage;
}

// Returns the distance between two power levels ----------------------------------------------------------------------
double RadioRay::distanceAtPower(double power0, double power1) {
    return LIGHTSPEED * sqrt(power0 / power1) / (4 * PI * this->frequency);
}

// Getters and Setters -------------------------------------------------------------------------------------------------
const point &RadioRay::getOrigin() const {
    return origin;
}

void RadioRay::setOrigin(const point &origin) {
    RadioRay::origin = origin;
}

// ---------------------------------------------------------------------------------------------------------------------
double RadioRay::getAngle1() const {
    return angle;
}

void RadioRay::setAngle(double angle) {
    RadioRay::angle = angle;
}

// ---------------------------------------------------------------------------------------------------------------------
double RadioRay::getPower() const {
    return power;
}

void RadioRay::setPower(double power) {
    RadioRay::power = power;
}

// ---------------------------------------------------------------------------------------------------------------------
double RadioRay::getMds() const {
    return mds;
}

void RadioRay::setMds(double mds) {
    RadioRay::mds = mds;
}

// ---------------------------------------------------------------------------------------------------------------------
double RadioRay::getFrequency() const {
    return frequency;
}

void RadioRay::setFrequency(double frequency) {
    RadioRay::frequency = frequency;
}

// ---------------------------------------------------------------------------------------------------------------------
double RadioRay::getRange() const {
    return range;
}

void RadioRay::setRange(double range) {
    RadioRay::range = range;
}

// ---------------------------------------------------------------------------------------------------------------------
const std::vector<point> &RadioRay::getPoints() const {
    return points;
}

void RadioRay::setPoints(const std::vector<point> &points) {
    RadioRay::points = points;
}



