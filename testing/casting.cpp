// Matthew Petrin / IFF Team 2022
#include <iostream>
#include <cmath>
#include <vector>

// User Defined Variables ----------------------------------------------------------------------------------------------
const int MAP_X = 100;
const int MAP_Y = 100;
const int MAX_REFLECTIONS = 10;
const double REFLECTIVITY = 0.4;

const int MAX_DISTANCE = sqrt(pow(double(MAP_X), 2) + pow(double(MAP_Y), 2));

// Basic Types ---------------------------------------------------------------------------------------------------------
struct point{
    double x;
    double y;
};

struct line{
    point p; // Origin
    point q; // Termination

    double length = sqrt(pow(q.x-p.x, 2) + pow(q.y-p.y, 2));
    //double a = p.y - q.y;
    //double b = q.x - p.x;
    //double c = (p.x-q.x)*p.y + (q.y-p.y)*p.x;

    double a = q.y - p.y;
    double b = -(q.x-p.x);
    double c = p.y * q.x - p.x * q.y;
};

struct ray{
    point origin; // Origin point
    double angle; // Angle in rads
    double power; // Power of signal

    line range = {origin, {origin.x+(MAX_DISTANCE*cos(angle)), origin.y+(MAX_DISTANCE*sin(angle))}};

    void update(){
        this->range = {origin, {origin.x+(MAX_DISTANCE*sin(angle)), origin.y+(MAX_DISTANCE*cos(angle))}}; // SPINGEY SPINGEY SOINGY SOINGY??????
    }
};

// Global Variables ----------------------------------------------------------------------------------------------------
const ray NULL_RAY = {{0, 0}, 0, 0};
int reflections = 0;

// Angle Finding Functions ---------------------------------------------------------------------------------------------
double distance(point point0, point point1) {
    return sqrt(pow(point1.x - point0.x, 2) + pow(point1.y - point0.y, 2));
}

double absoluteAngle(line line) {
    double a = line.q.y - line.p.y;
    double b = line.q.x - line.p.x;
    if (atan(a/b) < 0)
        return 3.1415 + atan(a/b);
    return atan(a/b);
}

double relativeAngle(line line0, line line1) {
    double a = ::distance(line1.p, line0.q);
    double b = ::distance(line0.p, line0.q);
    double c = ::distance(line0.p, line1.p);

    double ang = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
    return ang;
}

// Intersection Functions ----------------------------------------------------------------------------------------------
bool onSegment(point p, point q, point r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(point p, point q, point r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // collinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

bool doIntersect(line line0, line line1) {
    point p1 = line0.p;
    point q1 = line0.q;
    point p2 = line1.p;
    point q2 = line1.q;
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p, q1 and q are collinear and q lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // q, q2 and p are collinear and p lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // q, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

// Reflection Functions ------------------------------------------------------------------------------------------------
point intersection(line line0, line line1){
    double x = ((line0.b*line1.c) - (line1.b*line0.c))/((line0.a*line1.b) - (line1.a*line0.b));
    double y = ((line1.a*line0.c) - (line0.a*line1.c))/((line0.a*line1.b) - (line1.a*line0.b));
    return {x, y};
}

double pathLoss(double power, double distance){
    return 0;
}

std::vector<ray> getSignals(std::vector<ray> signals, std::vector<line> obstacles){ // Calculates all reflected rays
    // Define local original and reflected signals
    ray signal0 = signals[reflections];
    ray signal1 = NULL_RAY;
    // No reflections yet
    bool reflected = false;

    for (int i = 0; i < obstacles.size(); i++){ // Checks if signal reflects off any obstacles
        // Check for signal intersecting with any obstacles
        if (::doIntersect(signal0.range, obstacles[i]) && reflections < MAX_REFLECTIONS){
            // Define intersection point
            point intersection = ::intersection(signal0.range, obstacles[i]);
            // Check if obstacle is closer than all others
            if (::distance(signal0.origin, intersection) <
                ::distance(signal0.origin, signal0.range.q) &&
                // Stop signal from reflecting off origin wall
                ::distance(signal0.origin, intersection) > 0.01){
                // Redefine original signal range to this obstacle
                signal0.range.q = intersection;
                std::cout << "Reflected by obstacle " << i << "\n";

                // Define reflected signal
                signal1.origin = signal0.range.q;
                signal1.update();
                signal1.angle = ::absoluteAngle(obstacles[i]) + ::relativeAngle(signal0.range, obstacles[i]);
                signal1.power = REFLECTIVITY*(signal0.power - ::pathLoss(signal0.power, signal0.range.length));

                // Record reflection
                reflected = true;
                int obs = i;

                // Redefine original signal
                signals[reflections] = signal0;
                // Redefine reflected signal
                signals[reflections+1] = signal1;
            }
        }
    }
    // If reflection occurred
    if (reflected){
        // Update reflection count
        reflections++;
        // Check for reflections of reflected signal
        std::cout << "Number of reflections " << reflections << "\n";

        std::cout << "Reflected at {" << signal1.origin.x << ", " << signal1.origin.y << "} \n";
        std::cout << "Reflected with power " << signal1.power << "\n";
        std::cout << "Reflected at angle " << signal1.angle << "\n \n";

        getSignals(signals, obstacles);
    }
    // Return all signals
    return signals;
}

// Main ----------------------------------------------------------------------------------------------------------------
int main(){
    std::vector<line> obstacles;
    std::vector<ray> signals(10);

    ray signal = {{0, .5}, 0, 40};
    signals[0] = signal;

    obstacles.push_back({{0,3}, {2,3}});
    obstacles.push_back({{0,5}, {2,5}});
    obstacles.push_back({{0,0}, {2,2}});

    return 0;
}
