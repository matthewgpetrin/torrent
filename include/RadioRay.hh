// Petrin 2022

#ifndef IFF_RADIORAY_HH
#define IFF_RADIORAY_HH

#define PI M_PI
#define LIGHTSPEED (3 * pow(10, 8))

#include "Common.hh"
#include "Surface.hh"

class RadioRay {
private:
    point origin;
    double angle;
    double power;
    double mds;
    double frequency;

    double range;
    double reflectance;
    std::vector<point> points;

public:
    RadioRay(const point &origin, double angle, double power, double mds, double frequency);

public:
    void reflectRay(std::vector<Surface> &surfaces);

    bool canReflectOn(Surface surface);

    bool mayReflectOn(Surface surface);

    point reflectionOrigin(Surface surface);

    point reflectionTerminus(Surface surface);

    double reflectionAngle(Surface surface);

    double reflectionRange(Surface surface);

    double distanceAtPower(double power0, double power1);

public:
    const point &getOrigin() const;

    void setOrigin(const point &origin);

    double getAngle1() const;

    void setAngle(double angle);

    double getPower() const;

    void setPower(double power);

    double getMds() const;

    void setMds(double mds);

    double getFrequency() const;

    void setFrequency(double frequency);

    double getRange() const;

    void setRange(double range);

    const std::vector<point> &getPoints() const;

    void setPoints(const std::vector<point> &points);

    double mileage();
};


#endif //IFF_RADIORAY_HH
