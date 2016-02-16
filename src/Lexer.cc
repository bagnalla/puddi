#include "Lexer.h"
#include "SourceCode.h"
#include "GlmHeaders.h"
#include "FpsTracker.h"
#include "Rectangle.h"
#include "Cube.h"
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
        currentTokenStartPos = vec4();
        skipVelocity = 0.0f;
        readVelocity = 0.0f;
        state = LEXER_STATE_SKIPPING;

        scanBarColor = vec4(1.0f, 1.0f, 0.0f, 0.0f);

        scanBar = new Cube(this);
        scanBar->RotateX(M_PI / 2.0f);
        scanBar->SetEmissive(true);
        scanBar->SetEmissionColor(scanBarColor);
        scanBar->Scale(1.0f);
        scanBar->SetRenderGraph(2);
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
            //vec4 targetPosition = targetGlyph->GetPosition();

            float moveAmount = skipVelocity * FpsTracker::GetFrameTimeMs();

            vec4 displacement = targetPosition - position;
            if (length(displacement) <= moveAmount)
            {
                SetPosition(targetPosition);

                if (currentCharacterIndex == currentToken.start)
                {
                    cout << "reached token" << currentTokenIndex << " at pos " << currentCharacterIndex << ". entering read state until pos " << currentToken.end << endl;
                    currentTokenStartPos = targetPosition;
                    scanBarColor.w = 0.5f;
                    scanBar->SetEmissionColor(scanBarColor);
                    state = LEXER_STATE_READING;
                }
                else
                {
                    sourceCode->glyphs[currentCharacterIndex]->Cull();
                    currentCharacterIndex++;
                }
            }
            else
            {
                Translate(moveAmount * glm::normalize(displacement));
            }
		}
		else if (state == LEXER_STATE_READING)
		{
            //update scan bar size/position
            vec4 displacement = currentTokenStartPos - position;
            scanBar->SetScaleX(length(displacement));
            scanBar->SetPosition(vec4(-scanBar->GetScaleX() / 2.0f, 0.0f, 0.0f, 1.0f));

            DrawableObject *targetGlyph = sourceCode->glyphs[currentToken.end];

            vec4 targetPosition = targetGlyph->GetPosition() + vec4(targetGlyph->GetScaleX() / 2.0f, 0.0f, 0.0f, 0.0f);
            //vec4 targetPosition = targetGlyph->GetPosition();

            float moveAmount = readVelocity * FpsTracker::GetFrameTimeMs();

            if (length(targetPosition - position) <= moveAmount)
            {
                SetPosition(targetPosition);

                cout << "reached end of token" << currentTokenIndex << ". entering skip state" << endl;
                scanBarColor.w = 0.0f;
                scanBar->SetEmissionColor(scanBarColor);
                scanBar->SetScaleX(0.0f);
                scanBar->SetPosition(vec4());
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

    float Lexer::GetSkipVelocity() const
    {
        return skipVelocity;
    }
    void Lexer::SetSkipVelocity(float v)
    {
        skipVelocity = v;
    }

    float Lexer::GetReadVelocity() const
    {
        return readVelocity;
    }
    void Lexer::SetReadVelocity(float v)
    {
        readVelocity = v;
    }

    // PRIVATE
}
