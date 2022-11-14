// Petrin 2022

#ifndef IFF_RAY_HH
#define IFF_RAY_HH

#define LIGHTSPEED 299792458

#include <utility>
#include "common.hh"
#include "Surface.hh"

class Ray {
// User Passed Variables ------------------------------------------------------------------------------------------
    //const
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
    int index;
    const Surface surface;

public:
    Ray();

    Ray(point &origin, double angle, double frequency, double power, double mds);

    Ray(point &origin, double angle, double frequency, double power, double mds, int index, Surface surface);

    Ray(const Ray &ray);

    Ray &operator=(const Ray &ray0);

public:
    double getPathLoss(double distance);

    double getDistanceAtPower(double power);

    double getPowerAtDistance(double distance);

public:
    point getReflectionOrigin(Surface surface);

    double getReflectionAngle(Surface surface);

    double getReflectionPower(Surface surface);

public:
    Ray getReflection(Surface surface);

    std::vector<Ray> getReflections(std::vector<Surface> &surfaces);
    
    void updateTerminus(point point);

public:
    const point &getOrigin() const;

    const double getAngle() const;

    const double getFrequency() const;

    const double getPower() const;

    const double getMds() const;

    double getRange() const;

    void setRange(double range);

    const point &getTerminus() const;

    void setTerminus(const point &terminus);

    const line getRay() const;

    void setRay(const line &ray);

    const int getIndex() const;

    const Surface &getSurface() const;

};

#endif //IFF_RAY_HH
