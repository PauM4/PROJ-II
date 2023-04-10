#include "Enemy_AngryVillager.h"

#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"


Enemy_AngryVillager::Enemy_AngryVillager(iPoint pos) : Entity(EntityType::ANGRYVILLAGER) {
	//texture = app->tex->Load("");

	level = 6;
	health = 25;
	maxHealth = 25;
	attack = 13;
	defense = 7;
	magic = 0;
	resistance = 4;
	speed = 13;
	skill = 15;
	stamina = 15;
	maxStamina = 15;


	//Behavior Tree
	//Action Nodes
	auto getCloser = std::make_shared<GetCloser>();
	auto shoot = std::make_shared<Shoot>();
	auto moveAway = std::make_shared<MoveAway>();

	//Control Flow Nodes
	inRangeChecker = std::make_shared<SwitchConditionNode>(shoot, getCloser);
	inRangeChecker->SetNodeName("AngryVillager: inRangeChecker");

	isEnemyTooClose = std::make_shared<SwitchConditionNode>(moveAway, inRangeChecker);
	isEnemyTooClose->SetNodeName("AngryVillager: isEnemyTooClose");

	//Behavior Tree Node
	behaviorTree = std::make_unique<BehaviorTree>(isEnemyTooClose);
	behaviorTree->SetDebugEnabled(true);

}

bool Enemy_AngryVillager::Awake()
{
	return true;
}

bool Enemy_AngryVillager::Start()
{
	return true;
}

bool Enemy_AngryVillager::Update()
{
	return true;
}

bool Enemy_AngryVillager::PostUpdate()
{
	//Render
	return true;
}

bool Enemy_AngryVillager::CleanUp()
{
	return true;
}

bool Enemy_AngryVillager::LoadState(pugi::xml_node&)
{
	return true;
}

bool Enemy_AngryVillager::SaveState(pugi::xml_node&)
{
	return true;
}

void Enemy_AngryVillager::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

int Enemy_AngryVillager::GetState()
{
	return 0;
}

int Enemy_AngryVillager::Attack()
{
	return 0;
}

int Enemy_AngryVillager::Ability(int id)
{
	return 0;
}

void Enemy_AngryVillager::Movement()
{

}


NodeStatus Enemy_AngryVillager::GetCloser::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

NodeStatus Enemy_AngryVillager::Shoot::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

NodeStatus Enemy_AngryVillager::MoveAway::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}