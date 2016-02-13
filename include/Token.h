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
    };

    class Token : public puddi::DrawableObject
    {
    public:
        Token(const puddi::Object *par, const LexToken &lTok);

    private:
        LexToken lToken;
    };
}

#endif
