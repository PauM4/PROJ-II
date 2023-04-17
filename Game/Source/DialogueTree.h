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

	//Sets the nodeText member variable of the DialogueNode object to the provided text string.
	void SetText(std::string text) { nodeText = text; }

	//Returns the nodeText member variable of the DialogueNode object.
	std::string GetText() const { return nodeText; }

	//Activates the DialogueNode object by setting its isActivated member variable to true.
	void ActivateNode()
	{
		isActivated = true;
	}

	//Deactivates the DialogueNode object by setting its isActivated member variable to false.
	void DesactivateNode()
	{
		isActivated = false;
	}

	//Deactivates all child nodes of the DialogueNode object, as well as the DialogueNode object itself.
	void DesactivateAll()
	{
		//assert(this != nullptr);
		this->DesactivateNode();

		for (auto& child : children)
		{
			child->DesactivateAll();
		}

	}

	//Returns a vector of strings containing the text of activated node and his children. If the DialogueNode object is not activated, it recursively checks its children until it finds an activated node.
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

	//Returns a vector of DialogueNode objects containing activated node and his children. If the DialogueNode object is not activated, it recursively checks its children until it finds an activated node.
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

	//Updates the activation state of child nodes of the DialogueNode object based on the provided nextRoot int value. This function is used to progress the dialogue tree to the next set of child nodes.
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

	//Adds a child DialogueNode object to the DialogueNode object's children vector member variable.
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

	// Sets the root node of the DialogueTree object to the provided root std::shared_ptr to a DialogueNode object.
	void SetRoot(const std::shared_ptr<DialogueNode>& root) { rootNode = root; }

	//Returns a vector of strings containing the text of activated node and his children.
	std::vector<std::string> Run()
	{

		std::vector<std::string> dialogue = rootNode->GetDialogue();
		return dialogue;
	}

	//Returns a vector of DialogueNode objects containing activated node and his children.
	std::vector<DialogueNode> RunNodes()
	{

		std::vector<DialogueNode> nodes = rootNode->GetNodes();
		return nodes;
	}

	//Updates the activation state of child nodes of the root node of the `Dialogue
	void Update(int idx)
	{
		rootNode->UpdateNodes(idx);
	}

	//Deactivates all child nodes of the DialogueNode object, as well as the DialogueNode object itself.
	void DesactivateTree()
	{
		rootNode->DesactivateAll();
	}

private:
	std::shared_ptr<DialogueNode> rootNode;
};



#endif
