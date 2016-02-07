#ifndef TEXT_H
#define TEXT_H

#include "Rectangle.h"
#include <GL/glew.h>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace puddi
{
	class Text
	{
	public:
		static void Init();
		
		static std::vector<GLuint> CreateTexturesFromString(const std::string &str);
		static GLuint createTextureFromCharacter(char c);
		
	private:
		static FT_Library library;
		static FT_Face bitStreamFace;
		
		//static Rectangle* createTexturedRectangleFromCharacter(char c);
	};
}

#endif
