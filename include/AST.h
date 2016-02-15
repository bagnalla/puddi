#ifndef AST_H
#define AST_H

#include "DrawableObject.h"
#include <string>
#include <queue>

namespace grumpy
{
	class ASTNode : public puddi::DrawableObject
	{
	public:
		ASTNode(const puddi::Object *par, DrawableObject *connector, std::queue<char> &input);

		int GetWidth() const;

	private:
		std::vector<DrawableObject*> glyphs;
		DrawableObject *parentConnector;
		DrawableObject *parentConnectorLine;
		std::vector<ASTNode*> childNodes;
		int width;
		Object *container;
	};
}

#endif
