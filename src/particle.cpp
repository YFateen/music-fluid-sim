//
//  particle.cpp
//
//  Created by Yusuf Fateen on 4/30/19.
//

#include "particle.h"

void ParticleGrid::update_particles(uint8_t audio_magnitude) {
  float multiplier = audio_magnitude / 255.0;
  cout << "ts: " << ts << " sig:" << multiplier << endl;
  // Apply some gravity!
  for (Particle &particle : particles) {
    particle.acceleration.y = 1000 * multiplier - 200;
    particle.acceleration.x = 100 * multiplier - 20;
  }
  // TODO: optimize this using the grid!!!
  for (Particle &particle : particles) {
    for (Particle &neighbor : particles) {
      interact(particle, neighbor);
    }
  }
  for (Particle &particle : particles) {
    move(particle, multiplier);
  }
  ts++;
}

// Placeholder!!
void apply_force(Particle &particle, Particle &neighbor);

void ParticleGrid::interact(Particle &particle, Particle &neighbor) {
  apply_force(particle, neighbor);
}

void ParticleGrid::add(const Particle& particle) {
  cout << "particle at x=" << particle.position.x << endl;
  particles.push_back(particle);
  Particle *p = &particles.back();
  // TODO: add p to grid!
}


// FOLLOWING IS *PLACEHOLDER* INTERACTION CODE FROM CS 267 HOMEWORK

void ParticleGrid::move(Particle &particle, float multiplier) {
  //
  //
  //  slightly simplified Velocity Verlet integration
  //  conserves energy better than explicit Euler method
  //
  particle.velocity += particle.acceleration * dt;
  particle.position += particle.velocity * dt;

  //
  //  bounce from walls
  //
  double *x = &(particle.position.x);
  double *y = &(particle.position.y);
  if (*x < particle.radius || *x > width-particle.radius)
  {
    *x = *x < particle.radius ? particle.radius : width-particle.radius;
    particle.velocity.x *= -1;
  }
  if (*y < particle.radius || *y > height-particle.radius)
  {
    *y = *y < particle.radius ? particle.radius : height-particle.radius;
    particle.velocity.y *= -1;
  }
  *x = max(particle.radius, min((double) (width-particle.radius), *x));
  *y = max(particle.radius, min((double) (height-particle.radius), *y));
}

void apply_force(Particle &particle, Particle &neighbor)
{
  if (&particle == &neighbor) return;
  Vector2D dPosition = neighbor.position - particle.position;
  particle.acceleration -= 0.00000001 * dPosition.unit() * dPosition.norm2() / particle.mass;
  if (dPosition.norm() < particle.radius + neighbor.radius) {
    particle.velocity = -1 * dPosition;
  }
}
