// Petrin 2022

#ifndef IFF_SURFACE_HH
#define IFF_SURFACE_HH

#include "Common.hh"

class Surface {
private:
    // User Defined Variables ------------------------------------------------------------------------------------------
    point point0;
    point point1;
    line surface;

    double reflectivity;

public:
    // Constructors ----------------------------------------------------------------------------------------------------
    Surface();

    Surface(const line &surface, double reflectivity);

    Surface(const point &point0, const point &point1, double reflectivity);

public:
    // Setters and Getters ---------------------------------------------------------------------------------------------
    const point &getPoint0() const;

    void setPoint0(const point &point0);

    const point &getPoint1() const;

    void setPoint1(const point &point1);

    const line &getSurface() const;

    void setSurface(const line &surface);

    double getReflectivity() const;

    void setReflectivity(double reflectivity);
};

#endif //IFF_SURFACE_HH
