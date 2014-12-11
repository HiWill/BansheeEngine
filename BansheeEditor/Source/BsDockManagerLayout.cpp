#include "BsDockManagerLayout.h"
#include "BsDockManagerLayoutRTTI.h"
#include "BsEditorWidgetManager.h"

namespace BansheeEngine
{
	DockManagerLayout::Entry::Entry()
		:isLeaf(true), splitPosition(0), horizontalSplit(false),
		parent(nullptr)
	{
		children[0] = nullptr;
		children[1] = nullptr;
	}

	DockManagerLayout::Entry::~Entry()
	{ }

	DockManagerLayout::Entry* DockManagerLayout::Entry::createLeaf(Entry* parent, UINT32 childIdx, 
		const Vector<String>& widgetNames)
	{
		Entry* newEntry = bs_new<Entry>();
		newEntry->isLeaf = true;
		newEntry->parent = parent;

		if(parent != nullptr)
			parent->children[childIdx] = newEntry;

		newEntry->widgetNames = widgetNames;

		return newEntry;
	}

	DockManagerLayout::Entry* DockManagerLayout::Entry::createContainer(Entry* parent, UINT32 childIdx, 
		float splitPosition, bool horizontalSplit)
	{
		Entry* newEntry = bs_new<Entry>();
		newEntry->isLeaf = false;
		newEntry->parent = parent;

		if(parent != nullptr)
			parent->children[childIdx] = newEntry;

		newEntry->horizontalSplit = horizontalSplit;
		newEntry->splitPosition = splitPosition;

		return newEntry;
	}

	DockManagerLayout::~DockManagerLayout()
	{
		Stack<Entry*> todo;
		if(!mRootEntry.isLeaf)
		{
			todo.push(mRootEntry.children[0]);
			todo.push(mRootEntry.children[1]);
		}

		while(!todo.empty())
		{
			Entry* current = todo.top();
			todo.pop();

			if(!current->isLeaf)
			{
				todo.push(current->children[0]);
				todo.push(current->children[1]);
			}

			bs_delete(current);
		}
	}

	void DockManagerLayout::pruneInvalidLeaves()
	{
		Stack<Entry*> layoutTodo;
		layoutTodo.push(&mRootEntry);

		while (!layoutTodo.empty())
		{
			Entry* curEntry = layoutTodo.top();
			layoutTodo.pop();

			if (!curEntry->isLeaf)
			{
				layoutTodo.push(curEntry->children[0]);
				layoutTodo.push(curEntry->children[1]);
			}
			else
			{
				for (INT32 i = 0; i < (INT32)curEntry->widgetNames.size(); i++)
				{
					if (!EditorWidgetManager::instance().isValidWidget(curEntry->widgetNames[i]))
					{
						curEntry->widgetNames.erase(curEntry->widgetNames.begin() + i);
						i--;
					}
				}

				if (curEntry->widgetNames.size() == 0)
				{
					Entry* parent = curEntry->parent;
					if (parent != nullptr)
					{
						Entry* newParent = parent->parent;
						Entry* otherChild = parent->children[0] == curEntry ? parent->children[1] : parent->children[0];

						if (newParent != nullptr)
						{
							if (newParent->children[0] == parent)
								newParent->children[0] = otherChild;
							else
								newParent->children[1] = otherChild;
						}

						otherChild->parent = newParent;

						bs_delete(parent);
						bs_delete(curEntry);
					}
				}
			}
		}
	}

	/************************************************************************/
	/* 								RTTI		                     		*/
	/************************************************************************/

	RTTITypeBase* DockManagerLayout::getRTTIStatic()
	{
		return DockManagerLayoutRTTI::instance();
	}

	RTTITypeBase* DockManagerLayout::getRTTI() const
	{
		return DockManagerLayout::getRTTIStatic();
	}
}