#include "UpdateGraph.h"
#include "Object.h"
#include <algorithm>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <iostream>

namespace puddi
{
	// PUBLIC

	UpdateNode::UpdateNode(UpdateNode *par, Object *o)
	{
		parent = par;
		object = o;
		parallel = false;

		if (parent != NULL)
			parent->AddChild(this);
	}

	UpdateNode::~UpdateNode()
	{
		for (auto it = children.begin(); it != children.end(); ++it)
			delete *it;

		if (parent != NULL)
			parent->RemoveChild(this);
	}

	void UpdateNode::Update()
	{
		object->Update();

		if (parallel)
		{
			// update children in parallel
			//concurrency::parallel_for_each(begin(children), end(children), [&](UpdateNode *child)
			//{
				//child->Update();
			//}, concurrency::static_partitioner());
			
			//__gnu_parallel::for_each(begin(children), end(children), [&](UpdateNode *child)
			//{
				////std::cout << "threads = " << omp_get_num_threads() << std::endl;
				//child->Update();
			//});
			
			#pragma omp parallel for
			for (int i = 0; i < children.size(); ++i)
			{
				//if (omp_get_num_threads() != 1)
				//	std::cout << "i = " << i << ", threads = " << omp_get_num_threads() << std::endl;
				children[i]->Update();
			}
		}
		else
		{
			// update children sequentially
			for (auto it = children.begin(); it != children.end(); ++it)
				(*it)->Update();
		}
	}

	void UpdateNode::AddChild(UpdateNode *child)
	{
		children.push_back(child);
	}

	void UpdateNode::RemoveChild(UpdateNode *child)
	{
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end())
			children.erase(it);
	}
}
