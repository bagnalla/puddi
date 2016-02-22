#include "Token.h"
#include "SyntaxParser.h"
#include "FpsTracker.h"
#include "Schematic.h"
#include <iostream>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // LexToken PUBLIC

    LexToken::LexToken() : LexToken(0, "", 0, 0, "") {}

    LexToken::LexToken(int num, const std::string &n, int s, int e, const std::string &v)
    {
        number = num;
        name = n;
        start = s;
        end = e;
        value = v;
    }

    // Token PUBLIC

    Token::Token(Object *par, const LexToken &lTok, SyntaxParser *pars) : DrawableObject(par)
    {
        init(lTok, pars);
    }

    Token::Token(Object *par, const LexToken &lTok, SyntaxParser *pars, SchematicNode *schematic) : DrawableObject(par, schematic)
    {
        init(lTok, pars);
    }

    Token* Token::GetNext() const
    {
        return next;
    }
    void Token::SetNext(Token *t)
    {
        next = t;
    }

    Token* Token::GetPrevious() const
    {
        return previous;
    }
    void Token::SetPrevious(Token *t)
    {
        previous = t;
    }

    void Token::SetVelocity(float v)
    {
        velocity = v;
    }

    void Token::Consume()
    {
        if (previous != nullptr)
            previous->SetNext(next);
        if (next != nullptr)
            next->SetPrevious(previous);
    }

    void Token::Update()
    {
        vec4 targetPosition;
        if (next != nullptr)
            targetPosition = next->GetTokenConnectionPoint();
        else
        {
            //targetPosition = parser->GetWorldPosition();
            targetPosition = parser->GetWorldPosition() + normalize(GetWorldPosition() - parser->GetWorldPosition()) * parser->GetScaleX();
        }

        float moveAmount = velocity * FpsTracker::GetFrameTimeMs();

        vec4 displacement = targetPosition - position;
        if (length(displacement) <= moveAmount)
        {
            SetPosition(targetPosition);

            if (!addedToParser)
            {
                addedToParser = true;

                parser->AddToken(this);
            }
        }
        else
        {
            if (next != nullptr)
                LookAt(targetPosition);
            else
                LookAt(parser->GetWorldPosition());

            Translate(normalize(displacement) * moveAmount);
        }
    }

    vec4 Token::GetTokenConnectionPoint() const
    {
        return parentModel * model * vec4(-scale.x / 2.0f, 0.0f, 0.0f, 1.0f);
    }

    // PRIVATE

    void Token::init(const LexToken &lTok, SyntaxParser *pars)
    {
        LToken = lTok;
        parser = pars;
        next = nullptr;
        previous = nullptr;
        velocity = 0.0f;
        addedToParser = false;
    }
}
