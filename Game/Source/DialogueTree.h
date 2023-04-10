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

			for (auto &child : children)
			{
				dialogue.push_back(child->GetText());
			}
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
			}
		}

		return dialogue;
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

	void DesactivateTree()
	{
		rootNode->DesactivateAll();
	}

private:
	std::shared_ptr<DialogueNode> rootNode;
};



#endif



//auto raiz = std::make_shared<DialogueNode>(true, "hola, como te llamas?");
//
//auto uno = std::make_shared<DialogueNode>(false, "me llamo A");
//auto dos = std::make_shared<DialogueNode>(false, "me llamo B");
//auto tres = std::make_shared<DialogueNode>(false, "me llamo C");
//auto cuatro = std::make_shared<DialogueNode>(false, "me llamo D");
//auto cinco = std::make_shared<DialogueNode>(false, "me llamo CINCO");
//auto seis = std::make_shared<DialogueNode>(false, "me llamo SEIS");
//
//raiz->AddChild(uno);
//raiz->AddChild(dos);
//raiz->AddChild(tres);
//raiz->AddChild(cuatro);
//
//uno->AddChild(cinco);
//uno->AddChild(seis);
//
//auto tree = std::make_shared<DialogueTree>(raiz);
//
//auto imprimir = tree->Run();
//
//for (auto& a : imprimir)
//{
//	std::cout << a << std::endl;
//}
//
//std::cout << "SEGUNDA PARTE" << std::endl;
//tree->DesactivateTree();
//uno->ActivateNode();
//imprimir = tree->Run();
//
//for (auto& a : imprimir)
//{
//	std::cout << a << std::endl;
//}