#pragma once
#include "Entity.h"
#include "Pathfinding.h"
#include "UriBehaviorTree.h"
#include "Animation.h"
class Enemy_CorruptedSprout : public Entity {
public:
	Enemy_CorruptedSprout();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;


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

public:

	Animation* currentAnimation;
	Animation walkDownAnim;
	Animation walkUpAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation idleAnim;
	Animation takedmgAnim;

//Nodes
private:

	//Action Nodes
	class GetCloser : public BehaviorTreeNode
	{
	public:
		NodeStatus Run() override;
	};


	class ThornyVeins : public BehaviorTreeNode
	{
	public:
		NodeStatus Run() override;
	};


	class CloudOfSpores : public BehaviorTreeNode
	{
	public:
		NodeStatus Run() override;
	};


	//Control Flow Nodes
	std::shared_ptr<RandomWeightedDistribution> attackChooser;
	std::shared_ptr<SwitchConditionNode> inRangeChecker;


	//Behavior Tree Node
	std::unique_ptr<BehaviorTree> behaviorTree;



};