#include "Lexer.h"
#include "SourceCode.h"
#include "GlmHeaders.h"
#include "FpsTracker.h"
#include <iostream>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // PUBLIC

    Lexer::Lexer(const Object* par, SourceCode *code, const vector<LexToken> &lToks) : DrawableObject(par)
    {
        sourceCode = code;
        lTokens = lToks;
        currentCharacterIndex = 0;
        currentTokenIndex = 0;
        skipVelocity = 0.0f;
        readVelocity = 0.0f;
        state = LEXER_STATE_SKIPPING;
    }

    void Lexer::Update()
    {
        if (currentCharacterIndex >= sourceCode->characters.size() || currentTokenIndex >= lTokens.size())
                return;

        // get current token in token stream
        LexToken currentToken = lTokens[currentTokenIndex];

        if (state == LEXER_STATE_SKIPPING)
        {
            // get current glyph in character stream
            DrawableObject *targetGlyph = sourceCode->glyphs[currentCharacterIndex];
            while (targetGlyph == nullptr)
            {
                if (++currentCharacterIndex >= sourceCode->characters.size())
                    return;

                targetGlyph = sourceCode->glyphs[currentCharacterIndex];
            }

            vec4 targetPosition = targetGlyph->GetPosition() - vec4(targetGlyph->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f);

            float moveAmount = skipVelocity * FpsTracker::GetFrameTimeMs();

            vec4 displacement = targetPosition - position;
            float len = length(displacement);
            if (len <= moveAmount)
            {
                SetPosition(targetPosition);

                if (currentCharacterIndex == currentToken.start)
                {
                    cout << "reached token" << currentTokenIndex << ". entering read state" << endl;
                    state = LEXER_STATE_READING;
                }

                sourceCode->glyphs[currentCharacterIndex]->Cull();
                currentCharacterIndex++;
            }
            else
            {
                Translate(moveAmount * glm::normalize(displacement));
            }
		}
		else if (state == LEXER_STATE_READING)
		{
            DrawableObject *targetGlyph = sourceCode->glyphs[currentToken.end];

            vec4 targetPosition = targetGlyph->GetPosition() + vec4(targetGlyph->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f);

            float moveAmount = readVelocity * FpsTracker::GetFrameTimeMs();

            vec4 displacement = targetPosition - position;
            float len = length(displacement);
            if (len <= moveAmount)
            {
                SetPosition(targetPosition);

                cout << "reached end of token" << currentTokenIndex << ". entering skip state" << endl;
                state = LEXER_STATE_SKIPPING;

                for (;currentCharacterIndex <= currentToken.end; currentCharacterIndex++)
                {
                    sourceCode->glyphs[currentCharacterIndex]->Cull();
                }

                currentTokenIndex++;
            }
            else
            {
                Translate(moveAmount * glm::normalize(displacement));
            }
		}
		else
		{
            std::cerr << "invalid lexer state\n";
            return;
		}
    }

    float Lexer::GetSkipVelocity()
    {
        return skipVelocity;
    }
    void Lexer::SetSkipVelocity(float v)
    {
        skipVelocity = v;
    }

    float Lexer::GetReadVelocity()
    {
        return readVelocity;
    }
    void Lexer::SetReadVelocity(float v)
    {
        readVelocity = v;
    }

    // PRIVATE
}
