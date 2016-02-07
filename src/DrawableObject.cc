#include "DrawableObject.h"
#include "Shader.h"
#include "Material.h"
#include "RenderGraph.h"
#include "Puddi.h"
#include "Shadow.h"
#include "VertexMesh.h"
#include "Schematic.h"
#include "Texture.h"

namespace puddi
{
	// PUBLIC

	DrawableObject::DrawableObject(const Object *par) : Object(par)
	{
		init();
	}

	DrawableObject::DrawableObject(const Object *par, const VertexMesh &mesh) : DrawableObject(par)
	{
        VertexMesh *vertexMesh = new VertexMesh(mesh);
		vertexMesh->SetOwner(this);
		vertexMeshes.push_back(vertexMesh);
	}

	DrawableObject::DrawableObject(const Object *par, SchematicNode *schematic) : DrawableObject(par)
	{
		for (auto it = schematic->vertexMeshes.begin(); it != schematic->vertexMeshes.end(); ++it)
		{
			vertexMeshes.push_back(new VertexMesh(*it));
			vertexMeshes[vertexMeshes.size() - 1]->SetOwner(this);
		}

		for (auto it = schematic->children.begin(); it != schematic->children.end(); ++it)
			AddChild(new DrawableObject(this, *it));

		updateRenderNodes();
	}

	void DrawableObject::Draw() const
	{
		Shader::SetModel(finalModel);
		/*if (cubeMap != 0)
			Shader::SetReflectiveCubeMap(reflectiveCubeMap);*/
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->Draw();
	};

	void DrawableObject::SendTransformToGPU()
	{
		Shader::SetModel(finalModel);
	}

	void DrawableObject::AddVertexMesh(VertexMesh *vertexMesh)
	{
		vertexMeshes.push_back(vertexMesh);
		vertexMeshes[vertexMeshes.size() - 1]->SetOwner(this);
		updateRenderNodes();
	}

	void DrawableObject::SetEmissive(bool e)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->SetEmissive(e);
		updateRenderNodes();
	}

	//void DrawableObject::SetBlend(bool b)
	//{
	//	blend = b;
	//}

	void DrawableObject::SetEmissionColor(const vec4& c)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->SetEmissionColor(c);
		updateRenderNodes();
	}

	void DrawableObject::SetMaterial(const Material& m)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->SetMaterial(m);
		updateRenderNodes();
	}

	void DrawableObject::SetTexture(GLuint tex)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->SetTexture(tex);
		updateRenderNodes();
	}

	void DrawableObject::SetCubeMap(GLuint cm)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->SetCubeMap(cm);
		updateRenderNodes();
	}

	void DrawableObject::EnableRender()
	{
		renderEnabled = true;

		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->EnableRender();

		for (auto it = children.begin(); it != children.end(); ++it)
			static_cast<DrawableObject*>(*it)->EnableRender();

		updateRenderNodes();
	}

	void DrawableObject::DisableRender()
	{
		renderEnabled = false;

		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->DisableRender();

		for (auto it = children.begin(); it != children.end(); ++it)
			static_cast<DrawableObject*>(*it)->DisableRender();
	}

	void DrawableObject::Cull()
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->Cull();

		Object::Cull();
	}

	void DrawableObject::UnCull()
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->UnCull();

		Object::UnCull();

		updateRenderNodes();
	}

	bool DrawableObject::GetRenderEnabled() const
	{
		return renderEnabled;
	}

	void DrawableObject::SetBumpMapEnabled(bool b)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->SetBumpMapEnabled(b);
		for (auto it = children.begin(); it != children.end(); ++it)
			static_cast<DrawableObject*>(*it)->SetBumpMapEnabled(b);
	}

	void DrawableObject::SetReflectiveCubeMap(bool b)
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->SetReflectiveCubeMap(b);
		for (auto it = children.begin(); it != children.end(); ++it)
			static_cast<DrawableObject*>(*it)->SetReflectiveCubeMap(b);
	}

	void DrawableObject::DisableShadowCasting()
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			Shadow::RemoveFromDepthRenderList(*it);
		for (auto it = children.begin(); it != children.end(); ++it)
			static_cast<DrawableObject*>(*it)->DisableShadowCasting();
	};

	// PRIVATE

	void DrawableObject::init()
	{
		renderEnabled = true;
		updateRenderNodes();
	}

	void DrawableObject::updateRenderNodes()
	{
		for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
			(*it)->UpdateRenderNode();
	}
}
