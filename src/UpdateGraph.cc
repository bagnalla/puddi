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
		removed = false;

		if (parent != NULL)
			parent->AddChild(this);
	}

	UpdateNode::~UpdateNode()
	{
		/*for (auto it = children.begin(); it != children.end(); ++it)
			delete *it;*/
		for (size_t i = 0; i < children.size(); ++i)
			if (!children[i]->removed)
				delete children[i];
		//while (children.size())
		//	delete children[0];

		//if (parent != NULL)
		//	parent->RemoveChild(this);
		removed = true;
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
				auto child = children[i];
				//if (child != nullptr)
				if (!child->removed)
					child->Update();
			}
		}
		else
		{
			// update children sequentially
			//for (auto it = children.begin(); it != children.end(); ++it)
			for (size_t i = 0; i < children.size(); ++i)
			{
				auto child = children[i];
				//if (child != nullptr)
				if (child->object != nullptr && !child->removed)
					child->Update();
			}
		}

		// remove null children
		for (size_t i = 0; i < children.size();)
		{
			//if (children[i] == nullptr)
			if (children[i]->object == nullptr || children[i]->removed)
				children.erase(children.begin() + i);
			else
				++i;
		}
	}

	void UpdateNode::AddChild(UpdateNode *child)
	{
		children.push_back(child);
	}

	//void UpdateNode::RemoveChild(UpdateNode *child)
	//{
	//	auto it = std::find(children.begin(), children.end(), child);
	//	if (it != children.end())
	//		*it = nullptr;
	//		//children.erase(it);
	//}
}
