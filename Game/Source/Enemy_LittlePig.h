#pragma once
#include "Entity.h"
#include "Pathfinding.h"
#include "UriBehaviorTree.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "List.h"

class Enemy_LittlePig : public Entity {
public:
	Enemy_LittlePig();

	virtual ~Enemy_LittlePig();

	bool Awake() override;

	bool Start() override;

	bool Update(float ft) override;


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


	//Control Flow Nodes
	std::shared_ptr<SwitchConditionNode> inRangeChecker;
	std::shared_ptr<SwitchConditionNode> isEnemyTooClose;


	//Behavior Tree Node
	std::unique_ptr<BehaviorTree> behaviorTree;


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
	Animation abilityAnim;
	Animation* abilityAnimation;

	bool finishAnimBool;
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






};
