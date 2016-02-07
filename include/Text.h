#ifndef TEXT_H
#define TEXT_H

#include "Object.h"
#include <GL/glew.h>
#include <string>
#include <OGLFT.h>

namespace puddi
{
	class Text : public Object
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
