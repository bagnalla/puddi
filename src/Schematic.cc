#include <iostream>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "Schematic.h"
#include "Shader.h"
#include "Skeleton.h"
#include "Texture.h"
#include "Util.h"

using namespace std;

namespace puddi
{
  namespace Schematic
  {
    // PRIVATE
    namespace
    {
      std::unordered_map<string, SchematicNode*> schematicNodeMap;

      SchematicNode* buildSchematic(const aiScene *scene, aiNode *node,
				    const string& schematicName,
				    const string& subdirectory)
      {
	SchematicNode *schematicNode = new SchematicNode();

	/*aiVector3D scaling;
	  aiQuaterniont<float> rotation;
	  aiVector3D position;

	  node->mTransformation.DecomposeB(scaling, rotation, position);*/

	// TRANSFORM
	schematicNode->transform = Util::Mat4OfAiMat4(node->mTransformation);

	// may be multiple meshes per node (transform shared among them)
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
	  auto mesh = scene->mMeshes[node->mMeshes[i]];

	  int vertexOffset = Shader::Vertices.size();

	  // vertex positions
	  std::vector<vec4> vertices;
	  for (size_t j = 0; j < mesh->mNumVertices; ++j)
	    vertices.push_back(vec4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.0f));
	  // add to global array
	  Shader::Vertices.insert(Shader::Vertices.end(), vertices.begin(), vertices.end());

	  // vertex normals
	  std::vector<vec4> normals;
	  if (mesh->HasNormals())
	  {
	    for (size_t j = 0; j < mesh->mNumVertices; ++j)
	      normals.push_back(vec4(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z, 0.0f));
	    // add to global array
	    Shader::Normals.insert(Shader::Normals.end(), normals.begin(), normals.end());
	    normals.clear();
	    normals.shrink_to_fit();
	  }
	  else
	  {
	    Shader::Normals.insert(Shader::Normals.end(), vertices.begin(), vertices.end());
	  }

	  // vertex tangents and binormals
	  std::vector<vec4> tangents;
	  std::vector<vec4> binormals;
	  if (mesh->HasTangentsAndBitangents())
	  {
	    for (size_t j = 0; j < mesh->mNumVertices; ++j)
	      tangents.push_back(vec4(mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z, 0.0f));
	    // add to global array
	    Shader::Tangents.insert(Shader::Tangents.end(), tangents.begin(), tangents.end());
	    tangents.clear();
	    tangents.shrink_to_fit();
	    for (size_t j = 0; j < mesh->mNumVertices; ++j)
	      binormals.push_back(vec4(mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z, 0.0f));
	    // add to global array
	    Shader::Binormals.insert(Shader::Binormals.end(), binormals.begin(), binormals.end());
	    binormals.clear();
	    binormals.shrink_to_fit();
	  }
	  else
	  {
	    Shader::Tangents.insert(Shader::Tangents.end(), vertices.begin(), vertices.end());
	    Shader::Binormals.insert(Shader::Binormals.end(), vertices.begin(), vertices.end());
	  }

	  // vertex texture coordinates
	  std::vector<vec2> textureCoordinates;
	  if (mesh->HasTextureCoords(0))
	  {
	    for (size_t j = 0; j < mesh->mNumVertices; ++j)
	    {
	      //std::cout << mesh->mTextureCoords[0][j].x << " " << mesh->mTextureCoords[0][j].y << std::endl;
	      textureCoordinates.push_back(vec2(mesh->mTextureCoords[0][j].x, -mesh->mTextureCoords[0][j].y));
	    }
	  }
	  else
	  {
	    std::cerr << "no texture coordinates for model " << subdirectory << std::endl;
	    for (size_t j = 0; j < mesh->mNumVertices; ++j)
	      textureCoordinates.push_back(vec2(0.0f, 0.0f));
	  }
	  // add to global array
	  Shader::TextureCoordinates.insert(Shader::TextureCoordinates.end(), textureCoordinates.begin(), textureCoordinates.end());
	  textureCoordinates.clear();
	  textureCoordinates.shrink_to_fit();

	  // map vertices to bone (index, weight) pairs
	  vector<vector<pair<int, float> > > vertexBones(vertices.size());

	  // vertex bone indices and weights
	  std::vector<vec4> boneIndices;
	  std::vector<vec4> boneWeights;
	  if (mesh->HasBones())
	  {
	    // for each bone
	    for (size_t j = 0; j < mesh->mNumBones; ++j)
	    {
	      auto aiBone = mesh->mBones[j];
	      auto bone = Skeleton::GetBoneByName(schematicName, aiBone->mName.C_Str());

	      // set the bone's inverse bindpose
	      //Skeleton::SetBoneBindPoseInverse(schematicName, bone.name, Util::Mat4OfAiMat4(aiBone->mOffsetMatrix));

	      // add the (index, weight) pair for this bone for each vertex it influences
	      for (size_t k = 0; k < aiBone->mNumWeights; ++k)
	      {
		auto vertexWeight = aiBone->mWeights[k];
		vertexBones[vertexWeight.mVertexId].push_back(make_pair(bone.index, vertexWeight.mWeight));
	      }
	    }

	    // iterate over the vertex->bone mapping and create the vec4s to put in the vertex attribute array
	    for (auto it = vertexBones.begin(); it != vertexBones.end(); ++it)
	    {
	      vec4 vBoneIndices = vec4(-1.0f, -1.0f, -1.0f, -1.0f);
	      vec4 vBoneWeights = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	      // up to four bones
	      for (size_t k = 0; k < 4; ++k)
	      {
		if (it->size() > k)
		{
		  vBoneIndices[k] = (*it)[k].first;
		  vBoneWeights[k] = (*it)[k].second;
		}
	      }
	      boneIndices.push_back(vBoneIndices);
	      boneWeights.push_back(vBoneWeights);
	    }
	  }
	  else
	  {
	    // no bones for this mesh
	    for (size_t j = 0; j < mesh->mNumVertices; ++j)
	    {
	      boneIndices.push_back(vec4(-1.0f, -1.0f, -1.0f, -1.0f));
	      boneWeights.push_back(vec4(0.0f, 0.0f, 0.0f, 0.0f));
	    }
	  }
	  // add to global array
	  Shader::BoneIndices.insert(Shader::BoneIndices.end(), boneIndices.begin(), boneIndices.end());
	  boneIndices.clear();
	  boneIndices.shrink_to_fit();
	  Shader::BoneWeights.insert(Shader::BoneWeights.end(), boneWeights.begin(), boneWeights.end());
	  boneWeights.clear();
	  boneWeights.shrink_to_fit();

	  // mesh indices
	  std::vector<uint> indices;
	  for (size_t j = 0; j < mesh->mNumFaces; ++j)
	  {
	    auto face = mesh->mFaces[j];
	    if (face.mNumIndices == 3)
	    {
	      indices.push_back(face.mIndices[0] + vertexOffset);
	      indices.push_back(face.mIndices[1] + vertexOffset);
	      indices.push_back(face.mIndices[2] + vertexOffset);
	    }
	  }

	  int indexOffset = Shader::VertexIndices.size();
	  int indexCount = indices.size();

	  // add to global array
	  Shader::VertexIndices.insert(Shader::VertexIndices.end(), indices.begin(), indices.end());
	  indices.clear();
	  indices.shrink_to_fit();

	  // clear vertices
	  vertices.clear();
	  vertices.shrink_to_fit();

	  // MATERIAL
	  auto aiMat = scene->mMaterials[mesh->mMaterialIndex];
	  Material mat;

	  std::vector<aiMaterialProperty*> matProps;
	  for (size_t j = 0; j < aiMat->mNumProperties; ++j)
	  {
	    matProps.push_back(aiMat->mProperties[j]);
	  }

	  aiColor3D color(0.f, 0.f, 0.f);
	  aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	  mat.ambient = vec4(color.r, color.g, color.b, 1.0f);
	  aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	  mat.diffuse = vec4(color.r, color.g, color.b, 1.0f);
	  aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	  mat.specular = vec4(color.r, color.g, color.b, 1.0f);

	  float shininess;
	  aiMat->Get(AI_MATKEY_SHININESS, shininess);
	  mat.shininess = shininess;

	  // TEXTURE
	  aiString texturePath;
	  aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
	  aiString bumpMapPath;
	  aiMat->GetTexture(aiTextureType_HEIGHT, 0, &bumpMapPath);

	  // CREATE AND ADD MESH OBJECT TO SCHEMATIC
	  //VertexMesh vMesh(NULL, Material::Rubber(vec4(1.0f, 1.0f, 1.0f, 1.0f)), indexOffset, indexCount, false);
	  VertexMesh vMesh(NULL, Material::None(), indexOffset,
			   indexCount, V_TRIANGLES);
	  set<char> delims = { '\\', '/' };
	  string texName = Util::splitpath(texturePath.C_Str(), delims).back();
	  string bumpName = Util::splitpath(bumpMapPath.C_Str(), delims).back();
	  if (texName != "")
	  {
	    string texPath = "textures/" + (subdirectory != "" ? subdirectory + "/" : "") + texName;
	    string bumpPath = "bumpmaps/" + (subdirectory != "" ? subdirectory + "/" : "") + bumpName;
	    GLuint texture = 0;
	    if (texName != "")
	      texture = Texture::LoadTexture(texName.c_str(),
					     texPath.c_str(),
					     (bumpName.size() != 0 ?
					      bumpPath.c_str() : nullptr));
	    else
	      vMesh.SetMaterial(mat);
	    vMesh.SetTexture(texture);
	  }

	  schematicNode->vertexMeshes.push_back(vMesh);
	}

	// recursively build children schematic nodes
	for (uint i = 0; i < node->mNumChildren; ++i)
	{
	  SchematicNode *childNode = buildSchematic(scene, node->mChildren[i],
						    schematicName,
						    subdirectory);
	  if (childNode != nullptr)
	    schematicNode->children.push_back(childNode);
	}

	return schematicNode;
      }
    }

    // PUBLIC

    void Init()
    {
      //aiLogStream stream;
      //// get a handle to the predefined STDOUT log stream and attach
      //// it to the logging system. It remains active for all further
      //// calls to aiImportFile(Ex) and aiApplyPostProcessing.
      //stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
      //aiAttachLogStream(&stream);
      //// ... same procedure, but this stream now writes the
      //// log messages to assimp_log.txt
      //stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
      //aiAttachLogStream(&stream);
    }

    void Cleanup()
    {
      // We added a log stream to the library, it's our job to disable it
      // again. This will definitely release the last resources allocated
      // by Assimp.
      aiDetachAllLogStreams();
    }

    int LoadSchematic(const char *filepath, const string& name,
		      const string& subdirectory)
    {
      if (schematicNodeMap.find(name) != schematicNodeMap.end())
      {
	std::cerr << "schematic with name " << name << ". already exists\n";
	return -1;
      }

      const aiScene *scene = NULL;

      if ((scene =
	   aiImportFile(filepath,
			aiProcessPreset_TargetRealtime_MaxQuality)) != NULL)
	//if ((scene = aiImportFile("model\\Millennium_Falcon.obj", aiProcessPreset_TargetRealtime_Fast)) != NULL)
      {
	SchematicNode *schematic = buildSchematic(scene, scene->mRootNode,
						  name, subdirectory);

	schematicNodeMap.emplace(name, schematic);
      }
      else
      {
	std::cerr << aiGetErrorString() << std::endl;
	return -2;
      }

      // cleanup - calling 'aiReleaseImport' is important, as the library
      // keeps internal resources until the scene is freed again. Not
      // doing so can cause severe resource leaking.
      aiReleaseImport(scene);

      return 0;
    }

    SchematicNode* GetSchematicByName(const string& name)
    {
      return schematicNodeMap[name];
    }
  }
}
