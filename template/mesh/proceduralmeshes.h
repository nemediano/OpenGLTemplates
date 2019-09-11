#ifndef PROCEDURAL_MESHES_H_
#define PROCEDURAL_MESHES_H_

#include "mesh.h"
namespace mesh {
//! Factories for procedural meshes commonly used in Computer Graphics

//! Plane mesh
/*!
  Create a mesh that represent a plane with his corresponding normals and texture coordinates
*/
Mesh plane();
//! Sphere discretized from an icosahedral base
/*!
  Creates a mesh that represnts an sphere by recursive subdivision started from an icosahedron.
  This mesh does not have normals nor texture coordinates
  @param subdiv Number of times that the recursive subdivision is applied (defaults to 3)
*/
Mesh icosphere(int subdiv = 3);
//! Sphere
/*!
  Creates a mesh that represent a sphere
  This mesh include texture coordinates. Therefore needs to duplicate some vertex
  @param slices discretization along the meridians of the sphere
  @param stacks discretization along the topi of the sphere
*/
Mesh sphere(int slices = 20, int stacks = 15);
//! Cube
/*!
  Creates a mesh that represnts an cube including normals ans texture coordinates
*/
Mesh cube();
//! Inside out Cube
/*!
  Creates a mesh that represnts an cube which normals point inward of the cube.
  Therefore you need to place the camera in the inside of the cube to use it properly. You could
  use it as a base for an skybox or for an indoor enviroment
*/
Mesh insideOutCube();
//! Cylinder
/*!
  Creates a mesh that represnts a cylinder ligned to the y-axis, centered in the xz plane
  This mesh does not include texture coordinates.
  @param slices number of vertical subdivisions (along base)
  @param staks number of horizontal subdivisions (along height)
  @param caps if false the mesh will not include the circles in their base (will be not water tight)
*/
Mesh cylinder(int slices = 20, int stacks = 1, bool caps = true);
//! Textured Cylinder
/*!
  Creates a mesh that represnts a cylinder ligned to the y-axis, centered in the xz plane
  This mesh include texture coordinates. Therefore needs to duplicate some vertex
  @param slices number of vertical subdivisions (along base)
  @param staks number of horizontal subdivisions (along height)
  @param caps if false the mesh will not include the circles in their base (will be not water tight)
*/
Mesh cylinderTexture(int slices = 20, int stacks = 1, bool caps = true);
//! Cone
/*!
  Creates a mesh that represnts a cone
  This mesh does not include texture coordinates.
  @param slices number of vertical subdivisions (along circular base)
  @param staks number of horizontal subdivisions (along height)
  @param cap if false the mesh will not include the circle in his base (will be not water tight)
*/
Mesh cone(int slices = 18, int stacks = 4, bool cap = true);
//! Textured Cone
/*!
  Creates a mesh that represnts a cone
  This mesh include texture coordinates. Therefore needs to duplicate some vertex
  @param slices number of vertical subdivisions (along circular base)
  @param staks number of horizontal subdivisions (along height)
  @param cap if false the mesh will not include the circle in his base (will be not water tight)
*/
Mesh coneTexture(int slices = 18, int stacks = 4, bool cap = true);
//! Thethrahedra
/*!
  Creates a mesh that represnts a thethrahdra
*/
Mesh tethrahedra();
//! Pyramid
/*!
  Creates a mesh that represent a pyramid
*/
Mesh pyramid();
//! Torus
/*!
  This mesh does not include texture coordinates.
  @param outerRadius relative size of the center hole of the donout
  @param innerRadius relative size of the inner tube
  @param rings discretization along the tube
  @param sides discretization one section of the tube
*/
Mesh torus(float outerRadius = 1.0f, float innerRadius = 0.25f, int rings = 36, int sides = 12);
//! Textured Torus
/*!
  Creates a mesh that represent a torus
  This mesh include texture coordinates. Therefore needs to duplicate some vertex
  @param outerRadius relative size of the center hole of the donout
  @param innerRadius relative size of the inner tube
  @param rings discretization along the tube
  @param sides discretization one section of the tube
*/
Mesh torusTexture(float outerRadius = 1.0f, float innerRadius = 0.25f, int rings = 36, int sides = 12);
//! Supershape
/*!
  Creates a mesh that represent a quadratic supershape. See for
  example: https://en.wikipedia.org/wiki/Superformula
  @param a See supershape formula documentation
  @param b See supershape formula documentation
  @param n See supershape formula documentation
  @param discretization division in one orthogonal direction
*/
Mesh superShape(float a, float b, float m, glm::vec3 n, int discretization = 128);
//! Newel's (Utha) teapot
/*!
  Creates a mesh of the famous teapot
  @param subdivisions Number of recursive subdivision of each patch
*/
Mesh teapot(int subdivisions = 6);

} //namespace mesh

#endif