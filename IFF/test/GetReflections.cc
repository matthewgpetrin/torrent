// Testing getReflections

#include "../include/Ray.hh"

int main() {

    line wall11 = {1, 0, 2, 1};
    line wall22 = {0, 2.5, 5, 3};
    Surface wall1(wall11, 0.9);
    Surface wall2(wall22, 0.8);

    std::vector<Surface> walls{wall1, wall2};

    point o = {0, 0.5};
    Ray sig(o, 0, 2400000000, 1000000, 0.00001);
    std::vector<Ray> refs = sig.getReflections(walls);


    for (int i = 0; i < refs.size(); i++) {
        std::cout << "\nREFLECTION #" << refs[i].getIndex() << "\n";
        std::cout << "origin: " << refs[i].getOrigin().x << ", " << refs[i].getOrigin().y << "\n";
        std::cout << "angle: " << refs[i].getAngle() << "\n";
        std::cout << "power: " << refs[i].getPower() << "\n";
    }
}