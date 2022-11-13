// Petrin 2022

#ifndef IFF_RAY_HH
#define IFF_RAY_HH

#define LIGHTSPEED 3 * pow(10, 8)

#include "common.hh"
#include "Surface.hh"

class Ray {
    // User Passed Variables ------------------------------------------------------------------------------------------
    const point origin;
    const double angle;

    const double frequency;
    const double power;
    const double mds; // Minimum detectable signal: https://en.wikipedia.org/wiki/Minimum_detectable_signal

    // Auto Generated Variables ----------------------------------------------------------------------------------------
    double range;
    point terminus;
    line ray; // Line representation of ray

    // Reflected Ray Variables ----------------------------------------------------------------------------------------
    const int index;
    const Surface &surface;

public:
    Ray();

    Ray(const point &origin, const double angle, const double frequency, const double power, const double mds);

    Ray(const point &origin, const double angle, const double frequency, const double power, const double mds,
        const int index, const Surface &surface);

public:
    double getPathLoss(double distance);

    double getDistanceAtPower(double power);

    double getPowerAtDistance(double distance);

public:
    point getReflectionOrigin(Surface &surface);

    double getReflectionAngle(Surface &surface);

    double getReflectionPower(Surface &surface);

    Ray getReflection(Surface &surface);

    std::vector<Ray> getReflections(std::vector<Surface> &surfaces);
};

#endif //IFF_RAY_HH
