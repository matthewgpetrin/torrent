//
// Created by Omerange on 11/16/2022.
//
#include "../include/RadioRay.hh"

int main() {
    // Test Constructor:
    RadioRay sig({0, .5}, 0, 100, .00000001, 1000000000);

    std::cout << "origin: " << sig.getOrigin().x << ", " << sig.getOrigin().y << "\n";
    std::cout << "angle: " << sig.getAngle1() << "\n";
    std::cout << "power: " << sig.getPower() << "\n";
    std::cout << "mds: " << sig.getMds() << "\n";
    std::cout << "frequency: " << sig.getFrequency() << "\n";
    std::cout << "range: " << sig.getRange() << "\n";
    std::cout << "point0: " << sig.getPoints()[0].x << ", " << sig.getPoints()[0].y << "\n";
    std::cout << "point1: " << sig.getPoints()[1].x << ", " << sig.getPoints()[1].y << "\n";

    // Test Reflections
    line wall11 = {6, 0, 7, 4};
    line wall22 = {3, 12, 9, 6};
    line wall33 = {0, 1, .5, 12};
    line wall44 = {3, 0, 4.5, 1};
    Surface wall1(wall11, 0.9);
    Surface wall2(wall22, 0.8);
    Surface wall3(wall33, 0.8);
    Surface wall4(wall44, 0.9);

    // Obstacle vector
    std::vector<Surface> walls{wall1, wall2, wall3, wall4};

    sig.reflectRay(walls);
    for (int i = 0; i < sig.getPoints().size(); i++) {
        std::cout << "point " << i << ": " << sig.getPoints()[i].x << ", " << sig.getPoints()[i].y << "\n";
    }
    sig.

}
