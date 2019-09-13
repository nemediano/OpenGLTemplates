#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include <GL/glew.h>

#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

namespace mesh {

//!  A simple struct that act as a separator of the Meshes in this model.
/*!
  This struct encapsulate all the data needed to draw an individual Mesh
  of a model. If a model is compose of n meshes, it will also contain a
  vector of n MeshData elements where each one can be used as a helper
  to render an specific Mesh.
*/
typedef struct MeshData {
  //! The place of the first vertex of this mesh.
  GLint startVertex;
  //! The place of the first index of this mesh.
  GLint startIndex;
  //! Number of indexes in this mesh.
  /*! The number of triangles is this number
      divided by three.
  */
  GLsizei howMany;
  //! Index of the diffuse texture
  /*! The index of this mesh difusse texture in the textures vector
   * or -1 if this mesh does not have a Diffuse texture
  */
  int diffuseIndex;
  //! Index of the specular texture
  /*! The index of this mesh specular texture in the textures vector
   * or -1 if this mesh does not have a Diffuse texture
  */
  int specIndex;
} MeshData;

enum TextType {DIFFUSE, SPECULAR, NORMALS, OTHER};

//! Holds the info for a particular texture
/*!
  This strcuture contains the filepath of a texture and
  the Type of texture (So you know how to use it for render)
*/
struct TextureImage {
  //! The path of this texture (relative to the model's mesh folder)
  std::string filePath;
  //! The type of this texture (as reported by the model's mesh file)
  TextType type;
  //! Helper to compare if two textures refer to the same file (very common practice)
  bool operator==(const TextureImage& lhs) {
      return this->filePath == lhs.filePath;
  }
};


//!  This class loads a 3D model that are composed of more than one mesh.
/*!
  This is an specialized version of the Mesh class used to load a 3D model
  that is composed by more than one Mesh.
  This class only loads model from the file to main memmory in CPU. Therefore
  1) Is decouppled of any rendering code.
  2) It does not use the GPU at all.
  Saying the above, it is designed to provide you with the needed interface
  for a client that will use this data to render it using OpenGL.
  All the meshes are combined into a single Vertex array and a single indices
  array. Therefore this class can also providea vector of separators.

  If the file has a model with only one Mesh this class also works. However,
  in this case, is better to use the Mesh base class.
*/
class Model : public Mesh {

protected:
  std::vector<TextureImage> mTexturesData;
  void processNode(aiNode* node, const aiScene* scene);
  void addMeshData(const aiMesh* mesh, const aiScene* scene);
  std::vector<MeshData> mSeparators;
  int addTexture(const aiMaterial* material, aiTextureType ai_type);
  TextType toTextType(aiTextureType ai_type);

public:
  //! Simple constructor that only initialices the data struct.
  Model();
  //! Loads this 3D model from the fileName
  explicit Model(const std::string& fileName);
  //! Clears the current data. Then loads this 3D model from the fileName
  bool load(const std::string& fileName);
  //! Add a mesh to this model
  /*!
    Add the data from the mesh (indices and vertices) to the internal
    data structure model. It also updates the separator and the normals
    and texture coordinates flags
  */
  void addMesh(const Mesh& mesh);
  //! Get a vector of MeshData that act as a separator of the meshes.
  /*!
    Get a vector of MeshData, since all the model data is contained in a
    single buffer. This vector act as a series of separators of each mesh.
    They provide all the data needed to render each mesh using
    glDrawElementsBaseVertex.
  */
  std::vector<MeshData> getSeparators() const;
  //! Get the filenames of each of the Diffuse textures in this model.
  /*!
    The mesh's texture indexes retrived from separators are always in reference
    of this vector. If the mesh does not contain texture an index of
    -1 in the separator indicate that you should not query this vector
  */
  std::vector<TextureImage> getDiffuseTextures() const;
  //! Get the number of meshes in this Model.
  int numMeshes() const;
};

} // namespace mesh

#endif // MODEL_H