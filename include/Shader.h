#ifndef SHADER_H
#define SHADER_H

#define BUFFER_OFFSET(offset)   ((GLvoid*) (offset))

#include <string>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL_video.h>
#include "GlmHeaders.h"

namespace puddi
{
    struct Material;
    enum ShadowMode : int;

    enum TextureSampler
    {
        TEXTURE_2D,
        TEXTURE_2D_BUMP,
        TEXTURE_CUBE,
        TEXTURE_CUBE_BUMP,
        TEXTURE_SHADOW_2D,
        TEXTURE_SHADOW_CUBE,
        TEXTURE_TERRAIN_1,
        TEXTURE_TERRAIN_BUMP_1,
        TEXTURE_TERRAIN_2,
        TEXTURE_TERRAIN_BUMP_2,
        TEXTURE_TERRAIN_3,
        TEXTURE_TERRAIN_BUMP_3,
        TEXTURE_TERRAIN_4,
        TEXTURE_TERRAIN_BUMP_4,
        TEXTURE_BONE_TRANSFORM
    };

    struct VertexData
    {
        std::vector<vec4> vertices;
        std::vector<vec4> normals;
        std::vector<vec4> tangents;
        std::vector<vec4> binormals;
        std::vector<vec2> textureCoordinates;
        std::vector<vec4> boneIndices;
        std::vector<vec4> boneWeights;
    };

    namespace Shader
    {
        extern std::vector<vec4> Vertices;
        extern std::vector<vec4> Normals;
        extern std::vector<vec4> Tangents;
        extern std::vector<vec4> Binormals;
        extern std::vector<vec2> TextureCoordinates;
        extern std::vector<vec4> BoneIndices;
        extern std::vector<vec4> BoneWeights;

        extern std::vector<uint> VertexIndices;

        void Init();
        void InitOrtho(SDL_Window *window);
        void UpdateVertexPositions(size_t offset, const std::vector<vec4>& vertices);

        void SetProgram(std::string programName);

        void SetMaterialAmbient(const vec4& materialAmbient);
        void SetMaterialDiffuse(const vec4& materialDiffuse);
        void SetMaterialSpecular(const vec4& materialSpecular);
        void SetMaterialShininess(float materialShininess);
        void SetMaterial(const Material& material);
        void SetLightSource(const mat4& lightSource);
        void SetCameraPosition(const vec4& cameraPosition);
        void SetModel(const mat4& model);
        void SetCamera(const mat4& camera);
        void SetProjection(const mat4& projection);
        void SetLightProjection(const mat4& lightProj);
        void SetEmissionColor(const vec4& emissionColor);
        void SetTextureBlend(bool textureBlendb);
        void SetShadowMode(ShadowMode shadowMode);
        void SetShadowZRange(const vec2& shadowZRange);
        void SetReflectiveCubeMap(bool b);

        // terrain uniforms
        void SetGroundCoordZ(float z);
        void SetTerrainScaleZ(float z);
        void SetTerrainMaxHeight(float z);
        void SetTexHeights(const vec4& texHeights);

        // either depth texture or depth cubemap
        void BindShadowMap(GLuint shadowMap, ShadowMode mode);

        // switch between perspective and ortho projections (world and HUD)
        // when the shader program changes it resets to perspective
        void SetOrtho(bool o);

        void SetOrthoLightSource (const mat4& orthoLight);
    };
}

#endif
