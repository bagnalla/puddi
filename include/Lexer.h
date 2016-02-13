#ifndef LEXER_H
#define LEXER_H

#include "DrawableObject.h"
#include "Token.h"
#include <vector>

namespace grumpy
{
    class SourceCode;

    enum LexerState : int
    {
        LEXER_STATE_SKIPPING,
        LEXER_STATE_READING
    };

    class Lexer : public puddi::DrawableObject
    {
    public:
        Lexer(const Object* par, SourceCode *code, const std::vector<LexToken> &lToks);

        void Update();

        float GetSkipVelocity();
        void SetSkipVelocity(float v);

        float GetReadVelocity();
        void SetReadVelocity(float v);

    private:

        SourceCode *sourceCode;
        std::vector<LexToken> lTokens;
        int currentCharacterIndex;
        int currentTokenIndex;

        float skipVelocity;
        float readVelocity;

        LexerState state;

        DrawableObject *scanBar;
    };
}

#endif
