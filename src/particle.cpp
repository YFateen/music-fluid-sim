//
//  particle.cpp
//
//  Created by Yusuf Fateen on 4/30/19.
//

#include "particle.h"

void ParticleGrid::update_particles() {
  // TODO: optimize this using the grid!!!
  for (Particle &particle : particles) {
    for (Particle &neighbor : particles) {
      interact(particle, neighbor);
    }
  }
  for (Particle &particle : particles) {
    move(particle);
  }
  t += dt;
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

void ParticleGrid::move(Particle &particle) {
  //
  //
  //  slightly simplified Velocity Verlet integration
  //  conserves energy better than explicit Euler method
  //4
  particle.velocity += particle.acceleration * dt;
  particle.position += particle.velocity * dt;

  //
  //  bounce from walls
  //
  double *x = &(particle.position.x);
  double *y = &(particle.position.y);
  while(*x < 0 || *x > width)
  {
    *x = *x < 0 ? -(*x) : 2*width-(*x);
    particle.velocity.x *= -1;
  }
  while(*y < 0 || *y > height)
  {
    *y = *y < 0 ? -(*y) : 2*height-(*y);
    particle.velocity.y *= -1;
  }
}

void apply_force(Particle &particle, Particle &neighbor)
{
  Vector2D dPosition = neighbor.position - particle.position;
  double r2 = dPosition.norm2();
  if( r2 > cutoff*cutoff )
    return;
//  if (r2 != 0)
//  {
//    if (r2/(cutoff*cutoff) < *dmin * (*dmin))
//      *dmin = sqrt(r2)/cutoff;
//    (*davg) += sqrt(r2)/cutoff;
//    (*navg) ++;
//  }
  r2 = fmax( r2, min_r*min_r );
  double r = sqrt( r2 );
  //
  //  very simple short-range repulsive force
  //
  double coef = ( 1 - cutoff / r ) / r2 / particle.mass;
  particle.acceleration += coef * dPosition;
}
