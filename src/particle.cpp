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
    particle.velocity = 0.5 * particle.velocity + 1.5 * multiplier * particle.velocity;
//    particle.color = Color(1.0, 1.0 - multiplier, 1.0 - multiplier);
  }
  // TODO: optimize this using the grid!!!
  for (Particle &particle : particles) {
    for (Particle &neighbor : particles) {
      interact(particle, neighbor);
    }
  }
  for (Particle &particle : particles) {
    move(particle);
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

void ParticleGrid::move(Particle &particle) {
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
  *x = max(0.0, min((double) width, *x));
  *y = max(0.0, min((double) height, *y));
}

void apply_force(Particle &particle, Particle &neighbor)
{
  if (&particle == &neighbor) return;
  Vector2D dPosition = neighbor.position - particle.position;
  double r2 = dPosition.norm2();
  if( r2 > particle.radius*particle.radius )
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
  double coef = ( 1 - particle.radius / r ) / r2 / particle.mass;
  particle.acceleration += coef * dPosition;
}
