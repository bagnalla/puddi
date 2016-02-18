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
		ASTNode(puddi::Object *par, ASTNode *parentNode, DrawableObject *connector, std::queue<char> &input);

		int GetWidth() const;

		bool GetTokenRequired() const;

		std::vector<ASTNode*> ChildNodes;

		void Hide();
		void Show();

		void Resize();

		void Update();

		void SetAssignedLocation(const glm::vec4 &l);

		int GetParseIndex() const;

		void SetNodeColor(glm::vec4 c);

		DrawableObject* GetParentConnector() const;

		bool GetHidden() const;

		ASTNode* GetParent() const;

	private:
		ASTNode *parent;
		std::vector<DrawableObject*> glyphs;
		std::vector<DrawableObject*> childConnectors;
		DrawableObject *parentConnector;
		DrawableObject *parentConnectorLine;
		DrawableObject *body;
		int width;
		DrawableObject *container;
		bool tokenRequired;
		int glyphCount;
		int parseIndex;
		bool hidden;
		float normalAlpha;

		glm::vec4 assignedLocation;
		float velocity;

		int resizeRecursive();

		void rePositionConnectorLine();
	};
}

#endif
