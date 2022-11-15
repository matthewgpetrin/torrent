// Testing getReflections

#include "../include/Ray.hh"

int main() {
    // Define obstacles
    line wall11 = {3, 0, 4.5, 1};
    line wall22 = {3, 12, 9, 6};
    line wall33 = {0, 1, .5, 12};
    Surface wall1(wall11, 0.9);
    Surface wall2(wall22, 0.8);
    Surface wall3(wall33, 0.8);

    // Obstacle vector
    std::vector<Surface> walls{wall1, wall2, wall3};

    // Define signal ray
    point o = {0, 0.5};
    Ray sig(o, 0, 2400000000, 10000000000000000, 0.00001);

    // Reflections
    std::vector<Ray> refs = sig.getReflections(walls);

    for (int i = 0; i < refs.size(); i++) {
        std::cout << "\nREFLECTION #" << refs[i].getIndex() << "\n";
        std::cout << "origin: " << refs[i].getOrigin().x << ", " << refs[i].getOrigin().y << "\n";
        std::cout << "terminus: " << refs[i].getTerminus().x << ", " << refs[i].getTerminus().y << "\n";
        std::cout << "angle: " << refs[i].getAngle() << "\n";
        std::cout << "power: " << refs[i].getPower() << "\n";
        std::cout << "range: " << refs[i].getRange() << "\n";
    }
}