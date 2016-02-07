#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "DrawableObject.h"

namespace puddi
{
	class Rectangle : public DrawableObject
	{
	public:
		static void Init();

		Rectangle(const Object *par);

		//void Draw() const;

	private:
		static int indexOffset;
		static int indexCount;

		static void createPrototypeVertices();
	};
}

#endif
