/**
 * @file Font.h
 * @author Alexander Bagnall
 * @brief Provides the ability to render text.
 *
 * Contains a function for loading glyph textures for a font and
 * a convenience function for creating a vector of glyphs from
 * an input string.
 */

#ifndef FONT_H
#define FONT_H

#include <string>
#include <vector>

namespace puddi
{
    class DrawableObject;
    class Object;
    class Scene;

    namespace Font
    {
        void Init();

        std::vector<DrawableObject*> CreateGlyphString(Scene *scene,
						       Object *rootObject,
						       std::string font,
						       std::string str,
						       float scale = 1.0f);
    };
}

#endif
