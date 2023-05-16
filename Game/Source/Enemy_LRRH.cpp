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


Enemy_LRRH::Enemy_LRRH() : Entity(EntityType::ENEMYLRRH){
	//texture = app->tex->Load("");
	name.Create("enemy_lrrh");
	namechar.Create("---RedHoody---");

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
	getCloserChooser->SetNodeName("ENEMYLRRH: getCloserChooser");

	moveAwayChooser = std::make_shared<RandomUniformDistribution>(2);
	moveAwayChooser->AddChild(moveAway);
	moveAwayChooser->AddChild(jumpMoveAway);
	moveAwayChooser->SetNodeName("ENEMYLRRH: moveAwayChooser");

	inRangeChecker = std::make_shared<SwitchConditionNode>(shoot, getCloserChooser);
	healthChecker = std::make_shared<SwitchConditionNode>(moveAwayChooser, shoot);

	isEnemyTooClose = std::make_shared<SwitchConditionNode>(healthChecker, inRangeChecker);

	//Behavior Tree Node
	behaviorTree = std::make_unique<BehaviorTree>(isEnemyTooClose);
	isAlive = true;
}

bool Enemy_LRRH::Awake()
{
	id = 4;
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

	idleAnim.PushBack({ 0, 0, 140, 140 });
	idleAnim.loop = true;

	takedmgAnim.PushBack({ 0, 150, 140, 140 });
	takedmgAnim.PushBack({ 0, 150, 140, 140 });
	takedmgAnim.PushBack({ 0, 150, 140, 140 });
	takedmgAnim.PushBack({ 0, 150, 140, 140 });
	takedmgAnim.loop = false;
	takedmgAnim.speed = 0.10f;

	for (int i = 0; i < 10; i++) //penutlima:cabezon
	{
		walkDownAnim.PushBack({ (i * 150), 150, 150, 150 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkUpAnim.PushBack({ (i * 150), 600, 150, 150 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkRightAnim.PushBack({ (i * 150), 450, 150, 150 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkLeftAnim.PushBack({ (i * 150), 300, 150, 150 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.15f;


	texture = app->tex->Load("Assets/Characters/F_sprites_lrrh.png");

	currentAnimation = &idleAnim;
	return true;
}

bool Enemy_LRRH::Start()
{
	abilityFx = app->audio->LoadFx("Assets/Sounds/FX/fx_arrows.wav");
	return true;
}

bool Enemy_LRRH::Update(float dt)
{
	currentAnimation->Update();
	return true;
}

bool Enemy_LRRH::PostUpdate()
{
	//Render
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 13, position.y - 35, &rect);
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