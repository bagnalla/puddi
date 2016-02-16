#include "Parser.h"
#include "FpsTracker.h"
#include <iostream>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // PUBLIC

    Parser::Parser(const Object *par, ASTNode *root) : DrawableObject(par)
    {
        astRoot = root;
        currentNodeIndex = 0;
        velocity = 0.0f;
        state = PARSER_STATE_WAITING;

        createNodesVector();
    }

    void Parser::Update()
    {
        if (currentNodeIndex >= nodesVector.size())
            return;

        ASTNode *targetNode = nodesVector[currentNodeIndex];

        if (state == PARSER_STATE_WAITING)
        {
            if (tokenQueue.size())
            {
                //targetNode->Show();
                tokenQueue.pop();
                state = PARSER_STATE_MOVING;
            }
        }
        else
        {
            vec4 targetPosition = targetNode->GetWorldPosition();

            float moveAmount = velocity * FpsTracker::GetFrameTimeMs();

            vec4 displacement = targetPosition - position;
            if (length(targetPosition - position) <= moveAmount)
            {
                SetPosition(targetPosition);

                currentNodeIndex++;

                if (currentNodeIndex < nodesVector.size() && nodesVector[currentNodeIndex]->GetTokenRequired() && !tokenQueue.size())
                    state = PARSER_STATE_WAITING;
                //else
                //    nodesVector[currentNodeIndex]->Show();
            }
            else
            {
                Translate(moveAmount * glm::normalize(displacement));
            }
        }
    }

    float Parser::GetVelocity() const
    {
        return velocity;
    }
    void Parser::SetVelocity(float v)
    {
        velocity = v;
    }

    void Parser::SetTokenQueue(const queue<Token*> &tokenQ)
    {
        tokenQueue = tokenQ;
    }

    // PRIVATE

    void Parser::createNodesVector()
    {
        nodesVector.clear();

        addToNodesVectorRecursive(astRoot);
    }

    void Parser::addToNodesVectorRecursive(ASTNode *node)
    {
        nodesVector.push_back(node);
        for (auto it = node->ChildNodes.begin(); it != node->ChildNodes.end(); ++it)
            addToNodesVectorRecursive(*it);
    }
}
