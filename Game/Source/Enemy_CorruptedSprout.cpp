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

Enemy_CorruptedSprout::Enemy_CorruptedSprout() : Entity(EntityType::CORRUPTEDSPROUT) {
	//texture = app->tex->Load("");
	name.Create("sprout");
	namechar.Create("---Sprout---");

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
	isAlive = true;

}

bool Enemy_CorruptedSprout::Awake()
{
	id = 5;
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	 
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

	idleAnim.PushBack({ 0, 0, 140, 140 });
	idleAnim.loop = true;
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.loop = false;
	takedmgAnim.speed = 0.20f;

	for (int i = 0; i < 4; i++) //penutlima:cabezon
	{
		walkDownAnim.PushBack({ (3 * 150), 150, 150, 150 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.pingpong = true;
	walkDownAnim.speed = 0.20f;

	for (int i = 0; i < 4; i++)
	{
		walkUpAnim.PushBack({ (3 * 150), 150, 150, 150 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.pingpong = true;
	walkUpAnim.speed = 0.20f;

	for (int i = 0; i < 4; i++)
	{
		walkRightAnim.PushBack({ (3 * 150), 150, 150, 150 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.pingpong = true;
	walkRightAnim.speed = 0.20f;

	for (int i = 0; i < 4; i++)
	{
		walkLeftAnim.PushBack({ (3 * 150), 150, 150, 150 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.pingpong = true;
	walkLeftAnim.speed = 0.20f;

	texture = app->tex->Load("Assets/Characters/F_sprites_esbirro.png");

	currentAnimation = &idleAnim;
	return true;
}

bool Enemy_CorruptedSprout::Start()
{
	return true;
}

bool Enemy_CorruptedSprout::Update(float dt)
{
	currentAnimation->Update();
	return true;
}

bool Enemy_CorruptedSprout::PostUpdate()
{
	//Render
	if (health > 0) {
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 13, position.y - 35, &rect);
	}
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