//
// Created by Matthew on 11/13/2022.
//

#include "Ray.hh"
#include "Surface.hh"

int main() {

    line wall11 = {1, 1, 2, 0};
    line wall22 = {0, 3, 5, 3};
    Surface wall1(wall11, 0.9);
    Surface wall2(wall22, 0.8);

    std::vector<Surface> walls{wall1, wall2};

    point o = {0, 0.5};
    Ray sig(o, 30, 24000000, 12, 1);

    std::cout << sig.getPower();
    std::cout << sig.getAngle1();
    std::cout << "\n" << sig.getPathLoss(3) << "\n";


    std::cout << sig.getReflection(wall1).getOrigin().x << "\n";
    // std::cout << sig.getReflection(walls[i]).getOrigin().y << "\n";
    std::cout << sig.getReflection(wall1).getAngle1() << "\n";
    //std::cout << sig.getReflections(walls)[1].getOrigin().x;
    //std::cout << sig.getReflectionsLines(walls)[i].point0.y << "\n"
}