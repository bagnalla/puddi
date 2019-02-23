#include "Geometry.h"
#include "Line.h"
#include "Shader.h"
#include "Util.h"

namespace puddi
{
  // PUBLIC

  void Line::Init()
  {
    Line::createPrototypeVertices();
  }

  Line::Line(Object *par) :
    DrawableObject(par, VertexMesh::GetVertexMeshPrototypeByName("line"))
  {}

  void Line::SetWorldPoints(const vec4 &p1, const vec4 &p2)
  {
    // this->SetPosition((p1 + p2) / 2.0f);
    this->SetPosition(p1);
    // this->SetScaleY(Util::Distance(p1, p2));
    this->SetScaleX(Util::Distance(p1, p2));
    this->LookAt(p2);
    // this->RotateZ(-M_PI / 2.0);
  }

  // PRIVATE

  void Line::createPrototypeVertices()
  {
    // std::vector<vec4> rect = Geometry::CreateRectangle(1.0);
    std::vector<vec4> line({ vec4(0.0, 0.0, 0.0, 1.0),
    			     vec4(1.0, 0.0, 0.0, 1.0) });
    std::vector<vec4> normals({ vec4(0.0, 0.0, 1.0, 0.0),
				vec4(0.0, 0.0, 1.0, 0.0) });
    std::vector<vec4> tangents({ vec4(1.0, 0.0, 0.0, 0.0),
				 vec4(1.0, 0.0, 0.0, 0.0) });
    std::vector<vec4> binormals({ vec4(0.0, 1.0, 0.0, 0.0),
				  vec4(0.0, 1.0, 0.0, 0.0) });
    std::vector<vec2> texCoords({ vec2(0.0, 0.0), vec2(0.0, 0.0) });
    std::vector<vec4> empty({ vec4(0, 0, 0, 0), vec4(0, 0, 0, 0) });

    int vertexOffset = Shader::Vertices.size();
    int indexOffset = Shader::VertexIndices.size();
    int indexCount = line.size();

    std::vector<uint> indices;
    for (int i = 0; i < indexCount; ++i)
      indices.push_back(vertexOffset + i);

    Shader::Vertices.insert(Shader::Vertices.end(), line.begin(), line.end());
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

    VertexMesh::AddVertexMeshPrototype("line",
				       Material::Plastic(vec4(0.0, 0.0, 0.0, 1.0)),
				       indexOffset, indexCount, V_LINES);
  }
}
