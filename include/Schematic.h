#ifndef SCHEMATIC_H
#define SCHEMATIC_H

#include "Material.h"
#include "VertexMesh.h"
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

namespace puddi
{
	struct SchematicNode
	{
		mat4 transform;
		std::vector<VertexMesh> vertexMeshes;

		std::vector<SchematicNode*> children;
	};

	namespace Schematic
	{
        void Init();

        void Cleanup();

        int LoadSchematic(const char *filepath, const std::string& name, const std::string& subdirectory = "");

        SchematicNode* GetSchematicByName(const std::string& name);
	};
}

#endif
