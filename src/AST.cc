#include "AST.h"
#include "Texture.h"
#include "Cube.h"
#include "FpsTracker.h"
#include <iostream>
#include <string>

using namespace puddi;
using namespace std;

namespace grumpy
{
	// PUBLIC

	ASTNode::ASTNode(Object *par, ASTNode *parentNode, DrawableObject *connector, queue<char> &input) : DrawableObject(par)
	{
        if (!input.size() || input.front() != '[')
        {
            cerr << "ASTNode constructor: expected '[' in first character of input.\n";
            return;
        }
        input.pop();

        parent = parentNode;
        parentConnector = connector;
		velocity = 0.01f;

        container = new DrawableObject(this);
        glyphCount = 0;
        tokenRequired = false;

        while (true)
        {
            char c = input.front();

            if (c == '\t' || c == '\r' || c == '\n')
            {
                input.pop();
                continue;
            }

            if (c == '`')
            {
                tokenRequired = true;
                input.pop();
                while (c = input.front() != '`')
                    input.pop();
                input.pop();
                continue;
            }

            cout << c;

            if (c == ']')
                break;

            if (c == '[')
            {
                //DrawableObject *conn = new Cube(glyphContainer);
                DrawableObject *conn = new Cube(container);
                conn->Scale(0.9f);
                conn->SetScaleX(1.9f);
                conn->RotateX(M_PI / 2.0f);
                conn->Translate(vec4(glyphCount + 1.0f, 0.0f, 0.0f, 0.0f));
                conn->SetEmissive(true);
                conn->SetEmissionColor(vec4(1.0f, 1.0f, 1.0f, 0.5f));
                conn->SetRenderGraph(2);

                glyphCount += 2;

                auto node = new ASTNode(container, this, conn, input);
                //node->Translate(vec4(0.0f, 0.0f, -1.0f, 0.0f));
                //node->Translate(conn->GetPosition() + vec4(0.0f, 0.0f, -1.0f, 0.0f));
                ChildNodes.push_back(node);
				node->SetPosition(conn->GetPosition());
            }
            else
            {
                if (c != ' ')
                {
                    DrawableObject *glyph = new DrawableObject(container, VertexMesh::GetVertexMeshPrototypeByName(string(1, c)));
                    glyph->SetTexture(Texture::GetTextureByName("myfont"));
                    glyph->RotateX(M_PI / 2.0f);
                    glyph->Translate(vec4(glyphCount + 0.5f, 0.0f, 0.0f, 0.0f));
                }
                glyphCount++;
            }

            input.pop();
        }

        container->Translate(vec4(-glyphCount / 2.0f, 0.0f, 0.0f, 0.0f));

        //width = std::max(resizeRecursive(), glyphCount + 1);
		//resizeRecursive();

        // create connector lines
        for (auto it = ChildNodes.begin(); it != ChildNodes.end(); ++it)
        {
            auto child = *it;
            child->parentConnectorLine = new Cube(child);
            child->parentConnectorLine->Scale(0.1f);
            child->parentConnectorLine->SetScaleX(length(child->position - child->parentConnector->GetPosition()));
            //child->parentConnectorLine->SetPosition((child->position + child->parentConnector->GetPosition()) / 2.0f);
			child->parentConnectorLine->SetPosition(vec4(-child->position.x, -child->position.y, -child->position.z, 1.0f) + (child->position + child->parentConnector->GetPosition()) / 2.0f);
			//child->parentConnectorLine->SetPosition(child->parentConnector->GetPosition() + child->position / 2.0f);
            float theta = atan2(child->parentConnector->GetPosition().z - child->position.z, child->parentConnector->GetPosition().x - child->position.x);
			//float theta = atan2(child->position.z, child->position.x);
            child->parentConnectorLine->SetRotationY(theta);
            child->parentConnectorLine->SetEmissive(true);
            child->parentConnectorLine->SetEmissionColor(vec4(0.5f, 0.0f, 0.25f, 0.4f));
            child->parentConnectorLine->SetRenderGraph(1);
        }

		// create connector line
		//if (parentConnector != NULL)
		//{
		//	parentConnectorLine = new Cube(parent->container);
		//	parentConnectorLine->Scale(0.1f);
		//	parentConnectorLine->SetScaleX(length(position - parentConnector->GetPosition()) / 2.0f);
		//	/*parentConnectorLine->SetPosition(position / 2.0f);
		//	float theta = atan2(position.z, position.x);
		//	parentConnectorLine->SetRotationY(theta);*/
		//	parentConnectorLine->SetEmissive(true);
		//	parentConnectorLine->SetEmissionColor(vec4(0.5f, 0.0f, 0.25f, 0.4f));
		//	parentConnectorLine->SetRenderGraph(1);
		//}

        // create body
        auto body = new Cube(this);
        //body->Translate(vec4(-0.5f, 0.0f, 0.0f, 0.0f));
        body->SetScaleX(glyphCount);
        body->SetEmissive(true);
        if (tokenRequired)
            body->SetEmissionColor(vec4(0.3f, 0.15f, 0.0f, 0.4f));
        else
            body->SetEmissionColor(vec4(0.25f, 0.25f, 0.25f, 0.25f));
        body->SetRenderGraph(3);
	}

	int ASTNode::GetWidth() const
	{
        return width;
	}

	bool ASTNode::GetTokenRequired() const
	{
        return tokenRequired;
	}

    void ASTNode::Hide()
    {
        DisableRender();
    }

    void ASTNode::Show()
    {
        EnableRender();
        for (auto it = ChildNodes.begin(); it != ChildNodes.end(); ++it)
            (*it)->Hide();
    }

	void ASTNode::Resize()
	{
		resizeRecursive();
	}

	void ASTNode::Update()
	{
		if (position == assignedLocation || !GetRenderEnabled())
			return;

		float moveAmount = velocity * FpsTracker::GetFrameTimeMs();

		vec4 displacement = assignedLocation - position;
		if (length(displacement) <= moveAmount)
		{
			SetPosition(assignedLocation);
		}
		else
		{
			Translate(moveAmount * glm::normalize(displacement));
		}

		rePositionConnectorLine();
	}

	void ASTNode::SetAssignedLocation(const glm::vec4 &l)
	{
		assignedLocation = l;
	}

	// PRIVATE

	int ASTNode::resizeRecursive()
	{
		if (!GetRenderEnabled())
			return 0;

		int childrenWidthSum = 0;

		for (auto it = ChildNodes.begin(); it != ChildNodes.end(); ++it)
			childrenWidthSum += (*it)->resizeRecursive(); // recursive call

		vec4 cursor = vec4(glyphCount / 2.0f - childrenWidthSum / 2.0f, 0.0f, -3.0f, 1.0f);
		if (ChildNodes.size())
			cursor.x += ChildNodes[0]->width / 2.0f;
		for (int i = 0; i < ChildNodes.size(); ++i)
		{
			auto child = ChildNodes[i];
			child->assignedLocation = cursor;
			//child->SetPosition(cursor);
			//child->rePositionConnectorLine();
			cursor.x += child->width / 2.0f;
			if (i < ChildNodes.size() - 1)
				cursor.x += ChildNodes[i + 1]->width / 2.0f;
		}

		return width = std::max(childrenWidthSum, glyphCount + 1);
	}

	void ASTNode::rePositionConnectorLine()
	{
		if (parentConnectorLine == NULL)
			return;

		parentConnectorLine->SetScaleX(length(position - parentConnector->GetPosition()));
		parentConnectorLine->SetPosition(vec4(-position.x, -position.y, -position.z, 1.0f) + (position + parentConnector->GetPosition()) / 2.0f);
		float theta = atan2(parentConnector->GetPosition().z - position.z, parentConnector->GetPosition().x - position.x);
		parentConnectorLine->SetRotationY(theta);
	}
}
