#include "Token.h"

namespace grumpy
{
    // PUBLIC

    LexToken::LexToken() : LexToken("", 0, 0, "") {}

    LexToken::LexToken(const std::string &n, int s, int e, const std::string &v)
    {
        name = n;
        start = s;
        end = e;
        value = v;
    }

    Token::Token(Object *par, const LexToken &lTok) : DrawableObject(par)
    {
        LToken = lTok;
    }

    // PRIVATE
}
