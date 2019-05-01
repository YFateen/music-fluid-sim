//
//  particle.h
//
//  Created by Yusuf Fateen on 4/30/19.
//

#ifndef CGL_PARTICLE_H
#define CGL_PARTICLE_H

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "CGL/vector2D.h"
#include "CGL/color.h"

namespace CGL{ 

class Particle {
private:
  // X and Y coordinate in integer land. [x, y]
  Vector2D pPosition;

  // Velocity in both directions [Vx, Vy]
  Vector2D pVelocity;

  // Acceleration in both directions [Ax, Ay]
  Vector2D pAcceleration;

  double pRadius{};

  // Color for range in 0-255 [R, G, B]
  Color pColor;

public:
  // Class constructers
  explicit Particle(const Vector2D &position) :
      pPosition(position), pRadius(1.0), pVelocity({0, 0}), pAcceleration(), pColor() {}

  Particle(const Vector2D &position, double radius, const Vector2D& acceleration, const Vector2D& velocity,
           const Color color) :
      pPosition(position), pRadius(radius), pVelocity(velocity), pAcceleration(acceleration), pColor(color) {}

  //Get helpers
  Vector2D getPosition() { return pPosition; }

  Vector2D getVelocity() { return pVelocity; }

  Vector2D getAcceleration() { return pAcceleration; }

  double getRadius() { return pRadius; }

  Color getColor() { return pColor; }
};
}

#endif /* particle_hpp */
