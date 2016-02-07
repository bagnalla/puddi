#ifndef SPHERE_H
#define SPHERE_H

#include "DrawableObject.h"

namespace puddi
{
	class Sphere : public DrawableObject
	{
	public:
		static void Init();

		Sphere(const Object *par);

		//void Draw() const;

	private:
		static int indexOffset; // location of sphere vertices in global array
		static int indexCount; // # of cube vertices

		static void createPrototypeVertices();
	};
}

#endif
