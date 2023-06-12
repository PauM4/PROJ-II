#include "Enemy_MiddlePig.h"
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
#include "BattleManager.h"


Enemy_MiddlePig::Enemy_MiddlePig() : Entity(EntityType::ENEMYMPIG) {

	name.Create("enemy_middlePig");
	namechar.Create("Middle Pig");
	isAlive = true;

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
Enemy_MiddlePig::~Enemy_MiddlePig() {

}
bool Enemy_MiddlePig::Awake()
{
	id = 3;
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

	idleAnim.PushBack({ 0, 0, 210, 210 });
	idleAnim.loop = true;

	takedmgAnim.PushBack({ 225, 0, 210, 210 });
	takedmgAnim.PushBack({ 225, 0, 210, 210 });
	takedmgAnim.PushBack({ 225, 0, 210, 210 });
	takedmgAnim.PushBack({ 225, 0, 210, 210 });
	takedmgAnim.loop = false;
	takedmgAnim.speed = 0.20f;

	abilityAnim.PushBack({ 450, 0, 225, 225 });
	abilityAnim.PushBack({ 675, 0, 210, 225 });
	abilityAnim.loop = false;
	abilityAnim.speed = 0.1;

	for (int i = 0; i < 10; i++) //penutlima:cabezon
	{
		walkDownAnim.PushBack({ (i * 225), 225, 225, 225 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkUpAnim.PushBack({ (i * 225), 900, 225, 225 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkRightAnim.PushBack({ (i * 225), 600, 225, 225 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkLeftAnim.PushBack({ (i * 225), 450, 225, 225 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.15f;

	texture = app->tex->Load("Assets/Characters/Sprites_Cerdo_Mediano.png");
	IconGtexture = app->tex->Load("Assets/UI/MiddlePigIconG.png");
	Icontexture = app->tex->Load("Assets/UI/MiddlePigIcon.png");
	currentAnimation = &idleAnim;


	return true;
}

bool Enemy_MiddlePig::Start()
{
	abilityFx = app->audio->LoadFx("Assets/Sounds/FX/fx_cantmove.wav");
	return true;
}

bool Enemy_MiddlePig::Update(float dt)
{
	currentAnimation->Update();
	return true;
}

bool Enemy_MiddlePig::PostUpdate()
{
	//Render
	if (health > 0) {
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 50, position.y - 125, &rect);
	}

	if (currentAnimation == &abilityAnim)
	{
		if (abilityAnim.HasFinished())
		{
			finishAnimBool = true;
			abilityAnim.Reset();
			currentAnimation = &idleAnim;

		}
	}
	return true;
}

bool Enemy_MiddlePig::CleanUp()
{
	return true;
}
	
bool Enemy_MiddlePig::LoadState(pugi::xml_node&)
{
	return true;
}

bool Enemy_MiddlePig::SaveState(pugi::xml_node&)
{
	return true;
}

void Enemy_MiddlePig::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

int Enemy_MiddlePig::GetState()
{
	return 0;
}

int Enemy_MiddlePig::Attack()
{
	return 0;
}

int Enemy_MiddlePig::Ability(int id)
{
	return 0;
}

void Enemy_MiddlePig::Movement()
{



}


NodeStatus Enemy_MiddlePig::GetCloser::Run() {
	//Hacemos que actionType sea Move para que se mueva cuando entre al estado Inactive
	app->battleManager->actionType = ActionType::MOVE;

	return NodeStatus::Success;
}

NodeStatus Enemy_MiddlePig::Shoot::Run() {

	//Ataca al player
	app->battleManager->TriggerAIAttack();

	//--------
	app->battleManager->battleState = BattleState::THINKING;
	app->battleManager->UpdateTurnList();


	return NodeStatus::Success;
}

NodeStatus Enemy_MiddlePig::MoveAway::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

