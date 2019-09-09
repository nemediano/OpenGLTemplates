#ifndef PROCEDURAL_MESHES_H_
#define PROCEDURAL_MESHES_H_

#include "mesh.h"
namespace mesh {
//! Factories for procedural meshes commonly used in Computer Graphics

//! Plane mesh
/*!
  Create a texture that represents a centered grayscaled circle. Which is very usefull as default
  stencil for a spotlight.
  @param cutoff normalized distance in [0.0f, 1.0f] from the edge to the center
  at which the stencil lets all the light pass. Defaults to 0.2 (20%)
  @param size in pixels of the texture (it's always a square texture)
*/
Mesh plane();
Mesh icosphere(int subdiv = 3);
Mesh cube();
Mesh insideOutCube();
Mesh cylinder(int slices = 20, int stacks = 1, bool caps = true);
Mesh cone(int slices = 18, int stacks = 4, bool cap = true);
Mesh coneTexture(int slices = 18, int stacks = 4, bool cap = true);
Mesh tethrahedra();
Mesh pyramid();
Mesh torus(float outerRadius = 1.0f, float innerRadius = 0.25f, int rings = 36, int sides = 12);
Mesh torusTexture(float outerRadius = 1.0f, float innerRadius = 0.25f, int rings = 36, int sides = 12);
Mesh sphere(int slices = 20, int stacks = 15);
Mesh superShape(float a, float b, float m, glm::vec3 n, int discretization = 128);
Mesh teapot(int subdivisions = 6);
Mesh cylinderTexture(int slices = 20, int stacks = 1, bool caps = true);

} //namespace mesh

#endif