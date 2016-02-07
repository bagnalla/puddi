#ifndef SKYBOX_H
#define SKYBOX_H

#include "DrawableObject.h"

namespace puddi
{
	class Skybox : public DrawableObject
	{
	public:
		static void Init();

		Skybox(const Object *par);

		//void Draw() const;

	private:
        static int indexOffset;
        static int indexCount;

		static void createPrototypeVertices();
	};
}

#endif
