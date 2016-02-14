#ifndef AST_H
#define AST_H

#include "DrawableObject.h"
#include <string>

namespace grumpy
{
	class ASTNode : public puddi::DrawableObject
	{
	public:
		ASTNode(const puddi::Object *par, const std::string &input);

	private:
		std::string name;
	};
}

#endif
