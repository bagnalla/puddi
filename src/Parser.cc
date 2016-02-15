#include "Parser.h"

using namespace puddi;

namespace grumpy
{
    // PUBLIC

    Parser::Parser(const Object *par, ASTNode *root) : DrawableObject(par)
    {
        astRoot = cursor = root;
    }

    void Parser::Update()
    {
    }

    // PRIVATE
}
