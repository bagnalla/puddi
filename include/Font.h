#ifndef FONT_H
#define FONT_H

#include <unordered_map>
#include <string>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <OGLFT.h>

namespace puddi
{
	static const float FONT_POINT_SIZE = 16;

	class Font
	{
	public:
        static void Init();

		static Font* LoadFont(const std::string name, const std::string path);
		static Font* GetFontByName(const std::string name);

		OGLFT::Monochrome* commands_face;
		OGLFT::Monochrome* monochrome_face;
		OGLFT::Grayscale* grayscale_face;
		OGLFT::Translucent* translucent_face;
		OGLFT::Outline* outline_face;
		OGLFT::Filled* filled_face;
		#ifndef OGLFT_NO_SOLID
		OGLFT::Solid* solid_face;
		#else
		OGLFT::Monochrome* solid_face;
		#endif
		OGLFT::MonochromeTexture* monochrome_texture_face;
		OGLFT::GrayscaleTexture* grayscale_texture_face;
		OGLFT::TranslucentTexture* translucent_texture_face;

	private:
		static std::unordered_map<std::string, Font*> fontMap;

		static Font* loadFont(const std::string path);
	};
}

#endif
