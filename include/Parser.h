#ifndef PARSER_H
#define PARSER_H

#include "DrawableObject.h"
#include "Token.h"
#include "AST.h"
#include <vector>
#include <queue>

namespace grumpy
{
    enum ParserState : int
    {
        PARSER_STATE_WAITING,
        PARSER_STATE_MOVING
    };

    class Parser : public puddi::DrawableObject
    {
    public:
        Parser(const Object* par, ASTNode *root);

        void Update();

        float GetVelocity() const;
        void SetVelocity(float v);

        void SetTokenQueue(const std::queue<Token*> &tokenQ);

    private:
        ASTNode *astRoot;
        std::vector<ASTNode*> nodesVector;
        std::queue<Token*> tokenQueue;
        int currentNodeIndex;
        float velocity;
        ParserState state;

        void createNodesVector();
        void addToNodesVectorRecursive(ASTNode *node);
    };
}

#endif
