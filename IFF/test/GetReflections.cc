// Testing getReflections

#include "../include/Ray.hh"

int main() {
    line wall11 = {3, 0, 4.5, 1};
    line wall22 = {3, 15, 9, 6};
    line wall33 = {0, 1, .5, 17};
    Surface wall1(wall11, 0.9);
    Surface wall2(wall22, 0.8);
    Surface wall3(wall33, 0.8);

    std::vector<Surface> walls{wall1, wall2, wall3};

    point o = {0, 0.5};
    Ray sig(o, 0, 2400000000, 10000000000000000, 0.00001);
    std::vector<Ray> refs = sig.getReflections(walls);
    std::cout << "\nsize #" << refs.size() << "\n";


    for (int i = 0; i <= refs.size(); i++) {
        std::cout << "\nREFLECTION #" << refs[i].getIndex() << "\n";
        std::cout << "origin: " << refs[i].getOrigin().x << ", " << refs[i].getOrigin().y << "\n";
        std::cout << "angle: " << refs[i].getAngle() << "\n";
        std::cout << "power: " << refs[i].getPower() << "\n";
        std::cout << "range: " << refs[i].getRange() << "\n";
    }
}