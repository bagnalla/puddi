#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <SDL2/SDL_image.h>

namespace puddi
{
    namespace Texture
    {
        void InitTextureLoading();
        void EndTextureLoading();

        GLuint LoadTexture(const char *name, const char *filePath, const char *bumpPath = nullptr);
        GLuint LoadBumpMap(const char *name, const char *bumpPath);
        GLuint LoadCubeMap(const char *name, const char *filePath, const char *bumpPath = nullptr);
        GLuint LoadCubeMapMirrored(const char *name, const char *filePath, const char *bumpPath = nullptr);

        GLuint GetTextureByName(std::string name);
        GLuint GetBumpMapByName(std::string name); // only works if it was loaded by name
        GLuint GetTextureByFile(std::string path);
        GLuint GetBumpMapByTexture(GLuint tex);

        GLuint GetCubeMapByName(std::string name);
        GLuint GetCubeMapByFile(std::string path);
        GLuint GetBumpMapByCubeMap(GLuint cubeMap);

        void DestroyTexture(const std::string &name, const std::string &path = "");
    };
}

#endif
