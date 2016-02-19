#ifndef PARSER_H
#define PARSER_H

#include "DrawableObject.h"
#include "Schematic.h"
#include "Token.h"
#include "AST.h"
#include <vector>
#include <queue>

namespace grumpy
{
    enum SyntaxParserState : int
    {
        SYNTAXPARSER_STATE_WAITING,
        SYNTAXPARSER_STATE_MOVING,
        SYNTAXPARSER_STATE_DONE
    };

    class SyntaxParser : public puddi::DrawableObject
    {
    public:
		SyntaxParser(Object* par, ASTNode *root);
		SyntaxParser(Object* par, ASTNode *root, puddi::SchematicNode *schematic);

        void Update();

        float GetVelocity() const;
        void SetVelocity(float v);

		void SetHomePosition(glm::vec4 v);

    private:
        ASTNode *astRoot;
        std::vector<ASTNode*> nodesVector;
        int currentNodeIndex;
        float velocity;
        SyntaxParserState state;
        glm::vec4 homePosition;

		void init(ASTNode *root);
        void createNodesVector();
        void addToNodesVectorRecursive(ASTNode *node);
    };
}

#endif
