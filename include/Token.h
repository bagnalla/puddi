#ifndef TOKEN_H
#define TOKEN_H

#include "DrawableObject.h"
#include <string>

namespace grumpy
{
    struct LexToken
    {
        std::string name;
        int start;
        int end;
        std::string value;

        LexToken();
        LexToken(const std::string &name, int start, int end, const std::string &value);
    };

    class Token : public puddi::DrawableObject
    {
    public:
        Token(puddi::Object *par, const LexToken &lTok);

        LexToken LToken;

    private:
    };
}

#endif
