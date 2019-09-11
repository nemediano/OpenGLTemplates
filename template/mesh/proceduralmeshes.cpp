#include "proceduralmeshes.h"

namespace mesh {
  // In this file we are going to use a lot of math, lets abreviate
  using glm::vec3;
  using glm::vec4;
  using glm::vec2;
  using glm::mat4;
  using glm::mat3;
  using namespace std;
  // Usefull constants
  const float PI =  3.14159f;
  const float TAU = 6.28318f; // Math constant equal two PI (Remember, we are in radians)

Mesh cube() {
  Mesh cube;
  vector<vec3> points(8);
  vector<vec3> normals(6);
  vector<vec2> textCoords(4);

  points[0] = 0.5f * vec3(-1.0f, -1.0f, -1.0f);
  points[1] = 0.5f * vec3(1.0f, -1.0f, -1.0f);
  points[2] = 0.5f * vec3(1.0f, 1.0f, -1.0f);
  points[3] = 0.5f * vec3(-1.0f, 1.0f, -1.0f);
  points[4] = 0.5f * vec3(-1.0f, -1.0f, 1.0f);
  points[5] = 0.5f * vec3(1.0f, -1.0f, 1.0f);
  points[6] = 0.5f * vec3(1.0f, 1.0f, 1.0f);
  points[7] = 0.5f * vec3(-1.0f, 1.0f, 1.0f);

  normals[0] = vec3(0.0f, 0.0f, -1.0f);
  normals[1] = vec3(0.0f, -1.0f, 0.0f);
  normals[2] = vec3(-1.0f, 0.0f, 0.0f);
  normals[3] = vec3(0.0f, 0.0f, 1.0f);
  normals[4] = vec3(0.0f, 1.0f, 0.0f);
  normals[5] = vec3(1.0f, 0.0f, 0.0f);

  textCoords[0] = vec2(0.0f, 0.0f);
  textCoords[1] = vec2(0.0f, 1.0f);
  textCoords[2] = vec2(1.0f, 1.0f);
  textCoords[3] = vec2(1.0f, 0.0f);

  vector<unsigned int> indices;
  vector<Vertex> vertices;
  Vertex v;
  //Back face of the cube
  v.position = points[0]; v.normal = normals[0]; v.textCoords = textCoords[2]; vertices.push_back(v); //0
  v.position = points[1]; v.normal = normals[0]; v.textCoords = textCoords[1]; vertices.push_back(v); //1
  v.position = points[2]; v.normal = normals[0]; v.textCoords = textCoords[0]; vertices.push_back(v); //2
  v.position = points[3]; v.normal = normals[0]; v.textCoords = textCoords[3]; vertices.push_back(v); //3
  indices.push_back(2);
  indices.push_back(1);
  indices.push_back(0);
  indices.push_back(3);
  indices.push_back(2);
  indices.push_back(0);
  //Bottom face of the cube
  v.position = points[0]; v.normal = normals[1]; v.textCoords = textCoords[3]; vertices.push_back(v); //4
  v.position = points[1]; v.normal = normals[1]; v.textCoords = textCoords[0]; vertices.push_back(v); //5
  v.position = points[5]; v.normal = normals[1]; v.textCoords = textCoords[1]; vertices.push_back(v); //6
  v.position = points[4]; v.normal = normals[1]; v.textCoords = textCoords[2]; vertices.push_back(v); //7
  indices.push_back(4);
  indices.push_back(5);
  indices.push_back(6);
  indices.push_back(4);
  indices.push_back(6);
  indices.push_back(7);
  //Left face of the cube
  v.position = points[0]; v.normal = normals[2]; v.textCoords = textCoords[1]; vertices.push_back(v); //8
  v.position = points[3]; v.normal = normals[2]; v.textCoords = textCoords[0]; vertices.push_back(v); //9
  v.position = points[4]; v.normal = normals[2]; v.textCoords = textCoords[2]; vertices.push_back(v); //10
  v.position = points[7]; v.normal = normals[2]; v.textCoords = textCoords[3]; vertices.push_back(v); //11
  indices.push_back(8);
  indices.push_back(10);
  indices.push_back(11);
  indices.push_back(8);
  indices.push_back(11);
  indices.push_back(9);
  //Top face of the cube
  v.position = points[2]; v.normal = normals[4]; v.textCoords = textCoords[3]; vertices.push_back(v); //12
  v.position = points[3]; v.normal = normals[4]; v.textCoords = textCoords[0]; vertices.push_back(v); //13
  v.position = points[6]; v.normal = normals[4]; v.textCoords = textCoords[2]; vertices.push_back(v); //14
  v.position = points[7]; v.normal = normals[4]; v.textCoords = textCoords[1]; vertices.push_back(v); //15
  indices.push_back(12);
  indices.push_back(13);
  indices.push_back(14);
  indices.push_back(13);
  indices.push_back(15);
  indices.push_back(14);
  //Right face of the cube
  v.position = points[1]; v.normal = normals[5]; v.textCoords = textCoords[2]; vertices.push_back(v); //16
  v.position = points[2]; v.normal = normals[5]; v.textCoords = textCoords[3]; vertices.push_back(v); //17
  v.position = points[5]; v.normal = normals[5]; v.textCoords = textCoords[1]; vertices.push_back(v); //18
  v.position = points[6]; v.normal = normals[5]; v.textCoords = textCoords[0]; vertices.push_back(v); //19
  indices.push_back(16);
  indices.push_back(17);
  indices.push_back(18);
  indices.push_back(17);
  indices.push_back(19);
  indices.push_back(18);
  //Front face of the cube
  v.position = points[4]; v.normal = normals[3]; v.textCoords = textCoords[1]; vertices.push_back(v); //20
  v.position = points[5]; v.normal = normals[3]; v.textCoords = textCoords[2]; vertices.push_back(v); //21
  v.position = points[6]; v.normal = normals[3]; v.textCoords = textCoords[3]; vertices.push_back(v); //22
  v.position = points[7]; v.normal = normals[3]; v.textCoords = textCoords[0]; vertices.push_back(v); //23
  indices.push_back(20);
  indices.push_back(21);
  indices.push_back(22);
  indices.push_back(20);
  indices.push_back(22);
  indices.push_back(23);

  cube.loadVerticesAndIndices(vertices, indices, true, true);

  return cube;
}

Mesh sphere(int slices, int stacks) {
  Mesh sphere;
  vector<unsigned int> indices;
  vector<Vertex> vertices;
  // Polar angle is tropi goes in [0, TAU]
  // Azimuth is a meridian goes in [0, PI]
  const float deltaPolar = TAU / slices;
  const float deltaAzimuth = PI / stacks;
  // To keep the CCW orientation I will sweep polar from TAU to 0
  float polar = TAU;
  float azimuth = deltaAzimuth;
  // Generate the vertices
  Vertex v;
  assert(stacks >= 2);
  assert(slices >= 3);
  float s; // For one of the texture coordinates
  const float deltaS = 1.0f / slices;
  for (int i = 0; i < (stacks - 1); ++i) {
    polar = 0.0f;
    s = 0.0f;
    for (int j = 0; j < slices; j++) {
      v.position.x = sin(azimuth) * cos(polar);
      v.position.y = cos(azimuth);
      v.position.z = sin(azimuth) * sin(polar);;

      v.normal = glm::normalize(v.position);

      v.textCoords.s = s;
      v.textCoords.t = glm::clamp(1.0f - azimuth / PI, 0.0f, 1.0f);

      vertices.push_back(v);
      polar -= deltaPolar;
      s += deltaS;
    }
    //Dummy vertexes for the texture coordinates being able to close
    v.position = vertices[vertices.size() - slices].position;
    v.normal = vertices[vertices.size() - slices].normal;
    v.textCoords.s = s;
    v.textCoords.t = glm::clamp(1.0f - azimuth / PI, 0.0f, 1.0f);
    vertices.push_back(v);
    azimuth += deltaAzimuth;
  }
  //Create triangles for the center part (i.e: calculate the indices)
  for (int i = 0; i < (stacks - 2); ++i) {
    for (int j = 0; j < slices; ++j) {
      int a = i * (slices + 1) + j;
      int b = i * (slices + 1) + (j + 1) % (slices + 1);
      int c = a + (slices + 1);
      int d = b + (slices + 1);

      indices.push_back(a);
      indices.push_back(c);
      indices.push_back(b);

      indices.push_back(b);
      indices.push_back(c);
      indices.push_back(d);
    }
  }
  /**
    Those were the central parts of the sphere. Now we need to close the shape by
    generating the north and south poles caps
  */
  //Create north triangle fan
  v.position = vec3(0.0f, 1.0f, 0.0f);
  v.normal = v.position;
  v.textCoords.t = 1.0f;
  //Triangles for the "Triangle fan" in the north
  for (int i = 0; i < slices; ++i) {
    int b = i;
    int c = (i + 1) % (slices + 1);
    //The new dummy vertex
    v.textCoords.s = (vertices[b].textCoords.s + vertices[c].textCoords.s) / 2.0f;
    vertices.push_back(v);
    //Create triangle using the new dummy vertex
    indices.push_back(static_cast<unsigned int>(vertices.size() - 1));
    indices.push_back(b);
    indices.push_back(c);
  }
  // To store where the middle vertices started in the last row (we need to connect to these)
  const int indexLast = static_cast<int>(vertices.size() - slices - 1);
  v.position = vec3(0.0f, -1.0f, 0.0f);
  v.normal = v.position;
  v.textCoords.t = 0.0f;
  //"Triangle fan" in the south
  for (int i = 0; i < slices; ++i) {
    int b = indexLast + (i + 1) % (slices + 1);
    int c = indexLast + i;
    //The new dummy vertex
    v.textCoords.s = (vertices[b].textCoords.s + vertices[c].textCoords.s) / 2.0f;
    vertices.push_back(v);
    //Create triangle using the new dummy vertex
    indices.push_back(static_cast<unsigned int>(vertices.size() - 1));
    indices.push_back(b);
    indices.push_back(c);
  }

  sphere.loadVerticesAndIndices(vertices, indices, true, true);

  return sphere;
}

Mesh cylinder(int slices, int stacks, bool caps) {

  Mesh cylinder;
  vector<unsigned int> indices;
  vector<Vertex> vertices;

  const float deltaHeight = 1.0f / stacks;
  const float deltaAngle = TAU / slices;
  for (int i = 0; i <= stacks; ++i) {
    float angle = 0.0f;
    for (int j = 0; j < slices; ++j) {
      Vertex v;
      v.position.x = cos(angle);
      v.position.y = i * deltaHeight;
      v.position.z = sin(angle);
      v.normal = glm::normalize(vec3(v.position.x, 0.0f, v.position.z));
      vertices.push_back(v);
      angle += deltaAngle;
      //Start to create the triangles from second iteration and so on
      if (j > 0 && i > 0) {
        //Create two triangle
        int a = (i - 1) * slices + (j - 1);
        int b = (i - 1) * slices + j;
        int c = i * slices + (j - 1);
        int d = i * slices + j;
        indices.push_back(c);
        indices.push_back(b);
        indices.push_back(a);

        indices.push_back(c);
        indices.push_back(d);
        indices.push_back(b);
      }
    }
    //Last two
    if (i > 0) {
      int a = (i - 1) * slices + (slices - 1);
      int b = (i - 1) * slices + 0;
      int c = i * slices + (slices - 1);
      int d = i * slices + 0;
      indices.push_back(c);
      indices.push_back(b);
      indices.push_back(a);

      indices.push_back(c);
      indices.push_back(d);
      indices.push_back(b);
    }
  }
/**
  Those were the central parts of the sphere. Now we need to close the shape by
  generating the north and south poles circular caps
*/
  if (caps) {
    int last_index = static_cast<int>(vertices.size());
    Vertex v;
    v.position = vec3(0.0f);
    v.normal = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(v);
    // Bottom cap
    float angle = 0.0f;
    float deltaAngle = TAU / slices;
    for (int i = 0; i < slices; i++) {
      Vertex u;
      u.position.x = cos(angle);
      u.position.y = 0;
      u.position.z = sin(angle);
      u.normal = vec3(0.0f, -1.0f, 0.0f);
      vertices.push_back(u);
      angle += deltaAngle;
    }
    // Remember that index start at 0
    for (int i = 1; i <= slices; i++) {
      indices.push_back(last_index);
      int tmp = i;
      indices.push_back(last_index + tmp);
      tmp = (i % slices) + 1;
      indices.push_back(last_index + tmp);
    }
    // Top cap
    last_index = static_cast<int>(vertices.size());
    v.position = vec3(0.0f, 1.0f, 0.0);
    v.normal = vec3(0.0f, 1.0f, 0.0f);
    vertices.push_back(v);
    angle = 0.0f;
    for (int i = 0; i < slices; i++) {
      Vertex u;
      u.position.x = cos(angle);
      u.position.y = 1.0f;
      u.position.z = sin(angle);
      u.normal = vec3(0.0f, 1.0f, 0.0f);
      vertices.push_back(u);
      angle += deltaAngle;
    }
    //Remember that index start at 0
    for (int i = 1; i <= slices; i++) {
      indices.push_back(last_index);
      int tmp = (i % slices) + 1;
      indices.push_back(last_index + tmp);
      tmp = i;
      indices.push_back(last_index + tmp);
    }
  }

  cylinder.loadVerticesAndIndices(vertices, indices, true, false);

  return cylinder;
}

Mesh cylinderTexture(int slices, int stacks, bool caps) {
  Mesh cylinder;
  vector<unsigned int> indices;
  vector<Vertex> vertices;

  assert(slices >= 3);
  assert(stacks >= 1);

  const float deltaHeight = 1.0f / stacks;
  const float deltaPolar = TAU / slices;
  Vertex v;
  for (int i = 0; i <= stacks; ++i) {
    float polarAngle = 0.0f;
    for (int j = 0; j < slices; ++j) {
      v.position.x = cos(polarAngle);
      v.position.y = i * deltaHeight;
      v.position.z = sin(polarAngle);
      v.normal = glm::normalize(vec3(v.position.x, 0.0f, v.position.z));
      v.textCoords.s = polarAngle / TAU;
      v.textCoords.t = v.position.y;
      vertices.push_back(v);
      polarAngle += deltaPolar;
    }
    //Dummy vertex for the texture coordinates
    v.position = vertices[vertices.size() - slices].position;
    v.normal = vertices[vertices.size() - slices].normal;
    v.textCoords.s = 1.0f;
    v.textCoords.t = v.position.y;
    vertices.push_back(v);
  }
  //To store where the middle vertices start of the last row
  int indexLast = static_cast<int>(vertices.size() - slices - 1);
  //Create triangles for the center part
  for (int i = 0; i < stacks; ++i) {
    for (int j = 0; j < slices; ++j) {
      int a = i * (slices + 1) + j;
      int b = i * (slices + 1) + (j + 1) % (slices + 1);
      int c = a + (slices + 1);
      int d = b + (slices + 1);

      indices.push_back(a);
      indices.push_back(c);
      indices.push_back(b);

      indices.push_back(b);
      indices.push_back(c);
      indices.push_back(d);
    }
  }
  //Create caps if needed
  if (caps) {
    // Create down triangle fan
    v.position = vec3(0.0f, 0.0f, 0.0f);
    v.normal = vec3(0.0f, -1.0f, 0.0f);
    v.textCoords = vec2(0.5f, 0.5f);
    vertices.push_back(v);
    int indexCenter = static_cast<int>(vertices.size() - 1);
    //Create dummy vertices (share positions with other) in the circle
    for (int i = 0; i < slices; ++i) {
      int b = i;
      int c = (i + 1) % (slices + 1);

      v.position = vertices[c].position;
      v.textCoords.s = 0.5f * v.position.x + 0.5f;
      v.textCoords.t = 0.5f * v.position.z + 0.5f;
      vertices.push_back(v);

      v.position = vertices[b].position;
      v.textCoords.s = 0.5f * v.position.x + 0.5f;
      v.textCoords.t = 0.5f * v.position.z + 0.5f;
      vertices.push_back(v);

      indices.push_back(indexCenter);
      indices.push_back(static_cast<int>(vertices.size() - 1));
      indices.push_back(static_cast<int>(vertices.size() - 2));
    }
    //Create upper triangle fan
    v.position = vec3(0.0f, 1.0f, 0.0f);
    v.normal = v.position;
    v.textCoords = vec2(0.5f, 0.5f);
    vertices.push_back(v);
    indexCenter = static_cast<int>(vertices.size() - 1);
    //Create dummy vertices (share positions with other) in the circle
    for (int i = 0; i < slices; ++i) {
      int b = indexLast + (i + 1) % (slices + 1);
      int c = indexLast + i;

      v.position = vertices[c].position;
      v.textCoords.s = 0.5f * v.position.x + 0.5f;
      v.textCoords.t = 0.5f * v.position.z + 0.5f;
      vertices.push_back(v);

      v.position = vertices[b].position;
      v.textCoords.s = 0.5f * v.position.x + 0.5f;
      v.textCoords.t = 0.5f * v.position.z + 0.5f;
      vertices.push_back(v);

      indices.push_back(indexCenter);
      indices.push_back(static_cast<int>(vertices.size() - 1));
      indices.push_back(static_cast<int>(vertices.size() - 2));
    }
  }

  cylinder.loadVerticesAndIndices(vertices, indices, true, true);

  return cylinder;
}

/**************************************************************************************************/
/* Functions related to the teapot mesh                                                           */
/**************************************************************************************************/
// Indices for the patch vertices for the teapot
vector< vector<unsigned int> > patchIndices = {
  // rim
  { 102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
  // body
  { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 },{ 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40 },
  // lid
  { 96, 96, 96, 96, 97, 98, 99, 100, 101, 101, 101, 101, 0, 1, 2, 3, },{ 0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117 },
  // bottom
  { 118, 118, 118, 118, 124, 122, 119, 121, 123, 126, 125, 120, 40, 39, 38, 37 },
  // handle
  { 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56 },{ 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 28, 65, 66, 67 },
  // spout
  { 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83 },{ 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95 }
};
// Shape constants
const float LID = 1.0f;
const float LID_Z = 1.0f;
// Control points of the patches
vector<vec3> curveData{
  { 0.2f*LID, 0.f*LID, 2.7f*LID_Z },{ 0.2f*LID, -0.112f*LID, 2.7f*LID_Z },{ 0.112f*LID, -0.2f*LID, 2.7f*LID_Z },{ 0.f*LID, -0.2f*LID, 2.7f*LID_Z },
  { 1.3375f, 0.f, 2.53125f },{ 1.3375f, -0.749f, 2.53125f },{ 0.749f, -1.3375f, 2.53125f },{ 0.f, -1.3375f, 2.53125f },
  { 1.4375f, 0.f, 2.53125f },{ 1.4375f, -0.805f, 2.53125f },{ 0.805f, -1.4375f, 2.53125f },{ 0.f, -1.4375f, 2.53125f },
  { 1.5f, 0.f, 2.4f },{ 1.5f, -0.84f, 2.4f },{ 0.84f, -1.5f, 2.4f },{ 0.f, -1.5f, 2.4f },{ 1.75f, 0.f, 1.875f },
  { 1.75f, -0.98f, 1.875f },{ 0.98f, -1.75f, 1.875f },{ 0.f, -1.75f, 1.875f },{ 2.f, 0.f, 1.35f },{ 2.f, -1.12f, 1.35f },
  { 1.12f, -2.f, 1.35f },{ 0.f, -2.f, 1.35f },{ 2.f, 0.f, 0.9f },{ 2.f, -1.12f, 0.9f },{ 1.12f, -2.f, 0.9f },
  { 0.f, -2.f, 0.9f },{ -2.f, 0.f, 0.9f },{ 2.f, 0.f, 0.45f },{ 2.f, -1.12f, 0.45f },{ 1.12f, -2.f, 0.45f },{ 0.f, -2.f, 0.45f },
  // 33
  { 1.5f, 0.f, 0.225f },
  { 1.5f, -0.84f, 0.225f },{ 0.84f, -1.5f, 0.225f },{ 0.f, -1.5f, 0.225f },{ 1.5f, 0.f, 0.15f },{ 1.5f,
  -0.84f, 0.15f },{ 0.84f, -1.5f, 0.15f },{ 0.f, -1.5f, 0.15f },{ -1.6f, 0.f, 2.025f },{ -1.6f, -0.3f,
  2.025f },{ -1.5f, -0.3f, 2.25f },{ -1.5f, 0.f, 2.25f },{ -2.3f, 0.f, 2.025f },{ -2.3f, -0.3f, 2.025f },
  { -2.5f, -0.3f, 2.25f },{ -2.5f, 0.f, 2.25f },{ -2.7f, 0.f, 2.025f },{ -2.7f, -0.3f, 2.025f },{ -3.f,
  -0.3f, 2.25f },{ -3.f, 0.f, 2.25f },{ -2.7f, 0.f, 1.8f },{ -2.7f, -0.3f, 1.8f },{ -3.f, -0.3f, 1.8f },
  { -3.f, 0.f, 1.8f },{ -2.7f, 0.f, 1.575f },{ -2.7f, -0.3f, 1.575f },{ -3.f, -0.3f, 1.35f },{ -3.f, 0.f,
  1.35f },{ -2.5f, 0.f, 1.125f },{ -2.5f, -0.3f, 1.125f },{ -2.65f, -0.3f, 0.9375f },{ -2.65f, 0.f,
  0.9375f },{ -2.f, -0.3f, 0.9f },{ -1.9f, -0.3f, 0.6f },
  // 67
  { -1.9f, 0.f, 0.6f },{ 1.7f, 0.f, 1.425f },{ 1.7f, -0.66f, 1.425f },{ 1.7f, -0.66f, 0.6f },{ 1.7f, 0.f, 0.6f },{ 2.6f, 0.f, 1.425f },{ 2.6f, -0.66f,
  1.425f },{ 3.1f, -0.66f, 0.825f },{ 3.1f, 0.f, 0.825f },{ 2.3f, 0.f, 2.1f },{ 2.3f, -0.25f, 2.1f },
  { 2.4f, -0.25f, 2.025f },{ 2.4f, 0.f, 2.025f },{ 2.7f, 0.f, 2.4f },{ 2.7f, -0.25f, 2.4f },{ 3.3f, -0.25f,
  2.4f },{ 3.3f, 0.f, 2.4f },{ 2.8f, 0.f, 2.475f },{ 2.8f, -0.25f, 2.475f },{ 3.525f, -0.25f, 2.49375f },
  { 3.525f, 0.f, 2.49375f },{ 2.9f, 0.f, 2.475f },{ 2.9f, -0.15f, 2.475f },{ 3.45f, -0.15f, 2.5125f },
  { 3.45f, 0.f, 2.5125f },{ 2.8f, 0.f, 2.4f },{ 2.8f, -0.15f, 2.4f },{ 3.2f, -0.15f, 2.4f },{ 3.2f, 0.f,
  2.4f },
  // 96:
  { 0.f*LID, 0.f*LID, 3.15f*LID_Z },{ 0.8f*LID, 0.f*LID, 3.15f*LID_Z },{ 0.8f*LID, -0.45f*LID, 3.15f*LID_Z },{ 0.45f*LID, -0.8f*LID, 3.15f*LID_Z },
  { 0.f*LID, -0.8f*LID, 3.15f*LID_Z },{ 0.f*LID, 0.f*LID, 2.85f*LID_Z },
  // 102:
  { 1.4f, 0.f, 2.4f },{ 1.4f, -0.784f, 2.4f },{ 0.784f, -1.4f, 2.4f },{ 0.f, -1.4f, 2.4f },
  // 106:
  { 0.4f*LID, 0.f*LID, 2.55f*LID_Z },{ 0.4f*LID, -0.224f*LID, 2.55f*LID_Z },{ 0.224f*LID, -0.4f*LID, 2.55f*LID_Z },
  { 0.f*LID, -0.4f*LID,2.55f*LID_Z },{ 1.3f*LID, 0.f*LID, 2.55f*LID_Z },{ 1.3f*LID, -0.728f*LID, 2.55f*LID_Z },{ 0.728f*LID, -1.3f*LID, 2.55f*LID_Z },{ 0.f*LID, -1.3f*LID, 2.55f*LID_Z },
  { 1.3f*LID, 0.f*LID, 2.4f*LID_Z },{ 1.3f*LID, -0.728f*LID, 2.4f*LID_Z },{ 0.728f*LID, -1.3f*LID, 2.4f*LID_Z },{ 0.f*LID, -1.3f*LID, 2.4f*LID_Z },
  // 118:
  { 0.f, 0.f, 0.f },{ 1.425f, -0.798f, 0.f },{ 1.5f, 0.f, 0.075f },{ 1.425f, 0.f, 0.f },{ 0.798f, -1.425f, 0.f },{ 0.f,
  -1.5f, 0.075f },{ 0.f, -1.425f, 0.f },{ 1.5f, -0.84f, 0.075f },{ 0.84f, -1.5f, 0.075f }
};


vec3 evaluate(int gridU, int gridV, const vector<float>& B, const vec3 patch[][4]) {
  vec3 p;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      p += patch[i][j] * B[gridU * 4 + i] * B[gridV * 4 + j];
    }
  }

  return p;
}

vec3 evaluateNormal(int gridU, int gridV, const vector<float>& B, const vector<float>& dB, const vec3 patch[][4]) {
  vec3 du, dv;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      du += patch[i][j] * dB[gridU * 4 + i] * B[gridV * 4 + j];
      dv += patch[i][j] * B[gridU * 4 + i] * dB[gridV * 4 + j];
    }
  }

  return normalize(cross(du, dv));
}

void getPatch(int patchNum, vec3 patch[][4], bool reverseV) {
  for (int u = 0; u < 4; u++) {          // Loop in u direction
    for (int v = 0; v < 4; v++) {     // Loop in v direction
      if (reverseV) {
        patch[u][v] = vec3(
          curveData[patchIndices[patchNum][u * 4 + (3 - v)]][0],
          curveData[patchIndices[patchNum][u * 4 + (3 - v)]][1],
          curveData[patchIndices[patchNum][u * 4 + (3 - v)]][2]
        );
      }
      else {
        patch[u][v] = vec3(
          curveData[patchIndices[patchNum][u * 4 + v]][0],
          curveData[patchIndices[patchNum][u * 4 + v]][1],
          curveData[patchIndices[patchNum][u * 4 + v]][2]
        );
      }
    }
  }
}

void buildPatch(vec3 patch[][4], const vector<float>& B, const vector<float>& dB, vector<Vertex>& vertices,
  vector<unsigned int>& indices, int &index, int &elIndex, int &tcIndex, int grid, mat3 reflect, bool invertNormal) {
  int startIndex = index;
  float tcFactor = 1.0f / grid;

  float scale = 2.0f / 6.42813f; // awful hack to keep it within unit cube

  for (int i = 0; i <= grid; i++) {
    for (int j = 0; j <= grid; j++) {
      vec3 pt = reflect * evaluate(i, j, B, patch);
      vec3 norm = reflect * evaluateNormal(i, j, B, dB, patch);
      if (invertNormal)
        norm = -norm;
      // awful hack due to normals discontinuity
      if (abs(pt.x) < 0.01f && abs(pt.y) < 0.01f)
        norm = (pt.z < 1) ? vec3(0, 0, -1) : vec3(0, 0, 1);

      vertices[index].position = pt * scale;
      vertices[index].normal = norm;
      vertices[index].textCoords = tcFactor * vec2(i, j);
      index++;
      tcIndex += 2;
    }
  }

  for (int i = 0; i < grid; i++) {
    int iStart = i * (grid + 1) + startIndex;
    int nextiStart = (i + 1) * (grid + 1) + startIndex;
    for (int j = 0; j < grid; j++) {
      indices[elIndex] = iStart + j;
      indices[elIndex + 1] = nextiStart + j;
      indices[elIndex + 2] = nextiStart + j + 1;

      indices[elIndex + 3] = iStart + j;
      indices[elIndex + 4] = nextiStart + j + 1;
      indices[elIndex + 5] = iStart + j + 1;

      elIndex += 6;
    }
  }
}

void buildPatchReflect(int patchNum, const vector<float>& B, const vector<float>& dB, vector<Vertex>& vertices, vector<unsigned int>& indices,
  int &index, int &elIndex, int &tcIndex, int grid, bool reflectX, bool reflectY) {

  vec3 patch[4][4];
  vec3 patchRevV[4][4];

  mat4 I(1.0f);

  getPatch(patchNum, patch, false);
  getPatch(patchNum, patchRevV, true);

  // Patch without modification
  buildPatch(patchRevV, B, dB, vertices, indices, index, elIndex, tcIndex, grid, mat3(), false);

  // Patch reflected in x
  if (reflectX) {
    mat3 reflect(mat3(glm::scale(I, vec3(-1, 1, 1))));
    buildPatch(patch, B, dB, vertices, indices, index, elIndex, tcIndex, grid, reflect, true);
  }

  // Patch reflected in y
  if (reflectY) {
    mat3 reflect(mat3(glm::scale(I, vec3(1, -1, 1))));
    buildPatch(patch, B, dB, vertices, indices, index, elIndex, tcIndex, grid, reflect, true);
  }

  // Patch reflected in x and y
  if (reflectX && reflectY) {
    mat3 reflect(mat3(glm::scale(I, vec3(-1, -1, 1))));
    buildPatch(patchRevV, B, dB, vertices, indices, index, elIndex, tcIndex, grid, reflect, false);
  }

}

void computeBasisFunctions(vector<float>& B, vector<float>& dB, int grid) {
  float inc = 1.0f / grid;
  for (int i = 0; i <= grid; i++) {
    float t = i * inc;
    float tSqr = t * t;
    float oneMinusT = (1.0f - t);
    float oneMinusT2 = oneMinusT * oneMinusT;

    B[i * 4 + 0] = oneMinusT * oneMinusT2;
    B[i * 4 + 1] = 3.0f * oneMinusT2 * t;
    B[i * 4 + 2] = 3.0f * oneMinusT * tSqr;
    B[i * 4 + 3] = t * tSqr;

    dB[i * 4 + 0] = -3.0f * oneMinusT2;
    dB[i * 4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
    dB[i * 4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
    dB[i * 4 + 3] = 3.0f * tSqr;
  }
}

void generatePatches(vector<Vertex>& vertices, vector<unsigned int>& indices, int grid) {
  vector<float> B(4 * (grid + 1));  // Pre-computed Bernstein basis functions
  vector<float> dB(4 * (grid + 1)); // Pre-computed derivitives of basis functions
  int idx = 0, elIndex = 0, tcIndex = 0;

  // Pre-compute the basis functions  (Bernstein polynomials)
  // and their derivatives
  computeBasisFunctions(B, dB, grid);

  // Build each patch
  // The rim
  buildPatchReflect(0, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, true, true);
  // The body
  buildPatchReflect(1, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, true, true);
  buildPatchReflect(2, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, true, true);
  // The lid
  buildPatchReflect(3, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, true, true);
  buildPatchReflect(4, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, true, true);
  // The bottom
  buildPatchReflect(5, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, true, true);
  // The handle
  buildPatchReflect(6, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, false, true);
  buildPatchReflect(7, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, false, true);
  // The spout
  buildPatchReflect(8, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, false, true);
  buildPatchReflect(9, B, dB, vertices, indices, idx, elIndex, tcIndex, grid, false, true);
}

Mesh teapot(int subdivisions) {
  Mesh teapot;
  vector<unsigned int> indices;
  vector<Vertex> vertices;

  vertices.resize(32 * (subdivisions + 1) * (subdivisions + 1));
  indices.resize(6 * subdivisions * subdivisions * 32);
  generatePatches(vertices, indices, subdivisions);

  teapot.loadVerticesAndIndices(vertices, indices, true, true);

  glm::vec3 c = teapot.getBBCenter();
  float scaleFactor = teapot.scaleFactor();

  glm::mat4 T = glm::rotate(mat4(1.0f), -TAU / 4.0f, vec3(1.0f, 0.0f, 0.0f));
  T = glm::scale(T, glm::vec3(2.0f * scaleFactor));
  T = glm::translate(T, -c);

  teapot.transform(T);

  return teapot;
}
/**************************************************************************************************/
/* End of functions related to the teapot creation                                                */
/**************************************************************************************************/

Mesh torus(float outerRadius, float innerRadius, int rings, int sides) {
  Mesh torus;
  vector<unsigned int> indices;
  vector<Vertex> vertices;
  //Create a circular section
  float angle = 0.0f;
  float deltaAngle = TAU / sides;
  vector<Vertex> circle;
  for (int i = 0; i < sides; ++i) {
    Vertex v;
    v.position.x = innerRadius * cos(angle);
    v.position.y = innerRadius * sin(angle);
    v.position.z = 0.0f;
    v.normal = glm::normalize(v.position);
    angle += deltaAngle;
    circle.push_back(v);
  }
  //Transform the circle and insert it in vector
  angle = 0.0;
  deltaAngle = TAU / rings;
  for (int i = 0; i < rings; ++i) {
    mat4 T(1.0f);
    T = glm::rotate(T, angle, vec3(0.0f, 1.0f, 0.0));
    T = glm::translate(T, vec3(outerRadius, 0.0f, 0.0f));
    vector<Vertex> transformedCircle = circle;
    for (size_t j = 0; j < circle.size(); ++j) {
      transformedCircle[j].position = vec3(T * vec4(circle[j].position, 1.0f));
      transformedCircle[j].normal = vec3(glm::inverse(glm::transpose(T)) * vec4(circle[j].normal, 0.0f));
    }
    vertices.insert(vertices.end(), transformedCircle.begin(), transformedCircle.end());
    angle += deltaAngle;
  }
  //Create the triangles
  for (int i = 0; i < rings; ++i) {
    /*These are the literal vertex of the quadrialteral
      that I want to create using two triangles.
      I could not came up with a better naming convention */
    int a, b, c, d;
    for (int j = 0; j < sides; ++j) {
      a = i * sides + j;
      b = i * sides + ((j + 1) % sides);
      c = ((i + 1) % rings) * sides + j;
      d = ((i + 1) % rings) * sides + ((j + 1) % sides);
      //Create the two triangles
      indices.push_back(a);
      indices.push_back(d);
      indices.push_back(b);

      indices.push_back(a);
      indices.push_back(c);
      indices.push_back(d);
    }
  }

  torus.loadVerticesAndIndices(vertices, indices, true, false);

  return torus;
}

Mesh torusTexture(float outerRadius, float innerRadius, int rings, int sides) {
  Mesh torus;
  vector<unsigned int> indices;
  vector<Vertex> vertices;

  Vertex v;
  //Create a circular section
  float angle = 0.0f;
  float deltaAngle = TAU / sides;
  vector<Vertex> circle;
  float t = 0.0f;
  float deltaT = 1.0f / sides;
  for (int i = 0; i < sides; ++i) {
    v.position.x = innerRadius * cos(angle);
    v.position.y = innerRadius * sin(angle);
    v.position.z = 0.0f;
    v.normal = glm::normalize(v.position);
    v.textCoords.t = t;
    v.textCoords.s = 0.0f;
    angle += deltaAngle;
    t += deltaT;
    circle.push_back(v);
  }
  //One dummy vertex for the texture coordinates being able to close
  v.position = circle[0].position;
  v.normal = circle[0].normal;
  v.textCoords.t = 1.0f;
  v.textCoords.s = 0.0f;
  circle.push_back(v);
  //Transform the circle and insert it in vector
  angle = 0.0;
  deltaAngle = TAU / rings;
  float s = 0.0f;
  float deltaS = 1.0f / rings;
  for (int i = 0; i < rings; ++i) {
    mat4 T(1.0f);
    T = glm::rotate(T, angle, vec3(0.0f, 1.0f, 0.0));
    T = glm::translate(T, vec3(outerRadius, 0.0f, 0.0f));
    vector<Vertex> transformedCircle = circle;
    for (size_t j = 0; j < circle.size(); ++j) {
      transformedCircle[j].position = vec3(T * vec4(circle[j].position, 1.0f));
      transformedCircle[j].normal = vec3(glm::inverse(glm::transpose(T)) * vec4(circle[j].normal, 0.0f));
      transformedCircle[j].textCoords.s = s;
    }
    vertices.insert(vertices.end(), transformedCircle.begin(), transformedCircle.end());
    s += deltaS;
    angle += deltaAngle;
  }
  //An extra circle for texture coordinates
  vector<Vertex> transformedCircle = circle;
  for (size_t j = 0; j < circle.size(); ++j) {
    transformedCircle[j].position = vertices[j].position;
    transformedCircle[j].normal = vertices[j].normal;
    transformedCircle[j].textCoords.s = 1.0f;
  }
  vertices.insert(vertices.end(), transformedCircle.begin(), transformedCircle.end());
  //Create the triangles
  for (int i = 0; i < rings; ++i) {
    /*These are the literal vertex of the quadrialteral
    that I want to create using two triangles.
    I could not came up with a better naming convention */
    int a, b, c, d;
    for (int j = 0; j < sides; ++j) {
      a = i * (sides + 1) + j;
      b = i * (sides + 1) + ((j + 1) % (sides + 1));
      c = ((i + 1) % (rings + 1)) * (sides + 1) + j;
      d = ((i + 1) % (rings + 1)) * (sides + 1) + ((j + 1) % (sides + 1));
      //Create the two triangles
      indices.push_back(a);
      indices.push_back(d);
      indices.push_back(b);

      indices.push_back(a);
      indices.push_back(c);
      indices.push_back(d);
    }
  }

  torus.loadVerticesAndIndices(vertices, indices, true, true);

  return torus;
}

Mesh insideOutCube() {
  Mesh cube;

  vector<vec3> points(8);
  vector<vec3> normals(6);
  vector<vec2> textCoords(4);

  points[0] = 0.5f * vec3(-1.0f, -1.0f, -1.0f);
  points[1] = 0.5f * vec3(1.0f, -1.0f, -1.0f);
  points[2] = 0.5f * vec3(1.0f, 1.0f, -1.0f);
  points[3] = 0.5f * vec3(-1.0f, 1.0f, -1.0f);
  points[4] = 0.5f * vec3(-1.0f, -1.0f, 1.0f);
  points[5] = 0.5f * vec3(1.0f, -1.0f, 1.0f);
  points[6] = 0.5f * vec3(1.0f, 1.0f, 1.0f);
  points[7] = 0.5f * vec3(-1.0f, 1.0f, 1.0f);

  normals[0] = vec3(0.0f, 0.0f, -1.0f);
  normals[1] = vec3(0.0f, -1.0f, 0.0f);
  normals[2] = vec3(-1.0f, 0.0f, 0.0f);
  normals[3] = vec3(0.0f, 0.0f, 1.0f);
  normals[4] = vec3(0.0f, 1.0f, 0.0f);
  normals[5] = vec3(1.0f, 0.0f, 0.0f);

  textCoords[0] = vec2(0.0f, 0.0f);
  textCoords[1] = vec2(0.0f, 1.0f);
  textCoords[2] = vec2(1.0f, 1.0f);
  textCoords[3] = vec2(1.0f, 0.0f);

  vector<unsigned int> indices;
  vector<Vertex> vertices;
  Vertex v;
  //Back face of the cube
  v.position = points[0]; v.normal = normals[3]; v.textCoords = textCoords[2]; vertices.push_back(v); //0
  v.position = points[1]; v.normal = normals[3]; v.textCoords = textCoords[1]; vertices.push_back(v); //1
  v.position = points[2]; v.normal = normals[3]; v.textCoords = textCoords[0]; vertices.push_back(v); //2
  v.position = points[3]; v.normal = normals[3]; v.textCoords = textCoords[3]; vertices.push_back(v); //3
  indices.push_back(2);
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);
  indices.push_back(0);
  indices.push_back(2);
  //Bottom face of the cube
  v.position = points[0]; v.normal = normals[4]; v.textCoords = textCoords[3]; vertices.push_back(v); //4
  v.position = points[1]; v.normal = normals[4]; v.textCoords = textCoords[0]; vertices.push_back(v); //5
  v.position = points[5]; v.normal = normals[4]; v.textCoords = textCoords[1]; vertices.push_back(v); //6
  v.position = points[4]; v.normal = normals[4]; v.textCoords = textCoords[2]; vertices.push_back(v); //7
  indices.push_back(4);
  indices.push_back(6);
  indices.push_back(5);
  indices.push_back(4);
  indices.push_back(7);
  indices.push_back(6);
  //Left face of the cube
  v.position = points[0]; v.normal = normals[5]; v.textCoords = textCoords[1]; vertices.push_back(v); //8
  v.position = points[3]; v.normal = normals[5]; v.textCoords = textCoords[0]; vertices.push_back(v); //9
  v.position = points[4]; v.normal = normals[5]; v.textCoords = textCoords[2]; vertices.push_back(v); //10
  v.position = points[7]; v.normal = normals[5]; v.textCoords = textCoords[3]; vertices.push_back(v); //11
  indices.push_back(8);
  indices.push_back(11);
  indices.push_back(10);
  indices.push_back(8);
  indices.push_back(9);
  indices.push_back(11);
  //Top face of the cube
  v.position = points[2]; v.normal = normals[1]; v.textCoords = textCoords[3]; vertices.push_back(v); //12
  v.position = points[3]; v.normal = normals[1]; v.textCoords = textCoords[0]; vertices.push_back(v); //13
  v.position = points[6]; v.normal = normals[1]; v.textCoords = textCoords[2]; vertices.push_back(v); //14
  v.position = points[7]; v.normal = normals[1]; v.textCoords = textCoords[1]; vertices.push_back(v); //15
  indices.push_back(12);
  indices.push_back(14);
  indices.push_back(13);
  indices.push_back(13);
  indices.push_back(14);
  indices.push_back(15);
  //Right face of the cube
  v.position = points[1]; v.normal = normals[2]; v.textCoords = textCoords[2]; vertices.push_back(v); //16
  v.position = points[2]; v.normal = normals[2]; v.textCoords = textCoords[3]; vertices.push_back(v); //17
  v.position = points[5]; v.normal = normals[2]; v.textCoords = textCoords[1]; vertices.push_back(v); //18
  v.position = points[6]; v.normal = normals[2]; v.textCoords = textCoords[0]; vertices.push_back(v); //19
  indices.push_back(16);
  indices.push_back(18);
  indices.push_back(17);
  indices.push_back(17);
  indices.push_back(18);
  indices.push_back(19);
  //Front face of the cube
  v.position = points[4]; v.normal = normals[0]; v.textCoords = textCoords[1]; vertices.push_back(v); //20
  v.position = points[5]; v.normal = normals[0]; v.textCoords = textCoords[2]; vertices.push_back(v); //21
  v.position = points[6]; v.normal = normals[0]; v.textCoords = textCoords[3]; vertices.push_back(v); //22
  v.position = points[7]; v.normal = normals[0]; v.textCoords = textCoords[0]; vertices.push_back(v); //23
  indices.push_back(20);
  indices.push_back(22);
  indices.push_back(21);
  indices.push_back(20);
  indices.push_back(23);
  indices.push_back(22);

  cube.loadVerticesAndIndices(vertices, indices, true, true);

  return cube;
}

Mesh pyramid() {
  Mesh pyramid;
  vector<unsigned int> indices;
  vector<Vertex> vertices(16);

  vec3 positions[] = {
    vec3( 0.0f, 1.0f,  0.0f),  // 0
    vec3( 1.0f, 0.0f, -1.0f),  // 1
    vec3( 1.0f, 0.0f,  1.0f),  // 2
    vec3(-1.0f, 0.0f, -1.0f),  // 3
    vec3(-1.0f, 0.0f,  1.0f)   // 4
  };

  vec2 textCoords[] = {
    vec2(0.5f, 0.0f),   //0
    vec2(0.25f, 0.25f), //1
    vec2(0.75f, 0.25f), //2
    vec2(0.0f, 0.5f),   //3
    vec2(1.0f, 0.5f),   //4
    vec2(0.25f, 0.75f), //5
    vec2(0.75f, 0.75f), //6
    vec2(0.5f, 1.0f)    //7
  };

  //Front face
  vec3 normal = glm::triangleNormal(positions[0], positions[4], positions[2]);
  vertices[0].position = positions[0];
  vertices[0].textCoords = textCoords[7];
  vertices[0].normal = normal;

  vertices[1].position = positions[4];
  vertices[1].textCoords = textCoords[5];
  vertices[1].normal = normal;

  vertices[2].position = positions[2];
  vertices[2].textCoords = textCoords[6];
  vertices[2].normal = normal;

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);

  //Left face
  normal = glm::triangleNormal(positions[0], positions[2], positions[1]);
  vertices[3].position = positions[0];
  vertices[3].textCoords = textCoords[4];
  vertices[3].normal = normal;

  vertices[4].position = positions[2];
  vertices[4].textCoords = textCoords[6];
  vertices[4].normal = normal;

  vertices[5].position = positions[1];
  vertices[5].textCoords = textCoords[2];
  vertices[5].normal = normal;

  indices.push_back(3);
  indices.push_back(4);
  indices.push_back(5);

  //back face
  normal = glm::triangleNormal(positions[0], positions[1], positions[3]);
  vertices[6].position = positions[0];
  vertices[6].textCoords = textCoords[0];
  vertices[6].normal = normal;

  vertices[7].position = positions[1];
  vertices[7].textCoords = textCoords[2];
  vertices[7].normal = normal;

  vertices[8].position = positions[3];
  vertices[8].textCoords = textCoords[1];
  vertices[8].normal = normal;

  indices.push_back(6);
  indices.push_back(7);
  indices.push_back(8);

  //right face
  normal = glm::triangleNormal(positions[0], positions[3], positions[4]);
  vertices[9].position = positions[0];
  vertices[9].textCoords = textCoords[3];
  vertices[9].normal = normal;

  vertices[10].position = positions[3];
  vertices[10].textCoords = textCoords[1];
  vertices[10].normal = normal;

  vertices[11].position = positions[4];
  vertices[11].textCoords = textCoords[5];
  vertices[11].normal = normal;

  indices.push_back(9);
  indices.push_back(10);
  indices.push_back(11);


  //Base: square shape made of two triangles
  normal = glm::triangleNormal(positions[2], positions[3], positions[1]);
  vertices[12].position = positions[4];
  vertices[12].textCoords = textCoords[5];
  vertices[12].normal = normal;

  vertices[13].position = positions[2];
  vertices[13].textCoords = textCoords[6];
  vertices[13].normal = normal;

  vertices[14].position = positions[1];
  vertices[14].textCoords = textCoords[2];
  vertices[14].normal = normal;

  vertices[15].position = positions[3];
  vertices[15].textCoords = textCoords[1];
  vertices[15].normal = normal;

  indices.push_back(12);
  indices.push_back(14);
  indices.push_back(13);

  indices.push_back(12);
  indices.push_back(15);
  indices.push_back(14);

  pyramid.loadVerticesAndIndices(vertices, indices, true, true);

  return pyramid;
}

Mesh cone(int slices, int stacks, bool cap) {
  Mesh cone;
  vector<unsigned int> indices;
  vector<Vertex> vertices;

  const float deltaHeight = 1.0f / stacks;
  const float deltaAngle = TAU / slices;
  for (int i = 0; i <= stacks; ++i) {
    float angle = 0.0f;
    for (int j = 0; j < slices; ++j) {
      Vertex v;
      v.position.x = (1.0f - i * deltaHeight) * cos(angle);
      v.position.y = i * deltaHeight;
      v.position.z = (1.0f - i * deltaHeight) * sin(angle);
      v.normal = glm::normalize(vec3(v.position.x, cos(TAU / 8.0f), v.position.z));
      vertices.push_back(v);
      angle += deltaAngle;
      //Start to create the triangles from second iteration and so on
      if (j > 0 && i > 0) {
        //Create two triangle
        int a = (i - 1) * slices + (j - 1);
        int b = (i - 1) * slices + j;
        int c = i * slices + (j - 1);
        int d = i * slices + j;
        indices.push_back(c);
        indices.push_back(b);
        indices.push_back(a);
        if (i < stacks) {
          indices.push_back(c);
          indices.push_back(d);
          indices.push_back(b);
        }
      }
    }
    //Last two
    if (i > 0) {
      int a = (i - 1) * slices + (slices - 1);
      int b = (i - 1) * slices + 0;
      int c = i * slices + (slices - 1);
      int d = i * slices + 0;
      indices.push_back(c);
      indices.push_back(b);
      indices.push_back(a);

      indices.push_back(c);
      indices.push_back(d);
      indices.push_back(b);
    }
  }
/**
  That was the central parts of the cone. Now we need to close the shape by
  generating the base circle
*/
  if (cap) {
    int last_index = static_cast<int>(vertices.size());
    Vertex v;
    v.position = vec3(0.0f);
    v.normal = vec3(0.0f, -1.0f, 0.0f);
    vertices.push_back(v);
    //cap
    float angle = 0.0f;
    for (int i = 0; i < slices; i++) {
      Vertex u;
      u.position.x = cos(angle);
      u.position.y = 0;
      u.position.z = sin(angle);
      u.normal = vec3(0.0f, -1.0f, 0.0f);
      vertices.push_back(u);
      angle += deltaAngle;
    }
    //Remember that index start at 0
    for (int i = 1; i <= slices; i++) {
      indices.push_back(last_index);
      int tmp = i;
      indices.push_back(last_index + tmp);
      tmp = (i % slices) + 1;
      indices.push_back(last_index + tmp);
    }
  }

  cone.loadVerticesAndIndices(vertices, indices, true, false);

  return cone;
}

Mesh coneTexture(int slices, int stacks, bool cap) {
  Mesh cone;
  vector<unsigned int> indices;
  vector<Vertex> vertices;

  assert(slices >= 3);
  assert(stacks >= 1);

  Vertex v;

  const float deltaHeight = 1.0f / stacks;
  const float deltaPolar = TAU / slices;
  for (int i = 0; i < stacks; ++i) {
    float polarAngle = 0.0f;
    for (int j = 0; j < slices; ++j) {
      v.position.x = (1.0f - i * deltaHeight) * cos(polarAngle);
      v.position.y = i * deltaHeight;
      v.position.z = (1.0f - i * deltaHeight) * sin(polarAngle);
      v.normal = glm::normalize(vec3(v.position.x, cos(TAU / 8.0f), v.position.z));
      v.textCoords.s = polarAngle / TAU;
      v.textCoords.t = v.position.y;
      vertices.push_back(v);
      polarAngle += deltaPolar;
    }
    //Dummy vertex for the texture coordinates
    v.position = vertices[vertices.size() - slices].position;
    v.normal = vertices[vertices.size() - slices].normal;
    v.textCoords.s = 1.0f;
    v.textCoords.t = v.position.y;
    vertices.push_back(v);
  }
  //To store where the middle vertices start of the last row
  int indexLast = static_cast<int>(vertices.size() - slices - 1);
  //Create triangles for the center part
  for (int i = 0; i < (stacks - 1); ++i) {
    for (int j = 0; j < slices; ++j) {
      int a = i * (slices + 1) + j;
      int b = i * (slices + 1) + (j + 1) % (slices + 1);
      int c = a + (slices + 1);
      int d = b + (slices + 1);

      indices.push_back(a);
      indices.push_back(c);
      indices.push_back(b);

      indices.push_back(b);
      indices.push_back(c);
      indices.push_back(d);
    }
  }
  //Top of the cone
  v.position = vec3(0.0f, 1.0f, 0.0f);
  v.textCoords.t = 1.0f;
  v.normal = vec3(0.0f, 1.0f, 0.0f);
  //Triangles for the "Triangle fan" in the north
  for (int i = 0; i < slices; ++i) {
    int b = indexLast + i;
    int c = indexLast + (i + 1) % (slices + 1);
    //The new dummy vertex
    v.textCoords.s = (vertices[b].textCoords.s + vertices[c].textCoords.s) / 2.0f;
    vertices.push_back(v);
    //Create triangle using the new dummy vertex
    indices.push_back(static_cast<unsigned int>(vertices.size() - 1));
    indices.push_back(c);
    indices.push_back(b);
  }

  if (cap) {
    //Create down triangle fan
    v.position = vec3(0.0f, 0.0f, 0.0f);
    v.normal = vec3(0.0f, -1.0f, 0.0f);
    v.textCoords = vec2(0.5f, 0.5f);
    vertices.push_back(v);
    int indexCenter = static_cast<int>(vertices.size() - 1);
    //Create dummy vertices (share positions with other) in the circle
    for (int i = 0; i < slices; ++i) {
      int b = i;
      int c = (i + 1) % (slices + 1);

      v.position = vertices[c].position;
      v.textCoords.s = 0.5f * v.position.x + 0.5f;
      v.textCoords.t = 0.5f * v.position.z + 0.5f;
      vertices.push_back(v);

      v.position = vertices[b].position;
      v.textCoords.s = 0.5f * v.position.x + 0.5f;
      v.textCoords.t = 0.5f * v.position.z + 0.5f;
      vertices.push_back(v);

      indices.push_back(indexCenter);
      indices.push_back(static_cast<int>(vertices.size() - 1));
      indices.push_back(static_cast<int>(vertices.size() - 2));
    }
  }

  cone.loadVerticesAndIndices(vertices, indices, true, true);

  return cone;
}


//Evaluate the "superformula" i.e. Formula for supershapes
//Remember that n.z != 0.0f, a != 0.0f, b != 0.0f
float superformula(float angle, float a, float b, float m, vec3 n) {
  return glm::pow(
    glm::pow(glm::abs(glm::cos(m * angle / 4.0f) / a), n.y) +
    glm::pow(glm::abs(glm::sin(m * angle / 4.0f) / b), n.z),
    -1.0f / n.x);
}

Mesh superShape(float a, float b, float m, glm::vec3 n, int discretization) {
  Mesh supershape;
  vector<unsigned int> indices;
  vector<Vertex> vertices;

  //Calculate the positions
  for (int i = 0; i < discretization; ++i) {
    for (int j = 0; j < discretization; ++j) {
      Vertex v;
      vec2 r;
      float theta = j * TAU / (discretization - 1) - TAU / 2.0f;
      float phi   = i * PI  / (discretization - 1) - PI  / 2.0f;
      r.x = superformula(theta, a, b, m, n);
      r.y = superformula(phi, a, b, m, n);
      v.position.x = r.x * cos(theta) * r.y * cos(phi);
      v.position.y = r.x * sin(theta) * r.y * cos(phi);
      v.position.z = r.y * sin(phi);
      v.textCoords = vec2(float(i) / (discretization - 1), float(j) / (discretization - 1));
      vertices.push_back(v);
    }
  }

  //Calculate normals
  for (int i = 0; i < discretization; ++i) {
    for (int j = 0; j < discretization; ++j) {
      int a = i * discretization + j;
      int b = i * discretization + ((j + 1) % discretization);
      int c = ((i + 1) % discretization) * discretization + j;
      int d = ((i + 1) % discretization) * discretization + ((j + 1) % discretization);

      vec3 du = vertices[b].position - vertices[a].position;
      vec3 dv = vertices[c].position - vertices[a].position;

      vertices[a].normal = glm::normalize(glm::cross(du, dv));
      //First triangle
      indices.push_back(a);
      indices.push_back(d);
      indices.push_back(c);
      //Second triangle
      indices.push_back(a);
      indices.push_back(b);
      indices.push_back(d);
    }
  }

  supershape.loadVerticesAndIndices(vertices, indices, true, true);

  return supershape;
}


//Temporal container for triangles before mesh indexin
std::vector<Triangle> triangles;

void subdivide_face(vec3 v0, vec3 v1, vec3 v2, const size_t& level) {
  if (level > 0) {
    //Calculate midpoints and project them to sphere
    vec3 new_01 = glm::normalize(0.5f * (v0 + v1));
    vec3 new_12 = glm::normalize(0.5f * (v1 + v2));
    vec3 new_20 = glm::normalize(0.5f * (v2 + v0));
    //Subdivide again recursively using the new vertex
    subdivide_face(v0, new_01, new_20, level - 1);
    subdivide_face(new_01, new_12, new_20, level - 1);
    subdivide_face(new_01, v1, new_12, level - 1);
    subdivide_face(new_20, new_12, v2, level - 1);
  } else {
    //We reach the bottom of the recursion, we need to generate a triangle
    Triangle triangle;
    triangle.p0 = v0;
    triangle.p1 = v1;
    triangle.p2 = v2;
    triangles.push_back(triangle);
  }
}

Mesh icosphere(int subdiv) {
  Mesh sphere;

  //Start creating the 12 original vertices
  std::vector<vec3> initial_vertices(12);
  //Spherical coordinates
  float phi = 0.0f; //Between [0, and Pi]
  float psy = 0.0f; //Between [0, and Tau]
  const float radio = 1.0f;

  //North pole
  initial_vertices[0] = radio * vec3(sin(phi) * cos(psy), sin(phi) * sin(psy), cos(phi));
  //Create five vertex below the north pole at TAU/5 gaps
  phi = PI / 3.0f;;
  for (int i = 1; i <= 5; ++i) {
    initial_vertices[i] = radio * vec3(sin(phi) * cos(psy), sin(phi) * sin(psy), cos(phi));
    psy += TAU / 5.0f;
  }
  //Create another five vertex below the first strip. At TAU/GAP and a TAU/10 offset
  psy = TAU / 10.0f;
  phi = PI - (PI / 3.0f);
  for (int i = 1; i <= 5; ++i) {
    initial_vertices[i + 5] = radio * vec3(sin(phi) * cos(psy), sin(phi) * sin(psy), cos(phi));
    psy += TAU / 5.0f;
  }
  //South pole
  phi = PI;
  psy = 0.0f;
  initial_vertices[11] = radio * vec3(sin(phi) * cos(psy), sin(phi) * sin(psy), cos(phi));

  int subdiv_level = glm::abs(subdiv);
  triangles.clear();
  //Generate the initial 20 faces
  /************************************************************************/
  /* Connect the north pole to the first strip, a triangle fan            */
  /************************************************************************/
  subdivide_face(initial_vertices[0], initial_vertices[1], initial_vertices[2], subdiv_level);
  subdivide_face(initial_vertices[0], initial_vertices[2], initial_vertices[3], subdiv_level);
  subdivide_face(initial_vertices[0], initial_vertices[3], initial_vertices[4], subdiv_level);
  subdivide_face(initial_vertices[0], initial_vertices[4], initial_vertices[5], subdiv_level);
  subdivide_face(initial_vertices[0], initial_vertices[5], initial_vertices[1], subdiv_level);
  /************************************************************************/
  /* Connect the two mid rows of vertex in a triangle strip fashion       */
  /************************************************************************/
  subdivide_face(initial_vertices[1], initial_vertices[6], initial_vertices[2], subdiv_level);
  subdivide_face(initial_vertices[2], initial_vertices[6], initial_vertices[7], subdiv_level);
  subdivide_face(initial_vertices[2], initial_vertices[7], initial_vertices[3], subdiv_level);
  subdivide_face(initial_vertices[3], initial_vertices[7], initial_vertices[8], subdiv_level);
  subdivide_face(initial_vertices[3], initial_vertices[8], initial_vertices[4], subdiv_level);
  subdivide_face(initial_vertices[4], initial_vertices[8], initial_vertices[9], subdiv_level);
  subdivide_face(initial_vertices[4], initial_vertices[9], initial_vertices[5], subdiv_level);
  subdivide_face(initial_vertices[5], initial_vertices[9], initial_vertices[10], subdiv_level);
  subdivide_face(initial_vertices[5], initial_vertices[10], initial_vertices[1], subdiv_level);
  subdivide_face(initial_vertices[1], initial_vertices[10], initial_vertices[6], subdiv_level);
  /************************************************************************/
  /* Connect the south pole to the second strip, a triangle fan           */
  /************************************************************************/
  subdivide_face(initial_vertices[6], initial_vertices[11], initial_vertices[7], subdiv_level);
  subdivide_face(initial_vertices[7], initial_vertices[11], initial_vertices[8], subdiv_level);
  subdivide_face(initial_vertices[8], initial_vertices[11], initial_vertices[9], subdiv_level);
  subdivide_face(initial_vertices[9], initial_vertices[11], initial_vertices[10], subdiv_level);
  subdivide_face(initial_vertices[10], initial_vertices[11], initial_vertices[6], subdiv_level);

  sphere.loadFromTriangles(triangles);

  //Get the vectors back to add normal and text coords
  triangles.clear();

  return sphere;
}

Mesh tethrahedra() {
  Mesh teth;
  vector<Vertex> vertices;
  vector<unsigned int> indices;

  vec3 pos[4];
  pos[0] = vec3(0.0f, 0.0f, 1.0f);
  pos[1] = vec3(0.0f, (2.0f / 3.0f) * glm::sqrt(2.0f), -1.0f / 3.0f);
  pos[2] = vec3(-glm::sqrt(2.0f / 3.0f), -glm::sqrt(2.0f) / 3.0f, -1.0f / 3.0f);
  pos[3] = vec3(glm::sqrt(2.0f / 3.0f), -glm::sqrt(2.0f) / 3.0f, -1.0f / 3.0f);

  vec2 text[6];
  text[0] = vec2(0.0f, 0.0f);
  text[1] = vec2(0.5f, 0.0f);
  text[2] = vec2(1.0f, 0.0f);
  text[3] = vec2(0.25f, 0.5f);
  text[4] = vec2(0.75f, 0.5f);
  text[5] = vec2(0.5f, 1.0f);

  vec3 normal = glm::normalize(glm::cross(pos[3] - pos[0], pos[1] - pos[0]));

  Vertex v;
  v.position = pos[0];
  v.textCoords = text[2];
  v.normal = normal;
  vertices.push_back(v); //0

  v.position = pos[3];
  v.textCoords = text[4];
  v.normal = normal;
  vertices.push_back(v); //1

  v.position = pos[1];
  v.textCoords = text[1];
  v.normal = normal;
  vertices.push_back(v); //2

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);

  //Trinagle 2
  normal = glm::normalize(glm::cross(pos[1] - pos[0], pos[2] - pos[0]));
  v.position = pos[0];
  v.textCoords = text[0];
  v.normal = normal;
  vertices.push_back(v); //3

  v.position = pos[1];
  v.textCoords = text[1];
  v.normal = normal;
  vertices.push_back(v); //4

  v.position = pos[2];
  v.textCoords = text[3];
  v.normal = normal;
  vertices.push_back(v); //5

  indices.push_back(3);
  indices.push_back(4);
  indices.push_back(5);

  //Trinagle 3
  normal = glm::normalize(glm::cross(pos[2] - pos[0], pos[3] - pos[0]));
  v.position = pos[0];
  v.textCoords = text[5];
  v.normal = normal;
  vertices.push_back(v); //6

  v.position = pos[2];
  v.textCoords = text[3];
  v.normal = normal;
  vertices.push_back(v); //7

  v.position = pos[3];
  v.textCoords = text[4];
  v.normal = normal;
  vertices.push_back(v); //8

  indices.push_back(6);
  indices.push_back(7);
  indices.push_back(8);

  //Trinagle 4
  normal = glm::normalize(glm::cross(pos[3] - pos[1], pos[2] - pos[1]));
  v.position = pos[1];
  v.textCoords = text[1];
  v.normal = normal;
  vertices.push_back(v); //9

  v.position = pos[3];
  v.textCoords = text[4];
  v.normal = normal;
  vertices.push_back(v); //10

  v.position = pos[2];
  v.textCoords = text[3];
  v.normal = normal;
  vertices.push_back(v); //11

  indices.push_back(9);
  indices.push_back(10);
  indices.push_back(11);

  teth.loadVerticesAndIndices(vertices, indices, true, true);

  return teth;
}

Mesh plane() {

  Mesh plane;

  Vertex v;
  vector<Vertex> vertices;
  v.normal = vec3(0.0f, 0.0f, 1.0f);

  v.position = vec3(-0.5f, 0.5f, 0.0f);
  v.textCoords = vec2(0.0, 0.0);
  vertices.push_back(v);

  v.position = vec3(-0.5f, -0.5f, 0.0f);
  v.textCoords = vec2(0.0, 1.0);
  vertices.push_back(v);

  v.position = vec3(0.5f, -0.5f, 0.0f);
  v.textCoords = vec2(1.0, 1.0);
  vertices.push_back(v);

  v.position = vec3(0.5f, 0.5f, 0.0f);
  v.textCoords = vec2(1.0, 0.0);
  vertices.push_back(v);

  vector<unsigned int> indices;

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);

  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(3);

  plane.loadVerticesAndIndices(vertices, indices, true, true);

  return plane;
}

} // namespace mesh