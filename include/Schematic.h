#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include "Material.h"
#include "VertexMesh.h"
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <assimp/scene.h>
#include <vector>
#include <unordered_map>

namespace puddi
{
	struct SchematicNode
	{
		mat4 transform;
		std::vector<VertexMesh> vertexMeshes;

		std::vector<SchematicNode*> children;
	};

	class Schematic
	{
	public:
		static void Init();

		static void Cleanup();

		static int InitSchematic(const char *filepath, const char *name, std::string subdirectory = "");

		static SchematicNode* GetSchematicByName(const char *name);

	private:
		static std::unordered_map<const char*, SchematicNode*> schematicNodeMap;

		static SchematicNode* buildSchematic(const aiScene *scene, aiNode *node, std::string subdirectory);
	};
}

#endif
