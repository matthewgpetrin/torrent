//
// Created by Matthew on 11/13/2022.
//

#include "../include/Ray.hh"
#include "../include/Surface.hh"

int main() {

    line wall11 = {1, 0, 2, 1};
    line wall22 = {0, 3, 5, 3};
    Surface wall1(wall11, 0.9);
    Surface wall2(wall22, 0.8);

    std::vector<Surface> walls{wall1, wall2};

    point o = {0, 0.5};
    Ray sig(o, 0, 2400000000, 10, 0.00001);

    std::cout << sig.getPower();
    std::cout << sig.getAngle1();
    std::cout << "\n" << sig.getPowerAtDistance(3) << "\n";
    std::cout << "\n" << sig.getDistanceAtPower(.000109) << "\n";
    std::cout << "\n" << sig.getPathLoss(3) << "\n";
    std::cout << "\n" << sig.getRange() << "\n";


    std::cout << sig.getReflection(wall1).getOrigin().x << "\n";
    std::cout << sig.getReflection(wall1).getOrigin().y << "\n";
    std::cout << sig.getReflection(wall1).getAngle1() << "\n";
    std::cout << sig.getReflection(wall1).getPower();
}