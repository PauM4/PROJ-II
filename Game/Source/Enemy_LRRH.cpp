#include "Enemy_LRRH.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"


Enemy_LRRH::Enemy_LRRH(iPoint pos) : Entity(EntityType::LRRH){
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
	auto jumpGetCloser = std::make_shared<JumpGetCloser>();

	auto moveAway = std::make_shared<MoveAway>();
	auto jumpMoveAway = std::make_shared<JumpMoveAway>();

	auto shoot = std::make_shared<Shoot>();

	//Control Flow Nodes
	getCloserChooser = std::make_shared<RandomUniformDistribution>(2);
	getCloserChooser->AddChild(getCloser);
	getCloserChooser->AddChild(jumpGetCloser);
	getCloserChooser->SetNodeName("LRRH: getCloserChooser");

	moveAwayChooser = std::make_shared<RandomUniformDistribution>(2);
	moveAwayChooser->AddChild(moveAway);
	moveAwayChooser->AddChild(jumpMoveAway);
	moveAwayChooser->SetNodeName("LRRH: moveAwayChooser");

	inRangeChecker = std::make_shared<SwitchConditionNode>(shoot, getCloserChooser);
	healthChecker = std::make_shared<SwitchConditionNode>(moveAwayChooser, shoot);

	isEnemyTooClose = std::make_shared<SwitchConditionNode>(healthChecker, inRangeChecker);

	//Behavior Tree Node
	behaviorTree = std::make_unique<BehaviorTree>(isEnemyTooClose);
}

bool Enemy_LRRH::Awake()
{
	return true;
}

bool Enemy_LRRH::Start()
{
	return true;
}

bool Enemy_LRRH::Update(float dt)
{
	return true;
}

bool Enemy_LRRH::PostUpdate()
{
	//Render
	return true;
}

bool Enemy_LRRH::CleanUp()
{
	return true;
}

bool Enemy_LRRH::LoadState(pugi::xml_node&)
{
	return true;
}

bool Enemy_LRRH::SaveState(pugi::xml_node&)
{
	return true;
}

void Enemy_LRRH::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

int Enemy_LRRH::GetState()
{
	return 0;
}

int Enemy_LRRH::Attack()
{
	return 0;
}

int Enemy_LRRH::Ability(int id)
{
	return 0;
}

void Enemy_LRRH::Movement()
{

}


NodeStatus Enemy_LRRH::GetCloser::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

NodeStatus Enemy_LRRH::JumpGetCloser::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

NodeStatus Enemy_LRRH::Shoot::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

NodeStatus Enemy_LRRH::MoveAway::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

NodeStatus Enemy_LRRH::JumpMoveAway::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}