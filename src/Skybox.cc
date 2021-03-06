#include "Geometry.h"
#include "Shader.h"
#include "Skybox.h"
#include "Util.h"

namespace puddi
{
  // PUBLIC

  void Skybox::Init()
  {
    createPrototypeVertices();
  }

  Skybox::Skybox(Object *par) : DrawableObject(par, VertexMesh::GetVertexMeshPrototypeByName("skybox"))
  {
    RotateX(static_cast<float>(-M_PI / 2.0f));
    RotateZ(static_cast<float>(M_PI));
    AddIgnoreParentModelFlag(IGNORE_PARENT_ROTATIONX | IGNORE_PARENT_ROTATIONY | IGNORE_PARENT_ROTATIONZ);
  }

  //  void Skybox::Draw() const
  //  {
  //      DrawableObject::Draw();
  //      glDrawArrays(GL_TRIANGLES, indexOffset, indexCount);
  //  }

  // PRIVATE

  void Skybox::createPrototypeVertices()
  {
    std::vector<vec4> cube = Geometry::CreateCube(1.0);
    std::vector<vec4> normals = Geometry::CreateCubeNormals();
    // negate the normals
    Util::Apply(normals, [](const vec4& v) -> vec4
			 {
			   return -v;
			 });
    std::vector<vec4> tangents = Geometry::CreateCubeTangents();
    std::vector<vec4> binormals = Geometry::CreateCubeBiNormals();
    std::vector<vec2> texCoords = Geometry::CreateSkyboxTextureCoordinates();
    std::vector<vec4> empty = std::vector<vec4>(cube.size(), vec4(-1, -1, -1, -1));

    int vertexOffset = Shader::Vertices.size();
    int indexOffset = Shader::VertexIndices.size();
    int indexCount = cube.size();

    std::vector<uint> indices;
    for (int i = 0; i < indexCount; ++i)
      indices.push_back(vertexOffset + i);

    Shader::Vertices.insert(Shader::Vertices.end(), cube.begin(), cube.end());
    Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
    Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
    Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
    Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
    Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());
    Shader::BoneIndices.insert(Shader::BoneIndices.end(), empty.begin(), empty.end());
    Shader::BoneWeights.insert(Shader::BoneWeights.end(), empty.begin(), empty.end());

    VertexMesh::AddVertexMeshPrototype("skybox",
				       Material::Plastic(vec4(0.0, 0.0, 0.0, 1.0)),
				       indexOffset, indexCount, V_TRIANGLES);
  }
}
