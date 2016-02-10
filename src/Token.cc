#include "Token.h"

namespace grumpy
{
    // PUBLIC

    Token::Token(const Object *par, const LexToken &lTok) : DrawableObject(par)
    {
        lToken = lTok;
    }

    // PRIVATE
}
