#include "mesh.h"

#include <iostream>
#include <set>

namespace mesh {

using glm::vec3;
using glm::vec4;
using glm::mat4;

Mesh::Mesh() : mHasNormals(false), mHasTexture(false) {
  mLowerCorner = mUpperCorner = vec3(0.0f);
}

Mesh::Mesh(const std::string& fileName) : Mesh(){
  loadFromFile(fileName);
}

Mesh::~Mesh() {

}

std::vector<unsigned int> Mesh::getIndices() const {
  return mIndices;
}

std::vector<Vertex> Mesh::getVertices() const {
  return mVertices;
}

bool Mesh::loadFromFile(const std::string& fileName) {
  using std::cerr;
  using std::endl;
  // Create an instance of the Importer class
  Assimp::Importer importer;
  const aiScene* scenePtr = importer.ReadFile(fileName,
      aiProcess_CalcTangentSpace       |
      aiProcess_Triangulate            |
      aiProcess_JoinIdenticalVertices  |
      aiProcess_SortByPType);
  /* If the import failed, report it (I am guessing that he will be able
  to check if the file exist and if its writable itself. */
  if( !scenePtr) {
    cerr << importer.GetErrorString() << endl;
    return false;
  }
  // See if the file contains a mesh and if it has at least positions
  const aiMesh* mesh = scenePtr->mMeshes[0];
  if (!mesh->HasPositions()) {
    cerr << "Weird mesh without vertex positions!" << endl;
    return false;
  }
  // Parse Mesh data
  /* First the indices */
  unsigned int numFaces = mesh->mNumFaces;
  for (unsigned int t = 0; t < numFaces; ++t) {
    const aiFace* face = &mesh->mFaces[t];
    for (unsigned int i = 0; i < face->mNumIndices; ++i) {
      mIndices.push_back(face->mIndices[i]);
    }
  }
  /* Now, the Vertices */
  Vertex v;
  mHasNormals = mesh->HasNormals();
  mHasTexture = mesh->HasTextureCoords(0);
  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    v.position.x = mesh->mVertices[i].x;
    v.position.y = mesh->mVertices[i].y;
    v.position.z = mesh->mVertices[i].z;
    if (mHasNormals) {
      v.normal.x = mesh->mNormals[i].x;
      v.normal.y = mesh->mNormals[i].y;
      v.normal.z = mesh->mNormals[i].z;
    }
    if (mHasTexture) {
      v.textCoords.s = mesh->mTextureCoords[0][i].x;
      v.textCoords.t = mesh->mTextureCoords[0][i].y;
    }
    mVertices.push_back(v);
  }
  // See if the mesh uses a diffuse texture
  if (mesh->mMaterialIndex > 0) {
    aiMaterial* material = scenePtr->mMaterials[mesh->mMaterialIndex];
    addDiffuseTexture(material);
  }
  // Update our bb
  updateBoundingBox();
  return true;
}

bool Mesh::loadVerticesAndIndices(const std::vector<Vertex>& vertices, 
    const std::vector<unsigned int>& indices, bool normals, bool textCoords) {
  // Blindlly trust the supplied parameters
  if (vertices.empty()) {
    return false;
  } else {
    mVertices = vertices;
  }

  if (indices.empty()) {
    return false;
  } else {
    mIndices = indices;
  }

  mHasNormals = normals;
  mHasTexture = textCoords;

  updateBoundingBox();
  return true;
}

bool Mesh::loadFromTriangles(const std::vector<Triangle>& triangles) {

  auto lessThan = [](const vec3& a, const vec3& b){
    //Two vertex positions that are less than square root of EPSILON
    //apart are consider the same vertex
    const float EPSILON = 0.00000001f;  // sqrt(EPSILON) = 0.0001
    //check equality
    if (glm::length2(a - b) < EPSILON) {
      return false;
    }
    //Lexicographical order
    if (glm::abs(a.x - b.x) > EPSILON) {
      return a.x < b.x;
    } else if (glm::abs(a.y - b.y) > EPSILON) {
      return a.y < b.y;
    } else {
      return a.z < b.z;
    }
  };
  //This set will be used to get rid of the duplicate vertex
  std::set<vec3, decltype(lessThan)> tmpStorage(lessThan);
  //Clear the previous data in the indices and points arrays, since we are about to
  //start a new indexing
  mIndices.clear();
  mVertices.clear();
  //Insert all the vertex in the tmp_storage
  for (auto t : triangles) {
    tmpStorage.insert(t.p0);
    tmpStorage.insert(t.p1);
    tmpStorage.insert(t.p2);
  }
  //Insert indexes for the vertices
  for (auto t : triangles) {
    // First vertex of triangle
    auto it = tmpStorage.find(t.p0);
    auto index = std::distance(tmpStorage.begin(), it);
    mIndices.push_back(static_cast<unsigned int>(index));
    // Second
    it = tmpStorage.find(t.p1);
    index = std::distance(tmpStorage.begin(), it);
    mIndices.push_back(static_cast<unsigned int>(index));
    // Third
    it = tmpStorage.find(t.p2);
    index = std::distance(tmpStorage.begin(), it);
    mIndices.push_back(static_cast<unsigned int>(index));
  }
  //Create the Vertex buffer storage
  Vertex tmpVertex;
  for (auto position : tmpStorage) {
    tmpVertex.position = position;
    mVertices.push_back(tmpVertex);
  }
  //Since we created the mesh from triangles, we do no have normal nor texture coordinates
  mHasNormals = mHasTexture = false;
  updateBoundingBox();
  return true;
}

bool Mesh::empthy() const {
  return mVertices.empty();
}

bool Mesh::hasTexture() const {
  return mHasTexture;
}

bool Mesh::hasNormals() const {
  return mHasNormals;
}

void Mesh::clear() {
  mVertices.clear();
  mIndices.clear();
  mHasNormals = mHasTexture = false;
  mLowerCorner = mUpperCorner = vec3(0.0f);
}

void Mesh::transform(const mat4& T) {
  // Apply troansformation to the vertices
  for (auto& v : mVertices) {
    v.position = vec3(T * vec4(v.position, 1.0f));
  }
  // If you have normals, then you need to trasmform them as well
  if (mHasNormals) {
    mat4 normalMat = glm::inverse(glm::transpose(T)); // Get rid of scaling
    for (auto& v : mVertices) {
        v.normal = vec3(normalMat * vec4(v.normal, 0.0f));
    }
  }
  // We just moved the vertex we need to recalculate our bb
  updateBoundingBox();
}

void Mesh::toUnitCube() {
  float s = this->scaleFactor();
  vec3 c = this->getBBCenter();
  // Calculate the transform that will take us to the unit cube
  mat4 T(1.0f);
  T = glm::scale(T, vec3(s));
  T = glm::translate(T, -c);
  // Apply
  this->transform(T);
}

float Mesh::scaleFactor() const {
  vec3 size = this->getBBSize();
  // Inverse of the maximum lenght of the bb
  return 1.0f / (glm::max(size.x, glm::max(size.y, size.z)));
}

vec3 Mesh::getBBCenter() const {
  return 0.5f * (mUpperCorner + mLowerCorner);
}

vec3 Mesh::getBBSize() const {
  return mUpperCorner - mLowerCorner;
}

size_t Mesh::trianglesCount() const {
  return mIndices.size() / 3;
}

size_t Mesh::indicesCount() const {
  return mIndices.size();
}

size_t Mesh::vertexCount() const {
  return mVertices.size();
}

bool Mesh::save(const std::string& fileName) const {
  //Create a scene
  aiScene* scene = new aiScene();
  scene->mRootNode = new aiNode();
  //Assimp requires a material to work. I create an empthy one
  scene->mMaterials = new aiMaterial* [1];
  scene->mMaterials[0] = nullptr;
  scene->mNumMaterials = 1;
  scene->mMaterials[0] = new aiMaterial();
  //Now create the mesh
  scene->mMeshes = new aiMesh* [1];
  scene->mMeshes[0] = nullptr;
  scene->mNumMeshes = 1;
  //Now allocate the mesh and regiter the material
  scene->mMeshes[0] = new aiMesh();
  scene->mMeshes[0]->mMaterialIndex = 0;
  //Register the mesh in the scene
  scene->mRootNode->mMeshes = new unsigned int[1];
  scene->mRootNode->mMeshes[0] = 0;
  scene->mRootNode->mNumMeshes = 1;
  //Get handle
  auto meshPtr = scene->mMeshes[0];
  //Allocate space for vertex data
  meshPtr->mVertices = new aiVector3D[mVertices.size()];
  // If you have normal allocate space for them too
  if (mHasNormals) {
    meshPtr->mNormals = new aiVector3D[mVertices.size()];
    meshPtr->mNumVertices = static_cast<unsigned int>(mVertices.size());
  }
  // If you have texture coordinates allocate space for them too
  if (mHasTexture) {
    meshPtr->mTextureCoords[0] = new aiVector3D[mVertices.size()];
    meshPtr->mNumUVComponents[0] = static_cast<unsigned int>(mVertices.size());
  }
  //Fill vertex data
  Vertex v;
  for (size_t i = 0; i < mVertices.size(); ++i) {
    v = mVertices[i];
    meshPtr->mVertices[i] = aiVector3D(v.position.x, v.position.y, v.position.z);
    
    if (mHasNormals) {
      meshPtr->mNormals[i] = aiVector3D(v.normal.x, v.normal.y, v.normal.z);
    }

    if (mHasTexture) {
      meshPtr->mTextureCoords[0][i] = aiVector3D(v.textCoords.s, v.textCoords.t, 0);
    }
  }
  //Alocate space for face data (trinagular faces)
  meshPtr->mFaces = new aiFace[mIndices.size() / 3];
  meshPtr->mNumFaces = static_cast<unsigned int>(mIndices.size() / 3);
  //Fil face data
  for (size_t i = 0; i < mIndices.size(); i += 3) {
    aiFace& face = meshPtr->mFaces[i / 3];
    face.mIndices = new unsigned int[3];
    face.mNumIndices = 3;
    face.mIndices[0] = mIndices[i];
    face.mIndices[1] = mIndices[i + 1];
    face.mIndices[2] = mIndices[i + 2];
  }
  // Data structure is complete, now use importer and save it to disk
  if (scene) {
    Assimp::Exporter exporter;
    exporter.Export(scene, "obj", fileName.c_str());
    delete scene;
  }

  return true;
}

void Mesh::updateBoundingBox() {
  // Numeric limit boundaries, minimun ans maximum float point values
  mLowerCorner = FLT_MAX * vec3(1.0f);
  mUpperCorner = -FLT_MAX * vec3(1.0f);
  // Loop trought all vertex
  for (auto v : mVertices) {
    //Check if this vertex changes the bounding box
    mUpperCorner = glm::max(v.position, mUpperCorner);
    mLowerCorner = glm::min(v.position, mLowerCorner);
  }
}

void Mesh::addDiffuseTexture(const aiMaterial* mat) {
  // This mesh does not have material
  if (!mat) {
    return;
  }
  // Try to get a diffuse texture file from this material
  if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
    aiString fileName;
    mat->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
    mDiffuseText = std::string(fileName.C_Str()); //Store the filename
  } else {
    mDiffuseText = std::string(""); //Material did not contain diffuse texture
  }
  
  return;
}

} //namespaces mesh