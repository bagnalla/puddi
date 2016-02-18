#include "SyntaxParser.h"
#include "FpsTracker.h"
#include "Util.h"
#include <iostream>
#include <algorithm>

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
        homePosition = vec4(0.0f, 0.0f, 0.0f, 1.0f);

        createNodesVector();
    }

    void SyntaxParser::Update()
    {
        if (state == SYNTAXPARSER_STATE_DONE)
        {
            if (position == homePosition)
                return;

            float moveAmount = velocity * FpsTracker::GetFrameTimeMs();

			MoveToPoint(homePosition, moveAmount, nullptr);

            return;
        }

		if (currentNodeIndex >= nodesVector.size())
		{
			state = SYNTAXPARSER_STATE_DONE;
			return;
		}

        ASTNode *targetNode = nodesVector[currentNodeIndex];

        if (state == SYNTAXPARSER_STATE_WAITING)
        {
            //targetNode->Show();
			//astRoot->Resize();
            state = SYNTAXPARSER_STATE_MOVING;
        }
        else if (state == SYNTAXPARSER_STATE_MOVING)
        {
//            if (targetNode->GetParent() != nullptr && targetNode->GetParent()->GetHidden())
//            {
//                nodesVector[currentNodeIndex]->Show();
//				astRoot->Resize();
//				currentNodeIndex++;
//				return;
//            }

			vec4 targetPosition;
			if (targetNode->GetParentConnector() != nullptr)
                targetPosition = targetNode->GetParentConnector()->GetWorldPosition();
			else
				targetPosition = targetNode->GetWorldPosition();

            float moveAmount = velocity * FpsTracker::GetFrameTimeMs();

			MoveToPoint(targetPosition, moveAmount, [&]()
			{
				nodesVector[currentNodeIndex]->Show();
				astRoot->Resize();
				currentNodeIndex++;
			});
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

	void SyntaxParser::SetHomePosition(glm::vec4 v)
	{
        homePosition = v;
	}

    // PRIVATE

    void SyntaxParser::createNodesVector()
    {
        nodesVector.clear();

        addToNodesVectorRecursive(astRoot);

        sort(nodesVector.begin(), nodesVector.end(),
        [](ASTNode *a, ASTNode *b) -> bool
        {
            return a->GetParseIndex() < b->GetParseIndex();
        });

        for (int i = 0; i < nodesVector.size(); ++i)
        {
            nodesVector[i]->Hide();
        }

        for (int i = 0; i < nodesVector.size(); ++i)
        {
            nodesVector[i]->SetNodeColor(Util::InterpolateRainbow(i / static_cast<float>(nodesVector.size()), 0.25f));
        }
    }

    void SyntaxParser::addToNodesVectorRecursive(ASTNode *node)
    {
        nodesVector.push_back(node);
        for (auto it = node->ChildNodes.begin(); it != node->ChildNodes.end(); ++it)
            addToNodesVectorRecursive(*it);
    }
}
