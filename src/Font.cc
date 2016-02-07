#include "Font.h"
#include <iostream>

using namespace std;

namespace puddi
{
	// PUBLIC

	void Font::Init()
	{
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	}

	Font* Font::LoadFont(const std::string name, const std::string path)
	{
		Font *font = loadFont(path);
		if (font != nullptr)
		{
			fontMap.emplace(name, font);
		}
		return font;
	}

	Font* Font::GetFontByName(const std::string name)
	{
        return fontMap[name];
	}

	// PRIVATE

	std::unordered_map<std::string, Font*> Font::fontMap;

	Font* Font::loadFont(const std::string path)
	{
		Font *font = new Font();
		font->commands_face = new OGLFT::Monochrome( path.c_str(), FONT_POINT_SIZE / 2. );
		font->commands_face->setHorizontalJustification( OGLFT::Face::CENTER );

		font->monochrome_face = new OGLFT::Monochrome( path.c_str(), FONT_POINT_SIZE );
		font->monochrome_face->setHorizontalJustification( OGLFT::Face::CENTER );
		font->monochrome_face->setForegroundColor( 1., 0., 0., 1. );

		if ( !font->monochrome_face->isValid() ) {
			cerr << "failed to open face for font at " << path << "." << endl;
		}

		font->grayscale_face = new OGLFT::Grayscale( path.c_str(), FONT_POINT_SIZE );
		font->grayscale_face->setHorizontalJustification( OGLFT::Face::CENTER );
		font->grayscale_face->setForegroundColor( 0., 0., .5, 1. );
		font->grayscale_face->setBackgroundColor( 0., 1., 1., 1. );

		font->translucent_face = new OGLFT::Translucent( path.c_str(), FONT_POINT_SIZE );
		font->translucent_face->setHorizontalJustification( OGLFT::Face::CENTER );
		font->translucent_face->setForegroundColor( 0., .5, 0., 1. );

		font->outline_face = new OGLFT::Outline( path.c_str(), FONT_POINT_SIZE );
		font->outline_face->setHorizontalJustification( OGLFT::Face::CENTER );
		font->outline_face->setForegroundColor( 1., 1., 0., 1. );

		font->filled_face = new OGLFT::Filled( path.c_str(), FONT_POINT_SIZE );
		font->filled_face->setHorizontalJustification( OGLFT::Face::CENTER );
		font->filled_face->setForegroundColor( .5, 0., 1., 1. );

		#ifndef OGLFT_NO_SOLID
		font->solid_face = new OGLFT::Solid( path.c_str(), FONT_POINT_SIZE );
		font->solid_face->setDepth( 10. );
		font->solid_face->setCharacterRotationX( 25. );
		font->solid_face->setCharacterRotationY( 25. );
		font->solid_face->setTessellationSteps( 3 );
		#else
		font->solid_face = new OGLFT::Monochrome( path.c_str(), FONT_POINT_SIZE );
		#endif
		font->solid_face->setHorizontalJustification( OGLFT::Face::CENTER );
		font->solid_face->setForegroundColor( 1., .5, 0., 1. );

		font->monochrome_texture_face = new OGLFT::MonochromeTexture( path.c_str(), FONT_POINT_SIZE );
		font->monochrome_texture_face->setHorizontalJustification( OGLFT::Face::CENTER );
		font->monochrome_texture_face->setForegroundColor( 0., .5, .75, 1. );

		font->grayscale_texture_face = new OGLFT::GrayscaleTexture( path.c_str(), FONT_POINT_SIZE );
		font->grayscale_texture_face->setHorizontalJustification( OGLFT::Face::CENTER );
		font->grayscale_texture_face->setForegroundColor( 0.9, .65, .9, 1. );
		font->grayscale_texture_face->setBackgroundColor( 0.5, .5, .75, 0.3 );

		font->translucent_texture_face = new OGLFT::TranslucentTexture( path.c_str(), FONT_POINT_SIZE );
		font->translucent_texture_face->setHorizontalJustification( OGLFT::Face::CENTER );
		font->translucent_texture_face->setForegroundColor( 0.75, 1., .75, 1. );

		return font;
	}
}
