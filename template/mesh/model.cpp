#include <iostream>

#include "model.h"

namespace mesh {

Model::Model() : Mesh() {

}

Model::Model(const std::string& fileName) : Mesh(){
  load(fileName);
}

bool Model::load(const std::string& fileName) {
  // Create an instance of the Importer class
  Assimp::Importer importer;
  // Loads the mesh data and metadata into memmory
  const aiScene* scenePtr = importer.ReadFile( fileName,
      aiProcess_GenNormals       |
      aiProcess_Triangulate            |
      aiProcess_JoinIdenticalVertices);

  /* If the importer failed, report it (I am guessing that he will be able
   to check if the file exist and if it's writable) as it is. */
  if(!scenePtr || scenePtr->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scenePtr->mRootNode) {
    std::cerr << importer.GetErrorString() << std::endl;
    return false;
  }
  // Clear our data buffers
  mIndices.clear();
  mVertices.clear();
  mSeparators.clear();
  //Start the recursivelly process at the root
  processNode(scenePtr->mRootNode, scenePtr);
  updateBoundingBox();

  return true;
}

std::vector<MeshData> Model::getSeparators() const {
  return mSeparators;
}

int Model::numMeshes() const {
  //We have a separator at the begining and at the end
  return static_cast<int>(mSeparators.size() - 1);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  // Process all the meshes (if any) in this node
  for(unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    addMeshData(mesh, scene);
  }
  // Then, recursivelly procees the child nodes
  for(unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

void Model::addMesh(const Mesh& mesh) {
  MeshData bookMark; // New bookmar to keep track of the new mesh
  // keep the current number of indices (before adding this mesh)
  unsigned int indicesBefore = static_cast<unsigned int>(mIndices.size());
  // Insert new indices
  std::vector<unsigned int> newIndices = mesh.getIndices();
  mIndices.insert(mIndices.end(), newIndices.begin(), newIndices.end());
  // Update the bookmark to reflect the new indices
  unsigned int indicesAfter = static_cast<unsigned int>(mIndices.size());
  bookMark.startIndex = int(indicesBefore);
  bookMark.howMany = int(indicesAfter - indicesBefore);
  // Prepare for the new vertices
  bookMark.startVertex = int(mVertices.size());
  // Insert new vertices
  std::vector<Vertex> newVertices = mesh.getVertices();
  mVertices.insert(mVertices.end(), newVertices.begin(), newVertices.end());
  // Update our internal flags
  mHasNormals = mHasNormals && mesh.hasNormals();
  mHasTexture = mHasTexture && mesh.hasTexture();
  // Finalize to update the bookmark
  bookMark.diffuseIndex = -1;
  bookMark.specIndex = -1;
  mSeparators.push_back(bookMark);
}

void Model::addMeshData(const aiMesh* mesh, const aiScene* scene) {
  // Parse Mesh data
  if (!mesh || !mesh->HasPositions() || !scene) {
    std::cerr << "Weird mesh without vertex positions!" << std::endl;
    return;
  }

  MeshData bookMark;
  /* First the indices */
  unsigned int numFaces = mesh->mNumFaces;
  // keep the current number of indices (before adding this mesh)
  unsigned int indicesBefore = static_cast<unsigned int>(mIndices.size());
  for (unsigned int t = 0; t < numFaces; ++t) {
    const aiFace* face = &mesh->mFaces[t];
    for (unsigned int i = 0; i < face->mNumIndices; ++i) {
      mIndices.push_back(face->mIndices[i]);
    }
  }
  // Start filling this mesh's separators info
  unsigned int indicesAfter = static_cast<unsigned int>(mIndices.size());
  bookMark.startIndex = int(indicesBefore);
  bookMark.howMany = int(indicesAfter - indicesBefore);

  /* Now, the Vertices */
  Vertex v;
  mHasNormals = mesh->HasNormals();
  mHasTexture = mesh->HasTextureCoords(0);
  bookMark.startVertex = int(mVertices.size());
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

  // Add textures for this mesh to our collection
  int diffuseTexture = -1;
  int specularTexture = -1;
  if (mesh->mMaterialIndex > 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    diffuseTexture = addTexture(material, aiTextureType_DIFFUSE);
    specularTexture = addTexture(material, aiTextureType_SPECULAR);
  }
  // Finish filling the separator for this mesh
  bookMark.diffuseIndex = diffuseTexture;
  bookMark.specIndex = specularTexture;
  mSeparators.push_back(bookMark);
}

int Model::addTexture(const aiMaterial* mat, aiTextureType ai_type) {
  if (!mat) {
      return -1;
  }
  // Try to get a texture name of this type in this material
  std::string textPath = "";
  if (mat->GetTextureCount(ai_type) > 0) {
    aiString fileName;
    mat->GetTexture(ai_type, 0, &fileName);
    textPath = std::string(fileName.C_Str());
  } else {
    //NO texture of this type for this Mesh
    return -1;
  }
  //Check if this texture is already in the vector
  for (size_t i = 0; i < mTexturesData.size(); ++i) {
    if (mTexturesData[i].filePath == textPath) {
      return int(i); // If it is, return the index
    }
  }
  //It's is not, then create it an push it into the vector
  TextureImage text;
  text.type = toTextType(ai_type);
  text.filePath = textPath;
  mTexturesData.push_back(text);
  return static_cast<int>(mTexturesData.size() - 1);

}


// Helper function to map between our texture types and those used by Assimp
TextType Model::toTextType(aiTextureType ai_type) {

  TextType type = OTHER;

  switch (ai_type) {

    case aiTextureType_DIFFUSE:
      type = DIFFUSE;
    break;

    case aiTextureType_SPECULAR:
      type = SPECULAR;
    break;

    case aiTextureType_NORMALS:
      type = NORMALS;
    break;

    default:
      type = OTHER;
  }

  return type;
}

std::vector<TextureImage> Model::getDiffuseTextures() const {
    return mTexturesData;
}

} // namespace mesh