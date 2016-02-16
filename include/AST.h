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
		ASTNode(const puddi::Object *par, ASTNode *parentNode, DrawableObject *connector, std::queue<char> &input);

		int GetWidth() const;

		bool GetTokenRequired() const;

        std::vector<ASTNode*> ChildNodes;

        void Hide();
        void Show();

	private:
        ASTNode *parent;
		std::vector<DrawableObject*> glyphs;
		DrawableObject *parentConnector;
		DrawableObject *parentConnectorLine;
		int width;
		DrawableObject *container;
		bool tokenRequired;
	};
}

#endif
