#include "Geometry.h"
#include "Rectangle.h"
#include "Shader.h"
#include "Util.h"

namespace puddi
{
  // PUBLIC

  void Rectangle::Init()
  {
    createPrototypeVertices();
  }

  Rectangle::Rectangle(Object *par) :
    DrawableObject(par, VertexMesh::GetVertexMeshPrototypeByName("rectangle"))
  {}

  //void Rectangle::Draw() const
  //{
  //DrawableObject::Draw();
  //glDrawArrays(GL_TRIANGLE_STRIP, indexOffset, indexCount);
  //}

  // PRIVATE

  void Rectangle::createPrototypeVertices()
  {
    std::vector<vec4> rect = Geometry::CreateRectangle(1.0);
    std::vector<vec4> normals({ vec4(0.0, 0.0, 1.0, 0.0),
				vec4(0.0, 0.0, 1.0, 0.0),
				vec4(0.0, 0.0, 1.0, 0.0),
				vec4(0.0, 0.0, 1.0, 0.0) });
    std::vector<vec4> tangents({ vec4(1.0, 0.0, 0.0, 0.0),
				 vec4(1.0, 0.0, 0.0, 0.0), 
				 vec4(1.0, 0.0, 0.0, 0.0),
				 vec4(1.0, 0.0, 0.0, 0.0) });
    std::vector<vec4> binormals({ vec4(0.0, 1.0, 0.0, 0.0),
				  vec4(0.0, 1.0, 0.0, 0.0),
				  vec4(0.0, 1.0, 0.0, 0.0),
				  vec4(0.0, 1.0, 0.0, 0.0) });
    std::vector<vec2> texCoords =
      Util::Vec4toVec2(Util::TransformVertices
		       (rect, translate(vec3(0.5f, 0.5f, 0.0f)) *
			glm::scale(vec3(1.0, 1.0, 1.0))));
    std::vector<vec4> empty = std::vector<vec4>(rect.size(),
						vec4(-1, -1, -1, -1));

    int vertexOffset = Shader::Vertices.size();
    int indexOffset = Shader::VertexIndices.size();
    int indexCount = rect.size();

    std::vector<uint> indices;
    for (int i = 0; i < indexCount; ++i)
      indices.push_back(vertexOffset + i);

    Shader::Vertices.insert(Shader::Vertices.end(), rect.begin(), rect.end());
    Shader::Normals.insert(Shader::Normals.end(),
			   normals.begin(), normals.end());
    Shader::Tangents.insert(Shader::Tangents.end(),
			    tangents.begin(), tangents.end());
    Shader::Binormals.insert(Shader::Binormals.end(),
			     binormals.begin(), binormals.end());
    Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(),
				      texCoords.begin(), texCoords.end());
    Shader::VertexIndices.insert(Shader::VertexIndices.end(),
				 indices.begin(), indices.end());
    Shader::BoneIndices.insert(Shader::BoneIndices.end(),
			       empty.begin(), empty.end());
    Shader::BoneWeights.insert(Shader::BoneWeights.end(),
			       empty.begin(), empty.end());

    VertexMesh::AddVertexMeshPrototype("rectangle",
				       Material::Plastic(vec4(0.0, 0.0, 0.0, 1.0)),
				       indexOffset, indexCount, V_TRIANGLE_STRIP);
  }
}
