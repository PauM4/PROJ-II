#pragma once
#include "Entity.h"
#include "Pathfinding.h"
#include "UriBehaviorTree.h"

class Enemy_AngryVillager : public Entity {
	Enemy_AngryVillager(iPoint pos);

	bool Awake() override;

	bool Start() override;

	bool Update() override;


	bool PostUpdate() override;

	bool CleanUp() override;

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	int GetState() override;

	//In Combat Functions

	int Attack() override;

	int Ability(int id) override;

	void Movement() override;
private:
	SDL_Texture* texture;


private:

	//Action Nodes
	class GetCloser : public BehaviorTreeNode
	{
	public:
		NodeStatus Run() override;
	};


	class Shoot : public BehaviorTreeNode
	{
	public:
		NodeStatus Run() override;
	};


	class MoveAway : public BehaviorTreeNode
	{
	public:
		NodeStatus Run() override;
	};


	//Control Flow Nodes
	std::shared_ptr<SwitchConditionNode> inRangeChecker;
	std::shared_ptr<SwitchConditionNode> isEnemyTooClose;


	//Behavior Tree Node
	std::unique_ptr<BehaviorTree> behaviorTree;




};