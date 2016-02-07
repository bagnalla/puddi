#ifndef FONT_H
#define FONT_H

#include "Rectangle.h"
#include <vector>

namespace puddi
{
	class Font
	{
	public:
        static void Init();

	private:
		static std::vector<Rectangle*> characterRectangles;
	};
}

#endif
