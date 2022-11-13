// Petrin 2022

#include "../src/common.hh"

double getAngleReflection(line signal, line surface) {
    double angleOfSignal = getAngle(signal);
    double angleOfSurface = normalizeAngle180(getAngle(surface));
    double angleOfIntersection = getAngleBetween(signal, surface); // This is NOT the angle of incidence

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

int main() {
    line signal{{0, 1},
                {3, 1}};

    line wall = {{3, 3},
                 {0, 0}};

    std::cout << "Wall Angle: " << getAngle(wall) << "\n";
    std::cout << "Signal Angle: " << getAngle(signal) << "\n";

    std::cout << "Between Angle: " << getAngleBetween(signal, wall) << "\n";
    std::cout << "Reflection Angle: " << getAngleReflection(signal, wall) << "\n";

    return 0;
}
