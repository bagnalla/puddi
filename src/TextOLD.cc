#include "Text.h"
#include "Texture.h"
#include <iostream>

#include "Puddi.h"
#include "Rectangle.h"

namespace puddi
{
	// PUBLIC
	
	void Text::Init()
	{
		int error;
		if (error = FT_Init_FreeType(&library))
		{
			std::cerr << "error initializing FreeType library\n";
		}
		
		//error = FT_New_Face(library, "fonts/Bitstream/VeraMono-Bold.ttf", 0, &bitStreamFace);
		error = FT_New_Face(library, "fonts/SourceCodePro/SourceCodePro-Medium.otf", 0, &bitStreamFace);
		if (error == FT_Err_Unknown_File_Format)
		{
			std::cerr << "Bitstream/VeraMono-Bold font format not supported.\n";
		}
		else if (error)
		{
			std::cerr << "Bitstream/VeraMono-Bold unknown error.\n";
		}
		
		error = FT_Set_Char_Size(
			bitStreamFace,    /* handle to face object           */
			8*64,       /* char_width in 1/64th of points  */
			16*64,   /* char_height in 1/64th of points */
			1920,     /* horizontal device resolution    */
			1080		 /* vertical device resolution      */
		);
		
		//error = FT_Set_Pixel_Sizes(
			//bitStreamFace,   /* handle to face object */
			//0,      /* pixel_width. 0 means "same as other" */
			//256 );   /* pixel_height */
          
		if (error)
		{
			std::cerr << "couldn't set size of Bitstream/VeraMono-Bold for some reason.\n";
		}
	}
	
	std::vector<GLuint> Text::CreateTexturesFromString(const std::string &str)
	{
		std::vector<GLuint> textures;
		
		for (int i = 0; i < str.length(); ++i)
		{
			textures.push_back(createTextureFromCharacter(str[i]));
		}
		
		return textures;
	}
	
	// PRIVATE
	
	FT_Library Text::library;
	FT_Face Text::bitStreamFace;
	
	
	inline int next_p2 (int a )
	{
		int rval=1;
		// rval<<=1 Is A Prettier Way Of Writing rval*=2;
		while(rval<a) rval<<=1;
		return rval;
	}

	GLuint Text::createTextureFromCharacter(char c)
	{
		int error;
		
		/* retrieve glyph index from character code */
		FT_UInt glyph_index = FT_Get_Char_Index(bitStreamFace, c);
		
		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph(bitStreamFace, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{
			std::cerr << "error loading glyph for character '" << c << "'\n";
			return 0;
		}
		
		/* convert to an anti-aliased bitmap */
		error = FT_Render_Glyph(bitStreamFace->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
		{
			std::cerr << "error converting glyph to bitmap '" << c << "'\n";
			return 0;
		}
		
		auto bitmap = bitStreamFace->glyph->bitmap;
		//for (int j = 0; j < bitmap.rows; ++j)
		//{
			//for (int i = 0; i < bitmap.width; ++i)
			//{
				//std::cout << bitmap.buffer[i + bitmap.width*j] << " ";
			//}
			//std::cout << std::endl;
		//}
		
		//int width = next_p2( bitmap.width );
		//int height = next_p2( bitmap.rows );
		//GLubyte* expanded_data = new GLubyte[ 4 * width * height];
		//for(int j=0; j <height;j++) {
			//for(int i=0; i < width; i++){
				//expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = expanded_data[2*(i+j*width)+2] = expanded_data[2*(i+j*width)+3] = 
					//(i>=bitmap.width || j>=bitmap.rows) ?
					//0 : bitmap.buffer[i + bitmap.width*j];
			//}
		//}
		//GLuint tex = Texture::CreateTextureFromBitmap(expanded_data, width, height, GL_RGBA);
		
		int width = next_p2( bitmap.width );
		//int height = next_p2( bitmap.rows );
		//int width = bitmap.width;
		int height = bitmap.rows;
		GLubyte* expanded_data = new GLubyte[ 2 * width * height];
		for(int j=0; j <height;j++) {
			for(int i=0; i < width; i++){
				expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] = 
					(i>=bitmap.width || j>=bitmap.rows) ?
					0 : bitmap.buffer[i + bitmap.width*j];
			}
		}
		GLuint tex = Texture::CreateTextureFromBitmap(expanded_data, width, height, GL_LUMINANCE_ALPHA);
		
		//int width = bitmap.width * 4;
		//int height = bitmap.height * 4;
		//GLubyte* expanded_data = new GLubyte[width * height];
		//for(int j=0; j <height;j++)
		//{
			//for(int i=0; i < width; i++
			//{
				//expanded_data[4*(i+j*width)] = expanded_data[4*(i+j*width)] = expanded_data[4*(i+j*width)] = expanded_data[4*(i+j*width)] = 
			//}
		//}
		
		//GLuint tex = Texture::CreateTextureFromBitmap(bitmap.buffer, bitmap.width, bitmap.rows, GL_ALPHA, "muh");
		
		return tex;
	}
}
