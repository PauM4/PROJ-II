#ifndef DIALOGUE_TREE_H
#define DIALOGUE_TREE_H

#include <iostream>
#include <memory>
#include <vector>
#include <cassert>

class DialogueNode
{
public:

	DialogueNode() { isActivated = false; };
	DialogueNode(bool status) { isActivated = status; };
	DialogueNode(bool status, std::string text) { isActivated = status, nodeText = text; };
	~DialogueNode() {};


	void SetText(std::string text) { nodeText = text; }
	std::string GetText() const { return nodeText; }

	void ActivateNode()
	{
		isActivated = true;
	}

	void DesactivateNode()
	{
		isActivated = false;
	}

	void DesactivateAll()
	{
		//assert(this != nullptr);
		this->DesactivateNode();

		for (auto& child : children)
		{
			child->DesactivateAll();
		}

	}

	std::vector<std::string> GetDialogue()
	{
		std::vector<std::string> dialogue;

		if (isActivated)
		{

			dialogue.push_back(nodeText);

			for (auto& child : children)
			{
				dialogue.push_back(child->GetText());
			}

			return dialogue;
		}
		else
		{
			for (auto& child : children)
			{
				if (child->isActivated)
				{
					dialogue = child->GetDialogue();

					break;
				}
				else
				{
					auto grandchildren = child->children;

					for (auto& childv2 : grandchildren)
					{
						dialogue = childv2->GetDialogue();

						if (!dialogue.empty())
						{
							break;
						}						
						
					}
				}
				if (!dialogue.empty())
				{
					break;
				}
			}
		}

		return dialogue;
	}


	std::vector<DialogueNode> GetNodes()
	{
		std::vector<DialogueNode> nodes;

		if (isActivated)
		{
			nodes.push_back(this);

			for (auto& child : children)
			{
				//nodes.push_back(*child.get());
				nodes.push_back(*child);
			}
			return nodes;
		}
		else
		{
			for (auto& child : children)
			{
				if (child->isActivated)
				{
					nodes = child->GetNodes();

					break;
				}
				else
				{
					auto grandchildren = child->children;

					for (auto& childv2 : grandchildren)
					{
						nodes = childv2->GetNodes();

						break;
					}
				}
			}
		}

		return nodes;
	}

	//void UpdateNodes(int nextRoot)
	//{

	//	if (isActivated)
	//	{
	//		DesactivateAll();
	//		int index = nextRoot;

	//		for (auto& child : children)
	//		{
	//			if (index == 1)
	//			{
	//				child->ActivateNode();
	//			}
	//			index--;
	//		}

	//		return;
	//	}
	//	else
	//	{
	//		for (auto& child : children)
	//		{
	//			child->UpdateNodes(nextRoot);
	//		}

	//	}
	//}

	void UpdateNodes(int nextRoot)
	{

		if (isActivated)
		{
			DesactivateAll();
			int index = nextRoot;

			for (auto& child : children)
			{
				if (index == 1)
				{
					auto aux = child->children;

					for (auto& a : aux)
					{
						a->ActivateNode();
					}
				}
				index--;
			}

			return;
		}
		else
		{
			for (auto& child : children)
			{
				child->UpdateNodes(nextRoot);
			}

		}
	}

	void AddChild(std::shared_ptr<DialogueNode> child)
	{

		assert(child != nullptr);
		if (child != nullptr)
		{
			children.push_back(child);
		}



	}

private:

	std::string nodeText;
	std::vector<std::shared_ptr<DialogueNode>> children;

private:
	bool isActivated;

};


class DialogueTree
{
public:
	DialogueTree() {};
	DialogueTree(const std::shared_ptr<DialogueNode>& root) { rootNode = root; };
	~DialogueTree() {};

	void SetRoot(const std::shared_ptr<DialogueNode>& root) { rootNode = root; }

	std::vector<std::string> Run()
	{

		std::vector<std::string> dialogue = rootNode->GetDialogue();
		return dialogue;
	}

	std::vector<DialogueNode> RunNodes()
	{

		std::vector<DialogueNode> nodes = rootNode->GetNodes();
		return nodes;
	}

	void Update(int idx)
	{
		rootNode->UpdateNodes(idx);
	}

	void DesactivateTree()
	{
		rootNode->DesactivateAll();
	}

private:
	std::shared_ptr<DialogueNode> rootNode;
};



#endif
