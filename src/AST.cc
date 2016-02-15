#include "AST.h"
#include "Texture.h"
#include "Cube.h"
#include <iostream>
#include <string>

using namespace puddi;
using namespace std;

namespace grumpy
{
	// PUBLIC

	ASTNode::ASTNode(const Object *par, DrawableObject *connector, queue<char> &input) : DrawableObject(par)
	{
        if (!input.size() || input.front() != '[')
        {
            cerr << "ASTNode constructor: expected '[' in first character of input.\n";
            return;
        }
        input.pop();

        parentConnector = connector;
        int childrenWidthSum = 0;

        container = new Object(this);
        int glyphCount = 0;

        while (true)
        {
            char c = input.front();

            if (c == '\t' || c == '\r' || c == '\n')
            {
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
                conn->Scale(1.1f);
                conn->SetScaleX(2.1f);
                conn->RotateX(M_PI / 2.0f);
                conn->Translate(vec4(glyphCount + 1.0f, 0.0f, 0.0f, 0.0f));
                conn->SetEmissive(true);
                conn->SetEmissionColor(vec4(1.0f, 1.0f, 1.0f, 0.5f));
                conn->SetRenderGraph(2);

                glyphCount += 2;

                auto node = new ASTNode(container, conn, input);
                //node->Translate(vec4(0.0f, 0.0f, -1.0f, 0.0f));
                //node->Translate(conn->GetPosition() + vec4(0.0f, 0.0f, -1.0f, 0.0f));
                childNodes.push_back(node);
                childrenWidthSum += node->width;
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

        width = std::max(childrenWidthSum, glyphCount + 1);

        //if (width == glyphCount)
        //cout << childrenWidthSum << " " << glyphCount << endl;

        // position children
        //vec4 cursor = vec4(-childrenWidthSum / 2.0f, 0.0f, -1.0f, 1.0f);
        //vec4 cursor = vec4(-width / 4.0f, 0.0f, -10.0f, 1.0f);
        vec4 cursor = vec4(glyphCount / 2.0f - childrenWidthSum / 2.0f, 0.0f, -3.0f, 1.0f);
        //for (auto it = childNodes.begin(); it != childNodes.end(); ++it)
        if (childNodes.size())
            cursor.x += childNodes[0]->width / 2.0f;
        for (int i = 0; i < childNodes.size(); ++i)
        {
            auto child = childNodes[i];
            child->SetPosition(cursor);
            cursor.x += child->width / 2.0f;
            if (i < childNodes.size() - 1)
                cursor.x += childNodes[i + 1]->width / 2.0f;
        }

        // create connector lines
        for (auto it = childNodes.begin(); it != childNodes.end(); ++it)
        {
            auto child = *it;
            child->parentConnectorLine = new Cube(container);
            child->parentConnectorLine->Scale(0.1f);
            child->parentConnectorLine->SetScaleX(length(child->position - child->parentConnector->GetPosition()));
            child->parentConnectorLine->SetPosition((child->position + child->parentConnector->GetPosition()) / 2.0f);
            float theta = atan2(child->parentConnector->GetPosition().z - child->position.z, child->parentConnector->GetPosition().x - child->position.x);
            child->parentConnectorLine->SetRotationY(theta);
            //child->parentConnectorLine->RotateX(M_PI / 2.0f);
            child->parentConnectorLine->SetEmissive(true);
            child->parentConnectorLine->SetEmissionColor(vec4(0.5f, 0.0f, 0.25f, 0.4f));
            child->parentConnectorLine->SetRenderGraph(1);
        }

        // create body
        auto body = new Cube(this);
        //body->Translate(vec4(-0.5f, 0.0f, 0.0f, 0.0f));
        body->SetScaleX(glyphCount);
        body->SetEmissive(true);
        body->SetEmissionColor(vec4(0.25f, 0.25f, 0.25f, 0.25f));
        body->SetRenderGraph(3);
	}

	int ASTNode::GetWidth() const
	{
        return width;
	}

	// PRIVATE
}
