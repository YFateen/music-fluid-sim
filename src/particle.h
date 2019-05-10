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

using namespace CGL;
using namespace std;


class Particle {
public:
  explicit Particle(const Vector2D &position) :
                    position(position), radius(1.0), mass(1.0), velocity({0, 0}), acceleration(), color() {
  }

  Particle(const Vector2D &position, double radius, const Vector2D &velocity, const Vector2D &acceleration,
           const Color color, const int ballid) :
           position(position), radius(radius), mass(PI * radius * radius), velocity(velocity), acceleration(acceleration), color(color), ballid(ballid) {
  }

  Vector2D position;
  Vector2D velocity;
  Vector2D acceleration;

  double radius;
  double mass;

  int ballid;
//  Color color;
    
    void setColor(const Color c) {
        this->color = c;
    }
    
    Color getColor() const {
        return color;
    }
    
private:
    Color color;

    float density = 0;
    float pressure = 0;
};

class ParticleGrid {
public:
  int ts = 0;

  ParticleGrid(float interaction_radius, float dt) :
               interaction_radius(interaction_radius), dt(dt) {
  }

  void resize(size_t w, size_t h);

  void add(const Particle& particle);

  const list<Particle>* get_particles() {
    return &particles;
  }

  void update_particles(uint8_t magnitude, uint8_t onset, uint8_t beat, int t);

private:
  size_t width = 0;
  size_t height = 0;
  size_t grid_width = 0;
  size_t grid_height = 0;
  float interaction_radius;

  float dt;

  list<Particle> particles;
  vector<vector<Particle *>> grid;

    //Color stuff
    
    vector<Color> blue = { Color(240/255.0,248/255.0,255/255.0), Color(230/255.0,230/255.0,250/255.0), Color(176/255.0,224/255.0,230/255.0),
                           Color(173/25.0f,216/255.0,230/255.0), Color(135/255.0,206/255.0,250/255.0), Color(135/255.0,206/255.0,235/255.0),
                           Color(0/255.0,191/255.0,255/255.0), Color(176/255.0,196/255.0,222/255.0), Color(30/255.0,144/255.0,255/255.0),
                           Color(100/255.0,149/255.0,237/255.0), Color(70/255.0,130/255.0,180/255.0), Color(95/255.0,158/255.0,160/255.0),
                           Color(123/255.0,104/255.0,238/255.0), Color(106/255.0,90/255.0,205/255.0), Color(72/255.0,61/255.0,139/255.0),
                           Color(65/255.0,105/255.0,225/255.0), Color(0,0,255/255.0), Color(0,0,205/255.0),
                           Color(0,0,139/255.0), Color(0,0,128/255.0), Color(25/255.0,25/255.0,112/255.0),
                           Color(138/255.0,43/255.0,226/255.0), Color(75/255.0,0,130/255.0) };
    
    vector<Color> red = { Color(255/255.0,160/255.0,122/255.0), Color(250/255.0,128/255.0,114/255.0), Color(233/255.0,150/255.0,122/255.0),
                          Color(240/255.0,128/255.0,128/255.0), Color(205/255.0,92/255.0,92/255.0), Color(220/255.0,20/255.0,60/255.0),
                          Color(178/255.0,34/255.0,34/255.0), Color(255/255.0,0,0), Color(139/255.0,0,0),
                          Color(128/255.0,0,0), Color(255/255.0,99/255.0,71/255.0), Color(255/255.0,69/255.0,0), Color(219,112,147) };
    
    vector<Color> green = { Color(124/255.0,252/255.0,0), Color(127/255.0,255/255.0,0), Color(50/255.0,205/255.0,50/255.0),
                            Color(50/255.0,205/255.0,50/255.0), Color(0,255/255.0,0), Color(34/255.0,139/255.0,34/255.0),
                            Color(0,128/255.0,0), Color(0/255.0,100/255.0,0/255.0), Color(173/255.0,255/255.0,47/255.0),
                            Color(154/255.0,205/255.0,50/255.0), Color(0,255/255.0,127/255.0), Color(0,250/255.0,154/255.0),
                            Color(144/255.0,238/255.0,144/255.0), Color(152/255.0,251/255.0,152/255.0), Color(143/255.0,188/255.0,143/255.0),
                            Color(60/255.0,179/255.0,113/255.0), Color(32/255.0,178/255.0,170/255.0), Color(46/255.0,139/255.0,87/255.0),
                            Color(85/255.0,107/255.0,47/255.0), Color(107/255.0,142/255.0,35/255.0) };
    
    vector<Color> gold = { Color(250/255.0,250/255.0,210/255.0), Color(238/255.0,232/255.0,170/255.0), Color(240/255.0,230/255.0,140/255.0),
                           Color(218/255.0,165/255.0,32/255.0), Color(255/255.0,215/255.0,0), Color(255/255.0,165/255.0,0),
                           Color(255/255.0,140/255.0,0), Color(205/255.0,133/255.0,63/255.0), Color(210/255.0,105/255.0,30/255.0),
                           Color(139/255.0,69/255.0,19/255.0), Color(160/255.0,82/255.0,45/255.0), Color(255/255.0,223/255.0,0),
                           Color(212/255.0,175/255.0,55/255.0), Color(207/255.0,181/255.0,59/255.0), Color(197/255.0,179/255.0,88/255.0),
                           Color(230/255.0,190/255.0,138/255.0), Color(153/255.0,101/255.0,21/255.0) };
    
    vector<Color> purple = { Color(230/255.0,230/255.0,250/255.0), Color(216/255.0,191/255.0,216/255.0), Color(221/255.0,160/255.0,221/255.0),
                             Color(238/255.0,130/255.0,238/255.0), Color(218/255.0,112/255.0,214/255.0), Color(255/255.0,0,255/255.0),
                             Color(255/255.0,0,255/255.0), Color(186/255.0,85/255.0,211/255.0), Color(147/255.0,112/255.0,219/255.0),
                             Color(138/255.0,43/255.0,226/255.0), Color(148/255.0,0,211/255.0), Color(153/255.0,50/255.0,204/255.0),
                             Color(139/255.0,0,139/255.0), Color(128/255.0,0,128/255.0), Color(75/255.0,0,130/255.0) };
    
    vector<vector<Color>> rainbow = {purple, red, green, blue, gold};
    
    vector<Color> colorVector = blue;
    int colorCount = 0;
    
    
  void interact(Particle &particle, Particle &neighbor);
  void move(Particle &particle);
    
  static bool circle_overlap(float x1, float y1, float r1, float x2, float y2, float r2);
  static void particle_collision(Particle &particle, list<Particle> neighbors);
  static void colliding_pairs(const vector<pair<Particle*, Particle*>>& vecCollidingPairs);

  void init_boxes();
  vector<Particle *>* get_grid_box(Particle &particle);

  void compute_density(Particle &particle, list<Particle> &neighbors);
  void compute_pressure(Particle &particle, list<Particle> &neighbors);
};

#endif /* particle_hpp */
