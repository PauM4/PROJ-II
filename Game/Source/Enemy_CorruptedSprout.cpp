#include "Enemy_CorruptedSprout.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Enemy_CorruptedSprout::Enemy_CorruptedSprout(iPoint pos) : Entity(EntityType::CORRUPTEDSPROUT) {
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
	auto thornyVeins = std::make_shared<ThornyVeins>();
	auto cloudOfSpores = std::make_shared<CloudOfSpores>();

	//Control Flow Nodes
	attackChooser = std::make_shared<RandomWeightedDistribution>();
	attackChooser->AddChild(cloudOfSpores, 0.20);
	attackChooser->AddChild(thornyVeins, 0.80);
	attackChooser->SetNodeName("Sprout: attackChooser");

	inRangeChecker = std::make_shared<SwitchConditionNode>(attackChooser, getCloser);
	inRangeChecker->SetNodeName("Sprout: inRangeChecker");

	//Behavior Tree Node
	behaviorTree = std::make_unique<BehaviorTree>(inRangeChecker);
	behaviorTree->SetDebugEnabled(true);

}

bool Enemy_CorruptedSprout::Awake()
{
	return true;
}

bool Enemy_CorruptedSprout::Start()
{
	return true;
}

bool Enemy_CorruptedSprout::Update(float dt)
{
	return true;
}

bool Enemy_CorruptedSprout::PostUpdate()
{
	//Render
	return true;
}

bool Enemy_CorruptedSprout::CleanUp()
{
	return true;
}

bool Enemy_CorruptedSprout::LoadState(pugi::xml_node&)
{
	return true;
}

bool Enemy_CorruptedSprout::SaveState(pugi::xml_node&)
{
	return true;
}

void Enemy_CorruptedSprout::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

int Enemy_CorruptedSprout::GetState()
{
	return 0;
}

int Enemy_CorruptedSprout::Attack()
{
	return 0;
}

int Enemy_CorruptedSprout::Ability(int id)
{
	return 0;
}

void Enemy_CorruptedSprout::Movement()
{

}


NodeStatus Enemy_CorruptedSprout::GetCloser::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

NodeStatus Enemy_CorruptedSprout::ThornyVeins::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

NodeStatus Enemy_CorruptedSprout::CloudOfSpores::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}