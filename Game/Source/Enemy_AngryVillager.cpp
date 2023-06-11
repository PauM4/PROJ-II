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
#include "BattleManager.h"


Enemy_AngryVillager::Enemy_AngryVillager() : Entity(EntityType::ANGRYVILLAGER) {

	name.Create("enemy_angryVillager");
	namechar.Create("Villager");
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
	takedmgAnim.PushBack({150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.PushBack({ 150, 0, 140, 140 });
	takedmgAnim.loop = false;
	takedmgAnim.speed = 0.20f;
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

	texture = app->tex->Load("Assets/Characters/F_sprites_angry_Villager.png");
	IconGtexture = app->tex->Load("Assets/UI/VilagerIconG.png");
	Icontexture = app->tex->Load("Assets/UI/VilagerIcon.png");
	currentAnimation = &idleAnim;

	return true;
}

bool Enemy_AngryVillager::Start()
{
	
	return true;
}

bool Enemy_AngryVillager::Update(float dt)
{
	currentAnimation->Update();
	return true;
}

bool Enemy_AngryVillager::PostUpdate()
{
	//Render
	if (health > 0) {
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 13, position.y - 35, &rect);
	}
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
	//Hacemos que actionType sea Move para que se mueva cuando entre al estado Inactive
	app->battleManager->actionType = ActionType::MOVE;

	return NodeStatus::Success; 
}

NodeStatus Enemy_AngryVillager::Shoot::Run() {

	//Ataca al player
	app->battleManager->TriggerAIAttack();

	//--------
	app->battleManager->battleState = BattleState::THINKING;
	app->battleManager->UpdateTurnList();


	return NodeStatus::Success;
}

NodeStatus Enemy_AngryVillager::MoveAway::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

