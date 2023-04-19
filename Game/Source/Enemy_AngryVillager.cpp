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
#include "SceneBattle.h"


Enemy_AngryVillager::Enemy_AngryVillager() : Entity(EntityType::ANGRYVILLAGER) {

	name.Create("enemy_angryVillager");
	isAlive = true;
	battleState = IDLE;
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
Enemy_AngryVillager::~Enemy_AngryVillager() {

}
bool Enemy_AngryVillager::Awake()
{
	id = 3;
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	level = stats.attribute("level").as_int();
	health = stats.attribute("health").as_int();
	maxHealth = stats.attribute("maxHealth").as_int();
	defense = stats.attribute("defense").as_int();
	magic = stats.attribute("magic").as_int();
	stamina = stats.attribute("stamina").as_int();
	maxStamina = stats.attribute("maxStamina").as_int();
	speed = stats.attribute("speed").as_int();
	attack = stats.attribute("attack").as_int();
	AttArea = stats.attribute("AttArea").as_int();
	Ab1Type = stats.attribute("Ab1Type").as_int();
	Ab1Area = stats.attribute("Ab1Area").as_int();
	Ab1RangeType = stats.attribute("Ab1RangeType").as_int();
	Ab1Power = stats.attribute("Ab1Power").as_int();
	Ab2Type = stats.attribute("Ab2Type").as_int();
	Ab2Area = stats.attribute("Ab2Area").as_int();
	Ab2RangeType = stats.attribute("Ab2RangeType").as_int();
	Ab2Power = stats.attribute("Ab2Power").as_int();
	healingpower = stats.attribute("healingpower").as_int();
	movement = stats.attribute("movement").as_int();
	isEnemy = true;

	return true;
}

bool Enemy_AngryVillager::Start()
{
	return true;
}

bool Enemy_AngryVillager::Update(float dt)
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