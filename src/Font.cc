#include "Font.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Geometry.h"
#include "Util.h"
#include "Shader.h"

using namespace std;

namespace puddi
{
	// PUBLIC

	void Font::Init()
	{
//        Texture::LoadTexture("myfont", "textures/myfont2.png");
//
//        std::vector<vec4> rect = Geometry::CreateRectangle(1.0);
//		std::vector<vec4> normals({ vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0) });
//		std::vector<vec4> tangents({ vec4(1.0, 0.0, 0.0, 0.0), vec4(1.0, 0.0, 0.0, 0.0), vec4(1.0, 0.0, 0.0, 0.0), vec4(1.0, 0.0, 0.0, 0.0) });
//		std::vector<vec4> binormals({ vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0) });
//		std::vector<vec2> texCoords = Util::Vec4toVec2(Util::TransformVertices(rect, translate(vec3(0.5f, 0.5f, 0.0f)) * glm::scale(vec3(1.0, 1.0, 1.0))));
//
//		int vertexOffset = Shader::Vertices.size();
//		indexOffset = Shader::VertexIndices.size();
//		indexCount = rect.size();
//
//		std::vector<uint> indices;
//		for (int i = 0; i < indexCount; ++i)
//			indices.push_back(vertexOffset + i);
//
//		Shader::Vertices.insert(Shader::Vertices.end(), rect.begin(), rect.end());
//		Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
//		Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
//		Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
//		Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
//		Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());
	}


	// PRIVATE

	std::vector<Rectangle*> Font::characterRectangles;
}
