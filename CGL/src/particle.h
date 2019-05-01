//
//  particle.hpp
//  CGL
//
//  Created by Yusuf Fateen on 4/30/19.
//

#ifndef particle_hpp
#define particle_hpp
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

    
class Particle {
private:
    // X and Y coordinate in integer land. [x, y]
    std::vector<int> pPosition;
    
    // Velocity in both directions [Vx, Vy]
    std::vector<double> pVelocity;
    
    // Acceleration in both directions [Ax, Ay]
    std::vector<double> pAcceleration;
    
    double pRadius;
    
    // Color for range in 0-255 [R, G, B]
    std::vector<int> pColor;
    
public:
    // Class constructers
    Particle(std::vector<int> position);
    
    Particle(std::vector<int> position, double radius, std::vector<double> acceleration, std::vector<double> velocity, std::vector<int> color);
    
    //Get helpers
    std::vector<int> getPosition() {return pPosition;}
    std::vector<double> getVelocity() {return pVelocity;}
    std::vector<double> getAcceleration() {return pAcceleration;}
    double getRadius() {return pRadius;}
    std::vector<int> getColor() {return pColor;}
    };


#endif /* particle_hpp */
