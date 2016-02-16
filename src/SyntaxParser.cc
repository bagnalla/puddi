#include "SyntaxParser.h"
#include "FpsTracker.h"
#include <iostream>

using namespace puddi;
using namespace std;

namespace grumpy
{
    // PUBLIC

	SyntaxParser::SyntaxParser(Object *par, ASTNode *root) : DrawableObject(par)
    {
        astRoot = root;
        currentNodeIndex = 0;
        velocity = 0.0f;
        state = SYNTAXPARSER_STATE_WAITING;

        createNodesVector();
    }

    void SyntaxParser::Update()
    {
        if (currentNodeIndex >= nodesVector.size())
            return;

        ASTNode *targetNode = nodesVector[currentNodeIndex];

        if (state == SYNTAXPARSER_STATE_WAITING)
        {
            if (tokenQueue.size())
            {
                targetNode->Show();
				astRoot->Resize();
                tokenQueue.pop();
                state = SYNTAXPARSER_STATE_MOVING;
            }
        }
        else
        {
            vec4 targetPosition = targetNode->GetWorldPosition();

            float moveAmount = velocity * FpsTracker::GetFrameTimeMs();

            vec4 displacement = targetPosition - position;
            if (length(displacement) <= moveAmount)
            {
                SetPosition(targetPosition);

                currentNodeIndex++;

                if (currentNodeIndex >= nodesVector.size() || (nodesVector[currentNodeIndex]->GetTokenRequired() && !tokenQueue.size()))
                    state = SYNTAXPARSER_STATE_WAITING;
				else
				{
					nodesVector[currentNodeIndex]->Show();
					astRoot->Resize();
				}
            }
            else
            {
                Translate(moveAmount * glm::normalize(displacement));
            }
        }
    }

    float SyntaxParser::GetVelocity() const
    {
        return velocity;
    }
    void SyntaxParser::SetVelocity(float v)
    {
        velocity = v;
    }

    void SyntaxParser::SetTokenQueue(const queue<Token*> &tokenQ)
    {
        tokenQueue = tokenQ;
    }

	void SyntaxParser::AddTokenToQueue(Token *t)
	{
		tokenQueue.push(t);
	}

    // PRIVATE

    void SyntaxParser::createNodesVector()
    {
        nodesVector.clear();

        addToNodesVectorRecursive(astRoot);
    }

    void SyntaxParser::addToNodesVectorRecursive(ASTNode *node)
    {
        nodesVector.push_back(node);
        for (auto it = node->ChildNodes.begin(); it != node->ChildNodes.end(); ++it)
            addToNodesVectorRecursive(*it);
    }
}
