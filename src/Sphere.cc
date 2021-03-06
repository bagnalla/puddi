#include "Geometry.h"
#include "Shader.h"
#include "Sphere.h"
#include "Util.h"

using namespace std;

namespace puddi
{
  // PUBLIC

  void Sphere::Init()
  {
    createPrototypeVertices();
  }

  Sphere::Sphere(Object *par)
    : DrawableObject(par,
		     VertexMesh::GetVertexMeshPrototypeByName("sphere"))
  {}

  //void Sphere::Draw() const
  //{
  //  DrawableObject::Draw();
  //  glDrawArrays(GL_TRIANGLE_STRIP, globalVertexOffset, vertexCount);
  //}

  // PRIVATE

  vector<vec4> Sphere::sphere_vertices;
  vector<vec4> Sphere::GetSphereVertices()
  {
    return Sphere::sphere_vertices;
  }

  vector<vec4> Sphere::sphere_normals;
  vector<vec4> Sphere::GetSphereNormals()
  {
    return Sphere::sphere_normals;
  }

  void Sphere::createPrototypeVertices()
  {
    std::vector<vec4> tangents;
    std::vector<vec4> binormals;
    sphere_vertices = Geometry::CreateSphere(15.0f, tangents, binormals);
    auto sphere = sphere_vertices;
    
    std::vector<vec4> normals = Geometry::CreateSphereNormals(sphere);
    std::vector<vec4> empty = std::vector<vec4>(sphere.size(), vec4(-1, -1, -1, -1));

    // sphere texture coordinates
    std::vector<vec2> texCoords;
    for (auto it = normals.begin(); it != normals.end(); ++it)
    {
      auto n = *it;
      double u = asin(n.x) / M_PI + 0.5f;
      double v = asin(n.y) / M_PI + 0.5f;
      texCoords.push_back(vec2(u, v));
    }

    int vertexOffset = Shader::Vertices.size();
    int indexOffset = Shader::VertexIndices.size();
    int indexCount = sphere.size();

    std::vector<uint> indices;
    for (int i = 0; i < indexCount; ++i)
      indices.push_back(vertexOffset + i);

    Shader::Vertices.insert(Shader::Vertices.end(), sphere.begin(), sphere.end());
    Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
    Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
    Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
    Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
    Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());
    Shader::BoneIndices.insert(Shader::BoneIndices.end(), empty.begin(), empty.end());
    Shader::BoneWeights.insert(Shader::BoneWeights.end(), empty.begin(), empty.end());

    VertexMesh::AddVertexMeshPrototype("sphere", Material::Plastic(vec4(0.0, 0.0, 0.0, 1.0)),
				       indexOffset, indexCount, V_TRIANGLE_STRIP);
  }
}
