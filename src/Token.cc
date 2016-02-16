#include "Token.h"

namespace grumpy
{
    // PUBLIC

    LexToken::LexToken()
    {
        name = "";
        start = 0;
        end = 0;
        value = "";
    }

    Token::Token(Object *par, const LexToken &lTok) : DrawableObject(par)
    {
        lToken = lTok;
    }

    // PRIVATE
}
