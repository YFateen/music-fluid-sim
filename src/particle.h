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
#include <list>
#include <iostream>

#include "CGL/vector2D.h"
#include "CGL/color.h"

using namespace CGL;
using namespace std;


class Particle {
public:
  // Class constructers
  explicit Particle(const Vector2D &position, double radius) :
                    position(position), radius(radius), velocity({0, 0}), acceleration(), color() {
  }

  Particle(const Vector2D &position, double radius, const Vector2D &acceleration, const Vector2D &velocity,
           const Color color) :
           position(position), radius(radius), velocity(velocity), acceleration(acceleration), color(color) {
  }

  // Dynamic properties
  Vector2D position;
  Vector2D velocity;
  Vector2D acceleration;

  double radius;

  Color color;
};

class ParticleGrid {
public:
  ParticleGrid(int height, int width, float interaction_radius, float dt) :
               height(height), width(width), interaction_radius(interaction_radius), dt(dt) {
  }

  void interact_particles();

private:
  const int height;
  const int width;
  const float interaction_radius;

  float dt;

  list<Particle> particles;
  vector<vector<Particle *>> grid;

  void interact(Particle &particle, Particle &neighbor);
};

#endif /* particle_hpp */
