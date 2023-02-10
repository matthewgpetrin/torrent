// Petrin 2022

#include "../../include/RadioRay.hh"

int main() {
    // Test Constructor:
    RadioRay sig({0, .5}, 0, 100 , (16*pow(10, -12)), 1000000000);

    std::cout << "origin: " << sig.getOrigin().x << ", " << sig.getOrigin().y << "\n";
    std::cout << "angle: " << sig.getMds() << "\n";
    std::cout << "power: " << sig.getPower() << "\n";
    std::cout << "mds: " << sig.getMds() << "\n";
    std::cout << "frequency: " << sig.getFrequency() << "\n";
    std::cout << "range: " << sig.getRange() << "\n";
    std::cout << "point 0: " << sig.getPoints()[0].x << ", " << sig.getPoints()[0].y << "\n";
    std::cout << "point 1: " << sig.getPoints()[1].x << ", " << sig.getPoints()[1].y << "\n";

    // Test Reflections
    line wall11 = {6, 0, 7, 4};
    line wall22 = {3, 12, 9, 6};
    line wall33 = {0, 1, .5, 12};
    line wall44 = {3, 0, 4.5, 1};
    Surface wall1(wall11, .4);
    Surface wall2(wall22, .4);
    Surface wall3(wall33, .4);
    Surface wall4(wall44, .4);

    // Obstacle vector
    std::vector<Surface> walls{wall1, wall2, wall3, wall4};

    std::cout << "\nREFLECTIONS \n";
    sig.reflectRay(walls);
    for (int i = 0; i < sig.getPoints().size(); i++) {
        std::cout << "point " << i << ": " << sig.getPoints()[i].x << ", " << sig.getPoints()[i].y << "\n";
    }
    std::cout << "MILEAGE: " <<sig.mileage();
}
