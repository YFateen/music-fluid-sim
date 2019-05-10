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

#define BOX_SIZE 200

#define density 0.0005
//#define mass    0.01
//#define cutoff  10.0
//#define min_r   10.0
//#define dt      0.0005

using namespace CGL;
using namespace std;


class Particle {
public:
  explicit Particle(const Vector2D &position) :
                    position(position), radius(1.0), mass(1.0), velocity({0, 0}), acceleration(), color() {
  }

  Particle(const Vector2D &position, double radius, double mass, const Vector2D &velocity, const Vector2D &acceleration,
           const Color color, const int ballid) :
           position(position), radius(radius), mass(mass), velocity(velocity), acceleration(acceleration), color(color), ballid(ballid) {
  }

  Vector2D position;
  Vector2D velocity;
  Vector2D acceleration;

  double radius;
  double mass;

  int ballid;
  Color color;
};

class ParticleGrid {
public:
  int ts = 0;

  ParticleGrid(float interaction_radius, float dt) :
               interaction_radius(interaction_radius), dt(dt) {
  }

  void resize(size_t w, size_t h) {
    width = w;
    height = h;
    grid_width = width / BOX_SIZE;
    grid_height = height / BOX_SIZE;
    grid.clear();
//    grid.reserve(grid);
//    for (int i = 0; i < height / BOX_SIZE; i++) {
//      vector<Particle *> row(width / BOX_SIZE);
//      row.assign(row.size(), )
//      grid.push_back()
//    }
    cout << width << " " << height << endl;
    cout << grid_width << " " << grid_height << endl;
  };

  void add(const Particle& particle);

  const list<Particle>* get_particles() {
    return &particles;
  }

  void update_particles(uint8_t magnitude, uint8_t onset, uint8_t beat);

private:
  size_t width = 0;
  size_t height = 0;
  size_t grid_width = 0;
  size_t grid_height = 0;
  float interaction_radius;

  float dt;

  list<Particle> particles;
  vector<vector<Particle *>> grid;

  void interact(Particle &particle, Particle &neighbor);
  void move(Particle &particle, float multiplier);
    
  bool circle_overlap(float x1, float y1, float r1, float x2, float y2, float r2);
  void particle_collision(Particle &particle, list<Particle> neighbors);
  void colliding_pairs(vector<pair<Particle*, Particle*>> vecCollidingPairs);

  void get_grid_box(Particle &particle);

  void compute_density(Particle &particle, list<Particle> &neighbors);
  void compute_pressure(Particle &particle, list<Particle> &neighbors);
};

#endif /* particle_hpp */
