//
//  particle.cpp
//  CGL
//
//  Created by Yusuf Fateen on 4/30/19.
//

#include "particle.h"
    
Particle::Particle(std::vector<int> position) {
        pPosition = position;
    }
    
Particle::Particle(std::vector<int> position, double radius, std::vector<double> acceleration, std::vector<double> velocity, std::vector<int> color) {

        pPosition = position;
        pRadius = radius;
        pAcceleration = acceleration;
        pVelocity = velocity;
        pColor = color;
    }
