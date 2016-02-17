#ifndef PARSER_H
#define PARSER_H

#include "DrawableObject.h"
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

        void Update();

        float GetVelocity() const;
        void SetVelocity(float v);

        void SetTokenQueue(const std::queue<Token*> &tokenQ);

		void AddTokenToQueue(Token *t);

		void SetHomePosition(glm::vec4 v);

    private:
        ASTNode *astRoot;
        std::vector<ASTNode*> nodesVector;
        std::queue<Token*> tokenQueue;
        int currentNodeIndex;
        float velocity;
        SyntaxParserState state;
        glm::vec4 homePosition;

        void createNodesVector();
        void addToNodesVectorRecursive(ASTNode *node);
    };
}

#endif
