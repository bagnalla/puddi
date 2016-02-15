#ifndef PARSER_H
#define PARSER_H

#include "DrawableObject.h"
#include "Token.h"
#include "AST.h"
#include <queue>

namespace grumpy
{
    class Parser : public puddi::DrawableObject
    {
    public:
        Parser(const Object* par, ASTNode *root);

        void Update();

    private:
        ASTNode *astRoot;
        ASTNode *cursor;
        std::queue<Token*> tokenQueue;
    };
}

#endif
